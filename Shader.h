#pragma onca
#ifndef _SHADER_H
#define _SHADER_H



#include <string>


struct ShaderKind
{
    std::string Vertex;
    std::string Fragment;
};

 class Shader
 {
 private:
    std::string m_FilePath;
    unsigned int m_RendererID;
 public:
    
   Shader(const std::string &filepath);
   ~Shader();
   
   void Bind() const;
   void Unbind() const;

   //Set uniforms
   void SetUniform4(const std::string& name,float v0,float v1,float v2,float v3);
private:
    unsigned int GetUniformLocation(const std::string & name );
    unsigned int CompileShader(unsigned int type,const std::string &source);
    ShaderKind ParseShader(const std::string& filepath);
    int Createshader(const std::string&vertexShader, const std::string& fragmentShader);
 };
 
 #endif
 