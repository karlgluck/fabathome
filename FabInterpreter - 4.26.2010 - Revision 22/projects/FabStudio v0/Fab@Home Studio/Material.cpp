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

// Material.cpp: implementation of the CMaterial class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// -------------------------------------------------------------------
CMaterial::CMaterial()
// -------------------------------------------------------------------
{// default values

	SetDefaults();

}

// -------------------------------------------------------------------
CMaterial::~CMaterial()
// -------------------------------------------------------------------
{

}


//---------------------------------------------------------------------------
void CMaterial::Serialize(CArchive& ar)
//---------------------------------------------------------------------------
{// Save / Load

	int version = 10;

	if (ar.IsStoring()) { //add storing code here

		ar << version;

		// save data

		ar << name;
		color.Serialize(ar);
		ar << alpha;
		ar << description;

		ar << version;

	}	else	{  // add loading code here

		int version1, version2;
		ar >> version1; if (version1 < 10 || version1 > version) throw 6;

		// load data

		ar >> name;
		color.Serialize(ar);
		ar >> alpha;
		ar >> description;

		ar >> version2; if (version2 != version1) throw 6;

	}
}

//---------------------------------------------------------------------------
CMaterial::CMaterial(CMaterial& s)
//---------------------------------------------------------------------------
{
	*this = s;
}

//---------------------------------------------------------------------------
CMaterial& CMaterial::operator=(const CMaterial& s)
//---------------------------------------------------------------------------
{

	name = s.name;
	color = s.color;
	alpha = s.alpha;
	description = s.description;

	return *this;
}


//---------------------------------------------------------------------------
bool CMaterial::LoadFile(CString matfilename, CString& errorstr)
//---------------------------------------------------------------------------
{ // read material parameters from ascii file

	SetDefaults();

	CStdioFile file;
	if (file.Open(matfilename, CFile::modeRead) == 0)
		return false;

	CString line;
	BOOL readok;
	CString error;

	do {
		readok = file.ReadString(line);
		line.Trim();
		if (readok && !line.IsEmpty() && !(line[0]=='#')  && !(line[0]=='/') && !(line[0]=='%') && !(line[0]==';')) {
			CString str1 = line.SpanExcluding(" \t,:");
			CString str1a = line.Mid(str1.GetLength());
			CString str1b = str1a.SpanIncluding(" \t,:");
			CString str2 = str1a.Mid(str1b.GetLength());
			str1.Trim();
			str2.Trim();
			if (!str1.IsEmpty()) {
				if (str1.CompareNoCase("NAME")==0) {
					name = str2;
				} else if (str1.CompareNoCase("DESCRIPTION")==0) {
					description = str2;
				} else if (str1.CompareNoCase("COLOR")==0) {
					str2.Replace(","," ");
					int j=sscanf_s(str2,"%lf %lf %lf %lf", &color.x, &color.y, &color.z, &alpha);
					if (j != 4) {
						error.Format("%s: Cannot read color '%s'\n", matfilename, line);
						errorstr += error;
						color = CVec(0.5,0.5,0.5);
						alpha = 1;
						return false;
					}
				} else {
	//				error.Format("%s: Unrecognized token '%s'\n", toolfilename, str1);
	//				errorstr += error;
				}
			} else {
				error.Format("%s: Cannot parse '%s'\n", matfilename, line);
				errorstr += error;
			}
		}
	} while (readok);

	// convert current position from mm to steps

	file.Close();
	return true;
}

//---------------------------------------------------------------------------
bool CMaterial::SetDefaults()
//---------------------------------------------------------------------------
{ 
	name = "Undefined";
	description = "";
	color = CVec(0.5,0.5,0.5);
	alpha = 1;

	return true;
}


//---------------------------------------------------------------------------
bool CMaterial::LoadMaterials(CString directory)
//---------------------------------------------------------------------------
{ // open and read all material files in given directory

   CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();

   CMaterial newmat;
   CString errorstr;

   WIN32_FIND_DATA FindFileData;
   HANDLE hFind = INVALID_HANDLE_VALUE;

   hFind = FindFirstFile(directory+"\\*.material", &FindFileData);
   if (hFind == INVALID_HANDLE_VALUE) // Invalid file handle; GetLastError()
	   return false;

   do {
	   bool loaded = newmat.LoadFile(directory+"\\"+FindFileData.cFileName, errorstr);
	   if (loaded) 
		   pApp->material.Add(newmat);
	   TRACE("Loaded material %s\n", newmat.name);
   } while (FindNextFile(hFind, &FindFileData) != 0);

   if (!errorstr.IsEmpty())
	   ((CFabAtHomeApp *)AfxGetApp())->Log(CString("Error found while reading material files: ")+errorstr+" Erroneous files skipped.");

   DWORD dwError = GetLastError();
   FindClose(hFind);
   if (dwError != ERROR_NO_MORE_FILES) // FindNextFile error; dwError
	   return false;

   return true;

}

//---------------------------------------------------------------------------
int CMaterial::SearchByName(CString name)
//---------------------------------------------------------------------------
{ // search all material and find match by name

   CFabAtHomeApp *pApp = (CFabAtHomeApp *) AfxGetApp();
   name.Trim();
   if (name.IsEmpty())
	   return -1;

   // try exact match
   for (int i=0; i<pApp->material.GetSize(); i++) {
	   if (name.CompareNoCase(pApp->material[i].name) == 0) 
		   return i;
   }

   // try weak match

   for (int i=0; i<pApp->material.GetSize(); i++) {
	   if (name.Find(pApp->material[i].name) != -1) 
		   return i;
   }

   return -1;

}
