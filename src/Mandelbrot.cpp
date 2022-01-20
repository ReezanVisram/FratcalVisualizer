#include "Mandelbrot.h"

Mandelbrot::Mandelbrot() {
	m_name = "mandelbrot";
	m_enumType = GL_TRIANGLES;
	m_drawType = 0;

	m_vertices = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f };
	m_indices = { 0, 1, 2, 2, 3, 0 };

	m_vertexPath = "./res/shaders/mandelbrot/mandelbrot.vert";
	m_fragmentPath = "./res/shaders/mandelbrot/mandelbrot.frag";
}

float* Mandelbrot::ConvertVertices() {
	static float vertices[8];

	for (int i = 0; i < 8; i++) {
		vertices[i] = m_vertices[i];
	}

	return vertices;

}

unsigned int* Mandelbrot::ConvertIndices() {
	static unsigned int indices[6];

	for (int i = 0; i < 6; i++) {
		indices[i] = m_indices[i];
	}

	return indices;
}