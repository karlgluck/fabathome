#ifndef BAY_H
#define BAY_H

#include "Motor.h"
#include "Point.h"
#include "MaterialCalibration.h"

namespace PROJECT_NAMESPACE
{
     class Bay
     {          
          public:
               string name;
               Motor* motor;
               Point location;
               MaterialCalibration* materialCalibration;

               Bay(void);
               Bay(string,Point,Motor*);
     };
}

#endif //ndef BAY_H