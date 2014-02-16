#ifndef PATH_H
#define PATH_H

#include "Util.h"
#include "Point.h"
#include "Tool.h"

namespace PROJECT_NAMESPACE
{
     //A Path object is a sequence of points that is executed by a tool.
     class Path
     {          
          public:
               double length;
               MaterialCalibration* materialCalibration;
     		vector<Point> points;
     		
               Path(MaterialCalibration*, vector<Point>&);
               
          private:
               //Returns the length of points.
               static double calcLength(const vector<Point>& points);
     };
}
     
#endif //ndef PATH_H
