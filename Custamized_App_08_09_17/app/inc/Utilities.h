#include "stdio.h"
//#include "sms.h"

#define SOFT_VERSION  0x01000000
#define MAX_AUTH_NUMBERS 10
#define MAX_COM_FAILURE_FOR_RESET 10
#define MAX_SIM_TRIES 4

typedef struct
{
	char deviceID[11];
	char deviceDesc[21];
	UINT16 comInterval;   		//Communication Interval In Seconds
	char gprsAPN[26];
	char gprsUserName[16];
	char gprsPassword[16];
	char tcpUrl[41];
	UINT16 tcpPort;
	char tcpUsename[16];
	char tcpPassword[16];
	UINT8 keepPDpActive;
	char softVersion[7];
	char prevSoftVersion[7];
	char otaMsgSender[15];
	UINT8 debugStat;
	char curLogFile[16];
}GENERAL_PARAM;


typedef struct
{
	char aNumber[15];
	char aGroup[5];
}AUTH_NUMBER;
char IMEI_NUMBER[16];
extern GAPP_RTC_T curTime;
extern UINT32 curSeconds;
extern GENERAL_PARAM GP;
extern AUTH_NUMBER AN[MAX_AUTH_NUMBERS];
extern int netWorkStat;
extern int simStatus;
extern int signalStrength;
extern int comStatus;


void setParamToFactorySettings();
void setDefaultAuthNumbers();
int setAsDefaultParameters();
int setDefaultParameters();
int readAuthNumbersFile();
int writeAuthorizedNumbersFile();
void toUpper(char *iStr);
int ftostr(float fNum,char str[],int dotsize);
int readConfigFile();
int writeConfigFile();
void ulocaltime(UINT32 timer, GAPP_RTC_T *tm);
UINT32 umktime(GAPP_RTC_T *timeptr);
void printParameters();
int OTA(char *otaUrl,char *otaPORT);
void removeChar(char *str, char garbage);
void strreplace(char *s, char chr, char repl_chr);
void getRTCTime();
void uartbaudset(int uartId, UINT16 baudRate);
void trim(char * s) ;
int Cmd_confSet(char *cString);
void initializeGpios();
unsigned int getKey(unsigned long dt);
void otaRepair();
int getSignaStrengthDB();