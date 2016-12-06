#ifndef __APP_COMM_H__
#define __APP_COMM_H__

#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <cstdio>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "globalconfig.h"
#include "typedef.h"

using namespace std;

namespace AppComm
{ 


    template<typename T>
    string tostr(const T &t)
    {
        ostringstream sBuffer;
        sBuffer << t;
        return sBuffer.str();
    }

	void splitStr(const string &s, std::vector<string> &v_str, const char* sep);

	void splitStr(const std::string &s, std::set<std::string> &v_str, const char* sep);
	
	//把map拼接成 key1=value&key2=value格式
    void MapToStr(const std::map<std::string, std::string >& inMap, std::string& Str, std::string sEleSep = "&", std::string sNvSep = "=");
	

    int String2Vector(
        const std::string & sSrc,
        const std::string & sSep,
        std::vector<std::string> & vStr);

    /**
    * 把字符串按标志符分开后存放到vector, 只有key=value一个值
    * 区别String2Vector, 解决现存代码存在 key=value===, value中有=的问题
    *@param sSrc   	 	需要拆分的字符串
    *@param sSep		拆分标志符
    *@param vStr 		存放结果的vector
    */
    int String2VectorOneValue(const std::string& sStr, const std::string& sSep, std::vector<std::string>& vStr);

    bool StrToMap(std::map<std::string, std::string >& outMap, const std::string& Str,
        std::string sEleSep = "&", std::string sNvSep = "=", bool emptyFlag = false);

    time_t toUnixTime(const string& strTime, const string& timeFormat = "YYYY-MM-DD HH:MM:SS");

    void getKeySetOfMap(const CStr2Map& mapFields, CStrSet& setFields);

    int getUintByUin(std::string& sKey);

    unsigned int SDBMHash(const char *str);

    unsigned int murMurHash2(const void *key, int len);
    
    string getSysTime(time_t t, const string& timeFormat = "YYYY-MM-DD HH:MM:SS");
    
    string getSysTime();
    
    int year(const string& str);
    
    //获得更新子账户
    std::string getAddAccount();

    std::string getSubAccount();

    //获得当前时间(毫秒)
    long getCurrentTime();
	
	const char* getLocalIp();

    string MakeMsgNo();
};

#endif