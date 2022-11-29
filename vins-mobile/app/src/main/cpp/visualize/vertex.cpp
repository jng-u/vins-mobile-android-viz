#include "vertex.hpp"

vertex::vertex(GLenum mode)
: drawMode(mode) {
	vertices = nullptr;
	colors = nullptr;
	indices = nullptr;
	a = -1;
}

vertex::~vertex() {
}

void vertex::destroy() {
	free(vertices);
	free(colors);
	free(indices);
}

void vertex::setVertices(GLfloat vertices[], GLsizei length) {
	if (this->vertices != nullptr) free(this->vertices);
	vertexLength = length;
	this->vertices = new GLfloat[length];
	if (length == 0) return;
	memcpy(this->vertices, vertices, sizeof(GLfloat) * length);
}

void vertex::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	colorLength = 4;
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void vertex::setColors(GLfloat colors[], GLsizei length) {
	if (this->colors != nullptr) free(this->colors);
	colorLength = length;
	this->colors = new GLfloat[length];
	if (length == 0) return;
	memcpy(this->colors, colors, sizeof(GLfloat) * length);

}

void vertex::setIndices(GLuint *indices, GLsizei length) {
	if (this->indices != nullptr) free(this->indices);
	indexLength = length;
	this->indices = new GLuint[length];
	if (length == 0) return;
	memcpy(this->indices, indices, sizeof(GLfloat) * length);
}

void vertex::bind() {
	assert(vertices != nullptr);
	assert((colors != nullptr) || (a >= 0));
	if (drawMode != GL_POINTS) assert(indices != nullptr);

	if (vertexLength/3*4 != colorLength && a >= 0) {
		colorLength = vertexLength/3*4;
		colors = new GLfloat[colorLength];
		for(int i=0; i<colorLength/4; i++) {
			colors[i*4] = r;
			colors[i*4 + 1] = g;
			colors[i*4 + 2] = b;
			colors[i*4 + 3] = a;
		}
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, colors);
	if(drawMode == GL_POINTS) glDrawArrays(drawMode, 0, vertexLength/3);
	else glDrawElements(drawMode, indexLength, GL_UNSIGNED_INT, indices);
}
