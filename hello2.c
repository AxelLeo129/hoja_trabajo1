#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void openmp_hello(void);

int main(int argc, char* argv[]){
    int thread_count = strtol(argv[1], NULL, 10);

#pragma omp parallel num_threads(thread_count)
    openmp_hello();
    
    return 0;
}

void openmp_hello(void){
    int id = omp_get_thread_num();
    if (id%2 == 0){
        printf("Saludos del hilo %d\n", id);
    } else {
        printf("Feliz cumpleaños numero %d!\n", omp_get_num_threads());
    }
}