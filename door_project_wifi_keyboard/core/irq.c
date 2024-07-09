#include <stdio.h>
#include <larchintrin.h>

#include "cpu.h"
#include "regdef.h"

#include "ls1c102.h"
#include "ls1c102_irq.h"

extern int printk(const char* format, ...);

//-----------------------------------------------------------------------------
// 全局变量
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 中断向量表
//-----------------------------------------------------------------------------

typedef struct
{
    irq_handler_t   isr;
    void           *arg;
} ISR_Table_t;

static ISR_Table_t isr_table[LS1C102_IRQ_COUNT];    // 在内存中

//-----------------------------------------------------------------------------
// 初始化中断向量表
//-----------------------------------------------------------------------------

static void ls1c102_default_isr(int vector, void *arg);

void ls1c102_init_isr_table(void)
{
    int i;
    
    for (i=0; i<LS1C102_IRQ_COUNT; i++)
    {
        isr_table[i].isr = ls1c102_default_isr;
    }
}

//-----------------------------------------------------------------------------
// 默认中断
//-----------------------------------------------------------------------------
static inline char *ls1c102_irq_description(int vector)
{
	char *rt = NULL;

	switch (vector)
	{
        /* CPU IP Interrupts
         */
		case LS1C102_IRQ_SW0: 		rt = "Software0"; 	break;
		case LS1C102_IRQ_SW1: 		rt = "Software1"; 	break;
		case LS1C102_IRQ_WAKEUP: 	rt = "WakeUp"; 	    break;
		case LS1C102_IRQ_TOUCH: 	rt = "Touch"; 	    break;
		case LS1C102_IRQ_UART2: 	rt = "UART2"; 	    break;
		case LS1C102_IRQ_TICKER: 	rt = "Ticker"; 	    break;

        /* IP5 Interrupts from PMU
         */
		case LS1C102_IRQ_BATFAIL:   rt = "BatFail";     break;
		case LS1C102_IRQ_32KFAIL:   rt = "C32KFail";    break;
		case LS1C102_IRQ_8MFAIL:    rt = "C8MFail";     break;
		case LS1C102_IRQ_RTC:       rt = "RTC";         break;
		case LS1C102_IRQ_ADC:       rt = "ADC";         break;

        /* IP6 Interrupts from INTC
         */
		case LS1C102_IRQ_TIMER:     rt = "Timer";       break;
		case LS1C102_IRQ_I2C:       rt = "I2C";         break;
		case LS1C102_IRQ_UART1:     rt = "UART1";       break;
		case LS1C102_IRQ_UART0:     rt = "UART0";       break;
		case LS1C102_IRQ_FLASH:     rt = "Flash";       break;
		case LS1C102_IRQ_SPI:       rt = "SPI";         break;
		case LS1C102_IRQ_VPWM:      rt = "VPWM";        break;
		case LS1C102_IRQ_DMA:       rt = "DMA";         break;

        /* IP7 Interrupts from ExtINT
         */
		case LS1C102_IRQ_GPIO0:     rt = "gpio0";   break;
		case LS1C102_IRQ_GPIO1:     rt = "gpio1";   break;
		case LS1C102_IRQ_GPIO2:     rt = "gpio2";   break;
		case LS1C102_IRQ_GPIO3:     rt = "gpio3";   break;
		case LS1C102_IRQ_GPIO4:     rt = "gpio4";   break;
		case LS1C102_IRQ_GPIO5:     rt = "gpio5";   break;
		case LS1C102_IRQ_GPIO6:     rt = "gpio6";   break;
		case LS1C102_IRQ_GPIO7:     rt = "gpio7";   break;
		case LS1C102_IRQ_GPIO16:    rt = "gpio16";  break;
		case LS1C102_IRQ_GPIO17:    rt = "gpio17";  break;
		case LS1C102_IRQ_GPIO18:    rt = "gpio18";  break;
		case LS1C102_IRQ_GPIO19:    rt = "gpio19";  break;
		case LS1C102_IRQ_GPIO20:    rt = "gpio20";  break;
		case LS1C102_IRQ_GPIO21:    rt = "gpio21";  break;
		case LS1C102_IRQ_GPIO22:    rt = "gpio22";  break;
		case LS1C102_IRQ_GPIO23:    rt = "gpio23";  break;
		case LS1C102_IRQ_GPIO32:    rt = "gpio32";  break;
		case LS1C102_IRQ_GPIO33:    rt = "gpio33";  break;
		case LS1C102_IRQ_GPIO34:    rt = "gpio34";  break;
		case LS1C102_IRQ_GPIO35:    rt = "gpio35";  break;
		case LS1C102_IRQ_GPIO36:    rt = "gpio36";  break;
		case LS1C102_IRQ_GPIO37:    rt = "gpio37";  break;
		case LS1C102_IRQ_GPIO38:    rt = "gpio38";  break;
		case LS1C102_IRQ_GPIO39:    rt = "gpio39";  break;
		case LS1C102_IRQ_GPIO48:    rt = "gpio48";  break;
		case LS1C102_IRQ_GPIO49:    rt = "gpio49";  break;
		case LS1C102_IRQ_GPIO50:    rt = "gpio50";  break;
		case LS1C102_IRQ_GPIO51:    rt = "gpio51";  break;
		case LS1C102_IRQ_GPIO52:    rt = "gpio52";  break;
		case LS1C102_IRQ_GPIO53:    rt = "gpio53";  break;
		case LS1C102_IRQ_GPIO54:    rt = "gpio54";  break;
		case LS1C102_IRQ_GPIO55:    rt = "gpio55";  break;
		default:	                rt = "unknow";	break;
	}

	return rt;
}


