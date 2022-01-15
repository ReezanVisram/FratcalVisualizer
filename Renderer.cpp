#include "Renderer.h"
#include "glad/glad.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, GLenum type) const {
	shader.Bind();
	va.Bind();
	ib.Bind();

	glDrawElements(type, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::ClearBit(GLbitfield mask) const {
	glClear(mask);
}

void Renderer::ClearColor(float r, float g, float b, float a) const {
	glClearColor(r, g, b, a);
}