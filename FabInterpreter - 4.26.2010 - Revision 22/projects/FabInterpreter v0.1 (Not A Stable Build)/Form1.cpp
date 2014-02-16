#include "stdafx.h"
#include "Form1.h"

#include <map>
#include <utility>
#include <list>

static map<string, string, LessThanString> commandMap; //A map from the commands to their paramters
static const char* kCommandStrings[] = { "--config", "--fabfile" };
static const int kNumberOfCommands = sizeof(kCommandStrings)/sizeof(const char*);

/**
 * The command line can look like this:
 * "--config=<C:\path\to\config\file that can\include\spaces.config>" --fabfile=<..\relative\path\no\spaces.fab>
 */
void parseCommandLine(string command_line) {

  // Map of command index to its location in the string
  map<int, size_t> command_locations;

  for (int i = 0; i < kNumberOfCommands; ++i) {
    string command_string(kCommandStrings[i]);
    size_t location = command_line.find(command_string);
    if (location != command_line.npos) {
      command_locations.insert(map<int,size_t>::value_type(i, location));
    }
  }

  // Parse each command
  for (map<int, size_t>::iterator i = command_locations.begin(); i != command_locations.end(); ++i) {
    const char* command = kCommandStrings[i->first];
    std::string mask;
    mask.append(command);
    mask.append("=<%[^>]>");
    char* value_buffer = new char[command_line.length()+1];
    int fields = sscanf_s(command_line.c_str() + i->second, mask.c_str(), value_buffer, command_line.length()+1);
    if (fields == 1) {
      commandMap.insert(map<string,string,LessThanString>::value_type(command, value_buffer));
    }
    delete value_buffer;
  }
}

[STAThread]



int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	System::Threading::Thread::CurrentThread->ApartmentState = System::Threading::ApartmentState::STA;
	Util::runTestCases(); //Test helper functions for bugs.

	//store the commands into a string
  parseCommandLine(lpCmdLine);

	ofstream myfile;
	myfile.open ("commands.txt");
  myfile << "command-line:  " << lpCmdLine << "\n";
	myfile << "Commands with paramters.\n";

	for( map<string,string,LessThanString>::iterator i = commandMap.begin(); i != commandMap.end(); ++i)
	{
		myfile << (*i).first << ": " << (*i).second << endl;
	}
	myfile.close();

	//opens Form1 (main interface)
  Application::Run(new Form1(commandMap,
                             commandMap.find("--config") != commandMap.end(),
                             commandMap.find("--fabfile") != commandMap.end()));

	return 0;
}

class LessThanString
{
public:
	//Returns: true iff a comes before b lexicographically.
	bool operator()(const string& a, const string& b) const
	{
		return (a.compare(b) < 0);
	}
};
