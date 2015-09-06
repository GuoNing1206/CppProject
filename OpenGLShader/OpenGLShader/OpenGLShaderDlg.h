/*-----------------------------------------------------------------------------

	作者：郭宁 2015/04/28
	备注：
	审核：

-----------------------------------------------------------------------------*/
#pragma once

/*===========================================================================

	功能:  COpenGLShaderDlg 对话框   2015/04/28

===========================================================================*/
class COpenGLShaderDlg : public CDialog
{
public:
	COpenGLShaderDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OPENGLSHADER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	GLA::ViewerPtr m_GLViewer;

	void InitScreenFont();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/*===========================================================================

	功能:     2015/04/28

===========================================================================*/
class FractalDrawer : public GLA::ViewerCallback
{
	GLuint m_MyProg;
	GLBatch m_TriangleBatch;
	GLShaderManager m_ShaderMgr;
	GLTriangleBatch m_TriBatch;
	GLuint m_PanZoomVec;
	bool m_MouseDown;
	SYS::GPoint m_MousePos;
	GLfloat m_vPanZoom[4];
	SYS::GPoint2f m_ViewSize;
	GLuint m_TextureID;
	GLint m_TexUniform;

public:
	FractalDrawer();
	virtual ~FractalDrawer();

	// 初始化OpenGL环境
	virtual void OnInitializeRendContext();

	// 右键菜单回调
//	virtual bool OnContexMenu(InOut CMenu& A_Menu, CWnd* A_Wnd, CPoint A_Point);

	// 返回 false 表示该函数不处理这个消息
//	virtual bool OnMenuCommand(In Integer A_ID);

	// 渲染场景
	virtual void DoRend();
// 	virtual void DoRendPickBuffer();
// 	virtual void DoRendScreen();

	//====================  =====================//
	virtual void OnMouseButtonUp(In SYS::MouseButton A_Btn, In SYS::GPoint A_Point);
	virtual void OnMouseButtonDown(In SYS::MouseButton A_Btn, In SYS::GPoint A_Point);
// 	virtual void OnMouseDbClick(In SYS::MouseButton A_Btn, In SYS::GPoint A_Point);
 	virtual void OnMouseMove(In SYS::GPoint A_Point);
 	virtual void OnMouseWheel(In short A_Delta, In SYS::GPoint A_Point);
// 	virtual void OnKeyDown(UINT nChar);
 	virtual void OnViewerSize(In Integer A_CX, In Integer A_CY);

};

/*===========================================================================

	功能:     2015/04/29

===========================================================================*/
class ViewerCallbackPS : public GLA::ViewerCallback
{
	GLsizei	 screenWidth;			// Desired window or desktop width
	GLsizei  screenHeight;			// Desired window or desktop height

	GLboolean bFullScreen;			// Request to run full screen
	GLboolean bAnimated;			// Request for continual updates


	GLShaderManager		shaderManager;			// Shader Manager
	GLMatrixStack		modelViewMatrix;		// Modelview Matrix
	GLMatrixStack		projectionMatrix;		// Projection Matrix
	M3DMatrix44f        orthoMatrix;     
	GLFrustum			viewFrustum;			// View Frustum
	GLGeometryTransform	transformPipeline;		// Geometry Transform Pipeline
	GLFrame				cameraFrame;			// Camera frame

	GLTriangleBatch		torusBatch;
	GLBatch				floorBatch;
	GLBatch             screenQuad;

	GLuint				textures[1];
	GLuint				blurTextures[6];
	GLuint				pixBuffObjs[1];
	GLuint				curBlurTarget;
	bool				bUsePBOPath;
	GLfloat				speedFactor;
	GLuint				blurProg;
	void				*pixelData;
	GLuint				pixelDataSize;

public:
	ViewerCallbackPS();
	virtual ~ViewerCallbackPS();

	// 初始化OpenGL环境
	virtual void OnInitializeRendContext();

	// 渲染场景
	virtual void DoRend();

