#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlapi.h"



/**
 * Description: 构造函数
 * Input:         host         数据库主机IP地址
 *                   user         数据库用户ID
 *                   passwd     数据库用户密码
 *                   iPort      连接端口
 *                   iOverTime 连接超时时长
 */
CMySQL::CMySQL(const char* host, const char* user, const char* passwd, int iPort, int iOverTime, int iRWOverTime)
                               : m_strHost(host), m_strUser(user), m_strPasswd(passwd), m_iPort(iPort), m_iOverTime(iOverTime), m_iRWOverTime(iRWOverTime)
{
    m_SqlHandle = NULL;
    m_bInTransaction = false;
    m_avgConsume    =   0;
}

/**
 * Description: 构造函数
 * Input:         host         数据库主机IP地址
 *                   user         数据库用户ID
 *                   passwd     数据库用户密码
 *                   iOverTime 连接超时时长
 */
CMySQL::~CMySQL()
{
    Close();
}

/**
 * Description: 连接数据库
 * Return:       出错抛出异常
 */
void  CMySQL::Connect() throw(CException)
{
    // 初始化连接句柄
    if(m_SqlHandle == NULL)
    {
        m_SqlHandle =(MYSQL *)malloc(sizeof(MYSQL));
        m_bInTransaction = false;
    }

    // 初始化
    mysql_init(m_SqlHandle);
    
    // 设置超时选项
    mysql_options(m_SqlHandle, MYSQL_OPT_CONNECT_TIMEOUT,  (char *)&m_iOverTime);  // 连接超时时长
    mysql_options(m_SqlHandle, MYSQL_OPT_READ_TIMEOUT,  (char *)&m_iRWOverTime);   // 读写超时时长
    mysql_options(m_SqlHandle, MYSQL_OPT_WRITE_TIMEOUT,  (char *)&m_iRWOverTime);  // 读写超时时长
    mysql_options(m_SqlHandle, MYSQL_SET_CHARSET_NAME,  "utf8");   // 字符集

    // 建立连接
    if (!mysql_real_connect(m_SqlHandle, m_strHost.c_str(), m_strUser.c_str(), m_strPasswd.c_str(), NULL, m_iPort, NULL, 0))
    {
        string strError = string("Failed to connetct db :") + string(mysql_error(m_SqlHandle));

        Close();
        
        throw CException(ERR_DB_CONNECT, strError.c_str(), __FILE__, __LINE__);
    }
	
}

/**
 * Description: 关闭数据库
 */
void  CMySQL::Close()
{
    if (m_SqlHandle) 
    { 
        mysql_close(m_SqlHandle);
        free(m_SqlHandle); 
        m_SqlHandle = NULL;
        m_bInTransaction = false;   
    }
}

/**
 * Description: 检测数据库状态
 * Input:         mysql     SQL操作句柄
 */
bool CMySQL::Ping()
{
    return mysql_ping(m_SqlHandle) == 0;
}

/**
 * Description: 执行SELECT类型的SQL语句
 * Input:         query     SQL语句
 *                   length     SQL语句的长度
 * Return:       失败抛出异常
 */
