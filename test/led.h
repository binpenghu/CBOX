

#ifndef  _LED_H
#define  _LED_H
#include "stm8s_gpio.h"
#define  LEDPort  GPIOA
#define  LEDPin   (1 << 3) 



extern void LED_Init(void);
extern void LED_ON(void);
extern void LED_OFF(void);


#endif
