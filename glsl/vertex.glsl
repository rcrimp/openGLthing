#version 150 core
in vec3 position;
out vec3 normal;
out vec3 light;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
void main() {
   normal = normalize(position);
   vec4 t = inverse(model) * vec4(10, 1, 5, 1);
   light = normalize(vec3(t.x, t.y, t.z));
  gl_Position = proj * view * model * vec4(position, 1.0);
}
