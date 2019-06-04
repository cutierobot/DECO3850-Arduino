// const int gasPumpLever = 2;
#include "Tank.h"

IndividualTank pilot;
IndividualTank intel;
IndividualTank gunner;
TanksEnum tanksEnum;
// TODO fail safe  for not exceeding 3 individual lights
// fix random middle light in neopixels main tank

int gasNozleState = 0; // button state for button on gas handle

long tankDecreaseNumber =
    120000; //in ms how long the level will be in individual tanks

// const int mainTankLevel = 10; // the amount of lights for main tank
int mainTankCurrent = 0; // how many resources in main tank now
int pilotTank = 0; // how many resources the pilot has
int intelTank = 0; // resou
int gunnerTank = 0; // resou
// https://learn.adafruit.com/led-art-with-fadecandy?view=all
// @jason said for main will need external power source for it

// Declare our NeoPixel strip object:
Adafruit_NeoPixel mainStrip = Adafruit_NeoPixel(MAIN_TANK_COUNT, MAIN_TANK_PIN,
                                              NEO_RGB + NEO_KHZ800);

Adafruit_NeoPixel pilotStrip = Adafruit_NeoPixel(PILOT_TANK_COUNT,
                                                 PILOT_TANK_PIN, NEO_RGBW + NEO_KHZ800);

Adafruit_NeoPixel intelStrip = Adafruit_NeoPixel(INTEL_TANK_COUNT,
                                                 INTEL_TANK_PIN, NEO_RGBW + NEO_KHZ800);

Adafruit_NeoPixel gunnerStrip = Adafruit_NeoPixel(GUN_TANK_COUNT,
                                                 GUN_TANK_PIN, NEO_RGBW + NEO_KHZ800);

//=========== RESET MAIN TANK COUNT TO 10 ==========
const uint32_t lightBlueMain = mainStrip.Color(0, 0, 255);//GET RID OF ZERO LAST
const uint32_t lightBluePilot = pilotStrip.Color(0, 0, 255, 0);
const uint32_t lightBlueIntel = intelStrip.Color(0, 0, 255, 0);
const uint32_t lightBlueGun = gunnerStrip.Color(0, 0, 255, 0);

const uint32_t lightsOffMain = mainStrip.Color(0, 0, 0);//GET RID OF 0 LAST
const uint32_t lightsOffPilot = pilotStrip.Color(0, 0, 0, 0);
const uint32_t lightsOffIntel = intelStrip.Color(0, 0, 0, 0);
const uint32_t lightsOffGun = gunnerStrip.Color(0, 0, 0, 0);


int const intelButton2 = 10;
bool intelPressed = false;
void setup() {
	// https://learn.adafruit.com/multi-tasking-the-arduino-part-1/using-millis-for-timing
	// look at above for decresaing over time
	// This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
	#if defined (__AVR_ATtiny85__)
		if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
	#endif
	mainStrip.begin(); //initialise main tank NeoPixels
	mainStrip.show(); // Initialize all pixels to 'off'

	pilotStrip.begin(); // Initialize pilot tank lights
	// pilotStrip.setPixelColor(0, lightsOffPilot);
	// pilotStrip.setPixelColor(1, lightsOffPilot);
	// pilotStrip.setPixelColor(2, lightsOffPilot);
	pilotStrip.show();

	intelStrip.begin();
	intelStrip.show();

	gunnerStrip.begin();
	gunnerStrip.show();

	Serial.begin(9600);

	pinMode(gasPumpLever, INPUT);
	pinMode(intelButton, INPUT);
	pinMode(pilotButton, INPUT);
	pinMode(gunnerButton, INPUT);

	pinMode(intelButton2, INPUT);

	individaul_tanks_setup();
	// Serial.println("finished setup");
	// Serial.println("true = " + String(true) + "false = " + String(false));
	handshake();
	intelPressed = false;
}

void loop() {
	received_messages();
	if (is_gas_handle_pressed()) {
		delay(50);
		// get button of gasNozle
		individual_tank();
		delay(1000);
	}

	if(digitalRead(intelButton2) == HIGH && !intelPressed){
	    Serial.println("I");
	    Serial.flush();
	    intelPressed = true;
	    delay(10);
	}
	if(digitalRead(intelButton2) == LOW && intelPressed){
	    intelPressed = false;
	}
}

/**
 * Sets the three IndividualTank structs to defualt inital values for setup
 */
