// OpenGL.cpp : implementation file
//

#include "stdafx.h"
#include "Shadow.h"
#include "OpenGL.h"
#include "math3d.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <MATH.H>

GLint windowWidth = 800;               // window size
GLint windowHeight = 600;

GLint shadowWidth = 800;               // set based on window size
GLint shadowHeight = 600;

GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f};
GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f};

GLfloat lightPos[]     = { 200.0f, 200.0f, 0.0f, 1.0f};
M3DMatrix44f textureMatrix;

/////////////////////////////////////////////////////////////////////////////
// COpenGL

COpenGL::COpenGL()
{
	pos = 0;
	//给成员变量赋初值
	latitude=longitude=-45.0;
	trans_x=trans_y=trans_z=0;
	m_LBDown=false;
	viewDis = 300;
	m_LockLight = true;

	factor = 4.0f;
	ambientShadowAvailable = GL_FALSE;
	m_ShowMode = 0;
}

COpenGL::~COpenGL()
{
	wglMakeCurrent(NULL, NULL) ; 
	wglDeleteContext(hglrc);	//删除渲染描述表
	::ReleaseDC (m_hWnd, hdc) ; //释放设备描述表

}


BEGIN_MESSAGE_MAP(COpenGL, CWnd)
	//{{AFX_MSG_MAP(COpenGL)
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// 设置像素格式函数
int COpenGL::MySetPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),    // pfd结构的大小 
	    1,                                // 版本号 
	    PFD_DRAW_TO_WINDOW |              // 支持在窗口中绘图 
	    PFD_SUPPORT_OPENGL |              // 支持 OpenGL 
	    PFD_DOUBLEBUFFER,                 // 双缓存模式 
	    PFD_TYPE_RGBA,                    // RGBA 颜色模式 
	    24,                               // 24 位颜色深度 
	    0, 0, 0, 0, 0, 0,                 // 忽略颜色位 
	    0,                                // 没有非透明度缓存 
	    0,                                // 忽略移位位 
	    0,                                // 无累加缓存 
	    0, 0, 0, 0,                       // 忽略累加位 
	    32,                               // 32 位深度缓存     
	    0,                                // 无模板缓存 
	    0,                                // 无辅助缓存 
	    PFD_MAIN_PLANE,                   // 主层 
	    0,                                // 保留 
	    0, 0, 0                           // 忽略层,可见性和损毁掩模 
	}; 
	
	int  iPixelFormat; 
 
	// 为设备描述表得到最匹配的像素格式 
	if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
	{
		MessageBox("ChoosePixelFormat Failed", NULL, MB_OK);
		return 0;
	}
	 
	// 设置最匹配的像素格式为当前的像素格式 
	if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat Failed", NULL, MB_OK);
		return 0;
	}

	return 1;
}


/////////////////////////////////////////////////////////////////////////////
// COpenGL message handlers

int COpenGL::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// 设置当前的绘图像素格式
	MySetPixelFormat(::GetDC(m_hWnd));

	// 创建绘图描述表
	hdc = ::GetDC(m_hWnd);
	// 创建渲染描述表
	hglrc = wglCreateContext(hdc);
	// 使绘图描述表为当前调用现程的当前绘图描述表 
	wglMakeCurrent(hdc, hglrc);	

	m_Bitmap.LoadBitmapFile("Texture1.bmp");
	LoadTexture(0,NULL);
	return 0;
}
bool COpenGL::LoadTexture(int x, char *filename)
{
	glGenTextures(10,p_textures);//纹理对象,渲染时绑定用
	int map_height=m_Bitmap.map_height;
	int map_width=m_Bitmap.map_width;
	int map_size=map_height*map_width*3;

	glBindTexture(GL_TEXTURE_2D,p_textures[0]);
	
	data_map=new unsigned char[map_size];
	for(int i=0;i<map_height;i++)
	{
		for(int j=0;j<map_width;j++)
		{
			data_map[(i*map_width+j)*3+2]=m_Bitmap.bitmapImage[(i*map_width+j)*3];
			data_map[(i*map_width+j)*3+1]=m_Bitmap.bitmapImage[(i*map_width+j)*3+1];
			data_map[(i*map_width+j)*3]=m_Bitmap.bitmapImage[(i*map_width+j)*3+2];
		}
	}
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,map_width,map_height,0,GL_RGB,GL_UNSIGNED_BYTE,data_map);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, map_width, map_height,GL_RGB, GL_UNSIGNED_BYTE, data_map);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	delete[] data_map;
	return true;
}
void COpenGL::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_LBDown=false;
	CWnd::OnLButtonUp(nFlags, point);
}

