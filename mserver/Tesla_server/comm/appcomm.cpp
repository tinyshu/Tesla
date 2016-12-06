#include "appcomm.h"
#include <sys/types.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

namespace AppComm
{ 

    void splitStr(const string &s, std::vector<string> &v_str, const char* sep)
	{
		if (s.empty())
		{
			return;
		}

		unsigned index = 0;
		std::string::size_type pos = 0;

		do
		{
			pos = s.find_first_of(sep, index);
			if (std::string::npos != pos)
			{
				if (pos > 0)
				{
					v_str.push_back(s.substr(index, pos - index));
				}
				index = pos + 1;
			}
			else
			{
				v_str.push_back(s.substr(index));
				break;
			}
		} while (index < s.size());

	}

	void splitStr(const std::string &s, std::set<std::string> &v_str, const char* sep)
	{
		if (s.empty())
		{
			return;
		}

		unsigned index = 0;
		std::string::size_type pos = 0;
		do
		{
			pos = s.find_first_of(sep, index);
			if (std::string::npos != pos)
			{
				if (pos > 0)
				{
					v_str.insert(s.substr(index, pos - index));
				}
				index = pos + 1;
			}
			else
			{
				v_str.insert(s.substr(index));
				break;
			}
		} while (index < s.size());
	}

	void MapToStr(const std::map<std::string, std::string >& inMap, std::string& Str, string sEleSep, string sNvSep)
    {
        Str.clear();
        std::map<std::string,std::string >::const_iterator iter = inMap.begin();
        std::map<std::string, std::string >::const_iterator iter_end = inMap.end();

        if (inMap.size() == 0)
        {
            return;
        }

        Str += iter->first + sNvSep + iter->second;

        iter++;
        while (iter != iter_end)
        {
            Str += sEleSep + iter->first + sNvSep + iter->second;

            iter++;
        }
        
    }

    int String2VectorOneValue(const std::string & sSrc,const std::string & sSep,std::vector<std::string> & vStr)
    {
        vStr.clear();

        std::string::size_type iSepLen = sSep.size();
        std::string::size_type iStart = 0;

        std::string::size_type iTmp = sSrc.find(sSep);
        if (iTmp != std::string::npos)
        {
            vStr.push_back(sSrc.substr(iStart, iTmp - iStart));
            iStart = iTmp + iSepLen;
        }

        vStr.push_back(sSrc.substr(iStart));

        return vStr.size();
    }

    int String2Vector(const std::string & sSrc,const std::string & sSep,std::vector<std::string> & vStr
        )
    {
        vStr.clear();

        std::string::size_type iSepLen = sSep.size();
        std::string::size_type iStart = 0;

        std::string::size_type iTmp;
        for (iTmp = sSrc.find(sSep);
            iTmp != std::string::npos;
            iTmp = sSrc.find(sSep, iStart))
        {
            vStr.push_back(sSrc.substr(iStart, iTmp - iStart));
            iStart = iTmp + iSepLen;
        }
        vStr.push_back(sSrc.substr(iStart));

        return vStr.size();
    }


    bool StrToVector(std::vector<std::string> & strVec, const string & Str, const string& sEleSep)
    {
        //原实现有问题，修改成String2Vector
        return String2Vector(Str, sEleSep, strVec);
    }


    bool StrToMap(std::map<std::string, std::string > & outMap, const string & Str, string sEleSep, string sNvSep, bool emptyFlag)
    {
        std::vector<std::string> vEle, vNv;
        StrToVector(vEle, Str, sEleSep);

        for (unsigned int i = 0; i < vEle.size(); i++)
        {
            String2VectorOneValue(vEle[i], sNvSep, vNv);
            if (vNv.size() == 1 && emptyFlag)
            {
                outMap[vNv[0]] = "";
            }
            else if (vNv.size() == 2)
            {
                outMap[vNv[0]] = vNv[1];
            }
        }
        if (outMap.size() == 0)
        {
            return false;
        }
        return true;
    }

    time_t toUnixTime(const string& strTime, const string& strFormat)
    {
        // 取年、月、日段
        int year(0), month(0), day(0), hour(0), minute(0), second(0);

        if (0 == strFormat.compare("YYYY-MM-DD HH:MM:SS"))
        {
            sscanf(strTime.c_str(), "%04d-%02d-%02d %02d:%02d:%02d", &year, &month, &day, &hour, &minute, &second);
        }
        else if (0 == strFormat.compare("YYYYMMDDHHMMSS")) {
            sscanf(strTime.c_str(), "%04d%02d%02d%02d%02d%02d", &year, &month, &day, &hour, &minute, &second);
        }

        // 若日期小于1900，返回0
        if (year < 1900)
        {
            return 0;
        }

        // 转换为当地时间
        struct  tm tm_date;
        memset(&tm_date, 0, sizeof(tm));

        tm_date.tm_year = year - 1900;
        tm_date.tm_mon = month - 1;
        tm_date.tm_mday = day;
        tm_date.tm_hour = hour;
        tm_date.tm_min = minute;
        tm_date.tm_sec = second;

        // 转换为系统时间
        return  mktime(&tm_date);
    }


    /**
    * 获取系统时间: YYYY-MM-DD HH:MM:SS
    */
    string getSysTime()
    {
        return getSysTime(time(NULL));
    }

