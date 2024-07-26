#pragma once

#include <string>

class Image {
private:

public:

	unsigned char* data = nullptr;

	int width;
	int height;

	std::string path;

	Image(std::string _path, int forceLoadType = 0/*STBI_default*/);

	Image();
};