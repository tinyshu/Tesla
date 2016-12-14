#ifndef _GLOBAL_CONFIG_H_
#define _GLOBAL_CONFIG_H_

#include <string>
#include <vector>
#include "config.h"

/**
* ���ȫ�����õ���
*/
class GlobalConfig
{
public:

   bool InitConfig(const char* szFileName);

public:

    enum UNIT_STATUS
    {
        UNIT_READ_ONLY = 1,         //ֻ��  
        UNIT_WRITE_ONLY = 2,        //��д
        UNIT_READ_WRITE = 3,        //�ɶ�д
        UNIT_DILATACNCY =4,         //����
    };

    GlobalConfig();
    ~GlobalConfig();

    typedef struct tagServerConf
    {
        int isCanCoverWrite;  //�Ƿ���Ը���д
        int mysql_count;
        int UintCount; //
        int setid;     //setid
        int acc_modid;  //���ַ���l5
        int acc_cmdid;  
        int GuidTable; //  //GUID����һ���̶���table
        int cvuid_timeout; //תvuid��ʱ

        int acc_write_modid;
        int acc_write_cmdid;

        int Package_timeOut; 
		
		int db_check_time;  //db���Ӽ����
        int db_max_conn;    //���������
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



	// ��ȡʵ��
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
    // ʵ��
	static GlobalConfig* m_pInstance;

   

    std::string  Msgseq;

    std::string  msgname;
};


#define GCONF GlobalConfig::GetInstance()

#endif