#include "glsl.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
#include <fstream>

glsl::glsl() {
   attributeCount = 0;
   program = 0;
   vertexShader = 0;
   fragmentShader = 0;
}
glsl::~glsl() {

}

void glsl::compile(
      const char* vertexSourceFilePath,
      const char* fragmentSourceFilePath,
      const char* geometrySourceFilePath){
   if (vertexSourceFilePath != NULL){
      vertexShader = glCreateShader(GL_VERTEX_SHADER);
      compile(vertexSourceFilePath, vertexShader);
   }

   if (fragmentSourceFilePath != NULL) {
      fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      compile(fragmentSourceFilePath, fragmentShader);
   }

   if (geometrySourceFilePath != NULL) {
      geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
      compile(geometrySourceFilePath, geometryShader);
   }
}
void glsl::link(){
   program = glCreateProgram();

   if (vertexShader != 0)
      glAttachShader(program, vertexShader);
   if (fragmentShader != 0)
      glAttachShader(program, fragmentShader);
   if (fragmentShader != 0)
      glAttachShader(program, geometryShader);

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

   if (vertexShader != 0) {
      glDetachShader(program, vertexShader);
      glDeleteShader(vertexShader);
   }
   if (fragmentShader != 0) {
      glDetachShader(program, fragmentShader);
      glDeleteShader(fragmentShader);
   }
   if (geometryShader != 0) {
      glDetachShader(program, geometryShader);
      glDeleteShader(geometryShader);
   }
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
      std::cerr << "file: " << sourceFilePath << " not found" << std::endl;
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
