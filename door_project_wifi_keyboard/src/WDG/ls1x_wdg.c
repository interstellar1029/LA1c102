#include "ls1x_wdg.h"
#include "ls1c102.h"

extern HW_PMU_t *g_pmu;

void WdgInit(void)
{
    WDG_SetWatchDog(0x80007fff);// 看门狗最长延时
    // time = 0x80007fff = 0b10000000000000000111111111111111
    // time = 0x80007fff = 0b1000000000000000 0111111111111111
    // ~time =             0b1000000000000000 0000000000000000
    // PMU->WDT_CFG =      0b1000000000000000 0111111111111111
    WDG_DogFeed();
}

/*
time = 0x8000_7fff
~time = 0x7fff_8000
~time<<16 = 0x8000_0000
(~time<<16) | time = 0x8000_7fff
*/

/*
    设置看门狗复位时间
    time:32位数据，高16位数据是低16位数据取反
*/
void WDG_SetWatchDog(uint32_t time)
{
    WDG_DogFeed();
    g_pmu->WdtCfg = (((~time)<<16) | time);// WdtCfg 配置必须正确，否则直接复位。
}

void WDG_DogFeed(void)
{
    g_pmu->WdtFeed = WDTFEED_FOOD;
}

void my_delay(volatile int i)
{
    while(i--);
}



