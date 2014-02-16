#include "stdafx.h"
#include "MaterialCalibration.h"
////////////////////////////////////////////////////////////////////////////////
MaterialCalibration::MaterialCalibration(const string& name, const double& pathSpeed, const double& pathWidth, const double& depositionRate, const double& pushout, const double& suckback, const double& suckbackDelay, const double& clearance, int pausePaths, const double& pitch):
NAME(name),PATH_SPEED(pathSpeed),PATH_WIDTH(pathWidth),DEPOSITION_RATE(depositionRate),PUSHOUT(pushout),SUCKBACK(suckback),SUCKBACK_DELAY(suckbackDelay),CLEARANCE(clearance),PAUSE_PATHS(pausePaths),PITCH(pitch),pathsExecuted(0)
{
}
////////////////////////////////////////////////////////////////////////////////
MaterialCalibration::MaterialCalibration()
{
}
////////////////////////////////////////////////////////////////////////////////
