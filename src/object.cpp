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
   glDeleteBuffers(1, &ebo);
   glDeleteBuffers(1, &vbo);
   glDeleteVertexArrays(1, &vao);
}

void object::load() {
   mesh.open("mesh/model.obj"); 

   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);

   glGenBuffers(1, &vbo);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, mesh.VBO_len * sizeof(mesh.VBO[0]), mesh.VBO, GL_STATIC_DRAW);

   glGenBuffers(1, &ebo);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO_len * sizeof(mesh.EBO[0]), mesh.EBO, GL_STATIC_DRAW);
   
   GLint posAttrib = 0; //glGetAttribLocation(testShader.program, "position");
   glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(posAttrib);

   GLint norAttrib = 2; // get from shader
   glVertexAttribPointer(norAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); 
   glEnableVertexAttribArray(norAttrib);

   glBindVertexArray(0);
}

void object::draw() {
   glBindVertexArray(vao);
   glDrawElements(GL_TRIANGLES, mesh.EBO_len, GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);
}
