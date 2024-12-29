#ifndef TIMER_H
#define TIMER_H
#include "stm32l476xx.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

//Helper Functions
void delay_ms(unsigned int ms);
void delay_micro_s(unsigned int ms);
void SystemClock_Config(void);
bool gpio_Read(GPIO_TypeDef *port, uint8_t pin);

//Init functions
void motor_init();
void button_init();
void hallEffect_init();
void buzz_init(void);

//SysTick
void SysTick_Initialize(uint32_t ticks);
void SysTick_handler(void);

//State 0
void reset(void);
void driveSec(int steps, bool cw);
void driveMin(int steps, bool cw);
void driveReset(void);

//State 1
void preset(void);
void incramentSec(void);
void incramentMin(void);

//State 2
void decramentSec(void);
void decramentMin(void);


//State 3
void buzz_series(void);
void buzz(void);


#endif