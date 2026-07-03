#ifndef SHADER_CALSS_H
#define SHADER_CALSS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>

std::string get_file_contents(const char* filename);

class Shader
{
    public:
        GLuint ID;
        Shader(const char* vertexPath, const char* fragmentPath);

        void Activate();
        void Delete();

};

#endif