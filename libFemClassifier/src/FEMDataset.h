#ifndef FEM_DATASET_H_
#define FEM_DATASET_H_

#include <stdio.h>
#include <stdlib.h>
#include "FEMSample.h"

typedef struct femdataset
{
  int number_of_classes;   
  int number_of_samples;
  int number_of_features;
  int *samples_per_class;
  FEMSample *samples;
}  FEMDataset;

void alloc_FEMDataset(FEMDataset *dataset, int number_of_classes, int number_of_samples, int number_of_features);
void dealloc_FEMDataset(FEMDataset *dataset);
void readDataset(FEMDataset *dataset, char *name_file_samples);


#endif
