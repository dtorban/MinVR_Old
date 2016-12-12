#include <iostream>

#include <graphics/VRGraphicsApp.h>
#include "math/VRMath.h"

#if defined(WIN32)
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>
#elif defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#endif

using namespace MinVR;

class MyVRApp : public VRGraphicsApp {
public:
	MyVRApp(int argc, char** argv) : VRGraphicsApp(argc, argv) {}
	virtual ~MyVRApp() {
	}

	void onVREvent(const std::string &eventName, VRDataIndex *eventData) {
		std::cout << eventName << std::endl;
		if (eventName == "/KbdEsc_Down") {
			running = false;
		}
	}

	void onVRRenderContext(VRGraphicsState& renderState) {
		if (!running) {
			glDeleteBuffers(1, &vbo);
			glDeleteVertexArrays(1, &vao);
			glDetachShader(shaderProgram, vshader);
			glDetachShader(shaderProgram, fshader);
			glDeleteShader(vshader);
			glDeleteShader(fshader);
			glDeleteProgram(shaderProgram);
			return;
		}

		if (renderState.isInitialRenderCall()) {
			// Init GL
			glEnable(GL_DEPTH_TEST);
			glClearDepth(1.0f);
			glDepthFunc(GL_LEQUAL);
			glClearColor(0, 0, 0, 1);

			// Create VBO
			GLfloat vertices[]  = { 1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,  -1.0f,-1.0f, 1.0f,      // v0-v1-v2 (front)
					-1.0f,-1.0f, 1.0f,   1.0f,-1.0f, 1.0f,   1.0f, 1.0f, 1.0f,      // v2-v3-v0

					1.0f, 1.0f, 1.0f,   1.0f,-1.0f, 1.0f,   1.0f,-1.0f,-1.0f,      // v0-v3-v4 (right)
					1.0f,-1.0f,-1.0f,   1.0f, 1.0f,-1.0f,   1.0f, 1.0f, 1.0f,      // v4-v5-v0

					1.0f, 1.0f, 1.0f,   1.0f, 1.0f,-1.0f,  -1.0f, 1.0f,-1.0f,      // v0-v5-v6 (top)
					-1.0f, 1.0f,-1.0f,  -1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,      // v6-v1-v0

					-1.0f, 1.0f, 1.0f,  -1.0f, 1.0f,-1.0f,  -1.0f,-1.0f,-1.0f,      // v1-v6-v7 (left)
					-1.0f,-1.0f,-1.0f,  -1.0f,-1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,      // v7-v2-v1.0

					-1.0f,-1.0f,-1.0f,   1.0f,-1.0f,-1.0f,   1.0f,-1.0f, 1.0f,      // v7-v4-v3 (bottom)
					1.0f,-1.0f, 1.0f,  -1.0f,-1.0f, 1.0f,  -1.0f,-1.0f,-1.0f,      // v3-v2-v7

					1.0f,-1.0f,-1.0f,  -1.0f,-1.0f,-1.0f,  -1.0f, 1.0f,-1.0f,      // v4-v7-v6 (back)
					-1.0f, 1.0f,-1.0f,   1.0f, 1.0f,-1.0f,   1.0f,-1.0f,-1.0f };    // v6-v5-v4

			// normal array
			GLfloat normals[]   = { 0, 0, 1,   0, 0, 1,   0, 0, 1,      // v0-v1-v2 (front)
					0, 0, 1,   0, 0, 1,   0, 0, 1,      // v2-v3-v0

					1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v3-v4 (right)
					1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v5-v0

					0, 1, 0,   0, 1, 0,   0, 1, 0,      // v0-v5-v6 (top)
					0, 1, 0,   0, 1, 0,   0, 1, 0,      // v6-v1-v0

					-1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v1-v6-v7 (left)
					-1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v7-v2-v1

					0,-1, 0,   0,-1, 0,   0,-1, 0,      // v7-v4-v3 (bottom)
					0,-1, 0,   0,-1, 0,   0,-1, 0,      // v3-v2-v7

					0, 0,-1,   0, 0,-1,   0, 0,-1,      // v4-v7-v6 (back)
					0, 0,-1,   0, 0,-1,   0, 0,-1 };    // v6-v5-v4

			// color array
			GLfloat colors[]    = { 1, 1, 1,   1, 1, 0,   1, 0, 0,      // v0-v1-v2 (front)
					1, 0, 0,   1, 0, 1,   1, 1, 1,      // v2-v3-v0

					1, 1, 1,   1, 0, 1,   0, 0, 1,      // v0-v3-v4 (right)
					0, 0, 1,   0, 1, 1,   1, 1, 1,      // v4-v5-v0

					1, 1, 1,   0, 1, 1,   0, 1, 0,      // v0-v5-v6 (top)
					0, 1, 0,   1, 1, 0,   1, 1, 1,      // v6-v1-v0

					1, 1, 0,   0, 1, 0,   0, 0, 0,      // v1-v6-v7 (left)
					0, 0, 0,   1, 0, 0,   1, 1, 0,      // v7-v2-v1

					0, 0, 0,   0, 0, 1,   1, 0, 1,      // v7-v4-v3 (bottom)
					1, 0, 1,   1, 0, 0,   0, 0, 0,      // v3-v2-v7

					0, 0, 1,   0, 0, 0,   0, 1, 0,      // v4-v7-v6 (back)
					0, 1, 0,   0, 1, 1,   0, 0, 1 };    // v6-v5-v4

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER_ARB, vbo);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices)+sizeof(normals)+sizeof(colors), 0, GL_STATIC_DRAW);
			glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, sizeof(vertices), vertices);
			glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices), sizeof(normals), normals);
			glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices)+sizeof(normals), sizeof(colors), colors);

			// Create vao
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER_ARB, vbo);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (char*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (char*)0 + sizeof(vertices));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (char*)0 + sizeof(vertices) + sizeof(normals));

			// Create shader
			std::string vertexShader =
					"#version 330 \n"
					"layout(location = 0) in vec3 position; "
					"layout(location = 1) in vec3 normal; "
					"layout(location = 2) in vec3 color; "
					""
					"uniform mat4 ProjectionMatrix; "
					"uniform mat4 ViewMatrix; "
					"uniform mat4 ModelMatrix; "
					"uniform mat4 NormalMatrix; "
					""
					"out vec3 col;"
					""
					"void main() { "
					"	gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vec4(position, 1.0); "
					"	col = color;"
					"}";
			vshader = compileShader(vertexShader, GL_VERTEX_SHADER);

			std::string fragmentShader =
					"#version 330 \n"
					"in vec3 col;"
					"out vec4 colorOut;"
					""
					"void main() { "
					"	colorOut = vec4(col, 1.0); "
					"}";
			fshader = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

			shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vshader);
			glAttachShader(shaderProgram, fshader);
			linkShaderProgram(shaderProgram);
		}
	}

	void onVRRenderScene(VRGraphicsState& renderState) {
		if (running) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glUseProgram(shaderProgram);
			GLint loc = glGetUniformLocation(shaderProgram, "ProjectionMatrix");
			glUniformMatrix4fv(loc, 1, GL_FALSE, renderState.getProjectionMatrix());
			loc = glGetUniformLocation(shaderProgram, "ViewMatrix");
			glUniformMatrix4fv(loc, 1, GL_FALSE, renderState.getViewMatrix());

			float model[16];
			VRMatrix4 modelMatrix = VRMatrix4::rotationX(0.01*frame);
			modelMatrix = modelMatrix * VRMatrix4::rotationY(0.01*frame);
			for (int f = 0; f < 16; f++) {
				model[f] = modelMatrix.m[f];
			}

			loc = glGetUniformLocation(shaderProgram, "ModelMatrix");
			glUniformMatrix4fv(loc, 1, GL_FALSE, model);

			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			glUseProgram(0);
		}
	}

	GLuint compileShader(const std::string& shaderText, GLuint shaderType) {
		const char* source = shaderText.c_str();
		int length = shaderText.size();
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &source, &length);
		glCompileShader(shader);
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE) {
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> log(length);
			glGetShaderInfoLog(shader, length, &length, &log[0]);
			std::cerr << &log[0];
		}

		return shader;
	}

	void linkShaderProgram(GLuint shaderProgram) {
		glLinkProgram(shaderProgram);
		GLint status;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
		if(status == GL_FALSE) {
			GLint length;
			glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> log(length);
			glGetProgramInfoLog(shaderProgram, length, &length, &log[0]);
			std::cerr << "Error compiling program: " << &log[0] << std::endl;
		}
	}

private:
	GLuint vbo, vao, vshader, fshader, shaderProgram;
};

int main(int argc, char **argv) {
	MyVRApp app(argc, argv);
	app.run();
	return 0;
}
