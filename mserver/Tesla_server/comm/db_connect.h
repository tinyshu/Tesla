#ifndef __FILE_DB_CONNECT_H_
#define __FILE_DB_CONNECT_H_

#include <vector>
#include <map>

using std::vector;
using std::map;

#include "appcomm.h"
#include "sqlapi.h"
#include "globalconfig.h"
#include <list>

using namespace std;
class CDBConnect
{
public:

   
public:
    CDBConnect();
    ~CDBConnect();

    // 初始化
    void InitDB(GlobalConfig::MysqlInfo dbinfo);
    
    //通过deploy获得连接
  
    CMySQL* getConnBy();

    // 获取实例
    static CDBConnect* GetInstance()
    {
        if(m_pInstance == NULL)
        {
            m_pInstance = new CDBConnect();
        }

        return m_pInstance;
    }    

	  //释放一个DB连接
    void FreeSqlConn(CMySQL *Conn);
    
    //定时检查连接关联
    void CheckSqlConn();
	
private:
  
    void createDbConnect(GlobalConfig::MysqlInfo& Conncfg);
    
    CMySQL* getPhyMysql(GlobalConfig::MysqlInfo& Conncfg);
    
	


 private:
    /** 
	可读写的主DB连接 
	*/
    //CMySQL* m_masterConnect;
 
    // 读写超时(秒)
    int m_iRWOvertime;
    
    // 实例
    static CDBConnect* m_pInstance;

    struct SLessPhy
    {
        bool operator() (const GlobalConfig::MysqlInfo& left, const GlobalConfig::MysqlInfo& right);
    };


    typedef  map<GlobalConfig::MysqlInfo, CMySQL*, SLessPhy> tphyConnect;
    
    tphyConnect  m_mapPhyConnect;
    
	//保存db连接配置信息
	GlobalConfig::MysqlInfo m_dbconf;
	//连接池保存当前可用连接
	list<CMySQL*> m_dbpool;
	//当前是有创建的连接(pool+微线程在使用的和)
	int conns_count;

};

#endif  //__FILE_DB_CONNECT_H_

