#include "stdafx.h"
#include ".\coordsys.h"

//---------------------------------------------------------------------------
CCoordSys::CCoordSys(void)
//---------------------------------------------------------------------------
{
	m_vecDirX = CVec(1,0,0);
	m_vecDirY = CVec(0,1,0);
	m_vecDirZ = CVec(0,0,1);
	m_vecOrg = CVec(0,0,0);
	m_enumUnits = SI;
}

//---------------------------------------------------------------------------
CCoordSys::CCoordSys(const CCoordSys& s)
//---------------------------------------------------------------------------
{
	*this = s;
}

//---------------------------------------------------------------------------
CCoordSys::~CCoordSys(void)
//---------------------------------------------------------------------------
{
}

//---------------------------------------------------------------------------
CCoordSys& CCoordSys::operator=(const CCoordSys& s)
//---------------------------------------------------------------------------
{
	m_vecDirX = s.m_vecDirX;
	m_vecDirY = s.m_vecDirY;
	m_vecDirZ = s.m_vecDirZ;
	m_vecOrg = s.m_vecOrg;
	m_enumUnits = s.m_enumUnits;
	return *this;
}

//---------------------------------------------------------------------------
CCoordSys::CCoordSys(CVec& org, CVec& dirX, CVec& dirY, UNIT_SYSTEM units)
//---------------------------------------------------------------------------
{
	m_vecDirZ = dirX.Cross(dirY);
	m_vecDirX = dirX.Normalized();
	m_vecDirY = dirY.Normalized();
	m_vecDirZ = m_vecDirZ.Normalized();
	m_vecOrg = org;
	m_enumUnits = units;
}

//---------------------------------------------------------------------------
const CVec CCoordSys::GetOrg(UNIT_SYSTEM units)
//---------------------------------------------------------------------------
{
	if(units != m_enumUnits)
		return Convert(m_vecOrg, units);
	return m_vecOrg;
}

//---------------------------------------------------------------------------
void CCoordSys::SetOrg(const CVec& org, UNIT_SYSTEM units)
//---------------------------------------------------------------------------
{
	if(units != m_enumUnits)
		m_vecOrg = Convert(org, units);
	else
		m_vecOrg = org;
}

//---------------------------------------------------------------------------
CVec CCoordSys::Convert(const CVec& v, UNIT_SYSTEM newUnits)
//---------------------------------------------------------------------------
{
	//convert the given vector into the specified new unit system
	CVec tmp;
	if(newUnits == m_enumUnits)
	{
		tmp = v;
	}
	else
	{
		tmp.x = Convert(v.x, newUnits);
		tmp.y = Convert(v.y, newUnits);
		tmp.z = Convert(v.z, newUnits);
	}
	return tmp;
}

//---------------------------------------------------------------------------
double CCoordSys::Convert(const double d, UNIT_SYSTEM newUnits)
//---------------------------------------------------------------------------
{
	//TODO: finish unit conversion
	return d;
}

//---------------------------------------------------------------------------
CCoordSys CCoordSys::RotateXY(double rad)
//---------------------------------------------------------------------------
{
	//rotate the coord sys about its Z axis by rad radians
	CCoordSys tmp = *this;
	if(rad != 0.0)
	{
		tmp.m_vecDirX.RotZ(rad);
		tmp.m_vecDirY.RotZ(rad);
	}
	return tmp;
}

//---------------------------------------------------------------------------
CCoordSys CCoordSys::Translate(const CVec& disp)
//---------------------------------------------------------------------------
{
	//translate the origin of this coord sys by disp
	CCoordSys tmp = *this;
	tmp.m_vecOrg+=disp;
	return tmp;
}

//---------------------------------------------------------------------------
void CCoordSys::Draw(CVec color)
//---------------------------------------------------------------------------
{
	//draw coordinate system axes
	glBegin(GL_LINES);
	glColor3d(color.x,color.y,color.z);
	glVertex3d(m_vecOrg);
	glVertex3d(m_vecOrg+m_vecDirX);
	glVertex3d(m_vecOrg);
	glVertex3d(m_vecOrg+m_vecDirY);
	glVertex3d(m_vecOrg);
	glVertex3d(m_vecOrg+m_vecDirZ);
	glEnd();

	glListBase(1000); 
	glColor3d(0,1,1);
	glRasterPos3d(m_vecOrg+m_vecDirX);
	glCallLists(1, GL_UNSIGNED_BYTE, "x");
	glRasterPos3d(m_vecOrg+m_vecDirY);
	glCallLists(1, GL_UNSIGNED_BYTE, "y");
	glRasterPos3d(m_vecOrg+m_vecDirZ);
	glCallLists(1, GL_UNSIGNED_BYTE, "z");

	m_vecOrg.DrawMarker();
}


	