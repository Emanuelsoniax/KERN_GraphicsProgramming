#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "stb_image.h"

class Terrain
{
	private:
	const char* heightmap;
	GLenum format;
	int comp;
	GLuint terrainVAO;
	GLuint heightmapID;

	public:
	GLuint program;
	int boxSize, indexCount;

	Terrain(GLuint& _program, const char* _heightmap, float _hScale, float _xzScale) {
		program = _program;
		heightmap = _heightmap;
		format = GL_RGBA;
		comp = 4;

		terrainVAO = generatePlane(_hScale, _xzScale, indexCount);
	}

	void renderTerrain(Camera _cam, glm::vec3 _lightDir, glm::mat4 _projection) {
		glEnable(GL_DEPTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);

		glUseProgram(program);
		//matrices
		glm::mat4 world = glm::mat4(1.0f);
		world = glm::translate(world, glm::vec3(-500, -500, -500));
		//world = glm::scale(world, glm::vec3(0.5f, 0.5f, 0.5f));

		glUniformMatrix4fv(glGetUniformLocation(program, "world"), 1, GL_FALSE, glm::value_ptr(world));
		glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(_cam.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

		glUniform3fv(glGetUniformLocation(program, "lightDirection"), 1, glm::value_ptr(_lightDir));
		glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, glm::value_ptr(_cam.Position));
		
		//textures
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, tex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, heightmapID);

		//rendering
		glBindVertexArray(terrainVAO);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}

	unsigned int generatePlane(float _hScale, float _xzScale, int _indexCount) {

		int width, height, channels;
		unsigned char* data = nullptr;

		if (heightmap != nullptr) {
			data = stbi_load(heightmap, &width, &height, &channels, comp);
			if (data) {
				glGenTextures(1, &heightmapID);
				glBindTexture(GL_TEXTURE_2D, heightmapID);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		int stride = 8;
		float* vertices = new float[(width * height) * stride];
		unsigned int* indices = new unsigned int[(width - 1) * (height - 1) * 6];

		int index = 0;
		for (int i = 0; i < (width * height); i++) {
			//calculate x/z values
			int x = i % width;
			int z = i / width;

			//set position
			vertices[index++] = x * _xzScale;
			vertices[index++] = 0;
			vertices[index++] = z * _xzScale;

			//set normal
			vertices[index++] = 0;
			vertices[index++] = 1;
			vertices[index++] = 0;

			//set uv
			vertices[index++] = x / (float)width;
			vertices[index++] = z / (float)height;
		}

		index = 0;
		for (int i = 0; i < (width - 1) * (height - 1); i++) {
			//calculate x/z values
			int x = i % (width - 1);
			int z = i / (width - 1);

			int vertex = z * width + x;

			indices[index++] = vertex;
			indices[index++] = vertex + width;
			indices[index++] = vertex + width + 1;

			indices[index++] = vertex;
			indices[index++] = vertex + width + 1;
			indices[index++] = vertex + 1;
		}

		unsigned int vertSize = (width * height) * stride * sizeof(float);
		indexCount = ((width - 1) * (height - 1) * 6);

		unsigned int VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		// vertex information!
		// position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, 0);
		glEnableVertexAttribArray(0);
		// normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(1);
		// uv
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)(sizeof(float) * 6));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		delete[] vertices;
		delete[] indices;

		stbi_image_free(data);

		return VAO;
	}
};

