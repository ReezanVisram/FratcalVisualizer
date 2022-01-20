#pragma once
#include "Fractal.h"
#include <string>
#include <vector>

class Dragon : public Fractal {
public:
	Dragon(unsigned int generations, unsigned short initialHeading);

	float* ConvertVertices() override;
	unsigned int* ConvertIndices() override;
	unsigned int GetNumIndices() const override;

	void SetGenerations(unsigned int generations, unsigned short heading);

private:
	void GenerateLSystem();
	void GenerateVertices();
	void GenerateIndices();

	std::string SwapCharacters(std::string stringToSwap);

	unsigned int m_generations;
	std::string m_lSystem;
	float m_stepSize = 0.0005f;
	unsigned short m_heading;
	
};