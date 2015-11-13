#pragma once

#include <GL/glew.h>

class glsl {
   public:
      glsl();
      ~glsl();

      void compile(
            const char* vertexSourceFilePath,
            const char* fragmentSourceFilePath,
            const char* geometrySourceFilePath);
      void link();
      void enable();
      void disable();
      void addAttribute(const char* attributeName);

      GLuint program,
             vertexShader,
             fragmentShader,
             geometryShader;
   private:
      int attributeCount;

      void compile(const char* sourceFilePath, GLuint id);
};


