#include <sstream>
#include <assert.h>
#include <map>
#include "globalconfig.h"
#include "appcomm.h"
#include "db_inf.h"
#include "db_connect.h"


using namespace std;

class CResAutoFree
{
public:
    CResAutoFree(MYSQL_RES* pRes)
    {
        m_pRes = pRes;
    }
    ~CResAutoFree()
    {
        if (NULL != m_pRes)
        {
            mysql_free_result(m_pRes);
            m_pRes = NULL;
        }
    }

private:
    MYSQL_RES* m_pRes;
};

CDbInfCfg::CDbInfCfg(DB_TYPE eDbType, QCBC_DBCFG_TYPE eDbCfgType,const string& rule)
{
    int iUnit = 0;
    m_DBtype = eDbCfgType;
    //获取库表名
    switch (eDbCfgType)
    {
    case DBCFG_USER_BY_NONE:
    {
							  //user库表使用的单库单表，并且在一台Db上
							  sTableName = tesla_user_db_head + tesla_user_db_table_head;
							  pMySQL =  CDBConnect::GetInstance()->getConnBy();
    }break;

    case DBCFG_ORDER_MATCH_BY_TIME:
    {
                             
                               sTableName = getMatchOrderByTime(rule);
							   pMySQL =  CDBConnect::GetInstance()->getConnBy();
                            
    }break;

    }
    
    if (pMySQL == NULL)
    {

        throw CException(ERR_UNKNOWN_EXCP, "mysql connect null", __FILE__, __LINE__);
    }

   // SPP_ERROR_LOG(GCONF->pbase, " sTableName:%s,iUnit:%d", sTableName.c_str(), iUnit);
}



CMySQL* CDbInfCfg::getMySql()
{
    if (pMySQL == NULL)
    {
       
        throw CException(ERR_UNKNOWN_EXCP, "mysql connect null", __FILE__, __LINE__);
    }

    return pMySQL;
}





//sTime YYYYMMDDHH:MM:SS

string CDbInfCfg::getMatchOrderByTime(const string& sTime)
{
	string tablename;
	/*
    string  year = sTime.substr(0, 4);
    string  mm = sTime.substr(4, 2);
    string  dd = sTime.substr(6, 2);
    string tablename = COIN_ORDER_TABLE_NAME(year, mm, dd);

    //判断当前时间,
    time_t order_end_time_1;
    time_t order_end_time_2;
    time_t order_end_time_3;

    
    std::string ymd = year + mm + dd + "075959";
    order_end_time_1 = AppComm::toUnixTime(ymd, "YYYYMMDDHHMMSS");
    
    ymd = year + mm + dd + "155959";
    order_end_time_2 = AppComm::toUnixTime(ymd, "YYYYMMDDHHMMSS");


    ymd = year + mm + dd + "235959";
    order_end_time_3 = AppComm::toUnixTime(ymd, "YYYYMMDDHHMMSS");



    //time_t ts = time(NULL);
    
    time_t ts = AppComm::toUnixTime(sTime, "YYYYMMDDHHMMSS");
    if (ts <= order_end_time_1)
    {
        tablename = tablename + "_1";
    }
    else if (ts <= order_end_time_2)
    {
        tablename = tablename + "_2";
    }
    else if (ts <= order_end_time_3)
    {
        tablename = tablename + "_3";
    }
    else
    {
        SPP_ERROR_LOG(GCONF->pbase, "getUserByTime time error");
    }

    SPP_ERROR_LOG(GCONF->pbase, "getUserByTime time:%s tablename:%s", sTime.c_str(), tablename.c_str());
	*/
    return tablename;
}

static string escapeOnePara(MYSQL* handle, const std::string& str)
{
    char * buff = NULL;
    const int iFastBufferSize = 8 * 1024 + 1;
    const int iMinBufferSize = str.size() * 2 + 1;
    char szFastBuffer[iFastBufferSize];
    if (iMinBufferSize > iFastBufferSize)
    {
        buff = new char[iMinBufferSize];
    }
    else {
        buff = szFastBuffer;
    }
    memset(buff, 0, iMinBufferSize);
    mysql_real_escape_string(handle, buff, str.c_str(), str.size());
    std::string value = buff;
    if (buff != szFastBuffer)
    {
        delete[] buff;
    }
    return value;
}

