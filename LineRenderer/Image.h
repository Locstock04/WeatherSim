#pragma once

#include "stb_image.h"
#include <string>

class Image {
private:

public:

	unsigned char* data = nullptr;

	int width;
	int height;

	std::string path;

	Image(std::string _path, int forceLoadType = STBI_default);

	Image();
};