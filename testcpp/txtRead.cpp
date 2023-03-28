#include <fstream>
#include <string>
#include <iostream>
using namespace std;
int main(){
    ifstream fin("test323/pri_key.txt",ios::in);
    fin.seekg(0, ios_base::end);
    int model_lSize = fin.tellg();
    fin.seekg(0, std::ios::beg);
    char* buffer = (char*)malloc(sizeof(char) * model_lSize);
    fin.read((char*)buffer, sizeof(char) * model_lSize);
    fin.close();
    string pri_key = buffer;
    // cout<<pri_key<<endl;
    ofstream pri_key_write;
    pri_key_write.open("test323/pri_key_saveagain.txt", std::ios::out | std::ios::app);
    pri_key_write<<pri_key<<endl;
    pri_key_write.close();
    return 0;
}