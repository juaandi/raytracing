IN204 RAYTRACING PROJECT. Juan M. CARVAJAL, Juan D. RAMIREZ

This program parses a text description of a scene and generates a corresponding scene for a raytracing program. The program is compiled using g++ with the following options:

            g++ -fopenmp screen.cpp -o screen -lfltk -lfltk_images

The program also requires ImageMagick and LFTK libraries to be installed.


1. Usage
    To use the program, run the compiled executable file screen. The program will open a window
    corresponding to the GUI where there are fundamentally three steps:

    1.1. Click "Load file" button to load a file with the description of a scene in the
    format proposed in 2. or write it in the space dedied to this. In this step you can
    re-write and edit the text as you wish.
    
    1.2. Click "Confirm file" to submit your file to be renderized, in this moment you can
    still re-write and edit your text but you must click "Confirm file" whenever you want
    to confirm the changes.
    
    1.3. Select a engine of renderization, there are three engines named by the graphs
    that our inspire. "Super Mario 32 bits" being clearly the engine with the lowest amount
    of computational budget avalaible and "Elden Ring" being the engine with all the
    parameters sets to maximum.

    Once you have made these three steps you are ready to click the "Play" button and wait
    to see the results.

    This project supports the download of the images in .png and .jpeg format, so you have just
    to click these buttons to download the result.

2. Input Format
    The scene description should be submitted in an XML-like format, each object should be on a separate line.
    The first word on the line should be a tag, following the line of the name should be a list of properties where the order is not important.
    The properties will vary depending on the object. See the objects supported using the following tags:

    2.1. <scene> <\scene>: The root tag, encapsulating all other tags.

    2.2. <camera> <\camera>:  A tag indicating the camera parameters
            <look_from> <\look_from>: A tag indicating the position of the camera. It should contain three floating-point values separated by commas, enclosed in parentheses. After enough
            tries, we suggest values close to (-5,1,0)

            <look_at> <\look_at>: A tag indicating the point that the camera is looking at. It should contain three floating-point values separated by commas, enclosed in parentheses. We suggest values close to (0,0,0)

            <focus_dis> <\focus_dis>: A tag indicating the focus distance of the camera. It should contain a single floating-point value. We suggest values between 4-6 

            <hole> <\hole>: A tag indicating the aperture of the camera. It should contain a single floating-point value. If you want a image completely nitide let hole in 0, this value
            penalize the bad focus.
            
            <vfov> <\vfov>: A tag indicating the vertical field of view of the camera, in degrees. It should contain a single floating-point value. We suggest values between 0-90
        
            <aspect_ratio> <\aspect_ratio>: A tag indicating the aspect ratio of the camera. It should contain two integer values separated by a slash.

            <height> <\height>: A tag indicating the height of the output image in pixels. It should contain a single integer value.

            <background> <\background>: A tag indicating the background of the scene. It should contain a string value representing the file name of the image to be used as the background. The glossary of backgrounds is presented in the section 3.
    
    2.3. <object type = object> <\object>: A tag indicating an object in the scene. type="sphere": An attribute indicating that the object is a sphere. Other possible values are "cube", "plane", and "pyramid".

        2.3.1. <object type = "sphere"> <\object>
                2.3.1.1. <material> <\material>: A tag indicating the material of the object. It should contain a string value representing the name of the material. See the glossary of background in the section 3.

		        2.3.1.2  <radius> <\radius>: A tag indicating the radius of the sphere. It should contain a single floating-point value.
		        
                2.3.1.3  <center> <\center>: A tag indicating the position of the center of the sphere. It should contain three floating-point values separated by commas, enclosed in parentheses.
		        
                2.3.1.4. <texture> <\texture>: A tag indicating the texture of the object. It should contain a string value representing the name of the texture. See (encore fois) the glossary to see all the textures supported. 

        2.3.2. <object type = "cube"> <\object>:
                2.3.1.1. <material> <\material>: A tag indicating the material of the object. It should contain a string value representing the name of the material. See the glossary of background in the section 3.

		        2.3.1.2  <side_length> <\side_length>: A tag indicating the side length of the cube. It should contain a single floating-point value.
		        
                2.3.1.3  <center> <\center>: A tag indicating the position of the center of the cube. It should contain three floating-point values separated by commas, enclosed in parentheses.
		        
                2.3.1.4. <texture> <\texture>: A tag indicating the texture of the object. It should contain a string value representing the name of the texture. See (encore fois) the glossary to see all the textures supported. 
        
        2.3.3. <object type = "plane"> <\object>:
                2.3.1.1. <material> <\material>: A tag indicating the material of the object. It should contain a string value representing the name of the material. See the glossary of background in the section 3.

		        2.3.1.2  <bottom_left_corner> <\bottom_left_corner>, <bottom_right_corner> <\<bottom_right_corner>, <upper_left_corner> <\<upper_left_corner>, <upper_right_corner> <\upper_right_corner> : Tags indicating the corners of the plane. They should each contain three floating-point values separated by commas, enclosed in parentheses.
		        
                2.3.1.4. <texture> <\texture>: A tag indicating the texture of the object. It should contain a string value representing the name of the texture. See (encore fois) the glossary to see all the textures supported. 
        
        2.3.4. <object type = "pyramid"> <\object>:   
                2.3.1.1. <material> <\material>: A tag indicating the material of the object. It should contain a string value representing the name of the material. See the glossary of background in the section 3.

		        2.3.1.2  <side_length> <\side_length>: A tag indicating the side length of the pyramid. It should contain a single floating-point value.
		        
                2.3.1.3  <center> <\center>: A tag indicating the position of the center of the pyramid. It should contain three floating-point values separated by commas, enclosed in parentheses.
		        
                2.3.1.4. <texture> <\texture>: A tag indicating the texture of the object. It should contain a string value representing the name of the texture. See (encore fois) the glossary to see all the textures supported. 

                2.3.1.5 <height> <\height>: : A tag indicating the height of the pyramid. It should contain a single floating-point value.
    
    2.4. Example of a functional scene description

