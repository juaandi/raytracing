#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include "scene_base_object.h"
#include "texture.h"


/* Clase de material
Clase padre que se encarga de heredar su funcion rebote a sus hijas
*/
class material{
    
    public:
        virtual bool bounce(const ray& ray_in, hit_record& rec)const=0;

        virtual point3 emitted(double u, double v, const point3& p) const {
            return point3(0,0,0);
        }
};


/* Objeto blurry, hereda de material
Ofrece un material difuso
*/
class blurry: public material{

    public:

        //Constructor
        blurry(){}
        
        //Sobre-escribir de la funcion padre
        virtual bool bounce(const ray& ray_in, hit_record& rec)const override{
            
            //Calculo de punto de direccion de rebote
            point3 bounce_dir = rec.normal + rand_vec_in_unit_sphere(); 
            
            //Elimina problemas cuando el rebote es cercano a la normal
            if (bounce_dir.near_zero()){
                bounce_dir = rec.normal;
            }

            //calculo de vector de salida 
            rec.vec_out = ray(rec.point_touched, bounce_dir);
            return true;
        }
};

/* Objeto metal, hereda de material
Ofrece un material metalico con un indice de difusion
*/
class metal: public material{
    
    private:
        double fuzz;

    public:
         
        //Constructor
        metal():fuzz(0){}
        metal(double value):fuzz(value){}

        //Sobre-escribir de la funcion padre
        virtual bool bounce(const ray& ray_in, hit_record& rec)const override{
            
            //calculo de direccion de reflexion 
            point3 reflexion = reflect(vec_unit(ray_in.direction()), rec.normal);

            //Calculo de vector saliente completamente refleajdo 
            rec.vec_out = ray(rec.point_touched, reflexion + fuzz*rand_point_in_unit_sphere()); //Se le suma el valor de difusion
            
            //Garantiza que el vector no sea dentico a la normal
            return (dot(rec.vec_out.direction(), rec.normal)>0);
        }
};


/* Objeto glass, hereda de material
Ofrece un material transparente con un indice de refraccion
*/
class glass: public material{

    private:
        double index;
    
    public:

        //Constructor
        glass(double value):index(value){}

        //Sobre-escribir de la funcion padre
        virtual bool bounce(const ray& ray_in, hit_record& rec)const override{
            
            //calculo del ratio de refraccion
            double refrac_ratio = rec.front_face ? (1/index):index;

            //conversion a vector unitario la direccion del vector de entrada
            point3 unit_ray_in = vec_unit(ray_in.direction());

            //Vector completamente refractado
            point3 refracted = refract(unit_ray_in, rec.normal, refrac_ratio);

            //Vector de salida
            rec.vec_out = ray(rec.point_touched, refracted);
            return true;
        }

};


class light : public material{
    private:
        texture* emit;
    
    public:

        light(texture* value):emit(value){}
        
        virtual bool bounce(const ray& ray_in, hit_record& rec)const override{
            return false;
        }

        virtual point3 emitted(double u, double v, const point3& p)const override{
            return emit->value(u,v,p);
        }
};
#endif