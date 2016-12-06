#include "db_connect.h"
#include "globalconfig.h"

CDBConnect* CDBConnect::m_pInstance = NULL;

const char* MASTER_ROLE   =   "1";
const char* SLAVE_ROLE    =   "2";

CDBConnect::CDBConnect()
{
}

CDBConnect::~CDBConnect()
{}

void CDBConnect::InitDB(GlobalConfig::MysqlInfo dbinfo)
{
  createDbConnect(dbinfo);
}

/*
获取ID连接
暂时没有分库分表，所有数据放一台机器上，所以这里直接返回连接
*/
CMySQL* CDBConnect::getConnBy()
{
   return m_masterConnect;
}



void CDBConnect::createDbConnect(GlobalConfig::MysqlInfo& Conncfg)
{
    getPhyMysql(Conncfg);
}

CMySQL* CDBConnect::getPhyMysql(GlobalConfig::MysqlInfo& Conncfg)
{
   m_masterConnect = new CMySQL(Conncfg.mysql_vip.c_str(), Conncfg.mysql_user.c_str(), Conncfg.mysql_pswd.c_str(), Conncfg.mysql_port, Conncfg.mysql_overtime, Conncfg.mysql_rwovertime);
    
	return m_masterConnect;
}

bool CDBConnect::SLessPhy::operator()(const GlobalConfig::MysqlInfo& left, const GlobalConfig::MysqlInfo& right)
{
#define JUADGE_OPERATOR(iRet) \
    if(iRet < 0 )\
    {\
        return true;\
    }\
    else if(iRet > 0 )\
    {\
        return false;\
    }
    
    JUADGE_OPERATOR(left.dbid - right.dbid);
    return false;
}
