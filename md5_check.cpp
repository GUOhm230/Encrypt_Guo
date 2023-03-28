#include "md5_check.h"
//strBaye的长度
int strlength;
//A,B,C,D的临时变量
int atemp;
int btemp;
int ctemp;
int dtemp;
//常量ti unsigned int(abs(sin(i+1))*(2pow32))
unsigned const int k[]={
        0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
        0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,0x698098d8,
        0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,
        0xa679438e,0x49b40821,0xf61e2562,0xc040b340,0x265e5a51,
        0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
        0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,
        0xfcefa3f8,0x676f02d9,0x8d2a4c8a,0xfffa3942,0x8771f681,
        0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,
        0xbebfbc70,0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
        0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,0xf4292244,
        0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,
        0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,
        0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391};
//向左位移数
unsigned const int s[]={7,12,17,22,7,12,17,22,7,12,17,22,7,
        12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
        4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,
        15,21,6,10,15,21,6,10,15,21,6,10,15,21};
char const str16[]="0123456789abcdef";
void mainLoop(int M[])
{
    int f,g;
    int a=atemp;
    int b=btemp;
    int c=ctemp;
    int d=dtemp;
    for (int i = 0; i < 64; i++)
    {
        if(i<16){
            f=MD5_F(b,c,d);
            g=i;
        }else if (i<32)
        {
            f=MD5_G(b,c,d);
            g=(5*i+1)%16;
        }else if(i<48){
            f=MD5_H(b,c,d);
            g=(3*i+5)%16;
        }else{
            f=MD5_I(b,c,d);
            g=(7*i)%16;
        }
        int tmp=d;
        d=c;
        c=b;
        b=b+shift((a+f+k[i]+M[g]),s[i]);
        a=tmp;
    }
    atemp=a+atemp;
    btemp=b+btemp;
    ctemp=c+ctemp;
    dtemp=d+dtemp;
}

int* add(string str)
{
    int num=((str.length()+8)/64)+1;
    int *strByte=new int[num*16];    
    strlength=num*16;
    for (int i = 0; i < num*16; i++)
        strByte[i]=0;
    for (int i=0; i <str.length(); i++)
    {
        strByte[i>>2]|=(str[i])<<((i%4)*8);
    }
    strByte[str.length()>>2]|=0x80<<(((str.length()%4))*8);
    strByte[num*16-2]=str.length()*8;
    return strByte;
}

string changeHex(int a)
{
    int b;
    string str1;
    string str="";
    for(int i=0;i<4;i++)
    {
        str1="";
        b=((a>>i*8)%(1<<8))&0xff;   //逆序处理每个字节
        for (int j = 0; j < 2; j++)
        {
            str1.insert(0,1,str16[b%16]);
            b=b/16;
        }
        str+=str1;
    }
    return str;
}

// MD5散列值获取
string messageDigestFGet(string key1, const string randStr)
{
    atemp=MD5_A;    //初始化
    btemp=MD5_B;
    ctemp=MD5_C;
    dtemp=MD5_D;
    const string source = key1.append(randStr);
    int *strByte=add(source);
    for(int i=0;i<strlength/16;i++)
    {
        int num[16];
        for(int j=0;j<16;j++)
            num[j]=strByte[i*16+j];
        mainLoop(num);
    }
    return changeHex(atemp).append(changeHex(btemp)).append(changeHex(ctemp)).append(changeHex(dtemp));
}

// 随机字符串获取
string randStrGen(const int len)
{
    string str;                 
    char c;                     
    int idx;                    
    int temp;
    for(idx = 0;idx < len;idx++){
        temp = rand()%3;
        switch(temp){
            case 0:{
                c = '0' + rand()%10;
                str.push_back(c);
                break;
            }
            case 1:{
                c = 'a' + rand()%26;
                str.push_back(c);
                break;
            }
            case 2:{
                c = 'A' + rand()%26;
                str.push_back(c);
                break;
            }
        }
    }
    return str;                 
}

//字符串比较
bool strCompare(string str1, string str2){
    int temp = str1.compare(str2);
    bool flag;
    temp == 0?flag = true:flag = false;
    return flag;
}

// int main()
// {
//     string ss;
//     string randomStr;
//     string key1 = "hkjhkjkj";
//     randomStr = randStrGen(32);         /*调用函数 输入字符串长度*/
//     // cout << key1.append(randomStr) << endl;  
//     const string s=messageDigestFGet(key1, randomStr);
//     std::cout<<s<<std::endl;
//     // cout<<s<<"\t"<<s.size()<<endl;
//     return 0;
// }
