/*===============================================================

    贴图显示 2015.05.18
  
===============================================================*/
#version 330 compatibility

uniform sampler2D colorMap0;
uniform sampler2D colorMap1;
uniform float fSaturate;

in vec2 vVaryingTexCoord0;
in vec2 vVaryingTexCoord1;
out vec4 vFragColor;

/*------------------------------------------------
  2015/05/18 功能:
------------------------------------------------*/
vec4 CalcSaturate(vec4 color)
{
  vec4 w = vec4(0.299, 0.587, 0.1141, 0);
  float fGray = dot(w, color);
  return vec4(fGray, fGray, fGray, 1.0);
}

/*------------------------------------------------
  2015/05/18 功能:
------------------------------------------------*/
void main(void)
{
  vec4 vColo1r = texture(colorMap0, vVaryingTexCoord0);
  vec4 vColo1g = texture(colorMap0, vVaryingTexCoord0 + vec2(0.005, 0));
  vec4 vColo1b = texture(colorMap0, vVaryingTexCoord0 + vec2(0.01, 0));
  
  vec4 vColo2r = texture(colorMap1, vVaryingTexCoord1);
  vec4 vColo2g = texture(colorMap1, vVaryingTexCoord1 + vec2(0.005, 0));
  vec4 vColo2b = texture(colorMap1, vVaryingTexCoord1 + vec2(0.01, 0));

  vec2 p = vVaryingTexCoord1 * 2.0 - vec2(1.0);
  float fBlur = 10;
  float aSaturate = clamp(length(p) * fBlur + 1 - fSaturate * fBlur, 0.0, 1.0);

  vFragColor.r = mix(vColo2r.r, vColo1r.r, aSaturate);
  vFragColor.g = mix(vColo2g.g, vColo1g.g, aSaturate);
  vFragColor.b = mix(vColo2b.b, vColo1b.b, aSaturate);
  vFragColor.a = 1.0f;//clamp(2-length(p) * 2, 0.0, 1.0);

//  vFragColor = CalcSaturate(vFragColor);
 
/*
  vFragColor.r = mix(vColo2.r, vColo1.r, 0.5);
  vFragColor.g = mix(vColo2.g, vColo1.g, 0.5);
  vFragColor.b = mix(vColo2.b, vColo1.b, 0.5);
  vFragColor.a = 1.0*/
/*  
  vec4 vDistort = texture(colorMap1, vVaryingTexCoord1 + vec2(0/20, 0));
  vDistort -= vec4(0.5, 0.5, 0.5, 1);
  vDistort *= 0.2*fSaturate;
//  vec4 vColor = texture(colorMap0, vVaryingTexCoord0 + vDistort.xy);
  vec4 vColor = texture(colorMap0, vVaryingTexCoord0);
  vFragColor = vColor;*/
}