
#define GLEW_STATIC
#include <GL/glew.h>
//#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <math.h>

#include "VertexArray.h"
#include "Renderer.h"                               
#include "VertexBuffer.h"
#include "IndexBuffer.h"

static std::pair<std::string,std::string> ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

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
    
    return std::pair<std::string,std::string>  (ss[0].str(),ss[1].str());
}
static unsigned int CompileShader(unsigned int type,const std::string &source)
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
static int Createshader(const std::string&vertexShader, const std::string& fragmentShader)
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

int main() {
    // 初始化GLFW
    std::cout << sizeof(float) << std::endl;
    glfwInit();
    
    

    
    // 创建窗口和OpenGL上下文
    GLFWwindow* window = glfwCreateWindow(800, 600, "GLAD Example", NULL, NULL);
    glfwMakeContextCurrent(window);
    if(glewInit()!=GLEW_OK){
        std::cout << "Error" <<std::endl;
    }


    glfwSwapInterval(2);
    
    float positions[]{
        -0.5f,-0.5f,
         0.5f, 0.5f,
         0.5f,-0.5f,
         -0.5f,0.5f
    };
    unsigned int indices[] ={
        0,1,2,
        1,3,0
    };

    
    VertexArray va;
    VertexBuffer vb(positions,2*4*sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    va.AddBuffer(vb,layout);
    IndexBuffer ib(indices,6);
    



    std::pair<std::string,std::string> res = ParseShader("res/shader.shader"); //获得着色器源代码

    unsigned int shader = Createshader(res.first,res.second);
    glUseProgram(shader);

    unsigned int location = glGetUniformLocation(shader,"u_Color");

    float time=0.0f;
    float r = 0.0f;
    // 渲染循环
    while (!glfwWindowShouldClose(window)) {
        // 渲染代码...
        time = glfwGetTime();
        r=(sin(time)+1.0f)/2.0f;
        float g =(cos(time)+1.0f)/2.0f;
        glClear(GL_COLOR_BUFFER_BIT);
        glUniform4f (location,r,g,0.6f,1.0f);
        GLCall(glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
