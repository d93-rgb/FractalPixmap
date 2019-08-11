// FractalPixmap.cpp : Defines the entry point for the application.
//
#include "FractalPixmap.h"
using namespace std;

constexpr auto WIDTH = 640;
constexpr auto HEIGHT = 640;

int main()
{
	glm::vec2 res(WIDTH, HEIGHT);
	std::vector<glm::vec3> colors(WIDTH * HEIGHT);
	float exponent = 2;

	const std::string& file = "fractal_out.ppm";
	
	draw(colors, res, exponent);

	write_file(file, colors, WIDTH, HEIGHT);

	cout << "Done." << endl;
	return 0;
}
