#include "key.h"
#include "ls1c102.h"
#include "bsp.h"

#include "src/GPIO/user_gpio.h"

void key_init()
{
    gpio_init(out0, 1);
    gpio_init(out1, 1);
    gpio_init(out2, 1);
    gpio_init(out3, 1);
    gpio_init(int0, 0);
    gpio_init(int1, 0);
    gpio_init(int2, 0);
    gpio_init(int3, 0);
}

void key_write(uint8_t value)
{
    gpio_write(out0, (value & 0x01));
    gpio_write(out1, ((value>>1) & 0x01));
    gpio_write(out2, ((value>>2) & 0x01));
    gpio_write(out3, ((value>>3) & 0x01));
}

uint8_t key_read()
{
    uint8_t kvalue = 0;
    kvalue = gpio_read(int0);
    kvalue |= gpio_read(int1)<<1;
    kvalue |= gpio_read(int2)<<2;
    kvalue |= gpio_read(int3)<<3;
    return kvalue;
}

uint8_t key_scan()
{
    uint8_t value;
    //第一行如下
    key_write(0x01);// 根据 key_write() 函数，应该为0001
    value = key_read();// 根据 key_read() 函数，如果是第一列，value 应该为0001
    if(value != 0)
    {
        while(value == key_read());
        key_write(0);
        value |= (0x01<<4);
         printk("value1 = %u\r\n", value);
        return value;// 根据 key_write() 函数和 key_read() 函数，合并后是0b00010001 = 0x11。
        //0b00010001 = 0x11，0b00010010 = 0x12，0b00010100 = 0x14，0b00011000 = 0x18，
    }

    //第二行如下
    key_write(0x02);
    value = key_read();
    if(value != 0)
    {
        while(value == key_read());
        key_write(0);
        value |= (0x02<<4);
        switch(value)
        {
            case 33:value-=33;break;
            case 34:value-=33;break;
            case 36:value-=34;break;
            case 40:value-=37;break;
        }
        printk("value2 = %u\r\n", value);
        return value;
        //0b00100001 = 0x21，0b00100010 = 0x22，0b00100100 = 0x24，0b00101000 = 0x28，
    }

    //第三行如下
    key_write(0x04);
    value = key_read();
    if(value != 0)
    {
        while(value == key_read());
        key_write(0);
        value |= (0x04<<4);
        switch(value)
        {
            case 65:value-=61;break;
            case 66:value-=61;break;
            case 68:value-=62;break;
            case 72:value-=65;break;
        }
        printk("value3 = %u\r\n", value);
        return value;
        //0b01000001 = 0x41，0b01000010 = 0x42，0b01000100 = 0x44，0b01001000 = 0x48，
    }

    //第四行如下
    key_write(0x08);
    value = key_read();
    if(value != 0)
    {
        while(value == key_read());
        key_write(0);
        value |= (0x08<<4);
        switch(value)
        {
            case 129:value-=121;break;
            case 130:value-=121;break;
            case 132:value-=122;break;
            case 136:value-=125;break;
        }
        printk("value4 = %u\r\n", value);
        return value;
        //0b10000001 = 0x81，0b10000010 = 0x82，0b10000100 = 0x84，0b10001000 = 0x88，
    }

    return 0xFF;
}




