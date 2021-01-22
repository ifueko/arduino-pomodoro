//ABCDEFG,dp
// Variables for specific numbers/letters
const int numeral[10] = {
  B11111100, //0
  B01100000, //1
  B11011010, //2
  B11110010, //3
  B01100110, //4
  B10110110, //5
  B00111110, //6
  B11100000, //7
  B11111110, //8
  B11100110, //9
};
const int done[4] = {
  B01111010, // d
  B00111010, // o
  B00101010, // n
  B10011110, // e
};
const int go[3] = {
  B00001010, // r
  B00111000, // u
  B00101010, // n
};

const int clear_ = B00000000;

//pins for decimal point and each segment
//dp, G, F, E, D, C, B, A
const int segmentPins[] = { 4, 7, 8, 6, 5, 3, 2, 9};
const int numberofDigits = 4;
const int digitPins[numberofDigits] = { 10, 11, 12, 13}; //digits 1, 2, 3, 4

// Set up Segment pins as digital in/out
void setup() {
  for (int i = 0; i < 8; i++)
    pinMode(segmentPins[i], OUTPUT); //set segment and DP pins to output

  //sets the digit pins as outputs
  for (int i = 0; i < numberofDigits; i++)
    pinMode(digitPins[i], OUTPUT);
}

// Convert seconds to MMSS integer
int secs_to_mmss(int secs) {
  int mins = (secs - (secs % 60)) / 60;
  secs = secs % 60;
  int disp = mins * 100 + secs;
  return disp;
}

// Global Variables
int value = 5;
int diff = -1;
unsigned long startTime = millis();
int mode = 1; // work = 0, rest = 1
const int MODE_0_VAL = 900;
const int MODE_1_VAL = 300;
const int loop_delay = 2;
const int loop_count_go = 5;
const int loop_count_done = 10;
const int blink_count_go = 20;
const int blink_count_done = 5;

// Pomodoro Loop
void loop() {
  if (diff < 0) {
    diff = 0;
    if (mode == 1) {
      goAlert();
      mode = 0;
      value = MODE_0_VAL;
      startTime = millis();
    } else if (mode == 0) {
      doneAlert();
      mode = 1;
      value = MODE_1_VAL;
      startTime = millis();
    }
  }
  unsigned long curr_time = millis();
  diff = value - int((curr_time - startTime) / 1000);
  showNumber(secs_to_mmss(diff));
  delay(5);
}

// Show number (Time)
void showNumber (int number)
{
  if (number == 0) showDigit (0, numberofDigits - 1); //display 0 in the rightmost digit
  else {
    for (int digit = numberofDigits - 1; digit >= 0; digit--) {
      if (number > 0) {
        showSingleNumber(number % 10, digit);
        number = number / 10;
      }
    }
  }
}

// Alert at end of each cycle
void goAlert() {
  for (int i = 0; i < blink_count_go; i++) {
    for (int i = 0; i < loop_count_go; i++) {
      showGo();
      delay(loop_delay);
    }
    for (int i = 0; i < loop_count_go; i++) {
      clear_7();
      delay(loop_delay);
    }
  }
}

void doneAlert() {
  for (int i = 0; i < blink_count_done; i++) {
    for (int i = 0; i < loop_count_done; i++) {
      showDone();
      delay(loop_delay);
    }
    for (int i = 0; i < loop_count_done; i++) {
      clear_7();
      delay(loop_delay);
    }
  }
}


// Show + Clear 
void showDone () {
  for (int i = 0; i < 4; i ++)
    showDoneLetter(i, i);
}

void showGo () {
  for (int i = 0; i < 3; i ++)
    showGoLetter(i, i+1);
}

void clear_7 () {
  for (int i = 0; i < 4; i++)
    clearDigit(i);
}

void clearDigit(int digit) {
  showDigit(clear_, digit);
}


// Show Letters and Numbers
void showGoLetter (int letter, int digit) {
  showDigit(go[letter], digit);
}

void showDoneLetter (int letter, int digit) {
  showDigit(done[letter], digit);
}

void showSingleNumber (int number, int digit) {
  showDigit(numeral[number], digit);
}

// Show Digit Pins Base
void showDigit(int number, int digit) 
{
  digitalWrite(digitPins[digit], HIGH);
  for (int segment = 1; segment < 8; segment++)
  {
    boolean isBitSet = bitRead(number, segment);
    isBitSet = ! isBitSet; //remove this line if common cathode display
    digitalWrite(segmentPins[segment], isBitSet);
  }
  delay(5);
  digitalWrite(digitPins[digit], LOW);
}
