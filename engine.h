#ifndef ENGINE_H
#define ENGINE_H

class engine
{
    private:
        double samples_per_pixel;
        double max_depth;

    public:
        engine(int type){
            if (type == 1){
                samples_per_pixel = 25;
                max_depth = 5;
            }
            else if (type == 2){
                samples_per_pixel = 100;
                max_depth = 25;
            }
            else if (type == 3){
                samples_per_pixel = 400;
                max_depth = 50;
            }
            else{
                samples_per_pixel = 25;
                max_depth = 5;
            }
        }

        //Getters
        double get_samples_per_pixel() const {return samples_per_pixel;}
        double get_max_depth() const {return max_depth;}
};


#endif