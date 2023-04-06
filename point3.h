#ifndef POINT3_H
#define POINT3_H

#include <iostream>
#include <cmath>
#include "utility_func.h"

using namespace std;

/*------Esta clase modela el comportamiento de varios elementos dentro del ray-tracer--------

->Puntos en el espacio 3D
->Vectores en el espacio 3D
->Colo rgb 

Define todas las operaciones que esten relacionadas con objetos tipo <point3>

*/

class point3{
    
    private:
        double p[3]; //3 posiciones de la clase

    public:

        //Constructores
        point3():p{0,0,0}{} 
        point3(double p0, double p1, double p2):p{p0,p1,p2}{}


        //Getters
        double getX()const{return p[0];}
        double getY()const{return p[1];}
        double getZ()const{return p[2];}

        //Setters
        void setX(double x){p[0]=x;}
        void setY(double y){p[1]=y;}
        void setZ(double z){p[2]=z;}

        //Definicion para regresar el valor en la posicion i de un objeot point3
        double operator[](int i) const { return p[i]; }
        double& operator[](int i) { return p[i]; }


        //Returna la distancia cuadrada : x^2 + y^2 + z^2
        double getLengthSquare()const{return p[0]*p[0]+p[1]*p[1]+p[2]*p[2];}
        

        //Returna el modulo de un vector: sqrt(x^2 + y^2 + z^2)
        double getModulo()const{return sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);}


        //Permite imprimir objetos tipo <point3> en el formato descrito
        friend ostream& operator<<(ostream& os, const point3& u) 
        {
            os << u.p[0] << ", " << u.p[1] << ", " << u.p[2];
            return os;
        }


        //Returna <true> si los valores absolutos individuales del objeto son menores a un threshold
        bool near_zero() const {
            const double threshold = 1e-8;
            return (fabs(p[0]) < threshold) && (fabs(p[1]) < threshold) && (fabs(p[2]) < threshold);
        }

};


//Definicion de la multiplicacion de dos objetos <point3>
point3 operator*(const point3 &u, const point3 &v) {
    return point3(u.getX() * v.getX(), u.getY() * v.getY(), u.getZ() * v.getZ());
}


/*-Definicion de la multiplicacion de un objeto <point3> con un double-*/
point3 operator *(double t, const point3& u){
    return point3(u.getX()*t, u.getY()*t, u.getZ()*t);
}
point3 operator *(const point3& u, double t){
    return t*u;
}


//Definicion de la division de un objeto <point3> con un double>
point3 operator /(const point3& u,double t){
    return (1/t)*u;
}


//Definicion de la resta de 2 objetos tipo <point3>
point3 operator -(const point3& u,const point3& v){
    return point3(u.getX()-v.getX(),u.getY()-v.getY(),u.getZ()-v.getZ());
}


//Definicion de la suma de 2 objetos tipo <point3>
point3 operator +(const point3& u,const point3& v){
    return point3(u.getX()+v.getX(),u.getY()+v.getY(),u.getZ()+v.getZ());
}

//Definicion de la comparacion de 2 point3 objects
bool operator ==(const point3& u, const point3& v){
    return (u.getX() == v.getX() && u.getY() == v.getY() && u.getZ() == v.getZ());
} 

//Definicion de diferente entre 2 puntos
bool operator !=(const point3& u, const point3& v){
    return !(u == v);
}


//Recibe un vector_in tipo <point3>
//Returna el vector unitario de vector_in <point3> 
point3 vec_unit(const point3& u){
    return u/u.getModulo();
}

//Recibe 2 vectores <point3>
//Returna el producto punto entre estos 2 <double>
double dot(const point3& u, const point3& v){
    return (u.getX()*v.getX()+u.getY()*v.getY()+u.getZ()*v.getZ());
}

// Angle between
inline double angle_between(const point3 &u, const point3 &v) {
    return acos(dot(u, v)/(u.getModulo() * v.getModulo()));
}


//Recibe 2 vectores <point3>
//Returna el producto cruz entre estos 2 <point3>
//ATENCION: el orden de entrada de los vectores importa
point3 cross(const point3& u, const point3& v){
    return point3(u.getY() * v.getZ() - u.getZ() * v.getY(),
                u.getZ() * v.getX() - u.getX() * v.getZ(),
                u.getX() * v.getY() - u.getY() * v.getX());
}


//Returna un <point3> con componentes reales aleatorias en el rango [0,1)
point3 random_point(){
    return point3(random_double(), random_double(), random_double());
}


//Recibe un valor minimo y maximo
//Returna un <point3> con componentes reales aleatorias en el rango [minimo, maximo)
point3 random_point(double min, double max){
    return point3(random_double(min, max), random_double(min, max), random_double(min, max));
}


//Crea un punto <point3> que esta comprendido dentro de una esfera de radio unitario
//Returna este punto <point3> 
point3 rand_point_in_unit_sphere(){
    while(true){
        point3 p = random_point(-1,1);
        if (p.getLengthSquare() <= 1){
            return p;
        }
    }
}

//Returna un vector unitario apuntando al punto dentro de una esfera de radio unitario
point3 rand_vec_in_unit_sphere(){
    return vec_unit(rand_point_in_unit_sphere());
}


/* --- MODELA EL COMPORTAMIENTO DE REFLEXION TOTAL --- */
//Recibe un vector entrante a una superficio y el vector normal al punto de corte del vector entrante con la superficie
//Regresa un vector saliente completamente reflejado <point3> 
point3 reflect(const point3& v, const point3& n) {
    return v - 2*dot(v,n)*n;
}


/* --- MODELA EL COMPORTAMIENTO DE REFRACCION TOTAL --- */
/* --- RECIBE ---
->Vector entrante a la superficie
->Normal al punto de corte del vector entrante con la superficie
->Relacion entre el indice de refraccion del espacio de entrada y el espacio de salida index_ext/index_int

Returna el vector de salida completamente refractado (sigue la ley de snell para esto) 
*/
point3 refract(const point3& v, const point3& n, double relation){
    double cos_theta = fmin(dot(-1*v, n), 1.0);
    point3 refra_perpendicular = relation*(v + cos_theta*n);
    point3 refra_parallel = -1*sqrt(fabs(1 - refra_perpendicular.getLengthSquare()))*n;
    return refra_parallel + refra_perpendicular;

}


//Returna un punto <point3> centrado en z=0 y comprendido en un circulo unitario en el plano xy
point3 random_in_unit_disk() {
    while (true) {
        point3 p = point3(random_double(-1,1), random_double(-1,1), 0);
        if (p.getModulo() < 1){
            return p;
        }
    }
}
#endif