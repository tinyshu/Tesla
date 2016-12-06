#ifndef __FILE_DB_CONNECT_H_
#define __FILE_DB_CONNECT_H_

#include <vector>
#include <map>

using std::vector;
using std::map;

#include "appcomm.h"
#include "sqlapi.h"
#include "globalconfig.h"

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


private:
  
    void createDbConnect(GlobalConfig::MysqlInfo& Conncfg);
    
    CMySQL* getPhyMysql(GlobalConfig::MysqlInfo& Conncfg);
    
private:
    /** 
	可读写的主DB连接 
	*/
    CMySQL* m_masterConnect;
 
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


};

#endif  //__FILE_DB_CONNECT_H_

