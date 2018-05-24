#include <stdio.h>
#include <stdlib.h>
#include "tempo2pred.h"
#include "tempo2pred_int.h"
#include <math.h>
#include<string.h>

int main(int argc , char *argv[])
{
   T1Polyco t2p;
   int ret;
   double start,end;
   double phase,frequency;
   int npts=100;
   double mjd;
   int i;
   FILE *fp;
   for(i=0; i<argc; i++)
   {
      if(strcmp(argv[i],"-mjd")==0)
      {
         i++;
         mjd = atof(argv[i]);
      }
      if(strcmp(argv[i],"-p")==0)
      {
         i++;
         fp = fopen(argv[i],"r");
         if(fp == NULL)
         {
            printf("cannot open the file");
            exit(0);
         } 
      }
   }   
   if(fp == NULL)
   {
      printf("cannot open the file");
      exit(0);
   }    
   
   ret = T1Polyco_Read(&t2p,fp);

   printf("Hello %d %lf\n",ret,mjd);
   //printf("Site = %s\n",T1Polyco_GetSiteName(&t2p));
   //start = (double)T1Polyco_GetStartMJD(&t2p);
   //end = (double)T1Polyco_GetEndMJD(&t2p);
   //printf("Start = %.10f, end = %.10f\n",start,end);
   /*
   for (i=1;i<npts;i++)
   {
      mjd = start + (end-start)/(double)npts*i;
      phase = (double)T1Polyco_GetPhase(&t2p,mjd,1400.0);
      frequency = (double)T1Polyco_GetFrequency(&t2p,mjd,1400.0);
      printf("Result = %d %.15f %.15f %.15f \n",i,(double)mjd-start,phase,frequency);
   }
   */
   //mjd = 56524.786805555555;
   phase = (double)T1Polyco_GetPhase(&t2p,mjd,326.5);
   frequency = (double)T1Polyco_GetFrequency(&t2p,mjd,326.5);
   printf("Result = %d %.15f %.15f %.15f \n",i,(double)mjd,phase,1/frequency);
   fclose(fp);
    
}

 
