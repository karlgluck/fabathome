#pragma once

#define NET_DEFAULT_PORT	7871

class CConn
{
//protected:
public:	
	CSocket*		_sock;
	CSocketFile*	_file;
	CArchive*		_arIn;
	CArchive*		_arOut;
	
	bool			_connected;
	
	static HANDLE	_creationMutex;
	
public:
	
	CString	_name;
	int		_port;
	
public:
	
	CConn() : _sock(0), _file(0), _arIn(0), _arOut(0), _connected(false), _name(""), _port(NET_DEFAULT_PORT) 
	{
		if (_creationMutex == 0)
			_creationMutex = CreateMutex(NULL, FALSE, NULL);
	}
	
	~CConn() 
	{ 
		disconnect();
	}
	
	void dealloc()
	{
		if (_arIn)	delete _arIn;	_arIn	= 0;
		if (_arOut)	delete _arOut;	_arOut	= 0;
		if (_file)	delete _file;	_file	= 0;
		if (_sock)	delete _sock;	_sock	= 0;
	}
	
	void cancel();
	
	bool connect() { return connect(_name, _port); }
	bool connect(const CString name, int port = NET_DEFAULT_PORT) ;
	void disconnect();
	bool isConnected() const { return _connected; }
	
	bool listen(int port = NET_DEFAULT_PORT);
	bool accept(CConn& conn);
	
	bool flush();
	
	bool readn(void* buf, int bytes);
	bool sendn(const void* buf, int bytes);
	
	int readv(void* buf, int maxBytes);
	bool sendv(const void* buf, int bytes);
	
	bool readString(CString& str);
	bool sendString(const char* str);

	bool sendBool(bool      v)	{ return sendn(&v, sizeof(bool  )); }
	bool sendChar(char      v)	{ return sendn(&v, sizeof(char  )); }
	bool sendInt(int        v)	{ return sendn(&v, sizeof(int   )); }
	bool sendFloat(float    v)	{ return sendn(&v, sizeof(float )); }
	bool sendDouble(double  v)	{ return sendn(&v, sizeof(double)); }
	
	bool readBool(bool*     p)	{ return readn(p, sizeof(bool  )); }
	bool readChar(char*     p)	{ return readn(p, sizeof(char  )); }
	bool readInt(int*       p)	{ return readn(p, sizeof(int   )); }
	bool readFloat(float*   p)	{ return readn(p, sizeof(float )); }
	bool readDouble(double* p)	{ return readn(p, sizeof(double)); }
	
};

bool closeProcessByName(const char* name);
BOOL CALLBACK enumCloseProc(HWND hwnd, LPARAM lParam);
