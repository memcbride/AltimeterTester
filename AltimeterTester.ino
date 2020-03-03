/***************************************************************************
  An Altimeter Tester for checking dual deploy events.  Design is by Dave
  Combs of WSR703 (www.WSR703.org) and MacModeler. 

  The design uses the following components:
  
  Core Processor:    Adafruit Feather M4 proto.
  Touch Screen:      Adafruit 3.5" 320x480 Color TFT TouchScreen Featherwing
  Pressure Sensor:   Adafruit BMP388

  The code design is derived from the code from the many Adafruit learning 
  guides ( https://learn.adafruit.com ) and adapted to using objects in C++. 
  The code at: http://paulmurraycbr.github.io/ArduinoTheOOWay.html was 
  particularly helpful.  The code was built up slowly as a set of objects, 
  many inheriting from the basic runnable class described on the site
  listed above.
  
  Author:  Mark E. McBride
  Last Update:  2020.01.25

  ChangeLog:
  next up:     Remove flicker by buffering smaller parts of screen
  2020.01.25:  Changed font for numbers to 7 segment full
  2020.01.25:  Display working with flicker
  2020.01.25:  Basic HX8357 class displaying labels
  2020.01.25:  Adjusted what is being sampled
  2020.01.19:  set so that hardware BMP object passed by reference, rather 
               than put global variables inside class objects
  2020.01.17:  Reading Sensor properly, saving as global curPressure
  2020.01.17:  Create initial Runnable setup 
  
 ***************************************************************************/
/* Arduino Libraries */

/* MacModeler Altimeter Tester header files */
#include "Globals.h"
#include "Runnable.h"
#include "BMP.h"
#include "TFT35.h"

//#define debug_m  // comment out for final version

/*Per hardware/software driver setups */
void setup() {
  #ifdef debug_m
    while(!Serial) { // wait until console is open
      delay(100);
    }
    Serial.begin(115200);
    delay(100);
    Serial.println("Altimeter Testing Started");
    delay(2000);
  #endif

  // Configure all the runnable objects
  Runnable::setupAll();

}

/* Don't add anything else to the main loop, instead use a runnable oject */
void loop() {
  Runnable::loopAll();
}
