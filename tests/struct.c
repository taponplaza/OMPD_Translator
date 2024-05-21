#pragma omp declare cluster
typedef struct{
    unsigned char bl,gr,re,al;
    int number, number2;
} color;

#pragma omp end declare cluster
