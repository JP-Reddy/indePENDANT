
//Include
#include "generalInfo.h"

//Local Macros
//Globals

S8				smsSendBuff[SIZE_512];
S8				tmpBuff1[SIZE_64];
U8				pppIp[SIZE_32];
U8				simRegistered,lowSigStrength,lowSigStrength;
S32				network,modemTemperature,signalStrength;
U8				gprsAttached,gotPPPip,sigStrength;
char 			smsWriteStr[SIZE_512];
char			smsSenderMob[SIZE_32];
char 			smsReadStr[SIZE_256];
char 			smsReadBuff[SIZE_256];

//Externs
// For watchdog do not use these variables for other purposes
extern unsigned long			wdtCount;

extern S8						atCmd[];
extern U32						tmpSize,tmpSlen;  // only used for strlen & sizeof
extern char 					smsMsgBuffer[];
extern DEVICE_CONFIG_MODEM		modemConfig;

//Structure Variables
AT_HANDLER						*atHandler;
//Function Definitions

/*************************************
*Function 		: chkSimRegistration()
*Params 		: 
*Return			: 
*Description 	: 
**************************************/
int chkSimRegistration(void)
{
	U32 i=0;  

	sendCommand("AT\r",1000);
	for(i=0;i<2500;i++)
	{
		#if	WDT_IMPROVED
			++wdtCount;
		#endif
		sendCommand("AT+CSQ\r",1000);
		if ( CHECK_FLAG(MODEM_READY) )
			break;
		
		delayInms(3000);
		#if	WDT_IMPROVED
			++wdtCount;
		#endif
	}
	if(i >= 2500)
	{
		printf1("\rModem not Ready..System Reboot..!\n");
		powerCycle();
		//reboot
	}
	for(i=0;i<25;i++)
	{
		#if	WDT_IMPROVED
			++wdtCount;
		#endif
		sendCommand("AT+CREG?\r",1000);
		if(simRegistered)
			break;

		delayInms(3000);
		#if	WDT_IMPROVED
			++wdtCount;
		#endif
	}
	if(i >= 25)
	{
		printf1("\rSim Register Failed..System Reboot..!\n");
		powerCycle();
		//reboot
	}
	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	/* sendCommand("AT+XREG?\r",1000); */
	//IMEI
	sendCommand("AT+CGSN?\r",1000); 
	//IMSI
	sendCommand("AT+CIMI\r",1000); 
	//ICCID
	sendCommand("AT+CCID?\r",2000);
	//sendCommand("ATI8\r",1000);
	/* sendCommand("AT+MTSM=1\r",1000); */
	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	return 0;
}

/*************************************
*Function 		: sendCommand()
*Params 		: 
*Return			: 
*Description 	: 
**************************************/
int sendCommand(const char *cmdstr,unsigned int delay)
{
	int ret = 1;
    UINT16 len = strlen(cmdstr);
	//printf1("\rAT sent:%s\n",cmdstr);
	ret = sys_at_send((UINT8*)cmdstr,len);
	if(ret < 0 )
	{
		printf1("\rAT send Error ..! 1\n");
		return -1;
	}
	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	delayInms(delay);
	return 0;
}

