#version 330

uniform vec4 vPanZoom;
uniform sampler2D colorMap;

in vec4 vVaryingColor;
in vec2 vTexCoordF;
out vec4 vFragColor;

void main(void)
{
  int nIter = 0;
  const int nMaxIter = 256;
  
  const float fThresholdSquared = 4.0;
  vec2 vZ = vTexCoordF;
  vZ.x *= vPanZoom.z * 1.3333;
  vZ.y *= vPanZoom.z;
  vZ += vPanZoom.xy;

  // mandelbrot set
  vec2 vC = vZ;
  while(nIter < nMaxIter && dot(vZ, vZ) < fThresholdSquared)
  {
    vec2 vZSquared;
    vZSquared.x = vZ.x * vZ.x - vZ.y * vZ.y;
    vZSquared.y = 2.0 * vZ.x * vZ.y;
    vZ = vZSquared + vC;
    nIter++;
  }
/*
  // julia set
  vec2 vC = vec2(0.32, 0.043);
  while(nIter < nMaxIter && dot(vZ, vZ) < fThresholdSquared)
  {
    vec2 vZSquared;
    vZSquared.x = vZ.x * vZ.x - vZ.y * vZ.y;
    vZSquared.y = 2.0 * vZ.x * vZ.y;
    vZ = vZSquared + vC;
    nIter++;
  }
*/  
  if(nIter == nMaxIter)
    discard;
  else
  {
    float fGray = float(nIter) / float(nMaxIter);
    vFragColor = texture(colorMap, vec2(fGray, 0));
  }
}