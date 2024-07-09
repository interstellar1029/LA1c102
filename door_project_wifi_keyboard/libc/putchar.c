/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
    �� printf.h ��ʹ���ⲿ���������� extern int _putchar(char character, uint32_t uart_num);
    �Ϳ����ú��� _putchar �ˡ�
*/

#include "console.h"

int putchar(char ch, uint32_t uart_num)
{
    console_putch(ch, uart_num);
    return 0;
}

int _putchar(int ch, uint32_t uart_num)
{
    console_putch(ch, uart_num);
    return 0;
}

int puts(const char *s, uint32_t uart_num)
{
    int count = 0;
    while (*s)
    {
        putchar(*s++, uart_num);
        count++;
    }
    return count;
}


