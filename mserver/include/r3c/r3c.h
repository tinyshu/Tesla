/*
 * Copyright (c) 2016, Jian Yi <eyjian at gmail dot com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
// Redis Cluster Principle
// http://blog.chinaunix.net/uid-20682147-id-5727861.html
#ifndef REDIS_CLUSTER_CLIENT_H
#define REDIS_CLUSTER_CLIENT_H
#include <assert.h>
#include <exception>
#include <hiredis.h>
#include <inttypes.h> // PRId64
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <stdint.h> // uint16_t
#include <string>
#include <vector>

#define PRINT_COLOR_NONE         "\033[m"
#define PRINT_COLOR_RED          "\033[0;32;31m"
#define PRINT_COLOR_YELLOW       "\033[1;33m"
#define PRINT_COLOR_BLUE         "\033[0;32;34m"
#define PRINT_COLOR_GREEN        "\033[0;32;32m"
#define PRINT_COLOR_WHITE        "\033[1;37m"
#define PRINT_COLOR_CYAN         "\033[0;36m"      // ����ɫ, ��ɫ
#define PRINT_COLOR_PURPLE       "\033[0;35m"      // ��ɫ
#define PRINT_COLOR_BROWN        "\033[0;33m"      // ��ɫ, ��ɫ
#define PRINT_COLOR_DARY_GRAY    "\033[1;30m"      // �����ɫ
#define PRINT_COLOR_LIGHT_RED    "\033[1;31m"
#define PRINT_COLOR_LIGHT_GREEN  "\033[1;32m"
#define PRINT_COLOR_LIGHT_BLUE   "\033[1;34m"
#define PRINT_COLOR_LIGHT_CYAN   "\033[1;36m"
#define PRINT_COLOR_LIGHT_PURPLE "\033[1;35m"
#define PRINT_COLOR_LIGHT_GRAY   "\033[0;37m"      // ����ɫ
namespace r3c {

enum ZADDFLAG
{
    Z_NS, // Don't set options
    Z_XX, // Only update elements that already exist. Never add elements.
    Z_NX, // Don't update already existing elements. Always add new elements.
    Z_CH  // Modify the return value from the number of new elements added
};

// Error code
enum
{
    ERR_PARAMETER = -1,        // Parameter error
    ERR_INIT_REDIS_CONN = -2,  // Initialize redis connection error
    ERROR_COMMAND = -3,        // Command error
    ERROR_CONNECT_REDIS = -4,  // Can not connect any cluster node
    ERROR_FORMAT = -5,         // Format error
    ERROR_NOT_SUPPORT = -6,    // Not support
    ERROR_SLOT_NOT_EXIST = -7  // Slot not exists
};

// Consts
enum
{
    RETRY_TIMES = 10,                    // Default value
    RETRY_SLEEP_MILLISECONDS = 1000,     // Default value
    CONNECT_TIMEOUT_MILLISECONDS = 1000, // Connect timeout milliseconds
    DATA_TIMEOUT_MILLISECONDS = 2000     // Read and write socket timeout milliseconds
};

void millisleep(uint32_t millisecond);
std::string format_string(const char* format, ...) __attribute__((format(printf, 1, 2)));
int split(std::vector<std::string>* tokens, const std::string& source, const std::string& sep, bool skip_sep=false);

// Cluster node info
struct NodeInfo
{
    std::string id;         // The node ID, a 40 characters random string generated when a node is created and never changed again (unless CLUSTER RESET HARD is used)
    std::string ip;         // The node IP
    uint16_t port;          // The node port
    std::string flags;      // A list of comma separated flags: myself, master, slave, fail?, fail, handshake, noaddr, noflags
    bool is_fail;
    bool is_master;         // true if node is master, false if node is salve
    bool is_slave;
    std::string master_id;  // The replication master
    int ping_sent;          // Milliseconds unix time at which the currently active ping was sent, or zero if there are no pending pings
    int pong_recv;          // Milliseconds unix time the last pong was received
    int epoch;              // The configuration epoch (or version) of the current node (or of the current master if the node is a slave). Each time there is a failover, a new, unique, monotonically increasing configuration epoch is created. If multiple nodes claim to serve the same hash slots, the one with higher configuration epoch wins
    bool connected;         // The state of the link used for the node-to-node cluster bus
    std::vector<std::pair<int, int> > slots; // A hash slot number or range

    std::string str() const
    {
        return format_string("node://%s/%s:%d/%s", id.c_str(), ip.c_str(), port, flags.c_str());
    }
};

// Set NULL to discard log
typedef void (*LOG_WRITE)(const char* format, ...);
void set_error_log_write(LOG_WRITE info_log);
void set_info_log_write(LOG_WRITE info_log);
void set_debug_log_write(LOG_WRITE debug_log);
unsigned int get_key_slot(const std::string* key);
std::ostream& operator <<(std::ostream& os, const struct NodeInfo& node_info);

template <typename T>
inline std::string any2string(T m)
{
    std::stringstream ss;
    ss << m;
    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////
class CRedisException: public std::exception
{
public:
    CRedisException(int errcode, const std::string& errmsg, const char* file, int line, const std::string& node_ip=std::string("-"), uint16_t node_port=0, const char* command=NULL, const char* key=NULL) throw ();
    virtual ~CRedisException() throw () {}
    virtual const char* what() const throw ();
    std::string str() const throw ();

    const char* file() const throw () { return _file.c_str(); }
    int line() const throw () { return _line; }
    const char* node_ip() const throw () { return _node_ip.c_str(); }
    uint16_t node_port() const throw () { return _node_port; }
    const char* command() const throw() { return _command.c_str(); }
    const char* key() const throw() { return _key.c_str(); }

private:
    const int _errcode;
    const std::string _errmsg;
    const std::string _file;
    const int _line;
    const std::string _node_ip;
    const uint16_t _node_port;
    std::string _command;
    std::string _key;
};

////////////////////////////////////////////////////////////////////////////////
struct SlotInfo; // Forward declare
struct ParamInfo;

// NOTICE: not thread safe
class CRedisClient
{
public:
    // nodes - Redis cluster nodes separated by comma,
    //         e.g., 127.0.0.1:6379,127.0.0.1:6380,127.0.0.2:6379,127.0.0.3:6379,
    //         standalone mode if only one node, else cluster mode.
    CRedisClient(const std::string& nodes, int connect_timeout_milliseconds=CONNECT_TIMEOUT_MILLISECONDS, int data_timeout_milliseconds=DATA_TIMEOUT_MILLISECONDS) throw (CRedisException);
    ~CRedisClient();

    bool cluster_mode() const;
    void set_retry(int retry_times, int retry_sleep_milliseconds);

public:
    // Get all nodes information of redis cluster,
    // available only in the cluster mode, standalone mode unavailable
    // which - The node IP and node port which returns nodes information
    int list_nodes(std::vector<struct NodeInfo>* nodes_info, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);

    // key value
    bool key_type(const std::string& key, std::string* key_type, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    bool exists(const std::string& key, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    bool expire(const std::string& key, uint32_t seconds, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    void set(const std::string& key, const std::string& value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    bool setnx(const std::string& key, const std::string& value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    void setex(const std::string& key, const std::string& value, uint32_t seconds, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    bool get(const std::string& key, std::string* value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    bool del(const std::string& key, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int64_t incrby(const std::string& key, int64_t increment, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int scan(int cursor, std::vector<std::string>* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int scan(int cursor, int count, std::vector<std::string>* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int scan(int cursor, const std::string& pattern, std::vector<std::string>* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int scan(int cursor, const std::string& pattern, int count, std::vector<std::string>* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);

    // list
    int llen(const std::string& key, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    bool lpop(const std::string& key, std::string* value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int lpush(const std::string& key, const std::string& value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int lpush(const std::string& key, const std::vector<std::string>& values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int lrange(const std::string& key, int start, int end, std::vector<std::string>* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    bool ltrim(const std::string& key, int start, int end, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    bool rpop(const std::string& key, std::string* value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int rpush(const std::string& key, const std::string& value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int rpush(const std::string& key, const std::vector<std::string>& values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int rpushx(const std::string& key, const std::string& value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);

    // hash
    bool hdel(const std::string& key, const std::string& field, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int hdel(const std::string& key, const std::vector<std::string>& fields, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    bool hexists(const std::string& key, const std::string& field, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int hlen(const std::string& key, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    bool hset(const std::string& key, const std::string& field, const std::string& value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    bool hsetnx(const std::string& key, const std::string& field, const std::string& value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    bool hget(const std::string& key, const std::string& field, std::string* value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int64_t hincrby(const std::string& key, const std::string& field, int64_t increment, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    void hmset(const std::string& key, const std::map<std::string, std::string>& map, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int hmget(const std::string& key, const std::vector<std::string>& fields, std::map<std::string, std::string>* map, bool keep_null=false, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int hgetall(const std::string& key, std::map<std::string, std::string>* map, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int hstrlen(const std::string& key, const std::string& field, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int hkeys(const std::string& key, std::vector<std::string>* fields, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int hvals(const std::string& key, std::vector<std::string>* vals, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int hscan(const std::string& key, int cursor, std::map<std::string, std::string>* map, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int hscan(const std::string& key, int cursor, int count, std::map<std::string, std::string>* map, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int hscan(const std::string& key, int cursor, const std::string& pattern, std::map<std::string, std::string>* map, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int hscan(const std::string& key, int cursor, const std::string& pattern, int count, std::map<std::string, std::string>* map, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);

    // set
    int sadd(const std::string& key, const std::string& value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int sadd(const std::string& key, const std::vector<std::string>& values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int scard(const std::string& key, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    bool sismember(const std::string& key, const std::string& value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int smembers(const std::string& key, std::vector<std::string>* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    bool spop(const std::string& key, std::string* value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int spop(const std::string& key, int count, std::vector<std::string>* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int srandmember(const std::string& key, int count, std::vector<std::string>* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int srem(const std::string& key, const std::string& value, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int srem(const std::string& key, const std::vector<std::string>& values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int sscan(const std::string& key, int cursor, std::vector<std::string>* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int sscan(const std::string& key, int cursor, int count, std::vector<std::string>* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int sscan(const std::string& key, int cursor, const std::string& pattern, std::vector<std::string>* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int sscan(const std::string& key, int cursor, const std::string& pattern, int count, std::vector<std::string>* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);

    // sort set
    int zadd(const std::string& key, const std::string& field, int64_t score, ZADDFLAG flag=Z_NS, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int zadd(const std::string& key, const std::map<std::string, int64_t>& map, ZADDFLAG flag=Z_NS, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int zcount(const std::string& key, int64_t min, int64_t max , std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int64_t zincrby(const std::string& key, const std::string& field, int64_t increment, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int zrange(const std::string& key, int start, int end, bool withscores, std::vector<std::pair<std::string, int64_t> >* vec, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int zrevrange(const std::string& key, int start, int end, bool withscores, std::vector<std::pair<std::string, int64_t> >* vec, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int zrank(const std::string& key, const std::string& field, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int zrevrank(const std::string& key, const std::string& field, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int64_t zscore(const std::string& key, const std::string& field, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int zscan(const std::string& key, int cursor, std::vector<std::pair<std::string, int64_t> >* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int zscan(const std::string& key, int cursor, int count, std::vector<std::pair<std::string, int64_t> >* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int zscan(const std::string& key, int cursor, const std::string& pattern, std::vector<std::pair<std::string, int64_t> >* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);
    int zscan(const std::string& key, int cursor, const std::string& pattern, int count, std::vector<std::pair<std::string, int64_t> >* values, std::pair<std::string, uint16_t>* which=NULL) throw (CRedisException);

    // raw command, binary unsafe
    const redisReply* redis_command(int excepted_reply_type, std::pair<std::string, uint16_t>* which, const std::string* key, const char* command, const std::string& command_string) throw (CRedisException);
    // raw command, binary safe
    const redisReply* redis_command(int excepted_reply_type, std::pair<std::string, uint16_t>* which, const std::string* key, const char* command, int argc, const char* argv[], const size_t* argv_len) throw (CRedisException);

private:
    const redisReply* redis_command(int excepted_reply_type, std::pair<std::string, uint16_t>* which, const std::string* key, const char* command, const std::string& command_string, int argc, const char* argv[], const size_t* argv_len) throw (CRedisException);
    int64_t redis_command(int excepted_reply_type, struct ParamInfo* param_info) throw (CRedisException);
    int calc_argc(const struct ParamInfo* param_info) const;

private:
    void parse_nodes() throw (CRedisException);
    void init() throw (CRedisException);
    redisContext* get_redis_context(unsigned int slot, std::pair<std::string, uint16_t>* node) throw (CRedisException);
    void choose_node(int seed_factor, std::pair<std::string, uint16_t>* node) const;
    redisContext* connect_node(int* errcode, std::string* errmsg, std::pair<std::string, uint16_t>* node) const;

private:
    void clear();
    void clear_redis_contexts();
    void clear_slots();
    void retry_sleep() const;

private:
    bool _cluster_mode;
    std::string _nodes_string;
    int _connect_timeout_milliseconds;
    int _data_timeout_milliseconds; // read & write timeout
    int _retry_times;
    int _retry_sleep_milliseconds;
    redisContext* _redis_context; // Standalone mode

private: // Cluster mode
    std::map<std::pair<uint32_t, uint16_t>, redisContext*> _redis_contexts;
    std::vector<std::pair<std::string, uint16_t> > _nodes;
    std::vector<struct SlotInfo*> _slots;
};

} // namespace r3c {
#endif // REDIS_CLUSTER_CLIENT_H
