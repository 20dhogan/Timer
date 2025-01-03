#include "timer.h"
#include "stm32l476xx.h"
#include <stdbool.h>
#define hallEffectOffSet 9*8
#define usDelay 8

//Function Headers
void gpio_Write(GPIO_TypeDef *port, uint8_t pin, bool val);
bool gpio_Read(GPIO_TypeDef *port, uint8_t pin);

//Global Variables
volatile int state = 0;
volatile bool stopped = false;
static int seconds = 0;
static int secSteps = 0;
static int minutes = 0;
static int minSteps = 0;
static bool longBuzz = false;
static int hour = 0;
static bool bothHandsMoving = false;

static bool CW[8][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 1, 1},
		{0, 0, 0, 1},
		{1, 0, 0, 1}
};

static bool timeOut[4] = {0, 0, 0, 0};

//Helper Functions***************************************************************************
bool gpio_Read(GPIO_TypeDef *port, uint8_t pin){
	uint16_t x = 0;
	x |= port->IDR;
	x &= (1 << pin);
	if(x == (1 << pin)){
		return true;
	}
	else{
		return false;
	}
}

void delay_s(unsigned int s){
	unsigned int j, i;
	for(j = 0; j < s; j++){
		for(i = 0; i < 1000; i++){
			delay_ms(1);
		}
	}
}

void delay_ms(unsigned int ms) {
	unsigned int i,j;
	for (i=0;i<ms;i++){
		{
			for(j=0;j<300;j++);
		}
	}
}

void delay_us(unsigned int us) {
	unsigned int i,j;
	for (i=0;i<us;i++){
		{
			for(j=0;j<30;j++);
		}
	}
}



void gpio_Write(GPIO_TypeDef *port, uint8_t pin, bool val){
	if(val == true){
		port->ODR |= (1 << pin); //write a 1
	} else {
		port->ODR &= ~(1 << pin); //write a 0
	} 
}

void SystemClock_Config(void) {
    //Enable HSI. From Phillips slides.
		RCC->CR |= RCC_CR_HSION;
		while((RCC->CR  & RCC_CR_HSIRDY) == 0);
}


//Init Functions******************************************************************************
void motor_init(){
	//clock initialization
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	//Motors
	GPIOB->MODER &= 0xFFFF0000; //Clears MODER bits for PB0-PB7
	GPIOB->MODER |= 0x00005555; //Set B0-B7 as outputs
}

void button_init(){
	//clock initialization
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	GPIOC->MODER &= 0xFFFFFF00; //set pins pb0-3 to input
	
	GPIOC->PUPDR &= 0xFFFFFF00; //Clear pins pb0-3
	GPIOC->PUPDR |= 0x00000055; //set pins pb0-3 as pull-up (01)
}

void hallEffect_init(){
	GPIOC->MODER &= 0xFFFFF0FF; //set pins pb4-5 to input
}

void buzz_init(){
	GPIOC->MODER &= 0xFFFFCFFF; //Clear pin pc6
	GPIOC->MODER |= 0x00001000; //Set pin pc6 to output
}

//SysTick************************************************************************************
void SysTick_Initialize(unsigned int ticks){
	//Disable SysTick IRQ and SysTick counter
	SysTick->CTRL = 0;
	
	//Set reload register
	SysTick->LOAD = ticks - 1;
	
	//set interupt priority of SysTick
	//Make SysTick Least urgent (ie, highest priority number)
	//_NVIC_PRIO_BITS: number of bites for priority levels, defined in CMSIS
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 5);
	
	//Reset the SysTick counter value
	SysTick->VAL = 0;
	
	//Select processor clock
	//1 = processor clock; 0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	//SysTick->CTRL |= RCC->CR;
	
	//Enables Systick exception request
	//1 = counting down to zero asserts the SysTick exception request
	//0 = counting down to zero does not assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	//enable SysTick timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void){
	if(state == 2){ //This means we are counting down from a preset time.
		if(gpio_Read(GPIOC, 3) == 0 || stopped == true){
			state = 0;
		} else {
			if(hour == 0 && minutes == 0 && seconds == 1){
				decramentSec();
				state = 3;
			} else if(seconds == 0 && minutes == 0 && hour != 0){
				decramentSec();
				decramentMin();
				hour--;
			} else if(seconds == 0 && minutes != 0){
				decramentSec();
				decramentMin();
			} else {
				decramentSec();
			}
		}
	}
	
	if(state == 4){//This means we are just counting up. Like a stopwatch.
		if(gpio_Read(GPIOC, 3) == 0 || stopped == true){
			state = 5;
			stopped = false;
			delay_ms(500);
		} else {
			incramentSec();
		}
	}
}

