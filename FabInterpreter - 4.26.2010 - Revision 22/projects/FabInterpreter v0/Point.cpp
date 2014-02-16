#include "stdafx.h"
#include "Point.h"
////////////////////////////////////////////////////////////////////////////////
Point::Point():x(0),y(0),z(0)
{
}
////////////////////////////////////////////////////////////////////////////////
Point::Point(const double& x, const double& y, const double& z):x(x),y(y),z(z)
{
}
////////////////////////////////////////////////////////////////////////////////
double Point::calcDist(const Point& a, const Point& b)
{
	double xDiff = a.x-b.x;
	double yDiff = a.y-b.y;
	double zDiff = a.z-b.z;
	return sqrt(xDiff*xDiff+yDiff*yDiff+zDiff*zDiff);
}
////////////////////////////////////////////////////////////////////////////////
