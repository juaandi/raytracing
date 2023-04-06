#ifndef MATERIALS_AND_TEXTURES_H
#define MATERIALS_AND_TEXTURES_H

#include "cube.h"
#include "sphere.h"
#include "point3.h"
#include "triangle.h"

    //LIST OF MATERIALS
    blurry fuzzy;
    metal mercury(0.0);
    metal zinc(0.1);
    metal aluminum(0.2);
    metal nickel(0.3);
    metal copper(0.4);
    metal brass(0.5);
    metal bronze(0.6);
    metal silver(0.7);
    metal gold(0.8);
    metal platinum(0.9);
    metal chromium(1.0);


    //LIST OF TEXTURES
    solid_color white(point3(1.0, 1.0, 1.0));
    solid_color black(point3(0.0, 0.0, 0.0));
    solid_color red(point3(1.0, 0.0, 0.0));
    solid_color green(point3(0.0, 1.0, 0.0));
    solid_color blue(point3(0.0, 0.0, 1.0));
    solid_color yellow(point3(1.0, 1.0, 0.0));
    solid_color magenta(point3(1.0, 0.0, 1.0));
    solid_color cyan(point3(0.0, 1.0, 1.0));
    solid_color gray(point3(0.5, 0.5, 0.5));
    solid_color dark_gray(point3(0.2, 0.2, 0.2));
    solid_color light_gray(point3(0.8, 0.8, 0.8));
    solid_color pink(point3(1.0, 0.5, 0.5));
    solid_color orange(point3(1.0, 0.5, 0.0));
    solid_color purple(point3(0.5, 0.0, 1.0));
    solid_color lime(point3(0.5, 1.0, 0.0));
    solid_color turquoise(point3(0.0, 1.0, 1.0));
    solid_color navy(point3(0.0, 0.0, 0.5));
    solid_color olive(point3(0.5, 0.5, 0.0));
    solid_color bright_white(point3(1.0, 1.0, 1.0));
    solid_color bright_red(point3(1.0, 0.2, 0.2));
    solid_color bright_green(point3(0.2, 1.0, 0.2));
    solid_color bright_blue(point3(0.2, 0.2, 1.0));
    solid_color bright_yellow(point3(1.0, 1.0, 0.2));
    solid_color bright_purple(point3(0.8, 0.2, 1.0));
    solid_color bright_cyan(point3(0.2, 1.0, 1.0));
    solid_color bright_magenta(point3(1.0, 0.2, 1.0));
    solid_color bright_orange(point3(1.0, 0.6, 0.0));
    solid_color bright_pink(point3(1.0, 0.4, 0.7));
    solid_color bright(point3(3,3,3));
    solid_color superbright(point3(10, 10, 10));

    checker_texture chess(&white, &black);

    picture cucuball("pictures/CUCU.jpeg");
    picture vaneball("pictures/VANE.jpeg");
    picture pipeball("pictures/PIPE.jpeg");
    picture viniball("pictures/VINI.jpeg");
    picture juandiball("pictures/JUANDI.jpeg");
    
    noisy noise(3);

    light candle(&white);
    light flashlight(&bright);
    light lighthouse(&superbright);

#endif