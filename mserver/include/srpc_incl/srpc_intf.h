#ifndef __SRPC_INTF_H__
#define __SRPC_INTF_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

#include "srpc.pb.h"

using namespace google::protobuf;
using namespace std;

namespace srpc
{

enum {
    ENDPOINT_TYPE_NLB    = 1,       // 后端通过NLB寻址
    ENDPOINT_TYPE_ADDR   = 2,       // 后端直接直接地址
    ENDPOINT_TYPE_UNKOWN = 100,     // 未知类型
};

enum {
    PORT_TYPE_UDP = 1,  // UDP
    PORT_TYPE_TCP = 2,  // TCP
    PORT_TYPE_ALL = 3,  // ALL
};

class CProxyBase
{
public:
    CProxyBase(const string &endpoint); // "Login.ptlogin"  or "127.0.0.1:8888@udp"
    virtual ~CProxyBase();

    void SetEndPoint(const string &endpoint);
    const string &GetEndPoint(void);

    /**
     * @brief 设置调用方业务名
     */
    void SetCaller(const string &caller);

    /**
     * @brief 获取本地业务名
     */
    const string &GetCaller(void);

    /**
     * @brief 设置协议(PORT_TYPE_UDP/PORT_TYPE_TCP/PORT_TYPE_ALL)
     * @info  如果不设置，采用NLB获取的协议方式，优先使用TCP
     */
    void SetProtoType(int proto);

    /**
     * @brief 获取协议(PORT_TYPE_UDP/PORT_TYPE_TCP/PORT_TYPE_ALL)
     */
    int GetProtoType(void);

    /**
     * @brief 获取路由
     */
    int32_t GetRoute(struct sockaddr_in &addr, int32_t &type);

    /**
     * @brief 更新路由信息(回包统计)
     */
    void UpdateRoute(struct sockaddr_in &addr, int32_t failed, int32_t cost);

private:
    /**
     * @brief 解析endpoint地址信息
     */
    void ParseEndpoint(const string &endpoint);

private:

    int32_t m_type;                 // 后端寻址类型
    int32_t m_port_type;            // 端口类型
    string  m_endpoint;             // endpoint
    string  m_caller;               // 本地业务名
    string  m_service_name;         // NLB方式业务名
    struct sockaddr_in m_address;   // ADDR方式地址
};

/**
 * @brief 检测报文是否接收完整的回调函数定义
 * @param buf 报文保存缓冲区
 * @param len 已经接收的长度
 * @return >0 实际的报文长度; 0 还需要等待接收; <0 报文异常
 */
typedef int32_t (*CheckPkgLenFunc)(void *buf, int32_t len);

/**
 * @brief SRPC代理类
 */
class CSrpcProxy: public CProxyBase
{
public:
    CSrpcProxy(const string &endpoint);
    ~CSrpcProxy();

    /**
     * @brief 设置调用方法名
     */
    void SetMethod(const string &method); // 调用SRPC方法需要设置

    /**
     * @brief 设置日志是否染色
     */
    void SetColoring(bool coloring);

    /**
     * @brief 获取调用结果
     */
    void GetResult(int32_t &fret, int32_t &sret);

    /**
     * @brief 获取是否失败
     */
    bool Failed(void);

    /**
     * @brief 获取错误字符串描述
     */
    void GetErrText(string &err_text);

    /**
     * @brief 序列化请求报文
     * @info  业务需要自己free(pkg)
     * @return !=SRPC_SUCCESS   错误
     *         =SRPC_SUCCESS    成功
     */
    int32_t Serialize(char* &pkg, int32_t &len, const Message &request);

    /**
     * @brief 序列化请求报文
     * @return !=SRPC_SUCCESS   错误
     *         =SRPC_SUCCESS    成功
     */
    int32_t Serialize(const Message &request, string &out);

    /**
     * @brief 反序列化回复报文
     * @return !=SRPC_SUCCESS   错误
     *          =SRPC_SUCCESS   成功
     */
    int32_t DeSerialize(const char *pkg, int32_t len, Message &response);

    /**
     * @brief 反序列化回复报文
     * @return !=SRPC_SUCCESS   错误
     *          =SRPC_SUCCESS   成功
     */
    int32_t DeSerialize(const string &in, Message &response);

    /**
     * @brief  检查报文是否完整
     * @return <0  报文格式错误
     * @       =0  报文不完整
     * @       >0  报文有效长度
     */
    int32_t CheckPkgLen(void *pkg, int32_t len);

    /**
     * @brief 设置检查报文长度回调函数
     * @info  用于第三方协议检查报文完整性
     */
    void SetThirdCheckCb(CheckPkgLenFunc cb);

    /**
     * @brief 第三方法调用接口
     * @info  第三方协议调用接口
     * @param request    请求报文buffer
     *        req_len    请求报文长度
     *        response   回复报文buffer，接口malloc申请，调用者需要free释放
     *        rsp_len    回复报文长度
     *        timeout    超时时间
     * @return SRPC_SUCCESS 成功
     *         其它         失败
     */
    int32_t CallMethod(const char *request, int32_t req_len, char* &response, int32_t &rsp_len, int32_t timeout);

    /**
     * @brief SRPC方法调用接口
     * @param request    请求报文，业务自定义的pb格式
     *        response   回复报文，业务自定义的pb格式
     *        rsp_len    回复报文长度
     *        timeout    超时时间
     * @return  SRPC_SUCCESS 成功
     *          其它         失败
     */
    int32_t CallMethod(const Message &request, Message &response, int32_t timeout);

private:

    string   m_method_name;     // SRPC方法名
    bool     m_coloring;        // 设置是否日志染色
    uint64_t m_sequence;        // 网络收发sequence
    CRpcHead m_rsp_head;        // SRPC消息头
    void *   m_data;            // 自定义数据字段
    CheckPkgLenFunc m_check_cb; // 第三方协议检查报文函数
};


}

#endif

