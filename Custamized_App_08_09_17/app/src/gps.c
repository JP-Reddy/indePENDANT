/*
#include "sys_services.h"
#include "sys_ext.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h" */
//#include "debug.h"
#include "generalInfo.h"
#define ZERO 48
#define NINE 57
#define MINUS 45
#define DECPNT 46

GPS_COR gps_Cordinates;

void initGPSParams(){

	 gps_Cordinates=(GPS_COR) {
		"0.0",			//latString
		0.0,			//lattitude
		"N",     		//latDir
		"0.0",			//longString
		0.0,			//longitude
		"E",			//longDir
		"",				//gpsTime
		"",				//gpsDate
		"0",			//gpsStat
		"0.0",			//gpsSpeed
		};		
}


int strtoint_n(char* str, int n)
{
    int sign = 1;
    int place = 1;
    int ret = 0;

    int i;
    for (i = n-1; i >= 0; i--, place *= 10)
    {
        int c = str[i];
        switch (c)
        {
            case MINUS:
                if (i == 0) sign = -1;
                else return -1;
                break;
            default:
                if (c >= ZERO && c <= NINE) ret += (c - ZERO) * place;
                else return -1;
        }
    }

    return sign * ret;
}

float _float_fraction(char* str, int n)
{
    float place = 0.1f;
    float ret = 0.0f;

    int i;
    for (i = 0; i < n; i++, place /= 10)
    {
        int c = str[i];
        ret += (c - ZERO) * place;
    }
    return ret;
}

float strtoflt(char* str)
{
    int n = 0;
    int sign = 1;
    int d = -1;
    int ret = 0;

    char* temp = str;
    while (*temp != '\0')
    {
        switch (*temp)
        {
            case MINUS:
                if (n == 0) sign = -1;
                else return -1;
                break;
            case DECPNT:
                if (d == -1) d = n;
                else return -1;
                break;
            default:
                if (*temp < ZERO && *temp > NINE) return -1;
        }
        n++;
        temp++;
    }

    if (d == -1)
    {
        return (float)(strtoint_n(str, n));
    }
    else if (d == 0)
    {
        return _float_fraction((str+d+1), (n-d-1));
    }
    else if (sign == -1 && d == 1)
    {
        return (-1)*_float_fraction((str+d+1), (n-d-1));
    }
    else if (sign == -1)
    {
        ret = strtoint_n(str+1, d-1);
        return (-1) * (ret + _float_fraction((str+d+1), (n-d-1)));
    }
    else
    {
        ret = strtoint_n(str, d);
        return ret + _float_fraction((str+d+1), (n-d-1));
    }
}
/*
* Intialize GPS- Stop automatic pushing of data 
* Specific Commands FIBOCOM GPS
*/
void initializeGPS_LS2003C2R(){
		char gpsCmd[50];
		int slen;
//		strcpy(gpsCmd,"$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n");
		strcpy(gpsCmd,"$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n");
		slen=strlen(gpsCmd);
		printf1("Command String Length  %d\n",slen); 
		sys_uart_output(1,gpsCmd,slen);
		sys_taskSleep(100);

		
}
void initializeGPS_FIbocom(){
	char gpsCmd[30];
	int slen;
	strcpy(gpsCmd,"$PSRF103,0,0,0,1*24\r\n");
	slen=strlen(gpsCmd);
	printf1("Command String Length  %d",slen); 
	sys_uart_output(1,gpsCmd,slen);
	sys_taskSleep(100);
	strcpy(gpsCmd,"$PSRF103,1,0,0,1*25\r\n");
	sys_uart_output(1,gpsCmd,slen);
	sys_taskSleep(100);
	strcpy(gpsCmd,"$PSRF103,2,0,0,1*26\r\n");
	sys_uart_output(1,gpsCmd,slen);
	sys_taskSleep(100);
	strcpy(gpsCmd,"$PSRF103,3,0,0,1*27\r\n");
	sys_uart_output(1,gpsCmd,slen);
	sys_taskSleep(100);
	//strcpy(gpsCmd,"$PSRF103,4,0,1,1*21\r\n");
	strcpy(gpsCmd,"$PSRF103,4,0,0,1*20\r\n");
	sys_uart_output(1,gpsCmd,slen);
	sys_taskSleep(100);
	
}

