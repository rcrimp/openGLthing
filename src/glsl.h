#pragma once

#include <GL/glew.h>

class glsl {
   public:
      glsl();
      ~glsl();

      void compile(const char* vertexSourceFilePath, const char* fragmentSourceFilePath);
      void link();
      void enable();
      void disable();
      void addAttribute(const char* attributeName);

      GLuint program, vertexShader, fragmentShader;
   private:
      int attributeCount;

      void compile(const char* sourceFilePath, GLuint id);
};