/*******************************************
*Function 		: at_in()
*Params 		:  
*Return			:  
*Description 	:  callback when modem give 
				   AT response
********************************************/
void  at_in(UINT8 *data,UINT16 dataLen)
{
	S8 		*cPtr=NULL,*tcPtr=NULL;
	U8 		sStrnth[5],sentCnt=0,recCnt=0;
	U32 	i=0,ptrDiff=0;
	
	atHandler = sys_malloc(sizeof(AT_HANDLER) + dataLen + 1);
    if(atHandler)
    {
        atHandler->atResBuf = (UINT8*)(atHandler + 1);
        atHandler->atResLen = dataLen;
        memcpy(atHandler->atResBuf,data,dataLen);
        atHandler->atResBuf[dataLen] = '\0';
	}
	#if	WDT_IMPROVED
		++wdtCount;
	#endif

	if(dataLen != 0)
	{
		//printf1("\rAt Res: %s\n",(const char *)atHandler->atResBuf);
		if( strstr((const char *)atHandler->atResBuf,"+CREG:") )
		{
			cPtr = strchr((const char *)atHandler->atResBuf,',');
			cPtr +=1;
			if (*cPtr == '1' || *cPtr == '5')
			{
				simRegistered = 1;
				printf1("\rSIM Registered\n");
			}
			else
			{
				printf1("\rSIM NOT Registered..!\n");
				simRegistered = 0;
			}
		}
		else if( strstr((const char *)atHandler->atResBuf,"+CSQ:") )
		{
			cPtr = strchr((const char *)atHandler->atResBuf,':');
			tcPtr = strchr((const char *)atHandler->atResBuf,',');
			ptrDiff = (tcPtr - cPtr) - 1;
			memset(sStrnth,0,5);
			cPtr += 2;
			memcpy(sStrnth,cPtr,ptrDiff);
			sigStrength = atoi((const char *)sStrnth);
			if ( (sigStrength < 10) && (sigStrength > 5) )
			{
				printf1("\rLow Signal Strength: %d\n",sigStrength);
				signalStrength = sigStrength;
				lowSigStrength =1;
				SET_FLAG(MODEM_READY);
			}
			else if (sigStrength > 32)
			{
				printf1("\rModem Not Ready.. Signal Strength: %d\n",sigStrength);
				CLR_FLAG(MODEM_READY);
			}
			else if(sigStrength < 5)
			{
				printf1("\rUnhealthy Signal .. SS: %d\n",sigStrength);
				signalStrength = sigStrength;
				CLR_FLAG(MODEM_READY);
			}
			else
			{
				lowSigStrength =0;
				SET_FLAG(MODEM_READY);
				printf1("\rSignal Strength: %d\n",sigStrength);
				signalStrength = sigStrength;
			}
		}
		else if( strstr((const char *)atHandler->atResBuf,"+XREG:") )
		{
			cPtr = strstr((const char *)atHandler->atResBuf,"+XREG:");
			cPtr +=9;
			memset(sStrnth,0,5);
			sStrnth[0] = *cPtr;
			cPtr+=1;
			network = atoi((const char *)sStrnth);
			if ( network < 4)
			{
				printf1("\rSIM Registered in 2G Network %d\n",atoi((const char *)sStrnth));
			}
			else 
			{
				printf1("\rSIM Registered in 3G Network %d\n",atoi((const char *)sStrnth));
			}	
		}
		else if( strstr((const char *)atHandler->atResBuf,"+CGSN:") )
		{
			cPtr = strstr((const char *)atHandler->atResBuf,"+CGSN:");
			i=0;
			memset(tmpBuff1,0,SIZE_64);
			cPtr +=8;
			while(*cPtr != '"')
			{
				tmpBuff1[i++] = *cPtr;
				cPtr++;
			}
			tmpBuff1[i]='\0';
			tmpSlen = strlen((const char *)tmpBuff1);
			memcpy((void *)modemConfig.modemIMEINo,(const void *)tmpBuff1,tmpSlen);
			printf1("\rIMEI No: %s\n",modemConfig.modemIMEINo);
		}
		else if( strstr((const char *)atHandler->atResBuf,"+CCID:") )
		{
			cPtr = strstr((const char *)atHandler->atResBuf,"+CCID:");
			i=0;
			memset(tmpBuff1,0,SIZE_64);
			cPtr +=7;
			while( (*cPtr != '\0') && (*cPtr != '"') )
			{
				tmpBuff1[i++] = *cPtr;
				cPtr++;
			}
			tmpBuff1[i]='\0';
			tmpSlen = strlen((const char *)tmpBuff1);
			memcpy((void *)modemConfig.ccid,(const void *)tmpBuff1,tmpSlen);
			printf1("\rICID No: %s\n",modemConfig.ccid);
		}
		else if( strstr((const char *)atHandler->atResBuf,"+CIMI:") )
		{
			cPtr = strstr((const char *)atHandler->atResBuf,"+CIMI:");
			i=0;
			memset(tmpBuff1,0,SIZE_64);
			cPtr +=7;
			while(*cPtr != '\0')
			{
				tmpBuff1[i++] = *cPtr;
				cPtr++;
			}
			tmpBuff1[i]='\0';
			tmpSlen = strlen((const char *)tmpBuff1);
			memcpy((void *)modemConfig.imsi,(const void *)tmpBuff1,tmpSlen);
			printf1("\rIMSI No: %s",modemConfig.imsi);
		}
		else if( strstr((const char *)atHandler->atResBuf,"+CGATT:") )
		{
			cPtr = strstr((const char *)atHandler->atResBuf,"+CGATT:");
			cPtr += 8;
			if(*cPtr == '1')
				gprsAttached = 1;
			else
				gprsAttached = 0;
		}
		else if( strstr((const char *)atHandler->atResBuf,"+MIPCALL:") )
		{
			if( strstr((const char *)atHandler->atResBuf,"+MIPCALL: 0") )
				gotPPPip = 0;
			else if( strstr((const char *)atHandler->atResBuf,"+MIPCALL: 1,") )
			{
				cPtr = strstr((const char *)atHandler->atResBuf,"+MIPCALL: 1,");
				cPtr += 12;
				i=0;
				memset(pppIp,0,SIZE_32);
				while( *cPtr != '\0')
				{
					pppIp[i++] = *cPtr;	
					cPtr++;
				}
				pppIp[i]='\0';
				gotPPPip = 1;
			}
			else
			{
				cPtr = strstr((const char *)atHandler->atResBuf,"+MIPCALL:");
				cPtr += 10;
				i=0;
				while( *cPtr != '\0')
				{
					pppIp[i++] = *cPtr;	
					cPtr++;
				}
				pppIp[i]='\0';
				gotPPPip = 1;
			}
			tmpSlen = strlen((const char *)pppIp);
			memcpy((void *)modemConfig.dynamicPPPip,(const void *)pppIp,tmpSlen);
		}
		else if( strstr((const char *)atHandler->atResBuf,">") )
		{
			#if	WDT_IMPROVED
				++wdtCount;
			#endif
			sendCommand(smsSendBuff,1000);
			
		}
		else if( strstr((const char *)atHandler->atResBuf,"+CMGR") )
		{
			//printf1((const char *)atHandler->atResBuf);
			memset(smsMsgBuffer,0,SIZE_512);
			memcpy((void *)smsMsgBuffer,(const void *)atHandler->atResBuf,atHandler->atResLen);
		}
		else if( strstr((const char *)atHandler->atResBuf,"+MTSM:") )
		{
			cPtr = strchr((const char *)atHandler->atResBuf,':');
			memset(sStrnth,0,5);
			cPtr += 1;
			i=0;
			while(*cPtr != '\0')
			{
				sStrnth[i++] = *cPtr;
				cPtr++;
			}
			modemTemperature = atoi((const char *)sStrnth);
			printf1("\rModem Temperature %d\n",modemTemperature);
		}
		else if( strstr((const char *)atHandler->atResBuf,"+MPINGSTAT: 0,") )
		{
 			cPtr = strstr((const char *)atHandler->atResBuf,"\",");
			cPtr += 2;
			sentCnt = *cPtr - '0';
			cPtr += 2;
			recCnt = *cPtr - '0';
			if(sentCnt == recCnt)
				SET_FLAG(PING_SUCCESS);
			else
				SET_FLAG(PING_FAILURE);
 		}
		else if( strstr((const char *)atHandler->atResBuf,"+MPINGSTAT: 1,") )
			SET_FLAG(PING_FAILURE);
	}
	else
		printf1("\rIncomplete AT Response..!\n");
	sys_free((void *)atHandler);
	#if	WDT_IMPROVED
		++wdtCount;
	#endif

	return;
}