void individaul_tanks_setup() {
	pilot.onePrevious = 0;
	intel.onePrevious = 0;
	gunner.onePrevious = 0;

	pilot.twoPrevious = 0;
	intel.twoPrevious = 0;
	gunner.twoPrevious = 0;

	pilot.threePrevious = 0;
	intel.threePrevious = 0;
	gunner.threePrevious = 0;

	pilot.oneCurrent = 0;
	intel.oneCurrent = 0;
	gunner.oneCurrent = 0;

	pilot.twoCurrent = 0;
	intel.twoCurrent = 0;
	gunner.twoCurrent = 0;

	pilot.threeCurrent = 0;
	intel.threeCurrent = 0;
	gunner.threeCurrent = 0;

	pilot.oneOn = false;
	intel.oneOn = false;
	gunner.oneOn = false;

	pilot.twoOn = false;
	intel.twoOn = false;
	gunner.twoOn = false;

	pilot.threeOn = false;
	intel.threeOn = false;
	gunner.threeOn = false;
}

/**
 * Used to determine if the user is pressing down the bike brake on the gas
 * nozzle.
 * @return {Boolean} true if handle being pressed down, false otherwise
 */
bool is_gas_handle_pressed() {
	gasNozleState = digitalRead(gasPumpLever);
	if (gasNozleState == 1) {
		// button is pressed and held down
		return true;
	} else {
		// button is not pressed
		return false;
	}
}

/**
 * Determines, if any, what tank to give a block of resources to.
 * @todo finish of this function, add intel and gunner
 */
void individual_tank() {
	if (is_pilot_tank() && have_fuel()) {
		// give one level to pilot
		// remove level from main tank
		// Serial.println("going to fill pilot tank one level");
		give_fuel(&pilot, PILOT);
		int lightOn = which_light_on(&pilot);
		if(lightOn == -1){
		    lightOn = 3;
		}
		Serial.println("A" + String(lightOn));
	}

	if (is_intel_tank() && have_fuel()) {
		// give one level to intel tank
		// remove level from main tank
		// Serial.println("going to fill intel tank one level");
		give_fuel(&intel, INTEL);

		int lightOn = which_light_on(&intel);
		if(lightOn == -1){
		    lightOn = 3;
		}
		Serial.println("B" + String(lightOn));
	}

	if (is_gunnar_tank() && have_fuel()) {
		// give one level to gunner tank
		// remove level from main tank
		// Serial.println("going to fill intel tank one level");
		give_fuel(&gunner, GUNNER);
		int lightOn = which_light_on(&gunner);
		if(lightOn == -1){
		    lightOn = 3;
		}
		Serial.println("C" + String(lightOn));
	}
}

/**
 * Determines if the pilots tank is to be filled with resources
 * @return {Boolean} Returns true if button pressed, false otherwise
 */
bool is_pilot_tank() {
	int buttonState = digitalRead(pilotButton);
	// Serial.println("pilot state = " + String(buttonState));
	if (buttonState == 1) {
		return true;
	}
	return false;
}

/**
 * Determines if the intel pilot tanks is to be filled with resources
 * @return {Boolean} Returns true if button pressed, false otherwise
 */
bool is_intel_tank() {
	int buttonState = digitalRead(intelButton);
	// Serial.println("intel state = " + String(buttonState));
	if (buttonState == 1) {
		return true;
	}
	return false;
}

/**
 * Determines if the gunner tank is to be filled with resources
 * @return {Boolean} Returns true if the button is pressed, false otherwise
 */
bool is_gunnar_tank() {
	int buttonState = digitalRead(gunnerButton);
	// Serial.println("Gun state = " + String(buttonState));
	if (buttonState == 1) {
		return true;
	}
	return false;
}


/**
 * Determines if there is any fuel in the main tank. If there is decreases tank
 * by 1;
 * @return {Boolean} True if mainTank is full, false otherwise
 */
bool have_fuel() {
	if (mainTankCurrent > 0) {
		mainTankCurrent--;
		// Serial.println("has fuel true");
		return true;
	}
	return false;
}

/**
 * Gives one fuel level to the correct tank that it needs to go to. Turns the light
 * on and sets in the struct that the light is on.
 * @param  {struct IndividualTank} tank          The tank struct 
 * @param  {enum TanksEnum}       section       The role the level is being given to
 */