static void escapePara(MYSQL* handle, CStr2Map& inputData)
{
    assert(NULL != handle);
    CStr2Map::iterator it = inputData.begin();
    while (it != inputData.end())
    {
        it->second = escapeOnePara(handle, it->second);
        it++;
    }
}

string getSelectFieldByMap(const CStr2Map& mapFields)
{
    std::string sSelectFields;
    bool bFirstUpdateField = true;
    for (CStr2Map::const_iterator iter = mapFields.begin(); iter != mapFields.end(); ++iter)
    {
        if (bFirstUpdateField)
        {
            sSelectFields += iter->second;
            bFirstUpdateField = false;
        }
        else
        {
            sSelectFields += string(",") + iter->second;
        }
    }

    return sSelectFields;
}

string getSelectFieldBySet(const CStrSet& setFields)
{
    std::string sSelectFields;
    bool bFirstUpdateField = true;
    for (CStrSet::const_iterator iter = setFields.begin(); iter != setFields.end(); ++iter)
    {
        if (bFirstUpdateField)
        {
            sSelectFields += *iter;
            bFirstUpdateField = false;
        }
        else
        {
            sSelectFields += string(",");
            sSelectFields += *iter;
        }
    }
    return sSelectFields;
}

/**
* 根据入参，进行一次DB查询。
* 输入
* @param pMySQL 数据库连接实例
* @param sTableName 目标库表名
* @param vecFields 目标字段，将会做为SELECT子句使用。
* @param keyData 查询条件，做为WHERE子句使用。
* @param bLock 是否要锁定记录，如果true，SQL语句将会追加FOR UPDATE。但本函数 ，不负责启动、提交、回滚事务。
*
*
* 输出
* @param outputData 查询结果
* @param bExists 目标是否在DB内存在。
*
*/
void internalQueryOrder(CDbInfCfg& cbcDBInfCfg, const CStrSet& setFields, const CStr2Map& keyData, CStr2Map& outputData, bool& bExists, bool bLock)
{
    //这里没有连接成功是core 先判断是否连接
    if (NULL == cbcDBInfCfg.pMySQL->GetHandle())
    {
        //这里会抛异常,外层可以跟进情况处理
        cbcDBInfCfg.pMySQL->Connect();
    }
    // 条件字段
    CStr2Map	secureKeyData = keyData;
    ::escapePara(cbcDBInfCfg.pMySQL->GetHandle(), secureKeyData);

    bExists = false;
    string sLockPart;
    if (bLock)
    {
        sLockPart = " FOR UPDATE";
    }

    // select字段
    string sSelectFields = getSelectFieldBySet(setFields);

    string sSqlExpr =
        "SELECT " + sSelectFields
        + " FROM " + cbcDBInfCfg.sTableName;

    bool bFirstField = true;
    for (CStr2Map::const_iterator iter = secureKeyData.begin(); iter != secureKeyData.end(); ++iter)
    {
        string dbfieldname = iter->first;
        string dbfieldvalue = iter->second;

        // 只有字段存在时才进行更新
        if (bFirstField)
        {
            bFirstField = false;
            // 第一个更新的字段
            // key = 'value' ;
            sSqlExpr += string(" WHERE ") + dbfieldname + " = '" + dbfieldvalue + "' ";
        }
        else
        {
            // 非第一个更新的字段，前面要加AND，以分割前述字段
            // , key = 'value' ;
            sSqlExpr += string("AND ") + dbfieldname + " = '" + dbfieldvalue + "' ";
        }
    }

    sSqlExpr += " LIMIT 2" + sLockPart;

    cbcDBInfCfg.pMySQL->Query(sSqlExpr.c_str(), sSqlExpr.size());

    //SPP_ERROR_LOG(GCONF->pbase, "sSqlExpr:%s ", sSqlExpr.c_str());

    MYSQL_RES* pRes = cbcDBInfCfg.pMySQL->FetchResult();
    CResAutoFree resAutoFree(pRes);				//自动释放（解决抛异常释放麻烦的问题
    const int iResultRows = mysql_num_rows(pRes);
    if (iResultRows <= 0)
    {
        return;
    }
    if (iResultRows >= 2)
    {
        throw CException(ERR_DB_MULTI_ROW, "result row>=2", __FILE__, __LINE__);
    }
    bExists = true;

    // 取数据
    outputData.clear();
    const int iFieldCount = mysql_num_fields(pRes);
    MYSQL_FIELD* pFieldDef = mysql_fetch_fields(pRes);
    mysql_data_seek(pRes, 0);
    MYSQL_ROW rowData = mysql_fetch_row(pRes);
    for (int i = 0; i < iFieldCount; i++)
    {
        const char* szDbFieldName = pFieldDef[i].name;
        // 原始名按理不能使用
        //const char* szDbFieldOrgName = pFieldDef[i].org_name;
        string sFieldValue;
        if (NULL != rowData[i])
        {
            sFieldValue = rowData[i];
        }
        if (NULL != szDbFieldName)
        {
            outputData[szDbFieldName] = sFieldValue;
        }
    }
}

