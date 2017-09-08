#ifndef _SYS_SERVICES_H_
#define _SYS_SERVICES_H_

#include "cs_types.h"
#include "stdarg.h"

/*ERROR CODE*/
#define GAPP_RET_OK                  (0)
#define GAPP_RET_PARAMS_ERROR        (-1)
#define GAPP_RET_NOT_INIT            (-2)
#define GAPP_RET_MEMORY_ERROR        (-3)
#define GAPP_RET_OPTION_NOT_SUPPORT  (-4)
#define GAPP_RET_TIMEOUT             (-5)
#define GAPP_RET_UNKNOW_ERROR        (-6)


/*task or thread*/
#define GAPP_RET_TASK_ERR_BASE       (-100)
#define GAPP_RET_TASK_MSG_TOO_MUCH   (-101)
#define GAPP_RET_THREAD_NOT_CREATED  (-102)
#define GAPP_RET_THREAD_TOO_MUCH     (-103)

#define GAPP_RET_TASK_ERR_MAX        (-150)

/*timer's error*/
#define GAPP_RET_TIMER_ERR_BASE      (-151)
#define GAPP_RET_TIMER_TOO_MUCH      (-152)
#define GAPP_RET_TIMER_NOT_CREATE    (-153)

#define GAPP_RET_TIMER_ERR_MAX       (-200)

/*GPRS */
#define GAPP_RET_PDP_ERR_BASE        (-201)
#define GAPP_RET_PDP_NOT_ACTIVE      (-202)
#define GAPP_RET_PDP_BUSY            (-203)

#define GAPP_RET_PDP_ERR_MAX         (-250)

/*TCPIP*/
#define GAPP_RET_TCPIP_ERR_BASE      (-251)
#define GAPP_RET_DNS_BUSY            (-252)
#define GAPP_RET_DNS_ERROR           (-253)
#define GAPP_RET_TCPIP_ERROR         (-254)

#define GAPP_RET_TCPIP_ERR_MAX       (-300)

/*RTC*/
#define GAPP_RET_RTC_ERR_BASE        (-301)
#define GAPP_RET_RTC_ERR_0           (-302)

#define GAPP_RET_RTC_ERR_MAX         (-350)

/*TRACE*/
#define GAPP_RET_TRACE_ERR_BASE      (-351)
#define GAPP_RET_TRACE_ERR_TIMEOUT   (-352)

#define GAPP_RET_TRACE_ERR_MAX       (-400)

/*update*/
#define GAPP_RET_UD_ERR_BASE         (-401)
#define GAPP_RET_TIMER_CREATE_ERR    (-402)
#define GAPP_RET_RETRANS_ERROR       (-403)
#define GAPP_RET_SID_ERROR           (-404)
#define GAPP_RET_SEQ_ERROR           (-405)
#define GAPP_RET_CHECKSUM_ERROR      (-406)
#define GAPP_RET_FILE_LENGTH_ERROR   (-407)
#define GAPP_RET_FILE_WRITE_ERROR    (-408)
#define GAPP_RET_LAST_PACKET         (-409)
#define GAPP_RET_PACKET_LEN_ERROR    (-410)
#define GAPP_RET_EVT_ERROR           (-411)
#define GAPP_RET_UD_CONTINUE         (-412)
#define GAPP_RET_UD_UNKNOW_ERROR     (-413)
#define GAPP_RET_UD_REJECT_ERROR     (-414)

typedef struct
{
	UINT8  uart_id;
	UINT32 baud;
}GAPP_OPT_UART_BAUDREATE_T;

typedef enum
{
	GAPP_OPT_UART_BAUDRATE_ID,
	GAPP_OPT_PIN41_IRQ_ID,
	GAPP_OPT_PIN27_IRQ_ID,
	GAPP_OPT_LPG_CONTROL_ID,
	GAPP_OPT_SYS_VERSION_ID,
	GAPP_OPT_APP_UPDATA_ID,
}GAPP_OPTION_ID_T;

typedef struct
{
    const INT8 * image_name;
}GAPP_OPT_APP_UPDATA_T;

