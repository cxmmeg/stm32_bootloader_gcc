#ifndef __DMX_H__
#define __DMX_H__

#define FIX_LEN         (2 + 2) //pkg index + cs

#define ERR_LEN         (2)
#define ERR_FORMAT      (1)
#define ERR_CS          (3)
#define ERR_CMP_CS      (4)
#define PKG_OK          (0)
#define PKG_LEN         (512)

#define   Mark                      0x00
#define   VALUE_1F                  0x00

#define   Upgrade_Data              0x10
#define   Upgrade_End               0x11
#define   Long_Jump                 0x12
#define   CMD_EOF                   0x13

void isp_init(void);
void isp_send_char(unsigned char val);
unsigned char isp_rece_char(void);
void isp_send_word(unsigned short val);
void isp_send_int(unsigned int val);
void put_hex(unsigned long val);
void put_string(char * s);
void isp_rx_pkg(void);
void handle_upgrade_data(unsigned char *upgrade_buffer);
void response_to_server(unsigned char *dst, unsigned char *parm, unsigned short len);
void cmp_upgrade_data(unsigned char *data);
void print_mem(unsigned char * ptr, unsigned int length);

#endif
