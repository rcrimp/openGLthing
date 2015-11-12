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
   
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
   testShader.compile("glsl/vertex.glsl", "glsl/fragment.glsl");
   testShader.link();

   // shader uniforms
   GLint uniModel = glGetUniformLocation(testShader.program, "model");
   GLint uniView =  glGetUniformLocation(testShader.program, "view");
   GLint uniProj =  glGetUniformLocation(testShader.program, "proj");

   glm::mat4 view = glm::lookAt(
         glm::vec3(3.0f, 3.0f, 3.0f),
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
}

void engine::render() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   testShader.enable();
   testObject.draw();
   testShader.disable();

   SDL_GL_SwapWindow(window);
}