    string getSysTime(time_t t, const string& timeFormat)
    {
        struct tm tm_now;
        localtime_r(&t, &tm_now);
        char szTmp[256] = { 0 };

        if (strcmp(timeFormat.c_str(), "YYYY-MM-DD HH:MM:SS") == 0)
        {
            snprintf(szTmp, sizeof(szTmp)-1, "%04d-%02d-%02d %02d:%02d:%02d",
                tm_now.tm_year + 1900, tm_now.tm_mon + 1, tm_now.tm_mday,
                tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec);

        }
        else if (strcmp(timeFormat.c_str(), "YYYYMMDDHHMMSS") == 0)
        {
            snprintf(szTmp, sizeof(szTmp)-1, "%04d%02d%02d%02d%02d%02d",
                tm_now.tm_year + 1900, tm_now.tm_mon + 1, tm_now.tm_mday,
                tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec);

        }
        else if (strcmp(timeFormat.c_str(), "YYYYMMDD") == 0)
        {
            snprintf(szTmp, sizeof(szTmp)-1, "%04d%02d%02d", tm_now.tm_year + 1900, tm_now.tm_mon + 1, tm_now.tm_mday);
        }

        return szTmp;
    }

    void getKeySetOfMap(const CStr2Map& mapFields, CStrSet& setFields)
    {
        if (mapFields.size() <= 0) return;

        for (CStr2Map::const_iterator it = mapFields.begin(); it != mapFields.end(); ++it)
        {
            setFields.insert(it->first);
        }
    }

    int year(const string& str)
    {
        int year, month, day;
        sscanf(str.c_str(), "%04d-%02d-%02d", &year, &month, &day);
        return year;
    }

    unsigned int SDBMHash(const char *str)
    {
        unsigned int hash = 0;

        while (*str)
        {
            // equivalent to: hash = 65599*hash + (*str++);
            hash = (*str++) + (hash << 6) + (hash << 16) - hash;
        }

        return (hash & 0x7FFFFFFF);
    }

    unsigned int murMurHash2(const void *key, int len)
    {
        const unsigned int m = 0x5bd1e995;
        const int r = 24;
        const int seed = 97;
        unsigned int h = seed ^ len;
        // Mix 4 bytes at a time into the hash
        const unsigned char *data = (const unsigned char *)key;
        while (len >= 4)
        {
            unsigned int k = *(unsigned int *)data;
            k *= m;
            k ^= k >> r;
            k *= m;
            h *= m;
            h ^= k;
            data += 4;
            len -= 4;
        }
        // Handle the last few bytes of the input array
        switch (len)
        {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
            h *= m;
        };
        // Do a few final mixes of the hash to ensure the last few
        // bytes are well-incorporated.
        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;
        return h;
    }

    int getUintByUin(std::string& sKey)
    {
        //skey  是vuid格式 vuid457457065
        /*std::string sid;
        if ("vuid" == sKey.substr(0, 4))
        {
            sid = sKey.substr(4, sKey.length() - 4);
        }
        else
        {
            //guid直接走固定路由
            return GCONF->serverconf.GuidTable;

        }

        int ukey = atoll(sid.c_str());
        int UintID = (ukey / segment) + 1;
        
        return UintID;
		*/
		return 0;
    }



    long getCurrentTime()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }

	const char* getLocalIp()
    {
        int s;
        struct ifconf conf;
        struct ifreq *ifr;
        char buff[BUFSIZ];
        int num;
        int i;

        s = socket(PF_INET, SOCK_DGRAM, 0);
        conf.ifc_len = BUFSIZ;
        conf.ifc_buf = buff;

        ioctl(s, SIOCGIFCONF, &conf);
        num = conf.ifc_len / sizeof(struct ifreq);
        ifr = conf.ifc_req;

        for (i = 0; i < num; i++)
        {
            struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);

            ioctl(s, SIOCGIFFLAGS, ifr);
            if (((ifr->ifr_flags & IFF_LOOPBACK) == 0) && (ifr->ifr_flags & IFF_UP))
            {
                /* printf("%s (%s)\n",
                     ifr->ifr_name,
                     inet_ntoa(sin->sin_addr));*/

                return inet_ntoa(sin->sin_addr);
            }
            ifr++;
        }
		
		return NULL;
    }
    

    string MakeMsgNo()
    {
        //生成全局唯一的msgno
        static const char* pLocalIp = NULL;
        if (pLocalIp == NULL)
        {
            pLocalIp = AppComm::getLocalIp();
        }
        
        
        if (pLocalIp == NULL)
        {
           // throw CTrsExp(ERR_GET_SERVER_ADDR, "System Error 02 ,Please Contact WebMaster");
            return "";
        }

        in_addr_t lIp = inet_addr(pLocalIp);
        time_t t = time(NULL);
        pid_t iPid = getpid();
        static int index = 0;
        //280是系统标志
        static char cMsgNo[32] = { 0 };
        snprintf(cMsgNo, sizeof(cMsgNo)-1, "280%08x%09u%05u%04d", lIp, static_cast<unsigned int>(t) / 10, iPid, index % 10000);
        index++;
        string  g_sMsgNo = cMsgNo;
        return g_sMsgNo;
    }

};