	void UpdateFrameCount();
	void MoveCamera(void);
	void DrawWorld(GLfloat yRot, GLfloat xPos);
	bool LoadBMPTexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	void ShutdownRC(void);
	void SetupBlurProg(void);
	void ChangeSize(int nWidth, int nHeight);

	// returns 1 - 6 for blur texture units
	// curPixBuf is always between 0 and 5
	void AdvanceBlurTaget() { curBlurTarget = ((curBlurTarget+ 1) %6); }
	GLuint GetBlurTarget0(){ return (1 + ((curBlurTarget + 5) %6)); }
	GLuint GetBlurTarget1(){ return (1 + ((curBlurTarget + 4) %6)); }
	GLuint GetBlurTarget2(){ return (1 + ((curBlurTarget + 3) %6)); }
	GLuint GetBlurTarget3(){ return (1 + ((curBlurTarget + 2) %6)); }
	GLuint GetBlurTarget4(){ return (1 + ((curBlurTarget + 1) %6)); }
	GLuint GetBlurTarget5(){ return (1 + ((curBlurTarget) %6)); }

	virtual void OnViewerSize(In Integer A_CX, In Integer A_CY);

};

/*===========================================================================

	功能:     2015/05/18

===========================================================================*/
class TextureBind : public GLA::ViewerCallback
{
	GLuint m_ShaderProgram;
	GLBatch m_TriangleBatch;
//	GLTriangleBatch m_TriangleBatch;
	GLShaderManager m_ShaderMgr;
	GLuint m_TextureID_0;
	GLuint m_TextureID_1;
	GLint m_mvpMatrix;
	GLint m_fSaturate;
	GLint m_TexUniform0;
	GLint m_TexUniform1;

	SYS::Timer m_Timer;

public:
	TextureBind();
	virtual ~TextureBind();

	// 初始化OpenGL环境
	virtual void OnInitializeRendContext();

	// 右键菜单回调
//	virtual bool OnContexMenu(InOut CMenu& A_Menu, CWnd* A_Wnd, CPoint A_Point);

// 返回 false 表示该函数不处理这个消息
//	virtual bool OnMenuCommand(In Integer A_ID);

	// 渲染场景
	virtual void DoRend();
// 	virtual void DoRendPickBuffer();
// 	virtual void DoRendScreen();

	//====================  =====================//
// 	virtual void OnMouseButtonUp(In SYS::MouseButton A_Btn, In SYS::GPoint A_Point);
// 	virtual void OnMouseButtonDown(In SYS::MouseButton A_Btn, In SYS::GPoint A_Point);
// 	virtual void OnMouseDbClick(In SYS::MouseButton A_Btn, In SYS::GPoint A_Point);
// 	virtual void OnMouseMove(In SYS::GPoint A_Point);
// 	virtual void OnMouseWheel(In short A_Delta, In SYS::GPoint A_Point);
// 	virtual void OnKeyDown(UINT nChar);
// 	virtual void OnViewerSize(In Integer A_CX, In Integer A_CY);

};

/*===========================================================================

	功能:     2015/05/18

===========================================================================*/
class PointSprite : public GLA::ViewerCallback
{
	GLuint m_ShaderProgram;
	GLuint m_TexID;
	GLBatch m_PointBatch;

public:
	PointSprite();
	virtual ~PointSprite();

	// 初始化OpenGL环境
	virtual void OnInitializeRendContext();

	// 渲染场景
	virtual void DoRend();

};

/*===========================================================================

	功能:     2015/05/25

===========================================================================*/
class RendToTexture : public GLA::ViewerCallback
{
	GLuint m_FrameBuffer;
	GLuint m_DepthBuffer;
	SYS::GSize m_TextureSize;

	GLuint m_ShaderProgram;
	GLuint m_DepthTexture;
	GLuint m_RendTexID;
	GLuint m_TextureID;
	SYS::GSize m_ViewerSize;

	void DoRendObjs(float A_Angle);

public:
	RendToTexture();
	virtual ~RendToTexture();

	// 初始化OpenGL环境
	virtual void OnInitializeRendContext();

	virtual void OnViewerSize(In Integer A_CX, In Integer A_CY);

	// 渲染场景
	virtual void DoRend();

};