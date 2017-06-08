#include "incs.h"

static char get_hex(unsigned char v)
{
    if (v >= 0 && v <= 9)
        return v + '0';
    else
        return v - 10 + 'A';
}

static void uart_put_uchar(unsigned char val)
{
    char str[4];
    str[0] = get_hex((val & 0xF0) >> 4);
    str[1] = get_hex(val & 0xF);

    isp_send_char(str[0]);
    isp_send_char(str[1]);
}

void print_mem(unsigned char * ptr, unsigned int length)
{
    unsigned int i = 0;
    unsigned int cnt = 0;
    for (i = 0; i < length; ++i)
    {
        uart_put_uchar(ptr[i]);

        cnt++;
        if ((cnt % 16) == 0)
        {
            isp_send_char('\n');
            isp_send_char('\r');
        }
        else if ((cnt % 8) == 0)
        {
            isp_send_char(' ');
            isp_send_char(' ');
        }
        else
        {
            isp_send_char(' ');
        }
    }
    isp_send_char('\n');
    isp_send_char('\r');
}

void put_hex(unsigned long val)
{
    /* val = 0x1234ABCD */
    unsigned char c;
    int i = 0;

    isp_send_char('0');
    isp_send_char('x');

    for (i = 0; i < 8; i++)
    {
        c = (val >> ((7-i)*4)) & 0xf;
        if ((c >= 0) && (c <= 9))
        {
            c = '0' + c;
        }
        else if ((c >= 0xA) && (c <= 0xF))
        {
            c = 'A' + (c -  0xA);
        }
        isp_send_char(c);
    }
    isp_send_char('\n');
    isp_send_char('\r');
}

void put_string(char * s)
{
    int i = 0;
    while (s[i])
    {
        isp_send_char(s[i]);
        i++;
    }
}

void isp_send_int(unsigned int val)
{
    int i;
    unsigned char * src = (unsigned char *)&val;

    for (i = 0; i < 4; i++)
    {
        isp_send_char(src[i]);
    }
}

void isp_send_word(unsigned short val)
{
    int i;
    unsigned char * src = (unsigned char *)&val;

    for (i = 0; i < 2; i++)
    {
        isp_send_char(src[i]);
    }
}

void isp_send_char(unsigned char val)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART1->DR = val;
}

unsigned char isp_rece_char(void)
{
    unsigned short dat;
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    dat = USART1->DR;
    return (unsigned char)dat;
}

void isp_init(void)
{
    NVIC_InitTypeDef            NVIC_InitStructure;
    USART_InitTypeDef           USART_InitStructure;
    USART_ClockInitTypeDef      USART_ClockInitType;
    GPIO_InitTypeDef  			GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //TXD
    GPIO_InitStructure.GPIO_Pin = (1 << 9);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //RXD
    GPIO_InitStructure.GPIO_Pin = (1 << 10);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_DeInit(USART1);
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ClockInitType.USART_Clock = USART_Clock_Disable;
    USART_ClockInitType.USART_CPOL = USART_CPOL_High;
    USART_ClockInitType.USART_CPHA = USART_CPHA_1Edge;
    USART_ClockInitType.USART_LastBit = USART_LastBit_Enable;
    USART_ClockInit(USART1, &USART_ClockInitType);

    USART_ClearITPendingBit(USART1, USART_IT_TC);
    USART_ITConfig(USART1, USART_IT_TC, DISABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);

    USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void)
{
//    volatile unsigned int IIR3 = USART1->SR;
//    unsigned short w;

//    if (IIR3 & USART_FLAG_RXNE)
//    {
//        w = USART1->DR;
//    }

//    if (IIR3 & USART_FLAG_TC)
//    {
////        USART_ClearITPendingBit(USART2, USART_IT_TC);
////        if (Dmx.wDmxTransmitCount)
////        {
////            Dmx.wDmxTransmitCount--;
////            USART2->DR = (*Dmx_pDmxTransmitBuffer ++) | 0x100;
////        }
////        else
////        {
////            //DmxTransmitDisable();
////            Dmx.cDmxTransmitFinish = TRUE;
////            USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
////        }
//    }
}

void isp_rx_pkg(void)
{
    unsigned char temp, leave;
    volatile System * parm = (System *)(Global_Parm_Base);
    leave = 0;

    while (1)
    {
        temp = isp_rece_char();

        if (parm->rx_mark)
        {
            switch (temp)
            {
            case VALUE_1F:
            {
                if (parm->length < 1024)
                {
                    parm->rx_buffer[parm->length] = VALUE_1F;
                    parm->length++;
                }
                else
                {
                    parm->length = 0;
                    parm->rx_finish = 0;
                }
                break;
            }
            case Upgrade_Data:
            case Upgrade_End:
            case Long_Jump:
            {
                parm->length = 0;
                parm->cmd = temp;
                parm->rx_finish = 0;
                break;
            }
            case CMD_EOF:
            {
                parm->rx_finish = 1;
                parm->rx_mark = 0;
                leave = 1;
            }
            default:
            {
                break;
            }
            }

            if (leave)
            {
                break;
            }
            parm->rx_mark = 0;
        }
        else
        {
            if ((unsigned char)temp == Mark)//数据头
            {
                parm->rx_mark = 1;
            }
            else
            {
                if (parm->length < 1024)
                {
                    parm->rx_buffer[parm->length] = temp;
                    parm->length++;
                }
                else
                {
                    parm->length = 0;
                }
            }
        }
    }
}

