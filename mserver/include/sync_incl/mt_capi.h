#ifndef __MT_CAPI_H__
#define __MT_CAPI_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*  ΢�߳��û��ӿڶ���: ΢�̷߳�װϵͳ�ӿ� C                                  */
/******************************************************************************/

/**
 * @brief ΢�߳�����sleep�ӿ�, ��λms
 * @info  ҵ����Ҫ�����ó�CPUʱʹ��
 */
void mtc_sleep(int ms);

/**
 * @brief ΢�̻߳�ȡϵͳʱ�䣬��λms
 */
unsigned long long mtc_time_ms(void);

/******************************************************************************/
/*  ΢�߳��û��ӿڶ���: ΢�߳��û�˽�����ݽӿ�                                */
/******************************************************************************/

/**
 * @brief ���õ�ǰ΢�̵߳�˽�б���
 * @info  ֻ����ָ�룬�ڴ���Ҫҵ�����
 */
void mtc_set_private(void *data);

/**
 * @brief  ��ȡ��ǰ΢�̵߳�˽�б���
 * @return ˽�б���ָ��
 */
void* mtc_get_private(void);


/******************************************************************************/
/*  ΢�߳��û��ӿڶ���: ΢�̷߳�װϵͳ�ӿ�(���Ƽ�ʹ��)                        */
/******************************************************************************/

/**
 * @brief  ΢�߳̿�ܳ�ʼ��
 * @info   ҵ��ʹ��spp������΢�̣߳���Ҫ���øú�����ʼ����ܣ�
 *         ʹ��spp��ֱ�ӵ���SyncFrame�Ŀ�ܳ�ʼ����������
 * @return <0 ��ʼ��ʧ��  0 ��ʼ���ɹ�
 */
int mtc_init_frame(void);

/**
 * @brief ����΢�̶߳���ջ�ռ��С
 * @info  �Ǳ������ã�Ĭ�ϴ�СΪ128K
 */
void mtc_set_stack_size(unsigned int bytes);

/**
 * @brief ΢�̰߳�����ϵͳIO���� recvfrom
 * @param fd ϵͳsocket��Ϣ
 * @param buf ������Ϣ������ָ��
 * @param len ������Ϣ����������
 * @param from ��Դ��ַ��ָ��
 * @param fromlen ��Դ��ַ�Ľṹ����
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 �ɹ����ճ���, <0 ʧ��
 */
int mtc_recvfrom(int fd, void *buf, int len, int flags, struct sockaddr *from, socklen_t *fromlen, int timeout);

/**
 * @brief ΢�̰߳�����ϵͳIO���� sendto
 * @param fd ϵͳsocket��Ϣ
 * @param msg �����͵���Ϣָ��
 * @param len �����͵���Ϣ����
 * @param to Ŀ�ĵ�ַ��ָ��
 * @param tolen Ŀ�ĵ�ַ�Ľṹ����
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 �ɹ����ͳ���, <0 ʧ��
 */
int mtc_sendto(int fd, const void *msg, int len, int flags, const struct sockaddr *to, int tolen, int timeout);


/**
 * @brief ΢�̰߳�����ϵͳIO���� connect
 * @param fd ϵͳsocket��Ϣ
 * @param addr ָ��server��Ŀ�ĵ�ַ
 * @param addrlen ��ַ�ĳ���
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 �ɹ����ͳ���, <0 ʧ��
 */
int mtc_connect(int fd, const struct sockaddr *addr, int addrlen, int timeout);

/**
 * @brief ΢�̰߳�����ϵͳIO���� accept
 * @param fd �����׽���
 * @param addr �ͻ��˵�ַ
 * @param addrlen ��ַ�ĳ���
 * @param timeout ��ȴ�ʱ��, ����
 * @return >=0 accept��socket������, <0 ʧ��
 */
int mtc_accept(int fd, struct sockaddr *addr, socklen_t *addrlen, int timeout);

/**
 * @brief ΢�̰߳�����ϵͳIO���� read
 * @param fd ϵͳsocket��Ϣ
 * @param buf ������Ϣ������ָ��
 * @param nbyte ������Ϣ����������
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 �ɹ����ճ���, <0 ʧ��
 */
ssize_t mtc_read(int fd, void *buf, size_t nbyte, int timeout);

/**
 * @brief ΢�̰߳�����ϵͳIO���� write
 * @param fd ϵͳsocket��Ϣ
 * @param buf �����͵���Ϣָ��
 * @param nbyte �����͵���Ϣ����
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 �ɹ����ͳ���, <0 ʧ��
 */
ssize_t mtc_write(int fd, const void *buf, size_t nbyte, int timeout);

/**
 * @brief ΢�̰߳�����ϵͳIO���� recv
 * @param fd ϵͳsocket��Ϣ
 * @param buf ������Ϣ������ָ��
 * @param len ������Ϣ����������
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 �ɹ����ճ���, <0 ʧ��
 */
ssize_t mtc_recv(int fd, void *buf, int len, int flags, int timeout);

/**
 * @brief ΢�̰߳�����ϵͳIO���� send
 * @param fd ϵͳsocket��Ϣ
 * @param buf �����͵���Ϣָ��
 * @param nbyte �����͵���Ϣ����
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 �ɹ����ͳ���, <0 ʧ��
 */
ssize_t mtc_send(int fd, const void *buf, size_t nbyte, int flags, int timeout);


/**
 * @brief ΢�̵߳ȴ�epoll�¼��İ�������
 * @param fd ϵͳsocket��Ϣ
 * @param events �ȴ����¼� IN/OUT
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 ������¼�, <0 ʧ��
 */
int mtc_wait_events(int fd, int events, int timeout);


/**
 * @brief ����΢�߳�
 * @param entry   ��ں���ָ�룬���ͼ�ThreadStart
 * @param args    ��ں�������
 * @return   0 �����ɹ�  <0 ����ʧ��
 */
int mtc_start_thread(void* entry, void* args);

/**
 * @brief ��TCP���ӳ��л�ȡ���ӣ����û�У����´���
 * @param dst  Ŀ�ĵ�ַ
 * @param sock socket�ļ�������
 * @return ����΢�߳�TCP���Ӷ���
 */
void *mtc_get_keep_conn(struct sockaddr_in* dst, int *sock);

/**
 * @brief  �ͷ�TCP���ӵ����ӳ�
 * @param  conn   mt_get_keep_conn���ص����Ӷ���ָ��
 * @param  force  1  -> ǿ���ͷ����ӣ�����������ӳ�
 * @param         0  -> �ͷŵ����ӳأ����ӳ����ˣ��ر�����
 */
void mtc_free_keep_conn(void *conn, int force);



#ifdef __cplusplus
}
#endif


#endif

