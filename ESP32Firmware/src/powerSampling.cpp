#include "powerSampling.h"
#include <math.h>

//int convertToAnalogValue(float factor)

int calculateMeanValues(float *sampleValues, int sampleCount,int analogPinCount, float *meanArray)
{
  //clear memory
  for (int i = 0; i < analogPinCount; i++)
  {
    meanArray[i] = 0;
  }

  //calculate mean
  for (int port = 0; port < analogPinCount; port++)
  {
    for (int k = 0; k < sampleCount; k++)
    {
      meanArray[port] = meanArray[port] + sampleValues[port+k*analogPinCount]; //[k][port];
    }
    meanArray[port] = meanArray[port] / sampleCount;
  
  }

  return 0;
}

int calculateRMS(float *sampleValues, int sampleCount,int analogPinCount, float meanArray[], float rmsArray[])
{
  for (int i = 0; i < analogPinCount; i++)
  {
    rmsArray[i] = 0;
  }

  //calculate rms
  for (int port = 0; port < analogPinCount; port++)
  {
    for (int k = 0; k < sampleCount; k++)
    {
      rmsArray[port] = rmsArray[port] + (sampleValues[port+k*analogPinCount] - meanArray[port]) * (sampleValues[port+k*analogPinCount] - meanArray[port]);
    }
    rmsArray[port] = sqrt(rmsArray[port] / sampleCount);
  }

  return 0;
}