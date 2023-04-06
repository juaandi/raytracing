#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "point3.h"

class background {

    private:
        point3 values[2];
    
    public:
        background():values{point3(0,0,0), point3(0,0,0)}{}
        background(point3 color_r):values{color_r, color_r}{}
        background(point3 color1, point3 color2):values{color1, color2}{}
        background(std::string theme)
        {
            if (theme == "galactic")
            {
                values[0] = point3(0.0, 0.0, 0.3);
                values[1] = point3(0.8, 0.8, 1.0);
            }
            else if (theme == "cityscape")
            {
                values[0] = point3(0.0, 0.0, 0.0);
                values[1] = point3(0.5, 0.5, 0.2);
            }
            else if (theme == "tropical")
            {
                values[0] = point3(0.0, 0.4, 0.0);
                values[1] = point3(0.9, 1.0, 0.7);
            }
            else if (theme == "underwater")
            {
                values[0] = point3(0.0, 0.1, 0.2);
                values[1] = point3(0.0, 0.8, 1.0);
            }
            else if (theme == "castle")
            {
                values[0] = point3(0.2, 0.2, 0.2);
                values[1] = point3(0.9, 0.8, 0.5);
            }
            else if (theme == "rainforest")
            {
                values[0] = point3(0.0, 0.3, 0.0);
                values[1] = point3(0.8, 0.9, 0.5);
            }
            else if (theme == "desert")
            {
                values[0] = point3(0.9, 0.7, 0.2);
                values[1] = point3(1.0, 0.9, 0.5);
            }
            else if (theme == "futuristic")
            {
                values[0] = point3(0.1, 0.0, 0.2);
                values[1] = point3(0.5, 0.1, 0.5);
            }
            else if (theme == "winter")
            {
                values[0] = point3(0.0, 0.1, 0.3);
                values[1] = point3(0.8, 1.0, 1.0);
            }
            else if (theme == "gothic")
            {
                values[0] = point3(0.1, 0.1, 0.1);
                values[1] = point3(0.5, 0.0, 0.0);
            }
            else if (theme == "magical")
            {
                values[0] = point3(0.0, 0.1, 0.0);
                values[1] = point3(0.5, 0.8, 0.9);
            }
            else if (theme == "haunted")
            {
                values[0] = point3(0.2, 0.1, 0.1);
                values[1] = point3(0.8, 0.6, 0.6);
            }
            else if (theme == "sunrise")
            {
                values[0] = point3(0.0, 0.2, 0.3);
                values[1] = point3(1.0, 0.8, 0.6);
            }
            else if (theme == "enchanted")
            {
                values[0] = point3(0.1, 0.0, 0.2);
                values[1] = point3(0.8, 0.6, 0.2);
            }
            else if (theme == "jungle")
            {
                values[0] = point3(0.0, 0.2, 0.0);
                values[1] = point3(0.8, 0.9, 0.5);
            }
            else
            {
                values[0] = point3(0.5,0.5,0.5);
                values[1] = point3(0.8,0.8,0.8);
            }
                
        }
        //getters
        point3 get_color_1() const{return values[0];}
        point3 get_color_2() const{return values[1];}
};

    


#endif