#include <Arduino.h>

//#include "bleInterface.h"
#include <math.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>




unsigned long currentTime, t1, t2;
unsigned long lastSampleTime = 0;

const float n = 100 / 0.05;                        //100A:50mA
const unsigned long sampleTime = 2;                //ms //perform adc at given sampling rate
const int maxSamples = 10;                         //at lest two periods -> for 50 Hz: 20 samples @ 2ms sampling)
const int analogPins[] = {A0, A1, A2, A3, A4, A5}; //Define analog input pins
const int usedAnalogPinCount = sizeof(analogPins) / sizeof(analogPins[0]);




TaskHandle_t Task1;

float myanalogRead(int nb)
{

  return nb;
}

void printAllSamples(float array[][usedAnalogPinCount], int dimLength1, int dimLength2)
{
  Serial.println("Two Dimensional array elements:");
  for (int i = 0; i < dimLength1; i++)
  {
    for (int j = 0; j < dimLength2; j++)
    {
      Serial.print(array[i][j]);
      Serial.print(" - ");

      if (j >= dimLength2 - 1)
      {
        Serial.println("|");
      }
    }
  }
  Serial.println("");
}

int calculateMeanValues(float sampleValues[][usedAnalogPinCount], float *meanArray)
{
  //clear memory
  for (int i = 0; i < usedAnalogPinCount; i++)
  {
    meanArray[i] = 0;
  }

  //calculate mean
  for (int port = 0; port < usedAnalogPinCount; port++)
  {
    for (int k = 0; k < maxSamples; k++)
    {
      meanArray[port] = meanArray[port] + sampleValues[k][port];

    }
    meanArray[port] = meanArray[port] / maxSamples;
  
  }

  return 0;
}

int calculateRMS(float sampleValues[][usedAnalogPinCount], float meanArray[], float rmsArray[])
{
  for (int i = 0; i < usedAnalogPinCount; i++)
  {
    rmsArray[i] = 0;
  }

  //calculate rms
  for (int port = 0; port < usedAnalogPinCount; port++)
  {
    for (int k = 0; k < maxSamples; k++)
    {
      rmsArray[port] = rmsArray[port] + (sampleValues[k][port] - meanArray[port]) * (sampleValues[k][port] - meanArray[port]);
    }
    rmsArray[port] = sqrt(rmsArray[port] / maxSamples);
  }

  return 0;
}
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void setup(){
  //use internal LED
  pinMode(LED_BUILTIN, OUTPUT);

  //set all analog input pins as input
  for (int i = 0; i < usedAnalogPinCount; i++)
  {
    pinMode(analogPins[i], INPUT);
  }

  // put your setup code here, to run once:
  Serial.begin(115200);
  analogReadResolution(12);
  delay(1000);
  BLEDevice::init("EnergyMonitor");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue("Hello World");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
 
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

  float sampleValues[maxSamples][usedAnalogPinCount];
  float meanValues[usedAnalogPinCount];
  float rmsValues[usedAnalogPinCount];

  // put your main code here, to run repeatedly:

  //get more than 2 cycles of 50 Hz -> 40ms

  Serial.println("Start loop...");
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

  //printAllSamples(sampleValues, maxSamples, usedAnalogPinCount);
  calculateMeanValues(sampleValues, meanValues);
  calculateRMS(sampleValues, meanValues, rmsValues);

  //convert to current based on 100A:50m

  // calculate power

  Serial.println("Mean of all samples");
  Serial.println(meanValues[0]);
  Serial.println(meanValues[1]);
  Serial.println(meanValues[2]);
  Serial.println(meanValues[3]);
  Serial.println(meanValues[4]);
  Serial.println(meanValues[5]);
  Serial.println("---------------");

  delay(5000);
}