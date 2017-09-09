
/* Includes */
#include "generalInfo.h"

/* Local Macros */
#define BAUDRATE				115200
#define UART_0_ENABLE			1 //for debug
#define MAX_PING_FAILED_CNT		5

/* Extern */
extern U8		sockRecvBuffer[];
extern INT32	sockTcp,sockUdp;

/* Globals */
UINT32 	Task_Main,Task_GPS_Handle,Task_smsHandle,Task_GpsPostHandle;
char 	dbgBuf[SIZE_1024];
U32		tmpSize,tmpSlen;  // only used for strlen & sizeof
S8		sendFrameBuffer[SIZE_2048],recvFrameBuffer[SIZE_2048],dbgBuf[SIZE_1024],atCmd[SIZE_128];
U32		tempSerRespLen,curSockBuffLen,tempSerRespLen1;
char	smsMsgBuffer[SIZE_512];
GAPP_IO_ID_T gpioSOS_Switch=GAPP_IO_1;  

UINT8 status;
 
char gps_RMCCmd[55]="$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n";
char gps_VTGCmd[55]="$PMTK314,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n";
char gps_StopCmd[55]="$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n";

U64  	flag1;

unsigned short fcstab[256] = {

    0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
    0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
    0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
    0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
    0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
    0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
    0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
    0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
    0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
    0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
    0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
    0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
    0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
    0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
    0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
    0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
    0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
    0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
    0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
    0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
    0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
    0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
    0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
    0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
    0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
    0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
    0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
    0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
    0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
    0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
    0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
    0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78

};

// For watchdog do not use these variables for other purposes
unsigned long	wdtCount, prevWdtCount, fotaTimeOutVar;
unsigned char	wdtFailureCount;
U32				secGlobal;

DEVICE_CONFIG_MODEM		modemConfig;
GAPP_RTC_T				rtcTime;

static 	UINT8 app_init(GAPP_TASK_T **tl);

//
static char gStr[200];
/* The module will obtain sys_callback variable automatically when the user program is started, 
and calls the callback function at appropriate time. 
*/
SYS_CALLBACK_T sys_callback = 
{
	app_init,/*init function*/
	uart_in,/*uart input callback function*/
	at_in, /*at rsp callback function*/
	sig_in /*system signal callback function such as PDP and socket event,*/
};

/* 
Uart_input, uart input data function, after sys_hookUart is called, 
when uart receive data, this interface will be invoked.
 */
/*******************************************
*Function 		: uart_in()
*Params 		:  
*Return			:  
*Description 	: callback when uart receives data
********************************************/
void  uart_in(INT32 uid,UINT8 *data,UINT16 len)
{
	if(uid == 0) //uart 1 / flashing uart
	{
		if((tempSerRespLen1 + len) >= SIZE_2048)
			tempSerRespLen1=0;
		memcpy((void *)&recvFrameBuffer[tempSerRespLen1],(const void *)data,len);
		tempSerRespLen1 += len;
		//printf1("\rUART1 : %s\n",recvFrameBuffer);
	}
	else if(uid == 1) //communication port for GPS
	{
		memset(recvFrameBuffer,0,SIZE_2048);
		memcpy((void *)recvFrameBuffer,(const void *)data,len);
		tempSerRespLen = len;
		printf1("\rUART2 : %s\n",recvFrameBuffer);
		sys_taskSend(Task_GPS_Handle,T1_UART_DATA_RECV_IND,(UINT32)uid,(UINT32)recvFrameBuffer,(UINT32)len);
	}
}

