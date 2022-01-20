#include "Fractal.h"
#include <string>

std::string Fractal::GetName() const { 
	return m_name;
}

const char* Fractal::GetVertexPath() const {
	return m_vertexPath;
}

const char* Fractal::GetFragmentPath() const{
	return m_fragmentPath;
}