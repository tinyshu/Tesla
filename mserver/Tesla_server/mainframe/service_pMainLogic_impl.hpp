/**
 * @brief service impl
 */

#ifndef __pMainLogic_HEAD_H__
#define __pMainLogic_HEAD_H__

#include <string>
#include "syncincl.h"
#include "srpcincl.h"
#include "msg_pMainLogic_impl.h"


/**
 * @brief 默认生成的服务实现类型
 */
class CRpcMainLogicServiceImpl : public pMainLogic::MainLogicService
{
public:

    // 构造函数及析构函数
    CRpcMainLogicServiceImpl() {}
    virtual ~CRpcMainLogicServiceImpl() {}

    
    // RPC方法函数定义
    virtual void GetTitles(::google::protobuf::RpcController* controller,\
                const GetTitlesRequest* request, \
                GetTitlesResponse* response, \
                ::google::protobuf::Closure* done);

    // RPC方法函数定义
    virtual void GetUrlByTitle(::google::protobuf::RpcController* controller,\
                const GetUrlByTitleRequest* request, \
                GetUrlByTitleResponse* response, \
                ::google::protobuf::Closure* done);

    // RPC方法函数定义
    virtual void DownloadMP3(::google::protobuf::RpcController* controller,\
                const DownloadMP3Request* request, \
                DownloadMP3Response* response, \
                ::google::protobuf::Closure* done);


};


/**
 * @brief RPC方法函数定义
 */
void CRpcMainLogicServiceImpl::GetTitles(::google::protobuf::RpcController* controller,
            const GetTitlesRequest* request,
            GetTitlesResponse* response,
            ::google::protobuf::Closure* done)
{
    CMainLogicServiceMsg* msg = dynamic_cast<CMainLogicServiceMsg*>(controller);
    int32_t ret = msg->GetTitles(request, response);
    msg->SetSrvRet(ret);
}

/**
 * @brief RPC方法函数定义
 */
void CRpcMainLogicServiceImpl::GetUrlByTitle(::google::protobuf::RpcController* controller,
            const GetUrlByTitleRequest* request,
            GetUrlByTitleResponse* response,
            ::google::protobuf::Closure* done)
{
    CMainLogicServiceMsg* msg = dynamic_cast<CMainLogicServiceMsg*>(controller);
    int32_t ret = msg->GetUrlByTitle(request, response);
    msg->SetSrvRet(ret);
}

/**
 * @brief RPC方法函数定义
 */
void CRpcMainLogicServiceImpl::DownloadMP3(::google::protobuf::RpcController* controller,
            const DownloadMP3Request* request,
            DownloadMP3Response* response,
            ::google::protobuf::Closure* done)
{
    CMainLogicServiceMsg* msg = dynamic_cast<CMainLogicServiceMsg*>(controller);
    int32_t ret = msg->DownloadMP3(request, response);
    msg->SetSrvRet(ret);
}






#endif