/// 查询订单
void queryOrder(CDbInfCfg& cbcDBInfCfg, const CStrSet& setFields, const CStr2Map& keyData, CStr2Map& outputData, bool& bExists)
{
    internalQueryOrder(cbcDBInfCfg, setFields, keyData, outputData, bExists, false);
}

/// 查询订单
void queryOrder(CDbInfCfg& cbcDBInfCfg, const CStr2Map& mapFields, const CStr2Map& keyData, CStr2Map& outputData, bool& bExists)
{
    CStrSet setFields;
    //get map key assign to set
    AppComm::getKeySetOfMap(mapFields, setFields);
    internalQueryOrder(cbcDBInfCfg, setFields, keyData, outputData, bExists, false);
}

void queryUser(CDbInfCfg& cbcDBInfCfg, const CStr2Map& mapFields, const CStr2Map& keyData, CStr2Map& outputData, bool& bExists)
{
    CStrSet setFields;
    AppComm::getKeySetOfMap(mapFields, setFields);
    internalQueryOrder(cbcDBInfCfg, setFields, keyData, outputData, bExists, false);
}

void lockQueryUser(CDbInfCfg& cbcDBInfCfg, const CStr2Map& mapFields, const CStr2Map& keyData, CStr2Map& outputData, bool& bExists)
{
    CStrSet setFields;
    AppComm::getKeySetOfMap(mapFields, setFields);
    internalQueryOrder(cbcDBInfCfg, setFields, keyData, outputData, bExists, true);
}
/**
* @remark 相比queryOrder，该函数会使用SELECT * FOR UPDATE
*/
void lockQueryOrder(CDbInfCfg& cbcDBInfCfg, const CStr2Map& mapFields, const CStr2Map& keyData, CStr2Map& outputData, bool& bExists)
{
    CStrSet setFields;
    AppComm::getKeySetOfMap(mapFields, setFields);
    internalQueryOrder(cbcDBInfCfg, setFields, keyData, outputData, bExists, true);
}

