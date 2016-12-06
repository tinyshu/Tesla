/*
 * created by sdalinluo@20130922
 * The purpose of this file is to make declaration of CConfigFile
 * class which parses conf file.
 */
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <stdint.h>



class CConfigFile
{
public:
	CConfigFile();
	CConfigFile(const char* szFileName);
	virtual ~CConfigFile();

    int SetData(const char* pData, int iLen)
    {
        if (!pData)
        {
            m_bNull = true;
            return 0;
        }

        m_bNull = false;

        m_strData.resize(iLen+1);
        memcpy((char*)m_strData.data(), pData, iLen);
        ((char*)m_strData.data())[iLen] = '\0';

        return 0;
    }

    int size()
    {
        if (m_bNull)
        {
            return -1;
        }
        return m_strData.size() - 1;
    }

    const char* data()
    {
        if (m_bNull)
        {
            return NULL;
        }
        return m_strData.data();
    }

private:
   
	int pri_as(uint32_t & val)
	{
		if(m_bNull)
		{
			val = 0;
		}
		else
		{
			val = static_cast<uint32_t>(strtoul(m_strData.c_str(),NULL,10));
		}

		return 0;
	}



	int pri_as(uint16_t & val)
	{
		if(m_bNull)
		{
			val = 0;
		}
		else
		{
			val = static_cast<uint16_t>(strtoul(m_strData.c_str(),NULL,10));
		}

		return 0;
	}


	int pri_as(uint8_t & val)
	{
		if(m_bNull)
		{
			val = 0;
		}
		else
		{
			val = static_cast<uint8_t>(strtoul(m_strData.c_str(),NULL,10));
		}

		return 0;
	}


    int pri_as(std::string& val)
    {
        if (m_bNull)
        {
            val = "";
        }
        val = m_strData.data();
        return 0;
    }
    template <typename T> 
    int pri_as(T& val)
    {
        if (this->size() <= 0)
        {
            val = 0;
            return 0;
        }

		std::stringstream ss;
        ss << this->data();
        ss >> val;
        return 0;
    }

private:
	std::string m_strData;
    bool m_bNull;


public:
	std::string operator[](const char* szName);

	template<typename T>
	T GetConfValue(const char * szSection,const char  * szName,const char * szDefaultValue)
	{
		char szParam[64];
		snprintf(szParam,sizeof(szParam),"%s.%s",szSection,szName);

		if(m_ConfigMap[std::string(szParam)].empty())
		{
		  SetData(szDefaultValue,static_cast<int>(strlen(szDefaultValue)));
		}
		else
		{
			SetData(m_ConfigMap[std::string(szParam)].c_str(),static_cast<int>(m_ConfigMap[szParam].size()));
		}

		T asVal;
		pri_as(asVal);
		return asVal;

	}

	int ParseFile(const char* szConfigFile);

private:
	static int StrimString(char* szLine);
	int ParseFile();

private:
	std::ifstream m_ConfigFile;
	std::map<std::string, std::string> m_ConfigMap;
};

#endif

