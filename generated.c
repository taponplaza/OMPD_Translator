#include <assert.h>
#include <mpi.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<omp.h>
#include<complex.h>
#include<tgmath.h>
#include <sys/time.h>

int __taskid = -1, __numprocs = -1;


#define DIM 8192

typedef struct{
	unsigned char bl,gr,re;
} color;

MPI_Datatype MPIcolor_t;

void __Declare_MPI_Type_color() {
    int blocklengths[3];
    MPI_Datatype old_types[3];
    MPI_Aint disp[3];
    MPI_Aint lb;
    MPI_Aint extent;
    blocklengths[0] = 1;
    blocklengths[1] = 1;
    blocklengths[2] = 1;
    old_types[0] = MPI_UNSIGNED_CHAR;
    old_types[1] = MPI_UNSIGNED_CHAR;
    old_types[2] = MPI_UNSIGNED_CHAR;
    MPI_Type_get_extent(MPI_UNSIGNED_CHAR, &lb, &extent);
    disp[0] = lb;
    MPI_Type_get_extent(MPI_UNSIGNED_CHAR, &lb, &extent);
    disp[1] = disp[0] + extent;
    MPI_Type_get_extent(MPI_UNSIGNED_CHAR, &lb, &extent);
    disp[2] = disp[1] + extent;
    MPI_Type_create_struct(3, blocklengths, disp, old_types, &MPIcolor_t);
    MPI_Type_commit(&MPIcolor_t);
}

void Declare_MPI_Types() {
    __Declare_MPI_Type_color();
    return;
}













void tga_write ( int w, int h, color rgb[], char *filename );

color fcolor(int iter,int num_its){
        color c;


        c.re = (iter*20+0)%255;
        c.gr = (iter*20+85)%255;
        c.bl = (iter*20+170)%255;
        return c;
}
int explode (float _Complex z0, float _Complex c, float radius, int n)
{
int k=1;
float modul;

z0 = (z0*z0)+c;
modul = cabsf(z0);

while ((k<=n) && (modul<=radius)){ 
		z0 = (z0*z0)+c;
		modul = cabsf(z0);
                k++;
}
return k;
}
float _Complex mapPoint(int width,int height,float radius,int x,int y){
	float _Complex c;
	int l = (width<height)?width:height;
	float re = 2*radius*(x - width/2.0)/l;
        float im = 2*radius*(y - height/2.0)/l;
	c = re+im*I;
        return c;
}
color *juliaSet(int width,int height,float _Complex c,float radius,int iter){
	int x,y,i;
	float _Complex z0;
	int k=0;
	int count=0;

	color *rgb;

if (__taskid == 0) {
	rgb = calloc (width*height, sizeof(color));
 
}
// pragma aqui
// pragma aqui
// pragma aqui
















if (__taskid == 0) {
}
	return rgb;
}
void tga_write ( int w, int h, color rgb[], char *filename )
{
  FILE *file_unit;
  unsigned char header1[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
  unsigned char header2[6] = { w%256, w/256, h%256, h/256, 24, 0 };

  file_unit = fopen ( filename, "wb" );

  fwrite ( header1, sizeof ( unsigned char ), 12, file_unit );
  fwrite ( header2, sizeof ( unsigned char ), 6, file_unit );

  fwrite ( rgb, sizeof ( unsigned char ), 3 * w * h, file_unit );

  fclose ( file_unit );

  printf ( "\n" );
  printf ( "TGA_WRITE:\n" );
  printf ( "  Graphics data saved as '%s'\n", filename );

  return;
}
int main(int argc, char* argv[])
{
int width, height;
float _Complex c;
color *rgb;

#ifdef _OPENMP
double start_time, end_time;
#else
struct timeval tv_start, tv_end;
float tiempo_trans;
#endif

 
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD,&__numprocs);
MPI_Comm_rank(MPI_COMM_WORLD,&__taskid);


Declare_MPI_Types();

if (__taskid == 0) {
	if(argc != 6) {
		printf("Uso : %s\n", "<dim de la ventana, partes real e imaginaria de c, radio, iteraciones>");
		exit(1);
	}

		width = atoi(argv[1]);
		height = width; 
		if (width >DIM) {
                   printf("El tamanyo de la ventana deben ser menor que 1024\n");
                   exit(1);
                }
		float re = atof(argv[2]);
                float im = atof(argv[3]);

                c=re+im*I;

	printf("JuliaSet: %d, %d, %f, %f, %f, %d\n", width, height,creal(c),cimag(c),atof(argv[4]),atoi(argv[5]));
#ifdef _OPENMP
	start_time = omp_get_wtime();
#else
	gettimeofday(&tv_start, NULL);
#endif
}
	rgb = juliaSet(width,height,c,atof(argv[4]), atoi(argv[5]));


if (__taskid == 0) {
#ifdef _OPENMP
	end_time = omp_get_wtime();
	printf ( "Tiempo Julia = %f segundos\n",end_time-start_time);
#else
	gettimeofday(&tv_end, NULL);
	tiempo_trans=(tv_end.tv_sec - tv_start.tv_sec) * 1000000 +
	  (tv_end.tv_usec - tv_start.tv_usec); 
	printf("Tiempo Julia = %f segundos\n", tiempo_trans/1000000);
#endif
	

	tga_write ( width, height, rgb, "julia_set.tga" );

  	printf ( "\n" );
  	printf ( "JULIA_SET. Finalizado\n");

  	free(rgb);

}
MPI_Finalize();
  	return 0;
}
