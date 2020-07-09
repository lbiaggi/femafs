#include "FEMUtils.h"


double distanceFeatures(double a[], double b[], int number_of_features)
{
  int i = 0;
  double dist = 0.0;
  
  for(i = 0; i < number_of_features; i++)
  {
    dist  += pow(a[i] - b[i],2.0);
  }  
  return sqrt(dist);
}  

double distanceFEMSample(FEMSample a, FEMSample b)
{
  int i = 0;
  int  number_of_features = a.number_of_features;
  double dist = 0.0;
  
  for(i = 0; i < number_of_features; i++)
  {
    dist  += pow(a.features[i] - b.features[i],2.0);
  }  
  return sqrt(dist);
} 


void quickSort( double dist[], int index[], int left, int rigth)
{
   int j;

   if( left < rigth ) 
   {
   	// divide and conquer
       j = partition( dist, index, left, rigth);
       quickSort( dist, index, left, j-1);
       quickSort( dist, index, j+1, rigth);
   }	
}

int partition( double dist[], int index[], int left, int rigth) 
{
   int  i, j, index_temp;
   double pivot,temp;
   pivot = dist[left];
   i = left; j = rigth+1;
		
   while(1)
   {
   	do ++i; while( dist[i] <= pivot && i <= rigth );
   	do --j; while( dist[j] > pivot );
   	if( i >= j ) break;
   	temp = dist[i]; index_temp = index[i]; 
	dist[i] = dist[j]; index[i] = index[j];  
	dist[j] = temp; index[j] = index_temp;
   }
   temp = dist[left]; index_temp = index[left];
   dist[left] = dist[j]; index[left] = index[j];
   dist[j] = temp; index[j] = index_temp;
   return j;
}

void bubleSort(double dist[], int index_closer[], int number_of_points)
{
  int i,j;
  
  for(i = 0; i < number_of_points-1; i++)
  {
    for(j = i+1; j < number_of_points; j++)
    {
	if(dist[i] > dist[j])
	{
	  double daux = dist[i];
	  int iaux = index_closer[i];	  
	  
	  dist[i] = dist[j]; dist[j] = daux;
	  index_closer[i] = index_closer[j]; index_closer[j] = iaux;	  
	} 	  
    }  
  }  
  return;
}  


void computeDistanceDatasetFromSample(FEMDataset *dataset, FEMSample sample)
{
  int i = 0;
  
  for(i = 0; i < dataset->number_of_samples; i++)
  {
    dataset->samples[i].dist = distanceFEMSample(dataset->samples[i],sample);
  }  
  return;
}  

void closerSamplesPerClass(FEMDataset dataset, FEMSample sample, FEMDataset *closer, int number_of_points, int class)
{
  int i = 0, j = 0;
  double *dist;
  int    *index;
  int position = 0;
  
  dist = (double*)malloc(sizeof(double)*dataset.samples_per_class[class]);
  index = (int*)malloc(sizeof(int)*dataset.samples_per_class[class]);
  
  alloc_FEMDataset(closer,dataset.number_of_classes,number_of_points,dataset.number_of_features);
  
  for(i = 0; i < dataset.samples_per_class[class]; i++)
  {  
      dist[i] = DBL_MAX;      
  }
  
  int count = 0;
  
  for(i = 0; i < dataset.number_of_samples; i++)
  {
    if(dataset.samples[i].class == class)
    {  
      dist[count] = dataset.samples[i].dist;
      index[count] = i;
      count++;
    } 
  }  
  
  if(count < number_of_points)
  {
    fprintf(stderr,"\nERROR: CloserFEMSampleByClass(). Illegal number of points: %d: %d class:%d",number_of_points,count,class);
    exit(1);  
  }  
  
  quickSort(dist,index,0,dataset.samples_per_class[class]-1);
  //bubleSort(dist,index,dataset.samples_per_class[class]);
  
  for(i = 0; i < number_of_points; i++)
  {
    closer->samples[i].class = dataset.samples[index[i]].class;
    closer->samples[i].features = dataset.samples[index[i]].features;    
  }  
  
  free(dist);
  free(index);
}  

void mergeCloserFEMSamplesToDataset(FEMDataset *dataset, FEMDataset samples[], int number_of_classes, int number_of_points, int number_of_features)
{
  int i = 0, j = 0, count = 0;
  int total_points = number_of_classes*number_of_points;  
  
  alloc_FEMDataset(dataset,number_of_classes,total_points,number_of_features);    
  
  for(i = 0; i < number_of_classes; i++)
  {
      for(j = 0; j < number_of_points; j++)
      {
	if(count > total_points)
	{
	  fprintf(stderr,"\n ERRO: mergeCloser -> count (%d) > total (%d)",count,total_points);
	  exit(1);
	}  
	dataset->samples[count].class = samples[i].samples[j].class;
	dataset->samples[count].features = samples[i].samples[j].features;	
	count++;	
      }
  }  
}  

