#pragma once
#include "Fractal.h"
#include <string>
#include <vector>

class Dragon : public Fractal {
public:
	Dragon(unsigned int generations);

	float* ConvertVertices() override;
	unsigned int GetNumIndices();

	void SetGenerations(unsigned int generations);

private:
	void GenerateLSystem();
	void GenerateVertices();

	std::string SwapCharacters(std::string stringToSwap);

	unsigned int m_generations;
	std::string m_lSystem;
	float m_stepSize = 0.001f;
	
};