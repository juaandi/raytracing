#ifndef RENDER_H
#define RENDER_H
#include "color.h"
#include "camara.h"
#include "engine.h"

#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <omp.h>
#include <atomic>

/* Seccion de renderizado
Esta funcion recibe:
->Un objeto tipo imagen
->Un valor de inicio (paralelismo)
->Un valor final (paralelismo)
->Una lista de punteros de figuras (mundo)
->Una camara
->Una cantidad de muestras por pixel
->Una profunidad (recursividad)

Aqui se realiza todo el proceso de renderizado, colorear la imagen en funcion de los rebotes de los rayos con los objetos
*/




void render_section(Image<int>& img, int start, int end, const background& landscape,scene_base_object& world, const camara& cam, engine& eng){
    
    for (int j = 0; j<img.get_width(); j++)
    { 
        for (int i = start; i<=end-1; i++)
        { 
            point3 sampler; //Almacenará la suma de colores de un solo pixel

            //Se realizan k lanzamientos de rayo a un mismo pixel (ligeramente cambiados) y luego se promedian
            for (int k = 0; k < eng.get_samples_per_pixel(); k++){

                //Normalización de posiciones
                double pos_y = (double(i) + random_double())/(img.get_height()-1); 
                double pos_x = (double(j) + random_double())/(img.get_width()-1);

                //Obtencion del rayo adecuado a cada posicón  
                ray vec = cam.get_ray(pos_x, pos_y);
                
                //Obtencion del color adecuado para cada rayo
                point3 color_not_scale = color_ray(vec, landscape, world, eng.get_max_depth());
                
                
                //Acumulador de color
                sampler = sampler + color_not_scale;        


            }
            
            //Generación de color
            PixelColor<int> code_RGB = scale_RGB(sampler, eng.get_samples_per_pixel());
            
            //Coloración de la imagen
            
            img.set_pixel(j,i,code_RGB);
            
        }
    }
}

void update_percentage(double percentage) {
    std::cout << "\rPercentage in main: " << percentage << "%" << std::flush;
}

void parallelization(int thread_num, Image<int>& img, const background& landscape, scene_base_object& world, camara& cam, engine eng, void (*callback)(double)){
    
    // Each thread will render two line of the image and then take the next one without waiting for the others
    int section_size = 1;
    
    // Create the threads
    std::vector<std::thread> threads;

    // Create the mutex and the condition variable
    std::mutex mutex;
    std::condition_variable condition;

    // Create the section counter
    int section = 0;

    // Create the threads
    for (int i = 0; i < thread_num; i++) {
        threads.push_back(std::thread([&]() {
            // Lock the mutex
            std::unique_lock<std::mutex> lock(mutex);

            // While there are sections to render
            while (section < img.get_height()) {
                // Get the section to render
                int section_start = section;
                int section_limit = section + section_size;

                // Increment the section counter
                section += section_size;

                // Unlock the mutex
                lock.unlock();

                // Render the section
                render_section(img, section_start, section_limit, landscape, world, cam, eng);

                // Lock the mutex
                lock.lock();

                // Notify the other threads that the section has been rendered
                callback((double(section)/img.get_height())*100);
                condition.notify_all();

                double percentage = (double(section)/img.get_height())*100;
                // Only want two digits after the decimal point
                percentage = (int)(percentage * 100) / 100.0;
                std::cout << "\rPercentage: " << percentage << "%     " << std::flush;

            }
        }));
    }


    // Join the threads with the main thread
    for (auto& t : threads) {
        t.join();
    }

}

/* Funcion para generar la paralelizacion

Recibe:
->Numero de hilos
->Imagen
->lista de punteros de figuras (world)
->camara
->Numero de muestras por pixel
->profunidad (recursividad)
*/



void parallel_render(int thread_num, Image<int>& img, const background& landscape, scene_base_object& world, camara& cam, engine eng){
    
    auto start = std::chrono::high_resolution_clock::now();

    // Start with section 0
    int section_start = 0;
    int section_limit = img.get_height();

    std::cout << "Rendering..." << std::endl;


    parallelization(thread_num, img, landscape, world, cam, eng, update_percentage);
    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;

    cout << "Elapsed time: " << elapsed.count() << " s\n";

}

#endif
