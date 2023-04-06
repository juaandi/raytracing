/*
Program to parse a text description of a scene and create a structure
storing the objects in the scene. The only object type supported is
a sphere. The scene description is read from standard input.
*/
#include "sphere.h"
#include "triangle.h"
#include "cube.h"
#include <iostream>
#include "piramide4.h"
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include "materials_and_textures.h"

// Union of all the possible camera data
class camera_data 
        {
            public:
                camera_data() {}
                point3 look_from;
                point3 look_at;
                double fov;
                double hole;
                double focus_dist;
                double aspect_ratio;
                double height;
                std::string background;
            
        }; 

auto extractMaterial(std::string& material_description)
{
    material* m_material;

    if (material_description == "mercury")
        m_material = &mercury;
    else if (material_description == "zinc")
        m_material = &zinc;
    else if (material_description == "aluminum")
        m_material = &aluminum;
    else if (material_description == "nickel")
        m_material = &nickel;
    else if (material_description == "copper")
        m_material = &copper;
    else if (material_description == "brass")
        m_material = &brass;
    else if (material_description == "bronze")
        m_material = &bronze;
    else if (material_description == "silver")
        m_material = &silver;
    else if (material_description == "gold")
        m_material = &gold;
    else if (material_description == "platinum")
        m_material = &platinum;
    else if (material_description == "chromium")
        m_material = &chromium;
    else if (material_description == "fuzzy")
        m_material = &fuzzy;
    else if (material_description == "candle")
        m_material = &candle;
    else if (material_description == "lighthouse")
        m_material = &lighthouse;
    else if (material_description == "flashlight")
        m_material = &flashlight;
    
    return m_material;
}

auto extractTexture(std::string& texture_description)
{
    texture* m_texture;

    if (texture_description == "white")
        m_texture = &white;
    else if (texture_description == "black")
        m_texture = &black;
    else if (texture_description == "red")
        m_texture = &red;
    else if (texture_description == "green")
        m_texture = &green;
    else if (texture_description == "blue")
        m_texture = &blue;
    else if (texture_description == "yellow")
        m_texture = &yellow;
    else if (texture_description == "magenta")
        m_texture = &magenta;
    else if (texture_description == "cyan")
        m_texture = &cyan;
    else if (texture_description == "gray")
        m_texture = &gray;
    else if (texture_description == "dark_gray")
        m_texture = &dark_gray;
    else if (texture_description == "light_gray")
        m_texture = &light_gray;
    else if (texture_description == "pink")
        m_texture = &pink;
    else if (texture_description == "orange")
        m_texture = &orange;
    else if (texture_description == "purple")
        m_texture = &purple;
    else if (texture_description == "lime")
        m_texture = &lime;
    else if (texture_description == "turquoise")
        m_texture = &turquoise;
    else if (texture_description == "navy")
        m_texture = &navy;
    else if (texture_description == "olive")
        m_texture = &olive;
    else if (texture_description == "bright_white")
        m_texture = &bright_white;
    else if (texture_description == "bright_red")
        m_texture = &bright_red;
    else if (texture_description == "bright_green")
        m_texture = &bright_green;
    else if (texture_description == "bright_blue")
        m_texture = &bright_blue;
    else if (texture_description == "bright_yellow")
        m_texture = &bright_yellow;
    else if (texture_description == "bright_purple")
        m_texture = &bright_purple;
    else if (texture_description == "bright_cyan")
        m_texture = &bright_cyan;
    else if (texture_description == "bright_magenta")
        m_texture = &bright_magenta;
    else if (texture_description == "bright_orange")
        m_texture = &bright_orange;
    else if (texture_description == "bright_pink")
        m_texture = &bright_pink;
    else if (texture_description == "bright")
        m_texture = &bright;
    else if (texture_description == "superbright")
        m_texture = &superbright;
    else if (texture_description == "chess")
        m_texture = &chess;
    else if (texture_description == "cucuball")
        m_texture = &cucuball;
    else if (texture_description == "vaneball")
        m_texture = &vaneball;
    else if (texture_description == "pipeball")
        m_texture = &pipeball;
    else if (texture_description == "viniball")
        m_texture = &viniball;
    else if (texture_description == "juandiball")
        m_texture = &juandiball;
    else if (texture_description == "noise")
        m_texture = &noise;
    return m_texture;

}

std::string extractNumber(std::string& line)
{
    // Find the position of the first digit
    size_t pos = line.find_first_of("+-0123456789");

    // Find the position of the last digit
    size_t end = line.find_last_of("0123456789");

    // Extract the substring containing the number
    std::string number = line.substr(pos, end - pos + 1);

    return number;
}

