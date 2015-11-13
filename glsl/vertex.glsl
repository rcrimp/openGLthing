#version 330 core
layout(location = 0) in vec3 vertex;
layout(location = 2) in vec3 normal;

out vec4 colour;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
void main() {
   vec4 t = inverse(model) * vec4(3, 4, 10, 1);
   vec3 light = normalize(vec3(t.x, t.y, t.z));
   float i = max(0.1, dot(normal, light)); 
   
   colour = vec4(i, i, i, 1.0f);
   gl_Position = proj * view * model * vec4(vertex, 1.0);
}
