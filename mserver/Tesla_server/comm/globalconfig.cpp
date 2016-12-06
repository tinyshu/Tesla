#include <stdio.h>
#include<unistd.h>
#include "srpcincl.h"
#include "globalconfig.h"

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
    
	DBConnectInfo.mysql_vip = obj_conf.GetConfValue<std::string>("dbinfo", "mysql_vip", "");
    DBConnectInfo.mysql_port = obj_conf.GetConfValue<int32_t>("dbinfo", "mysql_port", "");
    DBConnectInfo.mysql_user = obj_conf.GetConfValue<std::string>("dbinfo", "mysql_user", "");
    DBConnectInfo.mysql_pswd = obj_conf.GetConfValue<std::string>("dbinfo", "mysql_pswd", "");
    DBConnectInfo.mysql_overtime = obj_conf.GetConfValue<int32_t>("dbinfo", "mysql_overtime", "1");
    DBConnectInfo.mysql_rwovertime = obj_conf.GetConfValue<int32_t>("dbinfo", "mysql_rwovertime", "1");
	
	NGLOG_INFO("mysql server ip:%s port:%d\n", DBConnectInfo.mysql_vip.c_str(),DBConnectInfo.mysql_port);
	//读取系统配置
	
	return true;
}

