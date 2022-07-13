/*
 * Ultrasonic Simple
 * Prints the distance read by an ultrasonic sensor in
 * centimeters. They are supported to four pins ultrasound
 * sensors (liek HC-SC04) and three pins (like PING)))
 * and Seeed Studio sensors).
 *
 * The circuit:
 * * Module HR-SC04 (four pins) or PING))) (and other with
 *   three pins), attached to digital pins as follows:
 * ---------------------    --------------------
 * | HC-SC04 | Arduino |    | 3 pins | Arduino |
 * ---------------------    --------------------
 * |   Vcc   |   5V    |    |   Vcc  |   5V    |
 * |   Trig  |   12    | OR |   SIG  |   13    |
 * |   Echo  |   13    |    |   Gnd  |   GND   |
 * |   Gnd   |   GND   |    --------------------
 * ---------------------
 * Note: You do not obligatorily need to use the pins defined above
 * 
 * By default, the distance returned by the read()
 * method is in centimeters. To get the distance in inches,
 * pass INC as a parameter.
 * Example: ultrasonic.read(INC)
 *
 * created 3 Apr 2014
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 * modified 23 Jan 2017
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 * modified 03 Mar 2017
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 * modified 11 Jun 2018
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 *
 * This example code is released into the MIT License.
 */

/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad.h>

#include <Ultrasonic.h>

/*
 * Pass as a parameter the trigger and echo pin, respectively,
 * or only the signal pin (for sensors 3 pins), like:
 * Ultrasonic ultrasonic(13);
 */

//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Ultrasonic ultrasonic(12, 13);
int distance;


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'-','9','5','1'},
  {'*','0','6','2'},
  {'/','A','7','3'},
  {'#','B','8','4'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 15000;           // interval at which to blink (milliseconds)

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Hello, world");
  Serial.begin(9600);

}

char PWD[8];
int ctr=0; //counter
int mltplr = 1;
char A_PWD[8] = {'0','1','2','3','4','5','6','7'};


void loop() {
  // Pass INC as a parameter to get the distance in inches
  distance = ultrasonic.read();
  char key = keypad.getKey();
  char keyprev = 'F';
  //else
    //lcd.clear();
  //Serial.print("Distance in CM: ");
  //Serial.println(distance);
  //delay(100);
  if(distance < 200){
    if(previousMillis == 0){
      previousMillis = millis();
    }
    if(key){
      if(ctr > 8){
        ctr = 0;
      }
      Serial.print(key);
      //lcd.clear();
      if (key != '#' && key != '-'){
        if(keyprev == 'F'){
          lcd.clear();
        }
        keyprev = key;
        PWD[ctr] = key;
        lcd.setCursor(0,0);
        for (int j=0; j<= ctr; ++j){
          lcd.print("*");      
        }
        ctr++; 
      }
      //print(key);
      if(key == '-'){
        ctr = 0;
        mltplr = 1;
        lcd.clear();  
        lcd.setCursor(0,0);
        keyprev = 'F';
        previousMillis = 0;
      }
      else if(key == '#'){
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          lcd.clear();
          lcd.setCursor(4,0);
          lcd.print("You took");
          lcd.setCursor(0,1);
          lcd.print("too long. SUS!");
          ctr = 0;
          mltplr = 1;
          keyprev = 'F';
          previousMillis = 0;
        }
        else{
          mltplr = 1;
          Serial.println(ctr);
          if(ctr != 8){
            mltplr = 0;
          }
          else{ 
            mltplr = 1;
            for(int ctr2=0; ctr2 < ctr ; ctr2++){
               if( PWD[ctr2] == A_PWD[ctr2]){
                 mltplr *= 1;
               }
               else mltplr=0 ;
               //lcd.setCursor(ctr2,0);
               //lcd.print("*");
               Serial.print(mltplr);
               Serial.print(" ");
               Serial.print(PWD[ctr2]);
               Serial.print(" ");
               Serial.println(A_PWD[ctr2]);
            }
          }
          
          if (mltplr != 0){
            lcd.clear();
            lcd.setCursor(4,0);
            lcd.print("Password");
            lcd.setCursor(4,1);
            lcd.print("Correct!");
            keyprev = 'F';
            //delay(3000);
          }
          else {
            lcd.clear();
            lcd.setCursor(1,0);
            lcd.print("Wrong Password");
            ctr = 0;
            mltplr = 1;
            keyprev = 'F';
            //delay(3000);
          }
          previousMillis = 0;
        }
      }
    }
  }
  /* if(distance < 100 && keyprev == 'F'){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("1234567890123456");
    lcd.setCursor(0,1);
    lcd.print("A741B852C963-*#0");
    delay(200);
  }
  */
}
