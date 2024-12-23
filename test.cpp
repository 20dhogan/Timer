#include <iostream>

int main(){
int secSteps = 204;
int steps = 68;
bool cw = false;	
	
	
	int start = secSteps % 8;
	int remainingSteps = steps - (8 - start);
	int fullTurns = remainingSteps/8;
	int end = remainingSteps % 8;
	
	std::cout << "secSteps: " << secSteps << std::endl;
	std::cout << "steps: " << steps << std::endl;
	std::cout << "start: " << start << std::endl;
	std::cout << "remainingSteps: " << remainingSteps << std::endl;
	std::cout << "fullTurns: " << fullTurns << std::endl;
	std::cout << "end: " << end << std::endl;
	std::cout << std::endl;
	
	int loopEnd = 0;
	if((8-start) > steps){
		loopEnd = start + steps - 1;
	} else {
		loopEnd = 7;
	}
	std::cout << loopEnd << std::endl;
	
	if(cw == true){
		for(int i = start; i <= loopEnd; i++){
			std::cout << "printing step: " << secSteps << std::endl;
			secSteps++;
			for(int j = 0; j <= 3; j++){
			}
		}
	} else {
		for(int i = loopEnd; i <= start; i--){
			std::cout << "printing step: " << secSteps << std::endl;
			secSteps--;
			for(int j = 0; j <= 3; j++){
			}
		}		
	}
	
	if((8-start) < steps){
		if(cw == true){
			for(int k = 1; k <= fullTurns; k++){
					for(int i = 0; i <= 7; i++){
						std::cout << "printing step: " << secSteps << std::endl;
						secSteps++;
						for(int j = 0; j <= 3; j++){				
						}
					}
				}
		
			for(int i = 0; i < end; i++){
				std::cout << "printing step: " << secSteps << std::endl;
				secSteps++;
				for(int j = 0; j <= 3; j++){
				}
			}
		} else {
			for(int k = fullTurns; k <= 1; k--){
				for(int i = 0; i <= 7; i++){
					std::cout << "printing step: " << secSteps << std::endl;
					secSteps--;
					for(int j = 0; j <= 3; j++){				
					}
				}
			}
	
			for(int i = end; i < 0; i--){
				std::cout << "printing step: " << secSteps << std::endl;
				secSteps--;
				for(int j = 0; j <= 3; j++){
				}
			}
			
		}
	}
	std::cout << "ending secSteps: " << secSteps << std::endl;
	return 0;
}