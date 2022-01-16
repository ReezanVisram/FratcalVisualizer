#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "glad/glad.h"

class Renderer {
public:
	void DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, GLenum type) const;
	void DrawArrays(const VertexArray& va, const Shader& shader, GLenum mode, unsigned int first, unsigned int count) const;
	void ClearBit(GLbitfield mask) const;
	void ClearColor(float r, float g, float b, float a) const;
};