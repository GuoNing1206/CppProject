
// OpenGLShaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenGLShader.h"
#include "OpenGLShaderDlg.h"
#include "afxdialogex.h"
#include "VertexBlend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COpenGLShaderDlg 对话框

COpenGLShaderDlg::COpenGLShaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenGLShaderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_GLViewer = new GLA::Viewer();
}

void COpenGLShaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COpenGLShaderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// COpenGLShaderDlg 消息处理程序

BOOL COpenGLShaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CRect oRect;
	GetClientRect(oRect);
	// TODO: 在此添加额外的初始化代码
	DWORD iStyle = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE;
	m_GLViewer->Create(	NULL,										// 缺省的窗口
		NULL,										// 无窗口名称
		iStyle,									// 定义窗口风格
		oRect,									// 窗口的大小
		this,										// 指定当前对话框为其父窗口指针
		0);

	GLA::ProjectErrandPtr oProjectErrand = m_GLViewer->GetProjection()->GetProjectErrand();
	oProjectErrand->SetNear(1.0f);
	oProjectErrand->SetFar(100000.0f);
	oProjectErrand->SetWidth(1.0f);
	oProjectErrand->SetHeight(1.0f);

	InitScreenFont();

	GLA::ScreenTextDominatorPtr oDom = m_GLViewer->GetScreenTextDominator();
	GLA::ScreenTextPtr oText = oDom->AllocateText();
	oText->SetScissorSize(SYS::GSize(500, 300));
	oText->SetInsertPt(SYS::GPoint(10, 10));
	oText->SetText("");
	oText->SetTextColor(RGB(0, 0, 0));

//	m_GLViewer->BindCallback(new PointSprite());
	m_GLViewer->BindCallback(new TextureBind());
	m_GLViewer->EnableRendScreenRect(false);
	m_GLViewer->Init();
	m_GLViewer->ShowWindow(SW_SHOW);
	m_GLViewer->StartTimer();

	SetWindowText("Mandelbrot set");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COpenGLShaderDlg::InitScreenFont()
{
// 	SYS::RegistryTool oRegistry;
// 	if(!oRegistry.OpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\GN-Platform\\GLibrary"))
// 	{
// 		MessageBox("无法打开注册表[SOFTWARE\\GN-Platform\\GLibrary]");
// 		exit(0);
// 	}

	String sGLibraryPath = "D:\\Visual C++\\GLibrary\\FontFile\\";

	GLA::ScreenTextDominatorPtr oGLTextDominator = m_GLViewer->GetScreenTextDominator();

	String sPathASC = sGLibraryPath + "ASC16";
	String sPathHZK = sGLibraryPath + "HZK16";

	if(!SYS::FileTool::IsFileExist(sPathASC))
	{
		MessageBox("文件[" + sPathASC + "]不存在");
		exit(0);
	}
	if(!SYS::FileTool::IsFileExist(sPathHZK))
	{
		MessageBox("文件[" + sPathHZK + "]不存在");
		exit(0);
	}
	oGLTextDominator->InitDefaultStencil(sPathASC, GFU::ASC_16, sPathHZK, GFU::HZK_16);
}

void COpenGLShaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COpenGLShaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COpenGLShaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COpenGLShaderDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(m_GLViewer->GetSafeHwnd())
	{
		m_GLViewer->MoveWindow(0, 0, cx, cy);
	}
	// TODO: 在此处添加消息处理程序代码
}

BOOL COpenGLShaderDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}

/*===========================================================================

	功能:     2015/04/28

===========================================================================*/
FractalDrawer::FractalDrawer() : ViewerCallback(), m_MouseDown(false)
{
	m_vPanZoom[0] = 0.0f;
	m_vPanZoom[1] = 0.0f;
	m_vPanZoom[2] = 1.0f;
	m_vPanZoom[3] = 0.0f;
}

FractalDrawer::~FractalDrawer()
{}

