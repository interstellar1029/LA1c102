/*
    1�����Ź�
    ��д�˿��Ź���غ����������˳�ʼ�����ʱ��Ż��Զ���λ��

    2������
    ��1������1��ӡ���С������˴���1�����жϡ�
    ��2����������ʱ�ų��� \r\n ��
    ��3����д����1���ͺ��� USART_SendData() ��
    ����
    ��1����Ϊ�ڲ� 8M ʱ�Ӳ���ȷ������ͨ�ŵĲ�����������3%�ᵼ�����룬����ֻ��ʹ���ⲿ 8M ʱ�ӽ��д���ͨ�š�
    ��2������1�жϿ��У�����ÿ���ж����ֻ�ܽ���16���ֽڡ�����16���ֽں���յ������ݻ������ֽ����ܶ�ᵼ��ֻ���յ���һ���ֽڡ�

    3������0����
    ��1�������˴��ڵ���غ�����������ʼ�������͡����պ��жϡ�
    ��2��
    ��ʼ���� NS16550_init() NS16550_open()
    ���ͣ� USART_SendData() console_putstr()
    ���գ�ֻ�е��ַ����պ��������ַ�����ʹ���жϽ��С�
    �жϣ� NS16550_interrupt_handler() NS16550_interrupt_process() NS16550_interrupt_handler_0() NS16550_interrupt_process_0()
    �ж�Ĭ��ʧ�ܣ�ʹ�������� NS16550_init() �����С�
    ��3�������˴���0�����жϣ�ʵ�ִ���0���յ����ݺ��򴮿�1���͡�
    ������������
            RX        TX
    ����0   GPIO6     GPIO7
    ����1   GPIO8     GPIO9

*/

#include <stdio.h>
#include <stdbool.h>

#include "bsp.h"
#include "console.h"
#include "ls1c102.h"
#include "src/Step_Motor/step_motor.h"
#include "src/KEY/key.h"
#include "src/GPIO/user_gpio.h"
#include "src/WDG/ls1x_wdg.h"
#include "src/AS608/as608.h"
#include "src/SPI/ls1x_spi.h"
#include "src/OLED/oled.h"
#include "src/ESP8266/esp8266.h"
#define CW_RUN  0
#define CCW_RUN 1
#define STOP_RUN  2

#define SETP_NUM    1000
#define STEP_CYC    5

extern HW_PMU_t *g_pmu;
extern int printk(const char* format, ...);
extern int usart0_print(const char* format, ...);

extern unsigned char UART0_RECV_STA;
extern unsigned char UART1_RECV_STA;

