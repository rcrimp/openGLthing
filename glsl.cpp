#include "glsl.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <fstream>

glsl::glsl() {
   attributeCount = 0;
   program = 0;
   vertexShader = 0;
   fragmentShader = 0;
}
glsl::~glsl() {

}

void glsl::compile(const char* vertexSourceFilePath, const char* fragmentSourceFilePath){
   vertexShader = glCreateShader(GL_VERTEX_SHADER);
   fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   compile(vertexSourceFilePath, vertexShader);
   compile(fragmentSourceFilePath, fragmentShader);
}
void glsl::link(){
   program = glCreateProgram();

   glAttachShader(program, vertexShader);
   glAttachShader(program, fragmentShader);

   //TODO fix this 
   glBindFragDataLocation(program, 0, "outColour");
   glLinkProgram(program);

   GLint status = 0;
   glGetProgramiv(program, GL_LINK_STATUS, (int*)&status);
   if (status == GL_FALSE) {
      GLint maxLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<char> errorLog(maxLength);
      glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

      glDeleteProgram(program);
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      std::printf("%s\n", &(errorLog[0]));
   }
   
   glDetachShader(program, vertexShader);
   glDetachShader(program, fragmentShader);
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);

   /* delete me */
   // enableShader
   glUseProgram(program);

   // more shader shit
   GLuint uniModel = glGetUniformLocation(program, "model");
   GLuint uniView =  glGetUniformLocation(program, "view");
   GLuint uniProj =  glGetUniformLocation(program, "proj");

   glm::mat4 view = glm::lookAt(
         glm::vec3(3.0f, 3.0f, 3.0f),
         glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3(0.0f, 0.0f, 1.0f)
         );
   glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
   glm::mat4 model = glm::mat4();

   glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
   glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
   glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
}

void glsl::enable(){
   glUseProgram(program);
   //for(int i = 0; i < attributeCount; i++){
   //   glEnableVertexAttribArray(i);
   //}
}

void glsl::disable(){
   glUseProgram(0);
   //for (int i = 0; i < attributeCount; i++){
   //   glDisableVertexAttribArray(i);
   //}
}

void glsl::addAttribute(const char* attributeName){
   glBindAttribLocation(program, attributeCount++, attributeName);
}

void glsl::compile(const char* sourceFilePath, GLuint id) {
   std::ifstream shaderFile(sourceFilePath);
   if (shaderFile.fail()) {
      return;
   };

   std::string fileContents = "";
   std::string line;
   while (std::getline(shaderFile, line)) {
      fileContents += line + "\n";
   }
   shaderFile.close();

   const GLchar* srcPtr = (const GLchar*)fileContents.c_str();

   glShaderSource(id, 1, &srcPtr, NULL);
   glCompileShader(id); 

   GLint status = 0;
   glGetShaderiv(id, GL_COMPILE_STATUS, &status);

   if (status == GL_FALSE) {
      GLint maxLength = 0;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<char> errorLog(maxLength);
      glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

      glDeleteShader(id);

      std::printf("%s\n", &(errorLog[0]));
   }
}
