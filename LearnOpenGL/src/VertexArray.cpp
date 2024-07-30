#include "VertexArray.h"
#include "Renderer.h"

//Constructor
VertexArray::VertexArray()
{
	glErrorCall( glGenVertexArrays(1, &rendererID) );
}

//Destructor
VertexArray::~VertexArray()
{
	glErrorCall( glDeleteVertexArrays(1, &rendererID) );
}


void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];

		//Enabling & specifying vertex attributes
		glErrorCall( glEnableVertexAttribArray(i) );
		glErrorCall( glVertexAttribPointer(i, element.count, element.type,
			element.normalized, layout.GetStride(), (const void*) offset) );

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}

	
}

void VertexArray::Bind() const
{
	glErrorCall( glBindVertexArray(rendererID) );
}

void VertexArray::Unbind() const
{
	glErrorCall( glBindVertexArray(0) );
}
