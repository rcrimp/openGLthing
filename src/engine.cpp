#include "engine.h"
#include <iostream>

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
   printGLSupport();
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
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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
   SDL_GL_SetSwapInterval(1);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glShadeModel(GL_FLAT);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   /* glew */
   glewExperimental = GL_TRUE;
   glewInit();
}

void engine::createScene() {
   initShaders();
   testObject.load();
   
   glFrontFace(GL_CCW);
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);

   if (debug_lines) {
      /* line mode for debugging */
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glLineWidth(1.0f);
      /* line antialiasing */
      glEnable(GL_BLEND);
      glEnable(GL_LINE_SMOOTH);
      glHint(GL_LINE_SMOOTH, GL_NICEST);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   }
}

void engine::destroyScene() {

}

void engine::initShaders() {
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
   testShader.compile(
         "glsl/lambert.vtx",
         "glsl/pass_colour.frg",
         NULL); //"glsl/triangle_identity.geo");
   testShader.link();
   
   normalShader.compile(
         "glsl/normal.vtx",
         "glsl/const_colour.frg",
         "glsl/normal.geo");
   normalShader.link();

   // shader uniforms
   GLuint uniModel = glGetUniformLocation(testShader.program, "model");
   GLuint uniView =  glGetUniformLocation(testShader.program, "view");
   GLuint uniProj =  glGetUniformLocation(testShader.program, "proj");
   if (uniModel == -1 || uniView == -1 || uniProj == -1) {
      std::cerr << "GLSL error: unable to obtain uniform locations" << std::endl;
   }

   glm::mat4 view = glm::lookAt(
         glm::vec3(3.0f, 3.0f, 0.0f),
         glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3(0.0f, 0.0f, 1.0f)
         );
   glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
   static glm::mat4 model = glm::mat4();
   model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0, 0.0, 1.0f));

   testShader.enable();
   glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
   glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
   glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
   testShader.disable();

   normalShader.enable();
   glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
   glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
   glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
   normalShader.disable();
}

void engine::render() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   testShader.enable();
   testObject.draw();
   testShader.disable();

   normalShader.enable();
   testObject.draw();
   normalShader.disable();

   SDL_GL_SwapWindow(window);
}

void engine::printGLSupport() {
   GLint val;
   glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &val);
   std::cout << "Maximum vertex attributes: " << val << std::endl;

   glGetIntegerv(GL_MAX_LIGHTS, &val);
   std::cout << "Maximum lights: " << val << std::endl;
}
