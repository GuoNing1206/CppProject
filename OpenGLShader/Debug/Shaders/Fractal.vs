#version 330

in vec4 vVertex;
in vec4 vColor;
in vec2 vTexCoord;

out vec4 vVaryingColor;
out vec2 vTexCoordF;

void main(void)
{
  vVaryingColor = vColor;
  gl_Position = vVertex;
  vTexCoordF = vTexCoord;
}