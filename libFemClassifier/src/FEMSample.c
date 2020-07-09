#include "FEMSample.h"

void alloc_FEMSample(FEMSample *sample,int number_of_features)
{
  if(number_of_features <= 0)
  {
    fprintf(stderr,"ERROR: alloc_FEMSample(). Illegal number of features: %d",number_of_features);
    exit(1);  
  }  
  
  sample->number_of_features = number_of_features;
  
  sample->features    = (double*)malloc(sizeof(double)*number_of_features);
  sample->probability = (double*)malloc(sizeof(double)*number_of_features);
  
  if(sample->features == NULL || sample->probability == NULL)
  {
     fprintf(stderr,"ERROR: alloc_FEMSample(). Can't alloc memory: %d",number_of_features);
     exit(1);  
  }
  return;  
}  

void dealloc_FEMSample(FEMSample *sample)
{
  free(sample->features); 
  free(sample->probability);
  sample->number_of_features = 0;
  
  
  return;  
}  
