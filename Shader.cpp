#include "Shader.h"
#include <GL/glew.h>
#include <iostream>
#include <sstream>
#include <fstream>



int Shader::Createshader(const std::string&vertexShader, const std::string& fragmentShader)
{
    unsigned int program =glCreateProgram();
    unsigned int vs =CompileShader(GL_VERTEX_SHADER,vertexShader);
    unsigned int fs =CompileShader(GL_FRAGMENT_SHADER,fragmentShader);

    glAttachShader(program,vs);
    glAttachShader(program,fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

ShaderKind Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    ShaderKind st;
    enum class Shadertype
    {
        NONE=-1,VERTEX =0,FRAGMENT =1
    };

    std::string line;
    std::stringstream ss[2];
    Shadertype type = Shadertype::NONE;
    while((getline(stream,line)))
    {
        if(line.find("#shader")!=std::string::npos)
        {
            if(line.find("vextex")!=std::string::npos)
            {
                type = Shadertype::VERTEX;
            }else if(line.find("fragment")!=std::string::npos)
            {
                type = Shadertype::FRAGMENT;
            }
        }else
        {
             ss[(int)type] << line<<'\n';
        }
    }
    st.Vertex=ss[0].str();
    st.Fragment=ss[1].str();
    return st;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char* src =source.c_str();
    glShaderSource(id,1,&src,nullptr);
    glCompileShader(id);
    
    int result;
    glGetShaderiv(id,GL_COMPILE_STATUS,&result);
    if(result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
        char* message = (char*)alloca(length*sizeof(char));
        glGetShaderInfoLog(id,length,&length,message);
        std::cout <<"Failed to compile" << (type==GL_VERTEX_SHADER ? "Vertex" : "Fragment") << "shader"
        << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }   
    return id;
}

Shader::Shader(const std::string &filepath)
:m_FilePath(filepath)
{
    ShaderKind res = ParseShader(filepath); //获得着色器源代码

    m_RendererID = Createshader(res.Vertex,res.Fragment);
}

Shader::~Shader()
{   
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform4(const std::string& name,float v0,float v1,float v2,float v3)
{
    glUniform4f(GetUniformLocation(name),v0,v1,v2,v3);
}

unsigned int Shader::GetUniformLocation(const std::string &name)
{
    unsigned int location = glGetUniformLocation(m_RendererID,name.c_str());
    if( location == -1 )
        std::cout << "warring from" << name << std::endl;
    return location;
}