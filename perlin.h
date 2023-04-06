#ifndef PERLIN_H
#define PERLIN_H

#include "point3.h"

//Clase que se encarga de generar ruido perlin
class perlin {
    
    
    private:

        //Atributos
        static const int point_count = 256; 
        point3* ranvec; 
        int* perm_x;
        int* perm_y;
        int* perm_z;

        //Funcion que crea un arreglo de enteros y lo desordena
        static int* perlin_generate_perm() {
            
            //Se crea un arreglo de enteros de tamaño point_count
            int* p = new int[point_count];

            //Se llena el arreglo con numeros del 0 al point_count
            for (int i = 0; i < perlin::point_count; i++)
                p[i] = i;

            //Se desordena el arreglo
            permute(p, point_count);

            //Se regresa el arreglo
            return p;
        }

        //Funcion que recibe un arreglo de enteros y su tamaño, y lo desordena
        static void permute(int* p, int n) {
            for (int i = n-1; i > 0; i--) {
                int target = random_int(0, i);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }

        //Funcion que recibe un arreglo de puntos de 3 dimensiones de tamaño 2 cada uno, y 3 valores entre -1 y 1
        static double perlin_interp(point3 c[2][2][2], double u, double v, double w) {
            
            //Hermite cubic
            double uu = u*u*(3-2*u);
            double vv = v*v*(3-2*v);
            double ww = w*w*(3-2*w);
            double accum = 0.0;

            //Se llena el acumulador con el truco de Perlin
            for (int i=0; i < 2; i++)
                for (int j=0; j < 2; j++)
                    for (int k=0; k < 2; k++) {
                        point3 weight_v(u-i, v-j, w-k);
                        accum += (i*uu + (1-i)*(1-uu))
                               * (j*vv + (1-j)*(1-vv))
                               * (k*ww + (1-k)*(1-ww))
                               * dot(c[i][j][k], weight_v);
                    }

            return accum;
        }
    
    public:
        //Constructor
        perlin() {

            //Se define randfloat como un arreglo de doubles de tamaño point_count
            ranvec = new point3[point_count];

            //Se llena el arreglo con numeros aleatorios entre 0 y 1
            for (int i = 0; i < point_count; ++i) {
                ranvec[i] = vec_unit(random_point(-1,1));
            }


            //Llena los arreglos de enteros con numeros aleatorios entre 0 y 255
            perm_x = perlin_generate_perm();
            perm_y = perlin_generate_perm();
            perm_z = perlin_generate_perm();
        }

        //Destructor
        ~perlin() {
            delete[] ranvec;
            delete[] perm_x;
            delete[] perm_y;
            delete[] perm_z;
        }

        //Funcion que recibe un punto de 3 dimensiones y regresa un valor entre 0 y 1
        double noise(const point3& p) const {
            
            //Se obtienen los valores de u, v y w como valores entre 0 y 1
            double u = p.getX() - floor(p.getX());
            double v = p.getY() - floor(p.getY());
            double w = p.getZ() - floor(p.getZ());

            //Se obtienen los valores de i, j y k como enteros de los valore de p
            int i = static_cast<int>(floor(p.getX()));
            int j = static_cast<int>(floor(p.getY()));
            int k = static_cast<int>(floor(p.getZ()));
            point3 c[2][2][2];

            //Se llena el arreglo c con valores pseudoaleatorios
            for (int di=0; di < 2; di++)
                for (int dj=0; dj < 2; dj++)
                    for (int dk=0; dk < 2; dk++)
                        c[di][dj][dk] = ranvec[
                            perm_x[(i+di) & 255] ^
                            perm_y[(j+dj) & 255] ^
                            perm_z[(k+dk) & 255]
                        ];

            //Se llama a la funcion que interpola los valores de c, u, v y w
            return perlin_interp(c, u, v, w);
            
        }

        double turbulence(const point3& p, int depth=5) const {
            
            double accum = 0.0;
            point3 temp_p = p;
            double weight = 1.0;

            for (int i = 0; i < depth; i++) {
                accum += weight*noise(temp_p);
                weight *= 0.5;
                temp_p = 2*temp_p;
            }

            return fabs(accum);
        }

};

#endif