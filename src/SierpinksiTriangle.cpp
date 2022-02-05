#define _USE_MATH_DEFINES

#include "SierpinskiTriangle.h"
#include <cmath>

struct TriangleHeading {
	unsigned short heading = 0;

	void increment() {
		if (heading == 2) {
			heading = 0;
			return;
		}
		heading++;
	}

	void decrement() {
		if (heading == 0) {
			heading = 2;
			return;
		}
		heading--;
	}
};

SierpinskiTriangle::SierpinskiTriangle(unsigned int generations) : m_generations(generations) {
	m_enumType = GL_LINE_STRIP;
	GenerateLSystem();
	GenerateVertices();
	m_vertexPath = "./res/shaders/triangle/triangle.vert";
	m_fragmentPath = "./res/shaders/triangle/triangle.frag";
	m_drawType = 3;
}

void SierpinskiTriangle::GenerateLSystem() {
	std::string prevGenString = "F-G-G";
	std::string currGenString = "";
	std::string fReplace = "F-G+F+G-F";
	std::string gReplace = "GG";
	unsigned int currGen = 0;

	while (currGen < m_generations) {
		for (int i = 0; i < prevGenString.length(); i++) {
			if (prevGenString.at(i) == 'F') {
				currGenString += fReplace;
			}
			else if (prevGenString.at(i) == 'G') {
				currGenString += gReplace;
			}
			else {
				currGenString += prevGenString.at(i);
			}
		}
		prevGenString = currGenString;
		currGenString = "";
		currGen++;
	}
	m_lSystem = prevGenString;
}

void SierpinskiTriangle::GenerateVertices() {
	float currX = -0.5f;
	float currY = 0.5f;

	m_vertices.push_back(currX);
	m_vertices.push_back(currY);

	TriangleHeading heading;

	for (int i = 0; i < m_lSystem.size(); i++) {
		if (m_lSystem.at(i) == 'F' || m_lSystem.at(i) == 'G') {
			switch (heading.heading) {
			case 0:
				currX += m_stepSize;
				break;
			case 1:
				currX -= m_stepSize * std::cos(M_PI / 3);
				currY += m_stepSize * std::sin(M_PI / 3);
				break;
			case 2:
				currX -= m_stepSize * std::cos(M_PI / 3);
				currY -= m_stepSize * std::sin(M_PI / 3);
				break;
			}
			
			m_vertices.push_back(currX);
			m_vertices.push_back(currY);
		}
		if (m_lSystem.at(i) == '+') {
			heading.increment();
		}
		if (m_lSystem.at(i) == '-') {
			heading.decrement();
		}
	}
}

void SierpinskiTriangle::GenerateIndices() {
	for (int i = 0; i < m_vertices.size() / 2; i++) {
		m_indices.push_back(i);
	}
}

float* SierpinskiTriangle::ConvertVertices() {
	float* arr = new float[m_vertices.size()];

	for (int i = 0; i < m_vertices.size(); i++) {
		arr[i] = m_vertices.at(i);
	}

	return arr;
}

unsigned int* SierpinskiTriangle::ConvertIndices() {
	GenerateIndices();
	unsigned int* arr = new unsigned int[m_indices.size()];

	for (int i = 0; i < m_indices.size(); i++) {
		arr[i] = m_indices.at(i);
	}

	return arr;
}

unsigned int SierpinskiTriangle::GetNumIndices() const {
	return (m_vertices.size() / 2);
}