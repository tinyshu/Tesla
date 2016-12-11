/**
  * FileName: sqlapi.h 
  * Author: Created by tinyshu 2016-11-9

  */
#ifndef _SQL_API_H_
#define _SQL_API_H_

#include "mysql.h"
#include "mysqld_error.h"
#include "errmsg.h" 
#include "exception.h"
#include "appcomm.h"


#define SYS_ERROR(var)  19841##var

const int ERR_STD_EXCP = SYS_ERROR(0001);
const int ERR_UNKNOWN_EXCP = SYS_ERROR(0002);
const int ERR_XYZ_OPEN = SYS_ERROR(0003);
const int ERR_XYZ_RECV = SYS_ERROR(0004);
const int ERR_XYZ_SOCKET = SYS_ERROR(0005);
const int ERR_LOAD_XML_CFG = SYS_ERROR(0006);
const int ERR_FAIL_CALL_MIDDLE = SYS_ERROR(0007);
const int ERR_NULL_XML_NODE = SYS_ERROR(0008);
const int ERR_XYZ_UNKNOWN = SYS_ERROR(0009);
const int ERR_CREATE_TRANS_SESSION = SYS_ERROR(0010);
const int ERR_BAD_PARAM = SYS_ERROR(0011);      //参数错误

/**
* 数据库操作错误
*/
const int ERR_DB_INITIAL = SYS_ERROR(0301);			// 分配数据库连接失败
const int ERR_DB_CONNECT = SYS_ERROR(0302);			// 连接数据库失败
const int ERR_DB_LOST = SYS_ERROR(0303);			// 和数据库断开连接
const int ERR_DB_BEGIN = SYS_ERROR(0304);			// 开始事务失败
const int ERR_DB_COMMIT = SYS_ERROR(0305);			// 提交事务失败
const int ERR_DB_ROLLBACK = SYS_ERROR(0306);			// 回滚事务失败
const int ERR_DB_NULL_RESULT = SYS_ERROR(0307);			// 获取到空数据库结果集
const int ERR_DB_AFFECT_ROW = SYS_ERROR(0308);			// 影响行数不符合
const int ERR_DB_UNKNOW = SYS_ERROR(0309);			// 意外错误
const int ERR_DB_AUTO_CMMIT = SYS_ERROR(0310);			// 设置自动提交模式失败
const int ERR_DB_NONE_ROW = SYS_ERROR(0311);			// 没有取到任何结果
const int ERR_DB_MULTI_ROW = SYS_ERROR(0312);			// 取到结果多余一行
const int ERR_DB_FETCH_ROW = SYS_ERROR(0314);			// 取结果行错误
const int ERR_DB_ROW_LENGTH = SYS_ERROR(0315);			// 结果行数过长
const int ERR_DB_SQL_NOVALUE = SYS_ERROR(0316);			// 拼接SQL语句错误,没有值
const int ERR_DB_DUP_UNIQUE = SYS_ERROR(0317);			// 唯一键值冲突，不可写入
const int ERR_DB_DB_TBL_NOTEXIST = SYS_ERROR(0318);          // 数据库或表不存在
const int ERR_DB_OVERLOAD = SYS_ERROR(0319);          // 数据库访问过载

/**
  * 本类对MySQL的接口进行了简单封装，主要包括如下功能:
  *     1、内部保存了连接信息
  *     2、数据库自动重连功能
  *     3、非SELECT语句后获取结果集抛出异常
  *     4、获取SQL影响行数出错时抛出异常
  */
class CMySQL
{
public:
    CMySQL(const char* host, const char* user, const char* pswd, int iPort, int iOverTime, int iRWOverTime);
    ~CMySQL();
    operator MYSQL*() { return m_SqlHandle; }
    MYSQL* GetHandle() { return m_SqlHandle; }

    /**
     * 数据库连接
     */
    void Connect() throw(CException);
    void Close();
    bool Ping(); 

    /**
     * 执行Query
     */
    void  Query(const char *query, unsigned int length) throw(CException);
    unsigned long AffectedRows() throw(CException);
    MYSQL_RES* FetchResult() throw(CException);
    MYSQL_RES* UseResult() throw(CException);

    /**
     * 事务处理语句
     */
    void Begin() throw(CException);
    void Commit() throw(CException);
    void Rollback() throw(CException);

    /**
     * 设置DB访问的平均耗时， 单位为毫秒 
     */
    void SetAvgConsume(int consume){ m_avgConsume = consume * 1000; };
    
    static int GetDBAndTableMd5(const char* szStr, int& iDbNum, int &iTbNum);

    static int GetDBAndTable(const char* szStr, int& iDbNum, int &iTbNum);
    
	long getActiveTime()
	{
		return m_activetime;
	}
	
	void setActiveTime(long activetime)
	{
		m_activetime = activetime;
	}
private:
    // 禁止拷贝和赋值操作
    CMySQL(const CMySQL& ref);
    CMySQL& operator=(const CMySQL& ref);
        
protected:    
    
    std::string m_strHost; // 登录主机IP
    std::string m_strUser; // 登录ID
    std::string m_strPasswd; // 登录密码
    MYSQL*    m_SqlHandle; // SQL操作句柄
    bool      m_bInTransaction;  // 当前SQL是否处于事务中
    unsigned int m_iPort; // 连接端口
    unsigned int m_iOverTime;    // 连接超时时长
	unsigned int m_iRWOverTime;    // 读写超时时长
    int         m_avgConsume;   // 平均访问时间, 单位是微秒
    
	long  m_activetime;  //分配的时间 
};

#endif

