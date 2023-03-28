#include <vector>
#include <string>
#include <iostream>

using namespace std;

int main(){

    vector<int> v = {0, 1};
    v = vector<int>(v.rbegin(), v.rend());
    for(int elm: v){
        cout<<elm<<endl;
    }
    return 0;
}