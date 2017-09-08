/**********************************************************************************
* Copyright (c) 2016 All Right Reserved
*
* Author:	Softel Technologies
* Contact:	support@cmsgp.com
*
* File: 	deviceConfig.c
***********************************************************************************/
//Include
#include "generalInfo.h"

//Local Macros
#define		ESC_CHAR		0x1B
//Globals
//Externs
// For watchdog do not use these variables for other purposes
extern unsigned long	wdtCount;

extern U32				tmpSize,tmpSlen;  // only used for strlen & sizeof
extern S8				recvFrameBuffer[];
extern U32				tempSerRespLen1;
//Structure Variables
extern DEVICE_CONFIG_MODEM		modemConfig;

//Function Definitions
void fillDefaultModemConfig(void)
{
	memset((void *)&modemConfig,0,sizeof(DEVICE_CONFIG_MODEM));
	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	modemConfig.baudRate = 9600;
	sprintf((char *)modemConfig.gprsDialNum,"*99#");
	sprintf((char *)modemConfig.apnName,"airtelgprs.com");
	sprintf((char *)modemConfig.logPasswd,"softel");
	sprintf((char *)modemConfig.serverIP,"117.218.55.125");
	modemConfig.serverPort = 8710;
	sprintf((char *)modemConfig.fotaServerIP,"117.218.55.125");
	modemConfig.fotaServerPort = 5000;
	sprintf((char *)modemConfig.debugLoggerIp,"117.218.55.125");
	modemConfig.debugLoggerPort = 8500;
	sprintf((char *)modemConfig.publicPingIP,"117.218.55.125");
	sprintf((char *)modemConfig.unitSerial,"CMSSER2GPRSTEST");
	modemConfig.enDbgLog = 0;
	sprintf((char*)modemConfig.smsNum[0], "7760073613"); 
    sprintf((char*)modemConfig.smsNum[1], "9620710195");
    sprintf((char*)modemConfig.smsNum[2], "8951461916"); 
    sprintf((char*)modemConfig.smsNum[3], "9845049177");
	sprintf((char*)modemConfig.smsNum[4], "9845165949");
	
	//write as a file
	writeModemConfigFile();
	return;
}

int writeModemConfigFile(void)
{
	int fd = 0,len = 0,ret = 0;
	#if	WDT_IMPROVED
		++wdtCount;
	#endif

	modemConfig.pppfcs = pppfcs16(PPPINITFCS16, (unsigned char *)&modemConfig, (sizeof(DEVICE_CONFIG_MODEM)-2) );
	fd = sys_file_open((const INT8 *)MODEM_CONFIG_FILE_NAME,(FS_O_WRONLY | FS_O_CREAT | FS_O_TRUNC));
	if( fd < 0)
	{
		printf1("\rCreate file Error-MODEM_CONFIG_FILE_NAME,ret:%d\n",fd);
		return -1;
	}
	len = sys_file_write(fd,(INT8 *)&modemConfig,sizeof(DEVICE_CONFIG_MODEM));
	if ( len < 0)
	{
		printf1("\rWrite file fail-MODEM_CONFIG_FILE_NAME,ret:%d\n",len);
		return -1;
	}
	ret = sys_file_flush(fd,10000);
	if ( ret < 0 )
	{
		printf1("\rflush file fail, ret:%d\n",ret);
		sys_file_close(fd);
		return -1;
	}
	sys_file_close(fd);
	#if	WDT_IMPROVED
		++wdtCount;
	#endif

	return 0;
}

int readModemConfigFile(void)
{
    int fd = 0;
    int len = 0;
	unsigned short fcs;
	#if	WDT_IMPROVED
		++wdtCount;
	#endif

	fd = sys_file_open(MODEM_CONFIG_FILE_NAME,FS_O_RDONLY);
	if ( fd < 0 )
	{
		printf1("\rOpen file Error-MODEM_CONFIG_FILE_NAME, ret:%d\n",fd);	
		sys_file_close(fd);
		return -1;
	}
	len = sys_file_read(fd,(INT8 *)&modemConfig,sizeof(DEVICE_CONFIG_MODEM));
	if ( len < 0)
	{
		printf1("\rRead file Error-MODEM_CONFIG_FILE_NAME,ret:%d\n",len);
		sys_file_close(fd);		
		return -1;
	}
	fcs = pppfcs16(PPPINITFCS16, (unsigned char *)&modemConfig, (sizeof(DEVICE_CONFIG_MODEM)-2) );
	if( fcs != modemConfig.pppfcs )
	{
		printf1( "Flash Read pppfcs Error..!\n", 0 );
		sys_file_close(fd);
		return -1;
	}
	sys_file_close(fd);
	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	return 0;
}

/*** EOF ***/