/*---------------------------------------------------------------------------
	2015/04/28 功能: 
---------------------------------------------------------------------------*/
void FractalDrawer::OnInitializeRendContext()
{
	ViewerCallback::OnInitializeRendContext();
	m_ShaderMgr.InitializeStockShaders();

	glClearColor(0, 0, 0, 1);

	GLfloat vVerts[] = 
	{
		-1.0f, -1.0f, 0.0f,
		-1.0f, +1.0f, 0.0f,
		+1.0f, -1.0f, 0.0f,
		+1.0f, +1.0f, 0.0f
	};

	GLfloat vColors[] =
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	Float fZoom = 1.2f;
	GLfloat vTexCoord[] =
	{
		-fZoom, -fZoom,
		-fZoom, fZoom,
		fZoom, -fZoom,
		fZoom, fZoom
	};

	m_MyProg = gltLoadShaderPairWithAttributes(
		Helper::GetShader("Fractal.vs"),
		Helper::GetShader("Fractal.fs"),
		3,
		GLT_ATTRIBUTE_VERTEX, "vVertex",
		GLT_ATTRIBUTE_COLOR, "vColor",
		GLT_ATTRIBUTE_TEXTURE0, "vTexCoord");

	m_TexUniform = glGetUniformLocation(m_MyProg, "colorMap");
	m_PanZoomVec = glGetUniformLocation(m_MyProg, "vPanZoom");

	m_TriangleBatch.Begin(GL_TRIANGLE_STRIP, 4, 1);
	{
		m_TriangleBatch.CopyVertexData3f(vVerts);
		m_TriangleBatch.CopyColorData4f(vColors);
		m_TriangleBatch.CopyTexCoordData2f(vTexCoord, 0);
	}
	m_TriangleBatch.End();

	glGenTextures(1, &m_TextureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	Helper::LoadTGATexture(Helper::GetImage("Hue.tga"), GL_LINEAR, GL_LINEAR, GL_REPEAT);
}

/*---------------------------------------------------------------------------
	2015/04/28 功能: 
---------------------------------------------------------------------------*/
void FractalDrawer::DoRend()
{
	if(m_MyProg != 0)
	{
		glUseProgram(m_MyProg);

		glUniform4fv(m_PanZoomVec, 1, m_vPanZoom);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glUniform1i(m_TextureID, 0);

		m_TriangleBatch.Draw();
	}

	glUseProgram(0);
}

void FractalDrawer::OnMouseButtonUp(In SYS::MouseButton A_Btn, In SYS::GPoint A_Point)
{
	m_MouseDown = false;
}

void FractalDrawer::OnMouseButtonDown(In SYS::MouseButton A_Btn, In SYS::GPoint A_Point)
{
	m_MouseDown = true;
	m_MousePos = A_Point;
}

void FractalDrawer::OnMouseMove(In SYS::GPoint A_Point)
{
	if(m_MouseDown)
	{
		m_vPanZoom[0] += -(A_Point.x - m_MousePos.x) / m_ViewSize.x * m_vPanZoom[2] * 2;
		m_vPanZoom[1] += (A_Point.y - m_MousePos.y) / m_ViewSize.y * m_vPanZoom[2] * 2;
		m_MousePos = A_Point;
	}
}

void FractalDrawer::OnMouseWheel(In short A_Delta, In SYS::GPoint A_Point)
{
	if(A_Delta < 0)
		m_vPanZoom[2] *= 1.2f;
	else
		m_vPanZoom[2] /= 1.2f;
}

void FractalDrawer::OnViewerSize(In Integer A_CX, In Integer A_CY)
{
	m_ViewSize.SetValue(A_CX, A_CY);
}

/*===========================================================================

	功能:     2015/05/18

===========================================================================*/
TextureBind::TextureBind() : ViewerCallback()
{}

TextureBind::~TextureBind()
{}

/*---------------------------------------------------------------------------
	2015/05/18 功能: 初始化OpenGL环境
---------------------------------------------------------------------------*/
void TextureBind::OnInitializeRendContext()
{
	SYS::Extent::OpenCommandWnd();

	ViewerCallback::OnInitializeRendContext();
	m_ShaderMgr.InitializeStockShaders();

	glClearColor(0, 0, 0, 1);

	GLfloat vVerts[] = 
	{
		-1.0f, -1.0f, 0.0f,
		-1.0f, +1.0f, 0.0f,
		+1.0f, -1.0f, 0.0f,
		+1.0f, +1.0f, 0.0f
	};

	int nCount = 1000000;

	GLfloat* pBuff = new GLfloat[nCount*2*3];
	for(int i = 0; i < nCount; i++)
	{
		GLfloat* pt1 = pBuff + i * 6;
		GLfloat* pt2 = pBuff + i * 6 + 3;

		pt1[0] = i * 2.0f / nCount - 1.0f;
		pt1[1] = -1.0f;
		pt1[2] = 0.0f;

		pt2[0] = i * 2.0f / nCount - 1.0f;
		pt2[1] = 1.0f;
		pt2[2] = 0.0f;
	}

	GLfloat* pTexBuff = new GLfloat[nCount*2*2];
	for(int i = 0; i < nCount; i++)
	{
		GLfloat* pt1 = pTexBuff + i * 4;
		GLfloat* pt2 = pTexBuff + i * 4 + 2;

		pt1[0] = (GLfloat)i / nCount;
		pt1[1] = 0.0f;

		pt2[0] = (GLfloat)i / nCount;
		pt2[1] = 1.0f;
	}

// 	GLfloat vVerts[] = 
// 	{
// 		-1.0f, -0.2f, -1.0f,
// 		-1.0f, +0.2f, 1.0f,
// 		+1.0f, -0.2f, -1.0f,
// 		+1.0f, +0.2f, 1.0f
// 	};

	Float fZoom = 1.0f;
	GLfloat vTexCoord0[] =
	{
		0, 0,
		0, fZoom,
		fZoom, 0,
		fZoom, fZoom
	};

	fZoom = 1;
	GLfloat vTexCoord1[] =
	{
		0, 0,
		0, fZoom,
		fZoom, 0,
		fZoom, fZoom
	};

	m_ShaderProgram = gltLoadShaderPairWithAttributes(
		Helper::GetShader("TextureBind.vs"), 
		Helper::GetShader("TextureBind.fs"), 
		3,
		GLT_ATTRIBUTE_VERTEX, "vVertex",
		GLT_ATTRIBUTE_TEXTURE0, "vTexCoord0",
		GLT_ATTRIBUTE_TEXTURE1, "vTexCoord1");

	m_mvpMatrix = glGetUniformLocation(m_ShaderProgram, "mvpMatrix");
	m_fSaturate = glGetUniformLocation(m_ShaderProgram, "fSaturate");
	m_TexUniform0 = glGetUniformLocation(m_ShaderProgram, "colorMap0");
	m_TexUniform1 = glGetUniformLocation(m_ShaderProgram, "colorMap1");

	m_TriangleBatch.Begin(GL_TRIANGLE_STRIP, nCount * 2, 2);
	{
		m_TriangleBatch.CopyVertexData3f(pBuff);
		m_TriangleBatch.CopyTexCoordData2f(pTexBuff, 0);
		m_TriangleBatch.CopyTexCoordData2f(pTexBuff, 1);
	}
	m_TriangleBatch.End();

	delete[] pBuff;
	delete[] pTexBuff;

//	gltMakeTorus(m_TriangleBatch, 1, 0.5, 32, 32);

	glGenTextures(1, &m_TextureID_0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID_0);
	Helper::LoadTGATexture(Helper::GetImage("Texture01.tga"), GL_LINEAR, GL_LINEAR, GL_REPEAT);//GL_CLAMP_TO_BORDER

	GLfloat border[] = {1.0f, 1.0f, 0.0f, 0.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border); 

	glGenTextures(1, &m_TextureID_1);
	glBindTexture(GL_TEXTURE_2D, m_TextureID_1);
	Helper::LoadTGATexture(Helper::GetImage("Texture02.tga"), GL_LINEAR, GL_LINEAR, GL_REPEAT);
}

/*---------------------------------------------------------------------------
	2015/05/18 功能: 
---------------------------------------------------------------------------*/
void TextureBind::DoRend()
{
	static int i = 0;
	i++;

	static GLfloat fSaturate = 0.0f;
	fSaturate += 0.01f;

	static M3DMatrix44f oMVP;
	if(i % 60 == 0)
	{
		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(m_ShaderProgram);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_TextureID_0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_TextureID_1);

		m_Timer.StartTimer();

		M3DMatrix44f oModelView;
		M3DMatrix44f oProjMat;
		m3dMakeOrthographicMatrix(oProjMat, -1, 1, -1, 1, -1, 1);
		m3dLoadIdentity44(oModelView);
		m3dMatrixMultiply44(oMVP, oProjMat, oProjMat);

		m_Timer.EndTimer();
		printf(m_Timer.FormatElapsedTime("矩阵计算: %f\n"));

		glUniformMatrix4fv(m_mvpMatrix, 1, GL_FALSE, oMVP);
		glUniform1i(m_TexUniform0, 0);
		glUniform1i(m_TexUniform1, 1);
	}

// 		m_Timer.StartTimer();
// 				glGetFloatv(GL_MODELVIEW_MATRIX, oModelView);
// 				glGetFloatv(GL_PROJECTION_MATRIX, oProjMat);
//		m_Timer.EndTimer();

	glUniform1f(m_fSaturate, MTH::Math::Sin(fSaturate)*MTH::Math::Sin(fSaturate));
	
	if(i % 60 == 0)
	{
		m_Timer.StartTimer();
		m_TriangleBatch.Draw();
		m_Timer.EndTimer();
		printf(m_Timer.FormatElapsedTime("0 三角渲染: %f\n"));

		m_Timer.StartTimer();
		m_TriangleBatch.Draw();
		m_Timer.EndTimer();
		printf(m_Timer.FormatElapsedTime("1 三角渲染: %f\n"));

		m_Timer.StartTimer();
		m_TriangleBatch.Draw();
		m_Timer.EndTimer();
		printf(m_Timer.FormatElapsedTime("2 三角渲染: %f\n"));
 	}
 	else
	{
		m_TriangleBatch.Draw();
	}
}

