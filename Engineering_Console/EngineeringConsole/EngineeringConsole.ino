#include "console.h"
Step step;

char previousState = '0'; // the previous state for the mdiddle button to send

void setup() {
	// put your setup code here, to run once:
	pinMode(D1, INPUT);
	pinMode(D2, INPUT);
	pinMode(D3, INPUT);

	pinMode(E1, INPUT);
	pinMode(E2, INPUT);
	pinMode(E3, INPUT);

	pinMode(F1, INPUT);
	pinMode(F2, INPUT);
	pinMode(F3, INPUT);
	pinMode(F4, INPUT);
	pinMode(F5, INPUT);

	pinMode(G1, INPUT);
	pinMode(G2, INPUT);
	pinMode(G3, INPUT);
	pinMode(G4, INPUT);
	pinMode(G5, INPUT);

	Serial.begin(9600);
	handshake();
	Serial.flush();
}

void loop() {
	recieve();

	if (repair) {
		if (!get_step_complete(STEP_ONE)) {
			int state = read_component(type1, seqPin1);
			delay(5);
			if_step_done(state, STEP_ONE);
		}

		if (!get_step_complete(STEP_TWO)) {
			int state = read_component(type2, seqPin2);
			delay(5);
			if_step_done(state, STEP_TWO);
		}

		if (!get_step_complete(STEP_THREE)) {
			int state = read_component(type3, seqPin3);
			delay(5);
			if_step_done(state, STEP_THREE);
		}

	}
	delay(50);
	if_sequence_complete();
}

/**
 * Determins what position the older pots are at. 5 states on pots
 * @param  {int} state         The current state of the older potentiometer
 * @return {int}     The level the current older is at, or -1 for error
 */
int other_pot(int state) {
	int ret = 0;
	if (state >= 0 && state < 25) {
		ret = 1;

	} else if (state > 30 && state < 55) {
		ret = 2;

	} else if (state > 125 && state < 145) {
		ret = 3;

	} else if (state > 215 && state < 230) {
		ret = 4;

	} else if (state > 240 && state <= 255) {
		ret = 5;

	} else {
		ret = -1;
	}
	delay(5);
	return ret;
}


/**
 * Establishes contact with Unity. In dummy terms keeps sending Marco until
 * Unity sends out a Polo.
 */
void handshake() {
	while (Serial.available() <= 0) {
		Serial.flush();
		Serial.println("M");   // send a capital M, for Marco
		Serial.flush();
		delay(300);
	}
}

/**
 * Determines what level the knob pot is at currently. 5 states on knob pot
 * @param  {int} state         The current state number of the pot knob
 * @return {int}     The level the current looked at knob pot is at, or -1 if error
 */
int knob_pot(int state) {
	int ret = 0;
	if (state > 230 && state < 256) {
		ret = 1;

	} else if (state > 210 && state < 220) {
		ret = 2;

	} else if (state > 120 && state < 130) {
		ret = 3;

	} else if (state > 35 && state < 50) {
		ret = 4;

	} else if (state > 0 && state < 10) {
		ret = 5;
	}

	delay(5);
	return ret;
}

/**
 * Listens for messages from Unity. If message has "reset" will perform the rest
 * function.
 */
void recieve() {
	if (Serial.available() > 0) {
		String message = Serial.readStringUntil('\n'); // read it and store it in val
		if (message != "P") {
			if (message == "R") {
				resetFunc();
			}

			if (message.length() == 9) {
				// get 3 char message for sequence
				String sequenceOne = message.substring(0, 3);
				String sequenceTwo = message.substring(3, 6);
				String sequenceThree = message.substring(6, 9);

				// get sequence types(slide, illum button, etc)
				type1 = sequenceOne.charAt(0);
				type2 = sequenceTwo.charAt(0);
				type3 = sequenceThree.charAt(0);

				// get pins for sequence components
				seqPin1 = name_to_pin(sequenceOne);
				seqPin2 = name_to_pin(sequenceTwo);
				seqPin3 = name_to_pin(sequenceThree);

				// get state looking for for pin
				seq1State = (int)sequenceOne.charAt(2) - '0';
				seq2State = (int)sequenceTwo.charAt(2) - '0';
				seq3State = (int)sequenceThree.charAt(2) - '0';

				repair = true;
			}
		}
	}
}

/**
 * Takes in a message and determines its pin number assigned to it.
 * @param  {String}  message       The 3 character message split from Unity message
 * @return {int}     The pin number of the component
 */
