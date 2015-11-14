#pragma once

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "object.h"
#include "glsl.h"

class engine {
   public:
      engine();
      ~engine();

      void run();

   private:
      int windowWidth;
      int windowHeight;
      char *windowName;
      bool quit = false;
      SDL_Window *window = NULL;
      SDL_GLContext glContext = NULL;

      bool debug_lines = false;

      object testObject;
      glsl testShader;
      glsl normalShader;

      void init();
      void input();
      void update();
      void render();
      void deinit();

      void createWindow(const char* title, unsigned width, unsigned height);
      void destroyWindow();
      void createOpenGLContext();
      void initShaders();
      void createScene();
      void destroyScene();

      void printGLSupport();
};

