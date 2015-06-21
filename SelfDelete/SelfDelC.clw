; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSelfDelCDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SelfDelC.h"

ClassCount=4
Class1=CSelfDelCApp
Class2=CSelfDelCDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SELFDELC_DIALOG

[CLS:CSelfDelCApp]
Type=0
HeaderFile=SelfDelC.h
ImplementationFile=SelfDelC.cpp
Filter=N

[CLS:CSelfDelCDlg]
Type=0
HeaderFile=SelfDelCDlg.h
ImplementationFile=SelfDelCDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=SelfDelCDlg.h
ImplementationFile=SelfDelCDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SELFDELC_DIALOG]
Type=1
Class=CSelfDelCDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_BTN_SELFDEL,button,1342242816

