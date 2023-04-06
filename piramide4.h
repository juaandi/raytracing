#ifndef PIRAMIDE4_H
#define PIRAMIDE4_H

#include "cube.h"
#include "triangle.h"
#include "materials_and_textures.h"

class piramide4: public scene_base_object{
    
    private:
            point3 centre;
            double size, high;
            obstacle_list* sides;
            plane* pln;
            triangle* tri;

            // ------------------
            point3 buffer_centre;
            double buffer_size, buffer_high;
            material* buffer_matter;
            texture* buffer_paint;

    public: 
        //Constructores
        piramide4(){}
        piramide4(const point3& centre, double size, double high, material* matter, texture* paint){
            
            point3 dif = point3(size/2, 0, size/2);
            point3 p0 = centre - dif;
            point3 p1 = centre + dif;
            point3 p2 = point3(p1.getX(), p0.getY(), p0.getZ());
            point3 p3 = point3(p0.getX(), p0.getY(), p1.getZ());
            
            sides = new obstacle_list();
            pln = new plane[1];
            tri = new triangle[4];
            
            pln[0] = plane(p0, p2, p1, p3,  matter, paint);
            sides->add(&pln[0]);

            
            tri[0] = triangle(p0, p3, point3(centre.getX(), centre.getY()+high, centre.getZ()), matter, paint);
            sides->add(&tri[0]);

            tri[1] = triangle(p3, p1, point3(centre.getX(), centre.getY()+high, centre.getZ()), matter, paint);
            sides->add(&tri[1]);

            tri[2] = triangle(p1, p2, point3(centre.getX(), centre.getY()+high, centre.getZ()), matter, paint);
            sides->add(&tri[2]);

            tri[3] = triangle(p2, p0, point3(centre.getX(), centre.getY()+high, centre.getZ()), matter, paint);
            sides->add(&tri[3]);
            
        }

        ~piramide4(){
            if (sides != nullptr) delete sides;
            if (pln != nullptr) delete[] pln;
            if (tri != nullptr) delete[] tri;
            
        }

        virtual bool hit(const ray& r, hit_record& h, double t_min, double t_max) const override{
            return sides->hit(r, h, t_min, t_max);
        }

        // Setters
        void set_origin(const point3& centre){
            this->centre = centre;
        }
        void set_size(double size){
            this->size = size;
        }
        void set_high(double high){
            this->high = high;
        }
        void set_matter(material* matter){
            this->buffer_matter = matter;
        }
        void set_paint(texture* paint){
            this->buffer_paint = paint;
        }

        // Getters
        void get_all() const{
            std::cout << "Piramide4: " << std::endl;
            std::cout << "Centre: " << centre << std::endl;
            std::cout << "Size: " << size << std::endl;
            std::cout << "High: " << high << std::endl;
            std::cout << "Matter: " << buffer_matter << std::endl;
            std::cout << "Paint: " << buffer_paint << std::endl;
            
        }

        void reinit_piramide()
        {
            point3 dif = point3(size/2, 0, size/2);
            point3 p0 = centre - dif;
            point3 p1 = centre + dif;
            point3 p2 = point3(p1.getX(), p0.getY(), p0.getZ());
            point3 p3 = point3(p0.getX(), p0.getY(), p1.getZ());
            
            sides = new obstacle_list();
            pln = new plane[1];
            tri = new triangle[4];
            
            pln[0] = plane(p0, p2, p1, p3,  buffer_matter, buffer_paint);
            sides->add(&pln[0]);

            
            tri[0] = triangle(p0, p3, point3(centre.getX(), centre.getY()+high, centre.getZ()), buffer_matter, buffer_paint);
            sides->add(&tri[0]);

            tri[1] = triangle(p3, p1, point3(centre.getX(), centre.getY()+high, centre.getZ()), buffer_matter, buffer_paint);
            sides->add(&tri[1]);

            tri[2] = triangle(p1, p2, point3(centre.getX(), centre.getY()+high, centre.getZ()), buffer_matter, buffer_paint);
            sides->add(&tri[2]);

            tri[3] = triangle(p2, p0, point3(centre.getX(), centre.getY()+high, centre.getZ()), buffer_matter, buffer_paint);
            sides->add(&tri[3]);
        }

}; 

#endif