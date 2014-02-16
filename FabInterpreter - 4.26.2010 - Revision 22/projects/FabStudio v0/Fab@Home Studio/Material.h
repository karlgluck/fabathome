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

// Material.h: interface for the CMaterial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATERIAL_H__AEB70B07_1E76_4D0C_A65A_141E6A50547F__INCLUDED_)
#define AFX_MATERIAL_H__AEB70B07_1E76_4D0C_A65A_141E6A50547F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMaterial : public CObject  
{
public:
	CMaterial();
	virtual ~CMaterial();
	void Serialize(CArchive& ar);
	CMaterial(CMaterial& s);
	CMaterial& operator=(const CMaterial& s);
	bool LoadFile(CString matfilename, CString& errorstr);
	bool SetDefaults();
	static bool LoadMaterials(CString directory);
	static int SearchByName(CString name);
	CString& GetName(){return name;};

	CString name, description;
	CVec color;
	double alpha;

};

#endif // !defined(AFX_MATERIAL_H__AEB70B07_1E76_4D0C_A65A_141E6A50547F__INCLUDED_)