unsigned char open_key[] ={0,1,2,3,4,5};
unsigned char close_key[] ={4,5,6,7,8,9};
unsigned char input_key[] ={};
unsigned char key_num;
uint8_t ggg[]={0xEF ,01 ,0xFF, 0xFF, 0xFF ,0xFF ,0x01, 00 ,03, 0x0D ,00, 0x11};
int main(void)
{
    unsigned char input_num =0,door_status =0xff;
    uint16_t vaildnum;
    WdgInit();
    BYJ_gpio_init();

    as608_init();
    printk("main() function\r\n");
    gpio_init(20, 1);// GPIO20 ʹ�����
    gpio_init(33, 0);
    Spi_Init(SPI_DIV_2);
    Spi_GPIO_Init();
    OLED_Init();// ��ʼ��OLED
    key_init();
    console0_init(115200);// ����0��ʼ��
    console_init(57600);
    
    esp8266_start_trans();
    
    
    char string0[100] = "A";
    char *string1 = "abc";
        register unsigned int ticks;
        ticks = get_clock_ticks();
        printk("tick count = %u\r\n", ticks);
        printk("abcdefghijklmnopqrstuvwxyz\r\n");

    OLED_Clear();
    OLED_ShowString(0, 0, "DOOR_SYSTEM");


        delay_ms(1000);
        PS_StoreChar(1,0); //ָ�ƽ���
        while(PS_HandShake(&AS608Addr))  {
         gpio_write(20, 1);// GPIO20 ����ߵ�ƽ
         delay_ms(100);
         gpio_write(20, 0);// GPIO20 ����͵�ƽ
         delay_ms(100);
        }

      if(PS_ValidTempleteNum(&vaildnum) ==0)
        OLED_ShowString(0, 0, "figner");
        if(PS_Sta)
        {
            OLED_Clear();
            OLED_ShowString(0, 0, "press");
             delay_ms(1000);
            if(as608_search() ==0)
            {
                OLED_Clear();
                OLED_ShowString(0, 0, "OPEN_DOOR");
            }
            else
            {
                OLED_Clear();
                OLED_ShowString(0, 0, "error");
            }
        }
        else
        {
             OLED_Clear();
            OLED_ShowString(0, 0, "nopress");
        }
        
    for (;;)
    {
        //wifi����
        if(UART0_RECV_STA ==1)
        {
            if(door_status !=1)
            {
                gpio_write(20, 1);
                door_status =1;
        	    OLED_Clear();
                OLED_ShowString(0, 0, "OPEN_DOOR");
                Step_Running(CW_RUN,8, SETP_NUM,STEP_CYC);
            }
            else
            {
         	   OLED_Clear();
              OLED_ShowString(0, 0, "DOOR_OPENED");
              delay_ms(1000);
              OLED_Clear();
              OLED_ShowString(0, 0, "OPEN_DOOR");
            }
            UART0_RECV_STA =0;
        }
        else if(UART0_RECV_STA ==2)
        {
            if(door_status !=0)
            {
                door_status =0;
                gpio_write(20, 0);
            	OLED_Clear();
                OLED_ShowString(0, 0, "CLOSE_DOOR");
                Step_Running(CCW_RUN,8, SETP_NUM,STEP_CYC);
            }
            else
            {
         	   OLED_Clear();
              OLED_ShowString(0, 0, "DOOR_CLOSEED");
              delay_ms(1000);
              OLED_Clear();
              OLED_ShowString(0, 0, "CLOSE_DOOR");
            }
            UART0_RECV_STA =0;
        }
        //���뿪��
     //   key_num=key_scan();
        key_num=0xFF;
        if(key_num !=0xFF)
        {
            OLED_ShowNum(4, 4, key_num,0, 16);
            input_key[input_num++] =key_num;
            if(input_num>=6)
            {
                for(input_num=0;input_num<6;input_num++)
                    printk("input_key[%d] = %u\r\n", input_num,input_key[input_num]);
                input_num =0;
                if(strncmp(input_key,open_key,6) ==0)
                {
                     if(door_status !=1)
                    {
                        door_status =1;
                        OLED_Clear();
                        OLED_ShowString(0, 0, "OPEN_DOOR");
                        Step_Running(CW_RUN,8, SETP_NUM,STEP_CYC);
                         gpio_write(20, 1);
                    }
                    else
                    {
         	          OLED_Clear();
                        OLED_ShowString(0, 0, "DOOR_OPENED");
                        delay_ms(1000);
                        OLED_Clear();
                        OLED_ShowString(0, 0, "OPEN_DOOR");
                    }
                }
                else  if(strncmp(input_key,close_key,6) ==0)
                {
                    if(door_status !=0)
                    {
                        OLED_Clear();
                        door_status =0;
                        OLED_ShowString(0, 0, "CLOSE_DOOR");
                        gpio_write(20, 0);
                        Step_Running(CCW_RUN,8, SETP_NUM,STEP_CYC);
                    }
                    else
                    {
         	          OLED_Clear();
                     OLED_ShowString(0, 0, "DOOR_CLOSEED");
                    delay_ms(1000);
                    OLED_Clear();
                    OLED_ShowString(0, 0, "CLOSE_DOOR");
                    }
                }
            }
            if(key_num ==11)
            {
                input_num=0;
                    if(door_status !=0)
                    {
                        OLED_Clear();
                        door_status =0;
                        OLED_ShowString(0, 0, "CLOSE_DOOR");
                        gpio_write(20, 0);
                        Step_Running(CCW_RUN,8, SETP_NUM,STEP_CYC);
                    }
                    else
                    {
         	          OLED_Clear();
                     OLED_ShowString(0, 0, "DOOR_CLOSEED");
                    delay_ms(1000);
                    OLED_Clear();
                    OLED_ShowString(0, 0, "CLOSE_DOOR");
                    }
            }
        }
    }

    /*
     * Never goto here!
     */
    return 0;
}

/*
 * @@ End
 */
