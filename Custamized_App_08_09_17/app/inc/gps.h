#include "stdio.h"
typedef enum
{
    GPS_OK,
    GPS_NO_FIX,
    GPS_NO_REPONSE,
    GPS_UNKNOWN_ERR,
   
}GPS_STAT;

typedef struct
{
	char latString[20];
	double   lattitude;
	char   latDir[2];
	char longString[20];
	double    longitude;
	char   longDir[2];
	char gpsTime[11];
	char gpsDate[7];
	char gpsStat[2];
	char gpsSpeed[10];
}GPS_COR;
extern GPS_COR gps_Cordinates;
int getGPSCordinates(UINT8 *gpsstr, UINT16 slen);
bool getGPSSpeed(INT8 *gpsstr, UINT16 slen);
void initGPSParams();

void initGPSParams();

void initializeGPS_LS2003C2R();

void getGPSTime(GAPP_RTC_T *rTime);
void setRTCtimeFromGps();
void setRTCtime();


void initializeGPS_FIbocom();



