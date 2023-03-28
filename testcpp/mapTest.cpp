# include "mapTest.h"
std::map<string, string> mymap;

int functionTest(string function, string privateKey){

    mymap.insert(std::pair<string, string>(function, privateKey));
    // cout<<mymap.at(function)<<endl;
    return 0;
}

void clearMap(std::map<string, string> &mymap){
    if(!mymap.empty()){
        cout<<"清除"<<endl;
        mymap.clear();
    }
}