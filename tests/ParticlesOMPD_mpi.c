
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <assert.h>

// #pragma omp declare cluster
typedef struct {
  float x, y, z;
  double velocity;
  int  n;
  char type;
} Particle;
// #pragma omp end declare cluster

void init(Particle p[], int nelem);

int __taskid = -1, __numprocs = -1;

MPI_Datatype MPIParticle_t;

void __Declare_MPI_Type_Particle () {
    int blocklengths[6];
    MPI_Datatype old_types[6];
    MPI_Aint disp[6];
    MPI_Aint lb;
    MPI_Aint extent;
    blocklengths[0]= 1;
    blocklengths[1]= 1;
    blocklengths[2]= 1;
    blocklengths[3]= 1;
    blocklengths[4]= 1;
    blocklengths[5]= 1;
    old_types[0]= MPI_FLOAT;
    old_types[1]= MPI_FLOAT;
    old_types[2]= MPI_FLOAT;
    old_types[3]= MPI_DOUBLE;
    old_types[4]= MPI_INT;
    old_types[5]= MPI_CHAR;

    MPI_Type_get_extent(MPI_FLOAT, &lb, &extent);
    disp[0]= lb;
    MPI_Type_get_extent(MPI_FLOAT, &lb, &extent);
    disp[1]= disp[0] + extent;
    MPI_Type_get_extent(MPI_FLOAT, &lb, &extent);
    disp[2]= disp[1] + extent;
    MPI_Type_get_extent(MPI_DOUBLE, &lb, &extent);
    disp[3]= disp[2] + extent;
    MPI_Type_get_extent(MPI_INT, &lb, &extent);
    disp[4]= disp[3] + extent;
    MPI_Type_get_extent(MPI_CHAR, &lb, &extent);
    disp[5]= disp[4] + extent;
    MPI_Type_create_struct(6,blocklengths, disp, old_types, &MPIParticle_t);
    MPI_Type_commit(&MPIParticle_t);
}

void Declare_MPI_Types () {
  __Declare_MPI_Type_Particle ();
  return;
}


int main(int argc, char **argv)
{
  Particle *particles;
  int part_num;
  int step_num;
  float dt;
  int i, t;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &__numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &__taskid);

// #pragma omp declare cluster
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
MPI_Barrier(MPI_COMM_WORLD);
// #pragma omp cluster broad(part_num, dt, step_num) scatter(particles[part_num]) gather(particles[part_num])
{
MPI_Bcast(&part_num, 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Bcast(&step_num, 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Bcast(&dt, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

// Gather Scatter de particles
Particle * __particles;
int __chunkG_particles;
int *displsG_particles = (int *)malloc(__numprocs*sizeof(int));
int *countsG_particles = (int *)malloc(__numprocs*sizeof(int));
int __chunkS_particles;
int *displsS_particles = (int *)malloc(__numprocs*sizeof(int));
int *countsS_particles = (int *)malloc(__numprocs*sizeof(int));

__chunkG_particles = (part_num / __numprocs);
if (__taskid < (part_num % __numprocs))
    __chunkG_particles++;
countsG_particles[__taskid] = __chunkG_particles;
displsG_particles[__taskid] = __chunkG_particles * __taskid;
if (__taskid >= (part_num % __numprocs))
    displsG_particles[__taskid] += (part_num % __numprocs);

if (__taskid == 0) {
    __chunkG_particles = (part_num / __numprocs);
    for (i=0; i < __numprocs; i++)
        if (i < (part_num % __numprocs))
            countsG_particles[i] = (__chunkG_particles+1);
        else
            countsG_particles[i] = __chunkG_particles;
    displsG_particles[0] = 0;
    for (i=1; i < __numprocs; i++)
        if (i <= (part_num % __numprocs))
            displsG_particles[i] = displsG_particles[i-1] + (__chunkG_particles+1);
        else
            displsG_particles[i] = displsG_particles[i-1] + __chunkG_particles;
    assert ((displsG_particles[__numprocs-1] + countsG_particles[__numprocs-1]) == part_num);
    if (__taskid < (part_num % __numprocs))
        __chunkG_particles++;
}

__chunkS_particles = (part_num / __numprocs);
if (__taskid < (part_num % __numprocs))
    __chunkS_particles++;
countsS_particles[__taskid] = __chunkS_particles;
displsS_particles[__taskid] = __chunkS_particles * __taskid;
if (__taskid >= (part_num % __numprocs))
    displsS_particles[__taskid] += (part_num % __numprocs);

if (__taskid == 0) {
    __chunkS_particles = (part_num / __numprocs);
    for (i=0; i < __numprocs; i++)
        if (i < (part_num % __numprocs))
            countsS_particles[i] = (__chunkS_particles+1);
        else
            countsS_particles[i] = __chunkS_particles;
    displsS_particles[0] = 0;
    for (i=1; i < __numprocs; i++)
        if (i <= (part_num % __numprocs))
            displsS_particles[i] = displsS_particles[i-1] + (__chunkS_particles+1);
        else
            displsS_particles[i] = displsS_particles[i-1] + __chunkS_particles;
    assert ((displsS_particles[__numprocs-1] + countsS_particles[__numprocs-1]) == part_num);
    if (__taskid < (part_num % __numprocs))
        __chunkS_particles++;
}

if (__taskid != 0)
    particles = (Particle *) malloc ( 1 * sizeof (Particle));

__particles = ( Particle * ) malloc ( __chunkS_particles * __numprocs * sizeof (Particle));


MPI_Scatterv(particles, countsS_particles, displsS_particles, MPIParticle_t, __particles, countsS_particles[__taskid] , MPIParticle_t, 0, MPI_COMM_WORLD);

  for (t=0; t<step_num; t++) {
// #pragma omp teams distribute 
{
int first_iter=0;
int last_iter=part_num;
int __iter;
int __start;
int __end;
__iter = ((last_iter - first_iter) / __numprocs);
if (__taskid < ((last_iter - first_iter) % __numprocs))
    __iter++;
__start = ( first_iter + __iter * __taskid) ;
if (__taskid >= ((last_iter - first_iter) % __numprocs))
    __start += ((last_iter - first_iter) % __numprocs);
__end = __start + __iter ;
if (__taskid == (__numprocs-1)) assert (__end == last_iter);
#pragma omp parallel for
    for (i=__start; i<__end; i++) {
      __particles[i].x += __particles[i].velocity * dt;
    }
}
  }
MPI_Gatherv((&__particles[0])+displsG_particles[__taskid], countsG_particles[__taskid], MPIParticle_t, particles, countsG_particles, displsG_particles, MPIParticle_t, 0, MPI_COMM_WORLD);
}

MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();

  return 0;
}

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
