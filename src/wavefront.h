#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

class wavefront {
   public:
      std::vector<glm::vec3> vertex;
      std::vector<glm::vec3> normal;
      std::vector<glm::vec2> uv;
      std::vector<GLuint> face;

      wavefront();
      ~wavefront();

      void open(const char* filename);
};