/*
	Get GPS cordinates from GPRMC String
*/
int getGPSCordinates(UINT8 *gpsstr, UINT16 slen){
	char *pch;
	int i=0;
	//gpsstr[slen]=0;
	//printf1("%s",gpsstr); 
	//sys_uart_output(0,gpsstr,slen);/*uart echo*/
	pch = strtok (gpsstr,",");
	if (!strcmp(pch,"$GPRMC")){
		while (pch != NULL)
			{
				//MDB(0,DB2,"%s",pch);
				switch (i){
					case 1:
						strcpy(gps_Cordinates.gpsTime,pch);
						break;
					case 2:
						strcpy(gps_Cordinates.gpsStat,pch);
						break;
					case 3:
						//gps_Cordinates.lattitude=strtoflt(pch);
						strcpy(gps_Cordinates.latString,pch);
						break;
					case 4:
						strcpy(gps_Cordinates.latDir,pch);
						break;
					case 5:
						strcpy(gps_Cordinates.longString,pch);
						//gps_Cordinates.longitude=strtoflt(pch);
						break;
					case 6:
						strcpy(gps_Cordinates.longDir,pch);
						break;
					case 7:
						strcpy(gps_Cordinates.gpsSpeed,pch);
						break;
					case 9:
						strcpy(gps_Cordinates.gpsDate,pch);
						break;
					default:
						break;
					
					}
				//sys_uart_output(0,(INT8*)pch,strlen(pch));/*uart echo*/	
				//sys_uart_output(0,"\n",1);
				
				if (i<=1){   //Time field recieved  
					pch = strtok (NULL, ",");
				}else if (gps_Cordinates.gpsStat[0]=='A'){
					pch = strtok (NULL, ",");
				}else{
					pch=NULL;
				}
				i++;
			}
			printf1("Status : %s Time:%s",gps_Cordinates.gpsStat,gps_Cordinates.gpsTime);
			if (gps_Cordinates.gpsStat[0]=='A'){
				printf1("Date & Time : %s %s",gps_Cordinates.gpsDate,gps_Cordinates.gpsTime);
				printf1("Lattitude : %s %s",gps_Cordinates.latString,gps_Cordinates.latDir);
				printf1("Longititude : %s %s",gps_Cordinates.longString,gps_Cordinates.longDir);
				initializeGPS_LS2003C2R();
				return GPS_OK;
			}else if(gps_Cordinates.gpsStat[0]=='V'){
				printf1("GPS Position not yet fixed");
				return GPS_NO_FIX;
			}else{
				printf1("Unknown GPS Status");
//				initGPSParams();
				return GPS_UNKNOWN_ERR;
				
			}
	}else{
		
		return GPS_NO_REPONSE;
	}
	return GPS_NO_REPONSE;
}
/*
Get GPS Spped from GPVTG string
*/
bool getGPSSpeed(INT8 *gpsstr, UINT16 slen){
	char *pch;
	int i=0;
//	sys_uart_output(0,gpsstr,slen);/*uart echo*/
	pch = strtok (gpsstr,",");
	if (!strcmp(pch,"$GPVTG")){
		while (pch != NULL)
			{
				//MDB(0,DB1,"%s",pch);
				switch (i){
					case 6:
						strcpy(gps_Cordinates.gpsSpeed,pch);
						break;

					default:
						break;
					
					}
				//sys_uart_output(0,(INT8*)pch,strlen(pch));/*uart echo*/	
				//sys_uart_output(0,"\n",1);
				i++;
				pch = strtok (NULL, ",");
			}
			printf1("GPS Speed : %s ",gps_Cordinates.gpsSpeed);	
			
	}else{
		initializeGPS_LS2003C2R();
		return 0;
	}
	initializeGPS_LS2003C2R();
	return 1;
}
/*
* Gets data nd time in data & time format fro GPS data
* GPS data structure has to be update from the Data string received from GPS
* before using this function.
* Timezone offse will also be added to the GPS time
*/
#if 0
void getGPSTime(GAPP_RTC_T *rTime){
	char dTmp[3];
	UINT32 tSec;
	strncpy(dTmp,gps_Cordinates.gpsDate,2);
	rTime->day=atoi(dTmp);
	strncpy(dTmp,gps_Cordinates.gpsDate+2,2);
	rTime->month=atoi(dTmp);
	strncpy(dTmp,gps_Cordinates.gpsDate+4,2);
	rTime->year=atoi(dTmp);
	strncpy(dTmp,gps_Cordinates.gpsTime,2);
	rTime->hour=atoi(dTmp);
	strncpy(dTmp,gps_Cordinates.gpsTime+2,2);
	rTime->min=atoi(dTmp);
	strncpy(dTmp,gps_Cordinates.gpsTime+4,2);
	rTime->sec=atoi(dTmp);
	printf1("%02d/%02d/%04d %02d:%02d:%02d",rTime->day,rTime->month,rTime->year+2000,rTime->hour,rTime->min,rTime->sec);
	tSec=umktime(rTime);
	printf1("Time in Seconds : %u",tSec);
	tSec=tSec+19800;
	ulocaltime(tSec,rTime);
	printf1("%02d/%02d/%04d %02d:%02d:%02d",rTime->day,rTime->month,rTime->year+2000,rTime->hour,rTime->min,rTime->sec);
}
/*
* Sets RTC time with the data and time recieved from GPS 
* Time zone offset will be added 
*/

