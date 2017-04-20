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
#ifndef _IOP_H_
#define _IOP_H_
#include <time.h>
#include "iop_def.h"
#include "iop_config.h"
#include "iop_util.h"
#include "array_list.h"
#include "iop_list.h"
#include "iop_buf.h"
#include "iop_thread.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************
*function:
*	�����µ�iop_base_t,linux��ʹ��epollģ�ͣ�windwos��ʹ��selectģ��
*parameters:
*	maxio:���֧�ֵĲ���io��
*Return:
*	�ɹ�����iop_base_t����ָ�룬ʧ�ܷ���NULL.
*/
iop_base_t* iop_base_new(int maxio);



/***************************************
*function:
*	�����µ�iop_base_t,ʹ���û�ָ����ģ��
*parameters:
*	maxio:���֧�ֵĲ���io��
*	model:�¼�ģ��, select/poll/epoll/timer, �ݲ�֧��iocp
*Return:
*	�ɹ�����iop_base_t����ָ�룬ʧ�ܷ���NULL.
*/
iop_base_t* iop_base_new_special(int maxio,const char *model);


/***********************************
*function:
*	�ͷ�iop_base_t
*parameters:
*	base:iop_base_tָ��
*Return:
*	��
*/
void iop_base_free(iop_base_t *base);


/***********************************
*function:
*	����һ���¼�����
*parameters:
*	base:iop_base_tָ��
*Return:
*	���δ�����¼�����
*/
int iop_dispatch(iop_base_t *base);


/*
*function:
*	����ѭ���¼����ȣ�ֱ���˳�
*parameters:
*	base:iop_base_tָ��
*Return:
*	��
*/
void iop_run(iop_base_t *base);


/*
*function:
*	�����߳�������ѭ���¼����ȣ�ֱ���˳���
*	��Ӧ��base�������߳��˳�ʱ���Զ�����
*parameters:
*	base:iop_base_tָ��
*	thread:���ص��߳�id.
*Return:
*	0:�ɹ���-1��ʧ��
*/
int iop_run_in_thread(iop_base_t *base,iop_thread_t *thread);


/*
*function:
*	�˳�ѭ���¼�����
*parameters:
*	base:iop_base_tָ��
*Return:
*	��
*/
void iop_stop(iop_base_t *base);


/*
*function:
*	����һ���µ��¼�����iop_base_t��
*parameters:
*	base:iop_base_tָ��
*	handle:socket���������Ƕ�ʱ������ΪINVALID_HANDLE
*	events:��ע���¼�
*	evcb:�¼��ص�����
*	arg:�û�ָ���Ĳ���,�û������ͷţ�һ����event_cb�У�EV_TYPE_DESTROY�¼����ͷ�
*	timeout:��ʱֵ���룩��-1����������ʱ
*Return:
*	�ɹ����ض���id,ʧ�ܷ���-1
*/
int iop_add(iop_base_t *base,io_handle_t handle,unsigned int events,iop_event_cb evcb,void *arg,int timeout);



/*
*function:
*	����һ���µĶ�ʱ���¼���iop_base_t��
*parameters:
*	base:iop_base_tָ��
*	evcb:�¼��ص�����
*	arg:�û�ָ���Ĳ���,�û������ͷţ�һ����event_cb�У�EV_TYPE_DESTROY�¼����ͷ�
*	timeout:��ʱֵ���룩��-1����������ʱ
*Return:
*	�ɹ����ض���id,ʧ�ܷ���-1
*/
int iop_add_timer(iop_base_t *base,iop_event_cb evcb,void *arg,int timeout);





/*
*function:
*	��iop_base_t��ɾ��ָ��id���¼�����
*parameters:
*	base:iop_base_tָ��
*	id:�¼�����id
*Return:
*	�ɹ�����0,ʧ�ܷ���-1
*/
int iop_del(iop_base_t *base,int id);



/*
*function:
*	��iop_base_t�в���ָ��id���¼�����,һ������²�Ҫֱ���޸�iop_t����
*parameters:
*	base:iop_base_tָ��
*	id:�¼�����id
*Return:
*	�ɹ�����iop_tָ��,ʧ�ܷ���NULL
*/
iop_t* iop_get(iop_base_t *base,int id);



/*
*function:
*	�޸Ĺ�ע���¼�
*parameters:
*	base:iop_base_tָ��
*	id:iop����id
*	events:�¹�ע���¼�
*Return:
*	�ɹ�����0,ʧ�ܷ���-1
*/
int iop_mod_events(iop_base_t *base,int id,unsigned int events);


/*
*function:
*	�������ݣ���������������ͣ��򱻼��뻺����У��ȵ��������ʱ�Զ�����
*parameters:
*	base:iop_base_tָ��
*	id:iop����id
*	data:Ҫ���͵�����
*	len:Ҫ���͵����ݳ���
*	events:�¹�ע���¼�
*Return:
*	�ɹ�����0,ʧ�ܷ���<0,  IOP_ERR_SOCKET:������� IOP_ERR_NO_MEM���ڴ��쳣
*/
int iop_buf_send(iop_base_t *base,int id,const void *data,int len);
/*
*function:
*	�������ݲ����뻺����
*parameters:
*	base:iop_base_tָ��
*	id:iop����id
*Return:
*	�ɹ�����>=0,ʧ��<0 ����IOP_ERR_SOCKET:������� IOP_ERR_NO_MEM���ڴ��쳣
*/
int iop_buf_recv(iop_base_t *base, int id);


/*
*function:
*	�����Զ������
*parameters:
*	base:iop_base_tָ��
*	id:iop����id
*	arg:�Զ������
*Return:
*/
void iop_set_arg(iop_base_t *base, int id,void *arg);




#ifdef __cplusplus
}
#endif

#endif

