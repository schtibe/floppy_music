#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

// pin 11 = wiringPi Pin 0. Use this for motor direction.
const int dirPin = 0;

// pin 12 supports pwm mode but it turns out I didn't need pwm mode in the end!
// pin 12 = wiringPi Pin 1. Use this for stepper motor.
const int stepPin = 1;

// Define an octave with naturals and sharps (Zz = rest)
enum { Cn, Cs, Dn, Ds, En, Fn, Fs, Gn, Gs, An, As, Bn, Zz };

// Define another one with flats and remaining sharps
enum { Bs, Df, Dn2, Ef, En2, Es, Gf, Gn2, Af, An2, Bf, Bn2, Zz2 };

/**
 * Frequencies in hundredths of Hz, e.g. middle A = 44000
 * 4 Octaves with 12 notes per octave, i.e. C to B
 */
const int freq[4][12] = {
   { 13081,13859,14683,15556,16481,17461,18500,19600,20765,22000,23308,24694 },
   { 26163,27718,29366,31113,32963,34923,36999,39200,41530,44000,46616,49388 },
   { 52325,55437,58733,62225,65925,69846,73999,78399,83061,88000,93233,98777 },
   { 104650,110873,117466,124451,131851,139691,147998,156798,166122,176000,186466,197553 }
};

/**
 * Frequency (in Hz) is converted to Floppy Delay using the formula:
 *   314000 / frequency = floppy delay
 * so middle A = 314000 / 440 = 714
 *
 * Lowest realistic note is delay = 1550
 * Highest realistic note is delay = 210
 */
const int floppyConv = 31400000;

// Calculate all our floppy delays at the start
int floppyDelay[4][12];


// Song2 is The Imperial March from Star Wars (note, octave, length)
const int song2_tempo = 104 * 8;
const int song2[][3] = {

};


/**
 *
 */
static void resetMotor()
{
   // To reset head position move back 10 then forward 5
   digitalWrite(dirPin, LOW);
   for (int i=0; i < 10; i++){
	  digitalWrite(stepPin, HIGH);
	  digitalWrite(stepPin, LOW);
	  delay(1);
   }

   digitalWrite(dirPin, HIGH);
   for (int i=0; i < 5; i++){
	  digitalWrite(stepPin, HIGH);
	  digitalWrite(stepPin, LOW);
	  delay(1);
   }

   delay(400);
}


/**
 *
 */
static int init()
{
   if (wiringPiSetup() == -1){
	  printf("Failed to initialize wiringPi\n");
	  return 1;
   }

   pinMode(stepPin, OUTPUT);
   pinMode(dirPin, OUTPUT);

   resetMotor();

   for (int octave = 0; octave < 4; octave++){
	  for (int note = 0; note < 12; note++){
		 floppyDelay[octave][note] = floppyConv / freq[octave][note];
	  }
   }

   return 0;
}


/**
 *
 */
static void playNote(int note, int octave, int length)
{
   static int dir = 1;
   int pause = floppyDelay[octave][note] * 10;

   int endTime = millis() + length;
   while (millis() < endTime){
	  digitalWrite(dirPin, dir);
	  if (dir == 0)
		 dir = 1;
	  else
		 dir = 0;

	  digitalWrite(stepPin, HIGH);
	  digitalWrite(stepPin, LOW);
	  delayMicroseconds(pause);
   }
}


/**
 *
 */
static void rest(int length)
{
   int endTime = millis() + length;
   while (millis() < endTime){
	  delay(5);
   }
}


/**
 * song[note_num][note, octave, length]
 */
static void playSong(const int song[][3], const int tempo)
{
   // Convert tempo in BPM to millisecs
   int noteLen = 60000 / tempo;

   for (int i = 0; song[i][0] != -1; i++){
	  int length = song[i][2] * noteLen;
	  if (song[i][0] == Zz){
		 rest(length);
	  }
	  else {
		 playNote(song[i][0], song[i][1], (7 * length) / 8);
		 rest(length / 8);
	  }
   }
}


/**
 *
 */
int main()
{
   if (init() != 0){
	  printf("init failed - Exiting\n");
	  return 1;
   }

   playSong(song2, song2_tempo);

   return 0;
}
