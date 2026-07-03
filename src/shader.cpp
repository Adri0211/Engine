#include "shader.h"

std::string get_file_contents(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode = get_file_contents(vertexPath);
    std::string fragmentCode = get_file_contents(fragmentPath);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    // Set up vertex shader and compile it
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Check if vertex shader's compilation was successful
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" 
            "%s\n", infoLog);
    }

    // Set up fragment shader and compile it
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Check if fragment shader's compilation was successful
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" 
            "%s\n", infoLog);
    }

    // Create shader program object and link vertex and fragment shaders
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // Check if creation and linking was successful
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" 
            "%s\n", infoLog);
    }

    // Delete shaders now that we've already created the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
    // Activate the shader for use
    glUseProgram(ID);
}

void Shader::Delete()
{
    glDeleteProgram(ID);
}