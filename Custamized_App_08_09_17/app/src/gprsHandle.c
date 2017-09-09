
//Include
#include "generalInfo.h"

//Local Macros
#define		RCVBUFSIZE		SIZE_2048
//Globals
U8		gprsAttached,gotPPPip,socketConeccted,socConSuccess104;
U8		tcpSockRcvBuff[SIZE_2048];
U8		sockRecvBuffer[SIZE_2048];
U8		tcpSockSendBuffer[SIZE_2048];
U8		udpSockSendBuffer[SIZE_2048];
U8		tmpTimeBuf[SIZE_32];
INT32	sockTcp,sockUdp;

//constant
char *postMsg = "POST /%s HTTP/1.0\r\nContent-Type: application/json\r\nContent-Length:%d\r\n\r\n%s";
char *jsonMsg = "{\"LON\":\"%lf\",\"LAT\":\"%lf\"}";

//Externs
// For watchdog do not use these variables for other purposes
extern unsigned long			wdtCount;

extern U32						tmpSize,tmpSlen;  // only used for strlen & sizeof
extern S8						atCmd[];
extern U32						tempSerRespLen,curSockBuffLen;
extern DEVICE_CONFIG_MODEM		modemConfig;

extern GPS_COR gps_Cordinates;

//Structure Variables
GAPP_TCPIP_ADDR_T				appUdpAddr;

//Function Definitions

/*******************************************
*Function 		: checkGprsAttached()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
int checkGprsAttached(void)
{
	int retry=3;
	while(retry)
	{
		#if	WDT_IMPROVED
			++wdtCount;
		#endif
		sendCommand("AT+CGATT?\r",1000);
		if(gprsAttached)
		{
			gprsAttached=0;
			return 0;
		}
		else
		{
			//sendCommand("AT+CGATT=0\r",1000);
			//delayInms(3000);
			//sendCommand("AT+CGATT=1\r",1000);
			retry--;
		}
	}
	if(retry == 0)
		return -1;
	
	return 0;
}
/*******************************************
*Function 		: getPppIP()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
int getPppIP(void)
{
	int ret = 1,retryGatt=0,retryGpppIp=0;
	U32 i=0;
	retryGatt = 3;
	while (retryGatt)
	{
		#if	WDT_IMPROVED
			++wdtCount;
		#endif
		ret = checkGprsAttached();
		if(ret != -1)
		{
			retryGpppIp = 3;
			while(retryGpppIp)
			{
				#if	WDT_IMPROVED
					++wdtCount;
				#endif
				memset(atCmd,0,SIZE_128);
				printf1("\rGPRS Attached\n");
				gotPPPip = 0;
				sprintf(atCmd,"AT+MIPCALL=1,\"%s\"\r",modemConfig.apnName);
				sendCommand(atCmd,1000);  
				for(i=0;i<180;i++)   /*Wait Maximum 180 sec to get ppp IP*/
				{
					#if	WDT_IMPROVED
						++wdtCount;
					#endif
					if(gotPPPip)
					{
						gotPPPip=0;
						break;
					}
					delayInms(1000);
				}
				if(i == 180)
				{
					printf1("\rpppIp Failed..%d\n",retryGpppIp);
					retryGpppIp--;
					delayInms(6000);
					continue;
				}
				else
				{
					#if	WDT_IMPROVED
						++wdtCount;
					#endif
					printf1("\rpppIp : %s",modemConfig.dynamicPPPip);
					//if udp debug is enabled create udp sock here
/* 					if(modemConfig.enDbgLog == 1)
						initUdpSocket(); */
					break;
				}
			}
			if(retryGpppIp == 0)
			{
				printf1("\rGet pppIp Error..!System Reboot..!\n");
				//System Reboot
				//powerCycle(GET_PPP_IP_FAILED);
				return -1;
			}
			else
				break;
		}
		else
		{
			#if	WDT_IMPROVED
				++wdtCount;
			#endif
			printf1("\rGPRS not attached..%d\n",retryGatt);
			deactivateGprs();
			retryGatt--;
		}
		delayInms(6000); 
		#if	WDT_IMPROVED
			++wdtCount;
		#endif
	}
	if(retryGatt == 0)
	{
		#if	WDT_IMPROVED
			++wdtCount;
		#endif
		printf1("\rGPRS attach Error..!System Reboot..!\n");
		//System Reboot
		//powerCycle(GPRS_ATT_FAILED);
		return -1;
	}
	return 0;
}

