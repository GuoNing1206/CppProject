#if !defined(AFX_VERTEXBLEND_H__0746E277_124D_47CF_9F1E_EE8C1CAEEC28__INCLUDED_)
#define AFX_VERTEXBLEND_H__0746E277_124D_47CF_9F1E_EE8C1CAEEC28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*===========================================================================

	功能:     2015/06/02

===========================================================================*/
class VertexBlend : public GLA::ViewerCallback
{
private:
	GLA::ScreenTextPtr m_Text;
	SYS::HiResTimer m_Timer;

	GLuint m_ShaderProgram;

	GLuint m_VertexBufferID;
	GLuint m_BlendTextureID;
	GLuint m_ColorTextureID;

	GLBatch m_TriangleBatch;

	GLint m_mvpMatrix;
	GLint m_fBlend;

	GLint m_BlendTexUniform;
	GLint m_TexUniform;
	GLint m_UniformStart;
	GLint m_UniformEnd;
	GLint m_UniformVertexCount;

	GLint m_FrameCount;
	GLint m_TriangleCount;
	GLfloat* m_ZeroVertexes;
	GLfloat* m_TexCoords;
	GLfloat* m_Indeces;
	GLfloat* m_BlendVertexes;

	Float m_AngleH;
	Float m_AngleV;
	Float m_Distance;

	void RendMopher(Integer A_Start, Integer A_End, Float A_Blend);

public:
	VertexBlend(GLA::ScreenTextPtr A_Text);
	virtual ~VertexBlend();

	// 初始化OpenGL环境
	virtual void OnInitializeRendContext();

	// 渲染场景
	virtual void DoRend();

	virtual void OnMouseMove(In SYS::GPoint A_Point);
	virtual void OnMouseWheel(In short A_Delta, In SYS::GPoint A_Point);

};

#endif // !defined(AFX_VERTEXBLEND_H__0746E277_124D_47CF_9F1E_EE8C1CAEEC28__INCLUDED_)