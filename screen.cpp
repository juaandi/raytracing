#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <iostream>
#include <fstream>
#include <FL/Fl_Choice.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <vector>
#include "cube.h"
#include "sphere.h"
#include "piramide4.h"
#include "background.h"
#include "render.h"
#include "engine.h"
#include "materials_and_textures.h"
#include "parser.h"

using namespace std;

// Declare global variables
Fl_Text_Buffer *textbuf;
Fl_Text_Editor *texteditor;
Fl_Button *loadButton, *saveButton, *showButton, *playButton; // Added showButton
Fl_Box *imageBox, *valueBox, *engineBox;
Fl_JPEG_Image *image = nullptr; // Initialize image to null pointer
Fl_RGB_Image *rgbImage = nullptr;   // Initialize rgbImage to null pointer

// Load ppm image from file
bool load_ppm_image(const std::string& filename, unsigned char*& data, int& width, int& height) {
    
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error: Could not open file!" << std::endl;
    }

    // read ppm header
    std::string magic;
    int max_val;
    infile >> magic >> width >> height >> max_val;

    if (magic != "P3" || max_val != 255) 
    {
            std::cerr << "Error: Unsupported format!" << std::endl;
    }

    // read ppm data
    data = new unsigned char[3 * width * height];
    for (int i = 0; i < width * height * 3; i++)
    {
            int val;
            infile >> val;
            data[i] = (unsigned char) val;
    }
    return true;
}

// Create Fl_RGB_Image from pixel data
Fl_RGB_Image* create_fl_rgb_image(unsigned char* data, int width, int height) {
    
    // Create a new Fl_RGB_Image object with the modified pixel data

    Fl_RGB_Image* rgbImage = new Fl_RGB_Image(data, width, height, 3); 
    
    return rgbImage;

}

// Callback function for Load button
void load_cb(Fl_Widget* widget, void*)
{
    // Create a file chooser dialog
    Fl_Native_File_Chooser chooser;
    chooser.title("Choose a file");
    chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);

    // Show the dialog and check the result
    switch (chooser.show()) {
        case -1:
            cout << "Error: Failed to show file chooser dialog.\n";
            break;
        case 1:
            cout << "Cancel button was pressed.\n";
            break;
        default:
            cout << "Selected file: " << chooser.filename() << "\n";

            // Load the text from the file into the text editor
            ifstream file(chooser.filename());
            if (file.is_open()) {
                string line;
                while (getline(file, line)) {
                    textbuf->append(line.c_str());
                    textbuf->append("\n");
                }
                file.close();

            } else {
                cout << "Error: Failed to open file.\n";
            }

            break;
    }
}

// Callback function for Save button
void save_cb(Fl_Widget* widget, void*)
{
    // Save the contents of the text editor to a file
    ofstream file("files/scene.txt");
    if (file.is_open()) {
        file << textbuf->text();
        file.close();
        cout << "File saved as scene.txt in /files/\n";
    } else {
        cout << "Error: Failed to open file for writing.\n";
    }
}

// Callback function for Show button
void show_cb(Fl_Widget* widget, void*)
{
    // Create a new window to show the image
    Fl_Window *win = new Fl_Window(640, 480, "Image");
    win->begin();
    
    // Create a image box and set its image
    Fl_Box *box = new Fl_Box(10, 10, 600, 400); // Fl_Box( x means left, y means top, width, height)
    box->image(rgbImage);
    win->show();
    win->end(); 
 
}

void save_as_png(Fl_Widget* widget, void*)
{
    // With ImageMagick, convert the image to png sending a command to the terminal
    system("magick output/output.ppm output/output.png");
}

void save_as_jpg(Fl_Widget* widget, void*)
{
    // With ImageMagick, convert the image to jpg sending a command to the terminal
    system("magick output/output.ppm output/output.jpeg");
}

void close_cb(Fl_Widget* widget, void*)
{
    // Close the window
    widget->parent()->hide();

    // Stop the program
    exit(0);
}

