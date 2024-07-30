#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


class VertexArray
{
private:
	unsigned int rendererID;

public:
	//Constructor & Destructor
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};