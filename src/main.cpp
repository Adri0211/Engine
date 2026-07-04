#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <stdio.h>

#include <shader.h>
#include <VBO.h>
#include <VAO.h>
#include <EBO.h>

float vertices[] = { // Coords (3 floats), Vertex Color (3 floats), UV Texture Coords (2 floats)
     0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,       1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,       1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,       0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,       0.0f, 1.0f  // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   
    GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
    
    if (window == NULL)
    {
        fprintf(stderr, "Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }    

    // Set up openGL's rendering dimensions and the resize callback
    glViewport(0, 0, 800, 800);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Compile and use our default shader program
    Shader defaultShader("../../assets/shaders/default.vert", "../../assets/shaders/default.frag");

    // Set up our Vertex Array Object, Vertex Buffer Object, and Element Buffer Object
    VAO VAO;
    VAO.Bind();
    VBO VBO(vertices, sizeof(vertices));
    EBO EBO(indices, sizeof(indices));

    VAO.LinkAttribute(VBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO.LinkAttribute(VBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO.LinkAttribute(VBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Unbind VAO, VBO, EBO to prevent accidentally modifying them
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();

    // Texture data
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Flip the texture on load to match OpenGL's coordinate system
    unsigned char *data = stbi_load("../../assets/textures/Test_Texture.png", &width, &height, &nrChannels, 0);

    if (!data)
    {
        defaultShader.Delete();
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    GLenum format = GL_RGB;
    if (nrChannels == 1)
    {
        format = GL_RED;
    }
    else if (nrChannels == 3)
    {
        format = GL_RGB;
    }
    else if (nrChannels == 4)
    {
        format = GL_RGBA;
    }
    else
    {
        stbi_image_free(data);
        defaultShader.Delete();
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tex0uni = glGetUniformLocation(defaultShader.ID, "tex0");
    defaultShader.Activate();
    glUniform1i(tex0uni, 0);

    while(!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        defaultShader.Activate();
        glBindTexture(GL_TEXTURE_2D, texture);

        VAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    // de-allocate all resources once they've outlived their purpose
    VAO.Delete();
    VBO.Delete();
    EBO.Delete();
    glDeleteTextures(1, &texture);
    defaultShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}