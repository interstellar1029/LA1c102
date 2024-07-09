/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
    在 printf.h 中使用外部函数声明： extern int _putchar(char character, uint32_t uart_num);
    就可以用函数 _putchar 了。
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


