#version 330 core

// Input vertex data
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal
layout(location = 2) in vec2 vertexUV;

out vec3 Normal;
out vec2 UV;

layout(stdl40) uniform Transform
{
  mat4 world;
  mat4 view;
  mat4 proj;
} transform;

void main()
{
  gl_Position = (transform.world * (transform.view * transform.proj)) * (vec4(vertexPosition, 1));

  Normal = vertexNormal;

  UV = vertexUV;
}