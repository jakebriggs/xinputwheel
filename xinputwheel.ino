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
 
// Input Pins
const uint8_t PinTriggerLeft = A0;
const uint8_t PinTriggerRight = A1;

const uint8_t Pin_ButtonA  = 2;   // Jump
const uint8_t Pin_TriggerR = 3;   // Accelerate
 
// Output Pins
const uint8_t Pin_LED = LED_BUILTIN;
 
// Analog Input Range
const int AnalogRead_Max = 1023;  // 10-bit ADC
 
void setup() {
  // Set input pin modes
  pinMode(Pin_ButtonA, INPUT_PULLUP);
  pinMode(Pin_TriggerR, INPUT_PULLUP);
 
  // Set output pin mode
  pinMode(Pin_LED, OUTPUT);
  digitalWrite(Pin_LED, LOW);  // Turn 'off'
 
  // Setup library
  //XInput.setRange(JOY_LEFT, 0, AnalogRead_Max);
  XInput.setTriggerRange(0, AnalogRead_Max);
  XInput.begin();
}
 
void loop() {
  // Read pin states
  boolean pressA = !digitalRead(Pin_ButtonA);
  boolean pressTrigger = !digitalRead(Pin_TriggerR);
  int ValueTriggerLeft = analogRead(PinTriggerLeft);
  int ValueTriggerRight = analogRead(PinTriggerRight);
  
  // Set button and trigger states
  XInput.setButton(BUTTON_A, pressA);
  XInput.setButton(TRIGGER_RIGHT, pressTrigger);
//  XInput.setJoystick(JOY_LEFT, ValueTriggerLeft, AnalogRead_Max / 2);  // move x, leave y centered
  XInput.setTrigger(TRIGGER_LEFT, ValueTriggerLeft);
  XInput.setTrigger(TRIGGER_RIGHT, ValueTriggerRight);

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
