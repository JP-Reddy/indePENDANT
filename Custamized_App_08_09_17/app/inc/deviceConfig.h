#ifndef  __DEVICE_CONFIG__
#define  __DEVICE_CONFIG__

//Includes
#pragma pack(1)
//Macros
#define 	MODEM_CONFIG_FILE_NAME		"mdmCfg.txt"

//Structures

//Device configuration structure
//Modem Related

typedef struct {
	unsigned long	baudRate;
	char 			logPasswd[32];
	char			gprsDialNum[16];
	char			apnName[32];
	char			username[32];
	char 			passwd[32];
	char			dynamicPPPip[32];
	char			modemIMEINo[32];
	char			imsi[32];
	char			ccid[32];
	char			smsNum[5][16];
	unsigned long	serverPort;
	char			serverIP[32];
	unsigned long	fotaServerPort;
	char			fotaServerIP[32];
	char			debugLoggerIp[32];
	unsigned long	debugLoggerPort;
	char			publicPingIP[32];
	unsigned char	enDbgLog;
	char			unitSerial[64];

	unsigned short	pppfcs;
} DEVICE_CONFIG_MODEM;

#endif

/*** EOF ***/
