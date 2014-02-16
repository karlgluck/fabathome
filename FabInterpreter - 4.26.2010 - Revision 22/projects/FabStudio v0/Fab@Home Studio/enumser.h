/*
Module : ENUMSER.H
Purpose: Defines the interface for a number of functions to 
         enumerate the serial ports installed on a PC
Created: PJN / 03-11-1998


Copyright (c) 1998 - 2003 by PJ Naughter.  (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/

#ifndef __ENUMSER_H__
#define __ENUMSER_H__


///////////////////////// Includes ////////////////////////////////////////////                      

#ifndef __AFXTEMPL_H__
#pragma message("To avoid this message put afxtempl.h in your PCH")                                                                                
#include <afxtempl.h>
#endif



///////////////////////// Classes /////////////////////////////////////////////


class CEnumerateSerial
{
public:
#ifndef NO_ENUMSERIAL_USING_CREATEFILE
  static BOOL UsingCreateFile(CUIntArray& ports);
#endif

#ifndef NO_ENUMSERIAL_USING_QUERYDOSDEVICE
  static BOOL UsingQueryDosDevice(CUIntArray& ports);
#endif

#ifndef NO_ENUMSERIAL_USING_GETDEFAULTCOMMCONFIG
  static BOOL UsingGetDefaultCommConfig(CUIntArray& ports);
#endif

#ifndef NO_ENUMSERIAL_USING_SETUPAPI
  static BOOL UsingSetupAPI(CUIntArray& ports, CStringArray& sFriendlyNames);
#endif

#ifndef NO_ENUMSERIAL_USING_ENUMPORTS
  static BOOL UsingEnumPorts(CUIntArray& ports);
#endif

#ifndef NO_ENUMSERIAL_USING_WMI
  static BOOL UsingWMI(CUIntArray& ports);
#endif

protected:
  static BOOL IsNumeric(LPCTSTR pszString, BOOL bIgnoreColon);
};

#endif //__ENUMSER_H__