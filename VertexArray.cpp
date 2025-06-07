#include "VertexArray.h"
#include  "Renderer.h"
void VertexArray::Bind()
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::UnBind()
{
    glBindVertexArray(0);
}
VertexArray::VertexArray()
{
    glGenVertexArrays(1,&m_RendererID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1,&m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned  int offset = 0;
    for(unsigned int i = 0 ; i<elements.size();i++)
    {
        const auto& element  = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i,element.count,element.type,element.normalized,layout.GetStride()*element.count,(const void*)offset);
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
    
    
}


