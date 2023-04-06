#ifndef COLOR_H
#define COLOR_H

#include "image.h"
#include "scene_base_object.h"
#include "background.h"

//devuelve la conversion de un vector unitario a un color RGB [0,255]
auto scale_RGB(const point3& v, int samples_per_pixel){

    //Toma X, Y, Z del vector "v" y lo normaliza
    //Se les saca raiz para hacer aproximación gamma=2
    //
    double r = limit(sqrt(v.getX()/samples_per_pixel), 0.0, 0.999);
    double g = limit(sqrt(v.getY()/samples_per_pixel), 0.0, 0.999);
    double b = limit(sqrt(v.getZ()/samples_per_pixel), 0.0, 0.999);

    //Conversion a escala RGB
    int ri = static_cast<int>(255.999*r); 
    int gi = static_cast<int>(255.999*g);
    int bi = static_cast<int>(255.999*b);
    
    PixelColor<int> color(ri,gi,bi);//Creación de pixel de color 

    return color;
}


//Crea un blend con una direccion y un color
point3 blend(const ray& r, const background& landscape){
    point3 vec_dir_unit = vec_unit(r.direction()); //Normalizacion de vector
    
    //Deficion de la direccion del blend
    double t = 0.5*(vec_dir_unit.getY()+1.0); //t entre 0 y 1

    //Defincion de color del fondo
    point3 blend = (1-t)*landscape.get_color_1()+t*landscape.get_color_2(); //Interpolacion para blend
    return blend;
}


/* Funcion para covertir un rayo en color
recibe un rayo, una lista de figuras (mundo) y una profundidad de recursividad
*/
point3 color_ray(const ray& r, const background& landscape ,scene_base_object& world, int prof){ 

    
    //record de golpes realizados por el rayo de entrada
    hit_record record;


    //Verificación de la profundidad
    if (prof <=0){
        return point3(0.0, 0.0, 0.0);
    }


    if(!world.hit(r, record, 0.0001, 9999999)){
        
        return blend(r, landscape);
    }


    if (!record.condition){
        return record.emitted;
    }
    
    return record.emitted + record.col * color_ray(record.vec_out, landscape, world, prof-1);
}

#endif