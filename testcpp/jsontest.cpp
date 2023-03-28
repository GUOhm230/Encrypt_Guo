/*
本段内容主要学习json的操作
*/

#include<iostream>
#include<fstream>
#include<assert.h>
#include<stdio.h>
#include "json.h"
#include <map>
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif
#define set_name(name) enc_aes_data##name

using namespace std;

// step1: 实现json文件的读取:从.json中读取
// void json_file_read(){
//     Json::Reader reader;
// 	Json::Value root;

// 	ifstream is;  

// 	is.open("test323/face.json", ios::binary);

// 	if (reader.parse(is, root))
// 	{
// 		Json::StyledWriter sw;     //缩进输出
// 		cout << "缩进输出" << endl;
// 		cout << sw.write(root) << endl << endl;
// 	}
// }

// 从字符串中读取json字符串

int json_read_fromStr()
{
	// string str = "{\"name\":\"shuiyixin\",\"age\":\"21\",\"sex\":\"man\"}";
    ifstream is;  

	is.open("test323/encodeInf.json", ios::binary);
    Json::Reader reader;
	Json::Value root;
    if (reader.parse(is, root))
	{
        // string age = root["en_params_aes_key"].asString();
        // cout<<age<<endl;
        // 复杂的json输出
        const Json::Value arr = root["enc_aes_data_dict"];
        cout<<arr["aaa"].asString();
        // cout<<arr.size()<<endl;
        // for(unsigned int i=0; i<arr.size(); i++){
        //     string music_det = arr[i]["music_rec_fri"].asString();
        //     cout<<music_det<<endl;
        // }
	}
    return 0;
}

int main(){
    string function = "music_det";
    string model = "hhahahahhajajkajk";
    map<string, string> enc_aes_data_dict;
    enc_aes_data_dict.insert(std::pair<string, string>(function, model));

    cout<<enc_aes_data_dict.at("music_det")<<endl; 
    return 0;
}
