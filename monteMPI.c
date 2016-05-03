#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
      
   long long int n;          /* Total number of trapezoids    */

   int id, processnum, process;
   MPI_Status status;
   int master=0;
   int tag=123;
   int mycount=0;
   long long int hits;

   if(argc<=1){
    fprintf(stderr,"Pass the number of tosses as an arg\n");
    return 0;
   }


   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD,&processnum);
   MPI_Comm_rank(MPI_COMM_WORLD,&id);
   
      if(id==0){  n=100;
	//	 sscanf(argv[1],"%llf",&n);
	n = strtol(argv[1], NULL, 10);
	for(process=1;process<processnum;process++){
	 MPI_Send(&n,1,MPI_LONG_LONG,process,tag,MPI_COMM_WORLD);
	 }
       }
       else{
	 MPI_Recv(&n,1,MPI_LONG_LONG,master,tag,MPI_COMM_WORLD,MPI_STATUSES_IGNORE);
	 
		  }
    double  h, x, my_result;
    double  local_a, local_b;
    int  i, local_n;

    //perform calculation                                                                                       
    //  h = (b-a)/n;                                                                                           
 
    double a,b;
    double distance;
    // mycount=0;
    // hits=0; //number of hits
    srand(id*time(NULL));
    for (i = 0; i < n; i++) {
      a = (rand()%20001);
      a=(a/10000) -1;
      b= rand()%20001;
      b=b/10000 -1;
      distance= (a*a+b*b);

      if(distance<=1){
	mycount++;

      }
    }
   
    /* if(id==0){
    hits=mycount;
          for(process=1;process<processnum;process++){
          MPI_Recv(&mycount,1,MPI_REAL,process,tag,MPI_COMM_WORLD,&status);
          hits+=mycount;
        }

        double pi=4*(hits/(double)(n*processnum));

   printf("With n = %d tosses, our estimate\n", n*processnum);
   printf("of pi  = %.14e\n",pi);
      }*/
      if(id!=0){
        printf("slave %d sending hits: %d to master\n",id,mycount);
        MPI_Send(&mycount,1,MPI_DOUBLE,master,tag,MPI_COMM_WORLD);
      }
      else{
	hits=mycount;
	printf("In the master\n");
	for(process=1;process<processnum;process++){
          MPI_Recv(&mycount,1,MPI_DOUBLE,process,tag,MPI_COMM_WORLD,MPI_STATUSES_IGNORE);
          hits+=mycount;
        }

        double pi=4*(hits/(double)(n*processnum));

	printf("With n = %d tosses, our estimate\n", n*processnum);
	printf("of pi  = %.14e\n",pi);
      }
      // printf("a is %f b is %f hits is %lld distance is %f\n",a,b,hits,distance);                         

    
    
    //   printf("pi is %f hits %lld n %lld\n",pi,hits,n);                                                    
   MPI_Finalize();



   return 0;
}  /* main */