static void ls1c102_default_isr(int vector, void * arg)
{
	unsigned int *	stack = (unsigned int *)arg;
	unsigned int	ecfg  = stack[R_ECFG];
	unsigned int	estat = stack[R_ESTAT];
	char *	      irqname = ls1c102_irq_description(vector);

	printk("Unhandled isr %s:\r\n  vector=%i, ecfg=0x%08X, estat=0x%08X\n",
		irqname, vector, ecfg, estat);

	return;
}

//-----------------------------------------------------------------------------
// 中断分发
//-----------------------------------------------------------------------------

static void ls1c102_dispatch_isr(int vector, unsigned int *stack)
{
    irq_handler_t isr;
    void *arg;
    
    if ((vector >= 0) && (vector < LS1C102_IRQ_COUNT))
    {
        isr = isr_table[vector].isr;
        arg = isr_table[vector].arg;
        
        if (NULL == isr)
        {
            isr = ls1c102_default_isr;
        }
        if (NULL == arg)
        {
            arg = (void *)stack;
        }
        isr(vector, arg);
    }
}

//-----------------------------------------------------------------------------
// 中断响应 
//-----------------------------------------------------------------------------

extern HW_PMU_t *g_pmu;

void c_interrupt_handler(unsigned int *stack)
{
    unsigned int tmp;
	unsigned int ecfg  = stack[R_ECFG];
	unsigned int estat = stack[R_ESTAT];

    estat &= ecfg & CSR_ESTAT_IS_MASK;
    if (estat == 0)                                 /* 会出现吗? tlbrerr/merr? */
    {
        return;
    }

    /**
     * IP11 定时器中断
     */
    if (estat & CSR_ECFG_TI)
    {
        __csrwr_w(1, LA_CSR_TINTCLR);               /* 清除 Timer 中断 */
        ls1c102_dispatch_isr(LS1C102_IRQ_TICKER, stack);
    }

    /**
     * HW-IP5 External Interrupt: GPIO
     */
	if ((estat & CSR_ECFG_HWI5) && (g_pmu->CmdSts & CMDSR_INTSRC_EXTINT))
	{
	    int i;
	    tmp = g_pmu->ExIntEn & g_pmu->ExIntSrc;
	    
	    for (i=0; i<32; i++)
	    {
            if (tmp & bit(i))
            {
                ls1c102_dispatch_isr(LS1C102_IRQ_GPIO_BASE + i, stack);
            }
        }

        g_pmu->CommandW |= CMDSR_INTSRC_EXTINT;
	}

	/**
	 * HW-IP4 INTC Interrupt
	 */
	if (estat & CSR_ECFG_HWI4)
	{
	    HW_INTC_t *intc = (HW_INTC_t *)LS1C102_INTC_BASE;
	    tmp = (unsigned int)(intc->en & intc->out);

	    if (tmp & INTC_DMA)
	    {
	        ls1c102_dispatch_isr(LS1C102_IRQ_DMA, stack);
        }
      
	    if (tmp & INTC_VPWM)
	    {
	        ls1c102_dispatch_isr(LS1C102_IRQ_VPWM, stack);
        }

	    if (tmp & INTC_SPI)
	    {
	        ls1c102_dispatch_isr(LS1C102_IRQ_SPI, stack);
        }
    
        if (tmp & INTC_FLASH)
        {
            ls1c102_dispatch_isr(LS1C102_IRQ_FLASH, stack);
        }

        if (tmp & INTC_UART0)
        {
            ls1c102_dispatch_isr(LS1C102_IRQ_UART0, stack);
        }

        if (tmp & INTC_UART1)
        {
            ls1c102_dispatch_isr(LS1C102_IRQ_UART1, stack);
        }

        if (tmp & INTC_I2C)
        {
            ls1c102_dispatch_isr(LS1C102_IRQ_I2C, stack);
        }
        
        if (tmp & INTC_TIMER)
        {
            ls1c102_dispatch_isr(LS1C102_IRQ_TIMER, stack);
        }
        
	    intc->clr = 0xFF;         // clear interrupt flag
	}

	/**
	 * HW-IP3 PMU Interrupt
	 */
	if (estat & CSR_ECFG_HWI3)
	{
	    tmp = g_pmu->CmdSts;

	    if (tmp & (CMDSR_INTSRC_ADC | CMDSR_INTEN_ADC))
	    {
	        ls1c102_dispatch_isr(LS1C102_IRQ_ADC, stack);
        }   

	    if (tmp & (CMDSR_INTSRC_RTC | CMDSR_INTEN_RTC))
	    {
	        ls1c102_dispatch_isr(LS1C102_IRQ_RTC, stack);
        }
  
	    if (tmp & (CMDSR_INTSRC_8MFAIL | CMDSR_INTEN_8MFAIL))
	    {
	        ls1c102_dispatch_isr(LS1C102_IRQ_8MFAIL, stack);
        }

	    if (tmp & (CMDSR_INTSRC_32KFAIL | CMDSR_INTEN_32KFAIL))
	    {
	        ls1c102_dispatch_isr(LS1C102_IRQ_32KFAIL, stack);
        }

	    if (tmp & (CMDSR_INTSRC_BATFAIL | CMDSR_INTEN_BATFAIL))
	    {
            ls1c102_dispatch_isr(LS1C102_IRQ_BATFAIL, stack);
        }
     
        g_pmu->CommandW |= CMDSR_INTSRC_ADC |
                           CMDSR_INTSRC_RTC |
                           CMDSR_INTSRC_8MFAIL |
                           CMDSR_INTSRC_32KFAIL |
                           CMDSR_INTSRC_BATFAIL;
    }

	/**
	 * HW-IP2 UART2 Interrupt
	 */
	if ((estat & CSR_ECFG_HWI2) & (g_pmu->CmdSts & (CMDSR_INTSRC_UART2 | CMDSR_INTEN_UART2)))
	{
	    g_pmu->CommandW |= CMDSR_INTSRC_UART2;
	    ls1c102_dispatch_isr(LS1C102_IRQ_UART2, stack);
	}
	
	/**
	 * HW-IP1 Touch Interrupt
	 */
	if ((estat & CSR_ECFG_HWI1) & (g_pmu->CmdSts & (CMDSR_INTSRC_TOUCH | CMDSR_INTEN_TOUCH)))
	{
	    g_pmu->CommandW |= CMDSR_INTSRC_TOUCH;
	    ls1c102_dispatch_isr(LS1C102_IRQ_TOUCH, stack);
	}
	
	/**
	 * HW-IP0 WAKEUP Interrupt
	 */
	if ((estat & CSR_ECFG_HWI0) & (g_pmu->CmdSts & (CMDSR_INTSRC_WAKE | CMDSR_INTEN_WAKE)))
	{
	    g_pmu->CommandW |= CMDSR_INTSRC_WAKE;
	    ls1c102_dispatch_isr(LS1C102_IRQ_WAKEUP, stack);
	}

	/**
	 * SW-IP0 Interrupt
	 */
	if (estat & CSR_ECFG_SWI1)
	{
	    clear_csr_estat(CSR_ECFG_SWI1);
	    ls1c102_dispatch_isr(LS1C102_IRQ_SW1, stack);
	}

	/**
	 * SW-IP0 Interrupt
	 */
	if (estat & CSR_ECFG_SWI0)
	{
	    clear_csr_estat(CSR_ECFG_SWI0);
	    ls1c102_dispatch_isr(LS1C102_IRQ_SW0, stack);
	}

    return;
}

