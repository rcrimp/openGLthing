#include "engine.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

engine::engine() {
   //TODO load settings from a file
   windowWidth = 800;
   windowHeight = 600;
   windowName = "title";
}

engine::~engine() {
}

void engine::run() {
   init();

   while (!quit){
      input();
      update();
      render();
   }

   deinit();
}

void engine::init() {
   createWindow(windowName, windowWidth, windowHeight);
   createOpenGLContext();
   createScene();
}

void engine::deinit() {
   destroyScene();
   destroyWindow();
}

void engine::createWindow(const char* title, unsigned width, unsigned height) {
   SDL_Init(SDL_INIT_VIDEO);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
   window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
} 

void engine::destroyWindow() {
   SDL_DestroyWindow(window);
   window  = NULL;
   SDL_Quit();
}

void engine::createOpenGLContext() {
   /* gl */
   glContext = SDL_GL_CreateContext(window);
   SDL_GL_SetSwapInterval(0);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glShadeModel(GL_FLAT);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   /* glew */
   glewExperimental = GL_TRUE;
   glewInit();
   //glGenBuffers(1, &vertexBuffer);
   //printf("%u\n", vertexBuffer);
}
void engine::createScene() {
   GLint status;
   char eBuffer[512];

   test.load();

   //glFrontFace(GL_CCW);
   //glCullFace(GL_BACK);
   //glEnable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
}

void engine::destroyScene() {
   /* glDeleteProgram(shaderProgram);
      glDeleteShader(fragShader);
      glDeleteShader(vertexShader);
      glDeleteBuffers(1, &vbo);
      glDeleteVertexArrays(1, &vao); 
    */
}

void engine::input() {
   SDL_Event e;
   while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
         quit = true;
      }
   }
}

void engine::update() {

}

void engine::render() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   test.draw();

   SDL_GL_SwapWindow(window);
}
