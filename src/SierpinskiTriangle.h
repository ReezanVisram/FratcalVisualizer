#pragma once
#include "Fractal.h"
#include <string>

class SierpinskiTriangle : public Fractal {
public:
	SierpinskiTriangle(unsigned int generations);

	float* ConvertVertices() override;
	unsigned int* ConvertIndices() override;
	unsigned int GetNumIndices() const override;

private:
	void GenerateLSystem();
	void GenerateVertices();
	void GenerateIndices();

	unsigned int m_generations;
	std::string m_lSystem;
	float m_stepSize = 0.001f;
};