#ifndef FEM_FEATURESELECTION_H_
#define FEM_FEATURESELECTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "FEMSample.h"
#include "FEMDataset.h"
#include "FEMUtils.h"
#include "FEMBasis.h"

double probabilityByClassFeature(FEMDataset *dataset, int class, int feat_id, double value, double min, double max, double additional_parameters[], motherFunctionF *FEMbasisF);
void getMinMaxFeature(FEMDataset *dataset, int feat_id, double *min, double *max);
void bubleSortF(double prob[], int index[], int number_of_points);
void createDatasetFeaturesSelectedOPFFormat(FEMDataset *dataset_train, FEMDataset *dataset_test, int feat_id[],double perc, char out_train[], char out_test[]);
double FeatureSelectionVector(FEMDataset *dataset_train, FEMDataset *dataset_test, int n_samples, double additional_parameters[], motherFunctionF *FEMbasisF,double perc, char out_train[], char out_test[]);
void Dif(FEMDataset *dataset_train, FEMDataset *dataset_test);

#endif
