#ifndef PLANE_H
#define PLANE_H

#include "material.h"

class plane: public scene_base_object{
    private:
        material* matter; //Material
        texture* paint; //Pintura aplicada
        point3 v1, v2, v3, v4;
        
    public:

        //Constructores
        plane(){}
        plane(const point3& p1, const point3& p2, const point3& p3, const point3& p4, material* matter, texture* paint):v1(p1), v2(p2), v3(p3), v4(p4), matter(matter), paint(paint){}
       
        //Funcion heredada de la funcion padre
        //Funcion que recibe un rayo, y actualiza un record en funcion a un impacto        //Si regresa false: El rayo no golpeó el triangulo
        //Si regresa true: El rayo si golpeó
        virtual bool hit(const ray& r, hit_record& h, double t_min, double t_max)const override{
           
            //Calculo de intersección entre rayo y el plano
            point3 normal = cross(v2-v1, v4-v1);
            point3 p_o = v1;

            double t = dot((p_o - r.origen()), normal)/dot(r.direction(), normal);

            if (t < t_min || t_max < t)
                return false;
            
            point3 p = r.getCoordinate(t);

            // First a check from v1
            point3 v1_v2 = v2 - v1;
            point3 v1_v4 = v4 - v1;
            point3 v1_p = p - v1;
            double v1_angle = angle_between(v1_v2, v1_v4);
            double v1_p_angle = angle_between(v1_v2, v1_p);
            if (v1_p_angle > v1_angle)
                return false;

            // Check from v2
            point3 v2_v3 = v3 - v2;
            point3 v2_v1 = v1 - v2;
            point3 v2_p = p - v2;
            double v2_angle = angle_between(v2_v3, v2_v1);
            double v2_p_angle = angle_between(v2_v3, v2_p);
            if (v2_p_angle > v2_angle)
                return false;

            // Check from v3
            point3 v3_v4 = v4 - v3;
            point3 v3_v2 = v2 - v3;
            point3 v3_p = p - v3;
            double v3_angle = angle_between(v3_v4, v3_v2);
            double v3_p_angle = angle_between(v3_v4, v3_p);
            if (v3_p_angle > v3_angle)
                return false;

            // Check from v4
            point3 v4_v1 = v1 - v4;
            point3 v4_v3 = v3 - v4;
            point3 v4_p = p - v4;
            double v4_angle = angle_between(v4_v1, v4_v3);
            double v4_p_angle = angle_between(v4_v1, v4_p);
            if (v4_p_angle > v4_angle)
                return false;
            

            h.u = (p.getX()-v1.getX()/v4.getX()-v1.getX());
            h.v = (p.getY()-v1.getY()/v4.getY()-v1.getY());
            h.t_touch = t;
            h.point_touched = p;
            h.set_normal_sig(r, vec_unit(normal)); // Set the normal
            h.condition = matter->bounce(r, h);
            h.emitted = matter->emitted(h.u, h.v, h.point_touched);
            h.col = paint->value(h.u, h.v, h.point_touched);
            return true;

        }

        // Setters
        void set_matter(material* matter){this->matter = matter;}
        void set_paint(texture* paint){this->paint = paint;}
        void set_v1(const point3& v1){this->v1 = v1;}
        void set_v2(const point3& v2){this->v2 = v2;}
        void set_v3(const point3& v3){this->v3 = v3;}
        void set_v4(const point3& v4){this->v4 = v4;}
       
       
};

#endif