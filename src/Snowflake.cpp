#define _USE_MATH_DEFINES

#include "Snowflake.h"
#include <cmath>

struct SnowflakeHeading {
	unsigned short heading = 0;

	void increment() {
		if (heading == 5) {
			heading = 0;
			return;
		}
		heading++;
	}

	void decrement() {
		if (heading == 0) {
			heading = 5;
			return;
		}
		heading--;
	}
};

Snowflake::Snowflake(unsigned int generations) : m_generations(generations) {
	m_enumType = GL_LINE_STRIP;
	GenerateLSystem();
	GenerateVertices();
	m_vertexPath = "./res/shaders/snowflake/snowflake.vert";
	m_fragmentPath = "./res/shaders/snowflake/snowflake.frag";
	m_drawType = 2;
}

void Snowflake::GenerateLSystem() {
	std::string prevGenString = "F--F--F";
	std::string currGenString = "";
	std::string replace = "F+F--F+F";
	unsigned int currGen = 0;

	while (currGen < m_generations) {
		for (int i = 0; i < prevGenString.length(); i++) {
			if (prevGenString.at(i) == 'F') {
				currGenString += replace;
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

float* Snowflake:: ConvertVertices() {
	float* arr = new float[m_vertices.size() + 1];

	for (int i = 0; i < m_vertices.size(); i++) {
		arr[i] = m_vertices.at(i);
	}

	return arr;
}

unsigned int* Snowflake::ConvertIndices() {
	GenerateIndices();
	unsigned int* arr = new unsigned int[m_indices.size()];

	for (int i = 0; i < m_indices.size(); i++) {
		arr[i] = m_indices.at(i);
	}

	return arr;
}

unsigned int Snowflake::GetNumIndices() const {
	return (m_vertices.size() / 2);
}

void Snowflake::GenerateVertices() {
	float currX = -0.5f;
	float currY = 0.25f;

	m_vertices.push_back(currX);
	m_vertices.push_back(currY);

	SnowflakeHeading heading;

	for (int i = 0; i < m_lSystem.size(); i++) {
		if (m_lSystem.at(i) == 'F') {
			switch (heading.heading) {
			case 0:
				currX += m_stepSize;
				break;
			case 1:
				currX += m_stepSize * std::cos(M_PI / 3);
				currY += m_stepSize * std::sin(M_PI / 3);
				break;
			case 2:
				currX -= m_stepSize * std::cos(M_PI / 3);
				currY += m_stepSize * std::sin(M_PI / 3);
				break;
			case 3:
				currX -= m_stepSize;
				break;
			case 4:
				currX -= m_stepSize * std::cos(M_PI / 3);
				currY -= m_stepSize * std::sin(M_PI / 3);
				break;
			case 5:
				currX += m_stepSize * std::cos(M_PI / 3);
				currY -= m_stepSize * std::sin(M_PI / 3);
			}
			m_vertices.push_back(currX);
			m_vertices.push_back(currY);
		}

		if (m_lSystem.at(i) == '-') {
			heading.decrement();
		}

		if (m_lSystem.at(i) == '+') {
			heading.increment();
		}
	}
}

void Snowflake::GenerateIndices() {
	for (int i = 0; i < m_vertices.size() / 2; i++) {
		m_indices.push_back(i);
	}
}

