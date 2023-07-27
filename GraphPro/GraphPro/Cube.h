#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"

class Cube
{
    
    GLuint program;
    GLuint VAO, EBO;
    int boxSize, boxIndexCount;
    GLuint tex, normal, specular;
    glm::vec3 cubePosition;

    public:
    Cube(GLuint& _program, glm::vec3 _position , GLuint _tex, GLuint _normal, GLuint _specular) {
        createGeometry(VAO, EBO, boxSize, boxIndexCount);
        program = _program;
        tex = _tex;
        normal = _normal;
        specular = _specular;
        cubePosition = _position;
    }

    Cube(GLuint& _program, glm::vec3 _position , GLuint _tex, GLuint _normal) {
        createGeometry(VAO, EBO, boxSize, boxIndexCount);
        program = _program;
        tex = _tex;
        normal = _normal;
        cubePosition = _position;
    }

    void renderCube(Camera _cam, glm::vec3 _lightDir, glm::mat4 _projection) {
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        glCullFace(GL_BACK);

        glUseProgram(program);

        //set texture channels
        glUniform1i(glGetUniformLocation(program, "mainTex"), 0);
        glUniform1i(glGetUniformLocation(program, "normalTex"), 1);
        glUniform1i(glGetUniformLocation(program, "specularTex"), 2);

        //matrices
        glm::mat4 world = glm::mat4(1.0f);
        world = glm::rotate(world, glm::radians(45.0f), glm::vec3(0, 1, 0));
        world = glm::scale(world, glm::vec3(1, 1, 1));
        world = glm::translate(world, _cam.Position);

        glUniformMatrix4fv(glGetUniformLocation(program, "world"), 1, GL_FALSE, glm::value_ptr(world));
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(_cam.GetViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

        glUniform3fv(glGetUniformLocation(program, "lightPosition"), 1, glm::value_ptr(_lightDir));
        glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, glm::value_ptr(_cam.Position));

        //bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, specular);

        //// create transformations
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::translate(transform, cubePosition);
        //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // get matrix's uniform location and set matrix
        glUniformMatrix4fv(glGetUniformLocation(program, "world"), 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, boxIndexCount, GL_UNSIGNED_INT, 0);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }

    void createGeometry(GLuint& VAO, GLuint& EBO, int& size, int& numIndices)
    {
        // need 24 vertices for normal/uv-mapped Cube
        float vertices[] = {
            // positions            //colors            // tex coords   // normals          //tangents      //bitangents
            0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.f, 1.f,       0.f, -1.f, 0.f,     -1.f, 0.f, 0.f,  0.f, 0.f, 1.f,
            0.5f, -0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   1.f, 0.f,       0.f, -1.f, 0.f,     -1.f, 0.f, 0.f,  0.f, 0.f, 1.f,
            -0.5f, -0.5f, 0.5f,     1.0f, 1.0f, 1.0f,   0.f, 0.f,       0.f, -1.f, 0.f,     -1.f, 0.f, 0.f,  0.f, 0.f, 1.f,
            -0.5f, -0.5f, -.5f,     1.0f, 1.0f, 1.0f,   0.f, 1.f,       0.f, -1.f, 0.f,     -1.f, 0.f, 0.f,  0.f, 0.f, 1.f,

            0.5f, 0.5f, -0.5f,      1.0f, 1.0f, 1.0f,   1.f, 1.f,       1.f, 0.f, 0.f,     0.f, -1.f, 0.f,  0.f, 0.f, 1.f,
            0.5f, 0.5f, 0.5f,       1.0f, 1.0f, 1.0f,   1.f, 0.f,       1.f, 0.f, 0.f,     0.f, -1.f, 0.f,  0.f, 0.f, 1.f,

            0.5f, 0.5f, 0.5f,       1.0f, 1.0f, 1.0f,   1.f, 0.f,       0.f, 0.f, 1.f,     1.f, 0.f, 0.f,  0.f, -1.f, 0.f,
            -0.5f, 0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   0.f, 0.f,       0.f, 0.f, 1.f,     1.f, 0.f, 0.f,  0.f, -1.f, 0.f,

            -0.5f, 0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   0.f, 0.f,      -1.f, 0.f, 0.f,     0.f, 1.f, 0.f,  0.f, 0.f, 1.f,
            -0.5f, 0.5f, -.5f,      1.0f, 1.0f, 1.0f,   0.f, 1.f,      -1.f, 0.f, 0.f,     0.f, 1.f, 0.f,  0.f, 0.f, 1.f,

            -0.5f, 0.5f, -.5f,      1.0f, 1.0f, 1.0f,   0.f, 1.f,      0.f, 0.f, -1.f,     1.f, 0.f, 0.f,  0.f, 1.f, 0.f,
            0.5f, 0.5f, -0.5f,      1.0f, 1.0f, 1.0f,   1.f, 1.f,      0.f, 0.f, -1.f,     1.f, 0.f, 0.f,  0.f, 1.f, 0.f,

            -0.5f, 0.5f, -.5f,      1.0f, 1.0f, 1.0f,   1.f, 1.f,       0.f, 1.f, 0.f,     1.f, 0.f, 0.f,  0.f, 0.f, 1.f,
            -0.5f, 0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   1.f, 0.f,       0.f, 1.f, 0.f,     1.f, 0.f, 0.f,  0.f, 0.f, 1.f,

            0.5f, -0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   1.f, 1.f,       0.f, 0.f, 1.f,     1.f, 0.f, 0.f,  0.f, -1.f, 0.f,
            -0.5f, -0.5f, 0.5f,     1.0f, 1.0f, 1.0f,   0.f, 1.f,       0.f, 0.f, 1.f,     1.f, 0.f, 0.f,  0.f, -1.f, 0.f,

            -0.5f, -0.5f, 0.5f,     1.0f, 1.0f, 1.0f,   1.f, 0.f,       -1.f, 0.f, 0.f,     0.f, 1.f, 0.f,  0.f, 0.f, 1.f,
            -0.5f, -0.5f, -.5f,     1.0f, 1.0f, 1.0f,   1.f, 1.f,       -1.f, 0.f, 0.f,     0.f, 1.f, 0.f,  0.f, 0.f, 1.f,

            -0.5f, -0.5f, -.5f,     1.0f, 1.0f, 1.0f,   0.f, 0.f,       0.f, 0.f, -1.f,     1.f, 0.f, 0.f,  0.f, 1.f, 0.f,
            0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.f, 0.f,       0.f, 0.f, -1.f,     1.f, 0.f, 0.f,  0.f, 1.f, 0.f,

            0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   0.f, 1.f,       1.f, 0.f, 0.f,     0.f, -1.f, 0.f,  0.f, 0.f, 1.f,
            0.5f, -0.5f, 0.5f,      1.0f, 1.0f, 1.0f,   0.f, 0.f,       1.f, 0.f, 0.f,     0.f, -1.f, 0.f,  0.f, 0.f, 1.f,

            0.5f, 0.5f, -0.5f,      1.0f, 1.0f, 1.0f,   0.f, 1.f,       0.f, 1.f, 0.f,     1.f, 0.f, 0.f,  0.f, 0.f, 1.f,
            0.5f, 0.5f, 0.5f,       1.0f, 1.0f, 1.0f,   0.f, 0.f,       0.f, 1.f, 0.f,     1.f, 0.f, 0.f,  0.f, 0.f, 1.f
        };

        unsigned int indices[] = {  // note that we start from 0!
            // DOWN
            0, 1, 2,   // first triangle
            0, 2, 3,    // second triangle
            // BACK
            14, 6, 7,   // first triangle
            14, 7, 15,    // second triangle
            // RIGHT
            20, 4, 5,   // first triangle
            20, 5, 21,    // second triangle
            // LEFT
            16, 8, 9,   // first triangle
            16, 9, 17,    // second triangle
            // FRONT
            18, 10, 11,   // first triangle
            18, 11, 19,    // second triangle
            // UP
            22, 12, 13,   // first triangle
            22, 13, 23,    // second triangle
        };


        int stride = (3 + 3 + 2 + 3 + 3 + 3) * sizeof(float);
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
        //positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);
        //colors
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //uvs
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        //normals
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)(11 * sizeof(float)));
        glEnableVertexAttribArray(4);

        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, stride, (void*)(14 * sizeof(float)));
        glEnableVertexAttribArray(5);

    }

};