void COpenGL::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_ShowMode != 2)
		m_LBDown=true;
	CWnd::OnLButtonDown(nFlags, point);
}

void COpenGL::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_LBDown)
	{
		latitude+=-(float)(point.x-m_OriginPoi.x);
		longitude+=-(float)(point.y-m_OriginPoi.y);
		if(longitude>90.0f)
			longitude=90.0f;
		if(longitude<-90.0f)
			longitude=-90.0f;
	}
	m_OriginPoi=point;
	//CDialog::OnMouseMove(nFlags, point);
	CWnd::OnMouseMove(nFlags, point);
}
// Called to regenerate the shadow map
void COpenGL::RegenerateShadowMap()
{
	// Set up some texture state that never changes
	glBindTexture(GL_TEXTURE_2D, shadowTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	GLfloat border[] = {1.0f, 1.0f, 1.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

	if (ambientShadowAvailable)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FAIL_VALUE_ARB, 0.5f);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

	GLfloat lightToSceneDistance, nearPlane, fieldOfView;
	GLfloat lightModelview[16], lightProjection[16];
	GLfloat sceneBoundingRadius = 95.0f; // based on objects in scene
	
	// Save the depth precision for where it's useful
	lightToSceneDistance = sqrt(lightPos[0] * lightPos[0] + 
		lightPos[1] * lightPos[1] + 
		lightPos[2] * lightPos[2]);
	nearPlane = lightToSceneDistance - sceneBoundingRadius;
	// Keep the scene filling the depth texture
	fieldOfView = (GLfloat)m3dRadToDeg(2.0f * atan(sceneBoundingRadius / lightToSceneDistance));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, 1.0f, nearPlane, nearPlane + (2.0f * sceneBoundingRadius));
	glGetFloatv(GL_PROJECTION_MATRIX, lightProjection);
	// Switch to light's point of view
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(lightPos[0], lightPos[1], lightPos[2], 
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightModelview);
	glViewport(0, 0, shadowWidth, shadowHeight);
	
	// Clear the depth buffer only
	glClear(GL_DEPTH_BUFFER_BIT);

	// All we care about here is resulting depth values
	glShadeModel(GL_FLAT);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_NORMALIZE);
	glColorMask(0, 0, 0, 0);
		
	// Overcome imprecision
	glEnable(GL_POLYGON_OFFSET_FILL);
	
	// Draw objects in the scene except base plane
	// which never shadows anything
	DrawModels(GL_FALSE, false);

	// Copy depth values into depth texture
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
		0, 0, shadowWidth, shadowHeight, 0);
	// Restore normal drawing state
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glColorMask(1, 1, 1, 1);
	glDisable(GL_POLYGON_OFFSET_FILL);

	// Set up texture matrix for shadow map projection,
	// which will be rolled into the eye linear
	// texture coordinate generation plane equations
	M3DMatrix44f tempMatrix;
	m3dLoadIdentity44(tempMatrix);
	m3dTranslateMatrix44(tempMatrix, 0.5f, 0.5f, 0.5f);
	m3dScaleMatrix44(tempMatrix, 0.5f, 0.5f, 0.5f);
	m3dMatrixMultiply44(textureMatrix, tempMatrix, lightProjection);
	m3dMatrixMultiply44(tempMatrix, textureMatrix, lightModelview);
	// transpose to get the s, t, r, and q rows for plane equations
	m3dTransposeMatrix44(textureMatrix, tempMatrix);
}
void COpenGL::SetupRC()
{
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
	
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	
	// Hidden surface removal
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonOffset(factor, 0.0f);
	
 	// Set up some lighting state that never changes
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	
	glGenTextures(1, &shadowTextureID);
//	glEnable(GL_CULL_FACE);
 	RegenerateShadowMap();
}
void COpenGL::DrawModels(GLboolean drawBasePlane, bool WithObjTexture)
{
	glColor3f(1.0f, 1.0f, 1.0f); // Blue
	if (drawBasePlane)
	{
		if(WithObjTexture)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, p_textures[0]);
		}
		else
			glColor3f(1.0f, 1.0f, 1.0f); // Blue

		float size = 200;
		// Draw plane that the objects rest on
		glNormal3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-size, -25.0f, -size);
		glTexCoord2f(0, 1);
		glVertex3f(-size, -25.0f, size);
		glTexCoord2f(1, 1);
		glVertex3f(size,  -25.0f, size);
		glTexCoord2f(1, 0);
		glVertex3f(size,  -25.0f, -size);
		glEnd();
		if(WithObjTexture)
			glDisable(GL_TEXTURE_2D);
	}

	// Draw red cube
	if(WithObjTexture)
	{
	}
	else
		glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glutSolidCube(48.0f);
	glPopMatrix();
	
	// Draw green sphere
	if(WithObjTexture)
	{
	}
	else
		glColor3f(0.0f, 1.0f, 0.0f);
	GLfloat zPlane[] = {0,1,0,10};
	glPushMatrix();
	glTranslatef(-60.0f, 0.0f, 0.0f);
	glutSolidSphere(25.0f, 50, 50);
	glPopMatrix();
	
	// Draw yellow cone
	if(WithObjTexture)
	{
	}
	else
		glColor3f(1.0f, 1.0f, 0.0f);
	glPushMatrix();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(60.0f, 0.0f, -24.0f);
	glutSolidCone(25.0f, 50.0f, 50, 50);
	glPopMatrix();
	
	// Draw magenta torus
	if(WithObjTexture)
	{
//		glEnable(GL_TEXTURE_2D);
//		glBindTexture(GL_TEXTURE_2D, p_textures[0]);
	}
	else
		glColor3f(1.0f, 0.0f, 1.0f);
	glPushMatrix();
	glTranslatef(0, 0.0f, 60.0f);
	glRotatef(-pos*2, 0,1,0);
	glutSolidTorus(8.0f, 16.0f, 50, 50);
	glPopMatrix();
	if(WithObjTexture)
		glDisable(GL_TEXTURE_2D);

	// Draw cyan octahedron
	if(WithObjTexture)
	{
	}
	else
		glColor3f(0.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -100.0f);
	glScalef(25.0f, 25.0f, 25.0f);
	glRotatef(-pos*2, 0,1,0);
	glutSolidOctahedron();
	glPopMatrix();

	// Draw texture teapot
	if(WithObjTexture)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, p_textures[0]);
	}
	else
		glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(0.0f, 30.0f, 0.0f);
	glScalef(-5.0f, -5.0f, -5.0f);
	glRotatef(180, 0,0,1);
	glutSolidTeapot(2);
	glPopMatrix();
	if(WithObjTexture)
		glDisable(GL_TEXTURE_2D);
}
void COpenGL::Init()
{
	SetupRC();
}

