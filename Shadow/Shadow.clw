; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CShadowDlg
LastTemplate=generic CWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Shadow.h"

ClassCount=4
Class1=CShadowApp
Class2=CShadowDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=COpenGL
Resource3=IDD_SHADOW_DIALOG

[CLS:CShadowApp]
Type=0
HeaderFile=Shadow.h
ImplementationFile=Shadow.cpp
Filter=N

[CLS:CShadowDlg]
Type=0
HeaderFile=ShadowDlg.h
ImplementationFile=ShadowDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CShadowDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=ShadowDlg.h
ImplementationFile=ShadowDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SHADOW_DIALOG]
Type=1
Class=CShadowDlg
ControlCount=8
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_RADIO_SHOWMODE,button,1342308361
Control4=IDC_RADIO_SHOWMODE2,button,1342177289
Control5=IDC_RADIO_SHOWMODE3,button,1342177289
Control6=IDC_STATIC,button,1342178055
Control7=IDC_CHECK_LOCK_LIGHT,button,1342242819
Control8=IDC_SLIDER_LIGHT_HEIGHT,msctls_trackbar32,1342242840

[CLS:COpenGL]
Type=0
HeaderFile=OpenGL.h
ImplementationFile=OpenGL.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=COpenGL

