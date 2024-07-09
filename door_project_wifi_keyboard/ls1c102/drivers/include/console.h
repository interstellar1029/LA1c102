#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ls1c102.h"

void console_init(unsigned int baudrate);

char console_getch(uint32_t uart_num);
void console_putch(char ch, uint32_t uart_num);
void console_putstr(char *s, uint32_t uart_num);

void console0_init(unsigned int baudrate);
void console2_init(unsigned int baudrate);

void USART_SendData(char *buf_all, int uart_rx_count, uint32_t uart_num);

#ifdef __cplusplus
}
#endif

#endif /*__CONSOLE_H__*/

