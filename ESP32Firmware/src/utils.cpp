#include <Arduino.h>
#include "utils.h"

float myanalogRead(int nb)
{

  return nb;
}

void printAllSamples(float *array, int dimRow, int dimCol)
{
  Serial.println("Two Dimensional array elements:");
  for (int i = 0; i < dimRow; i++)
  {
    for (int j = 0; j < dimCol; j++)
    {
      Serial.print(array[i*dimCol+j]);
      Serial.print(" - ");

      if (j >= dimCol - 1)
      {
        Serial.println("|");
      }
    }
  }
  Serial.println("");
}

void blinkTask(void * parameter){
    while(true){
        digitalWrite(LED_BUILTIN,HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN,LOW);
        delay(1000);
    }
}