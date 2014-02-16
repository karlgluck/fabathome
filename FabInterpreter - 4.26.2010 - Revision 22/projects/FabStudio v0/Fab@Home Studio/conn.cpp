#include "stdafx.h"
#include "conn.h"

HANDLE	CConn::_creationMutex = 0;

void CConn::cancel()
{
	if (_sock) 
		_sock->CancelBlockingCall();
}

bool CConn::connect(const CString name, int port) 
{
	TRACE("Connect()\n");

	disconnect();
	_name = name;
	_port = port;
	
	// wtf hack to avoid resource unavailable exception
	WaitForSingleObject(CConn::_creationMutex, INFINITE);
	_sock	= new CSocket();
	_sock->Create();
	ReleaseMutex(CConn::_creationMutex);
	
	if (_sock && _sock->Connect(_name, _port))
	{
		_file	= new CSocketFile(_sock);
		_arIn	= new CArchive(_file, CArchive::load);
		_arOut	= new CArchive(_file, CArchive::store);
		
		_connected = true;
	}
	else	disconnect();
	
	return isConnected();
}

bool CConn::listen(int port)
{
	TRACE("Listen()\n");
	
	disconnect();
	_name = "???";
	_port = port;
	
	_sock	= new CSocket();
	_sock->Create(_port);
	
	if (_sock->Listen())	{ return true; }
	else					{ disconnect(); return false; }

}
bool CConn::accept(CConn& conn)
{
	TRACE("Accept()\n");
	
	conn.disconnect();
	conn._sock	= new CSocket();

	if (_sock->Accept(*conn._sock))
	{
		conn._sock->GetPeerName(conn._name, (UINT&)conn._port);
		
		conn._file	= new CSocketFile(conn._sock);
		conn._arIn	= new CArchive(conn._file, CArchive::load);
		conn._arOut	= new CArchive(conn._file, CArchive::store);
		
		conn._connected = true;
	}
	else	conn.disconnect();
	
	return conn.isConnected();
}

void CConn::disconnect()
{
	TRACE("Disconnect()\n");
	
	_connected = false;

	try
	{
		if (_arOut)	_arOut->Close();
	}
	catch (CFileException* e)
	{
		if (_arOut) _arOut->Abort();
		e->Delete();
	}
	
	try
	{
		if (_arIn)	_arIn->Close();
	}
	catch (CFileException* e)
	{
		if (_arIn) _arIn->Abort();
		e->Delete();
	}
	
	try
	{
		if (_file)	_file->Close();
	}
	catch (CFileException* e)
	{
		if (_file) _file->Abort();
		e->Delete();
	}
	
	try
	{
		if (_sock)	_sock->ShutDown(2);
		if (_sock)	_sock->Close();
	}
	catch (CFileException* e)
	{
		if (_sock)	_sock->Close();
		e->Delete();
	}

	dealloc();
}

bool CConn::flush()
{
	if (!isConnected()) return false;
	
	try
	{
		_arOut->Flush();
		return true;
	}
	catch (CFileException* e)
	{
		disconnect();
		e->Delete();
		return false;
	}
}

bool CConn::readn(void* buf, int bytes)
{
	if (!isConnected()) return false;
	
	try
	{
		if (_arIn->Read(buf, bytes) == bytes)	{ return true; }
		else									{ disconnect(); return false; }
	}
	catch (CFileException* e)
	{
		disconnect();
		e->Delete();
		return false;
	}
}
bool CConn::sendn(const void* buf, int bytes)
{
	if (!isConnected()) return false;
	
	try
	{
		_arOut->Write(buf, bytes);
		return true;
	}
	catch (CFileException* e)
	{
		disconnect();
		e->Delete();
		return false;
	}
}

int CConn::readv(void* buf, int maxBytes)
{
	int len = 0;
	if (readn(&len, sizeof(int)) && len > 0)
	{
		if (len < maxBytes) readn(buf, len);
		else				readn(buf, maxBytes);
	}
	return len;
}
bool CConn::sendv(const void* buf, int bytes)
{
	if (sendn(&bytes, sizeof(int)))
		if (sendn(buf, bytes))
			return true;
	return false;
}

bool CConn::readString(CString& str)
{
	if (!isConnected()) return false;

	try
	{
		if (_arIn->ReadString(str))	{ return true; }
		else						{ disconnect(); return false; }
	}
	catch (CFileException* e)
	{
		disconnect();
		e->Delete();
		return false;
	}
}
bool CConn::sendString(const char* str)
{
	if (!isConnected()) return false;
	
	try
	{
		_arOut->WriteString(str);
		return true;
	}
	catch (CFileException* e)
	{
		disconnect();
		e->Delete();
		return false;
	}
}



// 
bool closeProcessByName(const char* name)
{
	return (EnumWindows(enumCloseProc, (LPARAM)name) != FALSE);
}
BOOL CALLBACK enumCloseProc(HWND hwnd, LPARAM lParam)
{
	const char* str1 = (const char*)lParam;
	char str2[256];	GetWindowText(hwnd, str2, 256);
	
	if (_strnicmp(str1, str2, 256) == 0)
	{
		PostMessage(hwnd, WM_CLOSE, 0, 0);
	}
	
	return TRUE;
}
