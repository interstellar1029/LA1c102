#include "asm.h"
#include "cpu.h"
#include "regdef.h"

#include "context.h"

//-----------------------------------------------------------------------------
// ld.script 中的变量
//-----------------------------------------------------------------------------

    .extern __DRamBase, 4
    .extern __DRamSize, 4
    
    .extern __rodata_start, 4
    .extern __rodata_end, 4

    .extern __data_start, 4
    .extern __data_end, 4

    .extern __bss_start, 4
    .extern __bss_end, 4

//-----------------------------------------------------------------------------
// 外部函数
//-----------------------------------------------------------------------------

    .extern bsp_start
    
    .text
    
//-----------------------------------------------------------------------------
// 启动代码, 链接到 0x1c000000
//-----------------------------------------------------------------------------

    .align 3
    .globl start
    .globl _start
start:
_start:

    li.w        t0, CSR_CRMD_IE             /* disable interrupt */
    csrxchg     zero, t0, LA_CSR_CRMD       /* clear IE bit */

    li.w        t1, CSR_ECFG_IM_MASK        /* set interrupt mask 0x1fff */
    csrwr       t1, LA_CSR_ECFG

    li.w        t2, 0x80000000              /* set common-exception entry */
    csrwr       t2, LA_CSR_EBASE

#if 0
    /**
     * XXX ls1c102 没有 TLB
     */
    li.w        t3, 0x80000040              /* set tlb-refill error entry */
    csrwr       t3, LA_CSR_TLBREBASE
#endif

    la.abs      gp, _gp

    li.w        sp, 0x80000ffc              /* 栈顶设置在 IRAM 内存的顶部 (低4K) */

    /*
     * initialized data, move to DRAM zone
     */
    la          t0, __data_start            // 0x1c008e70
    la          t1, __data_end              // 0x800010a0
    la          t2, __DRamBase              // 0x80001000
    nop

1:
    ld.w        t3, t0, 0
    st.w        t3, t2, 0
    addi.w      t2, t2, 4
    addi.w      t0, t0, 4
    blt         t2, t1, 1b
    nop

    /*
     * clear bss
     */
    la          t0, __bss_start             // 0x800010a0
    la          t1, __bss_end               // 0x800013c0
    nop
2:
    st.w        zero, t0, 0
    addi.w      t0, t0, 4
    ble         t0, t1, 2b
    nop
    
    bl          bsp_start                   /* 跳到 bsp_start 入口 */
    nop

loop:                                       /* 如果main函数返回, 则死循环 */
    nop
    b           loop
    nop
    nop
    
    .end start

//-----------------------------------------------------------------------------

/*
 * @@END
 */

