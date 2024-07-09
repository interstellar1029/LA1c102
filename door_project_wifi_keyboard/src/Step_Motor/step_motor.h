#ifndef _STEP_MOTOR_H
#define _STEP_MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ls1c102.h"

void BYJ_gpio_init(void);
int my_round(double num, int num1);// �Զ����������뺯��
void BYJ_gpio_write(unsigned char code);// �����������������ת��ΪGPIO���
void BYJ_gpio_stop(void);// �����������������ת��ΪGPIO���
void BYJ_run_angle(int p_degree, int p_dir, int p_step, long p_freq);// ���Ʋ����������Ƕȣ���λ�㡣
void BYJ_run_step(int p_count_step, int p_dir, int p_step, long p_freq);// ���Ʋ�����������������������

void Step_Running(unsigned char run_dir,unsigned char micro, uint32_t run_step,uint32_t run_freq);

#ifdef __cplusplus
}
#endif

#endif // _STEP_MOTOR_H

