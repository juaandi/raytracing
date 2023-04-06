#ifndef CAMARA_H
#define CAMARA_H

#include "ray.h"

//Clase camara: punto de vista del espectador, puede moverse, acercarse y jugar con el ancho de vision
class camara { 

    //Atributos de la camara
    private:
        point3 origin; //Punto de origen de la camara
        point3 horizontal; //Ancho del cuadro de vision
        point3 vertical; //Alto del cuadro de vision
        point3 lower_left_corner; //Punto de inicio de renderizacion
        int height; //alto de la imagen
        int width; //Ancho de la imagen
        double len_rad; //Radio de lente (blur)
        point3 u, v, w; //vectores de plano de sistema de referencia 

    public:

        /*Recibe:
        ->punto de origen
        ->punto de observación
        ->vector hacia arriba (y)
        ->campo de vision en grados
        ->relación de tamaño
        ->apertura del lente (blur)
        ->distancia focal (blur)
        */
        camara(const point3& lookfrom, const point3& lookat, const point3& vup, double vfov, double ratio, double pixels_h, double hole, double focus_dis){
            
            
            double theta = from_deg_to_rad(vfov); //angulo de campo de vision en grados
            double h = tan(theta/2); //altura del campo de vision
            double view_height = 2.0 * h; //alto del campo de la imagen
            double view_width = ratio * view_height; //ancho del campo de la imagen

            w = vec_unit(lookfrom-lookat); //Vector equivalente a z
            u = vec_unit(cross(vup,w)); //Vector equivalente a x
            v = cross(w,u); //Vector equivalente a y

            height = pixels_h; //Alto de la imagen en pixeles
            width = static_cast<int>(height*ratio); //Ancho de imagen en pixeles

            origin = lookfrom; //Origen
            horizontal = view_width*u*focus_dis; //definicion de ancho
            vertical = view_height*v*focus_dis; //definicion de alto
            lower_left_corner = origin - horizontal/2- vertical/2 - w*focus_dis; //punto de inicio para los rayos
            len_rad = hole/2; //radio del lente (blur)

        }

        //Funcion que convierte una coordenada en un rayo saliendo a esa coordenada
        ray get_ray(double x, double y)const{

            //Aplicación del blur
            point3 rd = len_rad * random_in_unit_disk();
            point3 offset = u * rd.getX() + v * rd.getY();

            //Aplicación del offset para el blur
            return ray(origin + offset, lower_left_corner + x*horizontal + y*vertical - origin - offset);
            
        }

        //getters
        int get_width()const{return width;}
        int get_height()const{return height;}
};



#endif