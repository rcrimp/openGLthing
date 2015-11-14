#version 330 core
out vec4 outColour;
in vec3 colour;
void main() {
   outColour = vec4(colour, 1.0);
}
