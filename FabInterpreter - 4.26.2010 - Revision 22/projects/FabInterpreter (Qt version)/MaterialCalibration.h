#ifndef MATERIALCALIBRATION_H
#define MATERIALCALIBRATION_H

#include "Util.h"

namespace PROJECT_NAMESPACE
{
     class MaterialCalibration
     {          
          public:
               //CONSTANTS/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	          string NAME;
	          double PATH_SPEED,PATH_WIDTH,DEPOSITION_RATE,PUSHOUT,SUCKBACK,SUCKBACK_DELAY,CLEARANCE,PITCH;
               int PAUSE_PATHS;
               //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		     int pathsExecuted; //Number of paths that have been executed using this.
                    
		     //Creates a material calibration named name.
               MaterialCalibration(const string& name, const double& pathSpeed, const double& pathWidth, const double& depositionRate, const double& pushout, const double& suckback, const double& suckbackDelay, const double& clearance, int pausePaths, const double& pitch);
     		     
		     MaterialCalibration();      
     };
}
     
#endif //ndef MATERIALCALIBRATION_H
