#include "FEMFeatureSelection.h"
#include <string.h>

double probabilityByClassFeature(FEMDataset* dataset, int class, int feat_id,
    double value, double min, double max, double additional_parameters[],
    motherFunctionF* FEMbasisF)
{

    int i = 0;
    double sum = 0.0;
    double probability = 0.0;

    for (i = 0; i < dataset->number_of_samples; i++) {
        if (dataset->samples[i].class == class) {
            dataset->samples[i].value = 1.0;
        } else {
            dataset->samples[i].value = 0.0;
        }
        dataset->samples[i].weigth = FEMbasisF((min - dataset->samples[i].features[feat_id]) / (min - max + 0.0000000000000001), value, additional_parameters);
        sum += dataset->samples[i].weigth;
    }

    for (i = 0; i < dataset->number_of_samples; i++){
        dataset->samples[i].weigth /= (sum + 0.0000000000000001);
        probability += dataset->samples[i].value * dataset->samples[i].weigth;
    }

    return probability;
}

void getMinMaxFeature(
    FEMDataset* dataset, int feat_id, double* min, double* max)
{
    int i;

    *min = *max = dataset->samples[0].features[feat_id];

    for (i = 1; i < dataset->number_of_samples; i++) {

        if (dataset->samples[i].features[feat_id] > *max)
            *max = dataset->samples[i].features[feat_id];

        if (dataset->samples[i].features[feat_id] < *min)
            *min = dataset->samples[i].features[feat_id];
    }
}

void bubleSortF(double prob[], int index[], int number_of_points)
{
    int i, j;

    for (i = 0; i < number_of_points - 1; i++) {
        for (j = i + 1; j < number_of_points; j++) {
            if (prob[i] > prob[j]) {
                double daux = prob[i];
                int iaux = index[i];

                prob[i] = prob[j];
                prob[j] = daux;
                index[i] = index[j];
                index[j] = iaux;
            }
        }
    }
    return;
}

void createDatasetFeaturesSelectedOPFFormat(FEMDataset* dataset_train,
    FEMDataset* dataset_test, int feat_id[], double perc, char out_train[],
    char out_test[])
{
    char filename[200];
    int i, j, k = (int)(perc * dataset_train->number_of_features);
    FILE *train, *test;

    train = fopen(out_train, "w");

    test = fopen(out_test, "w");

    fprintf(train, "%d %d %d", dataset_train->number_of_samples,
        dataset_train->number_of_classes, k);
    for (i = 0; i < dataset_train->number_of_samples; i++) {
        fprintf(train, "\n%d %d", i + 1, dataset_train->samples[i].class);
        for (j = 0; j < k; j++) {
            fprintf(
                train, " %f", dataset_train->samples[i].features[feat_id[j]]);
        }
    }

    fprintf(test, "%d %d %d", dataset_test->number_of_samples,
        dataset_test->number_of_classes, k);
    for (i = 0; i < dataset_test->number_of_samples; i++) {
        fprintf(test, "\n%d %d", i + 1, dataset_test->samples[i].class);
        for (j = 0; j < k; j++) {
            fprintf(
                test, " %f", dataset_test->samples[i].features[feat_id[j]]);
        }
    }
}

double FeatureSelectionVector(FEMDataset* dataset_train,
    FEMDataset* dataset_test, int n_samples, double additional_parameters[],
    motherFunctionF* FEMbasisF, double perc, char out_train[], char out_test[])
{
    int i, j, c1, c2, k;
    double valuei, valuej, probi, probj;
    //   I, K , J
    int feat, sample, class; // J

    double *prob_feat_discrepancy = (double*)malloc(sizeof(double) * dataset_train->number_of_features);
    double *max = (double*)malloc(sizeof(double) * dataset_train->number_of_features);
    double *min = (double*)malloc(sizeof(double) * dataset_train->number_of_features);
    double ***values = (double***)malloc( sizeof(double**) * dataset_train->number_of_features);

    int* feat_id = (int*)malloc(sizeof(int) * dataset_train->number_of_features);

    if (prob_feat_discrepancy == NULL || feat_id == NULL) {
        fprintf(stderr,
            "\nERROR: FeatureSelectionVector() in module FEMFeatureSelection, "
            "cannot allocate vectors!\n");
        exit(1);
    }

    for(feat = 0; feat < dataset_train->number_of_features; feat++) {
        prob_feat_discrepancy[feat] = 0.0;
        getMinMaxFeature(dataset_train, feat, &min[feat], &max[feat]);
        feat_id[feat] = feat;
        values[feat] = (double**)malloc(sizeof(double*) * n_samples);
        for (sample = 0; sample < n_samples; sample++) {
            values[feat][sample] = (double*)malloc( sizeof(double) * dataset_train->number_of_classes);
            for (class = 0; class < dataset_train->number_of_classes; class++) {
                double value = min[feat] + sample * ((max[feat] - min[feat]) / n_samples);
                values[feat][sample][class] = probabilityByClassFeature(dataset_train, class + 1, feat, value, min[feat], max[feat], additional_parameters, FEMbasisF);
            }
        }
    }

    for (i = 0; i < dataset_train->number_of_features; i++) // loop feature out
    {

        for (j = i + 1; j < dataset_train->number_of_features;
             j++) // loop feature in
        {
            for (c1 = 0; c1 < dataset_train->number_of_classes;
                 c1++) // loop class out
            {
                for (c2 = 0; c2 < dataset_train->number_of_classes;
                     c2++) // loop class in
                {
                    for (k = 0; k < n_samples; k++) {
                        double mult = values[i][k][c1] * values[j][k][c2];
                        prob_feat_discrepancy[i] += mult;
                        prob_feat_discrepancy[j] += mult;
                    }
                }
            }
        }
    }

    bubleSortF( prob_feat_discrepancy, feat_id, dataset_train->number_of_features);
    for(i = 0; i < dataset_train->number_of_features; i++)   {
        fprintf(stderr,"\n[BEST %d] => feat:%d score:%.17lf!",i,feat_id[i],prob_feat_discrepancy[i]);
    }
    createDatasetFeaturesSelectedOPFFormat(
        dataset_train, dataset_test, feat_id, perc, out_train, out_test);
}

