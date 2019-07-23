#ifndef POWERSAMPLING_H
#define POWERSAMPLING_H

int calculateMeanValues(float *sampleValues, int sampleCount,int analogPinCount, float *meanArray);
int calculateRMS(float *sampleValues, int sampleCount,int analogPinCount, float meanArray[], float rmsArray[]);

#endif