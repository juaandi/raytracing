#ifndef UTILITY_FUNC_H
#define UTILITY_FUNC_H
#include <cstdlib>


//Recibe un valor, un minino y un maximo
//Si el valor es menor del minimo returna el valor minimo
//Si el valor es mayor que el maximo returna el valor maximo
double limit(double val, double min_val, double max_val){
    if (val < min_val){ val = min_val;}
    if (val > max_val){ val = max_val;}
    return val;
}

//Returna un numero real aleatorio [0,1)
double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

//Recibe un minimo y un maximo
//Returna un numero real aleatorio [minimo, maximo)
double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

//Recibe un valor en grados
//Returna la conversion de grados a radianes
double from_deg_to_rad(double deg){
    return (deg*3.1416)/180;
}

// Recibe un minimo y un maximo
// Returna un numero entero aleatorio [minimo, maximo)
int random_int(int min, int max){
    return static_cast<int>(random_double(min, max+1));
}

#endif