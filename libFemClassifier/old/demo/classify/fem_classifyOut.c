#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>

#include <FEMDataset.h>
#include <FEMUtils.h>
#include <FEMClassify.h>
#include <FEMBasis.h>
#define MAX_CLASSES 100
#define SHEPARD_BASIS 0
#define RADIAL_BASIS  1
#define _PAUSE int pause; fprintf(stderr,"Aqui"); scanf("%d",&pause);


int main(int argc, char **argv)
{
  FEMDataset dataset_train;
  FEMDataset dataset_test;
  FEMDataset dataset_reconstruction;
  FEMDataset dataset_closer[MAX_CLASSES];  
    
  int basis_id = atoi(argv[1]);
  motherFunction *basis = NULL;
  
  if(basis_id == SHEPARD_BASIS)
    basis = &FEMShepardMotherFunction;
  else
    basis = &FEMRadialNormalizedMotherFunction;
  
    
  double additional_parameters[MAX_ADDITIONAL_PARAMETERS];
  clock_t begin, end;
  double time_spent;

  begin = clock();
  
  fprintf(stderr,"\n ./program <basis> <train_file> <test_file> <number_of_points_neigbhor> <aditional_params[0]> ..... <aditional_params[n]>\n\n");  
  
  readDataset(&dataset_train,argv[2]);
  readDataset(&dataset_test,argv[3]);    
  
  fprintf(stderr,"\n[Dataset: %s]Samples:%d classes:%d features %d",argv[2],dataset_train.number_of_samples, dataset_train.number_of_classes, dataset_train.number_of_features);
  fprintf(stderr,"\n[Dataset: %s]Samples:%d classes:%d features %d\n",argv[3],dataset_test.number_of_samples, dataset_test.number_of_classes, dataset_test.number_of_features);    
  
  int number_of_points = atoi(argv[4]);
  int class_predict = 0;
  int i,k;
  
  if(number_of_points == 0) //compute the best number_of_points
  {  
    number_of_points = dataset_train.samples_per_class[1];
    for(i = 1; i <= dataset_train.number_of_classes; i++)
    {  
      if(dataset_train.samples_per_class[i] < number_of_points)
	number_of_points = dataset_train.samples_per_class[i];
    }
  }  
  fprintf(stderr,"\n number_of_points:%d",number_of_points);
//  int step = (int)(255/dataset_train.number_of_classes); //to color the classes
  long int count = 0;  
  long int sample_number = 0;
  long int hit = 0;
  long int miss = 0;
  
  for(i = 5; i < argc; i++)
    additional_parameters[i-5] = atof(argv[i]);
  
  char predictfilename[256];
  sprintf(predictfilename,"%s.predict",argv[3]);
  FILE *predict_file = fopen(predictfilename,"w");

    
  for(i = 0; i < dataset_test.number_of_samples; i++)
  {   
      
      computeDistanceDatasetFromSample(&dataset_train,dataset_test.samples[sample_number]);	
      
      for(k = 1; k <= dataset_train.number_of_classes; k++)
      	closerSamplesPerClass(dataset_train,dataset_test.samples[sample_number],&(dataset_closer[k-1]),number_of_points,k);
        
      mergeCloserFEMSamplesToDataset(&dataset_reconstruction,dataset_closer,dataset_train.number_of_classes,number_of_points,dataset_train.number_of_features);
      
      double certain = FEMClassify(&dataset_reconstruction,dataset_train.number_of_classes,&class_predict,dataset_test.samples[sample_number],additional_parameters,basis);
      //int color = (int)(class_predict-1)*step;     
            
      if(dataset_test.samples[sample_number].class == class_predict) hit++;
      else							   miss++;
      fprintf(predict_file,"%d\n",class_predict);
      sample_number++;   
  }  
  fclose(predict_file);
  
  fprintf(stderr,"\nTotal samples:%ld Hit:%ld (%.2f%%) Miss:%ld (%.2lf%%)",sample_number,hit,100*(double)hit/(double)sample_number,miss,100*(double)miss/(double)sample_number);  
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  fprintf(stderr, "\n Estimation time: %f seconds\n", time_spent);
  
  char timefilename[256];
  sprintf(timefilename,"%s.time",argv[3]);
  FILE *time_file = fopen(timefilename,"a");
  fprintf(time_file,"%f\n", time_spent);
  fclose(time_file);
  
  char accfilename[256];
  sprintf(accfilename,"%s.%d.Kacc",argv[3],(int)additional_parameters[0]);
  FILE *acc_file = fopen(accfilename,"a");
  fprintf(acc_file,"%f\n",100*(double)hit/(double)sample_number);
  fclose(acc_file);
}