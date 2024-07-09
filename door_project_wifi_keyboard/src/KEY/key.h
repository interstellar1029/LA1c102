#ifndef _KEY_H
#define _KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ls1c102.h"

#define out0 13
#define out1 14
#define out2 16
#define out3 17

#define int0 34
#define int1 35
#define int2 36
#define int3 37

void key_init();
void key_write(uint8_t value);
uint8_t key_read();
uint8_t key_scan();

#ifdef __cplusplus
}
#endif

#endif // _KEY_H

