#ifndef _MD5_GET
#define _MD5_GET

#include<string>
#include<iostream>
#include "decode_encode.h"
using namespace std;
#define shift(x, n) (((x) << (n)) | ((x) >> (32-(n))))
//四个非线性函数对输入进行处理计算
#define MD5_F(x, y, z) (((x) & (y)) | ((~x) & (z)))    
#define MD5_G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define MD5_H(x, y, z) ((x) ^ (y) ^ (z))
#define MD5_I(x, y, z) ((y) ^ ((x) | (~z)))
#define MD5_A 0x67452301
#define MD5_B 0xefcdab89
#define MD5_C 0x98badcfe
#define MD5_D 0x10325476

void mainLoop(int M[]);

int* add(string str);

string changeHex(int a);

string messageDigestFGet(string key1, const string randStr);

string randStrGen(const int len=32);

bool strCompare(string str1, string str2);

#endif