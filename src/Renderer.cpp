#include "Renderer.h"
#include "glad/glad.h"

void Renderer::DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, GLenum type) const {
	shader.Bind();
	va.Bind();
	ib.Bind();

	glDrawElements(type, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawArrays(const VertexArray& va, const Shader& shader, GLenum mode, unsigned int first, unsigned int count) const{
	va.Bind();
	shader.Bind();

	glDrawArrays(mode, first, count);
}


void Renderer::ClearBit(GLbitfield mask) const {
	glClear(mask);
}

void Renderer::ClearColor(float r, float g, float b, float a) const {
	glClearColor(r, g, b, a);
}