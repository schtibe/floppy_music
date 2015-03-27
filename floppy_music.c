#include <wiringPi.h>
#include <stdio.h>

#define STEP 1
#define DIR 0

enum { C, Db, D, Eb, E, F, Gb, G, Ab, A, Bb B, rest};

const int freq[4][12] = {
   { 13081,13859,14683,15556,16481,17461,18500,19600,20765,22000,23308,24694 },
   { 26163,27718,29366,31113,32963,34923,36999,39200,41530,44000,46616,49388 },
   { 52325,55437,58733,62225,65925,69846,73999,78399,83061,88000,93233,98777 },
   { 104650,110873,117466,124451,131851,139691,147998,156798,166122,176000,186466,197553 }
};

const int floppyConv = 31400000;

int direction = 0;

float a_delay = (floppyConv / 44000) * 10;
float d_delay = (floppyConv / 58733) * 10;
float db_delay = (floppyConv / 55437) * 10;
float e_delay = (floppyConv / 65925) * 10;

void note(float, int);
void D();

static void resetMotor()
{
	// To reset head position move back 10 then forward 5
	digitalWrite(DIR, LOW);
	for (int i=0; i < 10; i++){
		digitalWrite(STEP, HIGH);
		digitalWrite(STEP, LOW);
		delay(1);
	}

	digitalWrite(DIR, HIGH);
	for (int i=0; i < 5; i++){
		digitalWrite(STEP, HIGH);
		digitalWrite(STEP, LOW);
		delay(1);
	}

	delay(400);
}


int main (void) {
	wiringPiSetup () ;
	resetMotor();

	pinMode (STEP, OUTPUT);
	pinMode (DIR,  OUTPUT);

	printf("TEST");

	for (;;) {
		note(a_delay, 500);
		delay(200);
		note(d_delay, 500);
		delay(200);
		note(d_delay, 200);
		delay(100);
		note(d_delay, 500);
		delay(500);
		note(a_delay, 500);
		delay(200);
		note(e_delay, 200);
		delay(200);
		note(db_delay, 200);
		delay(200);
		note(d_delay, 500);

		delay(1000);
	}


	return 0 ;
}



void note(float step, int length) {
	int endTime = millis() + length;

	while (millis() < endTime) {
		digitalWrite(DIR, direction);
		direction = !direction;
		digitalWrite(STEP, HIGH);
		digitalWrite(STEP, LOW);
	
		delayMicroseconds(step);
	}
}


/*
void D(int duration) {
	float delaytime = (1.0 / 440.0) * (TONE_STEP * 2.5);
	printf("delaytime %f", delaytime);

	for (int i = 0; i < duration; i++) {
		digitalWrite(STEP, LOW);
		delay(1);
		digitalWrite(STEP, HIGH);
		dir();
		delay(delaytime);
	}
}
*/
