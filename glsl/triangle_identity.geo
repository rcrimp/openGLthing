#version 330 core
layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

in VS_OUT {
   vec3 colour;
   vec3 normal;
} gs_in[];

out vec3 colour;

void main() {
   int i;
   
   for (i = 0; i < gl_in.length(); i++) {
      colour = gs_in[i].colour; 
      gl_Position = gl_in[i].gl_Position;
      EmitVertex();
   }
   EndPrimitive();
}