// 			m_Timer.StartTimer();
// 			m_TriangleBatch.Draw();
// 			m_Timer.EndTimer();
// 			Float fTime1 = m_Timer.GetElapsedTime();
// 			printf(m_Timer.FormatElapsedTime("Time0: %f\n"));
// 
// 			const int nCount = 10;
// 			Float aTimer[nCount];
// 			Float fAverage = 0.0f;
// 			for(Integer i = 0; i < nCount; i++)
// 			{
// 				m_Timer.StartTimer();
// 				m_TriangleBatch.Draw();
// 				m_Timer.EndTimer();
// 				aTimer[i] = m_Timer.GetElapsedTime();
// 				fAverage += aTimer[i];
// 				printf("Time%d: %f\n", i+1, aTimer[i]);
// 			}
// 			fAverage /= (float)nCount;
// 
// 			printf("第一次比例: %f\n", fTime1 / fAverage);

/*===========================================================================

	功能:     2015/05/18

===========================================================================*/
PointSprite::PointSprite() : ViewerCallback()
{}

PointSprite::~PointSprite()
{}

/*---------------------------------------------------------------------------
	2015/05/18 功能: 初始化OpenGL环境
---------------------------------------------------------------------------*/
void PointSprite::OnInitializeRendContext()
{
	ViewerCallback::OnInitializeRendContext();

	m_ShaderProgram = gltLoadShaderPairWithAttributes(
		Helper::GetShader("PointSprite.vs"), 
		Helper::GetShader("PointSprite.fs"), 
		1,
		GLT_ATTRIBUTE_VERTEX, "vVertex");

	GLfloat fSize = 0.5f;
	GLfloat vVerts[] = 
	{
		-fSize, -fSize, 0.0f,
		-fSize, +fSize, 0.0f,
		+fSize, -fSize, 0.0f,
		+fSize, +fSize, 0.0f
	};
	m_PointBatch.Begin(GL_POINTS, 4);
		m_PointBatch.CopyVertexData3f(vVerts);
	m_PointBatch.End();

	glClearColor(0.5, 0.5, 0.5, 1);

	glGenTextures(1, &m_TexID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TexID);
	Helper::LoadTGATexture(Helper::GetImage("Texture01.tga"), GL_LINEAR, GL_LINEAR, GL_REPEAT);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
}

