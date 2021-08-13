#include <Arduino.h>

void updateClock();
void displayNumber(int digitToDisplay, int offsetBy, uint32_t colourToUse);
void displayTheTime();
unsigned long getDCFTime();
void waitForDcf();
uint32_t Wheel(byte WheelPos);
void digitZero(int offset, uint32_t colour);
void digitOne(int offset, uint32_t colour);
void digitTwo(int offset, uint32_t colour);
void digitThree(int offset, uint32_t colour);
void digitFour(int offset, uint32_t colour);
void digitFive(int offset, uint32_t colour);
void digitSix(int offset, uint32_t colour);
void digitSeven(int offset, uint32_t colour);
void digitEight(int offset, uint32_t colour);
void digitNine(int offset, uint32_t colour);