#include <FEMimages.h>

void printPoints(char name_img[], FEMDataset *dataset, int nx, int ny)
{
  FILE *img = fopen(name_img,"w");
  fprintf(img,"P3\n");
  fprintf(img,"%d %d 255\n",nx,ny);  
  
  int i,j,k;
  int count = 0;
  
  for(i = 0; i < nx; i++)
  {
    
    for(j = 0; j < ny; j++)
    {      
        int print = 0;
	for(k = 0; k < dataset->number_of_samples; k++)
	{
	  if(i == dataset->samples[k].features[0] && j == dataset->samples[k].features[1] || 
	    i == dataset->samples[k].features[0]-1 && j == dataset->samples[k].features[1] ||
	    i == dataset->samples[k].features[0]+1 && j == dataset->samples[k].features[1] ||
	    i == dataset->samples[k].features[0]   && j == dataset->samples[k].features[1]-1 ||
	    i == dataset->samples[k].features[0]   && j == dataset->samples[k].features[1]+1 ||
	    i == dataset->samples[k].features[0]-1 && j == dataset->samples[k].features[1]-1 ||
	    i == dataset->samples[k].features[0]-1 && j == dataset->samples[k].features[1]+1 ||
	    i == dataset->samples[k].features[0]+1 && j == dataset->samples[k].features[1]-1 ||
	    i == dataset->samples[k].features[0]+1 && j == dataset->samples[k].features[1]+1)	  
	    print = dataset->samples[k].class;
	}
	
	switch(print)
	{
	  case 0: fprintf(img,"255 255 255 "); break;
	  case 1: fprintf(img,"255 0 0 "); break;
	  case 2: fprintf(img,"0 255 0 "); break;
	  case 3: fprintf(img,"0 0 255 "); break;
	  case 4: fprintf(img,"255 255 0 "); break;
	  case 5: fprintf(img,"0 255 255 "); break;
	}
	/*if(print == 0)
	  fprintf(img," %d %d %d ",255,255,255);
	else
	  fprintf(img," %d %d %d ",(int)print*(255/9),(int)print*(255/9),(int)print*(255/9));
	*/
	
	count++;
	if(count == 8)
	{
	  fprintf(img,"\n");
	  count = 0;
	}      
    }
  }  
  fclose(img);
}  

