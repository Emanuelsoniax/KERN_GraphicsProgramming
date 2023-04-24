#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
    //init glfw
    glfwInit();
    
    //window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create window & make context current
    GLFWwindow * window = glfwCreateWindow(1280, 720, "Hello World!", nullptr, nullptr);
    if (window == nullptr) {
        //error
        std::cout << "Failed to create window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        //error
        std::cout << "Error loading GLAD!" << std::endl;
        glfwTerminate();
        return -1;
    }

    //create gl viewport
    glViewport(0, 0, 1280, 720);

    //render loop
    while (!glfwWindowShouldClose(window)) {
        //input

        //events pollen
        glfwPollEvents();

        //rendering
        glClearColor(0.8f, 0.9f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //buffers swappen
        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}