#pragma once
#include <GL/glew.h>
#include "wavefront.h"
#include <glm/glm.hpp>

class object {
   public:
      object();
      ~object();

      void load();
      void draw();

   private:
      GLuint vao;
      GLuint vbo;
      GLuint ebo;

      wavefront mesh;

      GLint posAttrib;
      // shader stuff
      GLuint shaderProgram, fragShader, vertexShader;
      // camera projection stuff
      GLint uniModel, uniView, uniProj;
      glm::mat4 model, view, proj;
};