/*---------------------------------------------------------------------------
	2015/05/18 功能: 
---------------------------------------------------------------------------*/
void PointSprite::DoRend()
{
	glEnable(GL_PROGRAM_POINT_SIZE);
 	glUseProgram(m_ShaderProgram);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
 	glBindTexture(GL_TEXTURE_2D, m_TexID);
	GLint iTexUniform = glGetUniformLocation(m_ShaderProgram, "colorTex");
	glUniform1i(iTexUniform, 0);
	glPointSize(128);
	m_PointBatch.Draw();
	glUseProgram(0);

// 	glPushMatrix();
// 		gluLookAt(10, 0, 0, 0, 0, 0, 0, 0, 1);
// 
// 		glColor3ub(255, 255, 255);
// 		glDisable(GL_LIGHTING);
// 		glPointSize(256);
// 		glEnable(GL_TEXTURE_2D);  
// 		glEnable(GL_POINT_SPRITE);  
// 		glEnable(GL_ALPHA_TEST);  
// 		glBegin(GL_POINTS);  
// 			glVertex3d(0 , 0, 0);  
// 			glVertex3d(0 , 2, 0);  
// 			glVertex3d(0 , -2, 0);  
// 		glEnd();  
// 		glDisable(GL_ALPHA_TEST);  
// 		glDisable(GL_POINT_SPRITE);  
// 		glDisable(GL_TEXTURE_2D);  
// 
// 	glPopMatrix();
}

/*===========================================================================

	功能:     2015/05/25

===========================================================================*/
RendToTexture::RendToTexture() : ViewerCallback()
{}

RendToTexture::~RendToTexture()
{}

