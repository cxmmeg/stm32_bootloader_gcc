#include "incs.h"


#ifdef STDIO
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
/*
 * This is a pretty classic ring buffer for characters
 */
#define BUFLEN 127

static uint16_t start_ndx;
static uint16_t end_ndx;
static char buf[BUFLEN + 1];
#define buf_len ((end_ndx - start_ndx) % BUFLEN)
static inline int inc_ndx(int n) { return ((n + 1) % BUFLEN); }
static inline int dec_ndx(int n) { return (((n + BUFLEN) - 1) % BUFLEN); }

/* back up the cursor one space */
static inline void back_up(void)
{
    end_ndx = dec_ndx(end_ndx);
    usart_send_blocking(USART1, '\010');
    usart_send_blocking(USART1, ' ');
    usart_send_blocking(USART1, '\010');
}


void usart_send_blocking(USART_TypeDef * uart, unsigned char val)
{
    while (USART_GetFlagStatus(uart, USART_FLAG_TXE) == RESET);
    uart->DR = val;
}

unsigned char usart_recv_blocking(USART_TypeDef * uart)
{
    unsigned short dat;
    while (USART_GetFlagStatus(uart, USART_FLAG_RXNE) == RESET);
    dat = uart->DR;
    return (unsigned char)dat;
}

/*
 * A buffered line editing function.
 */
void get_buffered_line(void)
{
    char c;

    if (start_ndx != end_ndx)
    {
        return;
    }

    while (1)
    {
        c = usart_recv_blocking(USART1);

        if (c == '\r')
        {
            buf[end_ndx] = '\n';
            end_ndx = inc_ndx(end_ndx);
            buf[end_ndx] = '\0';
            usart_send_blocking(USART1, '\r');
            usart_send_blocking(USART1, '\n');
            return;
        }

        /* or DEL erase a character */
        if ((c == '\010') || (c == '\177'))
        {
            if (buf_len == 0)
            {
                usart_send_blocking(USART1, '\a');
            }

            else
            {
                back_up();
            }

            /* erases a word */
        }

        else if (c == 0x17)
        {
            while ((buf_len > 0) &&
                    (!(isspace((int) buf[end_ndx]))))
            {
                back_up();
            }

            /* erases the line */
        }

        else if (c == 0x15)
        {
            while (buf_len > 0)
            {
                back_up();
            }

            /* Non-editing character so insert it */
        }

        else
        {
            if (buf_len == (BUFLEN - 1))
            {
                usart_send_blocking(USART1, '\a');
            }

            else
            {
                buf[end_ndx] = c;
                end_ndx = inc_ndx(end_ndx);
                usart_send_blocking(USART1, c);
            }
        }
    }
}

int _read(int fd, char *ptr, int len)
{
    int my_len;

    if (fd > 2)
    {
        return -1;
    }

    get_buffered_line();
    my_len = 0;

    while ((buf_len > 0) && (len > 0))
    {
        *ptr++ = buf[start_ndx];
        start_ndx = inc_ndx(start_ndx);
        my_len++;
        len--;
    }

    return my_len; /* return the length we got */
}

/*
 * Called by libc stdio fwrite functions
 */
int _write(int fd, char *ptr, int len)
{
    int i = 0;

    /*
     * write "len" of char from "ptr" to file id "fd"
     * Return number of char written.
     *
    * Only work for STDOUT, STDIN, and STDERR
     */
    if (fd > 2)
    {
        return -1;
    }

    while (*ptr && (i < len))
    {
        usart_send_blocking(USART1, *ptr);

        if (*ptr == '\n')
        {
            usart_send_blocking(USART1, '\r');
        }

        i++;
        ptr++;
    }

    return i;
}

#endif
