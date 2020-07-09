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

int main(int argc, char **argv)
{
  int number_of_samples, number_of_classes, number_of_features;
  int i,j;
  FILE *f_opf = fopen(argv[1],"r");
  FILE *f_fem = fopen(argv[2],"w");
  
  fscanf(f_opf,"%d %d %d",&number_of_samples, &number_of_classes, &number_of_features);
  fprintf(f_fem,"%d %d %d\n",number_of_classes, number_of_samples, number_of_features);
  fprintf(stderr,"classes:%d samples:%d features:%d\n",number_of_classes, number_of_samples, number_of_features);
  
  for(i = 0; i < number_of_samples; i++)
  {
    int id;
    int class;
    double feature;
    
    fscanf(f_opf,"%d %d",&id, &class);
    fprintf(f_fem,"%d ",class);
    for(j = 0; j < number_of_features; j++)
    {
      fscanf(f_opf,"%lf",&feature);
      fprintf(f_fem,"%lf ",feature);
    } 
    //fscanf(f_opf,"%lf",&feature);
    fprintf(f_fem,"\n");
  }  
  fclose(f_fem);
  fclose(f_opf);
  
  return 0;
}  