/*******************************************
*Function 		: getIpAddr()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
int getIpAddr(const char* cp, unsigned long *ip)
{
	UINT32	addr;
	UINT32	value;
	int		part;
		
	*ip = 0;
	addr = 0;
	for (part = 1; part <= 4; part++)
	{
		#if	WDT_IMPROVED
			++wdtCount;
		#endif
		if ((*cp < '0') || (*cp > '9'))
			goto err_ip;
		
		value = 0;
		while ((*cp >= '0') && (*cp <= '9')) 
		{
			value *= 10;
			value += *cp++ - '0';
			if (value > 255)
				goto err_ip;
		}
		
		if (part < 4) 
		{
			if (*cp++ != '.')
				goto err_ip;
		}
		else 
		{
			char c = *cp++;
			if (c != '\0' && c != ' ')
				goto err_ip;
		}
		
		//addr <<= 8;
		value <<= ( (part-1) * 8);
		addr |= value;
	}
	
	//	*ip = htonl(addr);
	*ip = addr;
	return 1;
	
	err_ip:
		*ip = 0;
	return 0;
}

/*******************************************
*Function 		: sockCreate()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
int sockCreate(char *ipStr,UINT16 port)
{
    UINT8 *pip;
	struct ip_addr addr = {0};
	GAPP_TCPIP_ADDR_T appTcpAddr;
    INT32  ret=0,i=0,retry=0;

	getIpAddr((const char *)ipStr,(unsigned long *)&addr);
    pip = (UINT8 *)&addr;

    printf1("\rsockCreate(): %d.%d.%d.%d\n",pip[0],pip[1],pip[2],pip[3]);
	
	retry = 3;
	while(retry)
	{
		#if	WDT_IMPROVED
			++wdtCount;
		#endif
		/* TCP mode */
		sockTcp = sys_sock_create(GAPP_IPPROTO_TCP);
		if(sockTcp < 0)
		{
			printf1("\rCreate TCP socket fail %d, ret:%d\n",retry,sockTcp); 
			closeSocket(sockTcp);
			delayInms(3000);
			retry --;
			continue;
		}
		appTcpAddr.sin_port = htons(port);  /* TCP port number */
		appTcpAddr.sin_addr = addr;
		for(i=0;i<10;i++)
		{
			#if	WDT_IMPROVED
				++wdtCount;
			#endif
			ret = sys_sock_connect(sockTcp,&appTcpAddr);
			if(ret < 0)
			{
				printf1("\rConnect TCP server failed, retry after 5 sec.., ret: %d\n",ret); 
				delayInms(3000);
				continue;
			}
			else
				break;
		}
		if(i >= 10)
		{
			printf1("\rConnect TCP server failed %d\n",retry);
			closeSocket(sockTcp);
			retry --;
			continue;
		}
		else
			printf1("\rConnecting TCP server  %d.%d.%d.%d : %d.",pip[0],pip[1],pip[2],pip[3],port); 
		
		for(i=0;i<30;i++)
		{
			#if	WDT_IMPROVED
				++wdtCount;
			#endif
			if(socketConeccted)
				break;
			printf1(".");
			delayInms(1000);
		}
		printf1("\n");
		if(i >= 30)
		{
			printf1("\rConnecting server failed..%d\n",retry);
			closeSocket(sockTcp);	
			retry --;
			delayInms(2000);
			continue;
		}
		else
		{
			#if	WDT_IMPROVED
				++wdtCount;
			#endif
			break;
		}
	}
	if(retry == 0)
	{
		//printf1("\rConnect TCP server Error..!System Reboot..!\n");
		//System Reboot
		//powerCycle(SOC_CON_FAILED);
		return -1;
	}
 
    return 0;
}

