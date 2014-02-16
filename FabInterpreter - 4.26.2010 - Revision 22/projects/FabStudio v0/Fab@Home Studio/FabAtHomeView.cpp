/*License Notification
Fab@Home operates under the BSD Open Source License

Copyright (c) 2006, Hod Lipson and Evan Malone (evan.malone@cornell.edu) All rights reserved. 

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met: 

Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer. 
Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation and/or 
other materials provided with the distribution. 
Neither the name of the Fab@Home Project nor the names of its contributors may be 
used to endorse or promote products derived from this software without specific 
prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// FabAtHomeView.cpp : implementation of the CFabAtHomeView class
//

#include "stdafx.h"
#include ".\fabathomeview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeView

IMPLEMENT_DYNCREATE(CFabAtHomeView, CView)

BEGIN_MESSAGE_MAP(CFabAtHomeView, CView)
	//{{AFX_MSG_MAP(CFabAtHomeView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_VIEW_SHADED, OnViewShaded)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHADED, OnUpdateViewShaded)
	ON_COMMAND(ID_VIEW_SHOWPATHS, OnViewShowpaths)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWPATHS, OnUpdateViewShowpaths)
	ON_COMMAND(ID_VIEW_SHOWMODEL, OnViewShowmodel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWMODEL, OnUpdateViewShowmodel)
	ON_COMMAND(ID_VIEW_NEXTPATH, OnViewNextpath)
	ON_COMMAND(ID_VIEW_PREVIOUSPATH, OnViewPreviouspath)
	ON_COMMAND(ID_VIEW_ALLPATHS, OnViewAllpaths)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NEXTPATH, OnUpdateViewNextpath)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ALLPATHS, OnUpdateViewAllpaths)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREVIOUSPATH, OnUpdateViewPreviouspath)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_VIEW_SHOWTOOL, OnViewShowtool)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWTOOL, OnUpdateViewShowtool)
	ON_COMMAND(ID_VIEW_MODELNORMALS, OnViewModelnormals)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MODELNORMALS, OnUpdateViewModelnormals)
	ON_COMMAND(ID_VIEW_PATHNORMALS, OnViewPathnormals)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PATHNORMALS, OnUpdateViewPathnormals)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_SHOWCOMMANDED, OnViewShowcommanded)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWCOMMANDED, OnUpdateViewShowcommanded)
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_COMMAND(ID_VIEW_CURRENTPATH, OnViewCurrentpath)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CURRENTPATH, OnUpdateViewCurrentpath)
	ON_COMMAND(ID_VIEW_RESETVIEW, OnViewResetview)
	ON_COMMAND(ID_VIEW_SHOWPRINTER, &CFabAtHomeView::OnViewShowprinter)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWPRINTER, &CFabAtHomeView::OnUpdateViewShowprinter)
	ON_COMMAND(ID_VIEW_SHOWTRACE, &CFabAtHomeView::OnViewShowtrace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWTRACE, &CFabAtHomeView::OnUpdateViewShowtrace)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeView construction/destruction

//---------------------------------------------------------------------------
CFabAtHomeView::CFabAtHomeView()
//---------------------------------------------------------------------------
{ 

	hrc = 0;
	bRolling = FALSE;
	bPanning = FALSE;
	m_fov = 40;
	scale = 0.007;
	eye = CVec(0,0,5);
	view = CVec(0,0,-1000);
	up = CVec(0,1.0,0);
	bShowModel = true;
	bShowPrinter = true;
	bShaded = true;
	bShowPaths = true;
	bShowTrace = true;
	bFollowCurrentLayer = false;
	bShowCommanded =false; //try to show real-time positions
	firstlayer = -1; 
	lastlayer = -1;
	m_bDrawing = false;
	m_dwTimerPeriod = VIEW_REFRESH_MULT*STATUS_UPDATE_PERIOD;

	bModelNormals = false;
	bPathNormals = false;
}

//---------------------------------------------------------------------------
CFabAtHomeView::~CFabAtHomeView()
//---------------------------------------------------------------------------
{
}

//---------------------------------------------------------------------------
BOOL CFabAtHomeView::PreCreateWindow(CREATESTRUCT& cs)
//---------------------------------------------------------------------------
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

    // An OpenGL window must be created with the following flags and must not
    // include CS_PARENTDC for the class style. Refer to SetPixelFormat
    // documentation in the "Comments" section for further information.
	
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZE;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeView drawing

//---------------------------------------------------------------------------
void CFabAtHomeView::OnDraw(CDC* pDC)
//---------------------------------------------------------------------------
{
	CFabAtHomeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	ASSERT(hrc != NULL);
	//if(!m_bDrawing)
		DrawScene();
}

/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeView printing

//---------------------------------------------------------------------------
BOOL CFabAtHomeView::OnPreparePrinting(CPrintInfo* pInfo)
//---------------------------------------------------------------------------
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//---------------------------------------------------------------------------
{
	// TODO: add extra initialization before printing
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//---------------------------------------------------------------------------
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeView diagnostics

#ifdef _DEBUG
//---------------------------------------------------------------------------
void CFabAtHomeView::AssertValid() const
//---------------------------------------------------------------------------
{
	CView::AssertValid();
}

//---------------------------------------------------------------------------
void CFabAtHomeView::Dump(CDumpContext& dc) const
//---------------------------------------------------------------------------
{
	CView::Dump(dc);
}

//---------------------------------------------------------------------------
CFabAtHomeDoc* CFabAtHomeView::GetDocument() // non-debug version is inline
//---------------------------------------------------------------------------
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFabAtHomeDoc)));
	return (CFabAtHomeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFabAtHomeView message handlers

//---------------------------------------------------------------------------
void CFabAtHomeView::OnDestroy() 
//---------------------------------------------------------------------------
{
	CView::OnDestroy();
	
    ::wglMakeCurrent(NULL,  NULL);
	
    if (hrc)
        ::wglDeleteContext(hrc);
	
    if (m_pDC)
        delete m_pDC;
	
}

//---------------------------------------------------------------------------
BOOL CFabAtHomeView::bSetupPixelFormat()
//---------------------------------------------------------------------------
{
    static PIXELFORMATDESCRIPTOR pfd = 
	{
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
			1,                              // version number
			PFD_DRAW_TO_WINDOW |            // support window
			PFD_SUPPORT_OPENGL |          // support OpenGL
			PFD_DOUBLEBUFFER,             // double buffered
			PFD_TYPE_RGBA,                  // RGBA type
			24,                             // 24-bit color depth
			0, 0, 0, 0, 0, 0,               // color bits ignored
			0,                              // no alpha buffer
			0,                              // shift bit ignored
			0,                              // no accumulation buffer
			0, 0, 0, 0,                     // accum bits ignored
			32,                             // 32-bit z-buffer
			0,                              // no stencil buffer
			0,                              // no auxiliary buffer
			PFD_MAIN_PLANE,                 // main layer
			0,                              // reserved
			0, 0, 0                         // layer masks ignored
    };
    int pixelformat;
	
    if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
    {
        ((CFabAtHomeApp *)AfxGetApp())->Log("ChoosePixelFormat failed");
        return FALSE;
    }
	
    if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
    {
        ((CFabAtHomeApp *)AfxGetApp())->Log("SetPixelFormat failed");
        return FALSE;
    }
	
    return TRUE;
}

//---------------------------------------------------------------------------
int CFabAtHomeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
//---------------------------------------------------------------------------
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InitGL();	// initialize OpenGL
	
	return 0;
}


//---------------------------------------------------------------------------
bool CFabAtHomeView::ReportGLError(CString &note, CStdioFile *pFile)
//---------------------------------------------------------------------------
{
	//report the OpenGL error, if any, including the note provided
	//write to *pFile, if not NULL; otherwise just trace
	//return false if no error

	GLenum glErr = glGetError();
#ifdef _DEBUG
	if(!glErr) return false;
	if(pFile == NULL)
	{
		TRACE(note);
		TRACE((LPCTSTR)gluErrorString(glErr));
		TRACE("\n");
	}
	else
	{
		CString msg;
		msg.Format("%s%s\n",note,(LPCTSTR)gluErrorString(glErr));
		pFile->WriteString(msg);
	}
#endif
	return true;
}

//---------------------------------------------------------------------------
void CFabAtHomeView::InitGL()
//---------------------------------------------------------------------------
{
    PIXELFORMATDESCRIPTOR pfd;
    int         n;
	GLfloat		fMaxObjSize, fAspect;
	GLfloat		fNearPlane, fFarPlane;
	
    m_pDC = new CClientDC(this);
	
    ASSERT(m_pDC != NULL);
	
    if (!bSetupPixelFormat())
        return;
	
    n = ::GetPixelFormat(m_pDC->GetSafeHdc());
    ::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);
	
    hrc = wglCreateContext(m_pDC->GetSafeHdc());
    wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);
	
    GetClientRect(&m_oldRect);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
	
	if (m_oldRect.bottom)
		fAspect = (GLfloat)m_oldRect.right/m_oldRect.bottom;
	else	// don't divide by zero, not that we should ever run into that...
		fAspect = 1.0f;
	fNearPlane = 0.0f;
	fFarPlane = 100.0f;
	fMaxObjSize = 3.0f;
	m_fRadius = fNearPlane + fMaxObjSize / 2.0f;

	//initialize the panning/zooming cursor start point.
	startPoint = m_oldRect.CenterPoint();
	
	// initialize rolling matrix with unity
	
	glPushMatrix();
    glLoadIdentity();
	glRotated(-90,0,0,1);
	glRotated(-60,0,1,0);
	glScaled(scale, scale, scale);
	glGetDoublev(GL_MODELVIEW_MATRIX, rotmat);
	glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	SetViewTransformation();
    glMatrixMode(GL_MODELVIEW);

//  Load character Set
//  BOOL CreateFont( int nHeight, int nWidth, int nEscapement, int nOrientation, 
//		int nWeight, BYTE bItalic, BYTE bUnderline, BYTE cStrikeOut, BYTE nCharSet, 
//		BYTE nOutPrecision, BYTE nClipPrecision, BYTE nQuality,
//		BYTE nPitchAndFamily, LPCTSTR lpszFacename );
	CFont times;
	times.CreateFont(0,0,0,0,
		FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,
		OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
		DEFAULT_PITCH,"Times New Roman");
//	m_pDC->SelectObject(GetStockObject(SYSTEM_FONT)); 
	m_pDC->SelectObject(&times); 
	// create the bitmap display lists 
	// we're making images of glyphs 0 thru 255 
	// the display list numbering starts at 1000, an arbitrary choice 
	wglUseFontBitmaps(m_pDC->GetSafeHdc(), 0, 255, 1000); 
	// display a string: 
	// indicate start of glyph display lists 
	 glListBase(1000); 
	// to draw the characters in a string, do, for example:
//	 glCallLists(24, GL_UNSIGNED_BYTE, "Hello Win32 OpenGL World"); 

}


//---------------------------------------------------------------------------
void CFabAtHomeView::ResetView(void)
//---------------------------------------------------------------------------
{
	//reset the view back to its starting condition
	m_fov = 40;
	scale = 0.007;
	eye = CVec(0,0,5);
	view = CVec(0,0,-1000);
	up = CVec(0,1.0,0);

	// reinitialize rolling matrix with unity
	
	glPushMatrix();
    glLoadIdentity();
	glRotated(-90,0,0,1);
	glRotated(-60,0,1,0);
	glScaled(scale, scale, scale);
	glGetDoublev(GL_MODELVIEW_MATRIX, rotmat);
	glPopMatrix();

	Invalidate();
}


//---------------------------------------------------------------------------
void CFabAtHomeView::DrawScene(void)
//---------------------------------------------------------------------------
{// draw the scene

	m_bDrawing = true;
	CFabAtHomeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CSingleLock lockModel(&pDoc->model.m_mutAccess);
	lockModel.Lock(100);
	if(!lockModel.IsLocked()) return;
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	CVec platetop(0,0,0);
	//get the graphics position (offset by PrinterComponent home from the hardware home (0,0,0).
	if (pApp->printer.IsDefined())
		platetop = pApp->printer.component[3].GetGraphicsRTPos() + pApp->printer.component[3].pmax;

	// background

	glClearColor(1,1,1,1);
	ReportGLError(CString("ClearColor"));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ReportGLError(CString("Clear"));
	
	// lighting

	SetLighting();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	ReportGLError(CString("PolygonMode"));

	// draw

    glPushMatrix();
	ReportGLError(CString("PushMatrix1"));
	
	if(bPanning)
	{
		view.x=(eye.x+view.z)/view.z;
		view.y=(eye.y+view.z)/view.z;
	}
	gluLookAt(eye.x, eye.y, eye.z, view.x, view.y, view.z, up.x, up.y, up.z);
	ReportGLError(CString("gluLookAt"));

	glMultMatrixd(rotmat);
	ReportGLError(CString("MultMatrix"));
	glPushMatrix();
	ReportGLError(CString("PushMatrix2"));

	
	glEnable(GL_NORMALIZE);
	ReportGLError(CString("Enable"));

	if(bShowPrinter)
	{
		((CFabAtHomeApp*) AfxGetApp())->printer.Draw();
		ReportGLError(CString("printer.Draw"));
	}

	// model

	glPushMatrix();
	ReportGLError(CString("PushMatrix3"));
	glTranslated(0,0,platetop.z);
	ReportGLError(CString("Translate"));
	DrawMainAxes(20);
	ReportGLError(CString("DrawAxes"));

	glPushName(1);
	ReportGLError(CString("PushName1"));
	glLoadName(NAMESTACK_CHUNK);
	ReportGLError(CString("LoadName1"));
	glPushName(NAMESTACK_CHUNK);
	ReportGLError(CString("PushName"));

	if (bShowModel) {
		glColor3d(0.4,0.8,0.2);
		for (int i=0; i<pDoc->model.chunk.GetSize(); i++) {
			glLoadName(i);
			ReportGLError(CString("LoadName2"));

			pDoc->model.chunk[i].Draw(&pDoc->model, bModelNormals, bShaded);

			ReportGLError(CString("DrawChunk"));
		}
	}
	glPopName();
	ReportGLError(CString("PopName1"));
	
	// paths
	
	if(bFollowCurrentLayer) 
	{
		UpdateCurrentLayer();
	}
	pDoc->model.fab.DrawLayers(&pDoc->model, firstlayer, lastlayer, bShowPaths, bShowTrace);

	ReportGLError(CString("DrawLayers"));

	glColor3d(1,1,1);
	ReportGLError(CString("Color3d"));
	glLineWidth(5);
	ReportGLError(CString("LineWidth1"));
	glLineWidth(1);
	ReportGLError(CString("LineWidth2"));

	glPopName();
	ReportGLError(CString("PopName2"));
	glPopMatrix();
	ReportGLError(CString("PopMatrix1"));
	glPopMatrix();
	ReportGLError(CString("PopMatrix2"));

	// end

	glDisable(GL_COLOR_MATERIAL);
	ReportGLError(CString("Disable"));
	
    glPopMatrix();
	ReportGLError(CString("PopMatrix3"));
	glFinish();
	ReportGLError(CString("Finish"));
	HDC localDC = wglGetCurrentDC();
	ASSERT(localDC != NULL);
	SwapBuffers(localDC);
	ReportGLError(CString("SwapBuffers"));
	m_bDrawing = false;
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnTimer(UINT nIDEvent) 
//---------------------------------------------------------------------------
{// update display periodically if simulating or fabricating and something has changed

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

	if (nIDEvent == VIEW_REFRESH_TIMER)
	{
		//so that we don't refresh more often than necessary
		if(pApp->printer.bNewStatus)
		{
			pApp->printer.bNewStatus = false; 
			if ((pApp->pStatusDlg != NULL) && (pApp->pStatusDlg->IsWindowVisible()))
			{ // update status dialog
				CString text = pApp->printer.GetStatusText();
				pApp->pStatusDlg->statictext.SetWindowText(text);
			}
		}
		//show the build information
		CFabAtHomeDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if ((pApp->pBuildInfoDlg != NULL) && (pApp->pBuildInfoDlg->IsWindowVisible()))
		{ // update build info dialog
			CString text = pDoc->model.GetBuildInfo();
			pApp->pBuildInfoDlg->infotext.SetWindowText(text);
		}
		//pApp->UpdateAllViews();
	}
	//finally, check whether PC performance has required status update rate to drop
	//if so, adjust the view update rate accordingly
	double targetPeriod = pApp->printer.GetTargetStatusPeriod()*VIEW_REFRESH_MULT;
	if(targetPeriod > MAX_VIEW_REFRESH_PERIOD) targetPeriod = MAX_VIEW_REFRESH_PERIOD;
	if(abs(targetPeriod-m_dwTimerPeriod) > 0.1*targetPeriod)
	{
		KillTimer(m_uiTimer);
		m_dwTimerPeriod = (DWORD)targetPeriod;
		m_uiTimer = SetTimer(VIEW_REFRESH_TIMER, m_dwTimerPeriod, NULL);
	}
	CView::OnTimer(nIDEvent);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnInitialUpdate() 
//---------------------------------------------------------------------------
{
	CView::OnInitialUpdate();
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	
	//update every VIEW_REFRESH * STATUS_UPDATE_PERIOR ms
	m_uiTimer = SetTimer(VIEW_REFRESH_TIMER, m_dwTimerPeriod, NULL); 
	pApp->UpdateAllViews();
}


//---------------------------------------------------------------------------
BOOL CFabAtHomeView::OnEraseBkgnd(CDC* pDC) 
//---------------------------------------------------------------------------
{// don't erase background
	
	return TRUE;
}



//---------------------------------------------------------------------------
void CFabAtHomeView::OnSize(UINT nType, int cx, int cy) 
//---------------------------------------------------------------------------
{	
	CView::OnSize(nType, cx, cy);
	
    if(cy > 0) {    
		
		wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);
		
        glViewport(0, 0, cx, cy);
		
        if((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
            RedrawWindow();
		
        m_oldRect.right = cx;
        m_oldRect.bottom = cy;
		
		UpdatePerspective();
    }
}


//---------------------------------------------------------------------------
void CFabAtHomeView::OnMButtonDown(UINT nFlags, CPoint point) 
//---------------------------------------------------------------------------
{// Start 3D rolling
	
//	ASSERT(!bRolling);
	if (bRolling) OnMButtonUp(nFlags, point);

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	
	bRolling = TRUE;
	startPoint = point;
	memcpy(rotmat0, rotmat, 16*sizeof(GLdouble)); // initialize rolling matrix
	SetCursor(pApp->hHandCursor);
	SetCapture();
	
	CView::OnMButtonDown(nFlags, point);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnMButtonUp(UINT nFlags, CPoint point) 
//---------------------------------------------------------------------------
{ // End rolling
	
	if (bRolling) {
		bRolling = FALSE;
		ReleaseCapture();
	}
	
	CView::OnMButtonUp(nFlags, point);
}


//---------------------------------------------------------------------------
void CFabAtHomeView::OnMouseMove(UINT nFlags, CPoint point) 
//---------------------------------------------------------------------------
{// roll if middle button down (bRolling); pan if left button down (bPanning)
	
	if (bRolling) {
		glPushMatrix();
		glLoadIdentity();
		glRotated((point.x-startPoint.x)/2, 0, 1, 0);
		glRotated((point.y-startPoint.y)/2, 1, 0, 0);
		glMultMatrixd(rotmat0);
		glGetDoublev(GL_MODELVIEW_MATRIX, rotmat);
		glPopMatrix();
		Invalidate();
	}

	if (bPanning)
	{
		eye.x += (-point.x+startPoint.x)*eye.z/(1000.0);
		eye.y += (point.y-startPoint.y)*eye.z/(1000.0);
		startPoint = point;
		Invalidate();
	}
	
	CView::OnMouseMove(nFlags, point);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewZoomin() 
//---------------------------------------------------------------------------
{ // Zoom into world

	eye.z -= 0.1*fabs(eye.z) + 0.001;
	//TRACE("Eye(%f,%f,%f)\n",eye.x, eye.y, eye.z);
	Invalidate();
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewZoomout() 
//---------------------------------------------------------------------------
{// Zoom out
	
	eye.z += 0.1*fabs(eye.z) + 0.001;
	Invalidate();
}

//---------------------------------------------------------------------------
void CFabAtHomeView::UpdatePerspective()
//---------------------------------------------------------------------------
{// update perspective parameters
	
	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	SetViewTransformation();
    glMatrixMode(GL_MODELVIEW);	
}

//---------------------------------------------------------------------------
void CFabAtHomeView::SetViewTransformation()
//---------------------------------------------------------------------------
{ // setup perspective/orth tansformation

	GLdouble aspect;

	if (m_oldRect.bottom > 0) {
		aspect = (GLdouble) m_oldRect.right/m_oldRect.bottom;
	} else {
		aspect = 1;
	}

	gluPerspective(m_fov, aspect, 0.01, 100);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::DrawMainAxes(double size)
//---------------------------------------------------------------------------
{ // Draw axes of main coord system

	glLineWidth(2.0);
	glBegin(GL_LINES);
	glColor3d(size,0,0);
	glVertex3d(0,0,0);
	glVertex3d(size,0,0);
	glVertex3d(0,0,0);
	glVertex3d(0,size,0);
	glVertex3d(0,0,0);
	glVertex3d(0,0,size);
	glEnd();

	glListBase(1000); 
	glColor3d(0,1,1);
	glRasterPos3d(size,0,size/10.0);
	glCallLists(1, GL_UNSIGNED_BYTE, "x");
	glRasterPos3d(0,size,size/10.0);
	glCallLists(1, GL_UNSIGNED_BYTE, "y");
	glRasterPos3d(0,0,size);
	glCallLists(1, GL_UNSIGNED_BYTE, "z");

	glColor3d(1,0,0);
	CVec o(0,0,0);
	o.DrawMarker();
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewShaded() 
//---------------------------------------------------------------------------
{
	bShaded = !bShaded;
	Invalidate();
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnUpdateViewShaded(CCmdUI* pCmdUI) 
//---------------------------------------------------------------------------
{
	pCmdUI->Enable(bShowModel);
	if(bShaded)
		pCmdUI->SetText("Render Wireframe");
	else
		pCmdUI->SetText("Render Shaded");
}

//---------------------------------------------------------------------------
void CFabAtHomeView::SetLighting()
//---------------------------------------------------------------------------
{

	GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat mat_shininess[] = {50};
//	GLfloat light_position1[] = {-10.0f, -10.0f, -10.0f, 1.0f};
	GLfloat light_position2[] = {10.0f, 10.0f, -30.0f, 0.0f};
	GLfloat lmodel_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
//	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewShowpaths() 
//---------------------------------------------------------------------------
{
	bShowPaths = !bShowPaths;
	Invalidate();	
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnUpdateViewShowpaths(CCmdUI* pCmdUI) 
//---------------------------------------------------------------------------
{
	pCmdUI->SetCheck(bShowPaths);
	CFabAtHomeDoc* pDoc = GetDocument();
	int n = pDoc->model.fab.layer.GetSize();
	pCmdUI->Enable(n > 0);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewShowmodel() 
//---------------------------------------------------------------------------
{
	bShowModel = !bShowModel;
	Invalidate();
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnUpdateViewShowmodel(CCmdUI* pCmdUI) 
//---------------------------------------------------------------------------
{
	pCmdUI->SetCheck(bShowModel);
	Invalidate();
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewNextpath() 
//---------------------------------------------------------------------------
{
	CFabAtHomeDoc* pDoc = GetDocument();
	int n = pDoc->model.fab.layer.GetSize();
	firstlayer = (firstlayer +1) % n;
	lastlayer = firstlayer;
	bFollowCurrentLayer = false;
	CString msg;
	msg.Format("Showing layer %d of %d",firstlayer+1,n);
	pDoc->model.SetStatusMsg(msg);
	Invalidate();
	pDoc->UpdateAllViews(0);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewPreviouspath() 
//---------------------------------------------------------------------------
{
	CFabAtHomeDoc* pDoc = GetDocument();
	int n = pDoc->model.fab.layer.GetSize();
	firstlayer = (firstlayer + n - 1) % n;
	lastlayer = firstlayer;
	bFollowCurrentLayer = false;
	CString msg;
	msg.Format("Showing layer %d of %d",firstlayer+1,n);
	pDoc->model.SetStatusMsg(msg);
	Invalidate();
	pDoc->UpdateAllViews(0);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewCurrentpath() 
//---------------------------------------------------------------------------
{
	CFabAtHomeDoc* pDoc = GetDocument();
	int n = pDoc->model.fab.layer.GetSize();
	bFollowCurrentLayer = true;
	UpdateCurrentLayer();
	firstlayer = lastlayer;
	CString msg;
	msg.Format("Showing layer %d of %d",firstlayer+1,n);
	pDoc->model.SetStatusMsg(msg);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::UpdateCurrentLayer() 
//---------------------------------------------------------------------------
{
	CFabAtHomeDoc* pDoc = GetDocument();
	int n = pDoc->model.fab.GetCurrentLayer();
	if(pDoc->model.IsSlicing())
		n = pDoc->model.fab.layer.GetSize()-1;
	lastlayer = n;
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewAllpaths() 
//---------------------------------------------------------------------------
{
	CFabAtHomeDoc* pDoc = GetDocument();
	int n = pDoc->model.fab.layer.GetSize();
	firstlayer = 0;
	lastlayer = n-1;
	bFollowCurrentLayer = pDoc->model.IsSlicing();
	Invalidate();
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnUpdateViewNextpath(CCmdUI* pCmdUI) 
//---------------------------------------------------------------------------
{
	CFabAtHomeDoc* pDoc = GetDocument();
	int n = pDoc->model.fab.layer.GetSize();
	pCmdUI->Enable(bShowPaths || bShowTrace && n > 0);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnUpdateViewAllpaths(CCmdUI* pCmdUI) 
//---------------------------------------------------------------------------
{
	CFabAtHomeDoc* pDoc = GetDocument();
	int n = pDoc->model.fab.layer.GetSize();
	pCmdUI->Enable(bShowPaths || bShowTrace && n > 0);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnUpdateViewPreviouspath(CCmdUI* pCmdUI) 
//---------------------------------------------------------------------------
{
	CFabAtHomeDoc* pDoc = GetDocument();
	int n = pDoc->model.fab.layer.GetSize();
	pCmdUI->Enable(bShowPaths || bShowTrace && n > 0);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnUpdateViewCurrentpath(CCmdUI* pCmdUI) 
//---------------------------------------------------------------------------
{
	CFabAtHomeDoc* pDoc = GetDocument();
	int n = pDoc->model.fab.layer.GetSize();
	pCmdUI->Enable(bShowPaths || bShowTrace && n > 0);
	pCmdUI->SetCheck(bFollowCurrentLayer);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnLButtonDown(UINT nFlags, CPoint point) 
//---------------------------------------------------------------------------
{
	if (bPanning) OnLButtonUp(nFlags, point);

	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

    wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);
	Pick(nFlags, point);

	bPanning = TRUE;
	startPoint = point;
	SetCursor(pApp->hHandCursor);
	SetCapture();
	
	CView::OnLButtonDown(nFlags, point);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnLButtonUp(UINT nFlags, CPoint point)
//---------------------------------------------------------------------------
{
	if (bPanning) {
		bPanning = FALSE;
		ReleaseCapture();
	}
	CView::OnLButtonUp(nFlags, point);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::Pick(UINT nFlags, CPoint point)
//---------------------------------------------------------------------------
{// find objects in selction zone

	if(!this->bShowModel) return;
	#define BUFSIZE 1000

	GLuint selectBuf[BUFSIZE];
	GLint hits=0, viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPickMatrix(point.x, viewport[3]-point.y, 5, 5, viewport);
	SetViewTransformation();
    glMatrixMode(GL_MODELVIEW);

	glSelectBuffer(BUFSIZE, selectBuf);
	glRenderMode(GL_SELECT);
	glInitNames();
    DrawScene();
	hits = glRenderMode(GL_RENDER);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	SetViewTransformation();
    glMatrixMode(GL_MODELVIEW);

	// deselect all if necessary

	CFabAtHomeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if ((nFlags & MK_SHIFT) == 0) {
		pDoc->model.SelectAll(false);
	}

	// parse hit buffer and select items

	int idx=0,v;
	for (int hit = 0; hit<hits; hit++) {
  		int names = selectBuf[idx];
		if (names == 2) {
			switch (selectBuf[idx+3]) {
			case NAMESTACK_CHUNK:
				v = selectBuf[idx+4];
				ASSERT(v >= 0 && v <= pDoc->model.chunk.GetSize());
				pDoc->model.SelectOneChunk(v);
				pDoc->UpdateAllViews(0);
				return;
				break;
			}
		}
		idx += 3+names;
	}

	pDoc->UpdateAllViews(0);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnLButtonDblClk(UINT nFlags, CPoint point) 
//---------------------------------------------------------------------------
{
	CFabAtHomeDoc* pDoc = GetDocument();
	int n = pDoc->model.CountChunksSelected();
	if (n == 1) {
		int idx = pDoc->model.GetChunkSelected(0);
		pDoc->UpdateAllViews(0);
		pDoc->EditChunkProperties();
		pDoc->UpdateAllViews(0);
	}
	
	CView::OnLButtonDblClk(nFlags, point);
}


//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewShowtool() 
//---------------------------------------------------------------------------
{
	bShowTool = !bShowTool;
	Invalidate();
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnUpdateViewShowtool(CCmdUI* pCmdUI) 
//---------------------------------------------------------------------------
{
	pCmdUI->SetCheck(bShowTool);
	Invalidate();
}


//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewModelnormals() 
//---------------------------------------------------------------------------
{
	bModelNormals = !bModelNormals;
	Invalidate();
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnUpdateViewModelnormals(CCmdUI* pCmdUI) 
//---------------------------------------------------------------------------
{
	pCmdUI->SetCheck(bModelNormals);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewPathnormals() 
//---------------------------------------------------------------------------
{
	bPathNormals = !bPathNormals;
	Invalidate();
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnUpdateViewPathnormals(CCmdUI* pCmdUI) 
//---------------------------------------------------------------------------
{
	pCmdUI->SetCheck(bPathNormals);
}

//---------------------------------------------------------------------------
BOOL CFabAtHomeView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
//---------------------------------------------------------------------------
{
	if (zDelta < 0)
	{
		CRect rect;
		GetClientRect(&rect);
		//if zooming in, zoom toward the pointer
		//convert pointer coords to screen centered system
		//including inverted y direction
		double dx,dy,coeff;
		dx = ((pt.x-rect.CenterPoint().x)-eye.x);
		dy = ((rect.CenterPoint().y+100-pt.y)-eye.y);
		coeff = sqrt((double)(dx*dx + dy*dy));
		dx /= coeff;
		dy /= coeff*rect.right/rect.bottom; //increase the weight of dy by aspect ratio
		coeff = (0.05*eye.z*eye.z)/(1+eye.z);
		eye.x += dx*coeff;
		eye.y += dy*coeff;
		OnViewZoomin();
		startPoint = pt;
	} 
	else
	{
		//if zooming out, zoom away from the straight ahead
		OnViewZoomout();
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewShowcommanded()
//---------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	if(pApp->printer.bEmulateHardware)
		bShowCommanded = true;
	else
		bShowCommanded = !bShowCommanded;
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnUpdateViewShowcommanded(CCmdUI *pCmdUI)
//---------------------------------------------------------------------------
{
	CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
	pCmdUI->Enable( !bShowCommanded || (bShowCommanded && 
		(pApp->printer.bHardwareInitialized || pApp->printer.bEmulateHardware)) );

	if(bShowCommanded)
		pCmdUI->SetText("Show Real-time");
	else
		pCmdUI->SetText("Show Commanded");
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewResetview()
//---------------------------------------------------------------------------
{
	ResetView();
	Invalidate();
}


//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewShowprinter()
//---------------------------------------------------------------------------
{
	bShowPrinter = !bShowPrinter;
	Invalidate();
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnUpdateViewShowprinter(CCmdUI *pCmdUI)
//---------------------------------------------------------------------------
{
	pCmdUI->SetCheck(bShowPrinter);
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnViewShowtrace()
//---------------------------------------------------------------------------
{
	bShowTrace = !bShowTrace;
	Invalidate();
}

//---------------------------------------------------------------------------
void CFabAtHomeView::OnUpdateViewShowtrace(CCmdUI *pCmdUI)
//---------------------------------------------------------------------------
{
	pCmdUI->SetCheck(bShowTrace);
	CFabAtHomeDoc* pDoc = GetDocument();
	int n = pDoc->model.fab.layer.GetSize();
	pCmdUI->Enable(n > 0);
}