#include "Image.h"

#include <iostream>

// TODO: Will this cause an issue if happens in multiple spots, could potentially cause some issues

#include "stb_image.h"


Image::Image(std::string _path, int forceLoadType) :
	path(_path)
{
	data = stbi_load(path.c_str(), &width, &height, nullptr, forceLoadType);
	if (!data) {
		std::cout << "Texture failed to load, path: " << path << "\n";
		return;
	}
}

Image::Image() :
	width(0),
	height(0),
	path("")
{
}
