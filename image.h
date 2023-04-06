#ifndef IMAGE_H
#define IMAGE_H
//Juan Manuel Carvajal and Juan Diego Ramirez image's header file
//IN204 Ray-tracing program 2022/2023


#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>



template<typename T>
class PixelColor
	//Represents a RGB (red, green, blue) color for a pixel in an image
	//All the colors are clamped to the range 0-255.
{
	//Only integer types are allowed for the color components
	static_assert(std::is_integral<T>::value, "Error: Coordinate type must be an integer type.");
private:
	T r_, g_, b_;
	const int min_color_value{ 0 }, max_color_value{ 255 };

public:
	PixelColor() : r_{ 0 }, g_{ 0 }, b_{ 0 } {}

	PixelColor(T r, T g, T b) : r_{ r }, g_{ g }, b_{ b } 
	{
		r_ = std::clamp(r_, min_color_value, max_color_value);
		g_ = std::clamp(g_, min_color_value, max_color_value);
		b_ = std::clamp(b_, min_color_value, max_color_value);
		
		warning_if_invalid_color({ r, g, b });
	}
	
	//Getters 
	T get_r() const { return r_; }
	T get_g() const { return g_; }
	T get_b() const { return b_; }

	//Setters
	void set_r(T red)
	{
		warning_if_invalid_color({ red });
		r_ =  std::clamp(red, min_color_value, max_color_value) ;
	}
	
	void set_g(T green)
	{
		warning_if_invalid_color({ green });
		g_ = std::clamp(green, min_color_value, max_color_value) ;
	}
	
	void set_b(T blue)
	{
		warning_if_invalid_color({ blue });
		b_ = std::clamp(blue, min_color_value, max_color_value) ;
	}
	
	
	void adjust_brightness(T amount)
	// Adjust the color's brightness by the given amount.
	// If the amount is positive, the color is brightened.
	// If the amount is negative, the color is darkened.
	{
		warning_if_invalid_color({ r_ + amount, g_ + amount, b_ + amount });
		r_ = std::clamp( r_ + amount, min_color_value, max_color_value ) ;
		g_ = std::clamp( g_ + amount, min_color_value, max_color_value ) ;
		b_ = std::clamp( b_ + amount, min_color_value, max_color_value ) ;	
	}

	friend std::ostream& operator<<(std::ostream& os, const PixelColor& color) 
	//Overload the << operator to print the color in format "Color(r, g, b)"
	{
		os << "Color(" << color.r_ << ", " << color.g_ << ", " << color.b_ << ")";
		return os;
	}
		
	void warning_if_invalid_color(const std::initializer_list<T> color_values)
	//Prints a warning message to the console if in a list of colors there is an invalid value.
	{

		// Iterate over the list of color values
		for (T color_value : color_values)
		{
			// Check if the color value is outside the valid range
			if (color_value < min_color_value || color_value > max_color_value)
			{
				// Print the warning message
				std::cerr << "Warning: Invalid color value:( " << color_value << " ) trying to be used, clamping to valid range" << std::endl;
			}
		}
	}
};


template<typename T>
class Image
	//Represents an image with a width, height, and a vector of pixels.
	//The origin of coordinates is the top-left corner of the image.
{
	//Only integer types are allowed for size and color components.
	static_assert(std::is_integral<T>::value, "Error: Coordinate type must be an integer type.");
private:
	T width_, height_;
	std::vector<PixelColor<T>> pixels_;

public:
	//Creates by default an image with width 100 and height 100 with black background.
	Image() : width_{ 100 }, height_{ 100 }, pixels_(width_ * height_) {}

	//Creates an image with the given width and height and black background.
	Image(T width, T height) : width_{ width }, height_{ height }, pixels_(width_* height_) {}
	
	//Creates an image with the given width and height and a custom plain background color.
	Image(T width, T height, const PixelColor<T>& color) :
		width_{ width },
		height_{ height },
		pixels_(width_ * height_, color)
	{}

	//Getters
	const T get_width()  const { return width_; }
	const T get_height() const { return height_; }

	
	const PixelColor<T>& get_pixel(T x, T y) const
	//Get the color of the pixel at the given coordinates.
	{
		warning_if_invalid_coordinates(x, y);
		return pixels_[y * width_ + x];
	}
	
	//Doesn't allow the user to modify the size of the image after it's created.

	void set_pixel(T x, T y, const PixelColor<T>& color)
	//Set the color of the pixel at the given coordinates.
	{	
		warning_if_invalid_coordinates(x, y);
		pixels_[y * width_ + x].set_b(color.get_b());
		pixels_[y * width_ + x].set_g(color.get_g());
		pixels_[y * width_ + x].set_r(color.get_r());
	}
	
	
	friend std::ostream& operator<<(std::ostream & os, const Image<T> & image)
	//Overload the << operator to print the image in format "Image(width, height)"
	{
		os << "Image(" << image.width_ << ", " << image.height_ << ")";
		return os;
	}

	
	void save(const std::string & filename) const
	//Save the image to a file in the PPM format.
	{

		//Try to open the file for writing
		std::ofstream file(filename);

		//Throw an ofstream::failure exception if the file could not be opened
		if (!file.is_open())
		{
			throw std::ofstream::failure("Error: Could not open file for writing");
		}

		//Write the PPM header
		file << "P3" << std::endl;
		file << width_ << " " << height_ << std::endl;
		file << 255 << std::endl;

		//Write the pixels in the PPM format
		for (const auto& pixel : pixels_)
		{
			file << pixel.get_r() << " " << pixel.get_g() << " " << pixel.get_b() << " ";
		}

		//Close the file
		file.close();

	}

	void warning_if_invalid_coordinates(T x, T y) const
	{
		//Check if the given (x,y) coordinates are inside the image and throw an exception if they are not.
		if (x < 0 || x >= width_ || y < 0 || y >= height_)
		{
			throw std::out_of_range("Error: Pixel coordinates are out of bounds.");
		}
		
		
		//Check if the given (x,y) coordinates are positive and throw an exception if they are not.
		if (x < 0 || y < 0)
		{
			throw std::out_of_range("Error: Pixel coordinates are negative.");
		}

	}

};

//Juan Manuel Carvajal and Juan Diego Ramirez image's header file
//IN204 Ray-tracing program 2022/2023


#endif // !IMAGE_H
