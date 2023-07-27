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
	GLuint VAO, EBO;
    int boxSize, boxIndexCount;

	Skybox(GLuint& _program);
    
	void renderSkyBox(Camera _cam, glm::vec3 _lightDir, glm::mat4 _projection);

	void createGeometry(GLuint& VAO, GLuint& EBO, int& size, int& numIndices);
};

