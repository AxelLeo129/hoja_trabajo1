#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define A 1
#define B 40
#define N 10e6

double f(double x); // La funcion a integrar
double trapezoides(double a, double b, int n);
int cantidadTotalThreads;

int main(int argc, char* argv[]) {
    double integral;
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
    
    integral = trapezoides(a, b, n);
    
    printf("Con n = %d trapezoides y %d hilos, nuestra aproximacion \n", n, num_threads);
    printf("de la integral de %f a %f es = %.10f\n", a, b, integral);
    printf("\nCantidad total de threads leidos con omp_get_num_threads(): %d\n", cantidadTotalThreads);
    return 0;
}/*main*/

//------------------------------------------
// trapezoides
//
// Estimar la integral mediante sumas de Riemann
// Input: a,b,n,h
// Output: integral
//------------------------------------------
double trapezoides(double a, double b, int n) {
    double integral = 0.0;
    double h = (b-a)/n;
    int n_local = n / omp_get_max_threads(); // Número local de trapezoides

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        double a_local = a + id * n_local * h; // Valor inicial local
        double b_local = a_local + n_local * h; // Valor final local

        double integral_local = (f(a_local) + f(b_local)) / 2.0; // Valor inicial local de la integral
        for(int k = 1; k <= n_local-1; k++) {
            integral_local += f(a_local + k*h);
        }
        integral_local *= h;

#pragma omp critical
        integral += integral_local;
    }

    return integral;
}/*trapezoides*/

//------------------------------------------
// f
//
// Funcion a ser integrada
// Input: x
//------------------------------------------
double f(double x) {
    cantidadTotalThreads = omp_get_num_threads();
    double return_val;
    return_val = x*x;
    return return_val;
}/*f*/