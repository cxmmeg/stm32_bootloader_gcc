#include "incs.h"
#include <stdio.h>

static void print_usage(void)
{
    delay_ms(1000);
    put_string("***************************************************\n\r");
    put_string("*             STM32F10X Boot Menu v2.0            *\n\r");
    put_string("***************************************************\n\r");
    put_string("[a]: Test LED.\n\r");
    put_string("[b]: Uart Rx App, RX Address:0x20000200.\n\r");
    put_string("[c]: Jump To SRAM, Address:0x20000200.\n\r");
    put_string("[d]: Write App To Flash, Source Address: 0x20000200.\n\r");
    put_string("[e]: Run App, Entry Address: 0x08004000.\n\r");
    put_string("[f]: Read App And Boot From 0x20000200.\n\r");
    put_string("[g]: Print Mem, Address:0x20000200.\n\r");
}


static void Jump_2_App(unsigned int addr)
{
    pFunction ApplicationRun;

    ApplicationRun = (pFunction)(*(unsigned int*)(addr + 4));
    __set_MSP(*(unsigned int*)addr);
    ApplicationRun();
}

#ifndef STDIO

static unsigned char get_user_input(void)
{
    char temp[20] = {0};
    int length = 0;
    unsigned char ret;
    unsigned char dat;

    while (1)
    {
        dat = isp_rece_char();
        temp[length] = dat;
        length++;
        if ((char)dat == '\n')
        {
            break;
        }
    }
    ret = temp[0];

    return ret;
}

#endif


static void led_test(void)
{
    volatile int loops = 20;
    while (--loops)
    {
        Led0_Inversion();
        Led1_Inversion();
        delay_ms(1000);
    }
}

static void isp_rx_app(void)
{
    unsigned char cmd = 0xFF;
    volatile System * system = (System *)(Global_Parm_Base);
    system->length = 0;
    system->rx_finish = 0;
    system->rx_mark = 0;
    cmd = 0xFF;
    put_string("Wait user upload app, rx address: 0x20000200\n\r");

    while (1)
    {
        isp_rx_pkg();
        if (system->rx_finish)
        {
            cmd = system->cmd;

            switch (cmd)
            {
            case Upgrade_Data:
            {
                handle_upgrade_data(system->rx_buffer);
                response_to_server(system->rx_buffer, system->response, system->resp_length);
                break;
            }
            case Upgrade_End:
            {
                cmp_upgrade_data(system->rx_buffer);
                response_to_server(system->rx_buffer, system->response, system->resp_length);
                system->cmd = 0xFF;
                system->length = 0;
                system->rx_finish = 0;
                return;
            }
            default:
                break;
            }

            system->length = 0;
            system->rx_finish = 0;
        }
        Led0_Inversion();
        Led1_Inversion();
    }
}

static void copy_text_to_SRAM(unsigned int src, unsigned int dst)
{
    unsigned long *pulSrc, *pulDest;

    unsigned int _edata = dst + APP_MAX_SIZE;

    pulSrc = (unsigned long *)src;

    for (pulDest = (unsigned long *)dst; pulDest < (unsigned long *)_edata; )
    {
        *(pulDest++) = *(pulSrc++);
    }
}

static unsigned char FlashWrite(unsigned int offset, unsigned char *pBuffer, unsigned short wLength)
{
    unsigned int dw, addr;

    if (offset % FLASH_PAGE_ERASE_SIZE == 0)
    {
        if (FLASH_ErasePage(offset + FLASH_BASE + APP_OFFSET_FLASH) != FLASH_COMPLETE)
        {
            return FALSE;
        }
    }

    addr = FLASH_BASE + APP_OFFSET_FLASH + offset;

    while (wLength)
    {
        dw = *(unsigned int *)pBuffer;

        if (wLength < 4)
        {
            wLength = 4;
        }
        if (FLASH_ProgramWord(addr, dw) != FLASH_COMPLETE)
        {
            return FALSE;
        }

        addr += 4;
        pBuffer += 4;
        wLength -= 4;
    }

    return TRUE;
}

static void write_firm_to_Flash(unsigned int src, unsigned int dst)
{
    unsigned int AppOffset = 0;
    System * parm = (System *)(Global_Parm_Base);
    unsigned char * s = (unsigned char *)(src);
    unsigned short TotalLen = parm->upgrade_total_len;

    FLASH_Unlock();
    while (TotalLen)
    {
        if (TotalLen >= 512)
        {
            FlashWrite(AppOffset, s + AppOffset, 512);
            TotalLen -= 512;
            AppOffset += 512;
        }
        else
        {
            FlashWrite(AppOffset, s + AppOffset, TotalLen);
            AppOffset += TotalLen;
            TotalLen = 0;
        }
        Led0_Inversion();
        Led1_Inversion();
    }
    FLASH_Lock();

    delay_ms(1000);
}

int main()
{
    unsigned char menu_index;
    char str[100];
    NVIC_Configuration();
    system_init();
    delay_init();
    LED_Init();
    isp_init();
    led_test();

    while (1)
    {
        print_usage();
#ifdef STDIO
        scanf("%s", str);
        menu_index = str[0];
#else
        menu_index = get_user_input();
#endif

        switch (menu_index)
        {
        case 'a':
        {
            put_string("a\n\r");
            led_test();
            break;
        }
        case 'b':
        {
            put_string("b\n\r");
            isp_rx_app();
            break;
        }
        case 'c':
        {
            put_string("c\n\r");
            Jump_2_App(RAM_BASE + APP_OFFSET_SRAM);
            break;
        }
        case 'd':
        {
            put_string("d\n\r");
            write_firm_to_Flash(RAM_BASE + APP_OFFSET_SRAM, FLASH_BASE + APP_OFFSET_FLASH);
            break;
        }
        case 'e':
        {
            put_string("e\n\r");
            Jump_2_App(FLASH_BASE + APP_OFFSET_FLASH);
            break;
        }
        case 'f':
        {
            put_string("f\n\r");
            copy_text_to_SRAM(FLASH_BASE + APP_OFFSET_FLASH, RAM_BASE + APP_OFFSET_SRAM);
            Jump_2_App(RAM_BASE + APP_OFFSET_SRAM);
            break;
        }
        case 'g':
        {
            put_string("g\n\r");
            print_mem(RAM_BASE + APP_OFFSET_SRAM, 512);
            break;
        }
        default:
        {
            put_string("unknown menu index.\n\r");
            break;
        }
        }
    }
}
