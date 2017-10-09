#ifndef _SYS_CALLBACK_H_
#define _SYS_CALLBACK_H_

#include "cs_types.h"
#include "stdarg.h"
#include "sys_services.h"

typedef enum
{
	GAPP_SIG_PDP_RELEASE_IND,
	GAPP_SIG_SOCK_CONNECT_RSP,
	GAPP_SIG_SOCK_TCPXON_IND,
	GAPP_SIG_SOCK_CLOSE_IND,
	GAPP_SIG_SOCK_ERROR_IND,
	GAPP_SIG_SOCK_ACCEPT_IND,
	GAPP_SIG_SOCK_CLOSE_WITH_FIN_RSP,
	GAPP_SIG_SOCK_SEND_RSP,
	GAPP_SIG_SOCK_CLOSE_RSP,
	GAPP_SIG_SOCK_DATA_RECV_IND,
}GAPP_SIGNAL_ID_T;

typedef struct
{
	UINT8 (*init)(GAPP_TASK_T **ptl);
    void  (*uart_input)(INT32 uid,UINT8 *data,UINT16 len);
    void  (*at_resp)(UINT8 *rsp,UINT16 rsplen);
    void  (*sys_signal)(GAPP_SIGNAL_ID_T sig,va_list arg);
}SYS_CALLBACK_T;

extern SYS_CALLBACK_T sys_callback;
#endif
