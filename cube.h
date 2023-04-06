#ifndef CUBE_H
#define CUBE_H

#include "obstacle_list.h"
#include "plane.h"

#include <vector>

class cube : public scene_base_object  {
    
    private:
       
        obstacle_list* sides;
        plane* planes;

        // buffer to store the variables to initialize the cube
        point3 buffer_points;
        double buffer_sizes;
        material* buffer_materials;
        texture* buffer_textures;
    
    public:
        cube() {}

        cube(const point3& centre, double taille, material* matter, texture* paint){
            
            point3 dif = point3(taille/2, taille/2, taille/2);
            point3 p0 = centre - dif;
            point3 p7 = centre + dif;
            point3 p1 = point3(p0.getX(), p0.getY(), p7.getZ());
            point3 p2 = point3(p0.getX(), p7.getY(), p0.getZ());
            point3 p3 = point3(p0.getX(), p7.getY(), p7.getZ());
            point3 p4 = point3(p7.getX(), p0.getY(), p0.getZ());
            point3 p5 = point3(p7.getX(), p0.getY(), p7.getZ());
            point3 p6 = point3(p7.getX(), p7.getY(), p0.getZ());
   
            sides = new obstacle_list();
            
            planes = new plane[6];

            planes[0] = plane(p0, p2, p3, p1, matter, paint);
            sides->add(&planes[0]);

            planes[1] = plane(p4, p6, p7, p5, matter, paint);
            sides->add(&planes[1]);

            planes[2] = plane(p0, p4, p6, p2, matter, paint);
            sides->add(&planes[2]);

            planes[3] = plane(p1, p5, p7, p3, matter, paint);
            sides->add(&planes[3]);

            planes[4] = plane(p0, p1, p5, p4, matter, paint);
            sides->add(&planes[4]);

            planes[5] = plane(p2, p3, p7, p6, matter, paint);
            sides->add(&planes[5]);
            
        }

        //destructores
        ~cube(){
            if (sides != nullptr){
                delete sides;
            }
            if (planes != nullptr){
                delete[] planes;
            }
        

        }

        virtual bool hit(const ray& r, hit_record& h, double t_min, double t_max) const override{
            return sides->hit(r, h, t_min, t_max);
        }

    // Setters
    void set_origin(const point3& centre){
        this->buffer_points = centre;
    }
    void set_size(double taille){
        this->buffer_sizes = taille;
    }
    void set_matter(material* matter){
        this->buffer_materials = matter;
    }
    void set_paint(texture* paint){
        this->buffer_textures = paint;
    }

    // Getters
    const point3 get_origin(){
        return buffer_points;
    }
    const double get_size(){
        return buffer_sizes;
    }
    const material* get_matter(){
        return buffer_materials;
    }
    const texture* get_paint(){
        return buffer_textures;
    }

    void reinit_cube()
    {
        point3 dif = point3(buffer_sizes/2, buffer_sizes/2, buffer_sizes/2);
            point3 p0 = buffer_points - dif;
            point3 p7 = buffer_points + dif;
            point3 p1 = point3(p0.getX(), p0.getY(), p7.getZ());
            point3 p2 = point3(p0.getX(), p7.getY(), p0.getZ());
            point3 p3 = point3(p0.getX(), p7.getY(), p7.getZ());
            point3 p4 = point3(p7.getX(), p0.getY(), p0.getZ());
            point3 p5 = point3(p7.getX(), p0.getY(), p7.getZ());
            point3 p6 = point3(p7.getX(), p7.getY(), p0.getZ());
   
            sides = new obstacle_list();
            
            planes = new plane[6];

            planes[0] = plane(p0, p2, p3, p1, buffer_materials, buffer_textures);
            sides->add(&planes[0]);

            planes[1] = plane(p4, p6, p7, p5, buffer_materials, buffer_textures);
            sides->add(&planes[1]);

            planes[2] = plane(p0, p4, p6, p2, buffer_materials, buffer_textures);
            sides->add(&planes[2]);

            planes[3] = plane(p1, p5, p7, p3, buffer_materials, buffer_textures);
            sides->add(&planes[3]);

            planes[4] = plane(p0, p1, p5, p4, buffer_materials, buffer_textures);
            sides->add(&planes[4]);

            planes[5] = plane(p2, p3, p7, p6, buffer_materials, buffer_textures);
            sides->add(&planes[5]);
    }
};

#endif

