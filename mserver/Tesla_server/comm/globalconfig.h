#ifndef _GLOBAL_CONFIG_H_
#define _GLOBAL_CONFIG_H_

#include <string>
#include <vector>
#include "config.h"

/**
* 存放全局配置的类
*/
class GlobalConfig
{
public:

   bool InitConfig(const char* szFileName);

public:

    enum UNIT_STATUS
    {
        UNIT_READ_ONLY = 1,         //只读  
        UNIT_WRITE_ONLY = 2,        //读写
        UNIT_READ_WRITE = 3,        //可读写
        UNIT_DILATACNCY =4,         //扩容
    };

    GlobalConfig();
    ~GlobalConfig();

    typedef struct tagServerConf
    {
        int isCanCoverWrite;  //是否可以覆盖写
        int mysql_count;
        int UintCount; //
        int setid;     //setid
        int acc_modid;  //名字服务l5
        int acc_cmdid;  
        int GuidTable; //  //GUID放在一个固定的table
        int cvuid_timeout; //转vuid超时

        int acc_write_modid;
        int acc_write_cmdid;

        int Package_timeOut; 
		
		int db_check_time;  //db连接检测间隔
        int db_max_conn;    //最大连接数
		int db_active_time;
        

    }ServerConf;



    typedef struct tagMysqlInfo
    {
        int  dbid;
        std::string mysql_vip;
        int mysql_port;
        std::string mysql_user;
        std::string mysql_pswd;
		std::string mysql_name;
        int  mysql_overtime;
        int  mysql_rwovertime;
       
    }MysqlInfo;



	// 获取实例
	static GlobalConfig* GetInstance()
	{
		if (m_pInstance == NULL)
		{
			m_pInstance = new GlobalConfig();
		}

		return m_pInstance;
	}


    //

    void setMsgID(std::string& msgno)
    {
        Msgseq = msgno;
    }

    const std::string& getMsgID()
    {
        return Msgseq;
    }


    void setMsgName(std::string& name)
    {
        msgname = name;
    }

    const std::string getMsgName()
    {
        return msgname;
    }
public:
    
    ServerConf  m_serverconf;
   
    //std::vector<MysqlInfo> DBConnectInfo;
    
	
	MysqlInfo DBConnectInfo;
    // 实例
	static GlobalConfig* m_pInstance;

   

    std::string  Msgseq;

    std::string  msgname;
};


#define GCONF GlobalConfig::GetInstance()

#endif