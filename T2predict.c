#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tempo2pred.h"
#include "tempo2pred_int.h"


int main(int argc, char *argv[])
{
   T2Predictor t2p;
   int ret;
   double start,end;
   double phase,frequency;
   int npts=100;
   long double mjd;
   int i;
   double f;
   char filename[100];
   if(argc !=8)
   {
      printf("\n please check options ");
   }
   //mjd = atof(argv[1]);
   for(i=0;i<argc;i++)
   {
      if(strcmp(argv[i],"-p") == 0)
      {
         i++;
         strcpy(filename,argv[i]);
      }  
      
      if(strcmp(argv[i],"-f")==0)
      {
         i++;
         f = atoi(argv[i]);
      } 
      
      if(strcmp(argv[i],"-mjd") == 0)
      {
         i++;
         mjd = atoi(argv[i]);
      }
   }
   ret = T2Predictor_Read(&t2p,filename);
  

   printf("Hello %d\n",ret);
   printf("Site = %s\n",T2Predictor_GetSiteName(&t2p));
   start = (double)T2Predictor_GetStartMJD(&t2p);
   end = (double)T2Predictor_GetEndMJD(&t2p);
   if(mjd < start || mjd > end) 
   {
      printf("mjd is not in range of the predicter");
      exit(0);
   }
   printf("Start = %.10f, end = %.10f\n",start,end);
  
   mjd = start + (end-start)/(double)npts*i;
   phase = (double)T2Predictor_GetPhase(&t2p,mjd,f);
   frequency = (double)T2Predictor_GetFrequency(&t2p,mjd,f);
   printf("Result = %.15f %.15f \n",i,(double)phase,1/frequency);
  
}
