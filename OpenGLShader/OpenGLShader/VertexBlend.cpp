/*===========================================================================

	功能:     2015/06/04

===========================================================================*/
#include "stdafx.h"
#include "VertexBlend.h"

#ifdef _DEBUG
#undef THIS_FILE
		static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*===========================================================================

	功能:     2015/06/02

===========================================================================*/
VertexBlend::VertexBlend(GLA::ScreenTextPtr A_Text) : ViewerCallback(), m_Text(A_Text)
{
	SYS::Extent::OpenCommandWnd();
	m_AngleH = m_AngleV = 0.0f;
	m_Distance = 1200.0f;

	m_Text->SetTextColor(RGB(255, 255, 0));

	SYS::Timer oTimer;
	oTimer.StartTimer();

	TCHAR szModule [MAX_PATH];
	memset(szModule, 0, MAX_PATH);
	//获取当前应用程序目录
	::GetModuleFileName(0, szModule, MAX_PATH);
	TCHAR* pPos = NULL;
	for(int i = MAX_PATH - 1; i >= 0; i--)
	{
		if(szModule[i] == '\\')
		{
			pPos = szModule + i;
			break;
		}
	}

	strcpy(pPos, "\\Mesh\\Vertex_行走.mesh");

	SYS::StreamFile oStream;
	oStream.OpenFileForRead(szModule);

	m_TriangleCount = oStream.ReadInteger();
	Integer nTriSize = m_TriangleCount * 3 * 3;
	m_ZeroVertexes = new GLfloat[nTriSize];
	oStream.ReadData(m_ZeroVertexes, sizeof(GLfloat)*nTriSize);
// 	for(Integer i = 0; i < m_TriangleCount * 3 * 3; i++)
// 		m_ZeroVertexes[i] = oStream.ReadFloat();

	Integer nTexSize = m_TriangleCount * 3 * 2;
	m_TexCoords = new GLfloat[nTexSize];
	oStream.ReadData(m_TexCoords, sizeof(GLfloat)*nTexSize);
// 	for(Integer i = 0; i < m_TriangleCount * 3 * 2; i++)
// 		m_TexCoords[i] = oStream.ReadFloat();

	m_FrameCount = oStream.ReadInteger();
	Integer nBlendSize = m_TriangleCount * 3 * 3 * m_FrameCount;
	m_BlendVertexes = new GLfloat[nBlendSize];
	oStream.ReadData(m_BlendVertexes, sizeof(GLfloat)*nBlendSize);
// 	for(Integer i = 0; i < nBlendSize; i++)
// 		m_BlendVertexes[i] = oStream.ReadFloat();

	m_Indeces = new GLfloat[m_TriangleCount * 3 * 4];
	for(Integer i = 0; i < m_TriangleCount * 3; i++)
	{
		m_Indeces[i*4] = GLfloat(i);
		m_Indeces[i*4+1] = 0.0f;
		m_Indeces[i*4+2] = 0.0f;
		m_Indeces[i*4+3] = 0.0f;
	}

	Integer nFlag = oStream.ReadInteger();
	if(nFlag != 123456)
		printf("Flag error!");

	oTimer.EndTimer();
	printf("Load time: %f", oTimer.GetElapsedTime());
}

VertexBlend::~VertexBlend()
{
	delete[] m_ZeroVertexes;
	delete[] m_TexCoords;
	delete[] m_Indeces;
	delete[] m_BlendVertexes;
}

