#include "step_motor.h"
#include "ls1c102.h"
#include "src/GPIO/user_gpio.h"


int gpio20_state = 0;
int count = 0;
int dir = 0;
int count_step = 0;
//int count_step = 1024;
//int degree = 0;// 单位，度，360°就是一圈。
int radio = 64;// 减速比
int teeth_num = 8;// 齿数
uint32_t step = 8;// 默认以八拍驱动。

unsigned char BYJ_Code_8_ccw[8]= {0xE, 0xC, 0xD, 0x9, 0xB, 0x3, 0x7, 0x6}; // 步进电机8个细分的二进制码。八拍驱动：A→AB→B→BC→C→CD→D→DA 。逆时针。
//                    code = 1110, 1100, 1101, 1001, 1011, 0011, 0111, 0110
unsigned char BYJ_Code_8_cw[8]= {0xE, 0x6, 0x7, 0x3, 0xB, 0x9, 0xD, 0xC}; // 步进电机8个细分的二进制码。八拍驱动：A→DA→D→CD→C→BC→B→BA 。顺时针。
unsigned char BYJ_Code_4_ccw[4]= {0xE, 0xD, 0xB, 0x7}; // 步进电机4个细分的二进制码。四拍驱动：A→B→C→D 。逆时针。
unsigned char BYJ_Code_4_cw[4]= {0xE, 0x7, 0xB, 0xD}; // 步进电机4个细分的二进制码。四拍驱动：A→D→B→C 。顺时针。

unsigned char index = 0;

extern HW_TIMER_t *g_timer;// 获取定时器寄存器
uint32_t freq = 8000;// 转动频率。总频 8M 。

void BYJ_gpio_init()
{
    gpio_init(22, 1);                     //GPIO36 使能输出
    gpio_init(23, 1);                     //GPIO37 使能输出
    gpio_init(24, 1);                     //GPIO38 使能输出
    gpio_init(25, 1);                     //GPIO39 使能输出
}

int round(double num, int num1)// 自定义四舍五入函数
{
    int num2 = 0;
    double sub_num = 0, num3 = 0, num4 = 0;
    num3 = num1;
    num4 = num1 + 1;
    sub_num = (num - num3) - (num4 - num);// 1782.7 - 1782 - (1783 - 1782.7)// 浮点型变量运算中不能混有整型变量，否则报错。
    sub_num = 1;

    if(sub_num > 0)
    {
        num2 = num1 + 1;// 左端返回值是整型变量，所以右端必须是整型变量，否则报错。
    }
    else if(sub_num < 0)
    {
        num2 = num1;
    }
    return num2;
}

void BYJ_gpio_write(unsigned char code)// 步进电机将二进制码转化为GPIO输出
{
    gpio_write(22, (code&0x1));   // 0, 0, 1, 1, 1, 1, 1, 0   A-1号线-蓝
    gpio_write(23, (code&0x2)>>1);// 1, 0, 0, 0, 1, 1, 1, 1   B-2号线-粉
    gpio_write(24, (code&0x4)>>2);// 1, 1, 1, 0, 0, 0, 1, 1   C-3号线-黄
    gpio_write(25, (code&0x8)>>3);// 1, 1, 1, 1, 1, 0, 0, 0   D-4号线-橙

    // 逆时针如下
    // 0, 0, 1, 1, 1, 1, 1, 0   A-1号线-蓝
    // 1, 0, 0, 0, 1, 1, 1, 1   B-2号线-粉
    // 1, 1, 1, 0, 0, 0, 1, 1   C-3号线-黄
    // 1, 1, 1, 1, 1, 0, 0, 0   D-4号线-橙

    // 顺时针如下
    // 0, 0, 1, 1, 1, 1, 1, 0   A-1号线-蓝
    // 1, 1, 1, 1, 1, 0, 0, 0   B-2号线-粉
    // 1, 1, 1, 0, 0, 0, 1, 1   C-3号线-黄
    // 1, 0, 0, 0, 1, 1, 1, 1   D-4号线-橙

    // 逆时针如下
    // 0, 1, 1, 1    A-1号线-蓝
    // 1, 0, 1, 1    B-2号线-粉
    // 1, 1, 0, 1    C-3号线-黄
    // 1, 1, 1, 0    D-4号线-橙

    // 顺时针如下
    // 0, 1, 1, 1    A-1号线-蓝
    // 1, 1, 1, 0    B-2号线-粉
    // 1, 1, 0, 1    C-3号线-黄
    // 1, 0, 1, 1    D-4号线-橙
}

