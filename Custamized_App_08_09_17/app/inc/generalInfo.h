#ifndef  __GENERAL_INFO__
#define  __GENERAL_INFO__

//Includes
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "sys_callback.h"
#include "sys_services.h"
#include "sys_ext.h"
#include "string.h"
#include "app.h"
#include "deviceConfig.h"
#include "Utilities.h"
#include "gps.h"

#pragma pack(1)

#define		FW_VERSION			"GPS_SMS_1.0_07SEP_2017"
#define		DEBUG_LOG			1
#define  	SMS_TEXT_MODE		1
#define		WDT_IMPROVED		1
//Macros
#define 	SIZE_4 		4
#define 	SIZE_8 		8
#define 	SIZE_16 	16
#define 	SIZE_32 	32
#define 	SIZE_64 	64
#define 	SIZE_128	128
#define 	SIZE_256 	256
#define 	SIZE_512 	512
#define 	SIZE_1024 	1024
#define 	SIZE_2048 	2048

/* Configuration Parameters Type */
#define			NUMERAL		1
#define			WEB_ADDR	2
#define			MOB_NUMB	3
#define			AUTH_STR	4
#define			DIR_STR		5
#define			ALPNUM_STR	6

#define 	ASCII_ESC 			27
//for Flags use only
extern U64  flag1;
#define	POWER_ON_FLAG 			0
#define	MODEM_READY 			1
#define	FOTA_STATUS_FLAG_WDT	2
#define	UDP_SOCKET_CONECCTED	3
#define	PING_SUCCESS			4
#define	PING_FAILURE			5
#define	TCP_SOCKET_CONECCTED	6
#define ECHO_DISABLE			7
#define GET_NUMERIC				8
#define CONFIG_EXIT				9
#define GPRS_CONNECTED			10
#define PANIC_SWITCH_ON			11

#define SET_FLAG(n) 	((flag1) |= (U64)(1ULL << (n)))
#define CLR_FLAG(n) 	((flag1) &= (U64)~((1ULL) << (n)))
#define CHECK_FLAG(n) 	((flag1) & (U64)(1ULL<<(n)))

#define strequsub(s1, s2) (strncmp(s1, s2, strlen(s1)) == 0)
// FCS related
#define     PPPINITFCS16    	0xFFFF  // Initial FCS value
#define     PPPGOODFCS16     	0xF0B8  // Good final FCS value

//structure
//Structure to handle AT response
typedef struct
{
	U8 	*atResBuf;
	U16	atResLen;	
}AT_HANDLER;

/* Function Proto */

/* app.c */
void uart_in(INT32 uid,UINT8 *data,UINT16 len);
void at_in(UINT8 *rsp,UINT16 rsplen);
void sig_in(GAPP_SIGNAL_ID_T sig,va_list arg);
int SetBaudRate(void);
int printf1(const char *format, ...);
int printf2(const char *format, ...);
void delayInms(U32 time);
int powerCycle(void);
int readInputChar(void);
void initGpioPins(void);
void startWdtTimer(void);


/* atHandle.c */
int chkSimRegistration(void);
int readSMS(void);
int sendCommand(const char *cmdstr,unsigned int delay);
void sendSMS(char *sBuff,char *toWhom);
void deleteIdxMsg(int index);
int ping(char *Ip);
int procReceivedSmsCmd(void);
int	validateUserInput( char	type, char* sBuff );
/* int isdigit (int c);
int isalpha(int c); */

	/** deviceConfig.c **/
int writeModemConfigFile(void);
int readModemConfigFile(void);
void fillDefaultModemConfig(void);

	/** gprsHandlers.c **/
int checkGprsAttached(void);
int getPppIP(void);
int getIpAddr(const char* cp, unsigned long *ip);
int sockCreate(char *ipStr,UINT16 port);
int gprsStatus(void);
void deactivateGprs(void);
/* void tcpSocketHandler(INT32 socket,GAPP_SOCK_EVE_T event,void *arg); */
void closeSocket(INT32 sock);
void initUdpSocket(void);
int udpPrintf( const char *formatStr, ... );
/* void udpSocketHandler(INT32 socket,GAPP_SOCK_EVE_T event,void *arg); */
int udpSockCreate(char *ipStr,UINT16 port);
int sendPacket(const void* buf, unsigned int count);
int sockRecv( char *data, int len );
unsigned short  pppfcs16 (unsigned short fcs, unsigned char *cp, int len);

#endif
/*** EOF ***/