//-----------------------------------------------------------------------------
// SW0/SW1 Interrupt Support
//-----------------------------------------------------------------------------

/*
 * Generate a software interrupt
 */
int assert_sw_irq(unsigned int irqnum)
{
    if ((irqnum == CSR_ECFG_SWI0) || (irqnum == CSR_ECFG_SWI1))
    {
        set_csr_estat(irqnum);
        return irqnum;
    }

    return -1;
}

/*
 * Clear a software interrupt
 */
int negate_sw_irq(unsigned int irqnum)
{
    if ((irqnum == CSR_ECFG_SWI0) || (irqnum == CSR_ECFG_SWI1))
    {
        clear_csr_estat(irqnum);
        return irqnum;
    }
    
    return -1;
}

//-----------------------------------------------------------------------------
// 安装中断
//-----------------------------------------------------------------------------
// typedef void(*irq_handler_t) (int vector, void *arg);
int ls1c102_install_isr(int vector, irq_handler_t isr, void *arg)
{
    if ((vector >= 0) && (vector < LS1C102_IRQ_COUNT))
    {
        isr_table[vector].isr = isr ? isr : ls1c102_default_isr;
        isr_table[vector].arg = arg;
        
        return vector;
    }
    
	return -1;
}

//-----------------------------------------------------------------------------
// 移除中断
//-----------------------------------------------------------------------------