void updateOrderByKey(CDbInfCfg& cbcDBInfCfg, const CStr2Map& updateData, const CStr2Map& keyData, const string& sConditionAdd, const string& sUpdateAdd, bool bBatch)
{
    // 条件字段
    if (keyData.empty())
    {
        throw CException(ERR_BAD_PARAM, "keyData empty", __FILE__, __LINE__);
    }

    //这里没有连接成功是core 先判断是否连接
    if (NULL == cbcDBInfCfg.pMySQL->GetHandle())
    {
        //这里会抛异常,外层可以跟进情况处理
        cbcDBInfCfg.pMySQL->Connect();
    }

    CStr2Map	secureKeyData = keyData;
    ::escapePara(cbcDBInfCfg.pMySQL->GetHandle(), secureKeyData);

    // 更新字段
    CStr2Map	secureInputData = updateData;
    // 转义
    ::escapePara(cbcDBInfCfg.pMySQL->GetHandle(), secureInputData);

    string sSqlExpr = "UPDATE " + cbcDBInfCfg.sTableName + " SET ";

    bool bFirstField = true;
    for (CStr2Map::const_iterator iter = secureInputData.begin(); iter != secureInputData.end(); ++iter)
    {
        string dbfieldname = iter->first;
        string dbfieldvalue = iter->second;

        // 只有字段存在时才进行更新
        if (bFirstField)
        {
            bFirstField = false;
            // 第一个更新的字段
            // key = 'value' ;
            sSqlExpr += dbfieldname + " = '" + dbfieldvalue + "' ";
        }
        else
        {
            // 非第一个更新的字段，前面要加逗号(,)，以分割前述字段
            // , key = 'value' ;
            sSqlExpr += string(", ") + dbfieldname + " = '" + dbfieldvalue + "' ";
        }
    }

    // 追加自定义更新字段
    if (sUpdateAdd.length() > 0)
    {
        if (bFirstField)
        {
            bFirstField = false;
            // 第一个更新的字段
            // key = 'value' ;
            sSqlExpr += sUpdateAdd + " ";
        }
        else
        {
            // 非第一个更新的字段，前面要加逗号(,)，以分割前述字段
            // , key = 'value' ;
            sSqlExpr += string(", ") + sUpdateAdd + " ";
        }
    }

    // 更新where条件
    bool bFirstWhere = true;
    for (CStr2Map::const_iterator iter = secureKeyData.begin(); iter != secureKeyData.end(); ++iter)
    {
        string dbfieldname = iter->first;
        string dbfieldvalue = iter->second;

        // 只有字段存在时才进行更新
        if (bFirstWhere)
        {
            bFirstWhere = false;
            // 第一个更新的字段
            // key = 'value' ;
            sSqlExpr += string(" WHERE ") + dbfieldname + " = '" + dbfieldvalue + "' ";
        }
        else
        {
            // 非第一个更新的字段，前面要加AND，以分割前述字段
            // , key = 'value' ;
            sSqlExpr += string("AND ") + dbfieldname + " = '" + dbfieldvalue + "' ";
        }
    }

    if (sConditionAdd.length() > 0)
    {
        if (bFirstWhere)
        {
            bFirstWhere = false;
            // 第一个更新的字段
            // 追加自定义条件
            sSqlExpr += string(" WHERE ") + sConditionAdd + " ";
        }
        else
        {
            // 追加自定义条件
            sSqlExpr += string("AND ") + sConditionAdd + " ";
        }
    }

   
    if (false == bBatch)
    {
        sSqlExpr += " LIMIT 2";
        cbcDBInfCfg.pMySQL->Query(sSqlExpr.c_str(), sSqlExpr.size());

        //SPP_ERROR_LOG(GCONF->pbase, "sSqlExpr:%s ", sSqlExpr.c_str());
        int iAffectRows = cbcDBInfCfg.pMySQL->AffectedRows();
        if (iAffectRows != 1)
        {
            //ERROR("affect_row=%d", iAffectRows);
            throw CException(ERR_DB_AFFECT_ROW, "affect row != 1", __FILE__, __LINE__);
           
        }
    }
    else
    {
        cbcDBInfCfg.pMySQL->Query(sSqlExpr.c_str(), sSqlExpr.size());
    }
}

void insertOrder(CDbInfCfg& cbcDBInfCfg, const CStr2Map& inputData, bool& bDupUnique)
{
    bDupUnique = false; // 标记是否主键冲突

    CStr2Map	secureInputData = inputData;
    //这里没有连接成功是core 先判断是否连接
    if (NULL == cbcDBInfCfg.pMySQL->GetHandle())
    {
        //这里会抛异常,外层可以跟进情况处理
        cbcDBInfCfg.pMySQL->Connect();
    }
    ::escapePara(cbcDBInfCfg.pMySQL->GetHandle(), secureInputData);

    string sSqlExpr = "INSERT INTO " + cbcDBInfCfg.sTableName + " SET ";

    bool bFirstField = true;
    for (CStr2Map::const_iterator iter = secureInputData.begin(); iter != secureInputData.end(); ++iter)
    {
        string dbfieldname = iter->first;
        string dbfieldvalue = iter->second;

        // 只有字段存在时才进行更新
        if (bFirstField)
        {
            bFirstField = false;
            // 第一个更新的字段
            // key = 'value' ;
            sSqlExpr += dbfieldname + " = '" + dbfieldvalue + "' ";
        }
        else
        {
            // 非第一个更新的字段，前面要加逗号(,)，以分割前述字段
            // , key = 'value' ;
            sSqlExpr += string(", ") + dbfieldname + " = '" + dbfieldvalue + "' ";
        }
    }

    try
    {
        cbcDBInfCfg.pMySQL->Query(sSqlExpr.c_str(), sSqlExpr.size());
    }
    catch (CException& e)
    {
        if (e.error() == ERR_DB_DUP_UNIQUE)
        {
            // 唯一键冲突，不报错
            //WARNING("unique key duplicate:%d,%s\n", e.error(), e.what());
            bDupUnique = true;
            return;
        }
        throw e;
    }
    catch (...)
    {
        CException ex(ERR_DB_UNKNOW, "系统异常", __FILE__, __LINE__);
        //ERROR("Failed to insertOrder:%d,%s\n", ex.error(), ex.what());
        throw ex;
    }

    //ERR_DB_DUP_UNIQUE
}


