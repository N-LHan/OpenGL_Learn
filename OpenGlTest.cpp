
#define GLEW_STATIC
#include <GL/glew.h>
//#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <math.h>


// #define ASSERT(x) if (!(x)) __debugbreak();
// #define GLCall(x) GLClearError();\ 
//     x;\ 
//     ASSERT(GLLogCall(#x, __FILE__, __LINE__))


// static void GLClearError(){
//     while(!glGetError()!= GL_NO_ERROR);
// }

// static bool GLLogCall(const char* Function,const char *file, int line){
//     while(GLenum error =  glGetError())
//     {
//         std::cout << "OpenGl Error" << error << " " << Function << " "
//          << file  << " " << line << std::endl;
//         return false;
//     }
//     return true;
// }

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
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    
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

    unsigned int vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    
    unsigned int buffer=1;
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(positions),positions,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*2,0);
    glEnableVertexAttribArray(0);

    unsigned int ibo;
    glGenBuffers(1,&ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);



    std::pair<std::string,std::string> res = ParseShader("res/shader.shader"); //获得着色器源代码

    unsigned int shader = Createshader(res.first,res.second);
    glUseProgram(shader);

    unsigned int location = glGetUniformLocation(shader,"u_Color");

    float c_x = 0.09f;
    float increment = 0.05f;
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
        
        if (c_x <0.0f ){
            increment = 0.05f;
        }else if(c_x >0.9f){
            increment = -0.05f;
        }
        c_x+=increment;
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
