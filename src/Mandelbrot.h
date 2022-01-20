#pragma once
#include "Fractal.h"

class Mandelbrot : public Fractal {
public:
	Mandelbrot();
	float* ConvertVertices() override;
	unsigned int* ConvertIndices() override;
};