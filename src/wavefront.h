#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

class wavefront {
   private:
      typedef struct face_s {
         int vertex_index[3];
         int texture_index[3];
         int normal_index[3];
      } face_t;

      std::vector<glm::vec3> vertex;
      std::vector<glm::vec3> normal;
      std::vector<glm::vec2> texture;
      std::vector<face_t> face;
   public:
      GLuint VBO_len = 0;
      GLuint EBO_len = 0;

      GLfloat *VBO = NULL;
      GLuint *EBO = NULL;

      wavefront();
      ~wavefront();

      void open(const char* filename);
};
