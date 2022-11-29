#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <GLES3/gl3.h>

#include "utils.hpp"

class vertex
{
private:
	/// vertex, index buffer data
	GLfloat 	*vertices;
	GLfloat 	*colors;
	GLfloat		r, g, b, a;
	GLuint 		*indices;
	GLsizei		vertexLength;
	GLsizei		colorLength;
	GLsizei		indexLength;

	GLenum		drawMode;
public:
	vertex(GLenum mode);
	~vertex();

	void destroy();
	void setVertices(GLfloat *vertices, GLsizei length);
	void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void setColors(GLfloat *colors, GLsizei length);
	void setIndices(GLuint *indices, GLsizei length);
	void bind();

	inline GLfloat *getVertices() const { return vertices; }
	inline GLsizei getVertexSize() const { return vertexLength; }
	inline GLfloat *getColors() const { return colors; }
	inline GLsizei getColorSize() const { return colorLength; }
	inline GLuint *getIndices() const { return indices; }
	inline GLsizei getIndexSize() const { return indexLength; }
};

#endif // __VERTEX_H__