int name_to_pin(String message) {
	// 3 char string
	String name = message.substring(0, 2);
	if (name.charAt(0) == 'D' || name.charAt(0) == 'E') {
		if (name.charAt(0) == 'D') {
			switch (name.charAt(1)) {
				case '1':
					return A0;
					break;
				case '2':
					return A1;
					break;
				case '3':
					return A2;
					break;
				default:
					// something wrong
					break;
			}
		} else {
			// E
			switch (name.charAt(1)) {
				case '1':
					return A3;
					break;
				case '2':
					return A4;
					break;
				case '3':
					return A5;
					break;
				default:
					// something wrong
					break;
			}
		}
	} else {
		if (name == "F1") {
			return 2;
		} else if (name == "F2") {
			return 3;
		} else if (name == "F3") {
			return 4;
		} else if (name == "F4") {
			return 5;
		} else if (name == "F5") {
			return 6;
		} else if (name == "G1") {
			return 7;
		} else if (name == "G2") {
			return 8;
		} else if (name == "G3") {
			return 9;
		} else if (name == "G4") {
			return 10;
		} else if (name == "G5") {
			return 11;
		} else {
			// something whent wrong
		}
	}
}

/**
 * Reads in a component and returns its current value, whether it be current
 * level of whether a button was pressed
 * @param  {char} type          The character assigned to the different
 *                     					types of physical inputs
 * @param  {int}  pin           The pin number assigned to this componemt type
 * @return {int}      The level/button state of the component, or -1 if error
 */
int read_component(char type, int pin) {
	if (type == 'D') {
		int val = analogRead(pin) / 4;
		return other_pot(val);

	} else if (type == 'E') {
		int val = analogRead(pin) / 4;
		return knob_pot(val);
	} else if (type == 'F') {
		int buttonState = digitalRead(pin);
		if (buttonState == HIGH) {
			// button pressed
			return 1;
		}
		return 0;
	} else if (type == 'G') {
		int buttonState = digitalRead(pin);
		if (buttonState == HIGH) {
			// button pressed
			return 1;
		}
		return 0;
	} else {
		// not valid
		return -1;
	}
}

/**
 * Retrieves the requested steps completion status.
 * @param  {Step} step          The Step enum value for the sequence
 * @return {bool}           True if step completed, false otherwise
 */
bool get_step_complete(Step step) {
	switch (step) {
		case STEP_ONE:
			// do something
			return sequenceOneComplete;
			break;
		case STEP_TWO:
			// do something
			return sequenceTwoComplete;
			break;
		case STEP_THREE:
			// do something
			return sequenceThreeComplete;
			break;
	}
}

/**
 * Determines if a step has been completed and sets the value accordingly.
 * @param  {int}  compState     The state of the component in the step
 * @param  {Step} step          The Step enum value for the step
 */
void if_step_done(int compState, Step step) {
	if (step == STEP_ONE) {
		if (compState == seq1State) {
			Serial.println("true1");
			Serial.flush();
			sequenceOneComplete = true;
		}
	} else if (step == STEP_TWO) {
		if (compState == seq2State) {
			Serial.println("true2");
			Serial.flush();
			sequenceTwoComplete = true;
		}
	} else if (step == STEP_THREE) {
		if (compState == seq3State) {
			Serial.println("true3");
			Serial.flush();
			sequenceThreeComplete = true;
		}
	} else {
	}
}

/**
 * Determines if a repair has successfully been completed. If it has then sends
 * a message to Unity of "r" informing repair has been completed and sets 'repair'
 * to false
 * @see repair
 */
void if_sequence_complete() {
	int val = digitalRead(F3);
	if (sequenceOneComplete && sequenceTwoComplete && sequenceThreeComplete &&
	    val == HIGH && previousState != 'r') {
		repair = false;
		Serial.println("r");
		Serial.flush();
		previousState = 'r';
		reset();
	}

	if (val == HIGH && previousState != 'h' && (!sequenceOneComplete ||
	                                            !sequenceTwoComplete || !sequenceThreeComplete)) {
		Serial.println("H");
		Serial.flush();
		previousState = 'h';
	}
}

/**
 * Basic reset function that turns all the essentil values back to inital_setup
 */
void reset() {
	sequenceOneComplete = false;
	sequenceTwoComplete = false;
	sequenceThreeComplete = false;
	seqPin1 = 0;
	seqPin2 = 0;
	seqPin3 = 0;
	seq1State = 0;
	seq2State = 0;
	seq3State = 0;
}