void identity_matrix(double **identity,int n)
{
	int i,j;
	
	for (i = 0; i < n; ++i)
	  for (j = 0; j < n; ++j)
		if (i == j)
		  identity[i][j] = 1.0;
		else 
		  identity[i][j] = 0.0;
} 

/*** Returns a value of 1 if mat is invertable. ***/
/*** Returns a value of 0 if mat is not invertable. ***/
int invert_matrix (double **mat,double **inv, int n)
{     

	double	m;
	int	i,j,col,finder = 0;
	
	double **tmp = (double**)malloc(sizeof(double*)*n);    
	for(i = 0; i < n; i++)
	{
	  tmp[i] = (double*)malloc(sizeof(double)*n); 
	} 
	
	if(*tmp == NULL)
	{
	 fprintf(stderr,"\nERROR: - FEMUtils.c  - invert_matrix(%d,%d) - ALLOCATION ",n,n);
	 exit(1);
	} 

	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j)
			tmp[i][j] = mat[i][j];//temp[i*N + j] = mat[i*N + j];

	identity_matrix(inv,n);

	for (i = 0; i < n; ++i)
	{	
	    finder = i;
	    /*** Find a row with a non-zero ***/
	    /*** ith element and add it to  ***/
	    /*** the ith row.               ***/
	    if (tmp[i][i] == 0.0)	
	    {	
	      while ( (finder < n) && (tmp[finder][i] == 0.0) )
		  ++finder;
	      if (finder < n)
		for (col = 0; col < n; ++col)
		{	
		     tmp[i][n] += tmp[finder][col]; //temp[i*N + col] += temp[finder*N+col];
		     inv[i][col] += inv[finder][col]; // inv[i*N+col] += inv[finder*N+col];
		}
	     }
	     if (finder < n)
	     {	
	       m = tmp[i][i]; //temp[i*N+i];
	       for (col = 0; col < n; ++col)
	       {	
		 tmp[i][col] /= m; //temp[i*N+col] /= m;
		 inv[i][col] /= m; //inv[i*N+col] /= m;
	       }
	       for (j = i+1; j < n; ++j)
	       {	
		 m = -tmp[j][i]; //temp[j*N+i];
		 for (col = 0; col < n; ++col)
		 {	
		    tmp[j][col] += (tmp[i][col]*m); //temp[j*N+col] += ( temp[i*N+col] * m );
		    inv[j][col] += (inv[i][col]*m); //inv[j*N+col] += ( inv[i*N+col] * m );
		 }
	       }
	      }
	      else 
	      {
		printf ("\nMatrix uninvertable.\n");	  
		printMatrix(mat,n,"Error invert_matrix");
		exit(1);
	      } 	
	}

	if (finder < n)
	for (i = 0; i< n; ++i)
	  for (j = i+1; j < n; ++j)
	  {	
	    m = -tmp[i][j];//temp[i*N+j];
	    for (col = 0; col < n; ++col)
	    {	
	      tmp[i][col] += (tmp[j][col]*m); //temp[i*N+col] += ( temp[j*N+col] * m );
	      inv[i][col] += (inv[j][col]*m); //inv[i*N+col] += ( inv[j*N+col] * m );
	    }
	  }


	if (finder < n)
	    return (1);
	else 
	    return (0);

} /*** End invert_matrix () ***/

double* mulMatrixVetor(double **a, double *b, int n, int m)
{
 int i,j;
 double *c = (double*)malloc(sizeof(double)*n); 
 
 //inicializa o vetor auxiliar 
 for(i = 0; i < n; i++)
  c[i] = 0.0;   
 
 //calcula a multiplicacao  
 for(i = 0; i < n; i++)
  for(j = 0; j < m; j++)
  {    
    c[i] += a[i][j]*b[j];
  }
  return c;
}

double* samplesValuesToVector(FEMDataset *dataset)
{
  int i = 0;
  double *vec = (double*)malloc(sizeof(double)*dataset->number_of_samples);
  
  for(i = 0; i < dataset->number_of_samples; i++)
    vec[i] = dataset->samples[i].value;
  
  return vec;
}

void vectorToSampleValues(FEMDataset *dataset, double *vec)
{
  int i = 0;
  
  for(i = 0; i < dataset->number_of_samples; i++)
    dataset->samples[i].weigth = vec[i];
}

void printMatrix(double **m, int n, char tag[])
{
  int i,j;
  fprintf(stderr,"\n *** %s *** \n",tag);
  for(i = 0; i < n; i++)
  {
    for(j = 0; j < n; j++)
    {
      fprintf(stderr,"%.4f ",m[i][j]);
    }
    fprintf(stderr,"\n");
  }
}

void printVector(double *v, int n, char tag[])
{
  int i,j;
  fprintf(stderr,"\n *** %s *** \n",tag);
  for(i = 0; i < n; i++)
  {    
      fprintf(stderr,"%.4f ",v[i]);    
  }
  fprintf(stderr,"\n");
}