/*************************************
*Function 		: sendSMS()
*Params 		: 
*Return			: 
*Description 	: 
**************************************/
 void sendSMS(char *sBuff,char *toWhom)
{
	U16 len = 0;
	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	//set sms format
	memset(atCmd,0,SIZE_128);
	sprintf(atCmd,"AT+CMGF=%d\r",SMS_TEXT_MODE);
	sendCommand(atCmd,1000);
	//send sms
	tmpSlen = strlen(sBuff);
	memset(smsSendBuff,0,SIZE_512);
	memcpy(smsSendBuff,sBuff,tmpSlen);
	len = strlen(smsSendBuff);
	smsSendBuff[len] = 0x1A;
	smsSendBuff[len+1] = '\0';
	memset(atCmd,0,SIZE_128);
	sprintf(atCmd,"AT+CMGS=\"%s\"\r",toWhom);
	sendCommand(atCmd,1000);
	delayInms(5000); 
	return;
}
/*************************************
*Function 		: readSMS()
*Params 		: 
*Return			: 
*Description 	: 
**************************************/
int readSMS(void)
{
	U16 index=0;
	
	//set sms format
	memset(atCmd,0,SIZE_128);
	sprintf(atCmd,"AT+CMGF=%d\r",SMS_TEXT_MODE);
	sendCommand(atCmd,1000);

	for(index=1;index<=10;index++)
	{
		#if	WDT_IMPROVED
			++wdtCount;
		#endif
		memset(atCmd,0,SIZE_128);
		sprintf(atCmd, "AT+CMGR=%d\r", index);
		sendCommand(atCmd,1000);
		tmpSlen = strlen((const char *)smsMsgBuffer);
		if(tmpSlen > 15)
		{
			deleteIdxMsg(index);
			procReceivedSmsCmd();
		}
		delayInms(200);
		#if	WDT_IMPROVED
			++wdtCount;
		#endif
	}
	return 0;
}

