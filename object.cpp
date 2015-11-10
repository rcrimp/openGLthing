#include "object.h"
#include "wavefront.h"
#include <iostream>

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

object::object() {
   vbo = 0;
   vao = 0;
   ebo = 0;
}

object::~object() {
   glDeleteBuffers(1, &vbo);
   glDeleteVertexArrays(1, &vao);
}

void object::load() {
   GLint status;
   char eBuffer[512];

   mesh.open("model.obj"); 

   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);

   if (vbo == 0) {
      glGenBuffers(1, &vbo);
   }
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, mesh.vertex.size() * sizeof(float), mesh.vertex.data(), GL_STATIC_DRAW);
   //   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glGenBuffers(1, &ebo);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.face.size() * sizeof(GLuint), mesh.face.data(), GL_STATIC_DRAW);
   //  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER
}

void object::draw() {
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glEnableVertexAttribArray(0);
  
   glDrawElements(GL_TRIANGLES, mesh.face.size(), GL_UNSIGNED_INT, 0);

   glDisableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