typedef enum
{
	GAPP_THREAD_PRIO_0 = 240,
	GAPP_THREAD_PRIO_1,
	GAPP_THREAD_PRIO_2,
	GAPP_THREAD_PRIO_3,
	GAPP_THREAD_PRIO_4,
	GAPP_THREAD_PRIO_5,
	GAPP_THREAD_PRIO_6,
	GAPP_THREAD_PRIO_7,
	GAPP_THREAD_PRIO_8,
	GAPP_THREAD_PRIO_9
}GAPP_THREAD_PRIO_T;

#define GAPP_TASKn 5

#define GAPP_TASK_INITED (0)

typedef struct
{
	UINT8  *name;
	UINT8  priority;
	UINT16 stack_size;
	void  (*main)(UINT32 msgid,UINT32 n1,UINT32 n2,UINT32 n3);
}GAPP_TASK_T;

//--------------------------------------------------------------------------------------------------
// The FS API define begin.
//-------------------------------------------------------------------------------------------------
// Open flag define

// Read only.
#define FS_O_RDONLY	            0

// Write only.
#define FS_O_WRONLY	     1

// Read and Write. 
#define FS_O_RDWR	            2


// If the file exists, this flag has no effect except as noted under FS_O_EXCL below. Otherwise, the file shall be created.
#define FS_O_CREAT		      00100

// If FS_O_CREAT and FS_O_EXCL are set, the function shall fail if the file exists.
#define FS_O_EXCL		      00200

// If the file exists, and is a regular file, and the file is successfully opened FS_O_WRONLY or FS_O_RDWR, its length shall be truncated to 0.
#define FS_O_TRUNC		      01000

// If set, the file offset shall be set to the end of the file prior to each write.
#define FS_O_APPEND	      02000

//--------------------------------------------------------------------------------------------------
// Seek flag define.
//--------------------------------------------------------------------------------------------------
// Seek from beginning of file.
#define FS_SEEK_SET                 0

// Seek from current position.
#define FS_SEEK_CUR                 1

// Set file pointer to EOF plus "offset"
#define FS_SEEK_END                 2


struct ip_addr
{
	UINT32 addr;
};


#define GAPP_IPPROTO_TCP 0
#define GAPP_IPPROTO_UDP 1

typedef struct
{
	UINT16 sin_port;//save as network sequence
	struct ip_addr sin_addr;
}GAPP_TCPIP_ADDR_T;


typedef struct
{
  UINT8 sec;   ///< Second
  UINT8 min;   ///< Minute
  UINT8 hour;  ///< Hour
  UINT8 day;   ///< Day
  UINT8 month; ///< Month
  UINT8 year;  ///< Year
  UINT8 wDay;  ///< Week Day
}GAPP_RTC_T;

typedef enum
{
  GAPP_RTC_INT_DISABLED = 0, ///<    Interruption disabled
  GAPP_RTC_INT_PER_SEC  = 1, ///<    Interruption every second
  GAPP_RTC_INT_PER_MIN  = 2, ///<    Interruption every min
  GAPP_RTC_INT_PER_HOUR = 3  ///<    Interruption every hour
} GAPP_RTC_MODE_T ;

typedef enum
{
    GAPP_IO_0,//gpo
    GAPP_IO_1,//gpo
    GAPP_IO_2,//gpio
    GAPP_IO_3,//gpio
    GAPP_IO_4,//gpio
}GAPP_IO_ID_T;

typedef struct
{
    BOOL op;
}GAPP_OPT_LPG_CONTROL_T;

typedef struct
{
    const UINT8 *sys_ver;
    UINT32 api_ver;
}GAPP_OPT_SYS_VERSION_T;


typedef struct
{
    BOOL rising;
    BOOL falling;
    BOOL debounce;
    BOOL level;
    void (*cb)(void);
}GAPP_OPT_PIN_CFG_T;


typedef enum
{
	GAPP_GPIO_OUT,
	GAPP_GPIO_IN
}GAPP_GPIO_CFG_T;

void sys_uart_output(INT32 id,UINT8 *buff,UINT16 len);

INT32  sys_vsnprintf(INT8 *buff,UINT32 n, const INT8 *fmt,va_list ap);

INT32 sys_taskSend(UINT32 tid,UINT32 msg,UINT32 n1,UINT32 n2,UINT32 n3);

void sys_taskSleep(UINT32 ms);

INT32  sys_at_send(const UINT8 *cmd,UINT16 len);

INT32  sys_file_open(const INT8 *pathname,UINT32 opt);

