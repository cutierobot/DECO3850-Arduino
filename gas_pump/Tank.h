#ifndef TANKS
#define TANKS 

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
// Which pin on the Arduino is connected to the NeoPixels MAIN_TANK?
#define MAIN_TANK_PIN 6 //NeoPixel for main tank(purple)
#define PILOT_TANK_PIN 7 // NeoPixels for pilot pin(brown)
#define INTEL_TANK_PIN 8 // NeoPixels for intel pin(yellow)
#define GUN_TANK_PIN 9 // NeoPixels for gunner pin

// How many NeoPixels are attached to the Arduino MAIN_TANK?
#define MAIN_TANK_COUNT 10 //@tom said so
#define PILOT_TANK_COUNT 3 // number of lights on pilot NeoPixel
#define INTEL_TANK_COUNT 3 // number of lights on intel NeoPixel
#define GUN_TANK_COUNT 3 // number of lights on gunnar NeoPixel

typedef struct IndividualTank {
	long onePrevious; // previous value of light one
	long twoPrevious; // previous value of light two
	long threePrevious; // previous value of light three
	unsigned long oneCurrent; // current value of light one
	unsigned long twoCurrent; // current value of light two 
	unsigned long threeCurrent; // current value of light three
	bool oneOn; // is light one on?
	bool twoOn; // is light one two?
	bool threeOn; // is light one three?
};

typedef enum TanksEnum {
	PILOT,
	INTEL,
	GUNNER
};

const int gasPumpLever = 2; // gas handle pin number(blue)
const int intelButton = 3; // intel tank pin number(yellow)
const int pilotButton = 4; // pilot tank pin number(orange)
const int gunnerButton = 5; //gunnar tank pin number(green)

// the reset functuion call and will reset board
void(* resetFunc) (void) = 0;

// NeoPixels

// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

#endif