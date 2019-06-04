#ifndef CONSOLE
#define CONSOLE
bool sequenceOneComplete = false;
bool sequenceTwoComplete = false;
bool sequenceThreeComplete = false;
int seqPin1 = 0;
int seqPin2 = 0;
int seqPin3 = 0;
int seq1State = 0;
int seq2State = 0;
int seq3State = 0;

typedef enum Step {
	STEP_ONE,
	STEP_TWO,
	STEP_THREE
};

int const D1 = A0;
int const D2 = A1;
int const D3 = A2;
int const E1 = A3;
int const E2 = A4;
int const E3 = A5;

int const F1 = 2;
int const F2 = 3;
int const F3 = 4;
int const F4 = 5;
int const F5 = 6;
int const G1 = 7;
int const G2 = 8;
int const G3 = 9;
int const G4 = 10;
int const G5 = 11;





int potVal = 0;
bool repair = false;
char type1;
char type2;
char type3;

// the reset functuion call and will reset board
void(* resetFunc) (void) = 0;
#endif