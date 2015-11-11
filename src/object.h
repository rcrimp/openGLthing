#pragma once
#include <GL/glew.h>
#include "wavefront.h"
#include <glm/glm.hpp>
#include "glsl.h"

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
};
