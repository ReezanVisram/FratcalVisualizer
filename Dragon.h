#pragma once
#include "Fractal.h"
#include <string>
#include <vector>

class Dragon : public Fractal {
public:
	Dragon(unsigned int generations);

	float* ConvertVertices() override;
	unsigned int* ConvertIndices() override;
	unsigned int GetNumIndices() const override;

	void SetGenerations(unsigned int generations);

private:
	void GenerateLSystem();
	void GenerateVertices();
	void GenerateIndices();

	std::string SwapCharacters(std::string stringToSwap);

	unsigned int m_generations;
	std::string m_lSystem;
	float m_stepSize = 0.001f;
	
};