#include "ls1x_spi.h"
#include "ls1c102.h"

#include "src/GPIO/user_gpio.h"

/*** SPI READ OPTIONS ***/
#define NORMAL    0x0
#define FAST      0x1
#define DUAL_IO   0x2

/**
 * @Description: SPI ��Ƶϵ�����á��ֱ����õ����ƼĴ���SPCR���ⲿ�Ĵ���SPER���������ƼĴ���PARAM
 * @Input: ��Ƶϵ��
 * @Output: ��
 * @return {*}��
 * @param {uint8_t} spr ��Ƶϵ��
 */
void Spi_FreqDiv(uint8_t spr)// spr ����λ��Ч
{
	// ��ʼ��Ϊ2��Ƶ
    SPI->SPCR &= 0xFC;
    SPI->SPER &= 0xFC;
	// ��ʼ��Ϊ2��Ƶ

    SPI->SPCR |= (spr & 0x03);
    SPI->SPER |= ((spr>>2) & 0x03);
    
	SPI->PARAM |= ((spr<<4) & 0xF0);// ʱ�ӷ�Ƶ��Ҫ��{spre, spr}��ͬ
	printk("SPI->PARAM:0x%x\r\n", SPI->PARAM);
}

/**
 * @Description: spi init
 * @Input: spi ��Ƶϵ��
 * SPI_DIV_2 SPI_DIV_4 SPI_DIV_16  SPI_DIV_32  SPI_DIV_8 SPI_DIV_64  SPI_DIV_128
 * SPI_DIV_256 SPI_DIV_512 SPI_DIV_1024 SPI_DIV_2048 SPI_DIV_4096
 * @Output: ��
 * @return {*}
 * @param {uint8_t} spr ��Ƶϵ�����̶�Ϊ���ϵĺ궨��
 */
void Spi_Init(uint8_t spr)
{
    Spi_FreqDiv(spr);
    
	// ϵͳ����ʹ�ܣ�master ģʽ��
    SPI->SPCR |= 0x50; // 0b0101_0000
	// ϵͳ����ʹ�ܣ�master ģʽ��
    
    while(!SPI_TXEMPTY); // wait till tx empty // ����ֱ��д�Ĵ���Ϊ��
    while(!SPI_RXEMPTY); // read till rx empty // ����ֱ�����Ĵ���Ϊ��
}

void Spi_GPIO_Init(void)// Ӳ�� SPI ����
{
	// Ӳ�� SPI ����
	gpio_iosel(53, 1);// CLK
	gpio_iosel(55, 1);// MOSI
	gpio_iosel(56, 1);// CS
	// Ӳ�� SPI ����
}

/**
 * @Description: spi д����1~4�ֽ�
 * @Input: 	���ݻ��壬����д����
 * @Output: ��
 * @return {*}
 * @param {uint8_t*} buf ���ݻ���
 * @param {int} n		��д�����ֽ���
 */
void Spi_Write_Read_1to4(uint8_t* buf, int n)
{
    uint32_t i;
    for (i = 0; i < n; i++)
	{
		SPI->DATA = buf[i];// �� DATA д������ĸ��ֽڵ�����
	}
	while(!SPI_TXEMPTY);// ����ֱ��д�Ĵ���Ϊ�գ������ͽ���

    for (i = 0; i < n - 1; i++)// �� DATA ��������
	{
		buf[i] = SPI->DATA;
	}
	while(SPI_RXEMPTY);// ���Ĵ���Ϊ��������

    buf[n-1] = SPI->DATA;// ��ȡ���һ���ֽڵ�����
}

/**
 * @Description: spi ��д N ���ֽ�
 * @Input: ���ݻ��壬��д�ֽ���
 * @Output: ��
 * @return {*}
 * @param {uint8_t*} buf ���ݻ���
 * @param {int} n ��д�ֽ���
 */
void Spi_Write_Read_N(uint8_t* buf, int n)
{
	uint32_t i, j;
    for (i = 0; i < 4; i++)// ���� DATA д���ĸ��ֽڵ�����
	{
		SPI->DATA = buf[i];
	}
    for (j = 0; i<n; i++, j++)
	{
        while(SPI_RXEMPTY);// ���Ĵ���Ϊ��������
        buf[j] =  SPI->DATA;
        SPI->DATA = buf[i];// �� DATA д��ʣ�µ�����
    }
    for (; j<n; j++)
	{
        while(SPI_RXEMPTY);
        buf[j] = SPI->DATA;// ��ȡ�� n - 4 ���ֽڵ�����
    }
}

/**
 * @Description: SPI Ƭѡ�ź�1ʹ�ܲ�����
 * @Input: ��
 * @Output: ��
 * @return {*}
 */
void Spi_Cs_Down(void)
{
	SPI_ENABLE_CS(1);// Ƭѡʹ�ܣ�����Ч
	SPI_LOW_CS(1);// Ƭѡ���ͣ�����Ч
}

/**
 * @Description: SPI Ƭѡ�ź�1ʹ�ܲ�����
 * @Input: ��
 * @Output: ��
 * @return {*}
 */
void Spi_Cs_Up(void)
{
	SPI_ENABLE_CS(1);// Ƭѡʹ�ܣ�����Ч
	SPI_HIGH_CS(1);// Ƭѡ���ߣ�����Ч
}

/**
 * @Description: SPI ��������
 * @Input: ���ݻ��壻��������
 * @Output: ��
 * @return {*}
 * @param {uint8_t*} buf ���ݻ���
 * @param {int} n		������
 */
void Spi_Send(uint8_t* buf, int n)// �ȷ��͸�λ���ٷ��͵�λ
{
	Spi_Cs_Down();// Ƭѡ���ͣ�����Ч
    Spi_Write_Read_1to4(buf, n);
    Spi_Cs_Up();
}



