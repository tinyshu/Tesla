#ifndef __SRPC_COMM_H__
#define __SRPC_COMM_H__

#include <stdint.h>
#include <string>

namespace srpc {

// 本地返回码定义
enum SRPC_RETCODE
{
    SRPC_SUCCESS                    = 0,
    SRPC_ERR_PARA_ERROR             = -1,
    SRPC_ERR_SYSTEM_ERROR           = -2,
    SRPC_ERR_INVALID_PKG            = -3,
    SRPC_ERR_INVALID_PKG_HEAD       = -4,
    SRPC_ERR_INVALID_PKG_BODY       = -5,
    SRPC_ERR_INVALID_METHOD_NAME    = -6,
    SRPC_ERR_HEADER_UNINIT          = -7,
    SRPC_ERR_BODY_UNINIT            = -8,
    SRPC_ERR_NO_MEMORY              = -9,
    SRPC_ERR_TIMEOUT                = -10,
    SRPC_ERR_NETWORK                = -11,
    SRPC_ERR_RECV_TIMEOUT           = -12,
    SRPC_ERR_SEND_RECV_FAILED       = -13,
    SRPC_ERR_INVALID_ENDPOINT       = -14,
    SRPC_ERR_GET_ROUTE_FAILED       = -15,
    SRPC_ERR_INVALID_SEQUENCE       = -16,
    SRPC_ERR_NO_BODY                = -17,
    SRPC_ERR_SERVICE_IMPL_FAILED    = -18,
};

/**
 * @brief 通过错误码，获取错误信息描述
 */
const char *errmsg(int32_t err);

/**
 * @brief sequence生成函数
 */
uint64_t newseq(void);

/**
 * @brief 染色日志ID生成函数，通过方法名计算生成
 */
uint64_t gen_colorid(const char *method_name);

/**
 * @brief 时延到字符串转换函数
 */
const char *Cost2TZ(int32_t cost);

}

#endif
