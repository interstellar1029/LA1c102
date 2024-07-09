#include <stdio.h>
#include <larchintrin.h>

#include "cpu.h"
#include "regdef.h"

#include "ls1c102.h"
#include "ls1c102_irq.h"

//-------------------------------------------------------------------------------------------------

void dump_registers(unsigned int *regs)
{
	printk("Dump Registers:\r\n");

	printk("zero = 0x%08x\r\n", regs[R_ZERO]);
	printk("  ra = 0x%08x\r\n", regs[R_RA]);
	printk("  tp = 0x%08x\r\n", regs[R_TP]);
	printk("  sp = 0x%08x\r\n", regs[R_SP]);
	printk("  a0 = 0x%08x\r\n", regs[R_A0]);
	printk("  a1 = 0x%08x\r\n", regs[R_A1]);
	printk("  a2 = 0x%08x\r\n", regs[R_A2]);
	printk("  a3 = 0x%08x\r\n", regs[R_A3]);
	printk("  a4 = 0x%08x\r\n", regs[R_A4]);
	printk("  a5 = 0x%08x\r\n", regs[R_A5]);
	printk("  a6 = 0x%08x\r\n", regs[R_A6]);
	printk("  a7 = 0x%08x\r\n", regs[R_A7]);
	printk("  t0 = 0x%08x\r\n", regs[R_T0]);
	printk("  t1 = 0x%08x\r\n", regs[R_T1]);
	printk("  t2 = 0x%08x\r\n", regs[R_T2]);
	printk("  t3 = 0x%08x\r\n", regs[R_T3]);
	printk("  t4 = 0x%08x\r\n", regs[R_T4]);
	printk("  t5 = 0x%08x\r\n", regs[R_T5]);
	printk("  t6 = 0x%08x\r\n", regs[R_T6]);
	printk("  t7 = 0x%08x\r\n", regs[R_T7]);
	printk("  t8 = 0x%08x\r\n", regs[R_T8]);
	printk("  x  = 0x%08x\r\n", regs[R_X ]);
	printk("  fp = 0x%08x\r\n", regs[R_FP]);
	printk("  s0 = 0x%08x\r\n", regs[R_S0]);
	printk("  s1 = 0x%08x\r\n", regs[R_S1]);
	printk("  s2 = 0x%08x\r\n", regs[R_S2]);
	printk("  s3 = 0x%08x\r\n", regs[R_S3]);
	printk("  s4 = 0x%08x\r\n", regs[R_S4]);
	printk("  s5 = 0x%08x\r\n", regs[R_S5]);
	printk("  s6 = 0x%08x\r\n", regs[R_S6]);
	printk("  s7 = 0x%08x\r\n", regs[R_S7]);
	printk("  s8 = 0x%08x\r\n", regs[R_S8]);
	printk("prmd = 0x%08x\r\n", regs[R_PRMD]);
	printk("ecfg = 0x%08x\r\n", regs[R_ECFG]);
	printk("estat= 0x%08x\r\n", regs[R_ESTAT]);
	printk("epc  = 0x%08x\r\n", regs[R_ERA]);

	printk("\r\n");
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

static char *get_exception_description(unsigned int execcode)
{
	char *rt = NULL;

	switch (execcode)
	{
		case CSR_ESTAT_ECODE_PIL:   rt = "Invalid exception for Load operation exception."; break;
		case CSR_ESTAT_ECODE_PIS:   rt = "Invalid exception for Store operation exception."; break;
		case CSR_ESTAT_ECODE_PIF:   rt = "Invalid exception for Fetch operation exception."; break;
		case CSR_ESTAT_ECODE_PME:   rt = "Page Modification Exception."; break;
		case CSR_ESTAT_ECODE_PPI:   rt = "Page Privilege level Illegal exception."; break;
		case CSR_ESTAT_ECODE_ADEF:  rt = "Address error Exception for Memory access instructions exception."; break;
		case CSR_ESTAT_ECODE_ADEM:  rt = "Address error Exception for Fetching instructions exception."; break;
		case CSR_ESTAT_ECODE_ALE:   rt = "Address alignment fault Exception."; break;
		case CSR_ESTAT_ECODE_SYS:   rt = "System call exception."; break;
		case CSR_ESTAT_ECODE_BREAK: rt = "Breakpoint exception."; break;
		case CSR_ESTAT_ECODE_INE:   rt = "Instruction Non-defined Exception."; break;
		case CSR_ESTAT_ECODE_IPE:   rt = "Instruction Privilege error Exception."; break;
		case CSR_ESTAT_ECODE_FPD:   rt = "Floating-Point instruction Disable exception."; break;
		case CSR_ESTAT_ECODE_FPE:   rt = "Floating-Point error Exception."; break;
        case CSR_ESTAT_ECODE_TLBR:  rt = "TLB-Refill error Exception."; break;
        default:                    rt = "Unknown Exception."; break;
	}

	return rt;
}

//-------------------------------------------------------------------------------------------------

void c_exception_handler(unsigned int *regs)
{
    volatile unsigned int execcode;

    loongarch_interrupt_disable();

    execcode  = regs[R_ESTAT];
    execcode &= CSR_ESTAT_ESUBCODE_MASK | CSR_ESTAT_ECODE_MASK;
    
    printk("Unhandled exception: %s\r\n", get_exception_description(execcode));

    execcode = __csrrd_w(LA_CSR_BADV);
    printk("badvaddr == 0x%08x\r\n", execcode);

    dump_registers(regs);

    /*
     * dead loop
     */
    for (;;);

    /*
     * NEVER GO HERE!!!
     */
}

//-------------------------------------------------------------------------------------------------

/*
 * @@END
 */



