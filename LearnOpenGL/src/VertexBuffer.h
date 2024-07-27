#pragma once


class VertexBuffer
{
private:
	unsigned int m_RendererID;

public:
	//Constructor & Destructor
	VertexBuffer(const void* data, unsigned int sie);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};