/*******************************************
*Function 		: mainTask()
*Params 		:  
*Return			:  
*Description 	:  This is pmon Task
********************************************/
void mainTask(UINT32 id,UINT32 n1,UINT32 n2,UINT32 n3)
{
	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	switch(id)
	{
		case GAPP_TASK_INITED:
		{
			printf1("\rpmonTask Started\n");
			Task_Main = n1;//save task id
			initGPSParams();
			initializeGPS_LS2003C2R();
			initializeGPS_FIbocom();
			sys_taskSend(Task_Main, GAPP_TASK_RE_INITED, 0, 0, 0);
		}
		break;
		case GAPP_TASK_RE_INITED:
		{
			printf1("\rpmonTask Ready\n");
 			if(CHECK_FLAG(POWER_ON_FLAG))
			{
				CLR_FLAG(POWER_ON_FLAG);
				chkSimRegistration();
				getPppIP();
				sys_taskSend(Task_Main, GAPP_TASK_RE_INITED, 0, 0, 0);
			}
			else
			{
				#if	WDT_IMPROVED
					++wdtCount;
				#endif
				if( gprsStatus() == 1 )
				{
					printf1("\rGPRS/PPP Connected...\n");
					SET_FLAG(GPRS_CONNECTED);
					while(1) 
					{
						delayInms(500); 
						if(sys_gpio_get(GAPP_IO_3, &status) < 0) 
						{
							printf1("Switch Read3 Failed\n"); 
						}
						else
						{
							if(status == 0)
							{
								printf1("Switch ON %d \n", status); 
								SET_FLAG(PANIC_SWITCH_ON);
								for(int i=0;i<5;i++)
								{
								   printf1("Sending sms to %s\n" , modemConfig.smsNum[i]); 
								   sendSMS("Panic Button Pressed",modemConfig.smsNum[i]);	
								}
							}
						}
					}
				}
				else
				{
					printf1("\rGPRS/PPP Disconnected...\n");
					CLR_FLAG(TCP_SOCKET_CONECCTED);
					CLR_FLAG(UDP_SOCKET_CONECCTED);
					CLR_FLAG(GPRS_CONNECTED);
					chkSimRegistration();
					getPppIP();
					sys_taskSend(Task_Main, GAPP_TASK_RE_INITED, 0, 0, 0);
					//Socket Creation after close existing connection
/* 					closeSocket(sockTcp);
					if (sockCreate(modemConfig.serverIP,modemConfig.serverPort) < 0)
					{
						#if	WDT_IMPROVED
							++wdtCount;
						#endif
						//sys_taskSend(Task_Main, GAPP_TASK_RE_INITED, 0, 0, 0);
						deactivateGprs();
						sys_taskSend(Task_Main, GAPP_TASK_RE_INITED, 0, 0, 0);
					}  */ 
				}
			}

			#if	WDT_IMPROVED
				++wdtCount;
			#endif
			sys_taskSend(Task_Main, GAPP_TASK_RE_INITED, 0, 0, 0);
		}
		break;
		default:
		break;
	}
	return;
}

/*******************************************
*Function 		: gpsHandlingTask()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
void gpsHandlingTask(UINT32 id,UINT32 n1,UINT32 n2,UINT32 n3)
{
	int len=0;
	printf1("\rgpsHandlingTask Started\n");
	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	switch(id)
	{
		case GAPP_TASK_INITED:
		{
			Task_GPS_Handle = n1;//save task id	
			printf1("\rTask2 - gpsHandlingTask is ready\n");
			sys_taskSend(Task_GPS_Handle, T2_START_COMMUNICATION, 0, 0, 0);
		}
		break;
		case T2_START_COMMUNICATION:
		{
			printf1(" gpsHandlingTask is ready to GPS Info\n");
			while(1)
			{
				if(CHECK_FLAG(GPRS_CONNECTED))
				{
					len = strlen(gps_RMCCmd);
					sys_uart_output(1,gps_RMCCmd,len);
					printf1("Sending RMS cmd %s %d\n", gps_RMCCmd,len);
					delayInms(1500);
				}
				else
				{
					printf1("gpsHandlingTask() : Waiting to GPRS get Connect\n");
					delayInms(6000);
				}
			}
		}
		break;
		case T1_UART_DATA_RECV_IND:
		{
			if(n1 == 1) //uid 
			{
				printf1("GPS Resp :%s\n",(S8 *)n2);
				if (strequsub("$PMTK001",(UINT8*)n2))
				{
					break;
				}
				strncpy(gStr,(char *)n2,(UINT16)n3);
				if (getGPSCordinates(gStr,(UINT16)n3) == GPS_OK)
				{
					printf1("GPS Read Success\n");
				}
				else
				{
					printf1("GPS Read Error\n");
				}
			}
		}
		break;
		default:
		break;
	}
	sys_taskSend(Task_GPS_Handle, T2_START_COMMUNICATION, 0, 0, 0);
	return;
}

/*******************************************
*Function 		: gpsDataPostTask()
*Params 		:  
*Return			:  
*Description 	:
********************************************/
void gpsDataPostTask(UINT32 id,UINT32 n1,UINT32 n2,UINT32 n3)
{
	printf1("GPS Data Post Task Started\n");
	
	#if	WDT_IMPROVED
		++wdtCount;
	#endif
	switch(id)
	{
		case GAPP_TASK_INITED:
		{
			Task_GpsPostHandle = n1;//save task id
			printf1("\rgpsDataPostTask ready\n");
			sys_taskSend(Task_GpsPostHandle, T2_START_COMMUNICATION, 0, 0, 0);
		}
		break;
		case T2_START_COMMUNICATION:
		{
			while(0) //make it 1 later
			{
				#if	WDT_IMPROVED
					++wdtCount;
				#endif
				if(CHECK_FLAG(PANIC_SWITCH_ON))
				{
					//postGpsInfo();
					delayInms(10000);
				}
			}
		}
		break;
		default:
		break;
		#if	WDT_IMPROVED
			++wdtCount;
		#endif	
	}
	//sys_taskSend(Task_GpsPostHandle, T2_START_COMMUNICATION, 0, 0, 0);
	return;
}

