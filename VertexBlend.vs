#version 330

uniform int nStart;           // 起始帧
uniform int nEnd;             // 终止帧
uniform int nVertexCount;     // 顶点数
uniform float fBlend;         // 顶点混合度

uniform mat4 mvpMatrix;
uniform samplerBuffer vertexBuff; // 顶点缓存

in vec4 vVertex;    // 顶点位置
in vec4 vIndex;     // 顶点索引
in vec2 vTexCoord;  // 纹理坐标

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
