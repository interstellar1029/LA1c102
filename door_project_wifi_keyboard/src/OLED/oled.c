#include "oled.h"
#include "oledfont.h"

#include "src/SPI/ls1x_spi.h"
#include "src/GPIO/user_gpio.h"
#include "src/SPI/ls1x_spi.h"

//OLED显存总共分为8页
//每页8行，一行128个像素点
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127 (0~7)行	   
//[1]0 1 2 3 ... 127 (8~15)行	
//[2]0 1 2 3 ... 127 (16~23)行	
//[3]0 1 2 3 ... 127 (24~31)行	
//[4]0 1 2 3 ... 127 (32~39)行	
//[5]0 1 2 3 ... 127 (40~47)行	
//[6]0 1 2 3 ... 127 (48~55)行	
//[7]0 1 2 3 ... 127 (56~63)行			   

//数组每个bit存储OLED每个像素点的颜色值(1-亮(白色),0-灭(黑色))
//每个数组元素表示1列8个像素点，一共128列


void OLED_WR_Byte(uint8_t dat, uint8_t cmd)// 硬件实现。纵向写一个字节数据
{
	if(cmd)// 写数据
	{
		OLED_DC_Set();
	}
	else// 写命令
	{
		OLED_DC_Clr();
	}
	Spi_Send(&dat, sizeof(dat));// 先发送高位，再发送低位
}

void OLED_GPIO_Init(void)// 硬件 SPI 配置
{
    // 普通 GPIO 配置
	gpio_init(39, 1);// DC
	gpio_init(40, 1);// RST
    // 普通 GPIO 配置
}

void OLED_Reset(void)
{
	OLED_RST_Set();
	OLED_RST_Clr();
	OLED_RST_Set();
}

void OLED_Init(void)
{
 	OLED_GPIO_Init();// 初始化GPIO
	OLED_Reset();    // 复位OLED

    /**************初始化SSD1306*****************/
	OLED_WR_Byte(0xAE, OLED_CMD); /*display off*/
	OLED_WR_Byte(0x00, OLED_CMD); /*set lower column address*/
	OLED_WR_Byte(0x10, OLED_CMD); /*set higher column address*/
	OLED_WR_Byte(0x40, OLED_CMD); /*set display start line*/
	OLED_WR_Byte(0xB0, OLED_CMD); /*set page address*/
	OLED_WR_Byte(0x81, OLED_CMD); /*contract control*/
	OLED_WR_Byte(0xFF, OLED_CMD); /*128*/
	OLED_WR_Byte(0xA1, OLED_CMD); /*set segment remap*/
	OLED_WR_Byte(0xA6, OLED_CMD); /*normal / reverse*/
	OLED_WR_Byte(0xA8, OLED_CMD); /*multiplex ratio*/
	OLED_WR_Byte(0x3F, OLED_CMD); /*duty = 1/64*/
	OLED_WR_Byte(0xC8, OLED_CMD); /*Com scan direction*/
	OLED_WR_Byte(0xD3, OLED_CMD); /*set display offset*/
	OLED_WR_Byte(0x00, OLED_CMD);
	OLED_WR_Byte(0xD5, OLED_CMD); /*set osc division*/
	OLED_WR_Byte(0x80, OLED_CMD);
	OLED_WR_Byte(0xD9, OLED_CMD); /*set pre-charge period*/
	OLED_WR_Byte(0XF1, OLED_CMD);
	OLED_WR_Byte(0xDA, OLED_CMD); /*set COM pins*/
	OLED_WR_Byte(0x12, OLED_CMD);
	OLED_WR_Byte(0xDB, OLED_CMD); /*set vcomh*/
	OLED_WR_Byte(0x30, OLED_CMD);
	OLED_WR_Byte(0x8D, OLED_CMD); /*set charge pump disable*/
	OLED_WR_Byte(0x14, OLED_CMD);
	OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
}

void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD);// SET DCDC命令
	OLED_WR_Byte(0X14, OLED_CMD);// DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD);// DISPLAY ON
}

void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD);// SET DCDC命令
	OLED_WR_Byte(0X10, OLED_CMD);// DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD);// DISPLAY OFF
}

void OLED_FULL(void)// 全屏显示
{
	uint8_t i, n;
	for(i = 0; i < PAGE_SIZE; i++)
	{
		OLED_WR_Byte(YLevel + i, OLED_CMD);   // 设置页地址（0~7）
		OLED_WR_Byte(XLevelL, OLED_CMD);      // 设置显示位置—列低地址
		OLED_WR_Byte(XLevelH, OLED_CMD);      // 设置显示位置—列高地址
		for(n = 0; n < WIDTH; n++)
		{
			OLED_WR_Byte(0xFF, OLED_DATA);
		}
	}
}

