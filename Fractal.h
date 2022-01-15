#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "glad/glad.h"

class Fractal {
public:
	Fractal() {
		std::cout << "?" << std::endl;
	}
	
	std::string GetName() const;
	GLenum GetEnumType() const;

	virtual float* ConvertVertices() {
		static float what = 0.0f;
		return &what;
	}

	virtual unsigned int* ConvertIndices() {
		static unsigned int what = 0;
		return &what;
	}

protected:
	std::string m_name;

	std::vector<float> m_vertices;
	std::vector<unsigned int> m_indices;

	GLenum enumType;
};

