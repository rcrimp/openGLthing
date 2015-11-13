#include "wavefront.h"
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdlib.h>

wavefront::wavefront() { }

wavefront::~wavefront() {
   if (VBO != NULL){
      free(VBO);
   }
   if (EBO != NULL){
      free(EBO);
   }
}

void wavefront::open(const char* filename) {
   std::ifstream in(filename, std::ios::in);
   if (!in)
   {
      std::cerr << "Cannot open " << filename << std::endl;
      return;
   }

   std::string line;
   while (std::getline(in, line)) {
      if (line[0] == '#') { // comment

      } else if (line.substr(0, 2) == "v ") { // vertex 
         std::istringstream s(line.substr(2));
         glm::vec3 v;
         s >> v.x;
         s >> v.y;
         s >> v.z;
         vertex.push_back(v);
      } else if (line.substr(0, 3) == "vt ") { // vertex texture co-ord
         std::istringstream s(line.substr(3));
         glm::vec2 vt;
         s >> vt.x;
         s >> vt.y;
         texture.push_back(vt);
      } else if (line.substr(0, 3) == "vn ") { // vertex normal
         std::istringstream s(line.substr(3));
         glm::vec3 vn;
         s >> vn.x;
         s >> vn.y;
         s >> vn.z;
         normal.push_back(vn);
      } else if (line[0] == 'f') { // face
         std::istringstream s(line.substr(2));
         int u1, u2, u3;
         face_t f;

         for (int i = 0; i < 3; i++) {
            u1 = u2 = u3 = 0;
            s >> u1;
            s.get();
            if (s.peek() != '/') 
               s >> u2;
            s.get();
            s >> u3;

            f.vertex_index[i] = u1 - 1;
            f.texture_index[i] = u2 - 1;
            f.normal_index[i] = u3 - 1;
         }

         face.push_back(f);
      } else {
          std::cerr << "unkown line '" << line << "'" << std::endl;
      }
   }

   // print temporary storage 
   /*
   std::cerr << "vertex count = " << vertex.size() << std::endl;
   for (int i = 0; i < vertex.size(); i++) {
      std::cerr << "vertex[" << i << "] = (" << vertex[i].x << ", " << vertex[i].y << ", " << vertex[i].z << ")" << std::endl;
   }
   std::cerr << "normal count = " << normal.size() << std::endl;
   for (int i = 0; i < normal.size(); i++) {
      std::cerr << "normal[" << i << "] = (" << normal[i].x << ", " << normal[i].y << ", " << normal[i].z << ")" << std::endl;
   }
   std::cerr << "texture count = " << texture.size() << std::endl;
   for (int i = 0; i < texture.size(); i++) {
      std::cerr << "texture[" << i << "] = (" << texture[i].x << ", " << texture[i].y << ")" << std::endl;
   }
   std::cerr << "face count = " << face.size() << std::endl;
   for (int i = 0; i < face.size(); i++){ 
      std::cerr << "face[" << i << "] = ";
         for (int j = 0; j < 3; j++){
            std::cerr << "(" << face[i].vertex_index[j] << " "
               << face[i].texture_index[j] << " "
               << face[i].normal_index[j] << ") ";
         }
         std::cerr << std::endl;
   }
   */

   /* replace with new/delete */
   unsigned VBO_s = (3 + 3 + 2); // position, normal, texture
   unsigned EBO_s = 3; // triangles
   VBO_len = vertex.size() * VBO_s ;
   EBO_len = face.size() * EBO_s; // triangle faces 

   VBO = (GLfloat*)malloc(VBO_len * sizeof(VBO[0])); 
   EBO = (GLuint*)malloc(EBO_len * sizeof(EBO[0])); 

   
   for (int v = 0; v < vertex.size(); v++){
      // position vector
      VBO[v * VBO_s + 0] = vertex[v].x;
      VBO[v * VBO_s + 1] = vertex[v].y;
      VBO[v * VBO_s + 2] = vertex[v].z;
   }      
   for (int f = 0; f < face.size(); f++) {
      for (int i = 0; i < 3; i++){ // for each face vertex
         // element VBO 
         EBO[f * EBO_s + i] = face[f].vertex_index[i];      
      }

      for (int i = 0; i < 3; i++){ // for each face normal
         unsigned vi = face[f].vertex_index[i]; 
         unsigned ni = face[f].normal_index[i];

         // normal vector
         VBO[vi * VBO_s + 3] = normal[ni].x; 
         VBO[vi * VBO_s + 4] = normal[ni].y; 
         VBO[vi * VBO_s + 5] = normal[ni].z; 

         // texture coord
         VBO[vi * VBO_s + 6] = 0;
         VBO[vi * VBO_s + 7] = 0;
      }

   }
/*
   std::cerr << "VBO length = " << VBO_len << std::endl;
   for (int i = 0; i < VBO_len; i++){
      std::cerr << VBO[i] << " ";
   }
   std::cerr << std::endl;

   std::cerr << "EBO length = " << EBO_len << std::endl;
   for (int i = 0; i < EBO_len; i++) {
      std::cerr << EBO[i] << " ";
   }
   std::cerr << std::endl;
*/

}
