#version 330

in vec4 vVertex;

void main(void)
{
  gl_Position = vVertex;
  gl_PointSize = 256;
}