GAPP_TASK_T app_task[] ={ {"main task",GAPP_THREAD_PRIO_0,4096,mainTask},
						  {"GPS DataReadHandle task",GAPP_THREAD_PRIO_1,4096,gpsHandlingTask},
						  {"GPS DataPostHandle task",GAPP_THREAD_PRIO_1,4096,gpsDataPostTask}
						};

/*******************************************
*Function 		: app_init()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
static UINT8 app_init(GAPP_TASK_T **tl)
{
	/* Set the baud-rate */
	SetBaudRate();
	/*hook uart so we can get data form uart input*/
	sys_hookUart(0,1); //id=0,UART1 n op=1,UART1
	sys_taskSleep(100);
	sys_hookUart(1,1);

	//Initialize the Flags
	SET_FLAG(POWER_ON_FLAG);
	CLR_FLAG(MODEM_READY);
	CLR_FLAG(FOTA_STATUS_FLAG_WDT);
	CLR_FLAG(UDP_SOCKET_CONECCTED);
	CLR_FLAG(PING_SUCCESS);
	CLR_FLAG(PING_FAILURE);
	CLR_FLAG(TCP_SOCKET_CONECCTED);
	SET_FLAG(ECHO_DISABLE);
	CLR_FLAG(GET_NUMERIC);
	CLR_FLAG(CONFIG_EXIT);
	CLR_FLAG(GPRS_CONNECTED);
	CLR_FLAG(PANIC_SWITCH_ON);

	//Read Config and other files from file system
	//fillDefaultModemConfig();
	if( readModemConfigFile() < 0)
		fillDefaultModemConfig();
	
	//Initialize the modem interfaces
	initGpioPins();
	//startWdtTimer();
	
 	printf1("\n\r");
	printf1("<<< %s >>>\n\r",FW_VERSION);

	*tl = app_task;/*so we can got the task list at the modem side*/		
	return sizeof(app_task)/sizeof(GAPP_TASK_T);/*must return task list size*/
}