int ls1c102_remove_isr(int vector)
{
    if ((vector >= 0) && (vector < LS1C102_IRQ_COUNT))
    {
        isr_table[vector].isr = ls1c102_default_isr;
        isr_table[vector].arg = NULL;

        return vector;
    }

	return -1;
}

//-----------------------------------------------------------------------------
// 根据中断向量号开关中断
//-----------------------------------------------------------------------------

int ls1c102_interrupt_enable(int vector)
{
    HW_INTC_t *intc = (HW_INTC_t *)LS1C102_INTC_BASE;
    
    switch (vector)
    {
        case LS1C102_IRQ_WAKEUP:    g_pmu->CmdSts |= CMDSR_INTEN_WAKE;    break;
        case LS1C102_IRQ_TOUCH:     g_pmu->CmdSts |= CMDSR_INTEN_TOUCH;   break;
        case LS1C102_IRQ_UART2:     g_pmu->CmdSts |= CMDSR_INTEN_UART2;   break;
        case LS1C102_IRQ_TICKER:    break;
        case LS1C102_IRQ_BATFAIL:   g_pmu->CmdSts |= CMDSR_INTEN_BATFAIL; break;
        case LS1C102_IRQ_32KFAIL:   g_pmu->CmdSts |= CMDSR_INTEN_32KFAIL; break;
        case LS1C102_IRQ_8MFAIL:    g_pmu->CmdSts |= CMDSR_INTEN_8MFAIL;  break;
        case LS1C102_IRQ_RTC:       g_pmu->CmdSts |= CMDSR_INTEN_RTC;     break;
        case LS1C102_IRQ_ADC:       g_pmu->CmdSts |= CMDSR_INTEN_ADC;     break;

        case LS1C102_IRQ_TIMER:     intc->en |= INTC_TIMER; break;
        case LS1C102_IRQ_I2C:       intc->en |= INTC_I2C;   break;
        case LS1C102_IRQ_UART1:     intc->en |= INTC_UART1; break;
        case LS1C102_IRQ_UART0:     intc->en |= INTC_UART0; break;
        case LS1C102_IRQ_FLASH:     intc->en |= INTC_FLASH; break;
        case LS1C102_IRQ_SPI:       intc->en |= INTC_SPI;   break;
        case LS1C102_IRQ_VPWM:      intc->en |= INTC_VPWM;  break;
        case LS1C102_IRQ_DMA:       intc->en |= INTC_DMA;   break;

        default:
            if ((vector >= LS1C102_IRQ_GPIO0) && (vector <= LS1C102_IRQ_GPIO55))
            {
                g_pmu->ExIntEn |= 1 << (vector - LS1C102_IRQ_GPIO_BASE);
                g_pmu->CmdSts  |= CMDSR_EXINTEN;
                break;
            }
            else
            {
                return -1;
            }
    }
    
    return 0;
}

