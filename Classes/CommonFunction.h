#ifndef __GameTest__CommonFunction__
#define __GameTest__CommonFunction__

#include <iostream>
#include <sstream>

using namespace std;

namespace CommonFunction
{
    // 模板不支持分离式编译
    template <typename T>
    string num2str(const T &num)
    {
        stringstream ss;
        string str;
        ss << num;
        if(!ss.good()){
            cout<<"error CommonFunction::num2str"<<endl;
        }
        ss >> str;
        
        return str;
    }
    
    template <typename T>
    T str2num(const string &str)
    {
        T num;
        stringstream ss;
        if(!ss.good()){
            cout<<"error CommonFunction::str2num"<<endl;
        }
        ss<<str;
        ss>>num;
        
        return num;
    }
    
    template <typename OT,typename IT>
    OT convert(const IT &src)
    {
        OT dst;
        stringstream ss;
        ss<<src;
        if(!ss.good()){
            cout<<"error CommonFunction::convert"<<endl;
        }
        ss>>dst;
        
        return dst;
    }
    
    int getRandNum(int from, int to);
}

#endif
