#include "esp8266.h"
#include "ls1c102.h"
#include "ns16550.h"
#include "src/GPIO/user_gpio.h"
#include <string.h>
#include "src/OLED/oled.h"

// ESP8266 发送命令后，检测接收到的应答
// str：期待的应答结果
// 返回值：0，没有得到期待的应答结果；其他，期待应答结果的位置(str的位置)
char* esp8266_check_cmd(char *str)//
{
	char *strx = NULL;
	if(UART0_RECV_STA)// 接收到一次数据了
	{
	    printk("check UART0_RX_BUF:%s\r\n", UART0_RX_BUF);// 打印正确
		if(0== strncmp((const char*)UART0_RX_BUF, (const char*)str,2))
		strx =&UART0_RX_BUF;
		memset(UART0_RX_BUF, '\0', 128);
		UART0_RECV_STA=0;
	}
	return strx;
}

// 向 ESP8266 发送命令
// cmd：发送的命令字符串。
// ack：期待的应答结果，如果为空，则表示不需要等待应答
// waittime：等待时间
// 返回值：0，发送成功(得到了期待的应答结果)；1，发送失败
char esp8266_send_cmd(char *cmd, char *ack, uint16_t waittime)
{
	char res = 0;
	UART0_RECV_STA = 0;
//	printk("cmd:%s\r\n", cmd);
	usart0_print("%s\r\n", cmd);	//发送命令
	if(ack && waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			//delay_ms(500);
			delay_ms(300);
			//if(UART0_RX_STA)//接收到期待的应答结果
			//{
                printk("等待倒计时\r\n");
				if(esp8266_check_cmd(ack))
				{
					printk("ack:%s\r\n", ack);
					break;//得到有效数据
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

// ESP8266 模块进入透传模式
void esp8266_start_trans(void)
{
    int ret = 1;

	// 设置工作模式 1：station模式   2：AP模式   3：兼容 AP+station模式
	OLED_Clear();
    OLED_ShowString(0, 0, "s1");
	
	printk("AT+CWMODE=1\r\n");
	while(esp8266_send_cmd("AT+CWMODE=1", "OK", 50));

    delay_ms(100);
    
	// 让 WIFI 模块重启，发送该命令后需要延时3s，等待重启结束
	printk("AT+RST\r\n");
	//while(esp8266_send_cmd("AT+RST", "ready", 50));
    //esp8266_send_cmd("AT+RST", "ready", 50);
    usart0_print("AT+RST\r\n");

    delay_ms(1000);
    delay_ms(1000);

	// 让模块连接上自己的手机热点
	printk("AT+CWJAP=LoongArch,87654321\r\n");
	//while(esp8266_send_cmd("AT+CWJAP=\"xxxx\",\"xxxx\"", "WIFI GOT IP", 50));
	//"AT+CWJAP=\"X10\",\"12345678\"\r\n"
	
	OLED_Clear();
    OLED_ShowString(0, 0, "s2");
	while(esp8266_send_cmd("AT+CWJAP=\"LoongArch\",\"87654321\"", "OK", 50));

    delay_ms(500);
    
	// =0：单路连接模式；=1：多路连接模式
	
	OLED_Clear();
    OLED_ShowString(0, 0, "s3");
	
	printk("AT+CIPMUX=0\r\n");
    while(esp8266_send_cmd("AT+CIPMUX=0","OK",50));
    
    delay_ms(100);

	OLED_Clear();
    OLED_ShowString(0, 0, "s4");

    
	// 建立TCP连接  这三项分别代表：连接类型、远程服务器IP地址、远程服务器端口号
	printk("AT+CIPSTART=TCP,192.168.31.207,9988\r\n");
	//while(esp8266_send_cmd("AT+CIPSTART=\"TCP\",\"xxx.xxx.xxx.xxx\",xxxx","CONNECT",50));
	//"AT+CIPSTART=\"TCP\",\"183.230.40.39\",6002"
	//while(esp8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.205.131\",9988","CONNECT",50));
	while(esp8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.31.207\",9988","CONNECT",50));

	// 是否开启透传模式，=0：表示关闭透传；=1：表示开启透传
	printk("AT+CIPMODE=1\r\n");
	//esp8266_send_cmd("AT+CIPMODE=1", "OK", 50);//
	esp8266_send_cmd("AT+CIPMODE=1", "OK", 50);
    
	// 透传模式下开始发送数据的指令，这个指令之后就可以直接发数据了
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

// 向 ESP8266 发送数据
// cmd：发送的命令字符串
// waittime：等待时间
// 返回值：发送数据后，服务器的返回验证码
void esp8266_send_data(char *cmd)
{
	usart0_print("%s",cmd);// 发送数据
}








