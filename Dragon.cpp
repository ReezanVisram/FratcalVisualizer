#include "Dragon.h"
#include <string>
#include <vector>

struct Heading {
	unsigned short heading = 0;

	void increment() {
		if (heading == 3) {
			heading = 0;
			return;
		}
		heading++;
	}

	void decrement() {
		if (heading == 0) {
			heading = 3;
			return;
		}
		heading--;
	}
};

Dragon::Dragon(unsigned int generations) : m_generations(generations) {
	enumType = GL_LINE;
	GenerateVertices();
}

void Dragon::GenerateLSystem() {
	std::string prevGenString = "r";
	std::string currGenString = "";
	unsigned int currGen = 1;

	while (currGen <= m_generations) {
		std::string reversed(prevGenString.rbegin(), prevGenString.rend());
		std::string swapped = SwapCharacters(reversed);
		currGenString = prevGenString + "r" + swapped;
		currGen++;
		prevGenString = currGenString;
	}

	m_lSystem = currGenString;
}

std::string Dragon::SwapCharacters(std::string stringToSwap) {
	std::string newString = "";
	for (int i = 0; i < stringToSwap.size(); i++) {
		if (stringToSwap.at(i) == 'r') {
			newString += 'l';
		}
		else {
			newString += 'r';
		}
	}

	return newString;
}

void Dragon::GenerateVertices() {
	m_vertices = {};
	GenerateLSystem();

	float currX = 0.0f;
	float currY = 0.0f;

	Heading heading;

	for (int i = 0; i < m_lSystem.size(); i++) {
		m_vertices.push_back(currX);
		m_vertices.push_back(currY);

		switch (heading.heading) {
		case 0:
			currY += m_stepSize;
			break;
		case 1:
			currX += m_stepSize;
			break;
		case 2:
			currY -= m_stepSize;
			break;
		case 3:
			currX -= m_stepSize;
			break;
		}

		if (m_lSystem.at(i) == 'r') {
			heading.increment();
		}
		else {
			heading.decrement();
		}

		//std::cout << "(" << currX << ", " << currY << ")" << std::endl;
	}

}

float* Dragon::ConvertVertices() {
	float* arr = new float[m_vertices.size() + 1];

	for (int i = 0; i < m_vertices.size(); i++) {
		arr[i] = m_vertices.at(i);
	}

	return arr;
}

unsigned int Dragon::GetNumIndices() {
	return (m_vertices.size() / 2);
}

void Dragon::SetGenerations(unsigned int generations) {
	m_generations = generations;
	GenerateVertices();
}