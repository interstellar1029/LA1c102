#include "step_motor.h"
#include "ls1c102.h"
#include "src/GPIO/user_gpio.h"


int gpio20_state = 0;
int count = 0;
int dir = 0;
int count_step = 0;
//int count_step = 1024;
//int degree = 0;// ��λ���ȣ�360�����һȦ��
int radio = 64;// ���ٱ�
int teeth_num = 8;// ����
uint32_t step = 8;// Ĭ���԰���������

unsigned char BYJ_Code_8_ccw[8]= {0xE, 0xC, 0xD, 0x9, 0xB, 0x3, 0x7, 0x6}; // �������8��ϸ�ֵĶ������롣����������A��AB��B��BC��C��CD��D��DA ����ʱ�롣
//                    code = 1110, 1100, 1101, 1001, 1011, 0011, 0111, 0110
unsigned char BYJ_Code_8_cw[8]= {0xE, 0x6, 0x7, 0x3, 0xB, 0x9, 0xD, 0xC}; // �������8��ϸ�ֵĶ������롣����������A��DA��D��CD��C��BC��B��BA ��˳ʱ�롣
unsigned char BYJ_Code_4_ccw[4]= {0xE, 0xD, 0xB, 0x7}; // �������4��ϸ�ֵĶ������롣����������A��B��C��D ����ʱ�롣
unsigned char BYJ_Code_4_cw[4]= {0xE, 0x7, 0xB, 0xD}; // �������4��ϸ�ֵĶ������롣����������A��D��B��C ��˳ʱ�롣

unsigned char index = 0;

extern HW_TIMER_t *g_timer;// ��ȡ��ʱ���Ĵ���
uint32_t freq = 8000;// ת��Ƶ�ʡ���Ƶ 8M ��

void BYJ_gpio_init()
{
    gpio_init(22, 1);                     //GPIO36 ʹ�����
    gpio_init(23, 1);                     //GPIO37 ʹ�����
    gpio_init(24, 1);                     //GPIO38 ʹ�����
    gpio_init(25, 1);                     //GPIO39 ʹ�����
}

int round(double num, int num1)// �Զ����������뺯��
{
    int num2 = 0;
    double sub_num = 0, num3 = 0, num4 = 0;
    num3 = num1;
    num4 = num1 + 1;
    sub_num = (num - num3) - (num4 - num);// 1782.7 - 1782 - (1783 - 1782.7)// �����ͱ��������в��ܻ������ͱ��������򱨴�
    sub_num = 1;

    if(sub_num > 0)
    {
        num2 = num1 + 1;// ��˷���ֵ�����ͱ����������Ҷ˱��������ͱ��������򱨴�
    }
    else if(sub_num < 0)
    {
        num2 = num1;
    }
    return num2;
}

void BYJ_gpio_write(unsigned char code)// �����������������ת��ΪGPIO���
{
    gpio_write(22, (code&0x1));   // 0, 0, 1, 1, 1, 1, 1, 0   A-1����-��
    gpio_write(23, (code&0x2)>>1);// 1, 0, 0, 0, 1, 1, 1, 1   B-2����-��
    gpio_write(24, (code&0x4)>>2);// 1, 1, 1, 0, 0, 0, 1, 1   C-3����-��
    gpio_write(25, (code&0x8)>>3);// 1, 1, 1, 1, 1, 0, 0, 0   D-4����-��

    // ��ʱ������
    // 0, 0, 1, 1, 1, 1, 1, 0   A-1����-��
    // 1, 0, 0, 0, 1, 1, 1, 1   B-2����-��
    // 1, 1, 1, 0, 0, 0, 1, 1   C-3����-��
    // 1, 1, 1, 1, 1, 0, 0, 0   D-4����-��

    // ˳ʱ������
    // 0, 0, 1, 1, 1, 1, 1, 0   A-1����-��
    // 1, 1, 1, 1, 1, 0, 0, 0   B-2����-��
    // 1, 1, 1, 0, 0, 0, 1, 1   C-3����-��
    // 1, 0, 0, 0, 1, 1, 1, 1   D-4����-��

    // ��ʱ������
    // 0, 1, 1, 1    A-1����-��
    // 1, 0, 1, 1    B-2����-��
    // 1, 1, 0, 1    C-3����-��
    // 1, 1, 1, 0    D-4����-��

    // ˳ʱ������
    // 0, 1, 1, 1    A-1����-��
    // 1, 1, 1, 0    B-2����-��
    // 1, 1, 0, 1    C-3����-��
    // 1, 0, 1, 1    D-4����-��
}

void BYJ_gpio_stop(void)// �����������������ת��ΪGPIO���
{
    gpio_write(22, 0);
    gpio_write(23, 0);
    gpio_write(24, 0);
    gpio_write(25, 0);
}

/*
    p_degree ���ƽǶ�
    p_dir ������ת���� p_dir = 0��˳�� p_dir = 1���棬 p_dir = 2��ֹͣ��
    p_step ����������Ŀǰֻ��4�Ļ���8�ġ�
    p_freq ���ƶ�ʱ��Ƶ�ʡ�
*/
void BYJ_run_angle(int p_degree, int p_dir, int p_step, long p_freq)// ���Ʋ����������Ƕȣ���λ�㡣
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

    // ����
    //count_step =  degree * 64 / 5.625
    //count_step = p_degree * radio *(teeth_num * step_8 / 360);// false
    //count_step = p_degree * radio / 5.625;// false
    //count_step = p_degree * radio / 5;// ���ʽֻ��ȫ��Ϊ�����ſ��ԣ�����С���ᵼ�� count_step �����⣬�����ת
    //count_step = p_degree * radio * 1000 / 5625;// right
    //count_step = (int)(p_degree * radio * teeth_num * step_8 / 360);// right �����ʽ���Ľ�����԰���С����
    //count_step = (p_degree * radio * teeth_num * step_8 / 360);// right ������ (int) û��ȡ����Ч����ϵͳ�Զ��Ѹ���������ȡ����ֵ�� count_step ��
    // ����

    double num;
    int num1, num2;
    num = p_degree * radio * teeth_num * step / 360;
    num1 = p_degree * radio * teeth_num * step / 360;

    num2 = round(num, num1);
    count_step = num2;
}

void BYJ_run_step(int p_count_step, int p_dir, int p_step, long p_freq)// ���Ʋ�����������������������
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
    // ���ж���ѭ��д���Ļ�����������
    while(1)
    {
        if(micro == 8)
        {
            if(run_dir == 0)
            {
                 BYJ_gpio_write(BYJ_Code_8_cw[index]);// ˳
            }
             else if(run_dir == 1)
            {
                 BYJ_gpio_write(BYJ_Code_8_ccw[index]);// ��
            }
             else if(run_dir == 2)
            {
                 BYJ_gpio_stop();// ͣ
                 return;
            }
         }
        else if(micro == 4)
        {
            if(run_dir == 0)
            {
                BYJ_gpio_write(BYJ_Code_4_cw[index]);// ˳
            }
            else if(run_dir == 1)
            {
                BYJ_gpio_write(BYJ_Code_4_ccw[index]);// ��
            }
            else if(run_dir == 2)
            {
                BYJ_gpio_stop();// ͣ
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




