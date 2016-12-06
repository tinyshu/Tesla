#include "CDBUser.h"
#include <stdlib.h>

static const std::string& getString(const CStr2Map& inputData, const char* szFieldName)
{
    static const std::string sEmptyVal;
    CStr2Map::const_iterator iter = inputData.find(szFieldName);
    if (iter != inputData.end())
    {
        return iter->second;
    }
    return sEmptyVal;
}

static const int getInt(const CStr2Map& inputData, const char* szFieldName)
{
    const int iZeroVal = 0;
    CStr2Map::const_iterator iter = inputData.find(szFieldName);
    if (iter != inputData.end())
    {
        return atoi(iter->second.c_str());
    }
    return iZeroVal;
}

static const LONG getLong(const CStr2Map& inputData, const char* szFieldName)
{
    const LONG lZeroVal = 0;
    CStr2Map::const_iterator iter = inputData.find(szFieldName);
    if (iter != inputData.end())
    {
        return strtoll(iter->second.c_str(), NULL, 10);
    }
    return lZeroVal;
}

void CUser::copy(const CUser& order)
{
	m_FID = order.m_FID;
	m_FName = order.m_FName;
   
}

CUser::CUser(const CUser& order)
{
    copy(order);
}

CUser& CUser::operator = (const CUser& order)
{
    copy(order);
    return *this;
}


/// 从数据库中导出的CStr2Map中提取数据
void CUser::retriveFromStr2Map(const CStr2Map& inputData)
{
    m_FID = getString(inputData, USERDB_FID);
    m_FName = getString(inputData, USERDB_FNAME);
}

void CUser::addToStr2Map(CStr2Map& outputData)
{
    outputData[USERDB_FID] = m_FID;
    outputData[USERDB_FNAME] = m_FName;
  
}

/// 获取所有字段
void CUser::getAllFieldName(CStr2Map& mapFieldName)
{
    mapFieldName[USERDB_FID] = USERDB_FID;
    mapFieldName[USERDB_FNAME] = USERDB_FNAME;
}









