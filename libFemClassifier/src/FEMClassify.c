#include "FEMClassify.h"


void SetProbabilityByClass(FEMDataset *dataset, FEMSample sample, int class)
{
  int i = 0;
  double sum = 0.0;
  
  for(i = 0; i < dataset->number_of_samples; i++)
  {
    if(dataset->samples[i].class == class)
    {
      dataset->samples[i]. value = 1.0;      
    }  
    else
    {
      dataset->samples[i]. value = 0.0;      
    }    
  }  
}


void SetValueWeigthByClass(FEMDataset *dataset, FEMSample sample, double additional_parameters[], int class, motherFunction *FEMbasis)
{
  int i = 0;
  double sum = 0.0;
  
  for(i = 0; i < dataset->number_of_samples; i++)
  {
    if(dataset->samples[i].class == class)
    {
      dataset->samples[i]. value = 1.0;      
    }  
    else
    {
      dataset->samples[i]. value = 0.0;      
    }
    dataset->samples[i].weigth = FEMbasis(dataset->samples[i],sample,additional_parameters);
    sum += dataset->samples[i].weigth;
  } 
  
  //normalize the basis
  for(i = 0; i < dataset->number_of_samples; i++)
    dataset->samples[i].weigth /= sum; 
  
}



double probabilityByClass(FEMDataset *dataset, int class, FEMSample sample, double additional_parameters[], motherFunction *FEMbasis)
{
  double probability = 0.0;
  int  i = 0;
  
  if(*FEMbasis == FEMShepardMotherFunction)
    SetValueWeigthByClass(dataset,sample,additional_parameters,class,FEMbasis);  
  if(*FEMbasis == FEMRadialNormalizedMotherFunction)
  {
    SetProbabilityByClass(dataset,sample,class);
    double *y = samplesValuesToVector(dataset);
    double **z = FEMRadialcreateZMatrix(dataset,additional_parameters);
    double **inv = (double**)malloc(sizeof(double*)*dataset->number_of_samples);    
    for(i = 0; i < dataset->number_of_samples; i++)
    {
      inv[i] = (double*)malloc(sizeof(double)*dataset->number_of_samples); 
    } 
    
    int isInversible =  invert_matrix (z,inv,dataset->number_of_samples);
    if(isInversible == 0)
    {
      fprintf(stderr,"ERROR - probabilityByClass() - Matrix is not inversible");
      exit(1);
    }
    //printVector(y,dataset->number_of_samples,"y");
    //printMatrix(z,dataset->number_of_samples,"z");
    //printMatrix(inv,dataset->number_of_samples,"z^{-1}");
    double *a = mulMatrixVetor(inv,y,dataset->number_of_samples,dataset->number_of_samples);
    vectorToSampleValues(dataset,a);
    //printVector(a,dataset->number_of_samples,"a");
    //int pq; fprintf(stderr,"\n *** \n"); scanf("%d",&pq);
    for(i = 0; i < dataset->number_of_samples; i++)
    {
      free(inv[i]); free(z[i]);
    }
    free(inv); free(z); free(a); free(y);
    
    
  }
  
  for(i = 0; i < dataset->number_of_samples; i++)
  {
    probability += dataset->samples[i].value * dataset->samples[i].weigth;    
  }
  return probability;
}  

double FEMClassify(FEMDataset *dataset, int number_of_classes, int *class, FEMSample sample, double additional_parameters[], motherFunction *FEMbasis)
{
  double *probability;
  double certain = 0.0;
  double max_probability = 0.0;
  double sum = 0.0;
  int i = 0;
  
  
  probability = (double*)malloc(sizeof(double)*number_of_classes);  
  for(i = 0; i < dataset->number_of_classes; i++)
  {  
    probability[i] = probabilityByClass(dataset,i+1,sample,additional_parameters,FEMbasis);
    sum += probability[i];
    
    if(probability[i] > max_probability)
    {
      max_probability = probability[i];
      *class = i+1;
    }
  }  
 
  certain = max_probability/sum;
  if(*class == 0)
  { 
    fprintf(stderr,"\n Not_predicted:%d",*class);    
    exit(1);
  }
  
  free(probability);
  
  return certain;   
  
}  