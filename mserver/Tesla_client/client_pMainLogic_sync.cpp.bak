/**
 *  @brief 客户端示例
 */
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "srpcincl.h"
#include "v2.pb.h"

using namespace pMainLogic;
using namespace srpc;
using namespace std;

/**
 * @brief 采用微线程框架实现的简单客户端例子
 */
int main(int argc, char* argv[])
{
    // 初始化微线程框架
    if (!mt_init_frame())
    {
        cout << "Init frame failed, quit" << endl;
        return -1;
    }

    // 定义目标服务与请求消息
    CRpcUdpChannel channel("127.0.0.1:7963");
    GetTitlesRequest request;
    // TODO:设置请求报文格式
    request.set_type("special");
    
    // 执行RPC调用
    MainLogicService::Stub stub(&channel);
    CRpcCtrl ctrl;
    GetTitlesResponse response;
    stub.GetTitles(&ctrl, &request, &response, NULL);

    // 检查结果
    if (!ctrl.Failed())
    {
        cout << "Received response OK!" << response.DebugString() << endl;
    }
    else
    {
        cout << ctrl.ErrorText() << endl;
    }

    return 0;

}