3. Glossary
    This project supports a rich quantity of materials, textures and backgrounds.
    We are refering the background as the color of the sky.

    3.1. Materials
		    
        * blurry (doesn't reflect the rays)
        * mercury
        * zinc
        * aluminum
        * nickel
        * copper
        * brass
        * bronze
        * silver
        * gold
        * platinum
        * chromium

    3.2. Textures
		    
        * white
        * black
        * red
        * green
        * blue
        * yellow
        * magenta
        * cyan
        * gray
        * dark_gray
        * light_gray
        * pink
        * orange
        * purple
        * lime
        * turquoise
        * navy
        * olive
        * bright_white
        * bright_red
        * bright_green
        * bright_blue
        * bright_yellow
        * bright_purple
        * bright_cyan
        * bright_magenta
        * bright_orange
        * bright_pink
        * bright 
        * superbright 

        *chess

        * cucuball
        * juandiball
        * pipeball
        * viniball
        * juandiball

        * noise

        (these 3 below are not actually textures but sources of light)
        * candle (low intensity)
        * flashlight (medium intensity)
        * lighthouse (high intensity)

    3.3. Backgrounds
		    
        * galactic
        * cityscape
        * tropical
        * underwater
        * castle
        * rainforest
        * desert
        * futuristic
        * winter
        * gothic
        * magical
        * haunted
        * sunrise
        * enchanted
        * jungle

4. Engines
    There are 3 engines which define the bounce depth of the rays and the samples per pixel
    
    4.1 Super Mario: defines the values in 5 and 25
    
    4.2. Mortal Kombat: defines the values in 15 and 200

    4.3. Elden Ring: defines them in 25 and 1000

    We have choose this approach because its a lot easier to change the quality of the image and
    the speed of render from this two variables than find a way to "activate/deactivate" physics of our core model.

5. Dependencies
    This program requires the following libraries to be installed:
        * FLTK
        * FLTK Images
        * ImageMagick

    On Ubuntu or Debian, you can install these libraries using the following command:

    sudo apt-get install libfltk1.3-dev libfltk-image1.3-dev libmagick++-dev

    In windows we have installed both of them with vcpkg
