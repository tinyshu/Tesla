#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <string>
#include <exception>
#include <iostream>

using std::string;
using std::ostream;

//#define CEXCEPTION(errcode, msg)            CException(errcode, (msg), basename(__FILE__), __LINE__)
//#define CMIDEXCEPTION(svc, errcode, msg)    CMidException((svc), errcode, (msg), basename(__FILE__), __LINE__)
//#define CRELAYEXCEPTION(rt, errcode, msg)   CRelayException(rt, errcode, (msg), basename(__FILE__), __LINE__)
#//define THROW(errcode, msg)            throw CException(errcode, (msg), basename(__FILE__), __LINE__)
/**
 * 业务异常基类
 */
class CException : public std::exception
{   
public:

    /**
     * 异常打包返回标记
     */
    enum PACK_FORMAT
    {
        URL_FORMAT, // 打包时用result=xxx&res_info的格式
        RAW_FORMAT // 打包时直接返回_errinfo里边的信息
    };

public:
    
    /**
     * 构造函数
     */
    CException(int errNo, const char* szErrInfo, const char* szFile = NULL, const int iLine = 0, PACK_FORMAT format = RAW_FORMAT) throw();

    /**
     * 构造函数
     */
    CException(int errNo, const string& strErrInfo, const char* szFile = NULL, const int iLine = 0, PACK_FORMAT format = RAW_FORMAT) throw();

    /**
     * 析构函数
     */
    virtual ~CException() throw();
    
    /**
     * 是否透传异常
     */
    inline PACK_FORMAT format() const throw()  { return _format; }

    /**
     * 异常产生的文件名
     */
    inline const char* file() const throw(){    return _file.c_str(); }; 

    /**
     * 异常产生的行号
     */
    inline int line() const throw()    { return _line; } 

    /**
     * 异常对应的错误码
     */
    inline int error() const throw()   { return _errno;} 

    /**
     * 异常对应的错误描述信息
     */
    virtual const char* what() const throw(); 

protected:
    int  _errno; // 错误码
    string _errinfo; // 错误信息
    string _file; // 错误产生的代码文件
    int _line; // 错误产生的代码行号    
    PACK_FORMAT _format; // 直接返回缓存标记
};

/**
 * 打印异常信息的操作
 */
inline ostream& operator << (ostream& os, const CException& e)
{
    os<<e.error()<<":"<<e.what();
    
    return os;
}

#endif

