/***************************************************************************
  An Altimeter Tester for checking dual deploy events.  Design is by Dave
  Combs of WSR703 (www.WSR703.org) and MacModeler. 

  The BMP class handles the interactions with the BMP388 board including
  reading and calling a data struct.  The Adafruit tutorial for the BMP388
  at https://learn.adafruit.com/adafruit-bmp388 provided invaluable guidance
  in developing this class.
   
  Author:  Mark E. McBride
  Last Update:  2020.01.25

  ChangeLog:
  next up:     n/a
  2020.01.25:  Adjust what is being sampled
  2020.01.19:  set so that hardware BMP object passed by reference, rather 
               than put global variables inside class objects
  2020.01.17:  Create initial Runnable setup 
  
 ***************************************************************************/
#ifndef BMP_H
#define BMP_H

/* Arduino Libraries */
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

/* MacModeler Headers */
#include "Globals.h"

/* sensor defines */
#define BMP_SCK 24
#define BMP_MISO 23
#define BMP_MOSI 22
#define BMP_CS 11

#define SEALEVELPRESSURE_HPA 1013.25

Adafruit_BMP3XX myBMP = Adafruit_BMP3XX(BMP_CS);  // hardware SPI interface, Chip Select pin 11

//#define debug_bmp  // comment out for final version

class BMP: public Runnable {

  private:
    /* internal variables with default initializers */
    Adafruit_BMP3XX* theSensor;  // pointer to the sensor
    Readings* current;  // pointer to current readings structure
    float sealevel {SEALEVELPRESSURE_HPA};
    int temperature {72};
    int altitude {900};
    boolean stabilized {false};
    int loopCount {0};

    /* methods */
    float avgTemperature(float M) {
      #define LM_SIZE 20
      static float LM[LM_SIZE];      // LastMeasurements
      static int index = 0;
      static float sum = 0;
      static int count = 0;
    
      // keep sum updated to improve speed.
      sum -= LM[index];
      LM[index] = M;
      sum += LM[index];
      index++;
      index = index % LM_SIZE;
      if (count < LM_SIZE) count++;
    
      return sum / count;
    }

    float avgElevation(float M) {
      #define LM_SIZE 100
      static float LM[LM_SIZE];
      static int index = 0;
      static float sum = 0;
      static int count = 0;
      // keep sum updated to improve speed.
      sum -= LM[index];
      LM[index] = M;
      sum += LM[index];
      index++;
      index = index % LM_SIZE;
      if (count < LM_SIZE) count++;
      return sum / count;
    }

  public:
    /* Constructor */
    BMP(Adafruit_BMP3XX* sensor, Readings* values) : theSensor{sensor}, current{values} {}
    BMP(Adafruit_BMP3XX* sensor,Readings* values, float x) : theSensor{sensor}, current{values}, sealevel{x} {}  

    /* Setup method */
    void setup() {
      /* startup the sensor */
      if (!theSensor->begin()) {
        #ifdef debug_bmp
          Serial.println("Could not start up BMP388 sensor, check wiring");
        #endif
        while(1);
      } else {
        #ifdef debug_bmp
          Serial.println("BMP388 started!");
        #endif
      }
      /* Setup oversampling and filter initializations */
      theSensor->setTemperatureOversampling(BMP3_OVERSAMPLING_2X);
      theSensor->setPressureOversampling(BMP3_OVERSAMPLING_8X);
      theSensor->setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
      theSensor->setOutputDataRate(BMP3_ODR_50_HZ);
    }

    /* Loop method reads the sensor and sets global value */
    void loop() {
      if (!theSensor->performReading()) {
        #ifdef debug_bmp
          Serial.println("Failed to read BMP388");
        #endif
        return;
      }
      temperature = theSensor->temperature;
      current->temperature = int(avgTemperature(temperature * 1.8 + 32)+0.5);
      altitude = theSensor->readAltitude(sealevel);
      if (stabilized) {
        #ifdef debug_bmp
          Serial.println("stablized code");
        #endif
        current->agl = int(altitude * 3.280839895 + 0.5) - current->elevation;
      } else {
        #ifdef debug_bmp
          Serial.println("not stabilized code");
        #endif
        if (abs(altitude - current->elevation < 0.01) && loopCount > 100) stabilized = true;
        current->elevation = int(avgElevation(altitude * 3.280839895)+0.5);       
        current->agl = int(altitude * 3.280839895 + 0.5) - current->elevation;
      }
      #ifdef debug_bmp
        Serial.print(current->temperature);Serial.print(",");
        Serial.print(current->elevation);Serial.print(",");
        Serial.print(current->agl);Serial.println();
      #endif
      loopCount++;
    }
};

/* Create the sensor instance */
BMP mySensor = BMP(&myBMP,&curPressure);

#endif
