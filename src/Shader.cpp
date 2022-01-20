#include "Shader.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <glad/glad.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	GetSourceCode(vertexPath, fragmentPath);

	assert(m_vertexId != INT_MAX && m_fragmentId != INT_MAX);

	CreateShaderProgram(m_vertexId, m_fragmentId);
}

void Shader::Bind() const {
	glUseProgram(m_rendererId);
}

void Shader::GetSourceCode(const char* vertexPath, const char* fragmentPath) {
	std::ifstream vertexStream(vertexPath);

	std::string vertexLine;

	std::stringstream vertexSs;
	while (getline(vertexStream, vertexLine)) {
		vertexSs << vertexLine << '\n';
	}

	std::string vertexCode = vertexSs.str();
	const char* vertexCodeC = vertexCode.c_str();

	m_vertexId = CompileShader(vertexCodeC, true);

	vertexStream.close();

	std::ifstream fragmentStream(fragmentPath);

	std::string fragmentLine;
	std::stringstream fragmentSs;
	while (getline(fragmentStream, fragmentLine)) {
		fragmentSs << fragmentLine << '\n';
	}

	std::string fragmentCode = fragmentSs.str();
	const char* fragmentCodeC = fragmentCode.c_str();

	m_fragmentId = CompileShader(fragmentCodeC, false);

}

unsigned int Shader::CompileShader(const char* shaderCode, bool isVertex) {
	unsigned int shaderObject;
	int success;
	char infoLog[512];

	if (isVertex) {
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	}
	else {
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	}

	glShaderSource(shaderObject, 1, &shaderCode, NULL);
	glCompileShader(shaderObject);

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);
		if (isVertex) {
			std::cout << "Vertex Shader Compilation Failed:\n";
		}
		else {
			std::cout << "Fragment Shader Compilation Failed:\n";
		}
		std::cout << infoLog << std::endl;
	}

	return shaderObject;
}

void Shader::CreateShaderProgram(unsigned int vertexObject, unsigned int fragmentObject) {
	m_rendererId = glCreateProgram();
	glAttachShader(m_rendererId, vertexObject);
	glAttachShader(m_rendererId, fragmentObject);
	glLinkProgram(m_rendererId);

	glDeleteShader(vertexObject);
	glDeleteShader(fragmentObject);
}

void Shader::SetFloatUniform(const std::string& name, std::vector<float> values) const {
	std::size_t numValues = values.size();

	assert(numValues > 0 && numValues < 5);

	switch (numValues) {
	case 1:
		glUniform1f(glGetUniformLocation(m_rendererId, name.c_str()), values.at(0));
		return;
	case 2:
		glUniform2f(glGetUniformLocation(m_rendererId, name.c_str()), values.at(0), values.at(1));
		return;
	case 3:
		glUniform3f(glGetUniformLocation(m_rendererId, name.c_str()), values.at(0), values.at(1), values.at(2));
		return;
	case 4:
		glUniform4f(glGetUniformLocation(m_rendererId, name.c_str()), values.at(0), values.at(1), values.at(2), values.at(3));
		return;
	}
}
