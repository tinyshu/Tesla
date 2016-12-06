#include "exception.h"
#include <sstream>

/**
 * 异常类的构造函数
 * @input:  errNum       错误码
 *              szErrInfo     错误描述信息
 *              szFile    异常出现的文件名
 *              iLine     抛出异常的行号
 *              format  异常打包格式
 */
CException::CException(int errNum, const char* szErrInfo, const char* szFile, const int iLine, PACK_FORMAT format) throw() 
                       : _errno(errNum), _errinfo(szErrInfo), _file(szFile ? szFile : ""), _line(iLine), _format(format)
{
}

/**
 * 异常类的构造函数
 * @input:  errNum       错误码
 *              strErrInfo    错误描述信息
 *              szFile    异常出现的文件名
 *              iLine     抛出异常的行号
 *              format  异常打包格式
 */
CException::CException(int errNo, const string& strErrInfo, const char* szFile, const int iLine, PACK_FORMAT format) throw()
                        : _errno(errNo), _errinfo(strErrInfo), _file(szFile ? szFile : ""), _line(iLine), _format(format)
{
}

/**
 * 异常类的析构函数
 */
CException::~CException() throw()
{
}

/**
 * 获取异常错误信息
 * Return: 错误描述信息
 */
const char* CException::what() const throw()
{
    return _errinfo.c_str();
}

