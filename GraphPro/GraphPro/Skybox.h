#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"


class Skybox
{
	private:

	public:
	GLuint program;
    GLuint VAO;
    int boxSize, boxIndexCount;

    Skybox(GLuint& _program) {
        createGeometry(VAO, boxSize, boxIndexCount);
        program = _program;
    }

    void renderSkyBox(Camera _cam, glm::vec3 _lightDir, glm::mat4 _projection) {
        //glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH);

        //createGeometry(boxVAO, boxEBO, boxSize, boxIndexCount);

        glUseProgram(program);
        //matrices
        glm::mat4 world = glm::mat4(1.0f);
        world = glm::translate(world, _cam.Position);
        world = glm::scale(world, glm::vec3(100, 100, 100));

        glUniformMatrix4fv(glGetUniformLocation(program, "world"), 1, GL_FALSE, glm::value_ptr(world));
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(_cam.GetViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

        glUniform3fv(glGetUniformLocation(program, "lightDirection"), 1, glm::value_ptr(_lightDir));
        glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, glm::value_ptr(_cam.Position));

        //rendering
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, boxIndexCount, GL_UNSIGNED_INT, 0);

        //glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH);
    }

    void createGeometry(GLuint& VAO, int& size, int& numIndices)
    {
        // need 24 vertices for normal/uv-mapped Cube
        float vertices[] = {
            // positions            // normals      
            0.5f, -0.5f, -0.5f,     0.f, -1.f, 0.f,
            0.5f, -0.5f, 0.5f,      0.f, -1.f, 0.f,
            -0.5f, -0.5f, 0.5f,     0.f, -1.f, 0.f,
            -0.5f, -0.5f, -.5f,     0.f, -1.f, 0.f,

            0.5f, 0.5f, -0.5f,      1.f, 0.f, 0.f,
            0.5f, 0.5f, 0.5f,       1.f, 0.f, 0.f,

            0.5f, 0.5f, 0.5f,       0.f, 0.f, 1.f,
            -0.5f, 0.5f, 0.5f,      0.f, 0.f, 1.f,

            -0.5f, 0.5f, 0.5f,      -1.f, 0.f, 0.f,
            -0.5f, 0.5f, -.5f,      -1.f, 0.f, 0.f,

            -0.5f, 0.5f, -.5f,      0.f, 0.f, -1.f,
            0.5f, 0.5f, -0.5f,      0.f, 0.f, -1.f,

            -0.5f, 0.5f, -.5f,      0.f, 1.f, 0.f,
            -0.5f, 0.5f, 0.5f,      0.f, 1.f, 0.f,

            0.5f, -0.5f, 0.5f,      0.f, 0.f, 1.f,
            -0.5f, -0.5f, 0.5f,     0.f, 0.f, 1.f,

            -0.5f, -0.5f, 0.5f,     -1.f, 0.f, 0.f,
            -0.5f, -0.5f, -.5f,     -1.f, 0.f, 0.f,

            -0.5f, -0.5f, -.5f,     0.f, 0.f, -1.f,
            0.5f, -0.5f, -0.5f,     0.f, 0.f, -1.f,

            0.5f, -0.5f, -0.5f,     1.f, 0.f, 0.f,
            0.5f, -0.5f, 0.5f,      1.f, 0.f, 0.f,

            0.5f, 0.5f, -0.5f,      0.f, 1.f, 0.f,
            0.5f, 0.5f, 0.5f,       0.f, 1.f, 0.f,
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


        int stride = (3 + 3) * sizeof(float);
        size = sizeof(vertices) / stride;
        numIndices = sizeof(indices) / sizeof(int);

        //buffers
        unsigned int VBO, EBO;
        //referentie naar de vertex array
        glGenVertexArrays(1, &VAO);
        //configuratie id, binding
        glBindVertexArray(VAO);

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
        //normals
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(3);
    }
};

