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

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__18646888_D003_4C1A_91BC_1C338D2B8938__INCLUDED_)
#define AFX_STDAFX_H__18646888_D003_4C1A_91BC_1C338D2B8938__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINDOWS  0x0400
#define WINVER			0x0400       // Windows 98 and Windows 2000 or later.
#define _WIN32_IE       0x0400


#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// general

#include "resource.h"
#include <afxtempl.h>
#include "Vec.h"
#include <float.h>
#include "Utils.h"
#include "GLUtils.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <setupapi.h>
#include <winspool.h>
#include <WBemCli.h>
#include <comdef.h>
#include <Afxpriv.h>
#include <afxsock.h>
#include <setupapi.h>
#include <winspool.h>
#include <WBemCli.h>
#include <comdef.h>
#include <Afxpriv.h>

// project
#define _SCB_REPLACE_MINIFRAME
#include "Path.h"
#include "BoundaryTree.h"
#include "Region.h"
#include "CoordSys.h"
#include "sizecbar.h"
#include "scbarg.h"
#include "scbarcf.h"

#include "LogBar.h"
#include "ProgressDlg.h"
#include "AxesDlg.h"
#include "Layer.h"
#include "Fabrication.h"
#include "Triangle.h"
#include "Geometry.h"
#include "printercomponent.h"
#include "ChunkDlg.h"

#include "Chunk.h"
#include "Model.h"
#include "FabAtHome.h"
#include "FabAtHomeDoc.h"
#include "FabAtHomeView.h"
#include "ChildFrm.h"
#include "MainFrm.h"

#include "JogCarriageDlg.h"
#include "JogToolDlg.h"
#include "ToolChangeDlg.h"
#include "ToolDlg.h"
#include "PrinterDlg.h"
#include "StatusDlg.h"
#include <afxdlgs.h>


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__18646888_D003_4C1A_91BC_1C338D2B8938__INCLUDED_)
