/*
	SDL2
*/

#include <stdio.h>
#include <stdlib.h>

#include "glad.h"
#include <SDL2/SDL.h>

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

#define fragmentShaderFile "shaders/triangle.frag"
#define vertexShaderFile   "shaders/triangle.vert"

GLuint LoadShaders(const char* fragmentShader, const char* vertexShader);
char* LoadFile(const char* filePath);

int main(int argc, char** argv)
{
	// Init SDL2
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Unable to init SDL2: %s", SDL_GetError());
		return 1;
	}
	// Create window and OpenGL context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_Window* window = SDL_CreateWindow(
		"SDL2 Window",
    	SDL_WINDOWPOS_CENTERED,
    	SDL_WINDOWPOS_CENTERED,
    	1280, 720, SDL_WINDOW_OPENGL
	);
	if (window == NULL)
	{
		printf("Unable to create window: %s", SDL_GetError());
		return 1;
	}
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == NULL)
	{
		printf("Unable to create OpenGL context: %s", SDL_GetError());
		return 1;
	}
	// GLAD Loader
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));
	/// Window init
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	// Vertex Array Object
	GLuint VertexArrayId;
	glGenVertexArrays(1, &VertexArrayId);
	glBindVertexArray(VertexArrayId);
	/// Load Shaders
	char* fragmentShader = LoadFile(fragmentShaderFile);
	char* vertexShader   = LoadFile(vertexShaderFile);
	GLuint programID = LoadShaders(fragmentShader, vertexShader);
	// Vertex Buffer
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	/// Window main
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Use shader
	glUseProgram(programID);
	// Vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);
	// Draw Triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	// Update buffer
	SDL_GL_SwapWindow(window);
	SDL_Delay(5000);
	return 0;
}

char* LoadFile(const char* filePath)
{
	FILE* f = fopen(filePath, "r");
	fseek(f, 0, SEEK_END);
	long fSize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* shader = malloc(fSize + 1);
	fread(shader, fSize, 1, f);
	fclose(f);
	shader[fSize] = 0;
	return shader;
}

GLuint LoadShaders(const char* fragmentShader, const char* vertexShader)
{
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLint Result = GL_FALSE;
	int InfoLogLength;
	// Compile Fragment Shader
	glShaderSource(fragmentShaderId, 1, &fragmentShader, NULL);
	glCompileShader(fragmentShaderId);
	// Check Fragment Shader
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		char fragmentShaderErrorMessage[InfoLogLength + 1];
		glGetShaderInfoLog(fragmentShaderId, InfoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}
	// Compile Vertex Shader
	glShaderSource(vertexShaderId, 1, &vertexShader, NULL);
	glCompileShader(vertexShaderId);
	// Check Vertex Shader
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		char vertexShaderErrorMessage[InfoLogLength + 1];
		glGetShaderInfoLog(vertexShaderId, InfoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}
	// Create Program
	GLuint programId = glCreateProgram();
	glAttachShader(programId, fragmentShaderId);
	glAttachShader(programId, vertexShaderId);
	glLinkProgram(programId);
	// Check Program
	glGetProgramiv(programId, GL_LINK_STATUS, &Result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		char programErrorMessage[InfoLogLength + 1];
		glGetProgramInfoLog(programId, InfoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}
	// Clean Up
	glDetachShader(programId, fragmentShaderId);
	glDetachShader(programId, vertexShaderId);
	glDeleteShader(fragmentShaderId);
	glDeleteShader(vertexShaderId);
	return programId;
}
