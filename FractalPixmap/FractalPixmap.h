// FractalPixmap.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>

// TODO: Reference additional headers your program requires here.

// zoom out/in factor
constexpr float SCALE_FACTOR = 1.0;

/*
	Power function for complex numbers
	v: complex number as the base
	n: the exponent
*/
glm::vec2 cpow(glm::vec2 v, float n)
{
	float angle = glm::atan(v.y, v.x);

	return powf(glm::length(v), n) * glm::vec2(cos(angle * n), sin(angle * n));
}

/*
	Computes the color of a point in the complex plane.
	If the point belongs to the Mandelbrot set, the color is black,
	otherwise white.
	start: point in the complex plane
	exponent: exponent of the sequence z_n+1 = z_n^exponent + c
*/
glm::vec3 iterateFractal(glm::vec2 start, float exponent)
{
	const int threshold = 60;
	const int maxCount = 100;

	glm::vec2 z(0.0, 0.0);
	for (int i = 0; i < maxCount; ++i)
	{
		z = cpow(z, exponent) + start;
		if (glm::length(z) > threshold)
		{
			return glm::vec3(1.0);
		}
	}
	return glm::vec3(0.0);
}

/*
	Center the coordinate axes and normalize in vertical direction
	v: coordinates of the point to center/scale
	res: resolution in x and y direction
*/
glm::vec2 centerAndScale(const glm::vec2& v, const glm::vec2& res)
{
	// normalize in vertical direction and center coordinate axes
	float scale = res.x / res.y; //1.333f;
	glm::vec2 tmp = SCALE_FACTOR * (2.0f * glm::vec2(v.x, v.y) - res) / res.y;

	return tmp;
}

glm::vec3 getColor(glm::vec2 p, const glm::vec2 res, float exponent)
{
	glm::vec2 tmp = centerAndScale(p, res);
	return iterateFractal(tmp, exponent);
}

void draw(std::vector<glm::vec3>& colors, const glm::vec2& res, float exponent)
{
	for (unsigned int i = 0; i < res.y; ++i)
	{
		for (unsigned int j = 0; j < res.x; ++j)
		{
			colors[i * (unsigned int)(res.y) + j] = getColor(glm::vec2(j, i), res, exponent);
		}
	}
}

void write_file(const std::string& file,
	std::vector<glm::vec3>& color, unsigned int width, unsigned int height)
{
	std::ofstream ofs;

	/***************************************/
	// WRITING TO IMAGE FILE
	/***************************************/
	ofs.open(file, std::ios::binary);

	if (ofs.fail())
	{
		char err_str[75] = { '\0' };
		std::cout << "Error: Image could not be saved to \"" << file << "\"."
			<< std::endl;
		// print related error message
		//GET_STRERR(errno, err_str, 75);
		std::cout << err_str;
		exit(1);
	}

	//LOG(INFO) << "Writing image to \"" << file << "\"";

	// don't use \n as ending white space, because of Windows
	ofs << "P6 " << width << " " << height << " 255 ";

	// write to image file
	for (size_t i = 0; i < color.size(); ++i)
	{
		// gamma correction and mapping to [0;255]
		color[i] = glm::pow(glm::min(glm::vec3(1), color[i]),
			glm::vec3(1 / 2.2f)) * 255.f;

		// prevent sign extension by casting to unsigned int
		unsigned char r = (unsigned int)round(color[i].x);
		unsigned char g = (unsigned int)round(color[i].y);
		unsigned char b = (unsigned int)round(color[i].z);

		ofs << r << g << b;
	}

	ofs.close();

	//LOG(INFO) << "Writing image to \"" << file << "\" finished.";
}