void handle_upgrade_data(unsigned char * upgrade_buffer)
{
    volatile System * parm = (System *)(Global_Parm_Base);
    unsigned char * dst = (unsigned char *)(RAM_BASE + APP_OFFSET_SRAM);
    unsigned char * src = upgrade_buffer;
    unsigned char * resp = parm->response;
    unsigned short len = src[0] + src[1] * 256;
    int i;
    unsigned short cs = 0;
    unsigned short pkg_index;
    unsigned int offset;

    if (len != (PKG_LEN + FIX_LEN))
    {
        resp[0] = ERR_LEN;
        resp[1] = 0;
        resp[2] = 0;
        resp[3] = 0;
        resp[4] = 0;
        resp[5] = 0;
        resp[6] = 0;
        resp[7] = 0;
        parm->resp_length = 8;
        return;
    }

    for (i = 0; i < len - 2; i++)
    {
        cs += src[2 + i];
    }

    if (cs != (src[len] + src[len + 1] * 256))
    {
        resp[0] = ERR_CS;
        resp[1] = 0xFF;
        resp[2] = cs;
        resp[3] = cs >> 8;
        resp[4] = src[len];
        resp[5] = src[len + 1];
        resp[6] = src[0];
        resp[7] = src[1];
        parm->resp_length = 8;
        return;
    }

    pkg_index = src[2] + (src[3] * 256);
    offset = pkg_index * PKG_LEN;

    for (i = 0; i < PKG_LEN; i++)
    {
        dst[offset + i] = src[4 + i];
    }

    parm->upgrade_total_len = (pkg_index + 1) * PKG_LEN;

    resp[0] = PKG_OK;
    resp[1] = Upgrade_Data;
    resp[2] = src[2];
    resp[3] = src[3];
    resp[4] = 0;
    resp[5] = 0;
    resp[6] = 0;
    resp[7] = 0;
    parm->resp_length = 8;
}

void response_to_server(unsigned char * dst, unsigned char *parm, unsigned short len)
{
    int i;

    dst[0] = 0x68;
    dst[1] = len;
    dst[2] = len >> 8;
    for (i = 0; i < len; i++)
    {
        dst[3 + i] = parm[i];
    }
    dst[3 + len] = 0x16;

    for (i = 0; i < 12; i++)
    {
        isp_send_char(dst[i]);
    }
}

void cmp_upgrade_data(unsigned char * data)
{
    volatile System * parm = (System *)(Global_Parm_Base);
    unsigned char * upgrade_buffer = (unsigned char *)(RAM_BASE + APP_OFFSET_SRAM);
    unsigned char * src = data;
    unsigned char * resp = parm->response;
    unsigned short len = src[0] + src[1] * 256;
    int i;
    unsigned short cs = 0;
    unsigned int remote_cs, local_cs;

    if (len != 4 + 2)
    {
        resp[0] = ERR_LEN;
        resp[1] = 0;
        resp[2] = 0;
        resp[3] = 0;
        resp[4] = 0;
        resp[5] = 0;
        resp[6] = 0;
        resp[7] = 0;
        parm->resp_length = 8;
        return;
    }

    for (i = 0; i < len - 2; i++)
    {
        cs += src[2 + i];
    }

    if (cs != (src[len] + src[len + 1] * 256))
    {
        resp[0] = ERR_CS;
        resp[1] = 0;
        resp[2] = 0;
        resp[3] = 0;
        resp[4] = 0;
        resp[5] = 0;
        resp[6] = 0;
        resp[7] = 0;
        parm->resp_length = 8;
        return;
    }

    remote_cs = src[2] + (src[3] << 8) + (src[4] << 16) + (src[5] << 24);
    local_cs = 0;
    for (i = 0; i < parm->upgrade_total_len; i++)
    {
        local_cs += upgrade_buffer[i];
    }

    if (local_cs != remote_cs)
    {
        resp[0] = ERR_CMP_CS;
        resp[1] = 0;
        resp[2] = 0;
        resp[3] = 0;
        resp[4] = 0;
        resp[5] = 0;
        resp[6] = 0;
        resp[7] = 0;
        parm->resp_length = 8;
        return;
    }

    resp[0] = PKG_OK;
    resp[1] = Upgrade_End;
    resp[2] = 0;
    resp[3] = 0;
    resp[4] = 0;
    resp[5] = 0;
    resp[6] = 0;
    resp[7] = 0;
    parm->resp_length = 8;
}
