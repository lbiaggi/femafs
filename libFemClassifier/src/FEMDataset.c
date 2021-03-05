#include "FEMDataset.h"

void alloc_FEMDataset(FEMDataset* dataset, int number_of_classes,
    int number_of_samples, int number_of_features)
{
    int i = 0;

    if (number_of_classes <= 1 || number_of_features <= 1
        || number_of_samples < 1) {
        fprintf(stderr,
            "ERROR: alloc_FEMSample(). Illegal number of number_of_classes: "
            "%d OR number_of_samples: %d OR number_of_features: %d  ",
            number_of_classes, number_of_samples, number_of_features);
        exit(1);
    }

    dataset->samples
        = (FEMSample*)malloc(sizeof(FEMSample) * number_of_samples);
    if (dataset->samples == NULL) {
        fprintf(stderr,
            "ERROR: alloc_FEMDataset(). Can't alloc memory for samples: %d",
            number_of_samples);
        exit(1);
    }

    dataset->samples_per_class = (int*)malloc(sizeof(int) * number_of_classes);
    if (dataset->samples_per_class == NULL) {
        fprintf(stderr,
            "ERROR: alloc_FEMDataset(). Can't alloc memory for "
            "samples_per_class: %d",
            number_of_classes);
        exit(1);
    }

    dataset->number_of_classes = number_of_classes;
    dataset->number_of_samples = number_of_samples;
    dataset->number_of_features = number_of_features;

    for (i = 0; i < number_of_samples; i++)
        alloc_FEMSample(&(dataset->samples[i]), number_of_features);
}

void dealloc_FEMDataset(FEMDataset* dataset)
{
    int i = 0;

    for (i = 0; i < dataset->number_of_samples; i++)
        dealloc_FEMSample(&(dataset->samples[i]));
    free(dataset->samples);
    free(dataset->samples_per_class);
}

void readDataset(FEMDataset* dataset, char* name_file_samples)
{
    int samples = 0 , feature = 0;
    FILE* file_samples = fopen(name_file_samples, "r");
    int number_of_classes = 0, number_of_samples = 0, number_of_features = 0;

    if (file_samples == NULL) {
        fprintf(stderr, "ERROR: read_samples(). Can't read the file: %s",
            name_file_samples);
        exit(1);
    }

    fscanf(file_samples, "%d %d %d", &number_of_classes, &number_of_samples,
        &number_of_features);
    alloc_FEMDataset(dataset, number_of_classes, number_of_samples, number_of_features);

    for (samples = 0; samples < number_of_samples; samples++) {
        int class = -1;

        fscanf(file_samples, "%d", &class);
        if (class <= 0 || class > number_of_classes) {
            fprintf(stderr,
                "ERROR: readDataset(). Sample %d with illegal class: %d \
number_of_classes: %d\n",
                samples, class, number_of_classes);
            exit(1);
        }

        dataset->samples[samples].class = class;

        dataset->samples_per_class[class]++;

        for (feature = 0; feature < number_of_features; feature++)
            fscanf(file_samples, "%lf", &(dataset->samples[samples].features[feature]));
    }
    return;
}