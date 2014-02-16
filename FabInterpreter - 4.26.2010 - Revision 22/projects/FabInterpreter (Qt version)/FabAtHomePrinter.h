#ifndef FABATHOMEPRINTER_H
#define FABATHOMEPRINTER_H

#include "Axis.h"
#include "Model.h"
#include "Motor.h"
#include "Path.h"
#include "Tool.h"

class FabAtHomePrinter
{
     //CONSTANTS/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     private:
          byte X_Y_Z_GROUP_ADDRESS;
          unsigned int NUM_MODULES, BAUD_RATE;
          unsigned int COM_PORT;
          double PLATFORM_DELTA;
          double PRINT_ACCELERATION; //The acceleration that is used while printing.  The unit of PRINT_ACCELERATION is U/(second^2).
          double OLD_MSPS; //Magic constant from line 58 of FabAtHomeParameters.h in model 1 code.  Used to calculate values for pushout and suckback.
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     private:
          Model model;
          map<string, Motor, LessThanString> motors; //A map from motor names to motors.
          map<string, MaterialCalibration, LessThanString> materialCalibrations; //A map from material calibration names to material calibrations.
          bool pausePrintFlag, cancelPrintFlag, redoPathFlag, printing, initialized, paused;

          //Any \ in filePath must be escaped with \
          //Returns: "" iff successful or an error message.
          string loadConfigFile(string filePath);

          //Switch printer to path mode.
          //Returns: True iff successful.
          bool initializePathMode(void);         

          //Execute the path.
          //The bay will execute pushout, standard flow, and suckback iff flowDuringPath is true.
          //A call to this function returns when movement has finished.
          void executePath(const Path& path, Bay* bay, const bool flowDuringPath);

          //Execute the points.
          //The bay will execute pushout, standard flow, and suckback iff flowDuringPath is true.
          //A call to this function returns when all points are in the path point buffer.
          void executePoints(const vector<Point>& points, Bay* bay, const bool flowDuringPath, double pushoutDistance, double pushoutVelocity, double standardDistance, double standardVelocity, double suckbackDistance, double suckbackVelocity, const Point& suckbackPoint);

          //Move all points from segment list to path point buffer.  Physical movement begins as soon as a point is in the path point buffer.
          //The bay will execute pushout, standard flow, and suckback iff flowDuringPath is true.
          void downloadSegmentList(Bay* bay, bool startFlow, bool startSuckback, double pushoutDistance, double pushoutVelocity, double standardDistance, double standardVelocity, double suckbackDistance, double suckbackVelocity, const Point& suckbackPoint);

          //Clears the segment list and then tries to add points into the segment list starting from startIndex.
          //Segment list has size 1000 but this function does not necessarily fill it completely.
          //Returns: The last index of points that was added to the segment list or -1 if points is empty.	
          int fillSegmentList(const vector<Point>& points, const int startIndex);

          //Returns: A bay that has the material calibration loaded or NULL if no such bay exists.
          Bay* findBay(MaterialCalibration*);

          void executePausePath(const Point&);
          void executeSetupPath(const Point&, const Point&, const double&);
                    
          string loadMaterialCalibration(TiXmlNode*);
          string loadPath(TiXmlNode*);

          //The copy constructor.  Private to prevent printer from being copied.
          FabAtHomePrinter(const FabAtHomePrinter&);

          //The copy assignment operator.  Private to prevent printer from being copied.
          FabAtHomePrinter& operator=(const FabAtHomePrinter&);
          
     public:
          map<string, Axis, LessThanString> axes; //A map from axis names to axes.
          Tool tool;

          FabAtHomePrinter(void);

          //Inserts into result the names of all material calibrations that are currently loaded.
          void loadedMaterialCalibrations(vector<string>& result);

          //Returns: The number of paths currently loaded.
          int numLoadedPaths(void);

          //Returns: The number of bays currently loaded.
          int numLoadedBays(void);

          //Returns: True iff printer is printing.
          bool isPrinting(void);

          //Returns: True iff printer is paused.
          bool isPaused(void);

          //Pause print after the current path finishes.
          //Requires: isPrinting()
          void pausePrint(void);

          //Cancel print after the current path finishes.
          //Requires: isPrinting()
          void cancelPrint(void);

          //Redo the current path after the current path finishes.
          //Requires: isPrinting()
          void redoPath(void);

          //Any \ in filePath must be escaped with \
          //Load a file specifying material configurations and paths.
          //Returns: "" iff successful or an error message.
          string loadFabFile(string filePath);

          //Initialize the printer.
          //Returns: "" if successful or an error message if failed.
          string initialize(const string& configFilePath);

          //Equip a bay with a material calibration or unequip the bay if the material calibration is "".
          //Requires: !isPrinting() || isPaused() and the bay and material calibration have been loaded.
          void equipBay(const string& bayName, const string& materialCalibrationName);

          //Execute all paths which are currently loaded.
          //A call to this function returns when the print has finished.
          //If fabricationThread != NULL, messages will periodically be stored in displayText
          //describing what the printer is currently doing and progress will be reported to fabricationThread when the message changes.
          //void print(System::ComponentModel::BackgroundWorker* fabricationThread, string& displayText);

          //Perform clean up when done using printer.
          //Returns: True iff successful.
          bool cleanUp(void);

          //The destructor.
          ~FabAtHomePrinter(void);
};

#endif //ndef FABATHOMEPRINTER_H