/*************************************
*Function 		: procReceivedSmsCmd()
*Params 		: 
*Return			: 
*Description 	: 
**************************************/
int procReceivedSmsCmd(void)
{
	int				i=0, ret=0;
	char			*cPtr=NULL;
	char			*smsChPtr=NULL;

	//printf1("\rsmsBuffer: %s\n",smsMsgBuffer);
	#if	WDT_IMPROVED
		++wdtCount;
	#endif

	if( (cPtr=strchr(smsMsgBuffer, ',')) != NULL )	// Extracting Sender Mobile Number
	{
		cPtr += 2;
		memset((void*)smsSenderMob,0,SIZE_32);
		//smsSenderMob[0] = '+';
		for( i=0; (*cPtr)!='"'; cPtr++, i++)
			smsSenderMob[i] = *cPtr;
		ret = validateUserInput(MOB_NUMB,smsSenderMob);
		if(ret < 0)
		{
			printf1("\rSender Number Invalid - Ignore this Sms %s\n", smsSenderMob);
			return -1;
		}
		printf1("\rSender No:%s\n", smsSenderMob);
	}
	if( (cPtr=strchr(cPtr,'\n')) != NULL )	// Extracting Text Message
	{
		cPtr++;
		i=0;
		memset(smsReadStr,0,SIZE_256);
		while( *cPtr != '\r')
		{
			smsReadStr[i++] = *cPtr;	
			cPtr++;
		}
		printf1("\rsms :%s\n",smsReadStr);
	} 
	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	/*-----------------------*/
	if( strstr( smsReadStr, "GET PROFILE") )
	{
		memset((void*)smsWriteStr,0,SIZE_512);
		sprintf(smsWriteStr, "%s\n%s\n%s\n%s\n%s\n%ld\n%s\n%ld",modemConfig.apnName,modemConfig.gprsDialNum,modemConfig.username,
														modemConfig.passwd,modemConfig.serverIP,modemConfig.serverPort,modemConfig.publicPingIP,modemConfig.baudRate);
		sendSMS(smsWriteStr,smsSenderMob);
		delayInms(10000);
	}
	else if( strstr( smsReadStr, "GPRS APN" ) )
	{
		if( (smsChPtr=strchr(smsReadStr, 'N')) != NULL )
		{
			smsChPtr += 2;
			for( i=0; ((*smsChPtr)!=0x00)&&((*smsChPtr)!=0x0D); smsChPtr++, i++)
				smsReadBuff[i] = *smsChPtr;
		}
		if( (i=validateUserInput( AUTH_STR, smsReadBuff )) < 0 )
		{
			memset((void*)smsWriteStr,0,SIZE_512);
			sprintf(smsWriteStr,"Invalid Input");
			sendSMS(smsWriteStr,smsSenderMob);
			delayInms(10000);
			return i;
		}
		else
		{
			memset((void*)modemConfig.apnName, 0, 32);
			sprintf(modemConfig.apnName, "%s", smsReadBuff);
			printf1("\rGPRS apn changed - %s",modemConfig.apnName);
		}
		writeModemConfigFile();
	}
	else if( strstr( smsReadStr, "GPRS DIAL" ) )
	{
		if( (smsChPtr=strchr(smsReadStr, 'L')) != NULL )
		{
			smsChPtr += 2;
			for( i=0; ((*smsChPtr)!=0x00)&&((*smsChPtr)!=0x0D); smsChPtr++, i++)
				smsReadBuff[i] = *smsChPtr;
		}
		if( (i=validateUserInput( AUTH_STR, smsReadBuff )) < 0 )
		{
			memset((void*)smsWriteStr,0,SIZE_512);
			sprintf(smsWriteStr,"Invalid Input");
			sendSMS(smsWriteStr,smsSenderMob);
			delayInms(10000);
			return i;
		}
		else
		{
			memset((void*)modemConfig.gprsDialNum, 0, 16);
			sprintf(modemConfig.gprsDialNum, "%s", smsReadBuff);
			printf1("\rGPRS DialNo changed - %s",modemConfig.gprsDialNum);
		}
		writeModemConfigFile();
	}
	else if( strstr( smsReadStr, "GPRS UNAME" ) )
	{
		if( (smsChPtr=strchr(smsReadStr, 'E')) != NULL )
		{
			smsChPtr += 2;
			for( i=0; ((*smsChPtr)!=0x00)&&((*smsChPtr)!=0x0D); smsChPtr++, i++)
				smsReadBuff[i] = *smsChPtr;
		}
		if( (i=validateUserInput( AUTH_STR, smsReadBuff )) < 0 )
		{
			memset((void*)smsWriteStr,0,SIZE_512);
			sprintf(smsWriteStr,"Invalid Input");
			sendSMS(smsWriteStr,smsSenderMob);
			delayInms(10000);
			return i;
		}
		else
		{
			memset((void*)modemConfig.username, 0, 32);
			sprintf(modemConfig.username, "%s", smsReadBuff);
			printf1("\rGPRS Uname changed - %s",modemConfig.username);
		}
		writeModemConfigFile();
	}
	else if( strstr( smsReadStr, "GPRS PASS" ) )
	{
		if( (smsChPtr=strstr(smsReadStr, "SS")) != NULL )
		{
			smsChPtr += 3;
			for( i=0; ((*smsChPtr)!=0x00)&&((*smsChPtr)!=0x0D); smsChPtr++, i++)
				smsReadBuff[i] = *smsChPtr;
		}
		if( (i=validateUserInput( AUTH_STR, smsReadBuff )) < 0 )
		{
			memset((void*)smsWriteStr,0,SIZE_512);
			sprintf(smsWriteStr,"Invalid Input");
			sendSMS(smsWriteStr,smsSenderMob);
			delayInms(10000);
			return i;
		}
		else
		{
			memset((void*)modemConfig.passwd, 0, 32);
			sprintf(modemConfig.passwd, "%s", smsReadBuff);
			printf1("\rGPRS Pasword changed - %s",modemConfig.passwd);
		}
		writeModemConfigFile();
	}
	else if( strstr( smsReadStr, "TCPSER IP" ) )
	{
		if( (smsChPtr=strstr(smsReadStr, "IP")) != NULL )
		{
			smsChPtr += 3;
			for( i=0; ((*smsChPtr)!=0x00)&&((*smsChPtr)!=0x0D); smsChPtr++, i++)
				smsReadBuff[i] = *smsChPtr;
		}
		if( (i=validateUserInput( WEB_ADDR, smsReadBuff )) < 0 )
		{
			memset((void*)smsWriteStr,0,SIZE_512);
			sprintf(smsWriteStr,"Invalid Input");
			sendSMS(smsWriteStr,smsSenderMob);
			delayInms(10000);
			return i;
		}
		else
		{
			memset((void*)modemConfig.serverIP, 0, 32);
			sprintf(modemConfig.serverIP, "%s", smsReadBuff);
			printf1("\rTCP Server IP changed - %s", modemConfig.serverIP);
		}
		writeModemConfigFile();
	}
	else if( strstr( smsReadStr, "TCPSER PORT") )
	{
		if( (smsChPtr=strstr(smsReadStr, "PORT")) != NULL )
		{
			smsChPtr += 5;
			for( i=0; ((*smsChPtr)!=0x00)&&((*smsChPtr)!=0x0D); smsChPtr++, i++)
				smsReadBuff[i] = *smsChPtr;
		}
		if( (i=validateUserInput( NUMERAL, smsReadBuff )) < 0 )
		{
			memset((void*)smsWriteStr,0,SIZE_512);
			sprintf(smsWriteStr,"Invalid Input");
			sendSMS(smsWriteStr,smsSenderMob);
			delayInms(10000);
			return i;
		}
		else
		{
			modemConfig.serverPort = 8710;
			modemConfig.serverPort = atoi( smsReadBuff );
			printf1("\rTCP Port changed - %d", modemConfig.serverPort);
		}
		writeModemConfigFile();
	}
	else if( strstr( smsReadStr, "PING IP" ) )
	{
		if( (smsChPtr=strstr(smsReadStr, "IP")) != NULL )
		{
			smsChPtr += 3;
			for( i=0; ((*smsChPtr)!=0x00)&&((*smsChPtr)!=0x0D); smsChPtr++, i++)
				smsReadBuff[i] = *smsChPtr;
		}
		if( (i=validateUserInput( WEB_ADDR, smsReadBuff )) < 0 )
		{
			memset((void*)smsWriteStr,0,SIZE_512);
			sprintf(smsWriteStr,"Invalid Input");
			sendSMS(smsWriteStr,smsSenderMob);
			delayInms(10000);
			return i;
		}
		else
		{
			memset((void*)modemConfig.publicPingIP, 0, 32);
			sprintf(modemConfig.publicPingIP, "%s", smsReadBuff);
			printf1("\rPing IP changed - %s", modemConfig.publicPingIP);
		}
		writeModemConfigFile();
	}
	else if( strstr( smsReadStr, "UART BR") )
	{
		if( (smsChPtr=strstr(smsReadStr, "BR")) != NULL )
		{
			smsChPtr += 3;
			for( i=0; ((*smsChPtr)!=0x00)&&((*smsChPtr)!=0x0D); smsChPtr++, i++)
				smsReadBuff[i] = *smsChPtr;
		}
		if( (i=validateUserInput( NUMERAL, smsReadBuff )) < 0 )
		{
			memset((void*)smsWriteStr,0,SIZE_512);
			sprintf(smsWriteStr,"Invalid Input");
			sendSMS(smsWriteStr,smsSenderMob);
			delayInms(10000);
			return i;
		}
		else
		{
			modemConfig.baudRate = 9600;
			modemConfig.baudRate = atoi( smsReadBuff );
			printf1("\rUart Baudrate changed - %d", modemConfig.baudRate);
		}
		writeModemConfigFile();
	}
	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	return 0;
}

