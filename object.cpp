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
}

object::~object() {
   glDeleteProgram(shaderProgram);
   glDeleteShader(fragShader);
   glDeleteShader(vertexShader);
   glDeleteBuffers(1, &vbo);
   glDeleteVertexArrays(1, &vao);
}

void object::load() {
   GLint status;
   char eBuffer[512];

   mesh.open("model.obj"); 

//   float vertexData[24] = {
//      -0.5f, -0.5f,  0.5f,
//      0.5f, -0.5f,  0.5f,
//      0.5f,  0.5f,  0.5f,
//      -0.5f,  0.5f,  0.5f,
//      -0.5f, -0.5f, -0.5f,
//      0.5f, -0.5f, -0.5f,
//      0.5f,  0.5f, -0.5f,
//      -0.5f,  0.5f, -0.5,
//   };
//   GLuint elementData[36] = {
//      0, 1, 2,
//      2, 3, 0,
//      3, 2, 6,
//      6, 7, 3,
//      7, 6, 5,
//      5, 4, 7,
//      4, 5, 1,
//      1, 0, 4,
//      4, 0, 3,
//      3, 7, 4,
//      1, 5, 6,
//      6, 2, 1,
//   };

   const GLchar* vertexShaderSrc = 
      "#version 150 core\n"
      "in vec3 position;"
      "uniform mat4 model;"
      "uniform mat4 view;"
      "uniform mat4 proj;"
      "void main() {"
      "  gl_Position = proj * view * model * vec4(position, 1.0);"
      "}";
   const GLchar* fragmentShaderSrc = 
      "#version 150 core\n"
      "out vec4 outColour;"
      "void main() {"
      "   outColour = vec4(1.0f, 0.0f, 0.0f, 1.0f);"
      "}";

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

   vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
   glCompileShader(vertexShader);
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
   glGetShaderInfoLog(vertexShader, 512, NULL, eBuffer);
   std::cerr << eBuffer << std::endl;

   fragShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragShader, 1, &fragmentShaderSrc, NULL);
   glCompileShader(fragShader);
   glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
   glGetShaderInfoLog(fragShader, 512, NULL, eBuffer);
   std::cerr << eBuffer << std::endl;

   shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragShader);
   glBindFragDataLocation(shaderProgram, 0, "outColour");
   glLinkProgram(shaderProgram);
   glUseProgram(shaderProgram);

   posAttrib = glGetAttribLocation(shaderProgram, "position");
   glEnableVertexAttribArray(posAttrib);

   uniModel = glGetUniformLocation(shaderProgram, "model");
   uniView =  glGetUniformLocation(shaderProgram, "view");
   uniProj =  glGetUniformLocation(shaderProgram, "proj");

   view = glm::lookAt(
         glm::vec3(3.0f, 3.0f, 3.0f),
         glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3(0.0f, 0.0f, 1.0f)
         );
   proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
   model = glm::mat4();

   glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
   glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
   glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
}

void object::draw() {
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glEnableVertexAttribArray(0);
  
   glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
   glDrawElements(GL_TRIANGLES, mesh.face.size(), GL_UNSIGNED_INT, 0);

   glDisableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
