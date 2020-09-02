
// Include Libraries
#include "Arduino.h"
#include "BTHC05.h"
#include "DCMDriverL298.h"
#include "NewPing.h"
#include "Adafruit_NeoPixel.h"
#include "Servo.h"
#include "SpeakerStereo3W.h"
#include "Switchable.h"
#include "VibrationMotor.h"


// Pin Definitions
#define BTHC05_PIN_TXD	11
#define BTHC05_PIN_RXD	10
#define CAPACITIVETOUCH_PIN_SIG	2
#define DCMOTORDRIVERL298_PIN_INT1	4
#define DCMOTORDRIVERL298_PIN_ENB	5
#define DCMOTORDRIVERL298_PIN_INT2	7
#define DCMOTORDRIVERL298_PIN_ENA	3
#define DCMOTORDRIVERL298_PIN_INT3	8
#define DCMOTORDRIVERL298_PIN_INT4	9
#define HCSR04_PIN_TRIG	13
#define HCSR04_PIN_ECHO	12
#define LEDRGB_PIN_DIN	A0
#define SERVO360_PIN_SIG	A1
#define STEREOSPEAKER_PIN_POS	A2
#define VIBRATIONMOTOR_PIN_COIL1	6



// Global variables and defines
#define LedRGB_NUMOFLEDS 1
unsigned int StereoSpeakerHoorayLength          = 6;                                                      // amount of notes in melody
unsigned int StereoSpeakerHoorayMelody[]        = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5}; // list of notes. List length must match HoorayLength!
unsigned int StereoSpeakerHoorayNoteDurations[] = {8      , 8      , 8      , 4      , 8      , 4      }; // note durations; 4 = quarter note, 8 = eighth note, etc. List length must match HoorayLength!
// object initialization
BTHC05 bthc05(BTHC05_PIN_RXD,BTHC05_PIN_TXD);
DCMDriverL298 dcMotorDriverL298(DCMOTORDRIVERL298_PIN_ENA,DCMOTORDRIVERL298_PIN_INT1,DCMOTORDRIVERL298_PIN_INT2,DCMOTORDRIVERL298_PIN_ENB,DCMOTORDRIVERL298_PIN_INT3,DCMOTORDRIVERL298_PIN_INT4);
NewPing hcsr04(HCSR04_PIN_TRIG,HCSR04_PIN_ECHO);
Adafruit_NeoPixel LedRGB(LEDRGB_PIN_DIN);
Servo servo360;
SpeakerStereo3W StereoSpeaker(STEREOSPEAKER_PIN_POS);
VibrationMotor vibrationMotor(VIBRATIONMOTOR_PIN_COIL1);


// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    bthc05.begin(9600);
    //This example uses HC-05 Bluetooth to communicate with an Android device.
    //Download bluetooth terminal from google play store, https://play.google.com/store/apps/details?id=Qwerty.BluetoothTerminal&hl=en
    //Pair and connect to 'HC-05', the default password for connection is '1234'.
    //You should see this message from your arduino on your android device
    bthc05.println("Bluetooth On....");
    pinMode(CAPACITIVETOUCH_PIN_SIG, INPUT);
    LedRGB.begin(); // This initializes the NeoPixel library.
    LedRGB.show(); // Initialize all leds to 'off'
    menuOption = menu();
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    
    if(menuOption == '1') {
    // HC - 05 Bluetooth Serial Module - Test Code
    String bthc05Str = "";
    //Receive String from bluetooth device
    if (bthc05.available())
    {
    //Read a complete line from bluetooth terminal
    bthc05Str = bthc05.readStringUntil('\n');
    // Print raw data to serial monitor
    Serial.print("BT Raw Data: ");
    Serial.println(bthc05Str);
    }
    //Send sensor data to Bluetooth device  
    bthc05.println("PUT YOUR SENSOR DATA HERE");
    }
    else if(menuOption == '2') {
    // Capacitive Touch Sensor Module - Test Code
    //Read Capacitive Touch Sensor. CapacitiveTouchVar will be '1' if the sensor will be touched
    bool CapacitiveTouchVar = digitalRead(CAPACITIVETOUCH_PIN_SIG);
    Serial.print(F("Capacitive Touch Sensor: ")); Serial.println(CapacitiveTouchVar);

    }
    else if(menuOption == '3') {
    // L298N Motor Driver with Dual Hobby DC motors - Test Code
    //Start both motors. note that rotation direction is determined by the motors connection to the driver.
    //You can change the speed by setting a value between 0-255, and set the direction by changing between 1 and 0.
    dcMotorDriverL298.setMotorA(200,1);
    dcMotorDriverL298.setMotorB(200,0);
    delay(2000);
    //Stop both motors
    dcMotorDriverL298.stopMotors();
    delay(2000);

    }
    else if(menuOption == '4') {
    // Ultrasonic Sensor - HC-SR04 - Test Code
    // Read distance measurment from UltraSonic sensor           
    int hcsr04Dist = hcsr04.ping_cm();
    delay(10);
    Serial.print(F("Distance: ")); Serial.print(hcsr04Dist); Serial.println(F("[cm]"));

    }
    else if(menuOption == '5') {
    // LED - RGB Addressable, PTH, 5mm Diffused (5 Pack) - Test Code
    for(int i=0 ; i <= LedRGB_NUMOFLEDS ; i++){
    for (int k = 0 ; k <= 255 ; k++) {
    // set leds Color to RGB values, from 0,0,0 up to 255,255,255
    LedRGB.setPixelColor(i, LedRGB.Color(255-k,k,100)); // turn on green color on led #i.
    if (i > 0)
    LedRGB.setPixelColor(i-1, LedRGB.Color(0,0,0)); // turn off last led
    LedRGB.show(); //update led color to the hardware.
    delay(1);
    }
    }

    }
    else if(menuOption == '6') {
    // Servo - Generic Continuous Rotation (Micro Size) - Test Code
    // The servo will rotate CW in full speed, CCW in full speed, and will stop  with an interval of 2000 milliseconds (2 seconds) 
    servo360.attach(SERVO360_PIN_SIG);         // 1. attach the servo to correct pin to control it.
    servo360.write(180);  // 2. turns servo CW in full speed. change the value in the brackets (180) to change the speed. As these numbers move closer to 90, the servo will move slower in that direction.
    delay(2000);                              // 3. waits 2000 milliseconds (2 sec). change the value in the brackets (2000) for a longer or shorter delay in milliseconds.
    servo360.write(0);    // 4. turns servo CCW in full speed. change the value in the brackets (0) to change the speed. As these numbers move closer to 90, the servo will move slower in that direction.
    delay(2000);                              // 5. waits 2000 milliseconds (2 sec). change the value in the brackets (2000) for a longer or shorter delay in milliseconds.
    servo360.write(90);    // 6. sending 90 stops the servo 
    delay(2000);                              // 7. waits 2000 milliseconds (2 sec). change the value in the brackets (2000) for a longer or shorter delay in milliseconds.
    servo360.detach();                    // 8. release the servo to conserve power. When detached the servo will NOT hold it's position under stress.
    }
    else if(menuOption == '7') {
    // 3W Stereo Speaker - Test Code
    // The Speaker will play the Hooray tune
    StereoSpeaker.playMelody(StereoSpeakerHoorayLength, StereoSpeakerHoorayMelody, StereoSpeakerHoorayNoteDurations); 
    delay(500);   
    }
    else if(menuOption == '8') {
    // Vibration Motor - Test Code
    // The vibration motor will turn on and off for 500ms (0.5 sec)
    vibrationMotor.on();     // 1. turns on
    delay(500);           // 2. waits 500ms (0.5 sec). change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    vibrationMotor.off();    // 3. turns off
    delay(500);           // 4. waits 500ms (0.5 sec). change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    }
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) HC - 05 Bluetooth Serial Module"));
    Serial.println(F("(2) Capacitive Touch Sensor Module"));
    Serial.println(F("(3) L298N Motor Driver with Dual Hobby DC motors"));
    Serial.println(F("(4) Ultrasonic Sensor - HC-SR04"));
    Serial.println(F("(5) LED - RGB Addressable, PTH, 5mm Diffused (5 Pack)"));
    Serial.println(F("(6) Servo - Generic Continuous Rotation (Micro Size)"));
    Serial.println(F("(7) 3W Stereo Speaker"));
    Serial.println(F("(8) Vibration Motor"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {   
            
            if(c == '1') 
    			Serial.println(F("Now Testing HC - 05 Bluetooth Serial Module"));
    		else if(c == '2') 
    			Serial.println(F("Now Testing Capacitive Touch Sensor Module"));
    		else if(c == '3') 
    			Serial.println(F("Now Testing L298N Motor Driver with Dual Hobby DC motors"));
    		else if(c == '4') 
    			Serial.println(F("Now Testing Ultrasonic Sensor - HC-SR04"));
    		else if(c == '5') 
    			Serial.println(F("Now Testing LED - RGB Addressable, PTH, 5mm Diffused (5 Pack)"));
    		else if(c == '6') 
    			Serial.println(F("Now Testing Servo - Generic Continuous Rotation (Micro Size)"));
    		else if(c == '7') 
    			Serial.println(F("Now Testing 3W Stereo Speaker"));
    		else if(c == '8') 
    			Serial.println(F("Now Testing Vibration Motor"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
}

/*******************************************************

*    Circuito.io is an automatic generator of schematics and code for off
*    the shelf hardware combinations.

*    Copyright (C) 2016 Roboplan Technologies Ltd.

*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.

*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.

*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*    In addition, and without limitation, to the disclaimers of warranties 
*    stated above and in the GNU General Public License version 3 (or any 
*    later version), Roboplan Technologies Ltd. ("Roboplan") offers this 
*    program subject to the following warranty disclaimers and by using 
*    this program you acknowledge and agree to the following:
*    THIS PROGRAM IS PROVIDED ON AN "AS IS" AND "AS AVAILABLE" BASIS, AND 
*    WITHOUT WARRANTIES OF ANY KIND EITHER EXPRESS OR IMPLIED.  ROBOPLAN 
*    HEREBY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT 
*    NOT LIMITED TO IMPLIED WARRANTIES OF MERCHANTABILITY, TITLE, FITNESS 
*    FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND THOSE ARISING BY 
*    STATUTE OR FROM A COURSE OF DEALING OR USAGE OF TRADE. 
*    YOUR RELIANCE ON, OR USE OF THIS PROGRAM IS AT YOUR SOLE RISK.
*    ROBOPLAN DOES NOT GUARANTEE THAT THE PROGRAM WILL BE FREE OF, OR NOT 
*    SUSCEPTIBLE TO, BUGS, SECURITY BREACHES, OR VIRUSES. ROBOPLAN DOES 
*    NOT WARRANT THAT YOUR USE OF THE PROGRAM, INCLUDING PURSUANT TO 
*    SCHEMATICS, INSTRUCTIONS OR RECOMMENDATIONS OF ROBOPLAN, WILL BE SAFE 
*    FOR PERSONAL USE OR FOR PRODUCTION OR COMMERCIAL USE, WILL NOT 
*    VIOLATE ANY THIRD PARTY RIGHTS, WILL PROVIDE THE INTENDED OR DESIRED
*    RESULTS, OR OPERATE AS YOU INTENDED OR AS MAY BE INDICATED BY ROBOPLAN. 
*    YOU HEREBY WAIVE, AGREE NOT TO ASSERT AGAINST, AND RELEASE ROBOPLAN, 
*    ITS LICENSORS AND AFFILIATES FROM, ANY CLAIMS IN CONNECTION WITH ANY OF 
*    THE ABOVE. 
********************************************************/