
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

#include "Shader.h"
#include "Texture.h"



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
        -0.5f,-0.5f,0.0f,0.0f,
         0.5f, 0.5f,1.0f,1.0f,
         0.5f,-0.5f,1.0f,0.0f,
         -0.5f,0.5f,0.0f,1.0f,
    };
    unsigned int indices[] ={
        0,1,2,
        1,3,0
    }; 

    
    VertexArray va;
    VertexBuffer vb(positions,4*4*sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb,layout);
    IndexBuffer ib(indices,6);
    
    Shader program ("res/shader.shader");
    
    Texture texture("res/texture/redDeadredemption.png");
    texture.Bind();
    program.SetUniform1i("u_Texture",0);

    Renderer renderer;

    
    // 渲染循环
    while (!glfwWindowShouldClose(window)) {
        // 渲染代码...
        
        glClear(GL_COLOR_BUFFER_BIT);
        //program.SetUniform4("u_Color",r,g,0.6f,1.0f);
        renderer.Draw(va,ib,program);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
