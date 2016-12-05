/**
 * @brief 自动生成的业务代码逻辑实现
 */

#ifndef __pMainLogic_MSG_HEAD_H__
#define __pMainLogic_MSG_HEAD_H__

#include "syncincl.h"
#include "srpcincl.h"
#include "v2.pb.h"

using namespace pMainLogic;


/**
 * @brief 默认生成的服务实现类型
 * @info  [注意] 不建议业务修改该类的实现，以方便后续SRPC框架更新或者业务新加方法时可直接覆盖该文件
 */
class CMainLogicServiceMsg : public CRpcMsgBase
{
public:

    // 构造函数及析构函数定义
    CMainLogicServiceMsg() {}
    virtual ~CMainLogicServiceMsg() {}

    // 克隆方法定义
    CRpcMsgBase* New() {
        return new CMainLogicServiceMsg;
    }

    
    /**
     * @brief  自动生成的业务方法实现接口
     * @param  request  [入参]业务请求报文
     *         response [出参]业务回复报文
     * @return 框架会将返回值作为执行结果传给客户端
     */
    virtual int GetTitles(const GetTitlesRequest* request, GetTitlesResponse* response);

    /**
     * @brief  自动生成的业务方法实现接口
     * @param  request  [入参]业务请求报文
     *         response [出参]业务回复报文
     * @return 框架会将返回值作为执行结果传给客户端
     */
    virtual int GetUrlByTitle(const GetUrlByTitleRequest* request, GetUrlByTitleResponse* response);

    /**
     * @brief  自动生成的业务方法实现接口
     * @param  request  [入参]业务请求报文
     *         response [出参]业务回复报文
     * @return 框架会将返回值作为执行结果传给客户端
     */
    virtual int DownloadMP3(const DownloadMP3Request* request, DownloadMP3Response* response);

};


#endif


