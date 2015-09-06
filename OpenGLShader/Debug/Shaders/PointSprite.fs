#version 330

uniform sampler2D colorTex;

out vec4 vFragColor;

void main(void)
{
  vec2 p = gl_PointCoord*2.0-vec2(1.0);
  if(dot(p, p) > 1.0)
    discard;
  vFragColor = texture(colorTex, gl_PointCoord);
}