std::string extractText(std::string& line)
{
    // The text is enclosed in quotation marks
    size_t pos = line.find_first_of("\"");
    size_t end = line.find_last_of("\"");

    // Extract the substring containing the text
    std::string text = line.substr(pos + 1, end - pos - 1);

    return text;
}

void extractComponents(std::vector<double>& components, std::string line)
{
    // The line is in the format x,y,z so we can extract the components
    // by splitting the line at the commas

    // Define a vector to store the center coordinates
    std::string delimiter = ",";

    // Find the position of the first comma
    size_t pos = line.find(delimiter);

    // Extract the first component
    std::string number = line.substr(0, pos);

    // Convert the string to a double
    components.push_back(std::stod(number));

    // Find the position of the second comma
    size_t end = line.find(delimiter, pos + 1);

    // Extract the second component
    number = line.substr(pos + 1, end - pos - 1);

    // Convert the string to a double
    components.push_back(std::stod(number));

    // Extract the third component
    number = line.substr(end + 1);

    // Convert the string to a double
    components.push_back(std::stod(number));
}

class XMLParser 
{

    std::string m_filename;
    std::ifstream m_file;
    
    public:
    // Constructor receives the name of the file to parse and opens it
    XMLParser(const std::string& filename) : m_filename(filename), m_file(filename) 
    {
        if (!m_file.is_open()) 
        {
            throw std::runtime_error("Error: Could not open file " + filename);
        }
    }

    // Destructor closes the file
    ~XMLParser() 
    {
        m_file.close();
    }

