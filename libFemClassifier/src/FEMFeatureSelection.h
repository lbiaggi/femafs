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

/* novas adições Lucas */
double distanceFEM_feature(double a, double b);
double FEMShepardMotherFunction_feature(
    double a, double  value, double additional_parameters[]);
double FEMGaussianNormalizedMotherFunction_feature(
    double a, double value,double additional_parameters[]);
double FEMRadialNormalizedMotherFunction_feature(
    double a, double value,double additional_parameters[]);

typedef double motherFunctionF(double a, double value, double additional_parameters[]);

double parallel_basis(FEMDataset *dataset, int class, int feat_id, double value, double min, double max, double additional_parameters[], motherFunctionF *FEMbasisF, int start, int stop);
double probabilityByClassFeature(FEMDataset *dataset, int class, int feat_id, double value, double min, double max, double additional_parameters[], motherFunctionF *FEMbasisF);
void getMinMaxFeature(FEMDataset *dataset, int feat_id, double *min, double *max);
void bubleSortF(double prob[], int index[], int number_of_points);
void createDatasetFeaturesSelectedOPFFormat(FEMDataset *dataset_train, FEMDataset *dataset_test, int feat_id[],double perc, char out_train[], char out_test[]);
void FeatureSelectionVector(FEMDataset *dataset_train, FEMDataset *dataset_test, int n_samples, double additional_parameters[], motherFunctionF *FEMbasisF,double perc, char out_train[], char out_test[]);
void Dif(FEMDataset *dataset_train, FEMDataset *dataset_test);

#endif