/*---------------------------------------------------------------------------
	2015/06/05 功能: 初始化OpenGL环境
---------------------------------------------------------------------------*/
void VertexBlend::OnInitializeRendContext()
{
	ViewerCallback::OnInitializeRendContext();
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	m_ShaderProgram = gltLoadShaderPairWithAttributes(
		Helper::GetShader("VertexBlend.vs"),
		Helper::GetShader("VertexBlend.fs"),
		3,
		GLT_ATTRIBUTE_VERTEX, "vVertex",
		GLT_ATTRIBUTE_COLOR, "vIndex",
		GLT_ATTRIBUTE_TEXTURE0, "vTexCoord");

	m_mvpMatrix = glGetUniformLocation(m_ShaderProgram, "mvpMatrix");
	m_fBlend = glGetUniformLocation(m_ShaderProgram, "fBlend");
	m_TexUniform = glGetUniformLocation(m_ShaderProgram, "colorMap");
	m_BlendTexUniform = glGetUniformLocation(m_ShaderProgram, "vertexBuff");

	m_UniformStart = glGetUniformLocation(m_ShaderProgram, "nStart");
	m_UniformEnd = glGetUniformLocation(m_ShaderProgram, "nEnd");
	m_UniformVertexCount = glGetUniformLocation(m_ShaderProgram, "nVertexCount");

	m_TriangleBatch.Begin(GL_TRIANGLES, m_TriangleCount*3, 1);
	{
		m_TriangleBatch.CopyVertexData3f(m_ZeroVertexes);
		m_TriangleBatch.CopyColorData4f(m_Indeces);
		m_TriangleBatch.CopyTexCoordData2f(m_TexCoords, 0);
	}
	m_TriangleBatch.End();

	glGenTextures(1, &m_ColorTextureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ColorTextureID);
	Helper::LoadTGATexture(Helper::GetImage("Texture.tga"), GL_LINEAR, GL_LINEAR, GL_REPEAT);//GL_CLAMP_TO_BORDER

// 	m_FrameCount = 6;
// 	GLfloat vVertexBlend[] = 
// 	{
// 		-0.0f, -0.0f, -0.0f,
// 		-0.0f, +0.0f, 0.0f,
// 		+0.0f, -0.0f, 0.0f,
// 		+0.0f, +0.0f, -0.0f,
// 
// 		-0.0f, -0.0f, 0.2f,
// 		-0.0f, +0.0f, 0.2f,
// 		+0.0f, -0.0f, 0.2f,
// 		+0.0f, +0.0f, 0.2f,
// 
// 		-0.1f, -0.1f, 0.0f,
// 		-0.1f, +0.1f, 0.0f,
// 		+0.1f, -0.1f, 0.0f,
// 		+0.1f, +0.1f, 0.0f,
// 
// 		+2.0f, 0.0f, 0.0f,
// 		+2.0f, 0.0f, 0.0f,
// 		-2.0f, 0.0f, 0.0f,
// 		-2.0f, 0.0f, 0.0f,
// 
// 		0.0f, 1.0f, 0.0f,
// 		0.0f, 1.0f, 0.0f,
// 		0.0f, 1.0f, 0.0f,
// 		0.0f, 1.0f, 0.0f,
// 
// 		0.1f, -0.1f, 0.0f,
// 		-0.1f, 0.1f, 0.0f,
// 		-0.1f, 0.1f, 0.0f,
// 		0.1f, -0.1f, 0.0f
// 	};

	glGenBuffers(1, &m_VertexBufferID);
	glBindBuffer(GL_TEXTURE_BUFFER, m_VertexBufferID);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(GLfloat)*
		m_TriangleCount*3*3*m_FrameCount, m_BlendVertexes, GL_STATIC_READ);

	glGenTextures(1, &m_BlendTextureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_BUFFER, m_BlendTextureID);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, m_VertexBufferID);

	m_Timer.Init();
}

/*---------------------------------------------------------------------------
	2015/06/05 功能: 渲染场景
---------------------------------------------------------------------------*/
void VertexBlend::DoRend()
{
	Float fTime = m_Timer.GetElapsedSeconds();
	static Float fGlobalTime = 0.0f;
	fGlobalTime += fTime;

	static Float fBlendIntensity = 0.0f;
	static int nStart = 0;

	fBlendIntensity += 0.13f;
	if(fBlendIntensity > 1.0f)
	{
		fBlendIntensity -= 1.0f;
		nStart++;
	}

	glDisable(GL_LIGHTING);
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			glPushMatrix();
			{
				glLoadIdentity();
				glTranslatef(0, 0, -m_Distance);
				glRotatef(m_AngleV, 1, 0, 0);
				glRotatef(m_AngleH, 0, 0, 1);
				glTranslatef((i-5)*100, (j-5)*100, 0);
				//glRotatef(-45, 0, 0, 1);

				Integer nStartIndex = nStart + i + j;
				RendMopher(nStartIndex, nStartIndex + 1, fBlendIntensity);
			}
			glPopMatrix();
		}
	}

	Float fNewTime = m_Timer.GetElapsedSeconds();
	fGlobalTime += fNewTime;
	if(fGlobalTime > 1.0f)
	{
		fGlobalTime -= 1.0f;
		m_Text->SetText(SYS::StrTool::Format("帧率 %0.0f", 1.0f / fNewTime));
	}
	glDisable(GL_LIGHTING);
}

/*---------------------------------------------------------------------------
	2015/06/05 功能: 
---------------------------------------------------------------------------*/
void VertexBlend::RendMopher(Integer A_Start, Integer A_End, Float A_Blend)
{
	glUseProgram(m_ShaderProgram);

	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ColorTextureID);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_BlendTextureID);

	glUniform1i(m_TexUniform, 0);
	glUniform1i(m_BlendTexUniform, 1);

	glUniform1i(m_UniformStart, A_Start % m_FrameCount);
	glUniform1i(m_UniformEnd, A_End % m_FrameCount);

	glUniform1i(m_UniformVertexCount, m_TriangleCount * 3);
	glUniform1f(m_fBlend, A_Blend);

	M3DMatrix44f oModelView;
	glGetFloatv(GL_MODELVIEW_MATRIX, oModelView);
	M3DMatrix44f oProjMat;
	glGetFloatv(GL_PROJECTION_MATRIX, oProjMat);
	M3DMatrix44f oMVP;
	m3dMatrixMultiply44(oMVP, oProjMat, oModelView);
	glUniformMatrix4fv(m_mvpMatrix, 1, GL_FALSE, oMVP);

	m_TriangleBatch.Draw();

	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void VertexBlend::OnMouseWheel(In short A_Delta, In SYS::GPoint A_Point)
{
	if(A_Delta > 0)
		m_Distance *= 1.05f;
	else
		m_Distance /= 1.05f;
}

void VertexBlend::OnMouseMove(In SYS::GPoint A_Point)
{
	static SYS::GPoint oMousePt;
	if(SYS::Extent::IsKeyDown(VK_LBUTTON))
	{
		m_AngleH += A_Point.x - oMousePt.x;
		m_AngleV += A_Point.y - oMousePt.y;
	}
	oMousePt = A_Point;
}