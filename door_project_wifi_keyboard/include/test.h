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

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))// ������ GPIO7 ��0 |= 10000000 = 10000000 ��
/*
0 �� 7 λ�ֱ��Ӧ GPIO0 �� GPIO7
8 �� 15 λ�ֱ��Ӧ GPIO16 �� GPIO23
16 �� 23 λ�ֱ��Ӧ GPIO32 �� GPIO39
24 �� 31 λ�ֱ��Ӧ GPIO48 �� GPIO55
��������Ļ���0~31������ʵ��ʹ��ʱ���������Ҫ�Լ���0��8��16��24
�����ǵ�ƽģʽ�� GPIO16 ��0 |= 100000000 = 100000000 ��
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

