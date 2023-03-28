#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <map>

using namespace std;

string GeneratepubKey(string function);

int public_encrypt(unsigned char *data, int data_len, unsigned char *key, unsigned char *encrypted);

int private_decrypt(unsigned char *enc_data, int data_len, unsigned char *key,
                    unsigned char *decrypted);

void clearMap(map<string, string> &privateMap);