/**
* 根据入参，进行一次DB查询。
* 输入
* @param pMySQL 数据库连接实例
* @param sTableName 目标库表名
* @param vecFields 目标字段，将会做为SELECT子句使用。
* @param keyData 查询条件，做为WHERE子句使用。
* @param bLock 是否要锁定记录，如果true，SQL语句将会追加FOR UPDATE。但本函数 ，不负责启动、提交、回滚事务。
*
*
* 输出
* @param outputData 查询结果
* @param bExists 目标是否在DB内存在。
*
*/
static void internalQueryAllOrder(CDbInfCfg& cbcDBInfCfg, const CStr2Map& mapFields, const CStr2Map& keyData, const string& strOrder, int limit, int offset, vector<CStr2Map>& vecOutputData)
{
    // 条件字段
    CStr2Map	secureKeyData = keyData;
    //这里没有连接成功是core 先判断是否连接
    if (NULL == cbcDBInfCfg.pMySQL->GetHandle())
    {
        //这里会抛异常,外层可以跟进情况处理
        cbcDBInfCfg.pMySQL->Connect();
    }

    ::escapePara(cbcDBInfCfg.pMySQL->GetHandle(), secureKeyData);

    // select字段
    string sSelectFields = getSelectFieldByMap(mapFields);

    string sSqlExpr =
        "SELECT " + sSelectFields
        + " FROM " + cbcDBInfCfg.sTableName;

    bool bFirstField = true;
    for (CStr2Map::const_iterator iter = secureKeyData.begin(); iter != secureKeyData.end(); ++iter)
    {
        string dbfieldname = iter->first;
        string dbfieldvalue = iter->second;

        // 只有字段存在时才进行更新
        if (bFirstField)
        {
            bFirstField = false;
            // 第一个更新的字段
            // key = 'value' ;
            sSqlExpr += string(" WHERE ") + dbfieldname + " = '" + dbfieldvalue + "' ";
        }
        else
        {
            // 非第一个更新的字段，前面要加AND，以分割前述字段
            // , key = 'value' ;
            sSqlExpr += string("AND ") + dbfieldname + " = '" + dbfieldvalue + "' ";
        }
    }

    if (!strOrder.empty())
    {
        sSqlExpr += strOrder;
    }

    sSqlExpr += string(" limit ") + AppComm::tostr(limit) + string(" offset ") + AppComm::tostr(offset) + string(" ");

    cbcDBInfCfg.pMySQL->Query(sSqlExpr.c_str(), sSqlExpr.size());
    MYSQL_RES* pRes = cbcDBInfCfg.pMySQL->FetchResult();
    CResAutoFree resAutoFree(pRes);				//自动释放（解决抛异常释放麻烦的问题
    const int iResultRows = mysql_num_rows(pRes);
    if (iResultRows <= 0)
    {
        return;
    }

    // 取数据
    for (int i = 0; i < iResultRows; i++)
    {
        CStr2Map outputData;
        const int iFieldCount = mysql_num_fields(pRes);
        MYSQL_FIELD* pFieldDef = mysql_fetch_fields(pRes);
        mysql_data_seek(pRes, i);
        MYSQL_ROW rowData = mysql_fetch_row(pRes);
        for (int i = 0; i < iFieldCount; i++)
        {
            const char* szDbFieldName = pFieldDef[i].name;
            // 原始名按理不能使用
            //const char* szDbFieldOrgName = pFieldDef[i].org_name;
            string sFieldValue;
            if (NULL != rowData[i])
            {
                sFieldValue = rowData[i];
            }
            if (NULL != szDbFieldName)
            {
                outputData[szDbFieldName] = sFieldValue;
            }
        }

        if (!outputData.empty())
        {
            vecOutputData.push_back(outputData);
        }
        
    }
}


/// 查询所有付款单
void queryAllOrder(CDbInfCfg& DBInfCfg, const CStr2Map& mapFields, const CStr2Map& keyData, const string& strOrder, int limit, int offset,vector<CStr2Map>& vecOutputData)
{
    internalQueryAllOrder(DBInfCfg, mapFields, keyData, strOrder, limit, offset, vecOutputData);
}
