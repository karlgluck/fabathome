#include "stdafx.h"
#include "Path.h"
////////////////////////////////////////////////////////////////////////////////
double Path::calcLength(const vector<Point>& points)
{
     double length = 0;
     for(vector<Point>::const_iterator i = points.begin(); i != points.end()-1; ++i)
     {
          length += Point::calcDist(*i,*(i+1));
     }
     return length;
}
////////////////////////////////////////////////////////////////////////////////
Path::Path(MaterialCalibration* materialCalibration, vector<Point>& points):length(Path::calcLength(points)),materialCalibration(materialCalibration),points(points)
{	
}
////////////////////////////////////////////////////////////////////////////////
