#include "texture.h"
#include <stdio.h>
#include <stb/stb_image.h>
#include <GLFW/glfw3.h>

Texture::Texture(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
    type = texType;
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Flip the texture on load to match OpenGL's coordinate system
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);

    glGenTextures(1, &ID);
    glActiveTexture(texType);
    glBindTexture(texType, ID);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(texType, 0, format, width, height, 0, format, pixelType, data);
    glGenerateMipmap(texType);

    stbi_image_free(data);
    glBindTexture(texType, 0);

}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(texUni, unit);
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}