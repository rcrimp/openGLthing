#version 150 core
out vec4 outColour;
in vec3 normal;
in vec3 light;
void main() {
   float i = max(0.1, dot(normal, light));
   outColour = vec4(i, i, i, 1.0f);
}
