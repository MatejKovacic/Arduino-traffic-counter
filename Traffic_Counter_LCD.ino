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

GND <--- 10k Ohm ---> D10
                 |
                 ---reed switch --- > 5V

10k Ohm = brown - black - orange - gold


2. Buzzer:

D14 (A0) --- 220 Ohm --- buzzer --- GND

220 Ohm = red - red - brown - gold

(You can use the analog pins as digital pins: A0 to A5 are D14 to D19.)

3. LED diode:

D1/D12 -- LED (green and red) -- 300 Ohm -- GND

300 Ohm = orange - black - brown - gold

4. LCD display:

     [LCD - Arduino Nano]
     1. VSS Arduino GND
     2. VDD - Arduino 5V
     3. V0 (contrast) - 10k Ohm potentiometer (connect one wire to center pin and the other to left/right pin) - Arduino GND
     4. RS - Arduino D9
     5. RW - Arduino D8
     6. E (enable) - Arduino D7
     7. D0 (data 0) - no connection   
     8. D1 - no connection   
     9. D2 - no connection   
     10. D3 - no connection  
     11. D4 - Arduino D5
     12. D5 - Arduino D4
     13. D6 - Arduino D3
     14. D7 - Arduino D2
     15. A (backlight +) - 220 Ohm resistor (40 Ohm min.) - Arduino D6
     16. K (backlight GND) - GND Arduino pin*

    If there is no text displayed, LCD module may be out of sync with Arduino.
    In that case press the reset button on your Arduino to synchronize (it may take a couple of resets).

    Source: https://www.hacktronics.com/Tutorials/arduino-character-lcd-tutorial.html
*/

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

const int buzzer = 14;       // buzzer is connected to D14 (A0)

const int switchPin = 10;    // reed switch or button switch is connected to D10

const int ledOnboard = 13;   // onboard LED diode pin is D13
const int ledGreen = 11;     // green LED diode is connected to D11
const int ledRed = 12;       // red LED diode is connected to D12

LiquidCrystal lcd(9, 8, 7, 5, 4, 3, 2);
int backLight = 6;    // Arduino pin D6 will control the backlight


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
  
  Serial.println("Stevec prometa");  // print text to serial console
  
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH);  // Turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.
  lcd.begin(16,2);                // Columns and rows.  use 16,2 for a 16x2 LCD.
  lcd.clear();                    // Start with a blank screen...
  lcd.setCursor(0,0);             // Set cursor to column 0, row 0 (the beginning of the first line).
  lcd.print("Stevec prometa");    // Text displayed in a first line.
  lcd.setCursor(0,1);             // Set cursor to column 0, row 1 (the beginning of the second line).
  lcd.print("St. vozil: ");       // Text displayed in a second line.
  lcd.print(switchCounter);       // Text displayed in a second line.  
}

void loop() {
  // Read the state of the switch (1 = HIGH, 0 = LOW):
  switchState = digitalRead(switchPin);

  // Check if we need to increase switch counter:
  if (switchState == HIGH) {                 // if switch is on...
    if (switchState != prevSwitchState) {    // ...check if switch state has changed...
      switchCounter = switchCounter + 1;     // ...and then increase the counter

      // Clear the whole of line 1
      lcd.setCursor (0, 1);
      for (int i = 0; i < 16; ++i)
      {
        lcd.write(' ');
      }
      lcd.setCursor(0,1);
      lcd.print("St. vozil: ");
      lcd.print(switchCounter);       // displayed traffic counter on LCD     
    }
  }
  prevSwitchState = switchState;        // set new previous switch state
  
  // Print data to serial console:
  Serial.print("Stanje stikala: ");
  Serial.print(switchState);           // print switch status (1 or 0)
  Serial.print(", število vozil: ");
  Serial.println(switchCounter);       // print switch counter (i. e. traffic counter)
        
  // Check if the switch is on (i. e. switchState is HIGH):
  if (switchState == HIGH) {
    digitalWrite(ledOnboard, HIGH);  // turn onboard LED on
    digitalWrite(ledRed, HIGH);      // turn red LED on
    digitalWrite(ledGreen, LOW);     // turn green LED off
    tone(buzzer, 5000);              // beep with 1kHz sound signal
    
  // Else - switch is off (i. e. switchState is LOW):
  } else {
    digitalWrite(ledOnboard, LOW);   // turn onboard LED off
    digitalWrite(ledRed, LOW);       // turn red LED off
    digitalWrite(ledGreen, HIGH);    // turn green LED on
    noTone(buzzer);                  // stop sound
  }
}