void give_fuel(IndividualTank *tank, TanksEnum section) {
	int turnOn = which_light_on(tank);
	// Serial.println("turnOn = " + String(turnOn));
	if(turnOn != -1){
	    switch (section) {
	        case PILOT:
	          pilotStrip.setPixelColor(turnOn, lightBluePilot);
	          pilotStrip.show();
	          break;
	        case INTEL:
	          intelStrip.setPixelColor(turnOn, lightBlueIntel);
	          intelStrip.show();
	          break;
			case GUNNER:
			  gunnerStrip.setPixelColor(turnOn, lightBlueGun);
	          gunnerStrip.show();
	          break;
	        default:
	          Serial.println("E:TanksEnum not working correctly");
	          break;
	    }

	    switch (turnOn) {
	        case 0:
	          // do something
	          tank->oneOn = true;
	          // Serial.println("tank one turned on");
	          break;
	        case 1:
	          // do something
	          tank->twoOn = true;
	          break;
	        case 2:
	          // do something
	          tank->threeOn = true;
	          break;
	        default:
	          Serial.println("E: which_light_on() not returning correctly");
	    }
	    main_tank_remove_level();
	}
}

// a function for caluclating when to subtract fuel form individual tanks
// void take_fuel() {
	
// }

/**
 * Determins what lights are on for a particular tank and returns the array 
 * number of the light to turn on. Returns -1 if all lights are on for the tank.
 * @param  {struct IndividualTank} tank          The tank to check for lights on
 * @return {Integer}          The array number of the light to turn on, or -1
 *  if all lights already on.
 */
int which_light_on(IndividualTank *tank) {
	// if one not one then none on
	if(!tank->oneOn){
	    return 0;
	} 

	// if tank one on but not 2 then only 1 on
	if(tank->oneOn && !tank->twoOn){
	    return 1;
	}

	// if tank 1,2 on but not 3, then two lights on
	if(tank->oneOn && tank->twoOn && !tank->threeOn){
		return 2;
	}

	// if all lights on return three lights
	if(tank->oneOn && tank->twoOn && tank->threeOn){
	    return -1;
	}
}

/**
 * Gives the main tank one level of light
 */
void main_tank_give_level() {
	int pinNumber = mainTankCurrent - 1;
	// Serial.println("main tank lightup");
	mainStrip.setPixelColor(pinNumber, lightBlueMain);
	mainStrip.show();
}

/**
 * Removes one level of light from the main tank.
 */
void main_tank_remove_level() {
	// int light = mainTankCurrent - 1;
	mainStrip.setPixelColor(mainTankCurrent, lightsOffMain);
	mainStrip.show();
}

void remove_level(IndividualTank *tank, TanksEnum section) {
	int turnedOn = which_light_on(tank);
	int turnOff = turn_off(turnedOn);
	if(turnOff != -1){
        switch (section) {
            case PILOT:
              pilotStrip.setPixelColor(turnOff, lightsOffPilot);
              pilotStrip.show();
              break;
            case INTEL:
              intelStrip.setPixelColor(turnOff, lightsOffIntel);
              intelStrip.show();
              break;
    		case GUNNER:
    		  gunnerStrip.setPixelColor(turnOff, lightsOffGun);
              gunnerStrip.show();
              break;
            default:
              Serial.println("E: TanksEnum not working correctly");
              break;
        }

        switch (turnOff) {
	        case 0:
	          // do something
	          tank->oneOn = false;
	          // Serial.println("tank one turned on");
	          break;
	        case 1:
	          // do something
	          tank->twoOn = false;
	          break;
	        case 2:
	          // do something
	          tank->threeOn = false;
	          break;
	        default:
	          Serial.println("E:which_light_on() not returning correclty");
	    }
	}
}

int turn_off(int on) {
	if(on == -1){
	    return 2;
	}

	if(on == 2){
	    return 1;
	}

	if(on == 1){
	    return 0;
	}

	if(on == 0){
	    return -1;
	}
}

/**
 * Listens for messages from Unity
 */
void received_messages() {
	if (Serial.available() > 0) {
		// reads string until reads in a newline
		String message = Serial.readStringUntil('\n'); // read it and store it in val
		
		switch (message.charAt(0)) {
			case 'T':
				if(message.charAt(1) == '+' && mainTankCurrent != 10){
					// main tank resources increased by one
				   mainTankCurrent++;
				   // Serial.println("main tank give juice");
				   main_tank_give_level();
				}
				break;
			// TODO do check for minus when not tired
			case 'A':
				remove_level(&pilot,PILOT);
				break;
			case 'B':
				remove_level(&intel,INTEL);
				break;
			case 'C':
				remove_level(&gunner,GUNNER);
				break;
			case 'P':
				break;
			case 'R':
				resetFunc();
				break;
			default:
				Serial.println("E:havn't received message");
		}
	}
}

// void send_message(string message) {

// }

/** Establishes contact with Unity. In dummy terms keeps sending Marco until
 * Unity sends out a Polo.
 */
void handshake() {
	while (Serial.available() <= 0) {
		Serial.flush();
		Serial.println("M");   // send a capital M, for Marco
		delay(300);
	}
}
