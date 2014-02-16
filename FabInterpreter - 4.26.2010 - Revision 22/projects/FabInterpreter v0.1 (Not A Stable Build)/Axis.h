#ifndef AXIS_H
#define AXIS_H

#include "Motor.h"

namespace PROJECT_NAMESPACE
{
     class Axis
     {          
          public:
			  string name;
                 Motor* motor;
			  
			  Axis(void);
			  Axis(string,Motor*);
     };
}
     
#endif //ndef AXIS_H
