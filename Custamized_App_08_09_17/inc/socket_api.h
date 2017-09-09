/******************************************************************************
  Copyright (C), 2010, Shenzhen G&T Industrial Development Co.,Ltd

  File:        socket_api.h
  Author:	  shumin
  Version:  1.0
  Date:      2009.5
  
  Description:    TCP/IP API define 
******************************************************************************/
#ifndef  _COMMAND_SOCKET
#define  _COMMAND_SOCKET

#define  DEST_URL_LEN	64
#define  AT_BUF_SIZE		1024

#define  MAX_SOCKET		4

#define  AUTO_SOCK		0			/* transparent socket */ 
#define  CTRL_SOCK		1
#define  SELF_SOCK		2

#if 0
#define  FTP_DATA_SOCK		3		/* for ftp data transfer */ 
#define  FTP_LISTEN_SOCK	4		/* for ftp listen */ 
#define  FTP_CONTROL_SOCK	5		/* for ftp control command */ 
#endif

#define  AT_TCP			0
#define  AT_UDP			1

typedef enum
{
    EVENT_SOCKET_LINK = 0,		/* connect server OK */ 
    EVENT_SOCKET_CLOSE,		/* connect closed by myself */ 
    EVENT_SOCKET_BREAK,		/* connect closed by server */ 
    EVENT_SOCKET_FAIL,			/* connect server failure */ 
    EVENT_SOCKET_ERROR,		/* socket parameter error */ 

    EVENT_SEND_OK,				/* send data (to lwip buffer) ok */ 

    EVENT_PPP_ACTIVE,			/* PDP actived */ 
    EVENT_PPP_DEACTIVE,			/* PDP deactived */ 
    EVENT_PPP_BREAK,			/* PDP deactive by accident */ 
    
    EVENT_SOCKET_RECV,			/* data received from internet */ 

    EVENT_LISTEN_OK,				/* 10 listen OK */ 
    EVENT_LISTEN_ERROR,			/* */ 
    EVENT_LISTEN_CLOSE,			
    EVENT_ACCEPT_OK,
    EVENT_ACCEPT_FULL,
    EVENT_ACCEPT_ERROR,
    
    EVENT_TCP_ACKED,
	EVENT_COMMAND_ERROR
} NET_EVENT;

//typedef int (*recv_fun)(int, char *, int);
typedef int (*recv_fun)(int, char *, int, char *, unsigned short);

int  pdp_active(char *apn, char *name, char *pwd);
int  pdp_deactive(void);
int  pdp_get_status(void);
unsigned long pdp_get_my_ip(void);

int socket_SetServer(int sock, char *url, unsigned int lport, unsigned int rport, int proto);
int socket_Connect(int sock, recv_fun recvproc);
int socket_Close(int sock);
int socket_Send(int sock, char *data, int len);

int socket_StartListen(unsigned short port, recv_fun recvproc);
int socket_StopListen(void);

int socket_is_conn(int sock);
unsigned long socket_GetRecv(int sock);
unsigned long socket_GetSend(int sock);
unsigned long socket_GetAcked(int sock);
void net_SetDebug(char id);

int socket_DConnect(int sock, recv_fun recvproc);
int socket_DClose(int sock);
int socket_DSend(int sock, char *data, int len);

int SetUserDNS(char *dns1, char *dns2);
char *GetIPByName(char *url);

int socket_SendTo(int sock, char *data, int len, char *dstAddr, unsigned short port);
int socket_DSendTo(int sock, char *data, int len, char *dstAddr, unsigned short port);

char *socket_get_dst_ip(int sock);
unsigned short socket_get_dst_port(int sock);

#endif  /* _COMMAND_SOCKET */

