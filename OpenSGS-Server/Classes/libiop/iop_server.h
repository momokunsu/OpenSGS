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
#ifndef _IOP_SERVER_H_
#define _IOP_SERVER_H_
#include <time.h>
#include "iop_config.h"
#include "iop_def.h"
#include "iop.h"
#include "iop_util.h"
#include "array_list.h"
#include "iop_buf.h"
#include "iop_list.h"
#include "iop_hashmap.h"
#include "iop_thread.h"

#ifdef __cplusplus
extern "C" {
#endif




/*tcp�����¼��ص�����*/
typedef void (*iop_cb)(iop_base_t *,int,void *);
void iop_default_cb(iop_base_t *base, int id, void *arg);



/*
*   ����-1����Ҫɾ���¼�,����0����ɾ��
*/
typedef int (*iop_err_cb)(iop_base_t *,int,int,void *);

int iop_default_err_cb(iop_base_t *base, int id, int err, void *arg);


/************************************
*Э���������
* parameters:
*	char *buf:����
*	int len:���ݳ���
*return:
*	����0����Ҫ�ո��������Դ�������-1����Э�����>0��������ɹ�һ�����ݰ�
***********************************/
typedef int (*iop_parser)(char *, int);
int iop_default_parser(char *buf, int len);


/*
*���ݴ�����
*parameters:
*	base:iop_base_t ָ��
*	id:iop�����id
*	buf:���ݰ���ʼ��
*	len:���ݰ�����
*	arg:�Դ��Ĳ���
*return:
	-1: ����Ҫ�ر�����,0��������
*/
typedef int (*iop_processor)(iop_base_t *,int,char *,int,void *);


struct tag_iop_tcp_server_arg_t
{
	char host[64];
	unsigned short port;
	iop_cb on_connect;
	iop_cb on_destroy;
	iop_parser parser;
	iop_processor processor;
	iop_err_cb on_error;
	int timeout;
};
typedef struct tag_iop_tcp_server_arg_t iop_tcp_server_arg_t;


/***********************************
*function:
*	�����µ�tcp server������iop_base�С�
*parameters:
*	base:iop_base_tָ��
*	host:������ip
*	port:������port
*	parser:Э�������
*	processor:���ݴ�����
*	on_connect:�����Ӵ���ʱ�Ļص�
*	on_destory: �˳��¼��Ļص�
*	on_keepalive_tiimeout:��ʱ�¼��Ļص�
*	keepalive_tiimeout,keepalive��ʱ
*Return:
*	�ɹ�����>=0��id��ʧ�ܷ���-1.
*/
int iop_add_tcp_server(iop_base_t *base, const char *host, unsigned short port,
					   iop_parser parser, iop_processor processor,
					   iop_cb on_connect,iop_cb on_destroy,
					   iop_err_cb on_error, int keepalive_timeout);


int iop_add_t_server(iop_base_t *base, const char *host, unsigned short port,
					   iop_parser parser, iop_processor processor, int keepalive_timeout);




/*
*function:
*	�첽����
*parameters:
*	base:iop_base_tָ��
*	host:������host
*	port:�������˿�
*    timeout:��ʱʱ��
*    on_timeout:��ʱ�ص�
*    arg:�Զ������
*Return:
*     -1:ʧ��
*      0,���ӳɹ������ڳ�������
*      ����ʲô��������ն����ص�conn_cb.
*/
int iop_asyn_connect(iop_base_t *base, const char *host, unsigned short port,int timeout,iop_event_cb conn_cb, void *arg);


//int iop_http_get(const char *url, iop_event_cb cb,int timeout, void *arg);

#ifdef __cplusplus
}
#endif

#endif
