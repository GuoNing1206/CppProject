#version 330

uniform mat4 mvpMatrix;

in vec2 vTexCoord0;
in vec2 vTexCoord1;
in vec4 vVertex;

out vec2 vVaryingTexCoord0;
out vec2 vVaryingTexCoord1;

void main(void)
{
  vVaryingTexCoord0 = vTexCoord0;
  vVaryingTexCoord1 = vTexCoord1;
  
  gl_Position = mvpMatrix * vVertex;
  
/*  
  vVaryingTexCoord0 = gl_Position.xy / gl_Position.w;
  vVaryingTexCoord0 /= 2;
  vVaryingTexCoord0 += 0.5;
  
  vVaryingTexCoord1 = gl_Position.xy / gl_Position.w;*/
}