int ls1c102_interrupt_disable(int vector)
{
    HW_INTC_t *intc = (HW_INTC_t *)LS1C102_INTC_BASE;

    switch (vector)
    {
        case LS1C102_IRQ_WAKEUP:    g_pmu->CmdSts &= ~CMDSR_INTEN_WAKE;    break;
        case LS1C102_IRQ_TOUCH:     g_pmu->CmdSts &= ~CMDSR_INTEN_TOUCH;   break;
        case LS1C102_IRQ_UART2:     g_pmu->CmdSts &= ~CMDSR_INTEN_UART2;   break;
        case LS1C102_IRQ_TICKER:    break;
        case LS1C102_IRQ_BATFAIL:   g_pmu->CmdSts &= ~CMDSR_INTEN_BATFAIL; break;
        case LS1C102_IRQ_32KFAIL:   g_pmu->CmdSts &= ~CMDSR_INTEN_32KFAIL; break;
        case LS1C102_IRQ_8MFAIL:    g_pmu->CmdSts &= ~CMDSR_INTEN_8MFAIL;  break;
        case LS1C102_IRQ_RTC:       g_pmu->CmdSts &= ~CMDSR_INTEN_RTC;     break;
        case LS1C102_IRQ_ADC:       g_pmu->CmdSts &= ~CMDSR_INTEN_ADC;     break;

        case LS1C102_IRQ_TIMER:     intc->en &= ~INTC_TIMER; break;
        case LS1C102_IRQ_I2C:       intc->en &= ~INTC_I2C;   break;
        case LS1C102_IRQ_UART1:     intc->en &= ~INTC_UART1; break;
        case LS1C102_IRQ_UART0:     intc->en &= ~INTC_UART0; break;
        case LS1C102_IRQ_FLASH:     intc->en &= ~INTC_FLASH; break;
        case LS1C102_IRQ_SPI:       intc->en &= ~INTC_SPI;   break;
        case LS1C102_IRQ_VPWM:      intc->en &= ~INTC_VPWM;  break;
        case LS1C102_IRQ_DMA:       intc->en &= ~INTC_DMA;   break;

        default:
            if ((vector >= LS1C102_IRQ_GPIO0) && (vector <= LS1C102_IRQ_GPIO55))
            {
                g_pmu->ExIntEn &= ~(1 << (vector - LS1C102_IRQ_GPIO_BASE));
                if (0 == g_pmu->ExIntEn)
                {
                    g_pmu->CmdSts &= ~CMDSR_EXINTEN;
                }
                break;
            }
            else
            {
                return -1;
            }
    }

    return 0;
}

//-----------------------------------------------------------------------------
/*
 * @@ END
 */