void setRTCtimeFromGps(){
	GAPP_RTC_T rtcTime;
	char dTmp[3];
	UINT32 tSec;
	strncpy(dTmp,gps_Cordinates.gpsDate,2);
	rtcTime.day=atoi(dTmp);
	strncpy(dTmp,gps_Cordinates.gpsDate+2,2);
	rtcTime.month=atoi(dTmp);
	strncpy(dTmp,gps_Cordinates.gpsDate+4,2);
	rtcTime.year=atoi(dTmp);
	strncpy(dTmp,gps_Cordinates.gpsTime,2);
	rtcTime.hour=atoi(dTmp);
	strncpy(dTmp,gps_Cordinates.gpsTime+2,2);
	rtcTime.min=atoi(dTmp);
	strncpy(dTmp,gps_Cordinates.gpsTime+4,2);
	rtcTime.sec=atoi(dTmp);
	printf1("%02d/%02d/%04d %02d:%02d:%02d",rtcTime.day,rtcTime.month,rtcTime.year+2000,rtcTime.hour,rtcTime.min,rtcTime.sec);
	tSec=umktime(&rtcTime);
	printf1("Time in Seconds : %u",tSec);
	tSec=tSec+19800;
	ulocaltime(tSec,&rtcTime);
	printf1("%02d/%02d/%04d %02d:%02d:%02d",rtcTime.day,rtcTime.month,rtcTime.year+2000,rtcTime.hour,rtcTime.min,rtcTime.sec);
	sys_setRTC(&rtcTime);
}
void setRTCtime(){
	GAPP_RTC_T rtcTime;
	rtcTime.day=30;
	rtcTime.month=1;
	rtcTime.year=15;
	rtcTime.hour=17;
	rtcTime.min=01;
	rtcTime.sec=0;
	printf1("%02d/%02d/%04d %02d:%02d:%02d",rtcTime.day,rtcTime.month,rtcTime.year+2000,rtcTime.hour,rtcTime.min,rtcTime.sec);
	sys_setRTC(&rtcTime);
}

#endif