/*******************************************
*Function 		: sig_in()
*Params 		:  
*Return			:  
*Description 	: callback any signal comes
********************************************/
void  sig_in(GAPP_SIGNAL_ID_T signal,va_list arg)
{
	UINT32 sock,len;
	
	switch(signal)
	{
	    case GAPP_SIG_SOCK_CONNECT_RSP:/*tcp connect success event*/
	    {
	        /*send msg to TASK 1 , we handle socket event there*/
	        sock = va_arg(arg,UINT32);
			printf1("\n\rSocket Connected Success\n");
			socketConeccted = 1;
			SET_FLAG(TCP_SOCKET_CONECCTED);
	        //ret = sys_taskSend(TASK_ONE,T1_SOCK_CONNECT_RSP,sock,0,0);
	        va_end(arg);
	    }
	    break;
	    case GAPP_SIG_SOCK_ERROR_IND:/*socket error */
	    {
			printf1("\n\rSocket Error..!\n");
			closeSocket(sockTcp);
			socketConeccted = 0;
			CLR_FLAG(TCP_SOCKET_CONECCTED);	        
	    }
	    break;
	    case GAPP_SIG_SOCK_DATA_RECV_IND:/*peer data recving*/
	    {
	        sock = va_arg(arg,UINT32);
	        len  = va_arg(arg,UINT32);
			
			memset(tcpSockRcvBuff, 0, SIZE_2048);
			len = sys_sock_recv(sockTcp,tcpSockRcvBuff,SIZE_2048);
			if ( len <= 0)
			{
				printf1("\n\rTCP receive data fail, len:%d\n",len); 
				return;
			}
			//printf1("Soc Rec %d\n",len);
			sockRecv( (char *)tcpSockRcvBuff,len );
	        va_end(arg);	
	    }
	    break;
	    case GAPP_SIG_SOCK_CLOSE_IND:/*tcp peer close , maybe recv FIN*/ //3
	    {
			printf1("\n\rSocket Closed..!\n");
			closeSocket(sockTcp);
			socketConeccted = 0;
			CLR_FLAG(TCP_SOCKET_CONECCTED);	    
	    }
	    break;
	    case GAPP_SIG_SOCK_CLOSE_RSP:/*close respond */ //8
	    {
			printf1("\n\rSocket Res Closed..!\n");
			closeSocket(sockTcp);
			socketConeccted = 0;
			CLR_FLAG(TCP_SOCKET_CONECCTED);	 
	    }
		break;
		case GAPP_SIG_PDP_RELEASE_IND:
		{
			printf1("\n\rTCP Closed, Try to Reconnect...\n");
		}
	    break;	    
		case GAPP_SIG_SOCK_TCPXON_IND:
		{
			printf1("\n\rTCP flow control\n");
		}
		case GAPP_SIG_SOCK_ACCEPT_IND:
		{
			printf1("\n\rSocket Accept\n");
		}
	    default:
	    break;
	}
}

/*************************************
*Function 		: sockRecv()
*Params 		:  
*Return			:  
*Description 	:  
**************************************/
int sockRecv( char *data, int len )
{
	//pprintf("SR %d TL %d\n", len, curSockBuffLen);

	/**
			To prevent overflow of sockRecvBuffer and other related buffers.
		Note: Overflowing causes Modem Reset.
	**/
	if( (curSockBuffLen + len) > RCVBUFSIZE )
		len = RCVBUFSIZE - curSockBuffLen;

	if( len > 0 )
		memcpy( (void*)&sockRecvBuffer[curSockBuffLen], (const void*)data, len );

	curSockBuffLen += len;
	return 0;
}

/*******************************************
*Function 		: gprsStatus()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
int gprsStatus( void )
{
	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	gotPPPip = 0;
	sendCommand("AT+MIPCALL?\r",3000);
	if(gotPPPip)
	{
		return 1; //yes
	}
	else 
	{
		//deactivateGprs();
		return 0; //No
	}
}

/*******************************************
*Function 		: deactivateGprs()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
void deactivateGprs(void)
{
	printf1("\rIn deactivateGprs..\n");
	sendCommand("AT+MIPCALL=0\r",6000);
	return;
}

/*******************************************
*Function 		: closeSocket()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
void closeSocket(INT32 sock)
{
	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	sys_sock_close(sock);
}

/*************************************
*Function 		: send_packet()
*Params 		:  
*Return			:  
*Description 	:  
**************************************/
int sendPacket(const void* buf, unsigned int count)
{
	int	bytesSent=0;
	
	bytesSent = sys_sock_send(sockTcp,(UINT8 *)buf,(UINT16)count);

	return bytesSent;
}

