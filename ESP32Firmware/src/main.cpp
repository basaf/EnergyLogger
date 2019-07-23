#include <Arduino.h>
#include "utils.h"
#include "powerSampling.h"
#include "bleInterface.h"



//ADC configuration 
const int analogPins[] = { A2, A3, A4}; //Define analog input pins A0,A1 & A5 are on ADC2 which is not working with Bluetooth or WiFi! (see https://github.com/espressif/arduino-esp32/issues/2557)
const int usedAnalogPinCount = sizeof(analogPins) / sizeof(analogPins[0]);
const int resolution=12;            //bit

//conversion parameters
const float n = 100 / 0.05; //100A:50mA
const float uRef=3.3;               //V
const float convFactor=uRef/resolution*n;

//sampling parameters
const unsigned long sampleTime = 2;                //ms //perform adc at given sampling rate
const int maxSamples = 10;                         //at lest two periods -> for 50 Hz: 20 samples @ 2ms sampling)

//timing variables
unsigned long currentTime;
unsigned long lastSampleTime = 0;

TaskHandle_t Task1;

void setup(){
  //----------setup hardware--------------------
  //use internal LED
  pinMode(LED_BUILTIN, OUTPUT);

  //set all analog input pins as input
  for (int i = 0; i < usedAnalogPinCount; i++)
  {
    pinMode(analogPins[i], INPUT);
  }
  //set resolution of ADC
  analogReadResolution(resolution);

  //----------setup communication----------------
  Serial.begin(115200);
  
  bleInit();

  delay(1000);
}
//   xTaskCreate(
//       bleTask,   /* Task function. */
//       "TaskOne", /* String with name of task. */
//       10000,     /* Stack size in bytes. */
//       NULL,      /* Parameter passed as input of the task */
//       1,         /* Priority of the task. */
//       &Task1);     /* Task handle. */


void loop()
{

  float sampleValues[maxSamples][usedAnalogPinCount]; //row- col
  float meanValues[usedAnalogPinCount];
  float rmsValues[usedAnalogPinCount];

 //get more than 2 cycles of 50 Hz -> 40ms

  //get all analog inputs for at least 2 periods ()
  int sample = 0;
  while (sample < maxSamples)
  {
    currentTime = millis();
    //Read all analog inputs with sample rate //TODO: optimize analog read -> esp adc conversion
    if (currentTime - lastSampleTime >= sampleTime)
    {
      
      for (int j = 0; j < usedAnalogPinCount; j++)
      {
        sampleValues[sample][j] = analogRead(analogPins[j]);
      }
      
      sample++;
      lastSampleTime = millis();
    }
  }

  printAllSamples(&sampleValues[0][0], maxSamples, usedAnalogPinCount);
  calculateMeanValues(&sampleValues[0][0], maxSamples, usedAnalogPinCount,meanValues);
  calculateRMS(&sampleValues[0][0], maxSamples, usedAnalogPinCount, meanValues, rmsValues);
  
  //convert to current based on 100A:50m

  // calculate power

  Serial.println("Mean of all samples");
  Serial.println(meanValues[0]);
  Serial.println(meanValues[1]);
  Serial.println(meanValues[2]);

  Serial.println("---------------");

 delay(5000);
}