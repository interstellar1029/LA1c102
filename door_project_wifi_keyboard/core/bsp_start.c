#include <string.h>
#include <larchintrin.h>

#include "bsp.h"
#include "cpu.h"

#include "ls1c102.h"
#include "clock.h"

//-----------------------------------------------------------------------------

extern void exception_common_entry(void);
extern void console_init(unsigned int baudrate);
extern int main(void);

//-----------------------------------------------------------------------------

/*
 * global Variable
 */
unsigned int cpu_frequency;     // CPU 工作频率
unsigned int bus_frequency;     // BUS 工作频率

/******************************************************************************
 * 全局控制器 
 */
HW_PMU_t *g_pmu = (HW_PMU_t *)LS1C102_PMU_BASE;

#if 1
/*
 * 使用外部 8M 时钟配置
 */
static void OSC_init_outer(void)
{
    g_pmu->ChipCtrl &= ~CHIPCTRL_8M_SEL;// 选内部8M时钟
    g_pmu->ChipCtrl |= CHIPCTRL_8M_EN;// 外部8M时钟使能

    //TODO;     /* 不加延时，睡眠起来后会时钟失效? 用loop 计数循环 */

    while (g_pmu->CmdSts & CMDSR_8M_FAIL)// 当外部8M时钟失效时，循环。可能因为使能了，所以不会失效，所以不进入循环。
    {
        g_pmu->ChipCtrl &= ~CHIPCTRL_8M_SEL;// 选内部8M时钟
    }

    g_pmu->ChipCtrl |= CHIPCTRL_8M_SEL;// 选外部8M时钟
}
#endif

static void get_frequency(void)
{
    unsigned int val = g_pmu->CmdSts;

    if ((val & CMDSR_8M_SEL) && !(val & CMDSR_8M_FAIL)) /* 使用外部 8M 晶振*/
    {
        cpu_frequency = 8000000;
        bus_frequency = 8000000;
    }
    else
    {
#if 0
        /*
         * 这个内置晶振频率在寄存器 0xBF0201B0 中?
         */
        cpu_frequency = (*(volatile unsigned int *)0xBF0201B0) * 1000;

        if (cpu_frequency > 0)
        {
            bus_frequency = cpu_frequency;
            return;
        }
#endif

        /*
         * 内部 32MHZ 晶振？
         */
        val = g_pmu->ChipCtrl;
        
        if (val & CHIPCTRL_FASTEN)      // 3 分频
        {
            cpu_frequency = 32000000 / 3;
            bus_frequency = 32000000 / 3;
        }
        else                            // 4 分频
        {
            cpu_frequency = 8000000;
            bus_frequency = 8000000;
        }
    }
}

/****************************************************************************** 
 * ls1c102 bsp start
 */

extern void ls1c102_init_isr_table(void);

void bsp_start(void)
{
    unsigned int eentry;

    loongarch_interrupt_disable();

    /**
     * 把中断代码复制到0x80000000
     */
    eentry = __csrrd_w(LA_CSR_EBASE);
    memcpy((void *)eentry, (void *)exception_common_entry, 32);

    g_pmu->CommandW = 0;            // CMDSR_SLEEP_EN;

    OSC_init_outer();               // 设置为外部8M时钟， TODO delay_ms() 不运行。注释了不影响延时。
    // 不使用外部 8M 时钟会导致串口打印乱码，可能是内部时钟的频率不对，和波特率不匹配。
    // 默认应该是使用内部8M时钟，因为延时函数延时基本一致。
    /*
        配置选择外部32K时钟没有用，可能是没有连接到电路中。
        配置选择内部32K时钟没有用。
        可能是默认选择了内部8M时钟，不能再选这个32K。
    */
    
    get_frequency();                /* 获取频率配置 */

    ls1c102_init_isr_table();       /* 初始化中断向量表 */

    console_init(115200);           /* initialize Console */

    Clock_initialize();             /* initialize system ticker */

    loongarch_interrupt_enable();   /* Enable all interrupts */

    main();                         /* goto main function */
    
    while (1);                      /* XXX never goto HERE! */
    
    return;
}


