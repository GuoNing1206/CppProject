#version 330

uniform int nStart;           // ��ʼ֡
uniform int nEnd;             // ��ֹ֡
uniform int nVertexCount;     // ������
uniform float fBlend;         // �����϶�

uniform mat4 mvpMatrix;
uniform samplerBuffer vertexBuff; // ���㻺��

in vec4 vVertex;    // ����λ��
in vec4 vIndex;     // ��������
in vec2 vTexCoord;  // ��������

out vec2 vVaryingTexCoord;

void main(void)
{
  int nVIndex = int(vIndex.r);
  
  int nStartIndex = nStart * nVertexCount;
  vec4 oStartVec = texelFetch(vertexBuff, nStartIndex + nVIndex);
  oStartVec.w = 0;
  
  int nEndIndex = nEnd * nVertexCount;
  vec4 oEndVec = texelFetch(vertexBuff, nEndIndex + nVIndex);
  oEndVec.w = 0;
  
  vec4 oVert = oStartVec * (1.0 - fBlend) + oEndVec * fBlend;

  gl_Position = mvpMatrix * (vVertex + oVert);
  vVaryingTexCoord = vTexCoord;
}
