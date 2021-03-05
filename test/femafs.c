#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include <FEMBasis.h>
#include <FEMClassify.h>
#include <FEMDataset.h>
#include <FEMFeatureSelection.h>
#include <FEMUtils.h>
#define MAX_CLASSES 100
#define SHEPARD_BASIS 0
#define RADIAL_BASIS 1
#define GAUSSIAN_BASIS 2
#define _PAUSE                                                                 \
  int pause;                                                                   \
  fprintf(stderr, "Aqui");                                                     \
  scanf("%d", &pause);

int main(int argc, char **argv) {
  FEMDataset dataset_train;
  FEMDataset dataset_test;

  struct timespec begin, end;
  double time_spent;
  int basis_id = atoi(argv[1]);
  motherFunctionF *basis = NULL;

  switch (basis_id) {
  case RADIAL_BASIS:
    fprintf(stderr, "RADIAL BASIS\n");
    basis = &FEMRadialNormalizedMotherFunction_feature;
    break;
  case GAUSSIAN_BASIS:
    fprintf(stderr, "GAUSSIAN BASIS\n");
    basis = &FEMGaussianNormalizedMotherFunction_feature;
    break;
  default:
    fprintf(stderr, "SHEPARD BASIS\n");
    basis = &FEMShepardMotherFunction_feature;
  }

  double additional_parameters[MAX_ADDITIONAL_PARAMETERS];

  fprintf(stderr, "\n ./program <basis> "
                  "<0.0 - 1.0> percentage of features to be selected "
                  "<train_file> <test_file> "
                  "<aditional_params[0]> ..... "
                  "<aditional_params[n]>\n\n");

  readDataset(&dataset_train, argv[3]);
  readDataset(&dataset_test, argv[4]);

  fprintf(stderr, "\n[Dataset: %s]Samples:%d classes:%d features %d", argv[3],
          dataset_train.number_of_samples, dataset_train.number_of_classes,
          dataset_train.number_of_features);
  fprintf(stderr, "\n[Dataset: %s]Samples:%d classes:%d features %d\n", argv[4],
          dataset_test.number_of_samples, dataset_test.number_of_classes,
          dataset_test.number_of_features);

  //  int step = (int)(255/dataset_train.number_of_classes); //to color the
  for (int i = 5; i < argc; i++)
    additional_parameters[i - 5] = atof(argv[i]);

  char out_train_name[] = "train.feature.out";
  char out_test_name[] = "test.feature.out";
  clock_gettime(CLOCK_MONOTONIC, &begin);
  FeatureSelectionVector(&dataset_train, &dataset_test,
                         dataset_train.number_of_samples, additional_parameters,
                         basis, atof(argv[2]), out_train_name, out_test_name);
  clock_gettime(CLOCK_MONOTONIC, &end);
  time_spent = (end.tv_sec - begin.tv_sec);
  time_spent += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
  fprintf(stderr, "\n Estimation time: %f seconds\n", time_spent);
}
