#include "timer.h"

extern int state;
extern bool stopped;


int main(void){
	//init functions
	SysTick_Initialize(4000000);
	motor_init();
	button_init();
	hallEffect_init();
	buzz_init();
		
	while(1){
		if(state == 0){
			stopped = false;
			reset();
		}
		
		if(state == 1){
			preset();
		}
		
		if(state == 3){
			buzz_series();
		}
		
		if(gpio_Read(GPIOC, 3) == false){
			stopped = true;
		}
		
		if(state == 5){
			if(gpio_Read(GPIOC, 3) == false){
				state = 0;
			}
		}
	}	
}	