void OLED_Clear(void)// 清除显示
{
	uint8_t i, n;
	for(i = 0; i < PAGE_SIZE; i++)
	{
		OLED_WR_Byte(YLevel + i, OLED_CMD);   // 设置页地址（0~7）
		OLED_WR_Byte(XLevelL, OLED_CMD);      // 设置显示位置—列低地址
		OLED_WR_Byte(XLevelH, OLED_CMD);      // 设置显示位置—列高地址
		for(n = 0; n < WIDTH; n++)
		{
			OLED_WR_Byte(0, OLED_DATA);
		}
	}
}

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
 	OLED_WR_Byte(YLevel + y / PAGE_SIZE, OLED_CMD);
	OLED_WR_Byte((((x + 2) & 0xf0)>>4) | 0x10, OLED_CMD);
	OLED_WR_Byte(((x + 2) & 0x0f), OLED_CMD);
}

void OLED_Set_Pixel(unsigned char x, unsigned char y, unsigned char color)
{
    OLED_Set_Pos(x, y);
    OLED_WR_Byte(color, OLED_DATA);
}

void GUI_DrawPoint(uint8_t x, uint8_t y, uint8_t color)
{
	OLED_Set_Pixel(x, y, color);
}

////////////////////////////////
// 显示字符
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)  //Char_Size写16
{
    uint8_t c = 0, i = 0;
    c = chr - '!';// 得到偏移后的值
    if(x > WIDTH - 1)
    {
        x = 0;
        y = y + 2;
    }
    if(Char_Size == 16)
    {
        OLED_Set_Pos(x, y);
        for(i = 0; i < 8; i++)
            OLED_WR_Byte(Front_16x8Tab[c][i], OLED_DATA);
        OLED_Set_Pos(x, y + 8);
        for(i = 0; i < 8; i++)
            OLED_WR_Byte(Front_16x8Tab[c][i + 8], OLED_DATA);
    }
}



// ==========================================================================
// 显示字符串
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr)
{

    uint8_t j = 0;
    while (chr[j] != '\0')
    while (*(chr + j) != '\0')
    {
        if(x > 120)
        {
            x = 0;y += 2;
        }
        if(y > 6)
        {
            y = 0;
        }
        OLED_ShowChar(x, y, chr[j],16);
        j++;
        x += 8;
    }
}



// ==========================================================================
// 显示一个 16*16 点阵的字符（汉字）
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t num)
{
    if(x > 112) //128-16
    {
        x = 0;y += 2;
    }
    if(y > 48) //64-16
    {
        y = 0;
    }
    OLED_Set_Pos(x, y);
    for(int t = 0;t < 16; t++)
    {
        OLED_WR_Byte(Chi_Cha_Font_16x16[2 * num][t], OLED_DATA);
    }
    OLED_Set_Pos(x, y + 8);
    for(int t = 0; t < 16; t++)
    {
        OLED_WR_Byte(Chi_Cha_Font_16x16[2 * num + 1][t], OLED_DATA);
    }
}


////////////////////////////////
////////////////////////////////
// m^n函数 计算幂的函数
uint32_t oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while(n--)
        result *= m;
    return result;
}

// ==========================================================================

//函数使用了一个循环来逐个显示数字的每一位。首先，根据给定的起点坐标 x 和 y，判断是否需要换行。同时，
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    for(t = 0; t < len; t++)
    {

        temp = (num / oled_pow(10, len - t - 1)) % 10;    //通过除法和取模运算，从给定的 num 数值中依次提取出每一位数字。在这里，使用了之前定义的函数 oled_pow() 来计算 10 的幂次方，以便得到每一位数字的值。
        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)        //当该enshow为 0 时，判断是否需要跳过不显示，主要是为了去除前导零。如果该位数字为 0 且不是最后一位，就将 x 增加数字体长度（size/2），然后继续循环
            {
                x += (size / 2);
                continue;
            }
            else
                enshow = 1;
        }

        if(x > 119)
        {
            x = 0;y = y + 16;   //如果 x 超过了屏幕宽度，则将 x 重置为 0，并将 y 增加16，实现换行效果。
        }
        if(y > 48)  //64-16
        {
            y = 0;             //如果 y 超过了屏幕高度，则将 y 重置为 0。
        }

        OLED_ShowChar(x, y, temp + '0',size);
        
        x += (size / 2);

    }
}