// Callback function for Play button
void play_cb(Fl_Widget* widget, void*)
{    
    // Get the label of the engineBox
    const char* engineBoxLabel = engineBox->label();

    // Convert the label to a string
    std::string engineBoxLabelString(engineBoxLabel);

    // Convert the string to an integer
    int engineBoxLabelInt = std::stoi(engineBoxLabelString);

    std::cout << "Engine: " << engineBoxLabelInt << std::endl;

    
    // PARSER
    XMLParser parser("files/scene.txt");
    std::string line;
    int num_objects;
    std::vector<sphere> spheres;
    std::vector<std::unique_ptr<cube>> cubes;
    //std::vector<std::unique_ptr<piramide4>> piramides;
    std::vector<plane> planes;

    // Read the camara
    camera_data cam_data = parser.get_camara();

    //CAMARA
    point3 vup(0,-1,0); // Vector UP: indica hacia donde esta arriba
    const double aspect_ratio = cam_data.aspect_ratio; //Ratio de la imagen
    point3 lookfrom = cam_data.look_from; //Punto de vista (x,y,z
    point3 lookat = cam_data.look_at; //Punto hacia donde mira (x,y,z)
    double focus_dis = cam_data.focus_dist; // We recommend between 1 and 10
    double hole = cam_data.hole; // We recommend between 0 and 1
    double vfov = cam_data.fov; // We recommend between 0 and 90 (90 = fisheye)
    double height = cam_data.height;

    background custom_background(cam_data.background);

    camara cam(lookfrom, lookat, vup, vfov, aspect_ratio, height, hole, focus_dis);

    

    //IMAGE
    Image<int> img(cam.get_width(), cam.get_height());

    //WORLD
    obstacle_list world;
    
    // Use the method to get the spheres
    parser.get_spheres(spheres);

    //Add to world all the spheres in the vector
    for (int i = 0; i < spheres.size(); i++) {
        world.add(&spheres[i]);
    }

    // Use the method to get the planes
    parser.get_planes(planes);

    //Add to world all the planes in the vector
    for (int i = 0; i < planes.size(); i++) {
        world.add(&planes[i]);
    }
    
    // Use the method to get the cubes
    parser.get_cubes(cubes);

    //Add to world all the cubes in the vector
    for (int i = 0; i < cubes.size(); i++) {
        world.add(cubes[i].get());
    }

    std::vector<std::unique_ptr<piramide4>> piramides;
    
    // Use the method to get the piramides
    parser.get_pyramids4(piramides);

    //Add to world all the piramides in the vector
    for (int i = 0; i < piramides.size(); i++) {
        world.add(piramides[i].get());
    }
    
    //THREADS
    //int num_threads = thread::hardware_concurrency(); //Obtiene el número de núcleos disponibles en la máquina
    int num_threads = 4;

    engine eng(engineBoxLabelInt);
    

    parallel_render(num_threads, img, custom_background  ,world, cam, eng);
    cerr << "\nDone.\n";

    //SAVE
    string directory{ "output/" };
    string filename{ directory + "output.ppm" };
    cerr << "\nSaving image.\n";
    img.save(filename);
    cerr << "\nDone Saving.\n";

    unsigned char* data = nullptr;
        int width_, height_;

        if (!load_ppm_image("output/output.ppm", data, width_, height_)) 
        {
            std::cerr << "Error loading image" << std::endl;
        }

    rgbImage = create_fl_rgb_image(data, width_, height_);
    
    valueBox->label(("Percentage: " + to_string(2)).c_str());
    Fl::check(); 


        // Create a button to show the image
        Fl_Window *win = new Fl_Window(900, 600, "Image");
        win->begin();

        // Create a image box and set its image
        Fl_Box *box = new Fl_Box(150, 100, 600, 200); // Fl_Box( x means left, y means top, width, height)
        box->image(rgbImage);
        
        // Add a title to the image
        Fl_Box *title = new Fl_Box(100, 50, 750, 50, "Image rendered from the scene file");
            
        // Add a text to warn the user
        Fl_Box *warning = new Fl_Box(100, 290, 750, 300, "Warning: The image proportions possibly are not correct.\n For viewing the image correctly, please save the image and open it in a new window.");
            
        // Add a button to save the image as png
        Fl_Button *save_png = new Fl_Button(330, 470, 100, 50, "Save as png");
        save_png->callback(save_as_png);

        // Add a button to save the image as jpg
        Fl_Button *save_jpg = new Fl_Button(530, 470, 100, 50, "Save as jpg");
        save_jpg->callback(save_as_jpg);
            
        // Add a button to close the window and stop the program
        Fl_Button *close = new Fl_Button(330+(530-330)/2, 530, 100, 50, "Thanks! Bye");
        close->callback(close_cb);

        
        win->show();
        win->end();          

}

void choice_cb(Fl_Widget* widget, void*)
{
    Fl_Choice* choice = (Fl_Choice*)widget;
    const char* selected_option = choice->text();

    cout << "Selected option: " << selected_option << endl;
    
    // Set the engine to the selected option
    // 1 if Elden Ring, 2 if Mortal Kombat, 3 if Super Mario

    if (strcmp(selected_option, "Elden Ring") == 0) 
    {
        engineBox->label("3");
    }
    else if (strcmp(selected_option, "Mortal Kombat") == 0) 
    {
        engineBox->label("2");
    }
    else if (strcmp(selected_option, "Super Mario 32 bits") == 0) 
    {
        engineBox->label("1");
    }

}

int main(int argc, char **argv)
{
    // Create a window
    Fl_Window *win = new Fl_Window(720, 480, "JUAN M CARVAJAL et JUAN RAMIREZ - 2023");
    win->begin();
    
    // Create a text editor and set its buffer
    textbuf = new Fl_Text_Buffer();
    texteditor = new Fl_Text_Editor(20, 20, 580, 460, "Text Editor");
    texteditor->buffer(textbuf);
    

    // Create a load button and set its callback function
    loadButton = new Fl_Button(610, 20, 100, 30, "Load file");
    loadButton->callback(load_cb);

    // Create a save button and set its callback function
    saveButton = new Fl_Button(610, 60, 100, 30, "Confirm file");
    saveButton->callback(save_cb);

    // Create a button to play the code
    playButton = new Fl_Button(610, 180, 100, 30, "Play");
    playButton->callback(play_cb);

    // Create a text with the text "Motor:"
    Fl_Box *motorBox = new Fl_Box(610, 110, 100, 30, "Render engine:");
    
    // Create a space to show the value of the variable
    valueBox = new Fl_Box(500, 220, 320, 30, "");

    // Create a variable to show a value
    engineBox = new Fl_Box(610, 250, 100, 30, "Engine: ");

    // Extract the value of the label of engineBox and store it in a string
    string engine_str = engineBox->label();
    
    // Create a combobox with 3 options
    Fl_Choice *choice = new Fl_Choice(605, 135, 120, 30, "");
    choice->add("Elden Ring");
    choice->add("Mortal Kombat");
    choice->add("Super Mario 32 bits");
    choice->callback(choice_cb);
    
    win->end();
    win->show(argc, argv);
    return Fl::run();
    

}
