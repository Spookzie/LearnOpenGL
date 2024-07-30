#pragma once


class IndexBuffer
{
private:
	unsigned int rendererID;
	unsigned int count;

public:
	//Constructor & Destructor
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	//Getter
	inline unsigned int GetCount() const { return count; }
};