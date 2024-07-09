#ifndef __TEST_H__
#define __TEST_H__

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef __cplusplus
  #define     __I     volatile                /*!< defines 'read only' permissions      */
#else
  #define     __I     volatile const          /*!< defines 'read only' permissions      */
#endif
#define     __O     volatile                  /*!< defines 'write only' permissions     */
#define     __IO    volatile                  /*!< defines 'read / write' permissions   */

#define TRUE  1
#define FALSE 0

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))// 假设是 GPIO7 ，0 |= 10000000 = 10000000 。
/*
0 到 7 位分别对应 GPIO0 到 GPIO7
8 到 15 位分别对应 GPIO16 到 GPIO23
16 到 23 位分别对应 GPIO32 到 GPIO39
24 到 31 位分别对应 GPIO48 到 GPIO55
所以输入的还是0~31，但是实际使用时物理接线需要自己加0、8、16、24
假设是电平模式， GPIO16 ，0 |= 100000000 = 100000000 。
*/

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#ifdef __cplusplus
} 
#endif

#endif