/*************************************
*Function 		: deleteIdxMsg()
*Params 		: 
*Return			: 
*Description 	: 
**************************************/
void deleteIdxMsg(int index)
{
	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	memset(atCmd,0,SIZE_128);
	sprintf(atCmd,"AT+CMGD=%d\r",index);
	sendCommand(atCmd,1000);
	return;
}

/*************************************
*Function 		: ping()
*Params 		: 
*Return			: success 0, failure -1
*Description 	: 
**************************************/
int ping(char *Ip)
{
	int i=0;
	CLR_FLAG(PING_SUCCESS);
	CLR_FLAG(PING_FAILURE);
	//set sms format
	memset(atCmd,0,SIZE_128);
	sprintf(atCmd,"at+mping=1,\"%s\",1,1,255\r",Ip);
	sendCommand(atCmd,1000);
 	for(i=0;i<10;i++)
	{
		if(	CHECK_FLAG(TCP_SOCKET_CONECCTED) )
		{
			if(CHECK_FLAG(PING_SUCCESS))
				return 0;
			if(CHECK_FLAG(PING_FAILURE))
				return -1;
			delayInms(1000);
			#if	WDT_IMPROVED
				++wdtCount;
			#endif
		}
		else
			break;	
	}
	return -1;
}

/* int isdigit (int c)
{
        return((c>='0') && (c<='9'));
}

int isalpha(int c)
{
        return((c >='a' && c <='z') || (c >='A' && c <='Z'));
}*/

