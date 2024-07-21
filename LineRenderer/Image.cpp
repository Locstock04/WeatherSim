#include "Image.h"

#include <iostream>

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
