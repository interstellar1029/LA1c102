#include "esp8266.h"
#include "ls1c102.h"
#include "ns16550.h"
#include "src/GPIO/user_gpio.h"
#include <string.h>
#include "src/OLED/oled.h"

// ESP8266 ��������󣬼����յ���Ӧ��
// str���ڴ���Ӧ����
// ����ֵ��0��û�еõ��ڴ���Ӧ�������������ڴ�Ӧ������λ��(str��λ��)
char* esp8266_check_cmd(char *str)//
{
	char *strx = NULL;
	if(UART0_RECV_STA)// ���յ�һ��������
	{
	    printk("check UART0_RX_BUF:%s\r\n", UART0_RX_BUF);// ��ӡ��ȷ
		if(0== strncmp((const char*)UART0_RX_BUF, (const char*)str,2))
		strx =&UART0_RX_BUF;
		memset(UART0_RX_BUF, '\0', 128);
		UART0_RECV_STA=0;
	}
	return strx;
}

// �� ESP8266 ��������
// cmd�����͵������ַ�����
// ack���ڴ���Ӧ���������Ϊ�գ����ʾ����Ҫ�ȴ�Ӧ��
// waittime���ȴ�ʱ��
// ����ֵ��0�����ͳɹ�(�õ����ڴ���Ӧ����)��1������ʧ��
char esp8266_send_cmd(char *cmd, char *ack, uint16_t waittime)
{
	char res = 0;
	UART0_RECV_STA = 0;
//	printk("cmd:%s\r\n", cmd);
	usart0_print("%s\r\n", cmd);	//��������
	if(ack && waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			//delay_ms(500);
			delay_ms(300);
			//if(UART0_RX_STA)//���յ��ڴ���Ӧ����
			//{
                printk("�ȴ�����ʱ\r\n");
				if(esp8266_check_cmd(ack))
				{
					printk("ack:%s\r\n", ack);
					break;//�õ���Ч����
				}
				//UART0_RX_STA = 0;
			//}
		}
		if(waittime == 0)
        {
            res = 1;
        }
        else printk("res:%d\r\n", res);
	}
	return res;
}

// ESP8266 ģ�����͸��ģʽ
void esp8266_start_trans(void)
{
    int ret = 1;

	// ���ù���ģʽ 1��stationģʽ   2��APģʽ   3������ AP+stationģʽ
	OLED_Clear();
    OLED_ShowString(0, 0, "s1");
	
	printk("AT+CWMODE=1\r\n");
	while(esp8266_send_cmd("AT+CWMODE=1", "OK", 50));

    delay_ms(100);
    
	// �� WIFI ģ�����������͸��������Ҫ��ʱ3s���ȴ���������
	printk("AT+RST\r\n");
	//while(esp8266_send_cmd("AT+RST", "ready", 50));
    //esp8266_send_cmd("AT+RST", "ready", 50);
    usart0_print("AT+RST\r\n");

    delay_ms(1000);
    delay_ms(1000);

	// ��ģ���������Լ����ֻ��ȵ�
	printk("AT+CWJAP=LoongArch,87654321\r\n");
	//while(esp8266_send_cmd("AT+CWJAP=\"xxxx\",\"xxxx\"", "WIFI GOT IP", 50));
	//"AT+CWJAP=\"X10\",\"12345678\"\r\n"
	
	OLED_Clear();
    OLED_ShowString(0, 0, "s2");
	while(esp8266_send_cmd("AT+CWJAP=\"LoongArch\",\"87654321\"", "OK", 50));

    delay_ms(500);
    
	// =0����·����ģʽ��=1����·����ģʽ
	
	OLED_Clear();
    OLED_ShowString(0, 0, "s3");
	
	printk("AT+CIPMUX=0\r\n");
    while(esp8266_send_cmd("AT+CIPMUX=0","OK",50));
    
    delay_ms(100);

	OLED_Clear();
    OLED_ShowString(0, 0, "s4");

    
	// ����TCP����  ������ֱ�����������͡�Զ�̷�����IP��ַ��Զ�̷������˿ں�
	printk("AT+CIPSTART=TCP,192.168.31.207,9988\r\n");
	//while(esp8266_send_cmd("AT+CIPSTART=\"TCP\",\"xxx.xxx.xxx.xxx\",xxxx","CONNECT",50));
	//"AT+CIPSTART=\"TCP\",\"183.230.40.39\",6002"
	//while(esp8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.205.131\",9988","CONNECT",50));
	while(esp8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.31.207\",9988","CONNECT",50));

	// �Ƿ���͸��ģʽ��=0����ʾ�ر�͸����=1����ʾ����͸��
	printk("AT+CIPMODE=1\r\n");
	//esp8266_send_cmd("AT+CIPMODE=1", "OK", 50);//
	esp8266_send_cmd("AT+CIPMODE=1", "OK", 50);
    
	// ͸��ģʽ�¿�ʼ�������ݵ�ָ����ָ��֮��Ϳ���ֱ�ӷ�������
	printk("AT+CIPSEND\r\n");
	esp8266_send_cmd("AT+CIPSEND", "OK", 50);

	gpio_write(13, 1);
	delay_ms(100);
	gpio_write(13, 0);
	delay_ms(100);
	gpio_write(13, 1);
	delay_ms(100);
	gpio_write(13, 0);
	delay_ms(100);
}

// �� ESP8266 ��������
// cmd�����͵������ַ���
// waittime���ȴ�ʱ��
// ����ֵ���������ݺ󣬷������ķ�����֤��
void esp8266_send_data(char *cmd)
{
	usart0_print("%s",cmd);// ��������
}








