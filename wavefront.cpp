#include "wavefront.h"
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

wavefront::wavefront() { }

wavefront::~wavefront() { }

void wavefront::open(const char* filename) {
   std::ifstream in(filename, std::ios::in);
   if (!in)
   {
      std::cerr << "Cannot open " << filename << std::endl;
      return;
   }

   std::string line;
   while (std::getline(in, line)) {
      //std::cerr << line << std::endl;
      if (line[0] == '#') { // comment

      }
      else if (line[0] == 'v') { // vertex 
         std::istringstream s(line.substr(2));
         glm::vec3 v;
         s >> v.x;
         s >> v.y;
         s >> v.z;
         //mesh.face.push_back(mesh.vertex.size());
        vertex.push_back(v);
      }
      else if (line[0] == 'f') { // face
         std::istringstream s(line.substr(2));
         unsigned v1, v2, v3, vt1, vt2, vt3, vn1, vn2, vn3;
         s >> v1;
         if (s.peek() == '/') {
            while (s.peek() != ' ')
               s.get();
         }
         s >> v2;
         if (s.peek() == '/') {
            while (s.peek() != ' ')
               s.get();
         }
         s >> v3;

         //std::cerr << v1 << " " << v2 << " " << v3 << std::endl;
         face.push_back(v1-1);
         face.push_back(v2-1);
         face.push_back(v3-1);
      }
      else if (line.substr(0, 2) == "vt") { // vertex texture co-ord
         std::istringstream s(line.substr(3));
         glm::vec2 vt;
         s >> vt.x;
         s >> vt.y;
         uv.push_back(vt);
      }
      else if (line.substr(0, 2) == "vn") { // vertex normal
         std::istringstream s(line.substr(3));
         glm::vec3 vn;
         s >> vn.x;
         s >> vn.y;
         s >> vn.z;
         normal.push_back(vn);
      }
      else {
         std::cerr << "unkown line '" << line << "'" << std::endl;
      }
   }
}
