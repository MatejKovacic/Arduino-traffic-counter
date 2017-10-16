/* Arduino traffic counter
 *
 *  (CC) Matej Kovačič
 *  
 *  Arduino firmware for reading reed switch state and counting traffic.
 *  Magnet is mounted on a toy car. Reed switch is mounted inside the road.
 *  When car with magnet passes the switch, switch is activated.
 *  Green LED is turned off, red LED and onboard LED is turned on and beeper starts beeping.
 *  Traffic counter is incfreased and printed to serial console.

Wiring
======

1. Reed switch:

- 10k Ohm pull-up resistor in connected to GND and D5
- reed switch is connected to D5 and 5V

GND <--- 10k Ohm ---> D5
                 |
                 ---reed switch --- > 5V

10k Ohm = brown - black - orange - gold


2. Buzzer:

D9 --- 220 Ohm --- buzzer --- GND

220 Ohm = red - red - brown - gold


3. LED diode:

D3/D2 -- LED (green or red) -- 300 Ohm -- GND

300 Ohm = orange - black - brown - gold

*/

#include <SoftwareSerial.h>

const int buzzer = 9;        // buzzer is connected to D9

const int switchPin = 5;     // reed switch or button switch is connected to D5

const int ledOnboard = 13;   // onboard LED diode pin is D13
const int ledGreen = 3;      // green LED diode is connected to D3
const int ledRed = 2;        // red LED diode is connected to D2

int switchState = 0;         // switch status
int prevSwitchState = 0;     // previous switch status
int switchCounter = 0;       // switch counter

void setup() {
  pinMode(buzzer, OUTPUT);        // Set buzzer's pin 9 as an output
  pinMode(ledOnboard, OUTPUT);    // initialize onboard LED pin as an output
  pinMode(ledGreen, OUTPUT);      // initialize green LED pin as an output
  pinMode(ledRed, OUTPUT);        // initialize red LED pin as an output

  pinMode(switchPin, INPUT);      // initialize switch pin as an input

  Serial.begin(9600);             // initialize serial port (9600 bps)
  
  Serial.println("Magnetno stikalo - demo.");  // print text to serial console
}

void loop() {
  // Read the state of the switch (1 = HIGH, 0 = LOW):
  switchState = digitalRead(switchPin);

  // Check if we need to increase switch counter:
  if (switchState == HIGH) {                 // if switch is on...
    if (switchState != prevSwitchState) {    // ...check if switch state has changed...
      switchCounter = switchCounter + 1;     // ...and then increase the counter
    }
  }
  prevSwitchState = switchState;        // set new previous switch state
  
  // Print data to serial console:
  Serial.print("Stanje stikala: ");
  Serial.print(switchState);           // print switch status (1 or 0)
  Serial.print(", števec prometa: ");
  Serial.println(switchCounter);       // print switch counter (i. e. traffic counter)
    
  // Check if the switch is on (i. e. switchState is HIGH):
  if (switchState == HIGH) {
    digitalWrite(ledOnboard, HIGH);  // turn onboard LED on
    digitalWrite(ledRed, HIGH);      // turn red LED on
    digitalWrite(ledGreen, LOW);     // turn green LED off
    tone(buzzer, 1000);              // beep with 1kHz sound signal
  // Else - switch is off (i. e. switchState is LOW):
  } else {
    digitalWrite(ledOnboard, LOW);   // turn onboard LED off
    digitalWrite(ledRed, LOW);       // turn red LED off
    digitalWrite(ledGreen, HIGH);    // turn green LED on
    noTone(buzzer);                  // stop sound
  }
}
