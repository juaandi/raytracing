#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.h"
#include "point3.h"
#include "pro_stb_image.h"

/* Clase de material
Clase padre que se encarga de heredar su funcion rebote a sus hijas
*/
class texture {
    public:
        virtual point3 value(double u, double v, const point3& p) const = 0;
};

//Textura de color solido
class solid_color : public texture {
    
    //Atributos
    private:
        point3 color_value;
    
    public:
        //Constructores
        solid_color() {}
        solid_color(point3 c) : color_value(c){}
        solid_color(double red, double green, double blue): solid_color(point3(red,green,blue)) {}

        //Funcion que regresa el color de la textura
        virtual point3 value(double u, double v, const point3& p) const override {
            return color_value;
        }
};

//Textura de cuadricula
class checker_texture : public texture {
   
    //Atributos
    private:
        texture* odd; //Para cuadros impares
        texture* even; //Para cuadros pares
    
    public:
        
        //Constructores
        checker_texture() {}
        checker_texture(texture* even, texture* odd): even(even), odd(odd) {}
        checker_texture(point3 c1, point3 c2){}//Toca mirar como putas arreglar este problema de punteros
            

        //Funcion que recibe un punto y 2 coordenadas esfericas y regresa el color de la textura
        virtual point3 value(double u, double v, const point3& p) const override {
            
            //Se calcula el seno de 10 veces la coordenada x, y, z
            //Si el seno es menor a 0, se regresa el color de la textura impar
            //Si el seno es mayor a 0, se regresa el color de la textura par
            double sines = sin(10*p.getX())*sin(10*p.getY())*sin(10*p.getZ());

            if (sines < 0)
                return odd->value(u, v, p);
            else
                return even->value(u, v, p);
        }

};

//Textura de ruido Perlin
class noisy : public texture {
    
    //Atributos
    private:
        perlin noise; //Objeto de la clase perlin
        double scale; //Frecuencia del ruido

    public:

        //Constructores
        noisy():scale(1) {}
        noisy(double sc) : scale(sc) {}


        //Funcion que recibe un punto y 2 coordenadas esfericas y regresa el color de la textura
        virtual point3 value(double u, double v, const point3& p) const override {
            
            //Se regresa el color de la textura de ruido perlin
            return point3(1,1,1) * 0.5 * (1 + sin(scale*p.getZ() + 10*noise.turbulence(p)));
        }

    
};

class picture : public texture {
    
    private:
        unsigned char *data;
        int width, height;
        int bytes_per_scanline;

    public:
        const static int bytes_per_pixel = 3;

        picture(): data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

        picture(const char* filename) {
            int components_per_pixel = bytes_per_pixel;

            data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

            if (!data) {
                std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
                width = height = 0;
            }

            bytes_per_scanline = bytes_per_pixel * width;
        }

        ~picture() {
            delete data;
        }

        virtual point3 value(double u, double v, const point3& p) const override {
            // If we have no texture data, then return solid cyan as a debugging aid.
            if (data == nullptr)
                return point3(0,1,1);

            // Clamp input texture coordinates to [0,1] x [1,0]
            u = limit(u, 0.0, 1.0);
            v = 1 - limit(v, 0.0, 1.0);  // Flip V to image coordinates

            int i = static_cast<int>(u * width);
            int j = static_cast<int>(v * height);

            // Clamp integer mapping, since actual coordinates should be less than 1.0
            if (i >= width)  i = width-1;
            if (j >= height) j = height-1;

            const double color_scale = 1.0 / 255.0;
            auto pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;

            return point3(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
        }

    
};


#endif