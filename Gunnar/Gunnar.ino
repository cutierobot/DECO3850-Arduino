#include <Keyboard.h>

int const blue = 2;
int const green = 3;
int const red = 4;

int const redLed = 12;
int const greenLed = 11;
int const blueLed = 10;

int const slider = A0;
char previousState = '0';

int curColour = -1;
bool redPrev = false;
bool greenPrev = false;
bool bluePrev = false;



void setup() {
	// make pin 2 an input and turn on the
	// pullup resistor so it goes high unless
	// connected to ground:
	// pinMode(2, INPUT_PULLUP);
	pinMode(blue, INPUT);
	pinMode(green, INPUT);
	pinMode(red, INPUT);
	pinMode(redLed, OUTPUT);
	pinMode(greenLed, OUTPUT);
	pinMode(blueLed, OUTPUT);
	pinMode(slider, INPUT);

	digitalWrite(redLed, HIGH);
	digitalWrite(greenLed, HIGH);
	digitalWrite(blueLed, HIGH);

	Keyboard.begin();
}

void loop() {
	// put your main code here, to run repeatedly:

	if (digitalRead(red) == HIGH && redPrev == false && curColour != 1) {
		redPrev = true;
		curColour = 1;
		digitalWrite(redLed, HIGH);
		digitalWrite(greenLed, LOW);
		digitalWrite(blueLed, LOW);
		Keyboard.write('1');
		delay(100);
	}
	if (redPrev == true && digitalRead(red) == LOW) {
		redPrev = false;
	}

	if (digitalRead(green) == HIGH && greenPrev == false && curColour != 3) {
		greenPrev = true;
		curColour = 3;
		digitalWrite(redLed, LOW);
		digitalWrite(greenLed, HIGH);
		digitalWrite(blueLed, LOW);
		Keyboard.write('3');
		delay(100);
	}
	if (greenPrev == true && digitalRead(green) == LOW) {
		greenPrev = false;
	}

	if (digitalRead(blue) == HIGH && bluePrev == false && curColour != 2) {
		bluePrev = true;
		curColour = 2;
		digitalWrite(redLed, LOW);
		digitalWrite(greenLed, LOW);
		digitalWrite(blueLed, HIGH);
		Keyboard.write('2');
		delay(100);
	}
	if (bluePrev == true && digitalRead(blue) == LOW) {
		bluePrev = false;
	}


	// if (digitalRead(red) == HIGH) {
	// 	//Send the message
	// 	Keyboard.write('1');
	// 	// digitalWrite(redLed, HIGH);
	// 	digitalWrite(greenLed, LOW);
	// 	digitalWrite(blueLed, LOW);
	// 	delay(100);
	// }/* else {
	// 	// digitalWrite(redLed, LOW);
	// 	digitalWrite(greenLed, HIGH);
	// 	digitalWrite(blueLed, HIGH);
	// }*/
	
	// if (digitalRead(blue) == HIGH) {
	// 	//Send the message
	// 	Keyboard.write('2');
	// 	// digitalWrite(blueLed, HIGH);
	// 	digitalWrite(greenLed, LOW);
	// 	digitalWrite(redLed, LOW);
	// 	delay(100);
	// } /*else {
	// 	// digitalWrite(blueLed, LOW);
	// 	digitalWrite(greenLed, HIGH);
	// 	digitalWrite(blueLed, HIGH);
	// }*/

	// if (digitalRead(green) == HIGH) {
	// 	//Send the message
	// 	Keyboard.write('3');
	// 	// digitalWrite(greenLed, HIGH);
	// 	digitalWrite(blueLed, LOW);
	// 	digitalWrite(redLed, LOW);
	// 	delay(100);
	// } /*else {
	// 	// digitalWrite(greenLed, LOW);
	// 	digitalWrite(redLed, HIGH);
	// 	digitalWrite(blueLed, HIGH);
	// }*/



	slider_read();
	
}

void slider_read() {
	int state = analogRead(slider) / 4;
	
	// Serial.println(state);

	if (state > 253 && state < 256 && previousState != 'b') {
		// Serial.println("1");
		Keyboard.write('b');
		previousState = 'b';
		delay(50);

	} else if (state > 251 && state < 253 && previousState != 'n') {
		// Serial.println("2");
		Keyboard.write('n');
		previousState = 'n';
		delay(50);

	} else if (state > 226 && state < 230 && previousState != 'm') {
		// Serial.println("3");
		Keyboard.write('m');
		previousState = 'm';
		delay(50);

	} else {
	}  	

}



