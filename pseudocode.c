/*
main.c

motor init
button init
systick init

while(1)
	if state 0
		stopped = false;
		reset
		
	if state 1
		preset
		
	if state 3
		buzzing
		
	if stop pressed
		bool stopped = true
*/

/*
Global Varialbes
	int seconds
	int secSteps
	int minutes
	int minSteps
	int hours
	bool longBuzz = false;
	bool stopped = false;
*/

/*
void reset
	while pins are high
		turn motors CW
		
	end loop
	
	turn motors off set amount
	
	state = 1
*/

/*
void preset
	while(state = 1)
		if stopped pin pressed
			state = 0;
			
		if minute >= 5
			longBuzz = true
		
		if secButton
			incrament_second();
			delay()
			if secButton
				incrament_second()
				delay()
				if secButton
					incrament_second()
					delay()
					while (sec Button pressed)
						incrament_sec()
						smaller delay
			
		if minButton
			incrament_minute();
			delay()
			if minButton
				incrament_minute()
				delay()
				if minButton
					incrament_minute()
					delay()
					while (minute Button pressed)
						incrament_minute()
						smaller delay
*/

/*
void incrament_second()
	if sec == 60
		sec = 1
		min++
		incrament_minute()
		drive_second CW 68 steps
	else
		sec++
		if(sec == 15 || 30 || 45 || 60)
			secStep =+ 69
			drive_second CW 69 steps
		else
			secStep =+ 68
			drive_second CW 68 steps	
*/

/*
void incramnet_minute()
	if min == 60
			min = 1
			hour++
			incrament_minute()
			drive_minute CW 68 steps
		else
			min++
			if(min == 15 || 30 || 45 || 60)
				minStep =+ 69
				drive_minutes CW 69 steps
			else
				minStep =+ 68
				drive_minute CW 68 steps
*/


/*
void systick_handaler
	if state = 2
		if stopped == true
		state = 0
		
		if sec = 0 || min = 0 || hour != 0
			deincrament_second
			deincrament_minute			
			hour--
		else if sec = 0
			deincrament_second
			deincrament_minute
		else
			deincrament_second()
	
	if hour == 0 || min == 0 || sec == 0
		state = 3
*/

/*
deincramnet_second
	if sec == 0
		sec = 59
		drive second CCW 68
	else
		sec--
		if sec = 14 || 29 || 44 || 59
			drive_second CCW 69
			secStep -= 69
		else
			drive_second CCW 68
			secStep -= 68
*/

/*
Deincrament_minute
	if min == 0
			min = 59
			drive minute CCW 68
	else
		min--
		if min = 14 || 29 || 44 || 59
			drive_minute CCW 69
			minStep -= 69
		else
			drive_minute CCW 68
			minStep -= 68
*/

/*
Buzz_in_series
	if longBuzz
		while(!stopped && done < 10)
			buzz();
			delay_ms(800);
			done++
	else
		while(!stopped && done < 3)
			buzz();
			delay_ms(800);
			done++
			
			
	state = 1;
*/