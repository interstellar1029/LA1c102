#ifndef _USER_GPIO_H
#define _USER_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

void gpio_init(int gpio, int io);
void gpio_write(int gpio, int val);
int  gpio_read(int gpio);
int  gpio_iosel(int gpio, int iosel);

#ifdef __cplusplus
}
#endif

#endif // _USER_GPIO_H

