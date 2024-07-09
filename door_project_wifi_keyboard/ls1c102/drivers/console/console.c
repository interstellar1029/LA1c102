#include <stdbool.h>
 
#include "termios.h"
#include "ls1c102.h"

#include "bsp.h"

#include "ns16550.h"
#include "console.h"

void console_init(unsigned int baudrate)
{
    uint32_t uart_num = 1;
    struct termios t;
    NS16550_init(ConsolePort, NULL, uart_num);// initial // UART 复用 、 初始化 lcr R1.ier 、 fcr 、 bInterrupt 使能/失能
    t.c_cflag = BAUDRATE_TO_CFLAG(baudrate) | CS8;// 8N1
    // = B115200 | CS8 = 0010002 | 0000060 = 0b1000000_000010 | 0b110000 = 0b1000000_110010
    NS16550_open(ConsolePort, (void *)&t, uart_num);// initial // 初始化 rx_buf tx_buf 、 R1.ier 、 (samp 、 lcr 、 dll 、 dlh 、 dld 、 ier)
}

void console0_init(unsigned int baudrate)
{
    uint32_t uart_num = 0;
    struct termios t0;
    NS16550_init(ConsolePort0, NULL, uart_num);
    t0.c_cflag = BAUDRATE_TO_CFLAG(baudrate) | CS8;	// 8N1
    // = B115200 | CS8 = 0010002 | 0000060 = 0b1000000_000010 | 0b110000 = 0b1000000_110010
    NS16550_open(ConsolePort0, (void *)&t0, uart_num);
}

void console2_init(unsigned int baudrate)// 使用32K时钟
{
    uint32_t uart_num = 2;
    struct termios t2;
    NS16550_init(ConsolePort2, NULL, uart_num);
    t2.c_cflag = BAUDRATE_TO_CFLAG(baudrate) | CS8;	// 8N1
    // = B9600 | CS8 = 0000015 | 0000060 = 0b1101 | 0b110000 = 0b111101
    NS16550_open(ConsolePort2, (void *)&t2, uart_num);
}

char console_getch(uint32_t uart_num)
{
    if(uart_num == 0)
        return Console_get_char(ConsolePort0);
    else if(uart_num == 1)
        return Console_get_char(ConsolePort);
    else if(uart_num == 2)
        return Console_get_char(ConsolePort2);
    return Console_get_char(ConsolePort);
}

void console_putch(char ch, uint32_t uart_num)
{
    if(uart_num == 0)
        Console_output_char(ConsolePort0, ch);
    else if(uart_num == 1)
        Console_output_char(ConsolePort, ch);
    else if(uart_num == 2)
        Console_output_char(ConsolePort2, ch);
}

void console_putstr(char *s, uint32_t uart_num)// *s：待发送字符串，uart_num：串口号
{
    while (*s)
    {
		if(uart_num == 0)
            console_putch(*s++, uart_num);
        else if(uart_num == 1)
            console_putch(*s++, uart_num);
        else if(uart_num == 2)
            console_putch(*s++, uart_num);
    }
}

void USART_SendData(char *buf_all, int uart_rx_count, uint32_t uart_num)// *s：待发送字符串，uart_rx_count：发送长度，uart_num：串口号
{
    for(int i=0; i<uart_rx_count; i++)
    {
        console_putch(*buf_all, uart_num);
        buf_all++;
    }
}


