#ifndef FEM_SAMPLE_H_
#define FEM_SAMPLE_H_

#include <stdio.h>
#include <stdlib.h>

#define MAX_ADDITIONAL_PARAMETERS 5

typedef struct femsample
{
  int class;   
  int number_of_features;
  double dist;
  double weigth;
  double value;
  double *features;
  double *probability;
  int label;  
  double additional_parameters[MAX_ADDITIONAL_PARAMETERS];
}  FEMSample;

void alloc_FEMSample(FEMSample *sample,int number_of_features);
void dealloc_FEMSample(FEMSample *sample);

#endif