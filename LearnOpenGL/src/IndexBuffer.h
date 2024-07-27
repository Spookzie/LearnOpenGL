#pragma once


class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;

public:
	//Constructor & Destructor
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	//Getter
	inline unsigned int GetCount() const { return m_Count; }
};