void Dif(FEMDataset* dataset_train, FEMDataset* dataset_test)
{
    int i, j, k, c, f;

    double valuei, valuej, value;
    long int count_difs = 0, count_equals = 0;

    double *dif
        = (double*)malloc(sizeof(double) * dataset_train->number_of_features),
        *equals
        = (double*)malloc(sizeof(double) * dataset_train->number_of_features),
        *max
        = (double*)malloc(sizeof(double) * dataset_train->number_of_features),
        *min
        = (double*)malloc(sizeof(double) * dataset_train->number_of_features),
        *aux
        = (double*)malloc(sizeof(double) * dataset_train->number_of_features);

    FILE* f_dif_train = fopen("train_dif.txt", "w");
    FILE* f_dif_test = fopen("test_dif.txt", "w");

    if (dif == NULL || max == NULL || min == NULL || aux == NULL) {
        fprintf(stderr,
            "\nERROR: Dif() in module FEMFeatureSelection, cannot allocate "
            "vectors!\n");
        exit(1);
    }

    // set zero in the probability and set the class id
    for (i = 0; i < dataset_train->number_of_features; i++) {
        dif[i] = equals[i] = 0.0;
        getMinMaxFeature(dataset_train, i, &min[i], &max[i]);
    }

    for (i = 0; i < dataset_train->number_of_samples; i++) {
        for (j = i + 1; j < dataset_train->number_of_samples; j++) {
            if (dataset_train->samples[i].class
                != dataset_train->samples[j].class) {
                for (f = 0; f < dataset_train->number_of_features; f++) {
                    valuei = (dataset_train->samples[i].features[f] - min[f])
                        / (max[f] - min[f]);
                    valuej = (dataset_train->samples[j].features[f] - min[f])
                        / (max[f] - min[f]);
                    dif[f] += fabs(valuei - valuej);
                }
                count_difs++;
            } else {
                for (f = 0; f < dataset_train->number_of_features; f++) {
                    valuei = (dataset_train->samples[i].features[f] - min[f])
                        / (max[f] - min[f]);
                    valuej = (dataset_train->samples[j].features[f] - min[f])
                        / (max[f] - min[f]);
                    equals[f] += fabs(valuei - valuej);
                }
                count_equals++;
            }
        }
    }

    for (f = 0; f < dataset_train->number_of_features; f++) {
        dif[f] /= count_difs;
        equals[f] /= count_equals;
        aux[f] = fabs(dif[f] - equals[f]);
    }

    fprintf(f_dif_train, "%d %d %d", dataset_train->number_of_samples,
        dataset_train->number_of_classes, dataset_train->number_of_features);
    for (i = 0; i < dataset_train->number_of_samples; i++) {
        fprintf(
            f_dif_train, "\n%d %d", i + 1, dataset_train->samples[i].class);
        for (f = 0; f < dataset_train->number_of_features; f++) {
            value = (dataset_train->samples[i].features[f] - min[f])
                / (max[f] - min[f]);
            fprintf(f_dif_train, " %f", value * aux[f]);
        }
    }

    fprintf(f_dif_test, "%d %d %d", dataset_test->number_of_samples,
        dataset_test->number_of_classes, dataset_test->number_of_features);
    for (i = 0; i < dataset_test->number_of_samples; i++) {
        fprintf(f_dif_test, "\n%d %d", i + 1, dataset_test->samples[i].class);
        for (f = 0; f < dataset_test->number_of_features; f++) {
            value = (dataset_test->samples[i].features[f] - min[f])
                / (max[f] - min[f]);
            fprintf(f_dif_test, " %f", value * aux[f]);
        }
    }
    fclose(f_dif_test);
    fclose(f_dif_train);

    for (f = 0; f < dataset_test->number_of_features; f++) {
        fprintf(stderr, "\n [%d] : %f", f, aux[f]);
    }
}

double distanceFEM_feature(double a, double b)
{
    return sqrt(pow(a - b, 2.0));
}

double FEMShepardMotherFunction_feature(
    double a, double b, double additional_parameters[])
{
    return (1.0 / pow(pow(distanceFEM_feature(a, b) + 0.00000001, 1.0), (double)additional_parameters[0]));
}

double FEMGaussianNormalizedMotherFunction_feature(
    double a, double b, double additional_parameters[])
{
    double r = distanceFEM_feature(a, b) / additional_parameters[0];
    return 2.0 * r * r * r - 3.0 * r * r + 1;
}

double FEMRadialNormalizedMotherFunction_feature(
    double a, double b, double additional_parameters[])
{
    double r = distanceFEM_feature(a, b) / additional_parameters[0];
    return exp(-0.5 * r * r);
}
