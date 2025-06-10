#include "Texture.h"
#include "stb_image.h"
#include <GL/glew.h>


Texture::Texture(const std::string &path)
    :m_FilePath(path),m_LocalBUffer(nullptr),
        m_Width(0),m_Height(0),m_BPP(0);
{
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(),&m_Width,&m_Height,&m_BPP,4);
    
    glGenTextures(1,&m_RendererID);
    glBindTexture(GL_TEXTURE_2D,m_RendererID);
}

Texture::~Texture()
{

}

void Texture::Bind(unsigned int slot) const
{

}

void Texture::Unbind()
{

}