/*******************************************
*Function 		: SetBaudRate()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
int SetBaudRate(void)
{
	GAPP_OPT_UART_BAUDREATE_T uart_1;
	GAPP_OPT_UART_BAUDREATE_T uart_2;
	
    uart_1.uart_id = 0;
	uart_1.baud = BAUDRATE; //115200
	sys_set(GAPP_OPT_UART_BAUDRATE_ID,&uart_1,sizeof(uart_1));
	
    uart_2.uart_id = 1;
	uart_2.baud = 9600;
	sys_set(GAPP_OPT_UART_BAUDRATE_ID,&uart_2,sizeof(uart_2));
	
    return -1;
}

/*******************************************
*Function 		: printf1()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
int printf1(const char *format, ...)
{
#if UART_0_ENABLE
	va_list arg;
	volatile unsigned int done;
	
	memset(dbgBuf,0,SIZE_1024);
	va_start (arg, format);
	done = vsprintf(dbgBuf, format, arg);
	tmpSlen = strlen(dbgBuf);
	sys_uart_output(0, dbgBuf, tmpSlen);
	va_end(arg);
	return done;
#else
	va_list arg;
	volatile unsigned int done;
	
	memset(dbgBuf,0,SIZE_1024);
	va_start (arg, format);
	done = vsprintf(dbgBuf, format, arg);
	tmpSlen = strlen(dbgBuf);
	sys_uart_output(1, dbgBuf, tmpSlen);
	va_end(arg);
	return done;
#endif

}

/*******************************************
*Function 		: printf2()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
int printf2(const char *format, ...)
{
#if UART_0_ENABLE
	va_list arg;
	volatile unsigned int done;
	
	memset(dbgBuf,0,SIZE_1024);
	va_start (arg, format);
	done = vsprintf(dbgBuf, format, arg);
	tmpSlen = strlen(dbgBuf);
	sys_uart_output(1, dbgBuf, tmpSlen);
	va_end(arg);
	return done;
#else
	va_list arg;
	volatile unsigned int done;
	
	memset(dbgBuf,0,SIZE_1024);
	va_start (arg, format);
	done = vsprintf(dbgBuf, format, arg);
	tmpSlen = strlen(dbgBuf);
	sys_uart_output(0, dbgBuf, tmpSlen);
	va_end(arg);
	return done;
#endif

}

/*******************************************
*Function 		: delayInms()
*Params 		:  
*Return			:  
*Description 	:  Don't use this delay more than 30 sec 
********************************************/
void delayInms(U32 time)
{
	sys_taskSleep(time);
}

/*******************************************
*Function 		: powerCycle()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
int powerCycle(void)
{
	int i=0;
	
	for(i=0;i<9;i++)  //stop pulse more than 60 sec
		delayInms(10000);

	return 0;
}

/*****************************************************
*Function 		: pppfcs16()
*Params 		:  
*Return			:  
*Description 	:  
******************************************************/
// Calculate a new FCS given the current FCS and new data
unsigned short  pppfcs16 (unsigned short fcs, unsigned char *cp, int len)
{
   // assert( sizeof(unsigned short) == 2);
    //assert( ((unsigned short) - 1) > 0);
	#if	WDT_IMPROVED
		++wdtCount;
	#endif

    while (len--)
        fcs = (fcs >> 8) ^ fcstab[ (fcs ^ *cp++) & 0xFF];

    return fcs;
}

/*****************************************************
*Function 		: initGpioPins()
*Params 		:  
*Return			:  
*Description 	:  
******************************************************/
void initGpioPins(void)
{
	int ret=0;
	// Configure the pins
	// Make watchdog start in the beginning itself.
	//ret = sys_gpio_cfg(gpioWdtid, GAPP_GPIO_OUT);
	//if ( ret < 0 )
	
	//	printf1("\rConfig gpio %d out fail, ret:%d\n",52,ret); 
	
	// Setting Power Cycle out pin to be output

	ret = sys_gpio_cfg(GAPP_IO_3, GAPP_GPIO_IN);
	if ( ret < 0 )
	{
		printf1("\rConfig gpio %d out fail, ret:%d\n",3,ret); 
	} 

	return;
}

/*******************************************
*Function 		: startWdtTimer()
*Params 		:  
*Return			:  
*Description 	:  
********************************************/
void startWdtTimer(void)
{
/* 	int 		ret=0;
	ret = sys_rtc_register_cb(GAPP_RTC_CB_SECOND, wdtPullsHandler);
	if ( ret < 0 )
	{
		printf1("\rRegister second callback fail\n"); 
	}
	ret = sys_rtc_configure_cb(GAPP_RTC_CB_SECOND, TRUE);
	if (  ret < 0  )
	{
		printf1("\renable second callback fail\n"); 
	} */
	return;
}

/* EOF */