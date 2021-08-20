/*
 *  Project     Rocket League XInput Demo
 *  @author     David Madison
 *  @link       partsnotincluded.com/tutorials/how-to-emulate-an-xbox-controller-with-arduino-xinput
 *  @license    MIT - Copyright (c) 2019 David Madison
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
 
#include <XInput.h>

// Setup for button matrix input pins
byte rows[] = {8,9,10,11};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

byte cols[] = {2,3,4};
const int colCount = sizeof(cols)/sizeof(cols[0]);

byte keys[colCount][rowCount];

// setup constants for each button
// these are not correct or tested, just filled in for later
const uint8_t buttonA[]     = {0,0};
const uint8_t buttonB[]     = {1,1};
const uint8_t buttonX[]     = {2,2};
const uint8_t buttonY[]     = {0,3};
const uint8_t buttonUp[]    = {1,0};
const uint8_t buttonDown[]  = {2,1};
const uint8_t buttonLeft[]  = {0,2};
const uint8_t buttonRight[] = {1,3};
const uint8_t buttonL2[]    = {2,0};
const uint8_t buttonR2[]    = {0,1};

// Setup for button non-matrix input pins
const uint8_t PinTriggerLeft = A0;
const uint8_t PinTriggerRight = A1;
const uint8_t PinWheel = A2;

//const uint8_t Pin_ButtonA    = 2;   // Jump
const uint8_t Pin_ButtonStart  = 5;
const uint8_t Pin_ButtonBack = 6;
const uint8_t Pin_ButtonR3     = 7;
const uint8_t Pin_ButtonL3     = 12;

// 13
 
// Output Pins
const uint8_t Pin_LED = LED_BUILTIN;
 
// Analog Input Range
const int AnalogRead_Max = 1023;  // 10-bit ADC
 
void setup() {
  // button matrix input pin modes
  for(int x=0; x<rowCount; x++) {
    pinMode(rows[x], INPUT);
  }
 
  for (int x=0; x<colCount; x++) {
    //pinMode(cols[x], INPUT_PULLUP);
    pinMode(cols[x], INPUT);
  }  

  // non-matrix button input pin modes
  // pinMode(Pin_ButtonA, INPUT_PULLUP);
  pinMode(Pin_ButtonStart, INPUT_PULLUP);
  pinMode(Pin_ButtonBack, INPUT_PULLUP);
 
  // Set output pin mode
  pinMode(Pin_LED, OUTPUT);
  digitalWrite(Pin_LED, LOW);  // Turn 'off'
 
  // Setup library
  XInput.setRange(JOY_LEFT, 0, AnalogRead_Max);
  XInput.setTriggerRange(0, AnalogRead_Max);
  XInput.begin();
}
 
void readMatrix() {
    // iterate the columns
    for (int colIndex=0; colIndex < colCount; colIndex++) {
        // col: set to output to low
        byte curCol = cols[colIndex];
        pinMode(curCol, OUTPUT);
        digitalWrite(curCol, LOW);
 
        // row: interate through the rows
        for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
            byte rowCol = rows[rowIndex];
            pinMode(rowCol, INPUT_PULLUP);
            keys[colIndex][rowIndex] = digitalRead(rowCol);
            pinMode(rowCol, INPUT);
        }
        // disable the column
        pinMode(curCol, INPUT);
    }
} 
 
void setMatrixButtons() {
  XInput.setButton(BUTTON_A,     !keys[buttonA[0]]    [buttonA[1]]);
  XInput.setButton(BUTTON_B,     !keys[buttonB[0]]    [buttonB[1]]);
  XInput.setButton(BUTTON_X,     !keys[buttonX[0]]    [buttonX[1]]);
  XInput.setButton(BUTTON_Y,     !keys[buttonY[0]]    [buttonY[1]]);
  XInput.setButton(BUTTON_LB,    !keys[buttonL2[0]]   [buttonL2[1]]);
  XInput.setButton(BUTTON_RB,    !keys[buttonR2[0]]   [buttonR2[1]]);  
  
  XInput.setDpad(!keys[buttonUp[0]][buttonUp[1]],!keys[buttonDown[0]][buttonDown[1]], !keys[buttonLeft[0]][buttonLeft[1]], !keys[buttonRight[0]][buttonRight[1]]);
}
 
void loop() {
  // Read non-matrix pin states
  boolean pressStart = !digitalRead(Pin_ButtonStart);
  boolean pressBack = !digitalRead(Pin_ButtonBack);
  boolean pressL3 = !digitalRead(Pin_ButtonL3);
  boolean pressR3 = !digitalRead(Pin_ButtonR3);
  
  int ValueTriggerLeft = analogRead(PinTriggerLeft);
  int ValueTriggerRight = analogRead(PinTriggerRight);
  int ValueWheel = analogRead(PinWheel);
  
  // Set mon-matrix button and trigger states
  XInput.setButton(BUTTON_START, pressStart);
  XInput.setButton(BUTTON_BACK, pressBack);
  XInput.setButton(BUTTON_L3, pressL3);
  XInput.setButton(BUTTON_R3, pressR3);

  XInput.setJoystick(JOY_LEFT, ValueWheel, AnalogRead_Max / 2);  // move x, leave y centered
  XInput.setTrigger(TRIGGER_LEFT, ValueTriggerLeft);
  XInput.setTrigger(TRIGGER_RIGHT, ValueTriggerRight);

  // Read matrix pin states
  readMatrix();
  
  // Set matrix buttons states
  setMatrixButtons();
  
  // Get rumble value
  uint16_t rumble = XInput.getRumble();
 
  // If controller is rumbling, turn on LED
  if (rumble > 0) {
    digitalWrite(Pin_LED, HIGH);
  }
  else {
    digitalWrite(Pin_LED, LOW);
  }
}
