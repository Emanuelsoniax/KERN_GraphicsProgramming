#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//forward declaration
void processInput(GLFWwindow* window);
int init(GLFWwindow* &window);

void createTriangle(GLuint& VAO,GLuint &EBO , int& size, int& numIndices);
void createShaders();
void createProgram(GLuint& programID, const char* vertex, const char* fragment);

//util
void loadFile(const char* filename, char*& output);

//program IDs
GLuint simpleProgram;

int main()
{
    GLFWwindow* window;
    int result = init(window);
    if (result != 0) {
        return result;
    }

    GLuint triangleVAO, triangleEBO;
    int triangleSize, triangleIndexCount;
    createTriangle(triangleVAO, triangleEBO, triangleSize, triangleIndexCount);
    createShaders();


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

        glUseProgram(simpleProgram);

        glBindVertexArray(triangleVAO);
        //glDrawArrays(GL_TRIANGLES, 0, triangleSize);
        glDrawElements(GL_TRIANGLES, triangleIndexCount, GL_UNSIGNED_INT, 0);

        //buffers swappen
        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int init(GLFWwindow*& window)
{
    //init glfw
    glfwInit();

    //window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create window & make context current
    window = glfwCreateWindow(1280, 720, "Hello World!", nullptr, nullptr);
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

    return 0;
}

void createTriangle(GLuint& VAO,GLuint &EBO , int& size, int& numIndices)
{
    float vertices[] = {
        //position                  //color
        -0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f,          0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f,          0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f,           1.0f, 1.0f, 1.0f, 1.0f
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,   // first triangle
        2, 1, 3    // second triangle
    };
    int stride = (3 + 4) * sizeof(float);
    size = sizeof(vertices) / stride;
    numIndices = sizeof(indices) / sizeof(int);

    //buffers
    //referentie naar de vertex array
    glGenVertexArrays(1, &VAO);
    //configuratie id, binding
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    //configuratie id, binding
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //layout data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void createShaders()
{
    createProgram(simpleProgram, "shaders/simpleVertext.shader", "shaders/simpleFragment.shader");
}

void createProgram(GLuint& programID, const char* vertex, const char* fragment) {
    //create a GL program with a vertex & fragment shader
    char* vertexSource;
    char* fragmentSource;

    loadFile(vertex, vertexSource);
    loadFile(fragment, fragmentSource);

    GLuint vertexShaderID, fragmentShaderID;

    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &vertexSource, nullptr);
    glCompileShader(vertexShaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShaderID, 512, nullptr, infoLog);
        std::cout << "ERROR COMPILING VERTEX SHADER\n" << infoLog << std::endl;
    }

    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShaderID);

    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderID, 512, nullptr, infoLog);
        std::cout << "ERROR COMPILING FRAGMENT SHADER\n" << infoLog << std::endl;
    }

    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(programID, 512, nullptr, infoLog);
        std::cout << "ERROR LINKING PROGRAM\n" << infoLog << std::endl;
    }


    //cleanup
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    delete vertexSource;
    delete fragmentSource;

}

void loadFile(const char* filename, char*& output) {
    //open the file
    std::ifstream file(filename, std::ios::binary);

    //if file was succesfully opened
    if (file.is_open()) {
        //get file length
        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);

        //allocate memory for char pointer
        output = new char[length + 1];

        //read data as a block
        file.read(output, length);

        //add null terminator to end of char pointer
        output[length] = '\0';

        //close the file
        file.close();
    }
    else {
        //if the file failed to open, set the char pointer to NULL
        output = NULL;
    }
}