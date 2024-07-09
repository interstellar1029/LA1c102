// ================================================ 电源接线 ================================================ //
//   VCC         接          DC 5V/3.3V      // OLED 屏电源正
//   GND         接          GND             // OLED 屏电源地
// ================================================ 液晶屏数据线接线 ================================================ //
// 本模块默认数据总线类型为4线制SPI
//   OLED 模块
//   D0          接          GPIO53          // OLED 屏 SPI 时钟信号           // 硬件 SPI CLK
//   D1/MOSI     接          GPIO55          // OLED 屏 SPI 写信号             // 硬件 SPI MOSI
//   CS          接          GPIO56          // OLED 屏片选控制信号            // 硬件 SPI CSN0
//   RES         接          GPIO40          // OLED 屏复位控制信号。          // 普通 GPIO
//   DC          接          GPIO39          // OLED 屏数据/命令选择控制信号   // 普通 GPIO

#ifndef __OLED_H
#define __OLED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ls1c102.h"
#include "src/GPIO/user_gpio.h"
#include "src/SPI/ls1x_spi.h"

//--------------OLED参数定义---------------------
#define PAGE_SIZE    8
#define XLevelL		 0x00
#define XLevelH		 0x10
#define YLevel       0xB0
#define	Brightness	 0xFF
#define WIDTH 	     128
#define HEIGHT 	     64

// ---------------- 写命令和数据定义 ----------------
#define OLED_CMD     0	// 写命令
#define OLED_DATA    1	// 写数据
   						  
// ---------------- OLED端口定义 ----------------
#define OLED_CS   56   // 片选信号          // spi_csn[1]
#define OLED_DC   39   // 数据/命令控制信号 // 普通 GPIO
#define OLED_RST  40   // 复位信号          // 普通 GPIO

// ---------------- OLED端口操作定义 ----------------
#define OLED_CS_Clr()  Spi_Cs_Down()
#define OLED_CS_Set()  Spi_Cs_Up()

#define OLED_DC_Clr()  gpio_write(OLED_DC, 0)
#define OLED_DC_Set()  gpio_write(OLED_DC, 1)
 					   
#define OLED_RST_Clr()  gpio_write(OLED_RST, 0)
#define OLED_RST_Set()  gpio_write(OLED_RST, 1)

// 通过 SPI 向 OLED 发送数据函数
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);

// OLED 初始化函数
void OLED_GPIO_Init(void);
void OLED_Reset(void);
void OLED_Init(void);

// OLED 显示函数
// x, y : 起点坐标
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_FULL(void);
void OLED_Clear(void);
uint32_t oled_pow(uint8_t m,uint8_t n);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr, uint8_t Char_Size);                                 // 显示字符 16 * 8   chr： 示例：'A'       查看:oledfont.h 常用 ASCII 表是否有对应的字符
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr);                            // 显示字符串        *chr：示例："ABCDE"   查看:oledfont.h 常用 ASCII 表是否有对应的字符
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t num);                            // 显示汉字 16*16  num :oledfont.h   Chi_Cha_Font_16x16中对应数字  可以自行添加需要的汉字
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);    // 显示数字 num : 数值(0~4294967295); len : 数字的位数  size: 填写16



#ifdef __cplusplus
}
#endif

#endif