//State 0 **********************************************************************************
void reset(void){
	bool doneTotal = false;
	bool doneSec = false;
	bool doneMin = false;
	
	//make sure we were are at the top of the step array.
	driveReset();
	
	//incase we are already in the range when hall_effect is active
	if(gpio_Read(GPIOC, 4) == 0){
		driveSec(8*40, true);
	}
	if(gpio_Read(GPIOC, 5) == 0){
		driveMin(8*40, true);
	}
	
	//drive min/sec until hall effect is active
	while(!doneTotal){
		if(gpio_Read(GPIOC, 4) == 1){
			driveSec(1, true);
		} else {
			doneSec = true;
			bothHandsMoving = false;
		}
		
		if(gpio_Read(GPIOC, 5) == 1){
			driveMin(1, true);
		} else {
			doneMin = true;
			bothHandsMoving = false;
		}
		
		if(doneSec == true && doneMin == true){
			doneTotal = true;
		}
		
		if(doneSec == false && doneMin == false){
			bothHandsMoving = true;
		}
	}
	
	//now we need to add the offset
	driveSec(hallEffectOffSet, true);
	driveMin(hallEffectOffSet-16, true);
	
	//stop sending power to motors so it doesn't get hot.
	for(int j = 0; j <= 3; j++){
		gpio_Write(GPIOB, j, timeOut[j]);
		gpio_Write(GPIOB, j+4, timeOut[j]);
	}
	
	//Set state
	hour = 0;
	state = 1;
	seconds = 0;
	secSteps = 0;
	minutes = 0;
	minSteps = 0;
	longBuzz = false;
}

void driveSec(int steps, bool cw){
	int start = secSteps % 8;
	int count = 0;
	bool done = false;
	
	if(cw == true){
		while(!done){
			
			for(int j = 0; j <= 3; j++){
				gpio_Write(GPIOB, j, CW[start][j]);
			}
			secSteps++;
			
			if(start == 7){
				start = 0;
			} else {
				start++;
			}
			
			count++;
			
			if(count == steps){
				done = true;
			}
			
			if(bothHandsMoving == false){
				delay_us(usDelay);
			} else {
				delay_us(8);
			}
	
		}
	} else {
		while(!done){
			
			for(int j = 0; j <= 3; j++){
				gpio_Write(GPIOB, j, CW[start][j]);
			}
			secSteps--;
			
			if(start == 0){
				start = 7;
			} else {
				start--;
			}
			
			count++;
			
			if(count == steps){
				done = true;
			}
			
			delay_us(usDelay);
		}
	}	
}

void driveMin(int steps, bool cw){
	int start = minSteps % 8;
	int count = 0;
	bool done = false;
	
	if(cw == true){
		while(!done){
			
			for(int j = 0; j <= 3; j++){
				gpio_Write(GPIOB, j+4, CW[start][j]);
			}
			minSteps++;
			
			if(start == 7){
				start = 0;
			} else {
				start++;
			}
			
			count++;
			
			if(count == steps){
				done = true;
			}
		
			if(bothHandsMoving == false){
				delay_us(usDelay);
			} else {
				delay_us(8);
			}
			
		}
	} else {
		while(!done){
			
			for(int j = 0; j <= 3; j++){
				gpio_Write(GPIOB, j+4, CW[start][j]);
			}
			minSteps--;
			
			if(start == 0){
				start = 7;
			} else {
				start--;
			}
			
			count++;
			
			if(count == steps){
				done = true;
			}
			
			delay_us(usDelay);
			
		}
	}
}

