#ifndef QAAC_ORD_DB_H_
#define QAAC_ORD_DB_H_

#include <string>
#include "sqlapi.h"
#include "exception.h"

/////////////////////////////////////////////////////////////////
//库表名字映射规则
const string user_db_head = "vcoin_db";
const string user_db_table_head = ".t_user_";

const string order_db_head = "vcoin_order_db_";
const string order_db_table_head = ".t_order_";

//tesla的添加在下面
//用户表
const string tesla_user_db_head = "tesl_user_db";
const string tesla_user_db_table_head = ".t_user";

const string tesla_order_db_match_head = "tesl_match_db";
const string tesla_order_match_table_head = "tesl_match_db";

/////////////////////////////////////////////////////////////////

enum DB_TYPE
{
    DB_MASTER = 1,          // 主db
    DB_SLAVE = 2,           // 备db
};

enum QCBC_DBCFG_TYPE
{
    //DBCFG_USER_BY_UIN = 1,   //根据uin获取对应的库表名称
    //DBCFG_ORDER_BY_TIME =2,  //根据订单时间获取表名
    //DBCFG_INPUT_BY_ERROR =3,  //导入时候测试使用

     DBCFG_USER_BY_NONE =1,     //单库单表不需要规则
	 DBCFG_ORDER_MATCH_BY_TIME = 2,     //匹配表年表分库分表规则
};


//封装连接，库表名字类，实现查表规则函数
class CDbInfCfg
{
public:
    
	/*
	
	*/
    CDbInfCfg(DB_TYPE eDbType,QCBC_DBCFG_TYPE eDbCfgType,const string& rule);
    
    ~CDbInfCfg()
    {}

    CMySQL* getMySql();

protected:
 
    //string getUserByUinTableName(int iUnit);

    string getMatchOrderByTime(const string& sTime);

public:
    CMySQL                  *pMySQL;
    std::string              sTableName;
    QCBC_DBCFG_TYPE          m_DBtype;
  
  
};


//#define COIN_USER_TABLE_NAME(uintid) user_db_head + user_db_table_head + uintid

//vcoin_db_yyyy.t_order_mmdd
//#define COIN_ORDER_TABLE_NAME(year,mm,dd)  order_db_head + year + order_db_table_head + mm + dd

//获取AA总单表名称(总单按年分库按月分表)
//#define QAAC_ORDER_TABLE_NAME(year, month) qaac_collect_order_db_head + "_" + year + "." + qaac_collect_order_table_head + "_" + month
//表格式qq_aa_collection.t_collection_list_yyyy
//#define QAAC_ORDER_TABLE_NAME(year) qaac_collect_order_db_head + "." + qaac_collect_order_table_head + "_" + year

//获取付款AA分单库表
//表格式 qq_aa_collection_db.t_pay_list_yyyy
//#define QAAC_PAYLIST_TABLE_NAME(year, month) qaac_pay_order_db_head + "_" + year + "." + qaac_pay_order_table_head + "_" + month
//#define QAAC_PAYLIST_TABLE_NAME(year) qaac_pay_order_db_head + "." + qaac_pay_order_table_head + "_" + year

/// 查询订单
void queryOrder(CDbInfCfg& cbcDBInfCfg, const CStrSet& setFields, const CStr2Map& keyData, CStr2Map& outputData, bool& bExists);


void queryOrder(CDbInfCfg& cbcDBInfCfg, const CStr2Map& mapFields, const CStr2Map& keyData, CStr2Map& outputData, bool& bExists);

void queryUser(CDbInfCfg& cbcDBInfCfg, const CStr2Map& mapFields, const CStr2Map& keyData, CStr2Map& outputData, bool& bExists);

/// 以锁的方式查询订单
void lockQueryOrder(CDbInfCfg& cbcDBInfCfg, const CStr2Map& mapFields, const CStr2Map& keyData, CStr2Map& outputData, bool& bExists);

void lockQueryUser(CDbInfCfg& cbcDBInfCfg, const CStr2Map& mapFields, const CStr2Map& keyData, CStr2Map& outputData, bool& bExists);
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
void internalQueryOrder(CDbInfCfg& cbcDBInfCfg, const CStrSet& setFields, const CStr2Map& keyData, CStr2Map& outputData, bool& bExists, bool bLock);

/// 落地付款单
/**
* @remark
* 1. 该函数会抛出 CException 异常
*/
void insertOrder(CDbInfCfg& cbcDBInfCfg, const CStr2Map& inputData, bool& bDupUnique);


void updateOrderByKey(CDbInfCfg& cbcDBInfCfg, const CStr2Map& updateData, const CStr2Map& keyData, const string& sConditionAdd, const string& sUpdateAdd, bool bBatch = false);


void queryAllOrder(CDbInfCfg& DBInfCfg, const CStr2Map& mapFields, const CStr2Map& keyData, const string& strOrder, int limit, int offset, vector<CStr2Map>& vecOutputData);

#endif /* QPAYMENT_DB_INF_H_ */


