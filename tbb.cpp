#include <iostream>
#include <vector>
#include <chrono>
#include <tbb/tbb.h> // Necesario para TBB

class ImageInverter {
public:
    // Este operador() es llamado por TBB para procesar un rango de píxeles
    void operator()(const tbb::blocked_range<int>& r) const {
        for (int i = r.begin(); i != r.end(); ++i) {
            output_image_ptr[i] = 255 - input_image_ptr[i]; // Aquí se invierte el píxel
        }
    }

    // Constructor para que nuestro objeto tenga acceso a los datos de la imagen
    ImageInverter(const int* input_ptr, int* output_ptr)
        : input_image_ptr(input_ptr), output_image_ptr(output_ptr) {}

private:
    const int* input_image_ptr;
    int* output_image_ptr;
};

int main() {
    const int image_size = 100000000; // El tamaño de nuestra imagen (en píxeles)
    std::vector<int> original_image(image_size);
    std::vector<int> inverted_image(image_size);

    // Llenamos la imagen original con valores de píxeles simulados
    for (int i = 0; i < image_size; ++i) {
        original_image[i] = i % 256; // Valores entre 0 y 255
    }

    std::cout << "Iniciando la inversión de imagen con TBB..." << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    // tbb::parallel_for distribuye el trabajo definido en 'ImageInverter'
    // sobre el rango de píxeles de 0 a 'image_size'
    tbb::parallel_for(tbb::blocked_range<int>(0, image_size),
                      ImageInverter(original_image.data(), inverted_image.data()));

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "Tiempo total con TBB: " << duration.count() << " segundos" << std::endl;

    return 0;
}
