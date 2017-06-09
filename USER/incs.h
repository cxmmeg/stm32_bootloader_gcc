#ifndef __INCS_H__
#define __INCS_H__


#include <string.h>
#include "misc.h"
#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include "core_cmFunc.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "uart_isp.h"


#define TRUE    1
#define FALSE   0
//#define STDIO

#define RAM_BASE                        0x20000000
#define APP_OFFSET_SRAM                 0x200
#define APP_OFFSET_FLASH                0x4000
#define APP_MAX_SIZE                    (32 * 1024)
#define STACK_TOP                       0x2000c000
#define Global_Parm_Base                0x2000a000
#define FLASH_PAGE_ERASE_SIZE           2048

typedef void (* pFunction)(void);

#pragma pack(1)
typedef struct _System
{
    unsigned char bSystemTick;
    unsigned short length;
    unsigned char rx_finish;
    unsigned char rx_mark;
    unsigned char cmd;
    unsigned char rx_buffer[1024];
    unsigned char response[48];
    unsigned char resp_length;
    unsigned short upgrade_total_len;
} System;
#pragma pack()

#ifdef STDIO
void usart_send_blocking(USART_TypeDef * uart, unsigned char val);
unsigned char usart_recv_blocking(USART_TypeDef * uart);
int _write(int fd, char *ptr, int len);
int _read(int fd, char *ptr, int len);
void get_buffered_line(void);
#endif


#endif
