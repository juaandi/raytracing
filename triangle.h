#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "material.h"

//Clase triangulo: Hereda de la clase de objetos padre scene_base_object
class triangle: public scene_base_object{
    
    //atributos de un triangulo, 3 vertices
    private:
        material* matter; //Material
        texture* paint; //Pintura aplicada
        point3 v1, v2, v3;

        static void get_triangle_uv(const point3& p, double& u, double& v, const point3& v1, const point3& v2, const point3& v3) {
            
            /*
            double x_min = min(v1.getX(), min(v2.getX(), v3.getX()));
            double x_max = max(v1.getX(), max(v2.getX(), v3.getX()));
            */
            double y_min = min(v1.getY(), min(v2.getY(), v3.getY()));
            double y_max = max(v1.getY(), max(v2.getY(), v3.getY()));
            
            
            double z_min = min(v1.getZ(), min(v2.getZ(), v3.getZ()));
            double z_max = max(v1.getZ(), max(v2.getZ(), v3.getZ()));
            

            u = (p.getZ()-z_min) / (z_max-z_min);
            v = (p.getY()-y_min) / (y_max-y_min);
        }
    
    
    public:

        //Constructores
        triangle(){}
        triangle(const point3& p1, const point3& p2, const point3& p3, material* matter, texture* paint):v1(p1), v2(p2), v3(p3), matter(matter), paint(paint){}
       
        //Funcion heredada de la funcion padre
        //Funcion que recibe un rayo, y actualiza un record en funcion a un impacto        //Si regresa false: El rayo no golpeó el triangulo
        //Si regresa true: El rayo si golpeó
        virtual bool hit(const ray& r, hit_record& h, double t_min, double t_max)const override{
           
            //Calculo de intersección entre rayo y el plano
            point3 normal = cross(v2-v1, v3-v1);
            point3 p_o = v1;

            double t = dot((p_o - r.origen()), normal)/dot(r.direction(), normal);

            if (t < t_min || t_max < t)
                return false;
            
            point3 p = r.getCoordinate(t);

            // First a check from v1
            point3 v1_v2 = v2 - v1;
            point3 v1_v3 = v3 - v1;
            point3 v1_p = p - v1;
            double v1_angle = angle_between(v1_v2, v1_v3);
            double v1_p_angle = angle_between(v1_v2, v1_p);
            if (v1_p_angle > v1_angle)
                return false;

            // Check from v2
            point3 v2_v1 = v1 - v2;
            point3 v2_v3 = v3 - v2;
            point3 v2_p = p - v2;
            double v2_angle = angle_between(v2_v3, v2_v1);
            double v2_p_angle = angle_between(v2_v3, v2_p);
            if (v2_p_angle > v2_angle)
                return false;

            // Check from v3
            point3 v3_v1 = v1 - v3;
            point3 v3_v2 = v2 - v3;
            point3 v3_p = p - v3;
            double v3_angle = angle_between(v3_v1, v3_v2);
            double v3_p_angle = angle_between(v3_v1, v3_p);
            if (v3_p_angle > v3_angle)
                return false;
            
            h.t_touch = t;
            h.point_touched = p;
            get_triangle_uv(h.point_touched, h.u, h.v, v1, v2, v3); 
            h.set_normal_sig(r, vec_unit(normal)); // Set the normal
            h.condition = matter->bounce(r, h);
            h.emitted = matter->emitted(h.u, h.v, h.point_touched);
            h.col = paint->value(h.u, h.v, h.point_touched);
            return true;

        }
};

#endif