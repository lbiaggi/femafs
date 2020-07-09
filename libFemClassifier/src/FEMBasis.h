#ifndef FEM_BASIS_H_
#define FEM_BASIS_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "FEMSample.h"
#include "FEMDataset.h"
#include "FEMUtils.h"

double FEMShepardMotherFunction(FEMSample a, FEMSample b, double additional_parameters[]);
double FEMGaussianNormalizedMotherFunction(FEMSample a, FEMSample b, double additional_parameters[]);
double FEMRadialNormalizedMotherFunction(FEMSample a, FEMSample b, double additional_parameters[]);
double** FEMRadialcreateZMatrix( FEMDataset *dataset, double additional_parameters[]);
#endif
