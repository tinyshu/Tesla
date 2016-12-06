#include <stdio.h>
#include<unistd.h>
#include "srpcincl.h"
#include "globalconfig.h"

extern "C"
{
struct routeid {
	uint32_t ip;
	uint16_t port;
	int type;
};

int32_t getroutebyname(const char *name, struct routeid *);

};
GlobalConfig* GlobalConfig::m_pInstance = NULL;

GlobalConfig::GlobalConfig()
{

}

GlobalConfig::~GlobalConfig()
{}


bool GlobalConfig::InitConfig(const char* szFileName)
{
    if (NULL == szFileName  || -1 == access(szFileName, 0) )
	{
	  return false;
	}
	
    CConfigFile obj_conf(szFileName);

    //读取DB配置

    struct routeid r;
    if (0 != getroutebyname("Database.mysql", &r))
    {
	    NGLOG_ERROR("getroutebyname() failed!\n");
	    return false;
    }
    struct in_addr addr;
    addr.s_addr = r.ip;
    char * ip = inet_ntoa(addr);
    if (ip == NULL)
    {
	    NGLOG_ERROR("mysql server ip null:%s\n", ip);
	    return false;
    }
    //char host[100] = "1.2.3.4";
    //strcpy(host, ip);
    DBConnectInfo.mysql_vip = ip;
    DBConnectInfo.mysql_port = r.port; 
    DBConnectInfo.mysql_user = obj_conf.GetConfValue<std::string>("dbinfo", "mysql_user", "");
    DBConnectInfo.mysql_pswd = obj_conf.GetConfValue<std::string>("dbinfo", "mysql_pswd", "");
    DBConnectInfo.mysql_overtime = obj_conf.GetConfValue<int32_t>("dbinfo", "mysql_overtime", "1");
    DBConnectInfo.mysql_rwovertime = obj_conf.GetConfValue<int32_t>("dbinfo", "mysql_rwovertime", "1");
	
	NGLOG_INFO("mysql server ip:%s port:%d user %s\n", DBConnectInfo.mysql_vip.c_str(),DBConnectInfo.mysql_port, DBConnectInfo.mysql_user.c_str());
	//读取系统配置
	
	return true;
}

