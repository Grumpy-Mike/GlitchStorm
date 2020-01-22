/* Glitch Storm 0.7
// CC By Sa Spherical Sound Society 2019
// Heavy inspiration in Bytebeat (Viznut)
// Some equations are empty. You can collaborate sending your new finding cool sounding ones to the repository
// https://github.com/spherical-sound-society/glitch-storm
***********************************************************
Tided up by Mike Cook Jan 2020

Cleaned up the code to remove unused variables, unnecessary blank lines, and library #includes.

Added a more interesting blinking LED for panel display using a two colour LED.

The timing clockout easter-egg mode simply messes up the LED number display so I have removed it.

There were only sounds 1 to 15 so I added a sound 0 from a commented out sound algorithm in case 5.

The variables aTop, aBottom, bTop, bBottom, cTop and cBottom only change with program numbers, it is a waste of precious interrupt time to set them on every sample interrupt. So I set them with the program number, from a predefined array, with original values, when you read the pots with analogRead.
For case 14 -  "cTop = 1; cBottom = 6;" which makes no sense, apart from disabling the third pot. So these values were swapped.

The buttonsManager code was full of timing to see if it was a long press for either button. But then nothing was done with this information so I simplified the code by removing it. There was also code in the button manager to set variables to show a button was being held down, this is not necessary, again it was removed.
    
Using the C language there is no advantage in short variable names unlike interpreted languages, but I have kept most of them for compatible reasons
 
I converted the sample interrupt ISR to use a vectored jump table. This is faster than the case statement originally used so allows more time to compute the next sample.

If the blinking LED stops or slows down then the sound routine is too much for the sample rate or processor. What is happening is that the time spent generating each sample is making the millis() timer miss some calls and so the blinking rate which is controlled by it will slow down or appear to stop. It will still produces sound though.

Take time to listen to what it is producing as sometimes the sequences are long. Less knob twiddling is often more.
*/

#define ledPin   13
#define ledPin2   6
#define speaker 11
#define upButtonPin 2
#define downButtonPin 4

//int SAMPLE_RATE = 4096;
//int SAMPLE_RATE = 8192;
//int SAMPLE_RATE = 9216;
int SAMPLE_RATE = 16384;
//int SAMPLE_RATE = 22000;

long t = 0;
volatile int a, b, c;
int outValue;
byte countPins[] = {10, 9, 8, 7};
volatile char programNumber = 14; // turn on default
byte totalPrograms = 16;
byte buttonPin [] = {upButtonPin, downButtonPin}; 
bool buttonState [] = {false, false};              
bool lastButtonState [] = {false, false} ;   

// pot limits for all program numbers in soundx functions
// function number 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 
int aTop[] =    { 24,10,12,30,12,24,10,10,12,16, 8,16,18,18, 8,16}; 
int aBottom[] = { 0, 0, 0,6, 0,  0,  3, 0, 0, 0, 0, 4, 0,10, 0, 0};
int bTop[] =    { 22,14,20,16,16,22,28,22,20,86,22,22, 8,14,16,22};
int bBottom[] = { 0, 0, 4, 0, 0,  0, 0,10, 0, 0, 0, 1, 1, 1, 0, 0};
int cTop[] =    { 16,14,12,10,10,16,10, 8,20,26,13, 9,14,10, 6,13};
int cBottom[] = { 0, 0, 5, 0, 0, 0,  3, 0, 0, 0, 0, 2, 5, 1, 1, 0};
byte blinkCount = 0;
unsigned long  blinkRate = 500;
unsigned long lastBlink = 0;


//typedef void (*SoundFunction) ();
//SoundFunction soundPointers[16];

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  for(int i = 0; i<2; i++) { // initilise buttons
    pinMode(buttonPin[i], INPUT_PULLUP);
    buttonState[i] = digitalRead(buttonPin[i]);
    lastButtonState[i] = buttonState[i];
  }
    for(int i = 0; i<4; i++){
  pinMode(countPins[i], OUTPUT);
  }
  ledCounter();
  initSound();
}

void loop() {
  buttonsManager();
  potsManager();
  blinkLED();
}

void blinkLED() {
  // blink two colour LED through all combinations
  if (millis() - lastBlink > blinkRate) {
    lastBlink = millis();
    blinkCount += 1;
    if( blinkCount & 1) digitalWrite(ledPin, HIGH); else digitalWrite(ledPin, LOW);
    if( blinkCount & 2) digitalWrite(ledPin2, HIGH); else digitalWrite(ledPin2, LOW);
 }
}

void potsManager() {
    a = map(analogRead(0), 0, 1023, aBottom[programNumber], aTop[programNumber]);
    b = map(analogRead(1), 0, 1023, bBottom[programNumber], bTop[programNumber]);
    c = map(analogRead(2), 0, 1023, cBottom[programNumber], cTop[programNumber]);   
}

void ledCounter() {
  //show program number in binary
  digitalWrite(countPins[0], bitRead(programNumber, 0));
  digitalWrite(countPins[1], bitRead(programNumber, 1));
  digitalWrite(countPins[2], bitRead(programNumber, 2));
  digitalWrite(countPins[3], bitRead(programNumber, 3));
}

void buttonsManager() {
  for(int i = 0; i<2; i++) { // read
    buttonState[i] = digitalRead(buttonPin[i]);
    if(buttonState[i] != lastButtonState[i] && buttonState[i] == LOW) {
      if( i == 0 ) updateProgramNumber( 1, totalPrograms-1, 0);
      else updateProgramNumber( -1, 0, totalPrograms-1); 
    }             
    lastButtonState[i] = buttonState[i];
  } 
} 

void updateProgramNumber(char increment, char limit, char wrap){
  noInterrupts(); 
  programNumber += increment;
  if( increment < 0 && programNumber < limit ) programNumber = wrap;
  if( increment > 0 && programNumber > limit ) programNumber = wrap;
  interrupts();
  ledCounter(); // show new count
  delay(20); // debounce delay           
}