// 初始化OpenGL环境
void RendToTexture::OnInitializeRendContext()
{
	ViewerCallback::OnInitializeRendContext();

	m_TextureSize.SetPoint(512, 512);

	glGenFramebuffers(1, &m_FrameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FrameBuffer);

	glGenTextures(1, &m_RendTexID) ;  
	glBindTexture(GL_TEXTURE_2D, m_RendTexID) ;  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);   
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_TextureSize.x, m_TextureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);  
//	glGenerateMipmap(GL_TEXTURE_2D); 
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RendTexID, 0) ;  

	glGenRenderbuffers(1, &m_DepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_TextureSize.x, m_TextureSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	//====================  =====================//
	glGenTextures(1, &m_DepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

// 	glGenTextures(1, &m_TexID);
// 	glBindTexture(GL_TEXTURE_2D, m_TexID);
// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 800, 800, 0, GL_RGBA, GL_FLOAT, NULL);
// 
// 	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexID, 0);
// 	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);

	glGenTextures(1, &m_TextureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	Helper::LoadTGATexture(Helper::GetImage("Texture01.tga"), GL_LINEAR, GL_LINEAR, GL_REPEAT);
}

/*---------------------------------------------------------------------------
	2015/05/30 功能: 
---------------------------------------------------------------------------*/
void RendToTexture::DoRendObjs(float A_Angle)
{
	GLfloat clr[] = {1, 1, 1, 1};

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	//====================  =====================//

	// 	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBuffer);
	// 	glReadBuffer(GL_DEPTH_ATTACHMENT);
	// 	glBlitFramebuffer(0, 0, m_TextureSize.x, m_TextureSize.y, 
	// 		0, 0, m_ViewerSize.x, m_ViewerSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	//====================  =====================//

	glPushMatrix();
	{
		glRotatef(30, 1, 0, 0);
		glRotatef(A_Angle, 0, 1, 0);
		//		glRotatef(45, 1, 0, 0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		//		glutSolidTeapot(0.7);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, clr);
		glBindTexture(GL_TEXTURE_2D, m_RendTexID);

		// 		glEnable(GL_BLEND);
		// 		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// 		glEnable(GL_ALPHA_TEST);
		// 		glAlphaFunc(GL_GREATER, 0.5f);

		glPushMatrix();
		{
			glRotatef(0, 0, 1, 0);
			glBegin(GL_QUADS);
			{
				glNormal3f(0,0, 1);
				glTexCoord2f(0, 0);
				glVertex3f(-1, -1, 1);

				glTexCoord2f(1, 0);
				glVertex3f(1, -1, 1);

				glTexCoord2f(1, 1);
				glVertex3f(1, 1, 1);

				glTexCoord2f(0, 1);
				glVertex3f(-1, 1, 1);
			}
			glEnd();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glRotatef(90, 0, 1, 0);
			glBegin(GL_QUADS);
			{
				glNormal3f(0,0, 1);
				glTexCoord2f(0, 0);
				glVertex3f(-1, -1, 1);

				glTexCoord2f(1, 0);
				glVertex3f(1, -1, 1);

				glTexCoord2f(1, 1);
				glVertex3f(1, 1, 1);

				glTexCoord2f(0, 1);
				glVertex3f(-1, 1, 1);
			}
			glEnd();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glRotatef(180, 0, 1, 0);
			glBegin(GL_QUADS);
			{
				glNormal3f(0,0, 1);
				glTexCoord2f(0, 0);
				glVertex3f(-1, -1, 1);

				glTexCoord2f(1, 0);
				glVertex3f(1, -1, 1);

				glTexCoord2f(1, 1);
				glVertex3f(1, 1, 1);

				glTexCoord2f(0, 1);
				glVertex3f(-1, 1, 1);
			}
			glEnd();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glRotatef(270, 0, 1, 0);
			glBegin(GL_QUADS);
			{
				glNormal3f(0,0, 1);
				glTexCoord2f(0, 0);
				glVertex3f(-1, -1, 1);

				glTexCoord2f(1, 0);
				glVertex3f(1, -1, 1);

				glTexCoord2f(1, 1);
				glVertex3f(1, 1, 1);

				glTexCoord2f(0, 1);
				glVertex3f(-1, 1, 1);
			}
			glEnd();
		}
		glPopMatrix();

		glDisable(GL_ALPHA_TEST);
		glTranslatef(0, 1, 0);
		glutSolidTeapot(0.5f);
	}
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
	glBegin(GL_QUADS);
	{
		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 0);
		glVertex3f(-2, -2, 0);

		glTexCoord2f(1, 0);
		glVertex3f(-1, -2, 0);

		glTexCoord2f(1, 1);
		glVertex3f(-1, -1, 0);

		glTexCoord2f(0, 1);
		glVertex3f(-2, -1, 0);
	}
	glEnd();
	glEnable(GL_DEPTH_TEST);
}

/*---------------------------------------------------------------------------
	2015/05/27 功能: 渲染场景
---------------------------------------------------------------------------*/
void RendToTexture::DoRend()
{
	GLenum windowBuffer[] = {GL_BACK_LEFT} ;  
	GLenum fboBuffers[] = {GL_COLOR_ATTACHMENT0} ;  

	static int nCount = 0;
	nCount ++;

	static float aAngle = 0.0f;
	aAngle += 0.1f;

	GLfloat clr[] = {1, 1, 1, 1};
	GLfloat clrDiffuse[] = {0.5, 0.5, 0.5, 1};
	if(true || nCount % 2 == 0)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
		glColor3ub(255, 255, 255);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FrameBuffer) ;  
		glDrawBuffers(1, fboBuffers);
		glClearColor(0, 0, 0.25, 0.0f);  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
		glViewport(0, 0, m_TextureSize.x, m_TextureSize.y);
		glMatrixMode(GL_PROJECTION);
		{
			glLoadIdentity();
			glOrtho(-4, 4, -4, 4, -5, 5);
//			glFrustum(-1.5, 1.5, -1.5, 1.5, 3, 10);
		}
		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
//		gluLookAt(4, 4, -4, 0, 0, 0, 0, 0, 1);

		GLfloat pos[] = {10, 10, 10, 1.0f};
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		glMaterialfv(GL_FRONT, GL_SPECULAR, clr);
		glLightfv(GL_LIGHT0, GL_SPECULAR, clr);
		glMateriali(GL_FRONT, GL_SHININESS, 100);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, clrDiffuse);
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

