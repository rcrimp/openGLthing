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
   private:
      int attributeCount;
      
      void compile(const char* sourceFilePath, GLuint id);
      
      GLuint program, vertexShader, fragmentShader;
};      

