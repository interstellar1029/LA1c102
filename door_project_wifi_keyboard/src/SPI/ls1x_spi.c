#include "ls1x_spi.h"
#include "ls1c102.h"

#include "src/GPIO/user_gpio.h"

/*** SPI READ OPTIONS ***/
#define NORMAL    0x0
#define FAST      0x1
#define DUAL_IO   0x2

/**
 * @Description: SPI 分频系数配置。分别配置到控制寄存器SPCR、外部寄存器SPER、参数控制寄存器PARAM
 * @Input: 分频系数
 * @Output: 无
 * @return {*}无
 * @param {uint8_t} spr 分频系数
 */
void Spi_FreqDiv(uint8_t spr)// spr 低四位有效
{
	// 初始化为2分频
    SPI->SPCR &= 0xFC;
    SPI->SPER &= 0xFC;
	// 初始化为2分频

    SPI->SPCR |= (spr & 0x03);
    SPI->SPER |= ((spr>>2) & 0x03);
    
	SPI->PARAM |= ((spr<<4) & 0xF0);// 时钟分频数要与{spre, spr}相同
	printk("SPI->PARAM:0x%x\r\n", SPI->PARAM);
}

/**
 * @Description: spi init
 * @Input: spi 分频系数
 * SPI_DIV_2 SPI_DIV_4 SPI_DIV_16  SPI_DIV_32  SPI_DIV_8 SPI_DIV_64  SPI_DIV_128
 * SPI_DIV_256 SPI_DIV_512 SPI_DIV_1024 SPI_DIV_2048 SPI_DIV_4096
 * @Output: 无
 * @return {*}
 * @param {uint8_t} spr 分频系数。固定为以上的宏定义
 */
void Spi_Init(uint8_t spr)
{
    Spi_FreqDiv(spr);
    
	// 系统工作使能，master 模式，
    SPI->SPCR |= 0x50; // 0b0101_0000
	// 系统工作使能，master 模式，
    
    while(!SPI_TXEMPTY); // wait till tx empty // 阻塞直到写寄存器为空
    while(!SPI_RXEMPTY); // read till rx empty // 阻塞直到读寄存器为空
}

void Spi_GPIO_Init(void)// 硬件 SPI 配置
{
	// 硬件 SPI 配置
	gpio_iosel(53, 1);// CLK
	gpio_iosel(55, 1);// MOSI
	gpio_iosel(56, 1);// CS
	// 硬件 SPI 配置
}

/**
 * @Description: spi 写、读1~4字节
 * @Input: 	数据缓冲，读、写数量
 * @Output: 无
 * @return {*}
 * @param {uint8_t*} buf 数据缓冲
 * @param {int} n		读写数据字节数
 */
void Spi_Write_Read_1to4(uint8_t* buf, int n)
{
    uint32_t i;
    for (i = 0; i < n; i++)
	{
		SPI->DATA = buf[i];// 向 DATA 写入最多四个字节的数据
	}
	while(!SPI_TXEMPTY);// 阻塞直到写寄存器为空，即发送结束

    for (i = 0; i < n - 1; i++)// 从 DATA 读出数据
	{
		buf[i] = SPI->DATA;
	}
	while(SPI_RXEMPTY);// 读寄存器为空则阻塞

    buf[n-1] = SPI->DATA;// 读取最后一个字节的数据
}

/**
 * @Description: spi 读写 N 个字节
 * @Input: 数据缓冲，读写字节数
 * @Output: 无
 * @return {*}
 * @param {uint8_t*} buf 数据缓冲
 * @param {int} n 读写字节数
 */
void Spi_Write_Read_N(uint8_t* buf, int n)
{
	uint32_t i, j;
    for (i = 0; i < 4; i++)// 先向 DATA 写入四个字节的数据
	{
		SPI->DATA = buf[i];
	}
    for (j = 0; i<n; i++, j++)
	{
        while(SPI_RXEMPTY);// 读寄存器为空则阻塞
        buf[j] =  SPI->DATA;
        SPI->DATA = buf[i];// 向 DATA 写入剩下的数据
    }
    for (; j<n; j++)
	{
        while(SPI_RXEMPTY);
        buf[j] = SPI->DATA;// 读取后 n - 4 个字节的数据
    }
}

/**
 * @Description: SPI 片选信号1使能并拉低
 * @Input: 无
 * @Output: 无
 * @return {*}
 */
void Spi_Cs_Down(void)
{
	SPI_ENABLE_CS(1);// 片选使能，高有效
	SPI_LOW_CS(1);// 片选拉低，低有效
}

/**
 * @Description: SPI 片选信号1使能并拉高
 * @Input: 无
 * @Output: 无
 * @return {*}
 */
void Spi_Cs_Up(void)
{
	SPI_ENABLE_CS(1);// 片选使能，高有效
	SPI_HIGH_CS(1);// 片选拉高，低有效
}

/**
 * @Description: SPI 发送数据
 * @Input: 数据缓冲；数据数量
 * @Output: 无
 * @return {*}
 * @param {uint8_t*} buf 数据缓冲
 * @param {int} n		数据量
 */
void Spi_Send(uint8_t* buf, int n)// 先发送高位，再发送低位
{
	Spi_Cs_Down();// 片选拉低，低有效
    Spi_Write_Read_1to4(buf, n);
    Spi_Cs_Up();
}



