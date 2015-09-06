/*===============================================================

    ��ͼ��ʾ 2015.05.18
  
===============================================================*/
#version 330 compatibility

uniform sampler2D colorMap;
uniform float fBlend;         // �����϶�

in vec2 vVaryingTexCoord;
out vec4 vFragColor;

/*------------------------------------------------
  2015/05/18 ����:
------------------------------------------------*/
void main(void)
{
  vFragColor = texture(colorMap, vVaryingTexCoord);
}
