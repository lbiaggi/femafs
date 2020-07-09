#ifndef FEM_UTILS_H_
#define FEM_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include "FEMSample.h"
#include "FEMDataset.h"

typedef double motherFunction(FEMSample a, FEMSample b, double additional_parameters[]);


double distanceFeatures(double a[], double b[], int number_of_features);
double distanceFEMSample(FEMSample a, FEMSample b);
void quickSort( double dist[], int index[], int left, int rigth);
int partition( double dist[], int index[], int left, int rigth) ;
void computeDistanceDatasetFromSample(FEMDataset *dataset, FEMSample sample);
void closerSamplesPerClass(FEMDataset dataset, FEMSample sample, FEMDataset *closer, int number_of_points, int class);
void mergeCloserFEMSamplesToDataset(FEMDataset *dataset, FEMDataset samples[], int number_of_classes, int number_of_points, int number_of_features);

void identity_matrix(double **identity,int n);
int invert_matrix (double **mat,double **inv, int n);
double* mulMatrixVetor(double **a, double *b, int n,int m);
double* samplesValuesToVector(FEMDataset *dataset);
void vectorToSampleValues(FEMDataset *dataset, double *vec);
void printMatrix(double **m, int n, char tag[]);
void printVector(double *v, int n, char tag[]);


#endif
