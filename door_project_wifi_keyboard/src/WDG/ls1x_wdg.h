#ifndef _LS1X_WDG_H_
#define _LS1X_WDG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ls1c102.h"// ÒªÓÃ uint32_t

void WDG_SetWatchDog(uint32_t time);
void WDG_DogFeed(void);
void WdgInit(void);

void my_delay(volatile int i);

#ifdef __cplusplus
}
#endif

#endif // _LS1X_WDG_H_
