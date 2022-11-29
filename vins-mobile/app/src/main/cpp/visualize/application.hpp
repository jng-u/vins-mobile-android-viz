#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <iostream>
#include <vector>
#include <cmath>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

#include "utils.hpp"
#include "vertex.hpp"

class application
{
private:
	EGLContext eglContext;
	/// same with program obejct (may be..)
	GLuint      programObject;

	/// Window width
	GLint       width;
	/// Window height
	GLint       height;

	/// model view projection data
	glm::mat4 	mvp;
	glm::vec3	cameraPosition;
	glm::vec3	cameraOrientation;
	float r;
	
	/// flag
	bool isDrawAxis;

	/// temporal vertex vector
	std::vector<vertex *> vertexVector;
public:
	application();
	~application();

	int init();
	void resize(int width, int height);
	void draw();
	void shutdown();

	void updateMVP();

	void cameraRotation(double x, double y);
	void cameraDistance(double yScroll);

	void updateVertex(int index, GLfloat *vertices, GLsizei length);
    void updateIndex(int index, GLuint *indices, GLsizei length);

	void addLine(float *vertices, GLsizei length);
	void addLine(std::vector<float> vertices);
	void addPoints(float *vertices, GLsizei length);
	void addPoints(std::vector<float> vertices);
	void vertexDataClear();
private:
	GLuint loadShader(GLenum type, const char *shaderSrc);

	void createObject();
	void makeGrid(float x, float y, int limit);

	void drawAxis();
};

#endif // __APPLICATION_H__