INT32  sys_file_seek(INT32 fd,INT32 offset,UINT8 opt);

INT32  sys_file_read(INT32 fd,UINT8 *buff,UINT32 size);

INT32  sys_file_write(INT32 fd,UINT8 *buff,UINT32 size);

INT32  sys_file_close(INT32 fd);

INT32  sys_file_delete(const INT8 *pathName);

INT32  sys_file_getSize(INT32 fd);

INT32  sys_file_flush(INT32 fd,UINT32 ms);

INT32  sys_file_EOF(INT32 fd);

INT32  sys_file_clear(void);

INT32  sys_sem_new(UINT8 inivalue);

void sys_sem_free(INT32 semid);

void sys_sem_wait(INT32 semid);

void sys_sem_signal(INT32 semid);

INT32  sys_timer_new(UINT32 ms,void (*fn)(void *arg),void *arg);

INT32  sys_timer_free(INT32 timerid);

INT32  sys_getHostByName(INT8 *hostname,struct ip_addr *addr);

INT32 sys_sock_create(INT32 nProtocol);

INT32 sys_sock_close(INT32 sock);

INT32 sys_sock_connect(INT32 sock,GAPP_TCPIP_ADDR_T *addr);

INT32 sys_sock_listen(INT32 sock);

INT32 sys_sock_bind(INT32 sock,GAPP_TCPIP_ADDR_T *addr);

INT32 sys_sock_accept(INT32 sock,GAPP_TCPIP_ADDR_T *addr);

INT32 sys_sock_send(INT32 sock,UINT8 *buff,UINT16 len);

INT32 sys_sock_send2(INT32 sock,UINT8 *buff,UINT16 len,GAPP_TCPIP_ADDR_T *dest);

INT32 sys_sock_recv(INT32 sock,UINT8 *buff,UINT16 len);

INT32 sys_sock_recvFrom(INT32 sock,UINT8 *buff,UINT16 len,GAPP_TCPIP_ADDR_T *src);

INT32 sys_sock_setOpt(INT32 sock,INT32 level,INT32 optname,const void * optval,INT32 optlen);

INT32 sys_sock_getOpt(INT32 sock,INT32 level,INT32 optname,const void * optval,INT32 optlen);

void * sys_malloc(UINT32 size);

void sys_free(void *p);

UINT32 sys_getSysTick(void);

void sys_softReset(void);

INT32 sys_PDPActive(INT8 *apn,INT8 *usr,INT8 *pwd,UINT32 *ip);

INT32 sys_PDPRelease(void);


INT32 sys_PDPStatus(UINT32 *status);

INT32 sys_hookUart(INT32 id,INT32 op);

INT32 sys_setRTC(GAPP_RTC_T *set);

INT32 sys_getRTC(GAPP_RTC_T *get);

INT32 sys_setRTC_timerMode(GAPP_RTC_MODE_T mode);

INT32 sys_setRTC_timerSwitch(INT32 on);

INT32 sys_setRTC_timerCB(void (*cb)(void));

INT32 sys_setRTC_alarm(GAPP_RTC_T *set);

INT32 sys_setRTC_alarmON(void);

INT32 sys_setRTC_alarmOFF(void);

INT32 sys_setRTC_alarmCB(void (*cb)(void));

INT32 sys_eventTrace(UINT32 value);

INT32 sys_textTrace(INT8 *fmt,...);

UINT32 sys_enterCS(void);

void sys_exitCS(UINT32 status);

void sys_watchdog_enable(UINT32 ms);

void sys_watchdog_disable(void);

void sys_watchdog_feed(void);

INT32 sys_GB2UNI(UINT16 gb_char,UINT16 *unicode_char);

UINT8 sys_UNI2GB(UINT16 uCode,UINT16 * pDst);

INT32 sys_gpio_cfg(GAPP_IO_ID_T id,GAPP_GPIO_CFG_T cfg);

INT32 sys_gpio_set(GAPP_IO_ID_T id,UINT8 level);

INT32 sys_gpio_get(GAPP_IO_ID_T id,UINT8 * level);

INT32 sys_set(GAPP_OPTION_ID_T id,void *arg,UINT16 len);

INT32 sys_get(GAPP_OPTION_ID_T id,void *arg,UINT16 len);

//by Sangram
INT32 Sys_set_BR(UINT8  uart_id, UINT32 baud);


#endif