/*******************************************
*Function 		: udpSockCreate()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
/* int udpSockCreate(char *ipStr,UINT16 port)
{
   // UINT8 *pip;
	struct ip_addr addr = {0};

	getIpAddr((const char *)ipStr,(unsigned long *)&addr.ip);
    //pip = (UINT8 *)&addr.ip;

   // printf1("\rudpSockCreate(): %d.%d.%d.%d\n",pip[0],pip[1],pip[2],pip[3]);
	
	appUdpAddr.sin_port = htons(port);  
	appUdpAddr.sin_addr = addr;	
	
	sockUdp = sys_sock_create(GAPP_IPPROTO_UDP, udpSocketHandler, NULL);
	if(sockUdp < 0)
	{
		//printf1("\rCreate UDP socket fail, ret:%d\n",sockUdp); 
		return -1;
	}
	printf1("\rCreate UDP socket succeed, ret:%d\n",sockUdp); 
	
    return 0;
} */

/*******************************************
*Function 		: udpSocketHandler()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
/* void udpSocketHandler(INT32 socket,GAPP_SOCK_EVE_T event,void *arg)
{
	//printf1("\rIn UDP Socket handler..\n");
	switch(event)
	{
		case GAPP_EVE_SOCK_DATA_RECV_IND:
		{	
			;//printf1("\rUDP received data..\n");
		}
		break;
		case GAPP_EVE_SOCK_WRITE_IND:
		{
			//printf1("\rUDP Socket write Success\n");
			SET_FLAG(UDP_SOCKET_CONECCTED);
		}
		break;
		default:
		break;
	}
}
 */
/*******************************************
*Function 		: initUdpSocket()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
/* void initUdpSocket(void)
{

	tmpSlen = strlen((const char*)modemConfig.debugLoggerIp);
	if( (tmpSlen< 7) || (tmpSlen>15) )
		readModemConfigFile();
	
	//printf1("\rDebug %s:%d\n", modemConfig.debugLoggerIp, modemConfig.debugLoggerPort);

	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	udpSockCreate((char*)modemConfig.debugLoggerIp,(UINT16)modemConfig.debugLoggerPort);

	delayInms(2000);
	return;
}
 */
/*******************************************
*Function 		: udpPrintf()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
/* int udpPrintf( const char *formatStr, ... )
{
	int 	ret=0;

	#if	WDT_IMPROVED
		++wdtCount;
	#endif

	if(CHECK_FLAG(UDP_SOCKET_CONECCTED))
	{
		sys_rtc_get_time(&rtcTime);	
		memset(tmpTimeBuf,0,SIZE_32);
		sprintf((char *)tmpTimeBuf,"%d/%d/%d %d:%d:%d", rtcTime.day,
											rtcTime.month,
											rtcTime.year,
											rtcTime.hour,
											rtcTime.minute,
											rtcTime.second);
		#if	WDT_IMPROVED
			++wdtCount;
		#endif
		
		memset(udpSockSendBuffer,0,SIZE_2048);
		sprintf( (char *)udpSockSendBuffer, "%s:%s %s\n", modemConfig.modemIMEINo, tmpTimeBuf, formatStr );

		ret = strlen((const char*)udpSockSendBuffer);
		if( sys_sock_send2(sockUdp,(UINT8 *)udpSockSendBuffer,ret,&appUdpAddr) < 0 )
		{
			CLR_FLAG(UDP_SOCKET_CONECCTED);
		}
	
		#if	WDT_IMPROVED
			++wdtCount;
		#endif
		delayInms(100);
	}
	else
	{
		//if ( (CHECK_FLAG(SYN_TIME_SUCCESS)) && ( gprsStatus() == 1 ) )
		if ( gprsStatus() == 1 )
			initUdpSocket();
	}	
	return ret;	
}
 */

//GPS DATA POST
int postGpsInfo(void)
{
	//char postDataBuf[SIZE_256];

	//socket create

	//write data 
	//memset(postDataBuf,0,SIZE_256);
	//sprintf(postDataBuf,"",gps_Cordinates.lattitude,gps_Cordinates.longitude);
	 //close socket
	return 0;
}
 /*** EOF ***/
