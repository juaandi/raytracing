#ifndef SPHERE_H 
#define SPHERE_H

#include "scene_base_object.h"
#include "material.h"
#include "texture.h"

//Clase esfera: Hereda de la clase de objetos padre scene_base_object
class sphere: public scene_base_object{

    //atributos de una esfera
    private:
        point3 origin; //Punto de origen
        double radius; //Radio 
        material* matter; //Material 
        texture* paint; //Pintura aplicada

        static void get_sphere_uv(const point3& p, double& u, double& v) {
            double pi = 3.14159265;
            // p: a given point on the sphere of radius one, centered at the origin.
            // u: returned value [0,1] of angle around the Y axis from X=-1.
            // v: returned value [0,1] of angle from Y=-1 to Y=+1.
            //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
            //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
            //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

            double theta = acos(-p.getY());
            double phi = atan2(-p.getZ(), p.getX()) + pi;

            u = phi / (2*pi);
            v = theta / pi;
        }
    
    public:

        material* get_matter(){return matter;}
        texture* get_paint(){return paint;}


        //Constructores
        sphere(){}
        sphere(const point3& origin, double radius,  material* matter, texture* paint):origin(origin), radius(radius), matter(matter), paint(paint){}


        //Funcion heredada de la funcion padre
        //Funcion que recibe un rayo, y actualiza un record en funcion a un impacto        //Si regresa false: El rayo no golpeó la esfera
        //Si regresa true: El rayo si golpeó   
        virtual bool hit(const ray& r, hit_record& h, double t_min, double t_max) const override{
            
            
            //Calculo de interseccion de un rayo con una esfera
            point3 ac = r.origen()-origin;
            double a = dot(r.direction(),r.direction());
            double b = 2*dot(r.direction(),ac);
            double c = dot(ac,ac)-radius*radius;

            //Calculo de discriminante
            double discriminant = b*b - 4*a*c;
            if (discriminant < 0){return false;}


            //Se determina si el punto de contacto esta entre el rango admisible de golpe
            double sqrt_dis = sqrt(discriminant);
            auto root = (-b - sqrt_dis) / (2*a);
            if (root < t_min || t_max < root) {
                root = (-b + sqrt_dis) / (2*a);
                if (root < t_min || t_max < root)
                    return false;
            }

            //Actualizamos los valores del hittable record
            h.t_touch = (-b-sqrt(discriminant))/(2*a); //t de golpe
            h.point_touched = r.getCoordinate(h.t_touch); //punto en el espacio donde golpeó 
            h.normal = vec_unit(h.point_touched-origin); //vector normal a la superficie
            h.set_normal_sig(r,h.normal); //definición de la dirección de la normal para que sea saliendo
            get_sphere_uv(h.normal, h.u, h.v);
            h.condition = matter->bounce(r,h); //Condición dada por el material
            h.emitted = matter-> emitted(h.u, h.v, h.point_touched); //Emisión dada por el material
            h.col = paint->value(h.u, h.v, h.point_touched); //Color del material impactado
            return true;
            
        }

        
        //Getters
        point3 get_origin()const{return origin;}
        double get_radius()const{return radius;}

        //Setters
        void set_origin(const point3& origin){this->origin = origin;}
        void set_radius(double radius){this->radius = radius;}
        void set_matter(material* matter){this->matter = matter;}
        void set_paint(texture* paint){this->paint = paint;}
        
};




#endif