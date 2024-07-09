#ifndef _STEP_MOTOR_H
#define _STEP_MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ls1c102.h"

void BYJ_gpio_init(void);
int my_round(double num, int num1);// 自定义四舍五入函数
void BYJ_gpio_write(unsigned char code);// 步进电机将二进制码转化为GPIO输出
void BYJ_gpio_stop(void);// 步进电机将二进制码转化为GPIO输出
void BYJ_run_angle(int p_degree, int p_dir, int p_step, long p_freq);// 控制步进电机输出角度，单位°。
void BYJ_run_step(int p_count_step, int p_dir, int p_step, long p_freq);// 控制步进电机输出步数（拍数）。

void Step_Running(unsigned char run_dir,unsigned char micro, uint32_t run_step,uint32_t run_freq);

#ifdef __cplusplus
}
#endif

#endif // _STEP_MOTOR_H

