#include "db_connect.h"
#include "globalconfig.h"

CDBConnect* CDBConnect::m_pInstance = NULL;

const char* MASTER_ROLE   =   "1";
const char* SLAVE_ROLE    =   "2";

CDBConnect::CDBConnect()
{
	conns_count = 0;
}

CDBConnect::~CDBConnect()
{}

void CDBConnect::InitDB(GlobalConfig::MysqlInfo dbinfo)
{
  //createDbConnect(dbinfo);
   m_dbconf = dbinfo;
}

CMySQL* CDBConnect::getConnBy()
{
   CMySQL* mysql = NULL;
   
   //先判断pool里是否有可用连接
   if(m_dbpool.empty())
   {
	    //没有可用连接创建一个,就创建一个新连接
	    //判断是否错过连接最大值
	    if (conns_count > GCONF->serverconf.db_max_conn)
		{
			//log
			return mysql;
		}
		
		mysql = getPhyMysql(m_dbconf);
		//atomic_inc(&conns_count);
		conns_count++;
   }
   else
   {
	    mysql = *(m_dbpool.begin());
		m_dbpool.pop_front();
   }
   
   	if (NULL == mysql)
	{
			//log 
	}
	else
	{
		mysql->setActiveTime(time(NULL));
	}
	
   return mysql;
}

void CDBConnect::FreeSqlConn(CMySQL *Conn)
{
	if (NULL == Conn)
	{
		//log
		return;
	}
	
	//如果读写超时了连接会被close,Handle会被设置为NULL,这里不放回队列，直接释放资源
    if (NULL == Conn->GetHandle())
    {
        delete Conn;
		//atomic_dec(&conns_count);
		conns_count--;
        return;
    }
	
	m_dbpool.push_front(Conn);
}

void CDBConnect::CheckSqlConn()
{
	//是否已经过了检测时间
    static long long  checktime = time(NULL);
    long long curTime = time(NULL);
    bool exist = false;

    if (curTime - checktime <= GCONF->serverconf.db_check_time)
    {
        return;
    }
	
	list<CMySQL*>::iterator bit = m_dbpool.begin();
	list<CMySQL*>::iterator eit = m_dbpool.end();
	for (;bit!=eit;)
	{
		 CMySQL* dbconn = *bit;
         if (NULL == dbconn)
         {
                bit = m_dbpool.erase(bit);
                continue;
         }
		 
		 int timeabs = abs(int(curTime - dbconn->getActiveTime()));
		 if (timeabs > GCONF->serverconf.db_active_time)
		 {
			 //atomic_dec(&conns_count);
			 conns_count--;
			 if (conns_count < 0)
             {
                conns_count = 0;
             }
			 
			 //关闭db连接
             dbconn->Close();
                
             //释放内存
             delete dbconn;
			 
			 bit = m_dbpool.erase(bit);
			 
			 continue;
		 }
		 else
		 {
			 ++bit;
		 }
	}
}

void CDBConnect::createDbConnect(GlobalConfig::MysqlInfo& Conncfg)
{
    getPhyMysql(Conncfg);
}

CMySQL* CDBConnect::getPhyMysql(GlobalConfig::MysqlInfo& Conncfg)
{
    CMySQL* pdb_conn = new CMySQL(Conncfg.mysql_vip.c_str(), Conncfg.mysql_user.c_str(), Conncfg.mysql_pswd.c_str(), Conncfg.mysql_port, Conncfg.mysql_overtime, Conncfg.mysql_rwovertime);
    
	return pdb_conn;
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