void CMySQL::Query(const char *query, unsigned int length) throw(CException)
{
   struct timeval start, end;

    // 若处于断连状态，则进行重连
    if (!m_SqlHandle)
    {
        Connect();
    }

    // 获取调用起始时间
    //gettimeofday(&start, NULL);   

    // 执行查询
    int iRet = mysql_real_query(m_SqlHandle, query, length);

    // 若SQL处于非事务当中，重做一次
    if(iRet != 0 && !m_bInTransaction)
    {
        // 重连一次
        Ping();

        // 重新执行SQL
        iRet = mysql_real_query(m_SqlHandle, query, length);
    }

    // 获取调用结束时间
    //gettimeofday(&end, NULL);   

 

    //// 记录日志
    //gPtrAppLog->debug("|sql|%s|%s|%s|%s|3306|%ldus|%d|%s|", 
    //                                SRC_IP, HOST_IP, SERVICE, m_strHost.c_str(), 
    //                                (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec), 
    //                                iRet, query);
    
    if (iRet != 0)
    {
        // 保存错误信息
    	int iMySQLErrNo = mysql_errno(m_SqlHandle);
        string strError = mysql_error(m_SqlHandle);
                
        // 判断是否为数据库断连
        if (mysql_errno(m_SqlHandle) == CR_SERVER_LOST || mysql_errno(m_SqlHandle) == CR_SERVER_GONE_ERROR)
        {
            Close();// 关闭连接
            
            throw CException(ERR_DB_LOST, strError.c_str(), __FILE__, __LINE__);
        }
        else if (mysql_errno(m_SqlHandle) == ER_DUP_UNIQUE || mysql_errno(m_SqlHandle) == ER_DUP_KEY || mysql_errno(m_SqlHandle) == ER_DUP_ENTRY) // 主键冲突
        { 
            throw CException(ERR_DB_DUP_UNIQUE, strError.c_str(), __FILE__, __LINE__);
        }
        else if (mysql_errno(m_SqlHandle) == ER_BAD_TABLE_ERROR || mysql_errno(m_SqlHandle) == ER_NO_DB_ERROR || mysql_errno(m_SqlHandle) == ER_BAD_DB_ERROR || mysql_errno(m_SqlHandle)== ER_NO_SUCH_TABLE) // 库或表不存在
        { 
            throw CException(ERR_DB_DB_TBL_NOTEXIST, strError.c_str(), __FILE__, __LINE__);
        }
        else
        { 
        	char szErrorBuffer[1024];
            snprintf(szErrorBuffer, sizeof(szErrorBuffer), "mysql_errno=%d;mysql_error=%s query=%s", iMySQLErrNo, mysql_error(m_SqlHandle), query);
        	throw CException(ERR_DB_UNKNOW, szErrorBuffer, __FILE__, __LINE__);
        }
    }
}

/**
 * Description: 执行事务开始语句
 * Return:       失败抛出异常
 */
void CMySQL::Begin() throw (CException)
{
    Query("BEGIN", sizeof("BEGIN"));

    m_bInTransaction = true;
}

/**
 * Description: 执行事务提交
 * Return:       失败抛出异常
 */
void CMySQL::Commit() throw (CException)
{
    Query("COMMIT", sizeof("COMMIT"));

    m_bInTransaction = false;
}

/**
 * Description: 执行事务回滚
 * Return:      失败抛出异常
 */
void CMySQL::Rollback() throw (CException)
{
    Query("ROLLBACK", sizeof("ROLLBACK"));

    m_bInTransaction = false;
}

/**
 * Description: 获取SQL影响的行数
 * Return:      失败抛出异常
 */
unsigned long CMySQL::AffectedRows() throw(CException)
{
    my_ulonglong rownum = mysql_affected_rows(m_SqlHandle);
    if (rownum == (my_ulonglong) -1)
    {
        throw CException(ERR_DB_AFFECT_ROW, mysql_error(m_SqlHandle), __FILE__, __LINE__);
    }

    return rownum;
}

/**
 * Description: 取结果集
 * Return:      失败抛出异常
 */
MYSQL_RES* CMySQL::FetchResult() throw(CException)
{
    MYSQL_RES* pRes = mysql_store_result(m_SqlHandle);
    if (pRes == NULL)
    {
        throw CException(ERR_DB_NULL_RESULT, "Fetch result for non-select statement", __FILE__, __LINE__);
    }
    
    return pRes;
}

/**
 * Description: 取结果集
 * Return:      失败抛出异常
 */
MYSQL_RES* CMySQL::UseResult() throw(CException)
{
    MYSQL_RES* pRes = mysql_use_result(m_SqlHandle);
    if (pRes == NULL)
    {
        throw CException(ERR_DB_NULL_RESULT, "Fetch result for non-select statement", __FILE__, __LINE__);
    }
    
    return pRes;
}
