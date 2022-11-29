#include "application.hpp"

application::application()
: eglContext(eglGetCurrentContext()) {
	r = 5;
	cameraPosition = {0, r, 0};
	cameraOrientation = {0, 0, -1};
	updateMVP();

	isDrawAxis = true;
}

application::~application()
{
    if (eglGetCurrentContext() != eglContext)
        return;
    glDeleteProgram(programObject);
}

///
/// \brief Initialize the shader and program object
///
int application::init() {
	char vShaderStr[] =
		"#version 320 es									\n"
		"uniform mat4 mvp;									\n"
		// "uniform vec4 color;								\n"
		"layout(location = 0) in vec3 inPosition;			\n"
		"layout(location = 1) in vec4 inColor;				\n"
		"layout(location = 0) out vec4 fragColor;			\n"
		"void main() {										\n"
		"	gl_Position = mvp * vec4(inPosition, 1.0);		\n"
		// "	fragColor = color;								\n"
		"	fragColor = inColor;							\n"
		"}													\n";

	char fShaderStr[] =
		"#version 320 es                              	\n"
		"precision mediump float;                     	\n"
		"layout(location = 0) in vec4 fragColor;		\n"
		"layout(location = 0) out vec4 outColor;		\n"
		"void main() {									\n"
		"   outColor = fragColor;						\n"
		"}                                            	\n";

	GLuint vertexShader;
	GLuint fragmentShader;

	// Load the vertex/fragment shaders
	vertexShader = loadShader(GL_VERTEX_SHADER, vShaderStr);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fShaderStr);

	// Create the program object
	programObject = glCreateProgram();
	if (programObject == 0) {
		return 0;
	}

	// Attach shader
	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	// Link the program
	glLinkProgram(programObject);

	// Check the link status
	GLint linked;
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint infoLen = 0;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1) {
			char *infoLog = (char *)calloc(infoLen, sizeof(char));

			glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
			glesLog("Error linking program:\n%s\n", infoLog );

			free(infoLog);
		}
		glDeleteProgram(programObject);
		return false;
	}

	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	// Set the viewport
	glViewport(0, 0, width, height);

	/// 
	createObject();
	return true;
}

///
/// \brief window resize function
///
void application::resize(int width, int height) {
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
}

///
/// \brief Draw a triangle using the shader pair created in Init()
///
void application::draw() {
	// // Set the viewport
	// glViewport(0, 0, width, height);

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Use the program object
	glUseProgram(programObject);
	
	// binding uniform buffer
	GLint mvpLocation = glGetUniformLocation(programObject, "mvp");
	// GLint colorLocation = glGetUniformLocation(programObject, "color");
	glUniformMatrix4fv(mvpLocation, 1, false, &mvp[0][0]);
	// glUniform4f(colorLocation, 1, 0, 0, 1);

	glLineWidth(3);
	if(isDrawAxis) vertexVector[0]->bind();

	glLineWidth(2);
	if(vertexVector.size()>1) vertexVector[1]->bind();

	glLineWidth(1);
	for(uint i=2; i<vertexVector.size(); i++) vertexVector[i]->bind();
}

void application::shutdown() {
	glDeleteProgram(programObject);
	// free(indices);
	// free(vertices);
}

void application::updateMVP() {
	glm::mat4 model, view, proj;

	model = glm::identity<glm::mat4>();

	view = glm::lookAt (cameraPosition,
						{0, 0, 0},
						// cameraPosition + cameraOrientation,
						{0, 0, 1});

	proj = glm::perspective(glm::radians(60.0f), 		            //fov-y
							width / (float) height,					//aspect 
							0.001f,						            //z near
							100.0f);					            //z far

	mvp = proj*view*model;
}

void application::cameraRotation(double h, double v) {
	float theta = atan2(cameraPosition.y, cameraPosition.x) - h * M_PI / 180.0;
	float phi = atan2(cameraPosition.z, glm::length(glm::vec2({cameraPosition.x, cameraPosition.y}))) + v * M_PI / 180.0;
	if (cos(phi) < 0) {
		if (phi > 0) phi = M_PI_2-5e-8;
		else phi = -M_PI_2+5e-8;
	} 
	
	cameraPosition.x = r * cos(phi) * cos(theta);
	cameraPosition.y = r * cos(phi) * sin(theta);
	cameraPosition.z = r * sin(phi);
//    glesLog("x(%f), y(%f), z(%f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);
	updateMVP();
}

void application::cameraDistance(double yScroll) {
	cameraPosition /= r;
	r -= yScroll;
	if (r <= 0) r = 0.01;
	cameraPosition *= r;
	updateMVP();
}

void application::updateVertex(int index, GLfloat *vertices, GLsizei length) {
	// if (index == 2) {
	// 	int newLen = length + vertexVector[index]->getVertexSize();
	// 	GLfloat *newVer = new GLfloat[newLen];
	// 	memcpy(newVer, vertexVector[index]->getVertices(), vertexVector[index]->getVertexSize()*sizeof(GLfloat));
	// 	// std::cout<<"asdsad";
	// 	memcpy(newVer+vertexVector[index]->getVertexSize(), vertices, length*sizeof(GLfloat));
	// 	vertexVector[index]->setVertices(newVer, newLen);
	// } else 
		vertexVector[index]->setVertices(vertices, length);
	// vertexVector[1]->setColor(1, 0, 0, 1);
}

