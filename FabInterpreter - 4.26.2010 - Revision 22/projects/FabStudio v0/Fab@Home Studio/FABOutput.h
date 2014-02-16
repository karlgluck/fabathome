#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "tinyXml/tinyxml.h"
using namespace std;

class FABOutput
{
public:
	FABOutput(void);
	~FABOutput(void);
	void addLayer(CLayer layer, CChunk chunk,  int num); //Layer and layer num
	void addConstants();
	void addTool(int tool);

	int loadedTools;
	int maxTools;
	CTool *tools[2];
	int toolSelected; //array index of selected tool, starts at 0
	
private:
	TiXmlDocument file;
};