int	validateUserInput( char	type, char* sBuff )
{
	unsigned int	uint=0;
	short			sLen=0;
	char			*chPtr=0;
	char			tStr[4],ipaddrs[4], dots=0;

	sLen = (short)strlen((const char*)sBuff);
	#if	WDT_IMPROVED
		++wdtCount;
	#endif

	//printf1("\rvalidateUserInput(): Input String Len %d\n", sLen);
	for( uint=0; uint<sLen; ++uint)
	{
		if( (sBuff[uint]==0x00) || (sBuff[uint]==0x0D) || (sBuff[uint]==0x0A) )
			sBuff[uint] = 0x00;
		if( (sBuff[uint]==' ') && (uint<sLen) )
		{
			return -1;
		}
	}
	switch( type )
	{
		case NUMERAL:
			for( uint=0; isdigit( sBuff[uint] ); uint++ );
			if( uint > 5 )
			{
				return -1;
			}
			uint = atoi( sBuff );
			if( (uint==0) || (uint>65535) )
			{
				return -1;
			}
			break;
		case WEB_ADDR:
			if( sLen > 32 )
			{
				return -1;
			}
			chPtr = NULL; dots=0;
			if( (chPtr = strchr( sBuff, '.') ) != NULL )
			{
				++chPtr; ++dots;
				for( uint=(chPtr-sBuff); uint<sLen; ++uint )
				{
					if( sBuff[uint] == '.' )
						++dots;
				}
				if( dots > 3 )
				{
					return -1;
				}
			}
			else
			{
				return -1;
			}
			// Currently Supported Domains
			chPtr = sBuff; chPtr += sLen-4;
			if( (chPtr=strstr(chPtr,".com")) != NULL )
				break;
			chPtr = sBuff; chPtr += sLen-4;
			if( (chPtr=strstr(chPtr,".net")) != NULL )
				break;
			chPtr = sBuff; chPtr += sLen-3;
			if( (chPtr=strstr(chPtr,".in")) != NULL )
				break;
			chPtr = sBuff; chPtr += sLen-4;
			if( (chPtr=strstr(chPtr,".org")) != NULL )
				break;
			
			if( dots != 3 )
			{
				return -1;
			}
			memset((void*)ipaddrs, 0, 4);
			chPtr = sBuff;
			for( sLen=0; sLen < 4; sLen++, chPtr++ )
			{
				memset((void*)tStr,0,4);
				for( uint=0; isdigit(*chPtr); chPtr++, uint++ )
					tStr[uint] = *chPtr;
				if( (!isdigit(*chPtr)) && ((*chPtr)!='.') && (sLen!=3) )
				{
					return -1;
				}
				uint = atoi( tStr );
				if( ((sLen==0) && (uint==0)) || (uint>255) )
				{
					return -1;
				}
				ipaddrs[sLen] = uint;
			}
			memset((void*)sBuff, 0, 16);
			sprintf(sBuff, "%d.%d.%d.%d", ipaddrs[0], ipaddrs[1], ipaddrs[2], ipaddrs[3]);
			break;
		case MOB_NUMB:
			if( sLen > 13 )
			{
				return -1;
			}
			dots = 0;
			if( sBuff[0] == '+' )
			{
				uint = 1;
				sLen = 12;
				for( uint=1; sBuff[uint]; uint++ )
				{
					if( isdigit( sBuff[uint] ) )
					{
						dots++;
					}
					else
					{
						return -1;
					}
				}
				if( dots != 12 )
				{
					return -1;
				}
			}
			else if( sBuff[0] == '0' )
			{
				for( uint=1; sBuff[uint]; uint++ )
				{
					if( isdigit( sBuff[uint] ) )
						dots++;
					else
					{
						return -1;
					}
				}
				if( dots != 10 )
				{
					return -1;
				}
			}
			else
			{
				for( uint=0; sBuff[uint]; uint++ )
				{
					if( isdigit( sBuff[uint] ) )
						dots++;
					else
					{
						return -1;
					}
				}
				if( dots != 10 )
				{
					return -1;
				}
			}
			break;
		case AUTH_STR:
			if( (!isdigit(sBuff[0])) && (!isalpha(sBuff[0])) && (sBuff[0]!='*') )
			{
				return -1;
			}
			break;
		case ALPNUM_STR:
			for( uint=0; uint<sLen; ++uint)
			{
				if( isdigit(sBuff[uint]) || isalpha(sBuff[uint]) )
					;
				else
				{
					return -1;
				}
			}
			break;
		case DIR_STR:
			if( (!isdigit(sBuff[0])) && (!isalpha(sBuff[0])) && (sBuff[0]!='_') )
			{
				return -1;
			}
			if( (sBuff[(sLen-1)]==0x00) || (sBuff[(sLen-1)]==0x0D) || (sBuff[(sLen-1)]==0x0A) )
			{
				sBuff[(sLen-1)] = 0;
				sLen--;
			}
			for( uint=1; uint<sLen; ++uint)
			{
				if(!isdigit(sBuff[uint]))
				{
					if(!isalpha(sBuff[uint]))
					{
						if(!(sBuff[uint]=='_'))
						{
							if(!(sBuff[uint]=='-'))
							{
								if(!(sBuff[uint]=='/'))
								{
									return -1;
								}
							}
						}
					}
				}
			}
			break;
		default:
			return -1;
	}
	return 0;
}

/*** EOF ***/
