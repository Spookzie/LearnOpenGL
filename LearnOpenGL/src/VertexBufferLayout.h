#pragma once

#include <GL/glew.h>

#include <vector>
#include <stdexcept>

#include "Renderer.h"


struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 4;
		}

		ASSERT(false);
		return 0;
	}

	VertexBufferElement(unsigned int t, unsigned int c, bool n)
		: count(c), type(t), normalized(n)
	{

	}
};


class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;

public:
	//Constructor
	VertexBufferLayout()
		: stride(0)
	{

	}

	//Templates
	template<typename T>
	void Push(unsigned int count)
	{
		std::runtime_error(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		elements.push_back(VertexBufferElement({ GL_FLOAT, count, GL_FALSE }));
		stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		elements.push_back(VertexBufferElement({ GL_UNSIGNED_INT, count, GL_FALSE }));
		stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		elements.push_back(VertexBufferElement({ GL_UNSIGNED_BYTE, count, GL_TRUE }));
		stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return elements; };
	inline unsigned int GetStride() const { return stride; };
};