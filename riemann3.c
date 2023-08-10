#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define A 1
#define B 40
#define N 10e6

double f(double x); // La función a integrar
double trapezoides(double a, double b, int n, double *suma_global);
int cantidadTotalThreads;

int main(int argc, char* argv[]) {
    double integral;
    double suma_global = 0.0;
    double a = A, b = B;
    int n = N;
    int num_threads = 1; // Número predeterminado de hilos
    
    if (argc > 1) {
        a = strtol(argv[1], NULL, 10);
        b = strtol(argv[2], NULL, 10);
        n = strtol(argv[3], NULL, 10);
        
        if (argc > 4) {
            num_threads = strtol(argv[4], NULL, 10);
        }
    }
    
    omp_set_num_threads(num_threads); // Configura el número de hilos
    
    integral = trapezoides(a, b, n, &suma_global);
    
    printf("Con n = %d trapezoides y %d hilos, nuestra aproximacion \n", n, num_threads);
    printf("de la integral de %f a %f es = %.10f\n", a, b, integral);
    printf("\nCantidad total de threads leidos con omp_get_num_threads(): %d\n", cantidadTotalThreads);
    return 0;
}

double trapezoides(double a, double b, int n, double *suma_global) {
    double h = (b-a)/n;
    int n_local = n / omp_get_max_threads();

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        double a_local = a + id * n_local * h;
        double b_local = a_local + n_local * h;

        double suma_local = (f(a_local) + f(b_local)) / 2.0;
        for(int k = 1; k <= n_local-1; k++) {
            suma_local += f(a_local + k*h);
        }
        suma_local *= h;

#pragma omp critical
        printf("Suma local %.10f\n", suma_local);
        *suma_global += suma_local;
    }

    return *suma_global;
}

double f(double x) {
    cantidadTotalThreads = omp_get_num_threads();
    return x*x;
}
