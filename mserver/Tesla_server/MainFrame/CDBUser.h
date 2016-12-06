#ifndef CDB_USER_H_
#define CDB_USER_H_

#include <stdint.h>
#include <string>
#include "exception.h"
#include "appcomm.h"

using namespace std;
//这里定义数据库数字段
const static char* USERDB_FID = "FID";
const static char* USERDB_FNAME = "FName";

class CUser
{
public:

    CUser(){}

    ~CUser(){}
	
	CUser(const CUser& order);

    CUser& operator=(const CUser& order);
	
	    /// 从数据库中导出的CStr2Map中提取数据
    void retriveFromStr2Map(const CStr2Map& inputData);

    /// 转换成CStr2Map
    /**
    * @remark 本函数不会清空outputData。若要清楚外部数据，请在调用前清理
    */
    void addToStr2Map(CStr2Map& outputData);

    /// 获取所有字段
    void getAllFieldName(CStr2Map& mapFieldName);

    
private:

    void copy(const CUser& order);
public:
	
	//这里定义DB字段变量
	string m_FID;
	string m_FName;
};

#endif

