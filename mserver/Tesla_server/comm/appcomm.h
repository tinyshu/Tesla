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
	
	//��mapƴ�ӳ� key1=value&key2=value��ʽ
    void MapToStr(const std::map<std::string, std::string >& inMap, std::string& Str, std::string sEleSep = "&", std::string sNvSep = "=");
	

    int String2Vector(
        const std::string & sSrc,
        const std::string & sSep,
        std::vector<std::string> & vStr);

    /**
    * ���ַ�������־���ֿ����ŵ�vector, ֻ��key=valueһ��ֵ
    * ����String2Vector, ����ִ������� key=value===, value����=������
    *@param sSrc   	 	��Ҫ��ֵ��ַ���
    *@param sSep		��ֱ�־��
    *@param vStr 		��Ž����vector
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
    
    //��ø������˻�
    std::string getAddAccount();

    std::string getSubAccount();

    //��õ�ǰʱ��(����)
    long getCurrentTime();
	
	const char* getLocalIp();

    string MakeMsgNo();
};

#endif