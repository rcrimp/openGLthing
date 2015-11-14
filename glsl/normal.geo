#version 330 core
layout (triangles) in;
layout (line_strip) out;
layout (max_vertices = 6) out;

in VS_OUT {
   vec3 normal;
} gs_in[];

const float magnitude = 0.4f;

void main() {
   int i;
   
   for (i = 0; i < gl_in.length(); i++) {
      gl_Position = gl_in[i].gl_Position;
      EmitVertex();
      
      gl_Position = gl_in[i].gl_Position + vec4(gs_in[i].normal, 0.0f) * magnitude;
      EmitVertex();
      
      EndPrimitive();
   }
}
