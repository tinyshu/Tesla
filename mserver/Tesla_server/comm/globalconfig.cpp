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

    DBConnectInfo.mysql_user = obj_conf.GetConfValue<std::string>("dbinfo", "mysql_user", "");
	DBConnectInfo.mysql_name = obj_conf.GetConfValue<std::string>("dbinfo", "mysql_name", "");
    DBConnectInfo.mysql_pswd = obj_conf.GetConfValue<std::string>("dbinfo", "mysql_pswd", "");
    DBConnectInfo.mysql_overtime = obj_conf.GetConfValue<int32_t>("dbinfo", "mysql_overtime", "1");
    DBConnectInfo.mysql_rwovertime = obj_conf.GetConfValue<int32_t>("dbinfo", "mysql_rwovertime", "1");

	struct routeid r;
	if (0 != getroutebyname(DBConnectInfo.mysql_name.c_str(), &r))
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

	DBConnectInfo.mysql_vip = ip;
	DBConnectInfo.mysql_port = r.port; 
	NGLOG_INFO("mysql server ip:%s port:%d user %s\n", DBConnectInfo.mysql_vip.c_str(),DBConnectInfo.mysql_port, DBConnectInfo.mysql_user.c_str());
	//读取系统配置
	m_serverconf.db_max_conn = obj_conf.GetConfValue<int32_t>("ServerConf", "db_max_conn", "300");
	m_serverconf.db_active_time = obj_conf.GetConfValue<int32_t>("ServerConf", "db_active_time", "10");
	m_serverconf.db_check_time = obj_conf.GetConfValue<int32_t>("ServerConf", "db_check_time", "30");
	NGLOG_INFO("svrconf db_max_conn %d db_active_time %d  db_check_time %d",m_serverconf.db_max_conn,m_serverconf.db_active_time,m_serverconf.db_check_time);
	return true;
}

