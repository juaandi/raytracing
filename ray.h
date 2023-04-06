#ifndef RAY_H
#define RAY_H

#include <iostream>
#include "point3.h"

/* --- ESTA CLASE MODEL EL COMPORTAMIENTO DE LOS RAYOS --- */
class ray{
    private:
        point3 orig; //Punto de origen de salida del rayo 
        point3 dir; //Direccion del rayo

    public:

        //Constructores
        ray(){}
        ray(const point3& origin, const point3 direction): orig(origin),dir(direction) {}


        //Getters
        point3 origen()const{return orig;}
        point3 direction()const{return dir;}


        //Recibe un paso t de la recta del rayo
        //Returna el punto en ese paso
        point3 getCoordinate(const double t) const{
            return orig + t*dir;
        }
};

#endif