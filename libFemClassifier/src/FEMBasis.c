#include <FEMBasis.h>



// double FEMShepardMotherFunction(FEMSample a, FEMSample b, double additional_parameters[])
// {
//   return (1.0/ pow(pow(distanceFEMSample(a,b)+0.00000001,(double)1.0/(double)(b.number_of_features-1)),(double)additional_parameters[0]));
// }  

double FEMShepardMotherFunction(FEMSample a, FEMSample b, double additional_parameters[])
{
  return (1.0/ pow(pow(distanceFEMSample(a,b)+0.00000001,1.0),(double)additional_parameters[0]));
}  


double FEMGaussianNormalizedMotherFunction(FEMSample a, FEMSample b, double additional_parameters[])
{
  double r = distanceFEMSample(a,b)/additional_parameters[0];  
  
  //if(r > additional_parameters[1]) return 0.00000001; 
  return 2.0*r*r*r - 3.0*r*r + 1;  
}  

double FEMRadialNormalizedMotherFunction(FEMSample a, FEMSample b, double additional_parameters[])
{
  double r = distanceFEMSample(a,b)/additional_parameters[0];  
  
  //if(r > additional_parameters[1]) return 0.00000001; 
  return exp(-0.5*r*r);  
}  

double** FEMRadialcreateZMatrix( FEMDataset *dataset, double additional_parameters[])
{
    int i,j;
    
    //allocate matriz Z
    double **Z = (double**)malloc(sizeof(double*)*dataset->number_of_samples);    
    for(i = 0; i < dataset->number_of_samples; i++)
    {
      Z[i] = (double*)malloc(sizeof(double)*dataset->number_of_samples); 
    } 
    
    for(i = 0; i < dataset->number_of_samples; i++)
    {
	for(j = 0; j < dataset->number_of_samples; j++)
	{
	  Z[i][j] = FEMGaussianNormalizedMotherFunction(dataset->samples[i],dataset->samples[j],additional_parameters);
	} 	  
    }  
    return Z;
}  
