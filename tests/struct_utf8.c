#pragma omp declare cluster
typedef struct{
    unsigned char bl,gr,re;
} color;

#pragma omp end declare cluster