// 		glEnable(GL_TEXTURE_GEN_S);
// 		glEnable(GL_TEXTURE_GEN_T);

		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

		glEnable(GL_DEPTH_TEST);
		glPushMatrix();
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_TextureID);
			glRotatef(aAngle*20, 0, 1, 0);
			glRotatef(aAngle*20, 1, 0, 0);
			glEnable(GL_LIGHTING);
			glDisable(GL_BLEND);
			glutSolidTeapot(2);
		}
		glPopMatrix();

//		DoRendObjs(aAngle);

		//====================  =====================//

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBuffer);
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
			0, 0, m_TextureSize.x, m_TextureSize.y, 0);

		//====================  =====================//

		glBindTexture(GL_TEXTURE_2D, m_RendTexID);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffers(1, windowBuffer);

	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, m_ViewerSize.x, m_ViewerSize.y);
	glMatrixMode(GL_PROJECTION);
	{
		glLoadIdentity();
//		glOrtho(-1.2, 1.2, -1.2, 1.2, -10, 10);
		glFrustum(-0.3, 0.3, -0.3, 0.3, 1.2, 10);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 8, 0, 0, 0, 0, 1, 0);

	DoRendObjs(aAngle);

	//====================  =====================//

// 	glPushMatrix();
// 	{
// 		glEnable(GL_TEXTURE_2D);
// 		glBindTexture(GL_TEXTURE_2D, m_TextureID);
// 		glTranslatef(0, 0, 0.5);
// 		glRotatef(aAngle, 0, 1, 0);
// 		glRotatef(45, 1, 0, 0);
// 		glEnable(GL_LIGHTING);
// 		glDisable(GL_BLEND);
// 		glutSolidTeapot(0.5);
// 	}
// 	glPopMatrix();
}

void RendToTexture::OnViewerSize(In Integer A_CX, In Integer A_CY)
{
	m_ViewerSize.SetPoint(A_CX, A_CY);
}

static GLfloat vGreen[] = { 0.0f, 1.0f, 0.0f, 1.0f };
static GLfloat vWhite[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat vLightPos[] = { 0.0f, 3.0f, 0.0f, 1.0f };

/*===========================================================================

	功能:     2015/04/29

===========================================================================*/
ViewerCallbackPS::ViewerCallbackPS() : ViewerCallback(), pixelData(NULL)
{}

ViewerCallbackPS::~ViewerCallbackPS()
{}

void ViewerCallbackPS::OnViewerSize(In Integer A_CX, In Integer A_CY)
{
	ChangeSize(A_CX, A_CY);
}

void ViewerCallbackPS::ChangeSize(int nWidth, int nHeight)
{
	glViewport(0, 0, nWidth, nHeight);
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
	viewFrustum.SetPerspective(35.0f, float(nWidth)/float(nHeight), 1.0f, 100.0f);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
	modelViewMatrix.LoadIdentity();

	// update screen sizes
	screenWidth = nWidth;
	screenHeight = nHeight;

	// reset screen aligned quad
	gltGenerateOrtho2DMat(screenWidth, screenHeight, orthoMatrix, screenQuad);

	free(pixelData);
	pixelDataSize = screenWidth*screenHeight*3*sizeof(unsigned int);
	pixelData = (void*)malloc(pixelDataSize);

	//  Resize PBOs
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pixBuffObjs[0]);
	glBufferData(GL_PIXEL_PACK_BUFFER, pixelDataSize, pixelData, GL_DYNAMIC_COPY);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	gltCheckErrors();
}

