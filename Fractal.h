#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "glad/glad.h"

class Fractal {
public:
	std::string GetName() const;
	GLenum GetEnumType() const {
		return m_enumType;
	}

	unsigned short GetDrawType() const {
		return m_drawType;
	}

	virtual unsigned int GetNumIndices() const {
		return m_indices.size();
	}

	unsigned int GetNumVertices() const {
		return m_vertices.size();
	}

	virtual float* ConvertVertices() = 0;
	virtual unsigned int* ConvertIndices() = 0;
	


	const char* GetVertexPath() const;
	const char* GetFragmentPath() const;

protected:
	std::string m_name;

	std::vector<float> m_vertices;
	std::vector<unsigned int> m_indices;

	const char* m_vertexPath;
	const char* m_fragmentPath;

	GLenum m_enumType;
	unsigned short m_drawType;
};

