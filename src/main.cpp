#include "glm/ext/matrix_clip_space.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <Texture.h>
#include <VBO.h>
#include <VAO.h>
#include <EBO.h>
#include <Texture.h>

const unsigned int width = 800;
const unsigned int height = 800;

float vertices[] = { // Coords (3 floats), Vertex Color (3 floats), UV Texture Coords (2 floats)
     0.5f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f,       1.0f, 0.0f, // front right
     0.5f, -0.0f, -0.5f,     0.0f, 1.0f, 0.0f,       0.0f, 0.0f, // back right
    -0.5f, -0.0f, -0.5f,     0.0f, 0.0f, 1.0f,       1.0f, 0.0f, // back left
    -0.5f,  0.0f, 0.5f,     1.0f, 1.0f, 0.0f,       0.0f, 0.0f,  // front left 
    -0.0f,  0.8f, 0.0f,     1.0f, 0.0f, 1.0f,       0.5f, 1.0f,  // top vert
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3,
    4, 0, 3,
    4, 1, 2,
    4, 1, 0,
    4, 2, 3
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
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    
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
    glViewport(0, 0, width, height);
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

    // Load and bind our texture
    std::string texPath = std::string(ASSETS_PATH) + "textures/Test_Texture.png";
    Texture texture(texPath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    
    texture.texUnit(defaultShader, "tex0", 0);

    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        defaultShader.Activate();

        // Transform matrices
        glm::mat4 model = glm::mat4(1.0);
        glm::mat4 view = glm::mat4(1.0);
        glm::mat4 proj = glm::mat4(1.0);

        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)(width/height), 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(defaultShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(defaultShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projLoc = glGetUniformLocation(defaultShader.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        texture.Bind();

        VAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    // de-allocate all resources once they've outlived their purpose
    VAO.Delete();
    VBO.Delete();
    EBO.Delete();
    texture.Delete();
    defaultShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}