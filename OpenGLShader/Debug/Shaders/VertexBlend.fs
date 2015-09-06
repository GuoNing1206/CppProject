/*===============================================================

    贴图显示 2015.05.18
  
===============================================================*/
#version 330 compatibility

uniform sampler2D colorMap;
uniform float fBlend;         // 顶点混合度

in vec2 vVaryingTexCoord;
out vec4 vFragColor;

/*------------------------------------------------
  2015/05/18 功能:
------------------------------------------------*/
void main(void)
{
  vFragColor = texture(colorMap, vVaryingTexCoord);
}
