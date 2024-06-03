#include <assert.h>
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int __taskid = -1, __numprocs = -1;



typedef struct {
  float x, y, z;
  double velocity;
  int  n;
  char type;
} Particle;

MPI_Datatype MPIParticle_t;

void __Declare_MPI_Type_Particle() {
    int blocklengths[6];
    MPI_Datatype old_types[6];
    MPI_Aint disp[6];
    MPI_Aint lb;
    MPI_Aint extent;
    blocklengths[0] = 1;
    blocklengths[1] = 1;
    blocklengths[2] = 1;
    blocklengths[3] = 1;
    blocklengths[4] = 1;
    blocklengths[5] = 1;
    old_types[0] = MPI_FLOAT;
    old_types[1] = MPI_FLOAT;
    old_types[2] = MPI_FLOAT;
    old_types[3] = MPI_DOUBLE;
    old_types[4] = MPI_INT;
    old_types[5] = MPI_CHAR;
    MPI_Type_get_extent(MPI_FLOAT, &lb, &extent);
    disp[0] = lb;
    MPI_Type_get_extent(MPI_FLOAT, &lb, &extent);
    disp[1] = disp[0] + extent;
    MPI_Type_get_extent(MPI_FLOAT, &lb, &extent);
    disp[2] = disp[1] + extent;
    MPI_Type_get_extent(MPI_DOUBLE, &lb, &extent);
    disp[3] = disp[2] + extent;
    MPI_Type_get_extent(MPI_INT, &lb, &extent);
    disp[4] = disp[3] + extent;
    MPI_Type_get_extent(MPI_CHAR, &lb, &extent);
    disp[5] = disp[4] + extent;
    MPI_Type_create_struct(6, blocklengths, disp, old_types, &MPIParticle_t);
    MPI_Type_commit(&MPIParticle_t);
}

void Declare_MPI_Types() {
    __Declare_MPI_Type_Particle();
    return;
}





void init(Particle p[], int nelem);

void init(Particle p[], int nelem) {
  int i;

  for (i=0; i<nelem; i++) {
     p[i].x = i * 1.0;
     p[i].y = i * -1.0 * 2;
     p[i].z = i * 1.0 * 3; 
     p[i].velocity = 0.25;
     p[i].n = i;
     p[i].type = i % 2; 
  }
}
int main(int argc, char **argv)
{
  Particle *particles;
  int part_num;
  int step_num;
  float dt;
  int i, t;

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD,&__numprocs);
MPI_Comm_rank(MPI_COMM_WORLD,&__taskid);


Declare_MPI_Types();

if (__taskid == 0) {
  if (argc != 4) {
    printf("Particles part_num step_num dt\n");
    exit(-1);
  }

  part_num = atoi ( argv[1] );
  step_num = atoi ( argv[2] );
  dt = atof ( argv[3] );

  particles = ( Particle * ) malloc ( part_num * sizeof ( Particle ) );

  init(particles, part_num);

}
// pragma aqui









if (__taskid == 0) {
printf("particle[3]:   %3.2f %3.2f %3.2f %3.2f %d %d\n", particles[3].x,
     particles[3].y,particles[3].z,particles[3].velocity,particles[3].n,particles[3].type);

}
MPI_Finalize();
}
