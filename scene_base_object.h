#ifndef SCENE_BASE_OBJECT_H
#define SCENE_BASE_OBJECT_H

#include "ray.h"



/* --- Estructura para almacenar informacion relacionada con el contacto de un rayo y una superficie --- */
struct hit_record{

    double t_touch; //Paso de un rayo donde hace el contacto
    point3 point_touched; //Punto en el espacio donde se realiza el contacto
    point3 normal; //Vector normal a la superficie de contacto 
    bool front_face; //Variable que ayuda a determinar si un vector normal sale o entra de la figura
    point3 col; //Color de la figura de contacto
    ray vec_out; //Vector de salida al realizar un contacto
    bool condition; //Condicion de rebote dada por el material
    double u; //Coordenada u de textura [0,1] (equivalente a phi en coordenadas esfericas)
    double v; //Coordenada v de textura [0,1] (equivalente a theta en coordenadas esfericas)
    point3 emitted; //Color emitido por la figura

    //Funcion que asegura que el vector normal siempre apunte para afuera
    //Recibe el rayo de incidencia y la normal con la superficie de contacto:
    //-> si el producto punto entre estos 2 es menor de 0, implica que la normal apunta saliendo de la figura
    //-> si el producto punto es mayor a 0, implica que la normal apunta entrando a la figura, si esto pasa, se cambia la dirección
    void set_normal_sig(const ray& r, const point3& initial_normal) {
        front_face = dot(r.direction(),initial_normal) < 0;
        normal = front_face ? initial_normal : -1*initial_normal; 
    }
};

//Clase padre de la que se desprenden todas las figuras
class scene_base_object{

    public:
        virtual bool hit(const ray& r, hit_record& h, double t_min, double t_max)const=0;
        
        
};

#endif