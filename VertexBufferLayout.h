#pragma once
#include <vector>
#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type) {
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}

		assert(false);
		return 0;
	}
};

class VertexBufferLayout {
public:
	VertexBufferLayout() : m_stride(0) {}

	template<typename T>
	void Push(unsigned int count) {
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count) {
		VertexBufferElement el({ GL_FLOAT, count, GL_FALSE });
		m_elements.push_back(el);
		m_stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		VertexBufferElement el({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_elements.push_back(el);
		m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<char>(unsigned int count) {
		VertexBufferElement el({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_elements.push_back(el);
		m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	const std::vector<VertexBufferElement> GetElements() const { return m_elements; };
	unsigned int GetStride() const { return m_stride; };

private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
};