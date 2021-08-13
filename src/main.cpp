#include <Arduino.h>
#include <main.h>
#include <Adafruit_NeoPixel.h>
#include "DCF77.h"
#include "Time.h"

#define LEDCLOCK_PIN    6
#define LEDDOWNLIGHT_PIN    5

#define LEDCLOCK_COUNT 216
#define LEDDOWNLIGHT_COUNT 12

#define DCF_PIN 2 
#define DCF_INTERRUPT 0

bool timeSynced = false;
int prevMinute = 1;
DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT, false);

uint32_t clockMinuteColour = 0x800000; // red 
uint32_t clockHourColour = 0x008000;   // green
int clockFaceBrightness = 0;
unsigned long lastUpdate = 0 ;

Adafruit_NeoPixel stripClock(LEDCLOCK_COUNT, LEDCLOCK_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripDownlighter(LEDDOWNLIGHT_COUNT, LEDDOWNLIGHT_PIN, NEO_GRB + NEO_KHZ800);


//Smoothing of the readings from the light sensor so it is not too twitchy
const int numReadings = 12;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
long total = 0;                  // the running total
long average = 0;                // the average

void setup() {
  Serial.begin(9600);
  DCF.Start();
  setSyncInterval(1);
  setSyncProvider(getDCFTime);

  stripClock.begin();
  stripClock.clear();          
  stripClock.show();          
  stripClock.setBrightness(50);
 
  stripDownlighter.begin();   
  stripDownlighter.clear(); 
  stripDownlighter.show(); 
  stripDownlighter.setBrightness(35);
}

void loop() {
    if (minute() != prevMinute) {
        prevMinute = minute();
        updateClock();
    } 
    
    now();
    delay(100);
}

void updateClock() {
  displayTheTime();
}

unsigned long getDCFTime() { 
  time_t DCFtime = DCF.getTime();
  Serial.print(".");
  if (DCFtime != 0) {
    timeSynced = true;
    Serial.println("GOT TIME");
    stripDownlighter.fill(stripDownlighter.Color(random(1,250), random(1,250), random(1,250)), 0, LEDDOWNLIGHT_COUNT);
    stripDownlighter.show();
  }
  
  return DCFtime;
}



/*
  TIME WAITING ANIMATION
*/
void waitForDcf() {
  static uint16_t j=0;
    for(int i=0; i<stripClock.numPixels(); i++) {
      stripClock.setPixelColor(i, Wheel((i+j) & 255));
    }
    stripClock.show();
     j++;
  if(j >= 256) j=0;
  lastUpdate = millis(); // time for next change to the display
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return stripClock.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return stripClock.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return stripClock.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

/*
  DIGIT DISPLAYING
*/
void displayTheTime() {
  stripClock.clear(); //clear the clock face 

  int firstMinuteDigit = prevMinute % 10; //work out the value of the first digit and then display it
  displayNumber(firstMinuteDigit, 0, clockMinuteColour);

  int secondMinuteDigit = floor(prevMinute / 10); //work out the value for the second digit and then display it
  displayNumber(secondMinuteDigit, 63, clockMinuteColour);  


  int firstHourDigit = hour(); //work out the value for the third digit and then display it
  if (firstHourDigit > 12){
    firstHourDigit = firstHourDigit - 12;
  }

  firstHourDigit = firstHourDigit % 10;
  displayNumber(firstHourDigit, 126, clockHourColour);


  int secondHourDigit = hour(); //work out the value for the fourth digit and then display it
  if (secondHourDigit > 12){
    secondHourDigit = secondHourDigit - 12;
  }
  if (secondHourDigit > 9){
    stripClock.fill(clockHourColour,189, 18); 
  }

  stripClock.show();
}

void displayNumber(int digitToDisplay, int offsetBy, uint32_t colourToUse) {
    switch (digitToDisplay){
    case 0:
    digitZero(offsetBy,colourToUse);
      break;
    case 1:
      digitOne(offsetBy,colourToUse);
      break;
    case 2:
    digitTwo(offsetBy,colourToUse);
      break;
    case 3:
    digitThree(offsetBy,colourToUse);
      break;
    case 4:
    digitFour(offsetBy,colourToUse);
      break;
    case 5:
    digitFive(offsetBy,colourToUse);
      break;
    case 6:
    digitSix(offsetBy,colourToUse);
      break;
    case 7:
    digitSeven(offsetBy,colourToUse);
      break;
    case 8:
    digitEight(offsetBy,colourToUse);
      break;
    case 9:
    digitNine(offsetBy,colourToUse);
      break;
    default:
     break;
  }
}

void digitZero(int offset, uint32_t colour) {
    stripClock.fill(colour, (0 + offset), 27);
    stripClock.fill(colour, (36 + offset), 27);
}

void digitOne(int offset, uint32_t colour) {
    stripClock.fill(colour, (0 + offset), 9);
    stripClock.fill(colour, (36 + offset), 9);
}

void digitTwo(int offset, uint32_t colour){
    stripClock.fill(colour, (0 + offset), 18);
    stripClock.fill(colour, (27 + offset), 9);
    stripClock.fill(colour, (45 + offset), 18);
}

void digitThree(int offset, uint32_t colour){
    stripClock.fill(colour, (0 + offset), 18);
    stripClock.fill(colour, (27 + offset), 27);
}

void digitFour(int offset, uint32_t colour){
    stripClock.fill(colour, (0 + offset), 9);
    stripClock.fill(colour, (18 + offset), 27);
}

void digitFive(int offset, uint32_t colour){
    stripClock.fill(colour, (9 + offset), 45);
}

void digitSix(int offset, uint32_t colour){
    stripClock.fill(colour, (9 + offset), 54);
}

void digitSeven(int offset, uint32_t colour){
    stripClock.fill(colour, (0 + offset), 18);
    stripClock.fill(colour, (36 + offset), 9);
}

void digitEight(int offset, uint32_t colour){
    stripClock.fill(colour, (0 + offset), 63);
}

void digitNine(int offset, uint32_t colour){
    stripClock.fill(colour, (0 + offset), 45);
}