    void read_line(std::string& line, bool skip_empty_lines = true, bool print_line = false) 
    {
        std::getline(m_file, line);

        // Remove leading and trailing whitespace
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) {
            return !std::isspace(ch);
        }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), line.end());

        if (print_line) 
        {
            std::cout << line << std::endl;
        }

        if (skip_empty_lines) 
        {
            while (line.empty()) 
            {
                read_line(line, skip_empty_lines, print_line);
            }
        }
    }

    int count_objects() 
    {
        std::string line;
        int count = 0;
        while (std::getline(m_file, line)) 
        {
            if (line.find("<object") != std::string::npos) 
            {
                count++;
            }
        }
        std::cout << "Number of objects: " << count << std::endl;

        // Reset the file pointer to the beginning of the file
        m_file.clear();

        m_file.seekg(0, std::ios::beg);

        return count;
    }

    void go_to_next_object(std::string& line) 
    {
        while (line.find("<object") == std::string::npos) 
        {
            read_line(line);
        }
    }

    std::string get_object_type(std::string& line) 
    {
        // Find the position of the first quotation mark
        size_t pos = line.find_first_of("\"");

        // Find the position of the last quotation mark
        size_t end = line.find_last_of("\"");

        // Extract the substring containing the object type
        std::string type = line.substr(pos + 1, end - pos - 1);

        std::cout << "Object type: " << type << std::endl;

        // Read the next line
        read_line(line);

        return type;
    }

    void get_sphere_data(std::string& line, sphere& sphere) 
    {
        while (line.find("<\\object>") == std::string::npos) 
        {
            
            if (line.find("<radius>") != std::string::npos) 
            {
                std::string number = extractNumber(line);
                sphere.set_radius(std::stod(number));
            }
            else if (line.find("<texture>") != std::string::npos)  // color is texture
            {
                std::string text = extractText(line);
                auto texture = extractTexture(text);
                sphere.set_paint(texture);

            
            }
            else if (line.find("<material>") != std::string::npos)
            {
                std::string text = extractText(line);
                auto material = extractMaterial(text);
                sphere.set_matter(material);
            }
            else if (line.find("<center>") != std::string::npos) 
            {
                
                // Extract the raw center coordinates
                std::string text = extractNumber(line);

                // Extract the center coordinates
                std::vector<double> components;
                extractComponents(components, text);

                // Set the center coordinates
                point3 center(components[0], components[1], components[2]);
                sphere.set_origin(center);
                
            }

            read_line(line);
            
        }  
    }

    void get_spheres(std::vector<sphere>& spheres) 
    {
        std::string line;
        int num_objects;

        // Count the number of objects in the scene
        num_objects = count_objects();

        // For each object, read the object type
        for (int i = 0; i < num_objects; i++) 
        {
            std::string type;
            go_to_next_object(line);
            type = get_object_type(line);

            // If the object is a sphere, read the sphere data and create a sphere object
            if (type == "sphere") 
            {
                // Create a sphere object
                sphere sph;
                get_sphere_data(line, sph);

                // Add the sphere to the vector
                spheres.push_back(sph);
            }
        }
        // Reset the file pointer to the beginning of the file
        m_file.clear();

        m_file.seekg(0, std::ios::beg);
    }

 
    
    void get_pyramids4(std::vector<std::unique_ptr<piramide4>>& pyramids4) 
    {
        std::string line;
        int num_objects;

        // Count the number of objects in the scene
        num_objects = count_objects();

        // For each object, read the object type
        for (int i = 0; i < num_objects; i++) 
        {
            std::string type;
            go_to_next_object(line);
            type = get_object_type(line);

            // If the object is a sphere, read the sphere data and create a sphere object
            if (type == "pyramid") 
            {   
                std::unique_ptr<piramide4> pyramid4_ = std::make_unique<piramide4>();


                get_pyramid_data(line, *pyramid4_);

                pyramid4_->get_all();

                // Add the cube to the vector
                pyramids4.push_back(std::move(pyramid4_));
            }
        }

    }


    void get_cubes(std::vector<std::unique_ptr<cube>>& cubes) {
    std::string line;
    int num_objects;

    // Count the number of objects in the scene
    num_objects = count_objects();

    // For each object, read the object type
    for (int i = 0; i < num_objects; i++) {
        std::string type;
        go_to_next_object(line);
        type = get_object_type(line);

        // If the object is a cube, read the cube data and create a cube object
        if (type == "cube") {
            // Create a cube object
            std::unique_ptr<cube> cube_ = std::make_unique<cube>();
            get_cube_data(line, *cube_);

            
            // Add the cube to the vector
            cubes.push_back(std::move(cube_));
        }
    }
    // Reset the file pointer to the beginning of the file
    m_file.clear();

    m_file.seekg(0, std::ios::beg);
    
} 

    void get_plane_data(std::string& line, plane& plane_) 
    {
        while (line.find("<\\object>") == std::string::npos) 
        {
            
            if (line.find("<texture>") != std::string::npos)  // color is texture
            {
                std::string text = extractText(line);
                auto texture = extractTexture(text);
                plane_.set_paint(texture);            
            }
            else if (line.find("<material>") != std::string::npos)
            {
                std::string text = extractText(line);
                auto material = extractMaterial(text);
                plane_.set_matter(material);
            }
            else if (line.find("<bottom_left_corner") != std::string::npos) 
            {
                
                // Extract the raw center coordinates
                std::string text = extractNumber(line);

                // Extract the center coordinates
                std::vector<double> components;
                extractComponents(components, text);

                // Set the center coordinates
                point3 bottom_left_corner(components[2], components[1], components[0]);
                plane_.set_v1(bottom_left_corner);
            }
            else if (line.find("<bottom_right_corner") != std::string::npos) 
            {
                
                // Extract the raw center coordinates
                std::string text = extractNumber(line);

                // Extract the center coordinates
                std::vector<double> components;
                extractComponents(components, text);

                // Set the center coordinates
                point3 bottom_left_corner(components[2], components[1], components[0]);
                plane_.set_v2(bottom_left_corner);
            }
            else if (line.find("<upper_left_corner") != std::string::npos) 
            {
                
                // Extract the raw center coordinates
                std::string text = extractNumber(line);

                // Extract the center coordinates
                std::vector<double> components;
                extractComponents(components, text);

                // Set the center coordinates
                point3 bottom_left_corner(components[2], components[1], components[0]);
                plane_.set_v3(bottom_left_corner);
            }
            else if (line.find("<upper_right_corner") != std::string::npos) 
            {
                
                // Extract the raw center coordinates
                std::string text = extractNumber(line);

                // Extract the center coordinates
                std::vector<double> components;
                extractComponents(components, text);

                // Set the center coordinates
                point3 bottom_left_corner(components[2], components[1], components[0]);
                plane_.set_v4(bottom_left_corner);
            }

            read_line(line);
        }
    }

    void get_planes(std::vector<plane>& planes) 
    {
    std::string line;
    int num_objects;

    // Count the number of objects in the scene
    num_objects = count_objects();

    // For each object, read the object type
    for (int i = 0; i < num_objects; i++) 
    {
        std::string type;
        go_to_next_object(line);
        type = get_object_type(line);

        // If the object is a sphere, read the sphere data and create a sphere object
        if (type == "plane") 
        {
            // Create a sphere object
            plane plane_;
            get_plane_data(line, plane_);

            // Add the sphere to the vector
            planes.push_back(plane_);
        }
    }
    // Reset the file pointer to the beginning of the file
    m_file.clear();

    m_file.seekg(0, std::ios::beg);

    }
 
    void get_pyramid_data(std::string& line, piramide4& pyramid_)
    {
        while (line.find("<\\object>") == std::string::npos) 
        {
            // Read the next line
            
            if (line.find("<texture>") != std::string::npos)  // color is texture
            {
                std::string text = extractText(line);
                auto texture = extractTexture(text);
                pyramid_.set_paint(texture);            
            }
            else if (line.find("<material>") != std::string::npos)
            {
                std::string text = extractText(line);
                auto material = extractMaterial(text);
                pyramid_.set_matter(material);
            }
            else if (line.find("<center") != std::string::npos) 
            {
                // Extract the raw center coordinates
                std::string text = extractNumber(line);

                // Extract the center coordinates
                std::vector<double> components;
                extractComponents(components, text);

                // Set the center coordinates
                point3 center(components[2], components[1], components[0]);
                pyramid_.set_origin(center);
            }
            else if (line.find("<height>") != std::string::npos) 
            {
                std::string number = extractNumber(line);
                pyramid_.set_high(std::stod(number));
            }
            else if (line.find("<side_length>") != std::string::npos) 
            {
                std::string number = extractNumber(line);
                pyramid_.set_size(std::stod(number));
            }
            read_line(line);
        }
       pyramid_.reinit_piramide();

    }

   void get_cube_data(std::string& line, cube& cube_) 
    {
        while (line.find("<\\object>") == std::string::npos) 
        {
            
            if (line.find("<texture>") != std::string::npos)  // color is texture
            {
                std::string text = extractText(line);
                auto texture = extractTexture(text);
                cube_.set_paint(texture);
                
            
            }
            else if (line.find("<material>") != std::string::npos)
            {
                std::string text = extractText(line);
                auto material = extractMaterial(text);
                cube_.set_matter(material);
            }
            else if (line.find("<center>") != std::string::npos) 
            {
                
                // Extract the raw center coordinates
                std::string text = extractNumber(line);

                // Extract the center coordinates
                std::vector<double> components;
                extractComponents(components, text);

                // Set the center coordinates
                point3 center(components[2], components[1], components[0]);
                cube_.set_origin(center);
            }
            else if (line.find("<side_length>") != std::string::npos) 
            {
                std::string number = extractNumber(line);
                double side_length = std::stod(number);
                cube_.set_size(side_length);
            }
            read_line(line);
        }
        cube_.reinit_cube();
    }

    camera_data get_camera_data(std::string& line) 
    {
        camera_data cam_data;
        // Read the camera data
        while (line.find("<\\camara>") == std::string::npos) 
        {
            if (line.find("<look_from>") != std::string::npos) 
            {
                // Extract the raw look_from coordinates
                std::string text = extractNumber(line);
                
                std::vector<double> components;
                extractComponents(components, text);

                // Set the look_from coordinates
                point3 look_from(components[2], components[1], components[0]);

                cam_data.look_from = look_from;                
            }
            else if (line.find("<look_at>") != std::string::npos) 
            {
                // Extract the raw look_from coordinates
                std::string text = extractNumber(line);
                
                std::vector<double> components;
                extractComponents(components, text);

                // Set the look_from coordinates
                point3 look_at(components[2], components[1], components[0]);

                cam_data.look_at = look_at;
            }
            else if (line.find("<vfov>") != std::string::npos) 
            {
                std::string number = extractNumber(line);
                cam_data.fov = std::stod(number);
            }
            else if (line.find("<hole>") != std::string::npos) 
            {
                std::string number = extractNumber(line);
                cam_data.hole = std::stod(number);
            }
            else if (line.find("<focus_dis>") != std::string::npos) 
            {
                std::string number = extractNumber(line);
                cam_data.focus_dist = std::stod(number);
            }
            else if (line.find("<aspect_ratio>") != std::string::npos) 
            {
                std::string number = extractNumber(line);

                // It's in the form a/b, so we need to divide a by b
                double numerator = std::stod(number.substr(0, number.find("/")));
                double denominator = std::stod(number.substr(number.find("/") + 1, number.size()));

                cam_data.aspect_ratio = numerator / denominator;
            }
            else if (line.find("<background>") != std::string::npos)
            {
                std::string text = extractText(line);
                cam_data.background = text;
            }
            else if (line.find("<height>") != std::string::npos)
            {
                std::string number = extractNumber(line);
                cam_data.height = std::stoi(number);
            }

            read_line(line);
        }

        return cam_data;

    };
    
    camera_data get_camara() 
    {
        std::string line;
        read_line(line);
        while (line.find("<camara>") == std::string::npos) 
        {
            read_line(line);
        }
        read_line(line);
        camera_data cam_data = get_camera_data(line);

        return cam_data;        
    }

};