void application::updateIndex(int index, GLuint *indices, GLsizei length) {
	vertexVector[index]->setIndices(indices, length);
}

void application::addLine(float *vertices, GLsizei length) {
	int index_length = length/3*2 - 2;
    GLuint *indices = new GLuint[index_length];
	for(int i=0; i<index_length; i++) indices[i] = (i+1)/2;

	vertex *line = new vertex(GL_LINES);
	line->setVertices(vertices, length);
	line->setColor(1, 0, 1, 1);
	line->setIndices(indices, index_length);
	vertexVector.push_back(line);
}

void application::addLine(std::vector<float> vertices) {
	int length = vertices.size();
	int index_length = length/3*2 - 2;
    GLuint *indices = new GLuint[index_length];
	for(int i=0; i<index_length; i++) indices[i] = (i+1)/2;

	vertex *line = new vertex(GL_LINES);
	line->setVertices(vertices.data(), length);
	line->setColor(1, 0, 1, 1);
	line->setIndices(indices, index_length);
	vertexVector.push_back(line);
}

void application::addPoints(float *vertices, GLsizei length) {
	// int index_length = length/3*2 - 2;
    // GLuint *indices = new GLuint[index_length];
	// for(int i=0; i<index_length; i++) indices[i] = (i+1)/2;

	vertex *points = new vertex(GL_POINTS);
	points->setVertices(vertices, length);
	points->setColor(1, 1, 0, 1);
	// path->setIndices(indices, index_length);
	vertexVector.push_back(points);
}

void application::addPoints(std::vector<float> vertices) {
	vertex *points = new vertex(GL_POINTS);
	points->setVertices(vertices.data(), vertices.size()*3);
	points->setColor(1, 1, 0, 1);
	// path->setIndices(indices, index_length);
	vertexVector.push_back(points);
}

void application::vertexDataClear() {
	while (vertexVector.size() > 2) {
		vertexVector.back()->destroy();
		delete vertexVector.back();
		vertexVector.pop_back();
	}
}

/********************* private functuin ***********************/

///
/// \brief Create a shader object, load the shader source, and compile the shader.
///
GLuint application::loadShader(GLenum type, const char *shaderSrc) {
	// Create the shader object
	GLuint shader;
	shader = glCreateShader(type);
	if (shader == 0) return 0;

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen );
		if (infoLen > 1) {
			char *infoLog = (char *)calloc(infoLen, sizeof(char));

			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			glesLog("Error compiling shader:\n%s\n", infoLog);

			free(infoLog);
		}
		glDeleteShader(shader );
		return 0;
	}

	return shader;

}

void application::createObject() {
	GLfloat vertices[] = { 0, 0, 0,
						   0.3, 0, 0,
						   0, 0, 0,
						   0, 0.3, 0,
						   0, 0, 0,
						   0, 0, 0.3, };
	GLuint indices[] = { 0, 1, 
						 2, 3, 
						 4, 5,};
	GLfloat colors[] = { 1, 0, 0, 1,
						 1, 0, 0, 1,
						 0, 1, 0, 1,
						 0, 1, 0, 1,
						 0, 0, 1, 1,
						 0, 0, 1, 1, };
	vertex *v = new vertex(GL_LINES);
	v->setVertices(vertices, 18);
	v->setColors(colors, 24);
	v->setIndices(indices, 6);
	vertexVector.push_back(v);

	makeGrid(0, 0, 5);



	// vertex *path = new vertex(GL_LINES);
	// path->setVertices(nullptr, 0);
	// path->setColor(1, 0, 1, 1);
	// path->setIndices(nullptr, 0);
	// vertexVector.push_back(path);

	// vertex *point = new vertex(GL_POINTS);
	// point->setVertices(nullptr, 0);
	// point->setColor(1, 1, 0, 1);
	// point->setIndices(nullptr, 0);
	// vertexVector.push_back(point);
}

void application::makeGrid(float x, float y, int limit){

//    GLfloat vertices[] = { 1, -1, 0,
//                           1, 1, 0,
//                           1, 1, 0,
//                           1, -1, 0,};
//    GLuint indices[] = { 0, 1,
//                         2, 3,};
//    GLfloat colors[] = { 1, 1, 1, 1,
//                         1, 1, 1, 1,
//                         1, 1, 1, 1,
//                         1, 1, 1, 1,};
//    vertex *v = new vertex(GL_LINES);
//    v->setVertices(vertices, 12);
//    v->setColors(colors, 16);
//    v->setIndices(indices, 4);
//    vertexVector.push_back(v);

	int gx = floor(x), gy = floor(y);
	std::vector<GLfloat> vertices;
	for(int i=-limit; i<=limit; i++) {
		vertices.push_back(gx + i);
		vertices.push_back(gy - limit);
		vertices.push_back(0);

		vertices.push_back(gx + i);
		vertices.push_back(gy + limit);
		vertices.push_back(0);

		vertices.push_back(gx - limit);
		vertices.push_back(gy + i);
		vertices.push_back(0);

		vertices.push_back(gx + limit);
		vertices.push_back(gy + i);
		vertices.push_back(0);
	}
	std::vector<GLuint> indices(vertices.size()/3);
	for(int i=0; i<indices.size(); i++) indices[i] = i;

	vertex *v = new vertex(GL_LINES);
	v->setVertices(vertices.data(), vertices.size());
	v->setIndices(indices.data(), indices.size());
	v->setColor(0.3, 0.3, 0.3, 1);
	vertexVector.push_back(v);

}