/*---------------------------------------------------------------------------
	2015/04/28 功能: 
---------------------------------------------------------------------------*/
void ViewerCallbackPS::OnInitializeRendContext()
{
	ViewerCallback::OnInitializeRendContext();

	ChangeSize(512, 512);

	screenWidth  = 800;
	screenHeight = 600;
	bFullScreen = false; 
	bAnimated   = true;
	bUsePBOPath = false;
	blurProg    = 0;
	speedFactor = 1.0f;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	// Initialze Shader Manager
	shaderManager.InitializeStockShaders();
	glEnable(GL_DEPTH_TEST);

	// Black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	gltMakeTorus(torusBatch, 0.4f, 0.15f, 35, 35);

	GLfloat alpha = 0.25f;
	floorBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	floorBatch.Color4f(0.0f, 1.0f, 0.0f, alpha);
	floorBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	floorBatch.Normal3f(0.0, 1.0f, 0.0f);
	floorBatch.Vertex3f(-20.0f, -0.41f, 20.0f);

	floorBatch.Color4f(0.0f, 1.0f, 0.0f, alpha);
	floorBatch.MultiTexCoord2f(0, 10.0f, 0.0f);
	floorBatch.Normal3f(0.0, 1.0f, 0.0f);
	floorBatch.Vertex3f(20.0f, -0.41f, 20.0f);

	floorBatch.Color4f(0.0f, 1.0f, 0.0f, alpha);
	floorBatch.MultiTexCoord2f(0, 10.0f, 10.0f);
	floorBatch.Normal3f(0.0, 1.0f, 0.0f);
	floorBatch.Vertex3f(20.0f, -0.41f, -20.0f);

	floorBatch.Color4f(0.0f, 1.0f, 0.0f, alpha);
	floorBatch.MultiTexCoord2f(0, 0.0f, 10.0f);
	floorBatch.Normal3f(0.0, 1.0f, 0.0f);
	floorBatch.Vertex3f(-20.0f, -0.41f, -20.0f);
	floorBatch.End();

	glGenTextures(1, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	LoadBMPTexture("marble.bmp", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);

	// Create blur program
	blurProg =  gltLoadShaderPairWithAttributes("blur.vs", "blur.fs", 2,
		GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_TEXTURE0, "texCoord0");

	// Create blur textures
	glGenTextures(6, blurTextures);

	// XXX I don't think this is necessary. Should set texture data to NULL
	// Allocate a pixel buffer to initialize textures and PBOs
	pixelDataSize = screenWidth*screenHeight*3*sizeof(unsigned int); // XXX This should be unsigned byte
	void* data = (void*)malloc(pixelDataSize);
	memset(data, 0x00, pixelDataSize);

	// Setup 6 texture units for blur effect
	// Initialize texture data
	for (int i=0; i<6;i++)
	{
		glActiveTexture(GL_TEXTURE1+i);
		glBindTexture(GL_TEXTURE_2D, blurTextures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	// Alloc space for copying pixels so we dont call malloc on every draw
	glGenBuffers(1, pixBuffObjs);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pixBuffObjs[0]);
	glBufferData(GL_PIXEL_PACK_BUFFER, pixelDataSize, pixelData, GL_DYNAMIC_COPY);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	// Create geometry and a matrix for screen aligned drawing
	gltGenerateOrtho2DMat(screenWidth, screenHeight, orthoMatrix, screenQuad);

	// Make sure all went well
	gltCheckErrors();
}

/*---------------------------------------------------------------------------
	2015/04/28 功能: 
---------------------------------------------------------------------------*/
void ViewerCallbackPS::DoRend()
{
	static CStopWatch animationTimer;
	static float totalTime = 6; // To go back and forth
	static float halfTotalTime = totalTime/2;
	float seconds = animationTimer.GetElapsedSeconds() * speedFactor;
	float xPos = 0;

	// Calculate the next postion of the moving object
	// First perform a mod-like operation on the time as a float
	while(seconds > totalTime)
		seconds -= totalTime;

	// Move object position, if it's gone half way across
	// start bringing it back
	if(seconds < halfTotalTime)
		xPos = seconds -halfTotalTime*0.5f;
	else
		xPos = totalTime - seconds -halfTotalTime*0.5f;

	// First draw world to screen
	modelViewMatrix.PushMatrix();	
	M3DMatrix44f mCamera;
	cameraFrame.GetCameraMatrix(mCamera);
	modelViewMatrix.MultMatrix(mCamera);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]); // Marble
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_MODULATE, transformPipeline.GetModelViewProjectionMatrix(), vWhite, 0);

	floorBatch.Draw();
	DrawWorld(0.0f, xPos);
	modelViewMatrix.PopMatrix();

	if(bUsePBOPath)
	{
		// First bind the PBO as the pack buffer, then read the pixels directly to the PBO
		glBindBuffer(GL_PIXEL_PACK_BUFFER, pixBuffObjs[0]);
		glReadPixels(0, 0, screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

		// Next bind the PBO as the unpack buffer, then push the pixels straight into the texture
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixBuffObjs[0]);

		// Setup texture unit for new blur, this gets imcremented every frame 
		glActiveTexture(GL_TEXTURE0+GetBlurTarget0() ); 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	}
	else
	{
		// Grab the screen pixels and copy into local memory
		glReadPixels(0, 0, screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, pixelData);

		// Push pixels from client memory into texture
		// Setup texture unit for new blur, this gets imcremented every frame
		glActiveTexture(GL_TEXTURE0+GetBlurTarget0() );
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
	}

	// Draw full screen quad with blur shader and all blur textures
	projectionMatrix.PushMatrix(); 
	projectionMatrix.LoadIdentity();
	projectionMatrix.LoadMatrix(orthoMatrix);
	modelViewMatrix.PushMatrix();	
	modelViewMatrix.LoadIdentity();
	glDisable(GL_DEPTH_TEST); 
	SetupBlurProg();
	screenQuad.Draw();
	glEnable(GL_DEPTH_TEST); 
	modelViewMatrix.PopMatrix(); 
	projectionMatrix.PopMatrix();

	// Move to the next blur texture for the next frame
	AdvanceBlurTaget();

	// Do the buffer Swap
	glutSwapBuffers();

	// Do it again
	glutPostRedisplay();

	UpdateFrameCount();
}

