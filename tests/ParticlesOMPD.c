
#include <stdlib.h>
#include <stdio.h>

#pragma omp declare cluster
typedef struct {
  float x, y, z;
  double velocity;
  int  n;
  char type;
} Particle;
#pragma omp end declare cluster

void init(Particle p[], int nelem);

int main(int argc, char **argv)
{
  Particle *particles;
  int part_num;
  int step_num;
  float dt;
  int i, t;

  if (argc != 4) {
    printf("Particles part_num step_num dt\n");
    exit(-1);
  }

  part_num = atoi ( argv[1] );
  step_num = atoi ( argv[2] );
  dt = atof ( argv[3] );

  particles = ( Particle * ) malloc ( part_num * sizeof ( Particle ) );

  init(particles, part_num);

#pragma omp cluster broad(part_num, dt, step_num) scatter(particles[part_num]) gather(particles[part_num])
//   for (t=0; t<step_num; t++) {
// #pragma omp teams distribute 
// #pragma omp parallel for
//     for (i=0; i<part_num; i++) {
//       particles[i].x += particles[i].velocity * dt;
//     }
//   }

printf("particle[3]:   %3.2f %3.2f %3.2f %3.2f %d %d\n", particles[3].x,
     particles[3].y,particles[3].z,particles[3].velocity,particles[3].n,particles[3].type);

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
