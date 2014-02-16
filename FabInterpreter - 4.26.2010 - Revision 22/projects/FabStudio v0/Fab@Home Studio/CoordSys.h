// CoordSys.h: interface for the CCoordSys class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

enum UNIT_SYSTEM
{
	SI,
	CGS,
	IMPERIAL
};

class CCoordSys
{
public:
	CCoordSys(void);
	CCoordSys(CVec& org, CVec& dirX, CVec& dirY, UNIT_SYSTEM units = SI);
	CCoordSys(const CCoordSys& s);
	CCoordSys& operator=(const CCoordSys& s);
	~CCoordSys(void);
	const CVec& GetDirX(){return m_vecDirX;};
	const CVec& GetDirY(){return m_vecDirY;};
	const CVec& GetDirZ(){return m_vecDirZ;};
	const CVec GetOrg(UNIT_SYSTEM units = SI);
	void SetOrg(const CVec& org, UNIT_SYSTEM units = SI);
	UNIT_SYSTEM GetUnits(){return m_enumUnits;};
	void Draw(CVec color);
	CCoordSys RotateXY(double degAngle);
	CCoordSys Translate(const CVec& disp);
protected:
	CVec Convert(const CVec& v, UNIT_SYSTEM newUnits);
	double Convert(const double d, UNIT_SYSTEM newUnits);
protected:
	CVec m_vecDirX; //X direction vector
	CVec m_vecDirY;	//Y direction vector
	CVec m_vecDirZ;	//Z direction vector
	CVec m_vecOrg;	//Origin
	UNIT_SYSTEM m_enumUnits; //Unit system
};
