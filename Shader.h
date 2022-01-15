#pragma once

#include <string>
#include <vector>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);

	void SetFloatUniform(const std::string& name, std::vector<float> values) const;
	void SetDoubleUniform(const std::string& name, std::vector<double> values) const;
	void Bind() const;


private:
	void GetSourceCode(const char* vertexPath, const char* fragmentPath);
	unsigned int CompileShader(const char* shaderCode, bool isVertex);
	void CreateShaderProgram(unsigned int vertexObject, unsigned int fragmentObject);


	unsigned int m_rendererId;
	unsigned int m_vertexId = INT_MAX;
	unsigned int m_fragmentId = INT_MAX;
};