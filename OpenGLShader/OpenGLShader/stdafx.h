
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


//==================== 自定义库头文件 =====================//

#include <GTypes.hpp>

//定义这个宏后 GDataStructure.hpp 单元的所有 trace 输出消息都启用
#define GDS_TRACE_ENABLE
#include <GDataStructure.hpp>

#include <GOpenGL.h>
#include <GMath/GMath.h>
#include <SysUtils/SysUtils.h>
#include <GScript/GScript.h>
#include <GFontUtility/GFontUtility.h>
#include <Geometry/Geometry.h>
#include <MassFile/MassFile.h>
#include <Compile/Compile.h>
#include <GDODataBase/GDODataBase.h>
#include <GImage/GImage.h>
#include <GLAssistant/GLAssistant.h>
#include <GDynamicsEngine/GDynamicsEngine.h>
#include <UIM/UIM.h>
#include <GMultimedia/GMultimedia.h>
#include <EmbeddedUI/EmbeddedUI.h>
#include <GHaloScene/GHaloScene.h>

#include "Helper.h"

// #include <gl\glew.h>
// #include <gl\gl.h>
// #include <gl\glu.h>
// #include <gl\glaux.h>
//#include <gl\glut.h>
//#include <gl\freeglut.h>

//#pragma comment(lib, "glew32.lib")
//#pragma comment(lib, "freeglut.lib")

// #include <gl\GLTools.h>
// #include <gl\GLShaderManager.h>

//#pragma comment(lib, "gltools.lib")

// static const char *szIdentityShaderVP = 
// 	"#version 330\r\n"
// 	"attribute vec4 vVertex;"
// 	"void main(void) "
// 	"{ gl_Position = vVertex; "
// 	"}";
// 
// static const char *szIdentityShaderFP = 
// 	"#version 330\r\n"
// #ifdef OPENGL_ES
// 	"precision mediump float;"
// #endif
// 	"uniform vec4 vColor;"
// 	"void main(void) "
// 	"{ gl_FragColor = vColor;"
// 	"	 gl_FragColor = vec4(1, 0, 0, 0.5);"
// 	"}";

// void gltLoadShaderSrc(const char *szShaderSrc, GLuint shader)
// {
// 	GLchar *fsStringPtr[1];
// 
// 	fsStringPtr[0] = (GLchar *)szShaderSrc;
// 	glShaderSource(shader, 1, (const GLchar **)fsStringPtr, NULL);
// }

// GLuint gltLoadShaderPairSrcWithAttributes(const char *szVertexSrc, const char *szFragmentSrc, ...)
// {
// 	// Temporary Shader objects
// 	GLuint hVertexShader;
// 	GLuint hFragmentShader; 
// 	GLuint hReturn = 0;   
// 	GLint testVal;
// 
// 	// Create shader objects
// 	hVertexShader = glCreateShader(GL_VERTEX_SHADER);
// 	hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
// 
// 	// Load them. 
// 	gltLoadShaderSrc(szVertexSrc, hVertexShader);
// 	gltLoadShaderSrc(szFragmentSrc, hFragmentShader);
// 
// 	// Compile them
// 	glCompileShader(hVertexShader);
// 	glCompileShader(hFragmentShader);
// 
// 	// Check for errors
// 	glGetShaderiv(hVertexShader, GL_COMPILE_STATUS, &testVal);
// 	if(testVal == GL_FALSE)
// 	{
// 		glDeleteShader(hVertexShader);
// 		glDeleteShader(hFragmentShader);
// 		return (GLuint)NULL;
// 	}
// 
// 	glGetShaderiv(hFragmentShader, GL_COMPILE_STATUS, &testVal);
// 	if(testVal == GL_FALSE)
// 	{
// 		glDeleteShader(hVertexShader);
// 		glDeleteShader(hFragmentShader);
// 		return (GLuint)NULL;
// 	}
// 
// 	// Link them - assuming it works...
// 	hReturn = glCreateProgram();
// 	glAttachShader(hReturn, hVertexShader);
// 	glAttachShader(hReturn, hFragmentShader);
// 
// 	// List of attributes
// 	va_list attributeList;
// 	va_start(attributeList, szFragmentSrc);
// 
// 	char *szNextArg;
// 	int iArgCount = va_arg(attributeList, int);	// Number of attributes
// 	for(int i = 0; i < iArgCount; i++)
// 	{
// 		int index = va_arg(attributeList, int);
// 		szNextArg = va_arg(attributeList, char*);
// 		glBindAttribLocation(hReturn, index, szNextArg);
// 	}
// 	va_end(attributeList);
// 
// 
// 	glLinkProgram(hReturn);
// 
// 	// These are no longer needed
// 	glDeleteShader(hVertexShader);
// 	glDeleteShader(hFragmentShader);  
// 
// 	// Make sure link worked too
// 	glGetProgramiv(hReturn, GL_LINK_STATUS, &testVal);
// 	if(testVal == GL_FALSE)
// 	{
// 		glDeleteProgram(hReturn);
// 		return (GLuint)NULL;
// 	}
// 
// 	return hReturn;  
// }   


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


