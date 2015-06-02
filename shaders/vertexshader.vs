#version 330

uniform mat4 ProjectionViewModelMatrix;

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;

out vec4 vColor;

void main()
{
   gl_Position = ProjectionViewModelMatrix*vec4(Position.x, Position.y, Position.z, 1.0);
   vColor = vec4(Color.r, Color.g, Color.b, 1.0);
}
