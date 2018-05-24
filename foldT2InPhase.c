#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//#include"sigproc.h"
//#include"header.h"
#include "fold.h"
#include "tempo2pred.h"
#include "tempo2pred_int.h"

void foldT2_help()
{
  puts("");
  puts("foldT2InPhase: folds the sigproc timeseries file in phase ");
  puts("usage: foldT2InPhase [infile] \n -o [profFilename]\n -n [number of bins]\n -p [tempo2 polyco or predictor]");
  puts(" -pred (use this flag only for predictors)\n");
  puts("");
}

int main (int argc, char *argv[])
{
  /* local variables */
  T2Predictor t2p;
  int i,opened_input=0,opened_output=0;
  char string[80];
  int nfiles;
  FILE *fp, *fpPoly;
  char output[200];
  float *buffer,*prof;
  long double psec, phase, offset,frequency;
  double polyco_period( double mjd, struct POLYCO polyco);
  struct POLYCO polyco;
  int count =0,bins=100,fileFlag=0,ibin=0;
  long int numSamples=0,totalSamples=0,nread,*cbin,start,end,fileSize;
  long double *tempProf;
  int pred=0;
  long double mjd;
   
  T1Polyco *t1p;
  T1PolycoSet *t1ps;
  
  
  if (argc<2) {
    foldT2_help();
    exit(0);
  }
  offset=0.0;

  strcpy(polyco_file,"polyco_new.dat");
   
  /* check the command line parameters */
  i=1;
  while (i<argc) 
  {
    if (strings_equal(argv[i],"-p")) 
    {
      i++;
      fileFlag=1;
      if (file_exists(argv[i])) 
      {
	      strcpy(polyco_file,argv[i]);
	      folding_period=-1.0;
      }
    } 
    else if (strings_equal(argv[i],"-n")) 
    {
      i++;
      fileFlag=1;
      bins=atoi(argv[i]);
    } 
    else if(file_exists(argv[i])&& fileFlag==0)
    {
      nfiles++;
    }
    else if (strings_equal(argv[i],"-o")) 
    {
      i++;
      fileFlag=1;
      strcpy(output,argv[i]);
      
    }
    else if (strings_equal(argv[i],"-pred")) 
    {
      i++;
      fileFlag=1;
      pred=1;
      //strcpy(output,argv[i]);
      
    } 
    i++;
  }
  
  tempProf = (long double*) malloc(bins*sizeof(long double));
  prof = (float*) malloc(bins*sizeof(float));
  cbin = (long int*) malloc(bins*sizeof(long int));
  t1ps = (T1PolycoSet*) malloc(sizeof(T1PolycoSet));
  t1p = (T1Polyco*) malloc(sizeof(T1Polyco));
  for(i=0;i<bins;i++)
  {
    tempProf[i]=0.0;
    prof[1]=0.0;
    cbin[i]=0;
  }
  
  buffer = (float*)malloc(100000*sizeof(float));
    
  if(pred==1) T2Predictor_Read(&t2p,polyco_file);
  else 
  {
    
    fpPoly = fopen(polyco_file,"r");
    if(fpPoly==NULL)
    {
      printf("cannot open the polyco file \n");
      exit(0);
    }
    printf("Able to open the file \n");
    T1PolycoSet_Read(t1ps, fpPoly);
    
    memcpy(t1p,&t1ps->segments[0],sizeof(T1Polyco));
  }
  
  while(count < nfiles)
  {
    fp = fopen(argv[count+1],"rb");
    read_header(fp);
    printf(" The number of files are %d %5.20lf\n",nfiles,tstart);
    start = ftell(fp);
    fseek(fp,0,SEEK_END);
    end = ftell(fp);
    fileSize = (end -start)/sizeof(float);
    fseek(fp,0,SEEK_SET);
    read_header(fp);
    mjd = (long double) tstart;
    printf("%5.20Lf %lf\n",mjd,tsamp);
    while(!feof(fp))
    {
      nread=fread(buffer,sizeof(float),100000,fp);
      for(i=0;i<nread;i++)
      {
        if(pred!=1)
        {
           
          phase = T1PolycoSet_GetPhase(t1ps,mjd+(numSamples*tsamp/86400),t1p->frequency_obs);
          frequency = T1PolycoSet_GetFrequency(t1ps,mjd+(numSamples*tsamp/86400),t1p->frequency_obs);
          psec = 1/frequency;
        }
        else
        {
          phase = (long double)T2Predictor_GetPhase(&t2p,mjd+(numSamples*tsamp/86400),334.5);
          frequency = (long double)T2Predictor_GetFrequency(&t2p,mjd+(numSamples*tsamp/86400),334.5);
          psec = 1/frequency;
        }
        phase=phase-floor(phase);
        ibin = floor((double)bins*phase);
        tempProf[ibin] += (long double)buffer[i];
        cbin[ibin] +=1;
        numSamples++;
        totalSamples++;
      }
      printf("percentage = %ld %%\r",(numSamples*100)/fileSize);
      fflush(stdout);
    }
    numSamples=0;
    printf("%12.9Lf (sec) for mjd = %18.12lf Phase: %.12Lf\n", psec,tstart,phase);
    count++;
    fclose(fp);
  }
  fp = fopen(output,"w");
  if(fp==NULL)
  {
    printf("Cannot open the output file please check \n");
    exit(0);
  }
  fprintf(fp,"# %5.20Lf %d \n",mjd,bins);
  for(i=0;i<bins;i++)
  {
    prof[i] = (float)(tempProf[i]/(long double)cbin[i]);
    fprintf(fp,"%f \n",prof[i]);
  }
  
  exit(0);
  return(0);
}   
