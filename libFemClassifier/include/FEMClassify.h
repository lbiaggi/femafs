#ifndef FEM_CLASSIFY_H_
#define FEM_CLASSIFY_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "FEMSample.h"
#include "FEMDataset.h"
#include "FEMUtils.h"
#include "FEMBasis.h"


double probabilityByClass(FEMDataset *dataset, int class, FEMSample sample, double additional_parameters[], motherFunction *FEMbasis);
double FEMClassify(FEMDataset *dataset, int number_of_classes, int *class, FEMSample sample, double additional_parameters[], motherFunction *FEMbasis);

#endif
