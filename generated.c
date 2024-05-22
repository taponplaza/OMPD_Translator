# include <assert.h>
# include <mpi.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <sys/time.h>
# include <omp.h>


int main ( int argc, char *argv[] )




































































































{
# define M 500
# define N 500
# define MASTER 0
#ifdef _OPENMP
  double start_time, run_time;
#else
  struct timeval tv_start, tv_end;
  double run_time;
#endif


  double diff;
  double aux_diff;
  double epsilon;
  int i;
  int iterations;
  int iterations_print;
  int j;
  double mean;
  
  char output_filename[80];
  int success;
  double u[M][N];
  double w[M][N];

  int __taskid = -1, __numprocs = -1;

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD,&__numprocs);
MPI_Comm_rank(MPI_COMM_WORLD,&__taskid);
if (__taskid == 0) {
printf ( "\n" );
  printf ( "HEATED_PLATE <epsilon> <fichero-salida>\n" );
  printf ( "  C/serie version\n" );
  printf ( "  A program to solve for the steady state temperature distribution\n" );
  printf ( "  over a rectangular plate.\n" );
  printf ( "\n" );
  printf ( "  Spatial grid of %d by %d points.\n", M, N );




  epsilon = atof(argv[1]);
  printf("The iteration will be repeated until the change is <= %lf\n", epsilon);
  diff = epsilon;



  success = sscanf ( argv[2], "%s", output_filename );
  if ( success != 1 )
    {
        printf ( "\n" );
        printf ( "HEATED_PLATE\n" );
        printf ( " Error en la lectura del nombre del fichero de salida\n");
        return 1;
    }

 printf("  The steady state solution will be written to %s\n", output_filename);




  mean = 0.0;
  for ( i = 1; i < M - 1; i++ )
  {
  	  w[i][0] = 100.0;
      mean += w[i][0];
  }
  for ( i = 1; i < M - 1; i++ )
  {
    	w[i][N-1] = 100.0;
      mean += w[i][N-1];
  }

  for ( j = 0; j < N; j++ )
  {
    	w[M-1][j] = 100.0;
      mean += w[M-1][j]; 
  }

  for ( j = 0; j < N; j++ )
  {
      w[0][j] = 0.0;
      mean += w[0][j];
  }



  mean = mean / ( double ) ( 2 * M + 2 * N - 4 );

  printf ( "\n" );
  printf ( "  MEAN = %lf\n", mean );


  for ( i = 1; i < M - 1; i++ )
    for ( j = 1; j < N - 1; j++ )
        	w[i][j] = mean;
 


  
  iterations = 0;
  iterations_print = 1;
  printf ( "\n" );
  printf ( " Iteration  Change\n" );
  printf ( "\n" );

#ifdef _OPENMP
  start_time = omp_get_wtime();
#else
  gettimeofday(&tv_start, NULL);
#endif











    
    
    
    





    }
 





















#ifdef _OPENMP
  if (__taskid == 0) {
run_time = omp_get_wtime() - start_time;
#else
  gettimeofday(&tv_end, NULL);
  run_time=(tv_end.tv_sec - tv_start.tv_sec) * 1000000 +
        (tv_end.tv_usec - tv_start.tv_usec); 
  run_time = run_time/1000000; 
#endif


  printf ( "\n" );
  printf ( "  %8d  %lg\n", iterations, diff );
  printf ( "\n" );
  printf ( "  Error tolerance achieved.\n" );
  printf("\n Tiempo version Secuencial = %lg s\n", run_time);



  output = fopen(output_filename, "wt");

  fprintf(output, "%d\n", M);
  fprintf(output, "%d\n", N);

  for ( i = 0; i < M; i++ )
  {
    for ( j = 0; j < N; j++)
    {
	fprintf(output, "%lg ", w[i][j]);
    }
    fprintf(output, "\n");
  }
  fclose(output);

  printf ( "\n" );
  printf ( " Solucion escrita en el fichero %s\n", output_filename );



  printf ( "\n" );
  printf ( "HEATED_PLATE_Serie:\n" );
  printf ( "  Normal end of execution.\n" );

 }
MPI_finalize();
 return 0;
}
