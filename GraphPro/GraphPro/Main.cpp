#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "Skybox.h"
#include "Cube.h"
#include "Terrain.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//forward declaration
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
int init(GLFWwindow* &window);

void createShaders();
void createProgram(GLuint& programID, const char* vertex, const char* fragment);
GLuint loadTexture(const char* path);

//util
void loadFile(const char* filename, char*& output);

//program IDs
GLuint simpleProgram, skyProgram, terrainProgram;

const int WIDTH = 1280, HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

//light
glm::vec3 lightDirection = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
glm::vec3 cameraPosition = glm::vec3(0.0f, 2.5f, -5.0f);

glm::mat4 view, projection;


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    GLFWwindow* window;
    int result = init(window);
    if (result != 0) {
        return result;
    }
    
    camera.MovementSpeed = 100;

    createShaders();

    Skybox skybox = Skybox(skyProgram);
    Cube crate = Cube(simpleProgram, glm::vec3(0, 0, 0), loadTexture("textures/container2.png"), loadTexture("textures/container2_normal.png"), loadTexture("textures/container2_specular.png"));
    Cube brick = Cube(simpleProgram, glm::vec3(-1.5f, -2.2f, -2.5f), loadTexture("textures/brick.png"), loadTexture("textures/brick_normal.png"));
    Terrain terrain = Terrain(terrainProgram, "textures/Heightmap2.png", 100.0f, 5.0f);

    //create gl viewport
    glViewport(0, 0, WIDTH, HEIGHT);

    //render loop
    while (!glfwWindowShouldClose(window)) {
        //input
        processInput(window);

        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //pass projection matrix to shader (note that in this case it could change every frame)
        projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 4000.0f);

        //rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skybox.renderSkyBox(camera, lightDirection, projection);
        terrain.renderTerrain(camera, lightDirection, projection);
        //brick.renderCube(camera, lightDirection, projection);
        //crate.renderCube(camera, lightDirection, projection);

        //buffers swappen
        glfwSwapBuffers(window);
        //events pollen
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
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
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World!", nullptr, nullptr);
    if (window == nullptr) {
        //error
        std::cout << "Failed to create window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        //error
        std::cout << "Error loading GLAD!" << std::endl;
        glfwTerminate();
        return -1;
    }

    return 0;
}

void createShaders()
{
    createProgram(simpleProgram, "shaders/simpleVertext.shader", "shaders/simpleFragment.shader");
    createProgram(skyProgram, "shaders/skyVertexShader.shader", "shaders/skyFragmentShader.shader");
    createProgram(terrainProgram, "shaders/terrainVertexShader.shader", "shaders/terrainFragmentShader.shader");
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

GLuint loadTexture(const char* path) {
    
    //Gen & bind ID
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //load texture
    int width, height, numChannels;
    unsigned char* data = stbi_load(path, &width, &height, &numChannels, 0);
    //set data
    if (data) {
        if (numChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else if (numChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Error loading texture: " << path << std::endl;
    }

    //unload texture
    stbi_image_free(data);

    //unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}