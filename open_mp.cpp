#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip> // Para formatear la salida

#ifdef _OPENMP
#include <omp.h> // Necesario para OpenMP
#endif

int main() {
    const long long num_steps = 1000000000; // Cuántos "pasos" usaremos para la aproximación
    double step;
    double sum = 0.0;
    
    step = 1.0 / (double)num_steps;

    std::cout << "Iniciando el cálculo de Pi con OpenMP..." << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for private(i) reduction(+:sum)
    for (long long i = 0; i < num_steps; ++i) {
        double x = (i + 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }

    double pi = sum * step;

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Valor calculado de Pi: " << pi << std::endl;
    std::cout << "Tiempo total con OpenMP: " << duration.count() << " segundos" << std::endl;

    return 0;
}