void BYJ_gpio_stop(void)// 步进电机将二进制码转化为GPIO输出
{
    gpio_write(22, 0);
    gpio_write(23, 0);
    gpio_write(24, 0);
    gpio_write(25, 0);
}

/*
    p_degree 控制角度
    p_dir 控制旋转方向， p_dir = 0：顺， p_dir = 1：逆， p_dir = 2：停止。
    p_step 控制拍数。目前只有4拍或者8拍。
    p_freq 控制定时器频率。
*/
void BYJ_run_angle(int p_degree, int p_dir, int p_step, long p_freq)// 控制步进电机输出角度，单位°。
{
    if((dir == 0) | (dir == 1) | (dir == 2))
    {
        dir = p_dir;
    }
    if((p_step == 4) | (p_step == 8))
    {
        step = p_step;
    }
    freq = p_freq;

    // 八拍
    //count_step =  degree * 64 / 5.625
    //count_step = p_degree * radio *(teeth_num * step_8 / 360);// false
    //count_step = p_degree * radio / 5.625;// false
    //count_step = p_degree * radio / 5;// 表达式只有全部为整数才可以，出现小数会导致 count_step 出问题，电机不转
    //count_step = p_degree * radio * 1000 / 5625;// right
    //count_step = (int)(p_degree * radio * teeth_num * step_8 / 360);// right 。表达式最后的结果可以包含小数，
    //count_step = (p_degree * radio * teeth_num * step_8 / 360);// right 。发现 (int) 没有取整的效果，系统自动把浮点数向下取整后赋值给 count_step 。
    // 八拍

    double num;
    int num1, num2;
    num = p_degree * radio * teeth_num * step / 360;
    num1 = p_degree * radio * teeth_num * step / 360;

    num2 = round(num, num1);
    count_step = num2;
}

void BYJ_run_step(int p_count_step, int p_dir, int p_step, long p_freq)// 控制步进电机输出步数（拍数）。
{
    count_step = p_count_step;
    if((dir == 0) | (dir == 1) | (dir == 2))
    {
        dir = p_dir;
    }
    if((p_step == 4) | (p_step == 8))
    {
        step = p_step;
    }
    freq = p_freq;
}

void Step_Running(unsigned char run_dir,unsigned char micro, uint32_t run_step,uint32_t run_freq)
{
    // 在中断中循环写八拍或者四拍如下
    while(1)
    {
        if(micro == 8)
        {
            if(run_dir == 0)
            {
                 BYJ_gpio_write(BYJ_Code_8_cw[index]);// 顺
            }
             else if(run_dir == 1)
            {
                 BYJ_gpio_write(BYJ_Code_8_ccw[index]);// 逆
            }
             else if(run_dir == 2)
            {
                 BYJ_gpio_stop();// 停
                 return;
            }
         }
        else if(micro == 4)
        {
            if(run_dir == 0)
            {
                BYJ_gpio_write(BYJ_Code_4_cw[index]);// 顺
            }
            else if(run_dir == 1)
            {
                BYJ_gpio_write(BYJ_Code_4_ccw[index]);// 逆
            }
            else if(run_dir == 2)
            {
                BYJ_gpio_stop();// 停
                return;
            }
        }
        if(count <= run_step)
        {
            count++;
        }
        else
        {
            run_dir = 2;
            count =0;
        }
         index++;
        if(index > (micro - 1))
         {
            index = 0;
         }
    delay_ms(run_freq);
    }
}




