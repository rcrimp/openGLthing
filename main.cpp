#include <stdio.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <random>

#define WIND_WIDTH 800 
#define WIND_HEIGHT 600 

void createWindow(const char* title, unsigned width, unsigned height);
void createOpenGLContext();
void destroyWindow();
void input();
void render();

SDL_Window *window = NULL;
SDL_GLContext glContext = NULL;
bool quit = false;

void createGLScene();
void destroyGLScene();

GLuint shaderProgram;
GLuint fragShader;
GLuint vertexShader;
GLuint vbo;
GLuint uniTime;

GLint uniModel, uniView, uniProj;
glm::mat4 model, view, proj;

struct wavefront {
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec2> uv;
	std::vector<GLuint> face;
};

void importWavefront(const char *filename, wavefront &mesh) {
	std::ifstream in(filename, std::ios::in);
	if (!in)
	{
		std::cerr << "Cannot open " << filename << std::endl;
		return;
	}

	std::string line;
	while (std::getline(in, line)) {
		//std::cerr << line << std::endl;
		if (line[0] == '#') { // comment

		}
		else if (line[0] == 'v') { // vertex 
			std::istringstream s(line.substr(2));
			glm::vec3 v;
			s >> v.x;
			s >> v.y;
			s >> v.z;
			//mesh.face.push_back(mesh.vertex.size());
			mesh.vertex.push_back(v);
		}
		else if (line[0] == 'f') { // face
			std::istringstream s(line.substr(2));
			unsigned v1, v2, v3, vt1, vt2, vt3, vn1, vn2, vn3;
			s >> v1;
			if (s.peek() == '/') {
				while (s.peek() != ' ')
					s.get();
			}
			s >> v2;
			if (s.peek() == '/') {
				while (s.peek() != ' ')
					s.get();
			}
			s >> v3;

			//std::cerr << v1 << " " << v2 << " " << v3 << std::endl;
			mesh.face.push_back(v1-1);
			mesh.face.push_back(v2-1);
			mesh.face.push_back(v3-1);
		}
		else if (line.substr(0, 2) == "vt") { // vertex texture co-ord
			std::istringstream s(line.substr(3));
			glm::vec2 vt;
			s >> vt.x;
			s >> vt.y;
			mesh.uv.push_back(vt);
		}
		else if (line.substr(0, 2) == "vn") { // vertex normal
			std::istringstream s(line.substr(3));
			glm::vec3 vn;
			s >> vn.x;
			s >> vn.y;
			s >> vn.z;
			mesh.normal.push_back(vn);
		}
		else {
			std::cerr << "unkown line '" << line << "'" << std::endl;
		}
	}
}

int main(int argc, char *argv[]){
	createWindow("title", WIND_WIDTH, WIND_HEIGHT);
	createOpenGLContext();
	createGLScene();

	//GLfloat t = 0.0;
	view = glm::lookAt(
	glm::vec3(3.0f, 3.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f)
	);
	proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
	
	clock_t start, end;
	size_t t = 0;
	start = clock();
	do {
		input();
		model = glm::rotate(model, glm::radians(0.2f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		render();

		if (t++ > 10000) {
			end = clock();
			printf("%f\n", 10000 / (((float)(end - start)) / CLOCKS_PER_SEC));
			start = end;
			t = 0;
		}

	} while (!quit);

	destroyGLScene();
	destroyWindow();
	return 0;
}

void createWindow(const char* title, unsigned width, unsigned height) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); //4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2); //5);
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
}
void createOpenGLContext() {
	/* gl */
	glContext = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_FLAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* glew */
	glewExperimental = GL_TRUE;
	glewInit();
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	printf("%u\n", vertexBuffer);
}
void destroyWindow(){
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}
void input(){
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0){
		if (e.type == SDL_QUIT){
			quit = true;
		}
	}
}
void createGLScene() {
	GLint status;
	char eBuffer[512];

	wavefront mesh;
	importWavefront("../model.obj", mesh);

	/*for (int i = 0; i < mesh.vertex.size(); i++){
		std::cerr << "v(" << mesh.vertex[i].x << ", " << mesh.vertex[i].y << ", " << mesh.vertex[i].z << ')' << std::endl;
	}
	for (int i = 0; i < mesh.face.size(); i++){
		std::cerr << mesh.face[i] << std::endl;
	}*/

	/* vertex shader source */
	const GLchar* vertexShaderSrc =
		"#version 150 core\n"
		"in vec3 position;"
		"out vec3 normal;"
		"out vec3 light;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 proj;"
		"void main() {"
		"	normal = normalize(position);"
		"	vec4 t = inverse(model) * vec4(10, 1, 5, 1.0);"
		"	light = normalize(vec3(t.x, t.y, t.z));"
		"   gl_Position = proj * view * model * vec4(position, 1.0);"
		"}";
	/* fragment shader source */
	const GLchar* fragmentShaderSrc =
		"#version 150 core\n"
		"out vec4 outColour;"
		"in vec3 normal;"
		"in vec3 light;"
		"void main() {"
		"	float i = max (0.1, dot(normal, light));"
		"	outColour = vec4(i, i, i, 1.0f);"
		"}";

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/* load vertex data to VRAM */
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertex.size() * sizeof (mesh.vertex[0]), mesh.vertex.data(), GL_STATIC_DRAW);

	/* load element buffer to VRAM */
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.face.size() * sizeof (mesh.face[0]), mesh.face.data(), GL_STATIC_DRAW);

	/* compile vertex shader for GPU */
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(vertexShader, 512, NULL, eBuffer);
	printf("%s\n", eBuffer);

	/* compile the fragment shader for GPU */
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(fragShader, 512, NULL, eBuffer);
	printf("%s\n", eBuffer);

	/* create, link, use shader program */
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glBindFragDataLocation(shaderProgram, 0, "outColour");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	/* vertex data layout */
	/* vertex positions */
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	//GLint colAttrib = glGetAttribLocation(shaderProgram, "colour");
	//glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(colAttrib);

	uniModel = glGetUniformLocation(shaderProgram, "model");
	uniView = glGetUniformLocation(shaderProgram, "view");
	uniProj = glGetUniformLocation(shaderProgram, "proj");

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}
void destroyGLScene() {
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vbo);
}
void render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 3936 * 3, GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(window);
}