//This fuction resets the motors to be at the top of the steps
void driveReset(void){
	for(int i = 0; i <= 7; i++){
		for(int j = 0; j <= 3; j++){
			gpio_Write(GPIOB, j+4, CW[i][j]);
			gpio_Write(GPIOB, j, CW[i][j]);
		}
		delay_ms(3);
	}
}

//State 1 **********************************************************************************
void preset(void){
	//This variale will help us know if we are going to be using a timer or stopwatch.
	bool timer = false;
	
	while(state == 1){
		if(gpio_Read(GPIOC, 2) == 0){
			if(timer == false){
				//This means we have not started seting our timer. We want to just start a stop watch.
				state = 4;
			} else {
				//This means we have already preset our timer to some value and want to start counting down.
				state = 2;
			}
		}
		
		if(gpio_Read(GPIOC, 3) == 0){
			state = 0;
		}
		
		if(minutes >= 5){
			longBuzz = true;
		}
		
		if(gpio_Read(GPIOC, 0) == 0){
			timer = true;
			incramentSec();
			delay_ms(300);
			if(gpio_Read(GPIOC, 0) == 0){
				incramentSec();
				delay_ms(300);
				if(gpio_Read(GPIOC, 0) == 0){
					incramentSec();
					delay_ms(300);
					while(gpio_Read(GPIOC, 0) == 0){
						incramentSec();
						delay_ms(10);
					}
				}
			}
		}
		
		if(gpio_Read(GPIOC, 1) == 0){
			timer = true;
			incramentMin();
			delay_ms(300);
			if(gpio_Read(GPIOC, 1) == 0){
				incramentMin();
				delay_ms(300);
				if(gpio_Read(GPIOC, 1) == 0){
					incramentMin();
					delay_ms(300);
					while(gpio_Read(GPIOC, 1) == 0){
						incramentMin();
						delay_ms(10);
					}
				}
			}
		}
	}
}
void incramentSec(void){
	if(seconds == 59){
		seconds = 0;
		incramentMin();
		driveSec(72, true);
		secSteps = 0;
	} else {
		seconds++;
		if(seconds == 15 || seconds == 30 || seconds == 45){
			driveSec(72, true);
		} else {
			driveSec(68, true);
		}
	}
}

void incramentMin(void){
	if(minutes == 59){
		minutes = 0;
		hour++;
		driveMin(72, true);
		minSteps = 0;
	} else {
		minutes++;
		if(minutes == 15 || minutes == 30 || minutes == 45){
			driveMin(72, true);
		} else {
			driveMin(68, true);
		}
	}
}

//State 2 **********************************************************************************
void decramentSec(void){
	if(seconds == 0){
		seconds = 59;
		driveSec(72, false);
		secSteps = 4024;
	} else {
		seconds--;
		if(seconds == 14 || seconds == 29 || seconds == 44){
			driveSec(72, false);
		} else {
			driveSec(68, false);
		}
	}
}
void decramentMin(void){
	if(minutes == 0){
		minutes = 59;
		driveMin(72, false);
		minSteps = 4024;
	} else {
		minutes--;
		if(minutes == 14 || minutes == 29 || minutes == 44){
			driveMin(72, false);
		} else {
			driveMin(68, false);
		}
	}
}


//State 3 **********************************************************************************
void buzz_series(void){
	if(longBuzz == true){
		for(int i = 0; i < 10; i++){
			buzz();
			if(gpio_Read(GPIOC, 3) == 0){
				i = 10;
			}
			delay_ms(700);
			if(gpio_Read(GPIOC, 3) == 0){
				i = 10;
			}
		}
	} else {
		for(int i = 0; i < 3; i++){
			buzz();
			delay_ms(700);
			if(gpio_Read(GPIOC, 3) == 0){
				i = 3;
			}
		}
	}
	state = 0;
}

void buzz(void){
	for (int i = 0; i < 500; i++) {
		gpio_Write(GPIOC, 6, 1); // Turn on buzzer
		delay_ms(1); // delay
		gpio_Write(GPIOC, 6, 0); // Turn off buzzer
		delay_ms(1);
	}
}
	