void ViewerCallbackPS::SetupBlurProg(void)
{
	// Set the blur program as the current one
	glUseProgram(blurProg);

	// Set MVP matrix
	glUniformMatrix4fv(glGetUniformLocation(blurProg, "mvpMatrix"), 1, GL_FALSE, transformPipeline.GetModelViewProjectionMatrix());

	// Setup the textue units for the blur targets, these rotate every frame
	glUniform1i(glGetUniformLocation(blurProg, "textureUnit0"), GetBlurTarget0());
	glUniform1i(glGetUniformLocation(blurProg, "textureUnit1"), GetBlurTarget1());
	glUniform1i(glGetUniformLocation(blurProg, "textureUnit2"), GetBlurTarget2());
	glUniform1i(glGetUniformLocation(blurProg, "textureUnit3"), GetBlurTarget3());
	glUniform1i(glGetUniformLocation(blurProg, "textureUnit4"), GetBlurTarget4());
	glUniform1i(glGetUniformLocation(blurProg, "textureUnit5"), GetBlurTarget5());
}

void ViewerCallbackPS::ShutdownRC(void)
{
	// Make sure default FBO is bound
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	// Cleanup textures
	for (int i=0; i<7;i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Now delete detached textures
	glDeleteTextures(1, textures);
	glDeleteTextures(6, blurTextures);

	// delete PBO
	glDeleteBuffers(1, pixBuffObjs);
}

bool ViewerCallbackPS::LoadBMPTexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)	
{
	GLbyte *pBits;
	GLint iWidth, iHeight;

	pBits = gltReadBMPBits(szFileName, &iWidth, &iHeight);
	if(pBits == NULL)
		return false;

	// Set Wrap modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	// Do I need to generate mipmaps?
	if(minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST || minFilter == GL_NEAREST_MIPMAP_LINEAR || minFilter == GL_NEAREST_MIPMAP_NEAREST)
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, pBits);
	return true;
}

void ViewerCallbackPS::UpdateFrameCount()
{
	static int iFrames = 0;           // Frame count
	static CStopWatch frameTimer;     // Render time

	// Reset the stopwatch on first time
	if(iFrames == 0)
	{
		frameTimer.Reset();
		iFrames++;
	}
	// Increment the frame count
	iFrames++;

	// Do periodic frame rate calculation
	if (iFrames == 101)
	{
		float fps;

		fps = 100.0f / frameTimer.GetElapsedSeconds();
		if (bUsePBOPath)
			printf("Pix_buffs - Using PBOs  %.1f fps\n", fps);
		else
			printf("Pix_buffs - Using Client mem copies %.1f fps\n", fps);

		frameTimer.Reset();
		iFrames = 1;
	}
}

void ViewerCallbackPS::DrawWorld(GLfloat yRot, GLfloat xPos)
{
	M3DMatrix44f mCamera;
	modelViewMatrix.GetMatrix(mCamera);

	// Need light position relative to the Camera
	M3DVector4f vLightTransformed;
	m3dTransformVector4(vLightTransformed, vLightPos, mCamera);

	// Draw stuff relative to the camera
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Translate(0.0f, 0.2f, -2.5f);
	modelViewMatrix.Translate(xPos, 0.0f, 0.0f);
	modelViewMatrix.Rotate(yRot, 0.0f, 1.0f, 0.0f);

	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, 
		modelViewMatrix.GetMatrix(), 
		transformPipeline.GetProjectionMatrix(), 
		vLightTransformed, vGreen, 0);
	torusBatch.Draw();
	modelViewMatrix.PopMatrix();
}
