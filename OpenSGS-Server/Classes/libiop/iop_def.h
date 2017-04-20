/*
 * Copyright (c) 2011-2014 zhangjianlin �ż���
 * eamil:jonas88@sina.com
 * addr: china
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _IOP_DEF_H_
#define _IOP_DEF_H_
#include <time.h>
#include "iop_config.h"
#include "iop_util.h"
#include "array_list.h"
#include "iop_buf.h"

#ifdef __cplusplus
extern "C" {
#endif



#define IOP_ERR_SOCKET -1
#define IOP_ERR_NO_MEM -2
#define IOP_ERR_PROTOCOL -3
#define IOP_ERR_TIMEOUT -4
#define IOP_ERR_CLOSED -5



//���¼�
#define EV_TYPE_READ                         1
//д�¼�
#define EV_TYPE_WRITE                       2

//�����¼�
#define EV_TYPE_CREATE                   4

//�����¼�
#define EV_TYPE_DESTROY                   8

//��ʱ���¼�
#define EV_TYPE_TIMER                       16

// ��ʱ
#define EV_TYPE_TIMEOUT				32

//�����¼�
#define EV_TYPE_ERROR               64
//���Ӵ����¼�
#define EV_TYPE_CONNECT               128


#define IOP_ERR_SYS  -1


//һ������¼����͵ĺ�
#define EV_IS_SET_R(e) ((e)&EV_TYPE_READ)
#define EV_IS_SET_W(e) ((e)&EV_TYPE_WRITE)
#define EV_IS_SET(e,x) ((e)&x)


#define EV_SET_R(e) ((e) | EV_TYPE_READ)
#define EV_CLR_R(e) ((e) & ~EV_TYPE_READ)
#define EV_SET_W(e) ((e) | EV_TYPE_WRITE)
#define EV_CLR_W(e) ((e) & ~EV_TYPE_WRITE)

#define EV_SET_RW(e) ((e)|EV_TYPE_READ|EV_TYPE_WRITE)
#define EV_TYPE_RW (EV_TYPE_READ|EV_TYPE_WRITE)

//ϵͳ֧�ֵ��¼�ģ��

//NULL������ϵͳ�Զ�ѡ��
#define IOP_MODEL_NULL                      0
//event portsģ����δʵ��
#define IOP_MODEL_EVENT_PORTS       1
//kqueueģ����δʵ��
#define IOP_MODEL_KQUEUE                  2
//linux epollģ���Ѿ�֧��
#define IOP_MODEL_EPOLL                     3
//dev pollģ����δʵ��
#define IOP_MODEL_DEV_POLL              4
//pollģ���Ѿ�֧��
#define IOP_MODEL_POLL                       5
//select ģ���Ѿ�֧��. windows & linux
#define IOP_MODEL_SELECT                   6
//windows iocpģ����δʵ��
#define IOP_MODEL_IOCP                       7
//ֻ֧�ֶ�ʱ��
#define IOP_MODEL_TIMER                     8



#define DEFAULT_DISPATCH_INTERVAL	500

/*max socket data buf:64M*/
#define IOP_MAX_BUF_SIZE   67108864
#define IOP_TYPE_FREE	0
#define IOP_TYPE_IO		1
#define IOP_TYPE_TIMER	2
#define IOP_TYPE_CONNECT	3

struct tag_iop_op_t;
typedef struct tag_iop_op_t iop_op_t;
struct tag_iop_base_t;
typedef struct tag_iop_base_t iop_base_t;
struct tag_iop_t;
typedef struct tag_iop_t iop_t;

typedef time_t iop_time_t;

/*Э������ص�����*/
typedef int (*iop_parse_cb)(const void *,unsigned int);

/*�¼��ص�����,����-1����Ҫɾ������,����0��������*/
typedef int (*iop_event_cb)(iop_base_t *,int,unsigned int,void *);

typedef int (*dispatch_imp_cb)(iop_base_t *, int);

/*********************************************************************/
/*
*tag_iop_t:iop�ṹ��ÿһ��iop���󶼻��Ӧһ��tag_iop_t�ṹ
*/
struct tag_iop_t
{
	int id;					/*��Ӧ��id*/
	io_handle_t handle;		/*�����ľ��*/
	int iop_type;			/*�������ͣ�0��free,1:io,2:timer*/
	int prev;				/*��һ������*/
	int next;				/*��һ������*/
	unsigned int events;				/*��ע���¼�*/
	int timeout;			/*��ʱֵ*/
	iop_event_cb evcb;		/*�¼��ص�*/
	void *arg;				/*�û�ָ���Ĳ���,���û������ͷ���Դ*/
	void *sys_arg;			/*ϵͳָ���Ĳ�����ϵͳ�Զ��ͷ���Դ*/
	/*�����ֶζԶ�ʱ������*/
	dbuf_t *sbuf;		/*���ͻ�����*/
	dbuf_t *rbuf;		/*���ջ�����*/
	iop_time_t last_dispatch_time;	/*�ϴε��ȵ�ʱ��*/
};


struct tag_iop_op_t
{
    const char *name;                               //ģ������
    void (*base_free)(iop_base_t *);        //��Դ�ͷŵĽӿ�
    int (*base_dispatch)(iop_base_t *, int);  //ģ�͵��Ƚӿ�
    //����¼�
    int (*base_add)(iop_base_t *, int, io_handle_t, unsigned int);
    //ɾ���¼�
    int (*base_del)(iop_base_t *, int,io_handle_t);
    //�޸��¼�
    int (*base_mod)(iop_base_t *, int, io_handle_t, unsigned int);
};


struct tag_iop_base_t
{
	iop_t *iops;		/*����iop*/
	int maxio;			/*��󲢷�io��,������ʱ������*/
	int maxbuf;			/*�������ͻ���ջ�������ֵ*/
	int free_list_head;	/*����iop�б�*/
	int free_list_tail; /*���һ������iop*/
	int io_list_head;	/*����io���͵�iop�б�*/
	int timer_list_head;	/*����timer���͵�iop�б�*/
    int connect_list_head;  /*�첽���ӵ�iop�б�*/
    volatile int exit_flag;	/*�˳���־*/

	int dispatch_interval;		/*�߶ȵļ��ʱ��*/
    iop_op_t op_imp;           /*�¼�ģ�͵��ڲ�ʵ��*/
    void *model_data;         /*�¼�ģ���ض�������*/

	iop_time_t cur_time;		/*��ǰ����ʱ��*/
	iop_time_t last_time;		/*�ϴε���ʱ��*/
	iop_time_t last_keepalive_time; /*�ϴμ��keepalive��ʱ��*/

	_list_node_t * tcp_protocol_list_head;	/*use for advance tcp server model.*/
};


#define IOP_CB(base,iop,events)	{if(iop->iop_type != IOP_TYPE_FREE){if(0==(iop->evcb)(base, iop->id,events,iop->arg)){iop->last_dispatch_time = base->cur_time;}else{iop_del(base,iop->id);}}}

#ifdef __cplusplus
}
#endif

#endif


