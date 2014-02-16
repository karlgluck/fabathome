#include "stdafx.h"
#include "FABOutput.h"
#include "FabAtHome.h"
#include "PrinterDlg.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FABOutput::FABOutput(void)
{
	file.InsertEndChild(TiXmlDeclaration("1.0", "UTF-8", "yes"));

     TiXmlElement fabAtHomePrinter("fabAtHomePrinter");
    	
     fabAtHomePrinter.InsertEndChild(TiXmlComment("Acceleration in millimeters/(second^2) that is used while printing."));
     TiXmlElement printAcceleration("printAcceleration");
	printAcceleration.InsertEndChild(TiXmlText("100"));
     fabAtHomePrinter.InsertEndChild(printAcceleration);

	file.InsertEndChild(fabAtHomePrinter);



	loadedTools = 0;
	int index = 0;
	for(int i=0; i<CTool::LoadedToolCount(); i++) {
		CTool *tool = CTool::GetByIndex(i);
		if (tool->IsMounted()==true) { 
			tools[index] = tool;
			index++;
			loadedTools++;
		}
	}

	//variable initilizers
	maxTools = 2; //eventually move to a definition
	toolSelected = 0;
	int restrict = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FABOutput::~FABOutput(void)
{
    // char *fileName = "C:\\temp\\test10.fab";

    // CFileDialog dlg2(FALSE, "printer", (LPCTSTR)"C:\\", NULL,
	//		"Export Printer Configuration File (*.printer)|*.printer|All Files (*.*)|*.*||", 0);
	//dlg2.GetOFN().lpstrTitle = "Export Printer Configuration File";
	//dlg2.GetOFN().Flags |= OFN_OVERWRITEPROMPT;
	//dlg2.DoModal();
   //  if(dlg2.DoModal()==IDOK)
//	{
		//if(!SaveParameters(dlg.GetPathName()))
		//	pApp->Log("Unable to save printer configuration file.");
		//else
		//	pApp->Log("Printer configuration file saved successfully.");
//	}

	file.SaveFile(fileName);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FABOutput::addTool(int tool)
{
	stringstream ss;
	
	//Create elements of the tool.
	TiXmlElement tool_("materialCalibration");

	TiXmlElement name("name");
	ss.str("");
	ss << tools[tool]->GetName();
	name.InsertEndChild(TiXmlText(ss.str().c_str()));
	
	TiXmlElement pathSpeed("pathSpeed");
	ss.str("");
	ss << tools[tool]->pathspeed;
	pathSpeed.InsertEndChild(TiXmlText(ss.str().c_str()));

     TiXmlElement pathWidth("pathWidth");
	ss.str("");
	ss << tools[tool]->pathwidth;
	pathWidth.InsertEndChild(TiXmlText(ss.str().c_str()));
	
	TiXmlElement depositionRate("depositionRate");
	ss.str("");
	ss << tools[tool]->disprate;
	depositionRate.InsertEndChild(TiXmlText(ss.str().c_str()));

	TiXmlElement pushout("pushout");
	ss.str("");
	ss << tools[tool]->pushout;
	pushout.InsertEndChild(TiXmlText(ss.str().c_str()));

	TiXmlElement suckback("suckback");
	ss.str("");
	ss << tools[tool]->suckback;
	suckback.InsertEndChild(TiXmlText(ss.str().c_str()));

	TiXmlElement suckbackDelay("suckbackDelay");
	ss.str("");
	ss << tools[tool]->suckbackDelay;
	suckbackDelay.InsertEndChild(TiXmlText(ss.str().c_str()));

	TiXmlElement clearance("clearance");
	ss.str("");
	ss << tools[tool]->clearance;
	clearance.InsertEndChild(TiXmlText(ss.str().c_str()));

	TiXmlElement pausePaths("pausePaths");
	ss.str("");
	ss << tools[tool]->GetPausePathCount();
	pausePaths.InsertEndChild(TiXmlText(ss.str().c_str()));

	TiXmlElement pitch("pitch");
	ss.str("");
	ss << tools[tool]->mmps;
	pitch.InsertEndChild(TiXmlText(ss.str().c_str()));

	//Add elements to the tool.	
	tool_.InsertEndChild(TiXmlComment("Name of the material calibration."));
	tool_.InsertEndChild(name);
	tool_.InsertEndChild(TiXmlComment("Top speed in millimeters/second for this tool during execution of path."));
	tool_.InsertEndChild(pathSpeed);
	tool_.InsertEndChild(TiXmlComment("Width of the path in millimeters."));
	tool_.InsertEndChild(pathWidth);
	tool_.InsertEndChild(TiXmlComment("(millimeters of plunger motion)/(millimeters tool travel) along deposition path."));
	tool_.InsertEndChild(depositionRate);
	tool_.InsertEndChild(TiXmlComment("Seconds of early dispensing to start flow quickly."));
	tool_.InsertEndChild(pushout);
	tool_.InsertEndChild(TiXmlComment("Seconds reverse plunger motion to stop flow quickly."));
	tool_.InsertEndChild(suckback);
	tool_.InsertEndChild(TiXmlComment("Seconds to delay suckback by."));
	tool_.InsertEndChild(suckbackDelay);
	tool_.InsertEndChild(TiXmlComment("Millimeters of clearance between tip and last layer when traversing."));
	tool_.InsertEndChild(clearance);
	tool_.InsertEndChild(TiXmlComment("Number of paths after which to trigger an automatic pause."));
	tool_.InsertEndChild(pausePaths);
	tool_.InsertEndChild(TiXmlComment("no comment"));
	tool_.InsertEndChild(pitch);
	
	//Add the tool to the file.
	TiXmlNode* fabAtHomePrinter(file.FirstChild("fabAtHomePrinter"));
	fabAtHomePrinter->InsertEndChild(tool_);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//adds all tools
void FABOutput::addConstants()
{
	for(int i=0; i<loadedTools; i++)
	{
		addTool(i);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FABOutput::addLayer(CLayer layer, CChunk chunk, int num) {

	stringstream ss;

	for (int r=0; r < layer.region.GetCount(); r++) {

		CRegion region = layer.region.GetAt(r);

		for (int p=0; p < region.path.GetSize(); p++)
		{
			//Create elements of the path.
			TiXmlElement path_("path");	

			TiXmlElement executeToolName("materialCalibrationName");
			for(int i=0; i<loadedTools; i++)
			{
				if ((tools[i]!=NULL) && (chunk.toolname.Compare(tools[i]->GetName())==0))
				{
					toolSelected = i;
				}
			}
			ss.str("");
			ss << tools[toolSelected]->GetName();
			executeToolName.InsertEndChild(TiXmlText(ss.str().c_str()));

			//Add elements to the path.
			path_.InsertEndChild(TiXmlComment("The name of the material calibration used during execution of this path."));
			path_.InsertEndChild(executeToolName);


			CPath path = region.path.GetAt(p);
			bool finished = false;
			for (int i = 0; i < path.v.GetSize(); ++i)
			{
				CVec vec = path.v.GetAt(i);
				//Create and add point to the path.
				TiXmlElement point("point");
				
				TiXmlElement x("x");
				ss.str("");
				ss << vec.x;
				x.InsertEndChild(TiXmlText(ss.str().c_str()));
				
				TiXmlElement y("y");
				ss.str("");
				ss << vec.y;
				y.InsertEndChild(TiXmlText(ss.str().c_str()));
				
				TiXmlElement z("z");
				ss.str("");
				ss << vec.z;
				z.InsertEndChild(TiXmlText(ss.str().c_str()));
								
				point.InsertEndChild(x);
				point.InsertEndChild(y);
				point.InsertEndChild(z);
				path_.InsertEndChild(point);

				if(finished)
				{
					break;
				}
				if(i == path.v.GetSize() - 1 && path.IsClosed())
				{
					i = -1;
					finished = true;
				}
			}

			//Add the path to the file.
			TiXmlNode* fabAtHomePrinter(file.FirstChild("fabAtHomePrinter"));
			fabAtHomePrinter->InsertEndChild(path_);
			
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////