step1:算法模型model拆分成两个部分：model_p1(提交前端)以及model_p2（大致5kb提交后端传给授权服务器），
      授权服务器会给模型对应的salt_key。
      
      涉及代码：
      file_split.cpp中的file_string（）用于模型拆分：输入整个model文件，输出两个拆分后的model文件

step2:前端向算法请求获得一对公私钥。公钥进行base64加密得到base64_pu；
      该公钥+“&"+salt生成MD5。
      将加密后的公钥base64_pu以及MD5返回给前端。
      前端传送给授权服务器并认证之。

        1）生成公私钥：rsa_ed.cpp---GeneratepubKey();
           返回string类型：pub_key_back
           myprivatekey
        2）公钥加密并生成MD5：encode_decode.cpp----_getPubkey()
           生成.json文件，返回给前端

        注意：本部分，由前端直接调用_getPubkey(function)
        获取的返回值中即是加密后的base64_pu以及MD5值：public_key_sign
        包括后面需要的myprivatekey
        其中：string function需要一一对应，否则出错。对应关系如下：
        
        曲谱页面框选： "music_page_frame"
        曲谱正反判断： "updown_pruned_v2"
        曲谱页面检测： "music_det"
        曲谱中文识别： "music_rec_ch"
        曲谱外文识别： "music_rec_fri"


step3:授权服务器认证通过之后，服务器生成aes密钥，用以加密model_p2以及md5_key和authorization_id；
      并用base_64再次加密生成enc_aes_data。
      用step2中生成的公钥加密aes密钥并用base_64再次加密获得en_params_aes_key。
      以上在授权服务器完成，再由前端传给算法端。

      1）授权服务器传给前端的是：
      encodedInfo
      2）算法接收并处理的代码
      encode_decode.cpp-----decode_authorization();


step4:en_params_aes_key进行base64解密。用step2中的私钥进行解密，获得aes密钥
      1）授权服务器传过来的json文件处理：encode_decode.cpp-----decode_authorization();
         1.获取en_params_aes_key的base64解密结果key_decoded
            string key_decoded = base64_decode(encode_info_json["en_params_aes_key"].asString());
         2. 应用step2中的私钥进行解密：	
            int key_length = private_decrypt((unsigned char*)key_decoded.c_str(), key_decoded.length(), (unsigned char*)myprivatekey.c_str(), aes_keys);
             获取结果：aes_keys---user_key---decrypt_key
         3. 再次进行base64解密获得待拼接的字符串my_privatemodel_p2：
         my_privatemodel_p2 = base64_decode(decode_result["params"].asString());
        
        前端接收授权服务器的json文件后，直接调用decode_authorization()。
        获取my_privatemodel_p2后

step5:enc_aes_data进行base_64解密。并用解密后的aes密钥解密后再用base64解密获得model_p2.
      解密后的model_p2与前端传进的model_p1进行组合，生成新的buffer.传入算法运行。

      1）encode_decode.cpp---decode_authorization()对model_p2进行解密获取my_privatemodel_p2。之后用于合并
      2）encode_decode.cpp---modelCombine()模型合并。得到buffer.
      3）base64_ed.cpp-------base64_decode()base64对enc_aes_data和en_params_aes_key解密
    
      直接调用 modelCombine(),输入给到前端的5k数据即可生成创建解释器需要的buffer


第三流程：
step6:http联网验证
      1）step5步解密还能获取authorization_id以及md5_key
         算法生成随机字符串random_str + "&" + md5_key生成sign_data
         通过网络链接将sign_data和random_str发送给授权服务器
      2）授权服务器通过网络返回授权结果：random_str2, authorization_result（0 or 1）
         以及random_str2 + &1& + md5_key计算MD5值所得sign_data2：his_sign_data
         获得授权服务器数据后，用1）中的random_str + &1& + md5_key
         两个比较,通过后才能通过验证：
         1. random_str2与上次授权获得的random_str2进行比较，不同则满足2才能通过验证。相同则不通过
         2. 


------------------------------------------------------------------------
调试案例
------------------------------------------------------------------------

1.将大模型拆分成两个部分：updown_pruned_v2.mnn---->updown_pruned_v2(提交给授权服务器) + updown_pruned_v2_submit2frontEnd
  授权服务器返回salt_key: |1e2180fa-8017-11ed-a726-0242ac110005|

2.将上面salt_key放到encode_decode.cpp 放到开头的string XXX_salt_key中
  然后运行该文件中的_getPubkey()
  每运行一次，获得当前模型的：pub_key.txt提交给前端。
  pri_key.txt留待后用。直接复制的话会出现字符编码错误

3.授权服务器返回一个json格式的字符串
    {"enc_aes_data": "字符内容", "en_params_aes_key": "相关字符"}";

4.将上面的字符串encodedInfo 传入decode_authorization()中并运行之
  获取my_privatemodel_p2即完整模型头部的5kb字符串
  模型拼接modelCombine()生成完整的模型
  然后进行模型测试
  成功

  终端运行命令：
  g++ encode_decode.cpp messageDigestFive.cpp jsoncpp.cpp  base64_ed.cpp rsa_ed.cpp commonSense.cpp `pkg-config --cflags --libs opencv4` -L/usr/local/lib -lssl -lcrypto -ldl -lpthread -o u