void COpenGL::Rend()
{
	if(!m_LockLight)
	{
		lightPos[0] = 200*sin(pos/100);
		lightPos[2] = 200*cos(pos/100);
	}
	pos += 1.0f;
	RegenerateShadowMap();
	GLdouble cameraZoom = 0.3;
	// Track camera angle
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (windowWidth > windowHeight)
	{
		GLdouble ar = (GLdouble)windowWidth / (GLdouble)windowHeight;
		glFrustum(-ar * cameraZoom, ar * cameraZoom, -cameraZoom, cameraZoom, 1.0, 10000.0);
	}
	else
	{
		GLdouble ar = (GLdouble)windowHeight / (GLdouble)windowWidth;
		glFrustum(-cameraZoom, cameraZoom, -ar * cameraZoom, ar * cameraZoom, 1.0, 10000.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, viewDis, 0, 0, 0, 0, 1, 0); 
 	glRotatef(-longitude,1.0,0.0,0.0);
 	glRotatef(-latitude,0.0,1.0,0.0);
 	glTranslatef(trans_x,trans_z,trans_y);

	glViewport(0, 0, windowWidth, windowHeight);
	
	// Track light position
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	bool showShadowMap = false;
	bool noShadows = false;
	if(m_ShowMode == 1)
		noShadows = true;
	else if(m_ShowMode == 2)
		showShadowMap = true;

	if (showShadowMap)
	{
		// Display shadow map for educational purposes
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		// Show the shadowMap at its actual size relative to window
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(((GLfloat)shadowWidth/(GLfloat)windowWidth)*2.0f-1.0f, 
			-1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(((GLfloat)shadowWidth/(GLfloat)windowWidth)*2.0f-1.0f, 
			((GLfloat)shadowHeight/(GLfloat)windowHeight)*2.0f-1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-1.0f, 
			((GLfloat)shadowHeight/(GLfloat)windowHeight)*2.0f-1.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		gluPerspective(45.0f, 1.0f, 1.0f, 1000.0f);
		glMatrixMode(GL_MODELVIEW);
	}
	else if (noShadows)
	{
// Set up some simple lighting
//		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
//		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
		
		// Draw objects in the scene including base plane
		DrawModels(GL_TRUE, false);
		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		glBindTexture(GL_TEXTURE_2D, p_textures[0]);
		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
		glColor4f(0.5,0.5,0.5,0.5);
		DrawModels(GL_TRUE, true);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}
	else
	{
		if (!ambientShadowAvailable)
		{
			GLfloat lowAmbient[4] = {0.2f, 0.2f, 0.2f, 1.0f};
			GLfloat lowDiffuse[4] = {0.0f, 0.0f, 0.0f, 1.0f};
			GLfloat lowSpecular[4] = {0.0f, 0.0f, 0.0f, 0.0f};
			
			// Because there is no support for an "ambient"
			// shadow compare fail value, we'll have to
			// draw an ambient pass first...
			glLightfv(GL_LIGHT0, GL_AMBIENT, lowAmbient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, lowDiffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, lowSpecular);

			glMateriali(GL_FRONT, GL_SHININESS, 128);
			glMaterialfv(GL_FRONT, GL_AMBIENT, lowAmbient);
			glMaterialfv(GL_FRONT, GL_SPECULAR, lowSpecular);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, lowDiffuse);

			// Draw objects in the scene, including base plane
			DrawModels(GL_TRUE, false);

			// Enable alpha test so that shadowed fragments are discarded
			glAlphaFunc(GL_GREATER, 0.9f);
			glEnable(GL_ALPHA_TEST);
		}
		GLfloat lowAmbient[4] = {0.2f, 0.2f, 0.2f, 1.0f};
		GLfloat lowDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		GLfloat lowSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		
		// Because there is no support for an "ambient"
		// shadow compare fail value, we'll have to
		// draw an ambient pass first...
		glLightfv(GL_LIGHT0, GL_AMBIENT, lowAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lowDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lowSpecular);

		glMateriali(GL_FRONT, GL_SHININESS, 128);
		glMaterialfv(GL_FRONT, GL_AMBIENT, lowAmbient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, lowSpecular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, lowDiffuse);

//		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
//		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

		// Set up shadow comparison
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, 
			GL_COMPARE_R_TO_TEXTURE);

//		glBindTexture(GL_TEXTURE_2D, shadowTextureID);
		// Set up the eye plane for projecting the shadow map on the scene
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glEnable(GL_TEXTURE_GEN_Q);
		glTexGenfv(GL_S, GL_EYE_PLANE, &textureMatrix[0]);
		glTexGenfv(GL_T, GL_EYE_PLANE, &textureMatrix[4]);
		glTexGenfv(GL_R, GL_EYE_PLANE, &textureMatrix[8]);
		glTexGenfv(GL_Q, GL_EYE_PLANE, &textureMatrix[12]);
		
		// Draw objects in the scene, including base plane
		DrawModels(GL_TRUE, false);
		
		glDisable(GL_ALPHA_TEST);
		
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		glDisable(GL_TEXTURE_GEN_Q);
		
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
		DrawModels(GL_TRUE, true);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}
	// Flush drawing commands
  SwapBuffers(hdc);
}
