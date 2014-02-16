; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMaterialDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "scm.h"
LastPage=0

ClassCount=9
Class1=CChildFrame
Class2=CMainFrame
Class3=CMaterialDlg
Class4=CProgressDlg
Class5=CSCMApp
Class6=CAboutDlg
Class7=CSCMDoc
Class8=CSCMView

ResourceCount=6
Resource1=IDR_SCMTYPE
Resource2=IDD_MATERIAL
Resource3=IDD_ABOUTBOX
Resource4=IDR_MAINFRAME
Resource5=IDD_PROGRESSDLG
Class9=CAxesDlg
Resource6=IDD_AXES

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CMaterialDlg]
Type=0
BaseClass=CDialog
HeaderFile=MaterialDlg.h
ImplementationFile=MaterialDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_TOOLLIST

[CLS:CProgressDlg]
Type=0
BaseClass=CDialog
HeaderFile=ProgressDlg.h
ImplementationFile=ProgressDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CProgressDlg

[CLS:CSCMApp]
Type=0
BaseClass=CWinApp
HeaderFile=SCM.h
ImplementationFile=SCM.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=SCM.cpp
ImplementationFile=SCM.cpp
LastObject=ID_FABRICATE_FABRICATE

[CLS:CSCMDoc]
Type=0
BaseClass=CDocument
HeaderFile=SCMDoc.h
ImplementationFile=SCMDoc.cpp
Filter=N
VirtualFilter=DC
LastObject=CSCMDoc

[CLS:CSCMView]
Type=0
BaseClass=CView
HeaderFile=SCMView.h
ImplementationFile=SCMView.cpp
Filter=C
VirtualFilter=VWC
LastObject=ID_VIEW_PATHNORMALS

[DLG:IDD_MATERIAL]
Type=1
Class=CMaterialDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_COLOR,combobox,1342242835
Control5=IDC_TOOLLIST,combobox,1342242819
Control6=IDC_STATIC,static,1342308352
Control7=IDC_DESCRIPTION,static,1342308352
Control8=IDC_STATIC,static,1342308352

[DLG:IDD_PROGRESSDLG]
Type=1
Class=CProgressDlg
ControlCount=3
Control1=IDCANCEL,button,1342242816
Control2=IDC_BAR,msctls_progress32,1350565888
Control3=IDC_TEXT,static,1342308352

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_SCMTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_EDIT_SELECTALL
Command16=ID_CHUNK_IMPORTGEOMETRY
Command17=ID_MODEL_CHECKCOLLISIONS
Command18=ID_MODEL_MOVETOORIGIN
Command19=ID_MODEL_TRANSLATE
Command20=ID_MODEL_ROTATE
Command21=ID_MODEL_SCALE
Command22=ID_MODEL_MATERIAL
Command23=ID_MODEL_DELETE
Command24=ID_PROCESS_SLICE
Command25=ID_PROCESS_SLICECURVED
Command26=ID_PROCESS_SHOWERRORS
Command27=ID_FABRICATE_SIMULATEONLY
Command28=ID_FABRICATE_CALIBRATE
Command29=ID_FABRICATE_FABRICATE
Command30=ID_FABRICATE_PAUSE
Command31=ID_VIEW_TOOLBAR
Command32=ID_VIEW_STATUS_BAR
Command33=ID_VIEW_SHOWMODEL
Command34=ID_VIEW_SHADED
Command35=ID_VIEW_MODELNORMALS
Command36=ID_VIEW_SHOWPATHS
Command37=ID_VIEW_PATHNORMALS
Command38=ID_VIEW_NEXTPATH
Command39=ID_VIEW_PREVIOUSPATH
Command40=ID_VIEW_ALLPATHS
Command41=ID_VIEW_SHOWTOOL
Command42=ID_VIEW_ZOOMIN
Command43=ID_VIEW_ZOOMOUT
Command44=ID_WINDOW_NEW
Command45=ID_WINDOW_CASCADE
Command46=ID_WINDOW_TILE_HORZ
Command47=ID_WINDOW_ARRANGE
Command48=ID_HELP_FINDER
Command49=ID_APP_ABOUT
CommandCount=49

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_CONTEXT_HELP
CommandCount=9

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_HELP_FINDER
Command9=ID_APP_ABOUT
CommandCount=9

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_SELECTALL
Command2=ID_EDIT_COPY
Command3=ID_FILE_NEW
Command4=ID_VIEW_NEXTPATH
Command5=ID_FILE_OPEN
Command6=ID_FILE_PRINT
Command7=ID_FILE_SAVE
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_MODEL_DELETE
Command11=ID_EDIT_CUT
Command12=ID_VIEW_ZOOMOUT
Command13=ID_HELP
Command14=ID_CONTEXT_HELP
Command15=ID_NEXT_PANE
Command16=ID_PREV_PANE
Command17=ID_EDIT_COPY
Command18=ID_EDIT_PASTE
Command19=ID_VIEW_ZOOMIN
Command20=ID_EDIT_CUT
Command21=ID_EDIT_UNDO
CommandCount=21

[DLG:IDD_AXES]
Type=1
Class=CAxesDlg
ControlCount=9
Control1=IDC_XAX,edit,1350631552
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_YAX,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_ZAX,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[CLS:CAxesDlg]
Type=0
HeaderFile=AxesDlg.h
ImplementationFile=AxesDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_XAX
VirtualFilter=dWC

