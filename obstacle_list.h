#ifndef OBSTACLE_LIST_H
#define OBSTACLE_LIST_H


#include <vector>
#include "scene_base_object.h"


using namespace std;

//Clase lista de obstaculos: Hereda de la clase de objetos padre scene_base_object
class obstacle_list: public scene_base_object{

    
    public:

        //Vector de punteros de objetos
        vector<scene_base_object*> obstacles;
        
        //Constructores 
        obstacle_list(){}
        obstacle_list(scene_base_object* object){add(object);}

        //Limpia el vector
        void clear(){obstacles.clear();}

        void size(){std::cout << obstacles.size() <<std::endl;}

        //Añadir punteros de objetos al vector
        void add(scene_base_object* obstacle){obstacles.push_back(obstacle);}

        //Funcion heredada de la funcion padre
        //Funcion que recibe un rayo y comprueba la interacción de este con todos los objetos de la lista
        //regresa false: si el rayo no golpea nada
        //regresa true: si el rayo golpea algo
        virtual bool hit(const ray& r, hit_record& h, double t_min, double t_max)const override{
            
            //variables dinamicas de la funcion
            hit_record temp_rec; //record temporal 
            bool hit_anything = false; //variable de confirmación
            double closest_so_far = t_max; //rango maximo a tener en cuenta
            
            //Se recorre la lista
            for(scene_base_object* obstacle : obstacles){
                
                //se llama a su funcion hit y se verifica si el rayo golpeó
                if(obstacle->hit(r,temp_rec, t_min, closest_so_far)){
                    hit_anything = true; //actualiza el estado de impacto
                    closest_so_far = temp_rec.t_touch; //define el punto lejano mas cercano hasta el momento
                    h = temp_rec; //actualiza el record original en base al temporal
                }
            }

            return hit_anything;
        }


};

#endif