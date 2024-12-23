#include <iostream>

int main(){
int secSteps = 3;
int steps = 68;
bool cw = false;	
	
	
	int start = secSteps % 8;
	
	int count = 0;
	bool done = false;
	if(cw == true){
		while(!done){
			
			for(int j = 0; j <= 3; j++){
			}
			secSteps++;
			std::cout << "printing step: " << secSteps << std::endl;
			
			if(start == 7){
				start = 0;
			} else {
				start++;
			}
			
			count++;
			
			if(count == steps){
				done = true;
			}
			
		}
	} else {
		while(!done){
			
			for(int j = 0; j <= 3; j++){
			}
			secSteps--;
			std::cout << "printing step: " << secSteps << std::endl;
			
			if(start == 0){
				start = 7;
			} else {
				start--;
			}
			
			count++;
			
			if(count == steps){
				done = true;
			}
			
		}
	}
	return 0;
}


/*int loopEnd = 0;
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
	std::cout << "ending secSteps: " << secSteps << std::endl;*/