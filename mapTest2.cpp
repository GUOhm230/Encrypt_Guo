#include "mapTest.h"

extern map<string, string> mymap;

void mapOutput(std::map<string, string> map1){
    if(!mymap.empty()){
        for(auto elm: mymap){
            cout<<elm.first<<"\t"<<elm.second<<endl;
        }
    }
}


int main(){
    string function = "music_rec_ch";
    string privateKey  = "mawangchi";

    string function1 = "music_rec_fri";
    string privateKey1  = "wangzhisheng";

    int a = functionTest(function, privateKey);
    int b = functionTest(function1, privateKey1);
    mapOutput(mymap);
    cout<<mymap.size()<<endl;
    clearMap(mymap);
    // mymap.clear();
    cout<<mymap.size()<<endl;
    return 0;
}