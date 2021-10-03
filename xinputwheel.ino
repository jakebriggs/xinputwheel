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
// byte rows[] = {8,9,10,11};
byte rows[] = {3,4,5,6};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

// byte cols[] = {2,3,4};
byte cols[] = {0,1,2};
const int colCount = sizeof(cols)/sizeof(cols[0]);

byte keys[colCount][rowCount];

// setup constants for each button
const uint8_t buttonA[]     = {1,1};
const uint8_t buttonB[]     = {1,0};
const uint8_t buttonX[]     = {1,2};
const uint8_t buttonY[]     = {1,3};
const uint8_t buttonUp[]    = {2,0};
const uint8_t buttonDown[]  = {2,1};
const uint8_t buttonLeft[]  = {2,3};
const uint8_t buttonRight[] = {2,2};
const uint8_t buttonLB[]    = {0,3}; // L1
const uint8_t buttonRB[]    = {0,0}; // R1
const uint8_t buttonL2[]    = {0,2}; // L2
const uint8_t buttonR2[]    = {0,1}; // R2

// Setup for button non-matrix input pins
const uint8_t PinTriggerLeft = A1;
const uint8_t PinTriggerRight = A2;
const uint8_t PinWheel = A0;

const uint8_t Pin_ButtonBack  = 7; // labeled "Select"
const uint8_t Pin_ButtonLogo  = 8; // labeled "Mode"
const uint8_t Pin_ButtonStart = 9;
const uint8_t Pin_ButtonL3    = 10;
const uint8_t Pin_ButtonR3    = 11;

// Output Pins
const uint8_t Pin_LED = LED_BUILTIN;
 
// Analog Input Range
const int LeftAnalogRead_Min = 0;  // 10-bit ADC
const int LeftAnalogRead_Max = 1023;  // 10-bit ADC
const int LeftTriggerRead_Min = 0;  // 10-bit ADC
const int LeftTriggerRead_Max = 1023;  // 10-bit ADC
const int RightTriggerRead_Min = 0;  // 10-bit ADC
const int RightTriggerRead_Max = 1023;  // 10-bit ADC
 
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
  pinMode(Pin_ButtonLogo, INPUT_PULLUP);
  pinMode(Pin_ButtonStart, INPUT_PULLUP);
  pinMode(Pin_ButtonBack, INPUT_PULLUP);
  pinMode(Pin_ButtonL3, INPUT_PULLUP);
  pinMode(Pin_ButtonR3, INPUT_PULLUP);

  // Set output pin mode
  pinMode(Pin_LED, OUTPUT);
  digitalWrite(Pin_LED, LOW);  // Turn 'off'
 
  // Setup library
  XInput.setRange(JOY_LEFT, LeftAnalogRead_Min, LeftAnalogRead_Max);
  // XInput.setTriggerRange(0, LeftAnalogRead_Max);

  XInput.setRange(TRIGGER_LEFT, LeftTriggerRead_Min, LeftTriggerRead_Max);
  XInput.setRange(TRIGGER_RIGHT, RightTriggerRead_Min, RightTriggerRead_Max);

  
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
  XInput.setButton(BUTTON_LB,    !keys[buttonLB[0]]   [buttonLB[1]]);
  XInput.setButton(BUTTON_RB,    !keys[buttonRB[0]]   [buttonRB[1]]);
  // maybe these two next buttons should be triggers because L2 and R2 dont really mnatch anything else  
  XInput.setButton(BUTTON_LB,    !keys[buttonL2[0]]   [buttonL2[1]]);
  XInput.setButton(BUTTON_RB,    !keys[buttonR2[0]]   [buttonR2[1]]);
    
  XInput.setDpad(!keys[buttonUp[0]][buttonUp[1]],!keys[buttonDown[0]][buttonDown[1]], !keys[buttonLeft[0]][buttonLeft[1]], !keys[buttonRight[0]][buttonRight[1]]);
}
 
void loop() {
  // Read non-matrix pin states
  boolean pressLogo = !digitalRead(Pin_ButtonLogo);
  boolean pressStart = !digitalRead(Pin_ButtonStart);
  boolean pressBack = !digitalRead(Pin_ButtonBack);
  boolean pressL3 = !digitalRead(Pin_ButtonL3);
  boolean pressR3 = !digitalRead(Pin_ButtonR3);
  
  int ValueTriggerLeft = analogRead(PinTriggerLeft);
  int ValueTriggerRight = analogRead(PinTriggerRight);
  int ValueWheel = analogRead(PinWheel);
  
  // Set mon-matrix button and trigger states
  XInput.setButton(BUTTON_LOGO, pressLogo);
  XInput.setButton(BUTTON_START, pressStart);
  XInput.setButton(BUTTON_BACK, pressBack);
  XInput.setButton(BUTTON_L3, pressL3);
  XInput.setButton(BUTTON_R3, pressR3);

  XInput.setJoystick(JOY_LEFT, ValueWheel, LeftAnalogRead_Max / 2);  // move x, leave y centered
  XInput.setJoystick(JOY_RIGHT, LeftAnalogRead_Max / 2 , LeftAnalogRead_Max / 2);  // leave x and y centered

  // invert the right pedal
  XInput.setTrigger(TRIGGER_LEFT, LeftTriggerRead_Max - ValueTriggerLeft + LeftTriggerRead_Min);
  XInput.setTrigger(TRIGGER_RIGHT, RightTriggerRead_Max - ValueTriggerRight + RightTriggerRead_Min);

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
