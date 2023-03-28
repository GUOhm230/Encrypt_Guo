#include "rsa_ed.h"

#define KEY_LENGTH  2048

int padding = RSA_PKCS1_PADDING;

std::map<string, string> privateKey;

//前端请求，算法生成公钥
string GeneratepubKey(string function)
{

	size_t pri_len = 0;
	size_t pub_len = 0;
	char* pri_key = nullptr;
	char* pub_key = nullptr;
	string pub_key_back;

	RSA* keypair = RSA_generate_key(KEY_LENGTH, RSA_3, NULL, NULL);

	BIO* pri = BIO_new(BIO_s_mem());
	BIO* pub = BIO_new(BIO_s_mem());


	PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);

	PEM_write_bio_RSA_PUBKEY(pub, keypair);


	pri_len = BIO_pending(pri);
	pub_len = BIO_pending(pub);
  
	pri_key = (char*)malloc(pri_len + 1);
	pub_key = (char*)malloc(pub_len + 1);

	BIO_read(pri, pri_key, pri_len);
	BIO_read(pub, pub_key, pub_len);

	pri_key[pri_len] = '\0';
	pub_key[pub_len] = '\0';


	RSA_free(keypair);
	BIO_free_all(pub);
	BIO_free_all(pri);

	pub_key_back = pub_key;

	privateKey.insert(std::pair<string, string>(function, pri_key));
	ofstream pri_key_file;
	pri_key_file.open("test323/pri_key.txt", std::ios::out | std::ios::app);
    pri_key_file<<pri_key<<endl;
    pri_key_file.close();
	cout<<"++++++++++++++++++++++++私钥++++++++++++++++++++++++++"<<endl;
    cout<<pri_key<<endl;
	cout<<"++++++++++++++++++++++++私钥++++++++++++++++++++++++++"<<endl;
	free(pri_key);
	free(pub_key);
	return pub_key_back;

}


RSA* createRSA(unsigned char* key, int flag)
{
	RSA* rsa = NULL;
	BIO* keybio;
	keybio = BIO_new_mem_buf(key, -1);

	if (keybio == NULL) {
		printf("Failed to create key BIO");
		return 0;
	}

	if (flag)
		rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	else
		rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);

	if (rsa == NULL)
		printf("Failed to create RSA");

	return rsa;
}

int public_encrypt(unsigned char* data, int data_len, unsigned char* key, unsigned char* encrypted)
{
	RSA* rsa = createRSA(key, 1);
	int result = RSA_public_encrypt(data_len, data, encrypted, rsa, RSA_PKCS1_OAEP_PADDING);
	RSA_free(rsa);
	CRYPTO_cleanup_all_ex_data();
	return result;
}

int private_decrypt(unsigned char* enc_data, int data_len, unsigned char* key, unsigned char* decrypted)
{
	RSA* rsa = createRSA(key, 0);
	int  result = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, RSA_PKCS1_OAEP_PADDING);
	RSA_free(rsa);
	CRYPTO_cleanup_all_ex_data();
	return result;
}

void clearMap(std::map<string, string> &privateMap){
	if(!privateMap.empty()) privateMap.clear();
}