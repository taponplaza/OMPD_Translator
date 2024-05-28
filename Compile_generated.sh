mpicc -o generated generated.c
mpirun -np 4 ./generated

# COMPILACION DE JULIAOMPD_MPI
# Paso 1: Instalar dependencias (si no están instaladas)
sudo apt update
sudo apt install mpich libomp-dev

# Paso 2: Compilar el archivo
mpicc -o JuliaOMPD_mpi generated.c -fopenmp -lm

# Paso 3: Ejecutar el programa
mpirun -np 4 ./JuliaOMPD_mpi 800 0.355 0.355 1.5 1000