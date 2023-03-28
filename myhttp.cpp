#include <iostream>
#include <string>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "messageDigestFive.h"
#include "commonSense.h"

using namespace std;

struct MemoryStruct
{
    char* memory;
    size_t size;
    MemoryStruct()
    {
        memory = (char*)malloc(1);
        size = 0;
    }
    ~MemoryStruct()
    {
        free(memory);
        memory = NULL;
        size = 0;
    }
};

string rand_str_check;

size_t WriteMemoryCallback(void* ptr, size_t size, size_t nmemb, void* data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)data;

    mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory)
    {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;


//http链接，发送及验证
// input:授权服务器发送的enc_aes_data内含：model_p2，md5_key，authorization_id.
bool httpcheck(string authorization_id, string md5_key)
{
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if (CURLE_OK != res)
    {
        cout << "curl init failed" << endl;
        return false;
    }
    CURL* pCurl;
    pCurl = curl_easy_init();
    if (NULL == pCurl)
    {
        cout << "Init CURL failed..." << endl;
        return false;
    }
    
    //string url = "http://127.0.0.1:8888/";
    string url = "https://algorithm-authorization.leqiai.cn/app/client/" + authorization_id + "/authorization";    

    curl_easy_setopt(pCurl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(pCurl, CURLOPT_DEFAULT_PROTOCOL, "https");
    struct curl_slist* headers = NULL;
    string tmp_authorization_id = "authorization_id: " + authorization_id;
    headers = curl_slist_append(headers, "User-Agent: Apifox/1.0.0 (https://www.apifox.cn)");
    headers = curl_slist_append(headers, tmp_authorization_id.c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headers);

    //将返回结果通过回调函数写到自定义的对象中
    MemoryStruct oDataChunk;
    curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &oDataChunk);
    curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L); //启用时会汇报所有的信息

    //post表单参数.获取sign_data并需要发送给授权服务器：random_str和sign_data
    string random_str = randStrGen(32);
    string ranand = random_str + "&";
    string sign_data = messageDigestFGet(ranand, md5_key);
    string http_post_str = "{\"random_str\":\""  + random_str + "\",\"sign_data\":\""  + sign_data + "\" }";


    //libcur的相关POST配置项
    curl_easy_setopt(pCurl, CURLOPT_POST, 1L);
    curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, http_post_str.c_str());
    curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, http_post_str.size());
    res = curl_easy_perform(pCurl);
    long res_code = 0;
    res = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &res_code);
    if ((res == CURLE_OK) && (res_code == 200 || res_code == 201))
    {
        string response_result = oDataChunk.memory;
        Json::Value response_json = getJson(response_result);
        // 授权服务器返回random_str2
        random_str = response_json["random_str"].asString();

        if (rand_str_check.length() == 0)//authorization_result为0，未通过验证
            rand_str_check = random_str;
        else {
            if (rand_str_check == random_str) {
                cout << "random str are same for twice!!!" << endl;
                return false;
            }
            else
                rand_str_check = random_str;
        }
        rand_str_check.shrink_to_fit();

        // 授权服务器生成的sign_data2
        string his_sign_data = response_json["sign_data"].asString();
        ranand = random_str + "&1&";
        // 算法端生成的sign_data3
        sign_data = messageDigestFGet(ranand, md5_key);

        //cout << "his_sign_data: " << his_sign_data << endl;
        //cout << " my_sign_data: " << sign_data << endl;
        if (strCompare(sign_data, his_sign_data)) {
            //cout << "check successed!!!" << endl;
            return true;
        }
        else {
            //cout << "check Failed!!!" << endl;
            return false;
        }        
    }
    curl_easy_cleanup(pCurl);
    curl_global_cleanup();
    return false;
}

// int main() {
//    string authorization_id = "111";       //第二步解密后得到
//    string md5_key = "ASfdfsdfsdasASDf";   //第二步解密后得到
//    bool check3 = httpcheck(authorization_id, md5_key);
//    cout << "check3: " << check3 << endl;
// }