#ifndef UTIL_H
#define UTIL_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include <math.h>
#include <map>
#include <vector>
#include <time.h>
#include <algorithm>
#include "tinyxml.h"
using namespace std;


#define PROJECT_NAMESPACE FabAtHome

namespace PROJECT_NAMESPACE
{
     class Util
     { 
          //ALGORITHMS///////////////////////////////////////////////////////////////
          //random_shuffle(<vector object>.begin(),<vector object>.end());
          
          //STRINGS//////////////////////////////////////////////////////////////////
          //Prefer string since it can be initialized with both string and const char*.  Cannot initialize const char* with string.
          //<string object>.c_str() converts to const char* from string
          //string varName(const char* text) converts to string from const char*
          //stringstream ss; ss << "A" << 2; ss << 'a' << 4.5f; ss.str(); returns the string "A2a4.5".  ss.str(string text) sets data in stream to text, use "" to clear stream
          //<string object a>.compare(<string object b>) returns negative iff a < b, 0 iff a = b, or positive iff a > b.
          //strcmp(const char* a, const char* b) returns negative iff a < b, 0 iff a = b, or positive iff a > b
          ////strol interprets string as an integer in a specified base
          
          //Requires: T is a datatype that the >> operator of istringstream supports.
          //Returns: true iff text can be interpreted as a value with datatype T
     	public: template<typename T> static bool isType(const string& text)
        {
               istringstream iss(text);
               T stringAsT;
               iss >> noskipws >> stringAsT;
               return !iss.fail() && iss.eof();
     	}

        //Requires: isType(text)
     	//Returns: text interpreted as a value with datatype T
     	public: template<typename T> static T toType(const string& text)
          {
               istringstream iss(text);
               T stringAsT;
               iss >> noskipws >> stringAsT;
               return stringAsT;
     	}

	     public: template<typename T> static T assertType(const string& text)
		{
			 Util::assertTrue(Util::isType<T>(text),text+" cannot be interpreted as the required datatype.",__LINE__,__FILE__);
			 return toType<T>(text);
		}

         public: template<typename T> static string toString(const T& t)
         {
               stringstream ss;
               ss.setf(ios::fixed,ios::floatfield);
               ss.precision(6);
               ss << t;
               return ss.str();
         }
         
         //Result does not contain "".
         public: static void split(string text, const string& delim, vector<string>& result)
         {
               for(int cutAt = text.find(delim); cutAt != text.npos; cutAt = text.find(delim))
               {
                    if(cutAt > 0)
                    {
                         result.push_back(text.substr(0,cutAt));
                    }
                    text = text.substr(cutAt+delim.length());
               }
               if(text.length() > 0)
               {
                    result.push_back(text);
               }
         }

        public: static wchar_t* stringToUnicode(string s)
        {
            DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, 0);
            wchar_t* pwText;
            pwText = new wchar_t[dwNum];
            MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, pwText, dwNum);
            return pwText;
        }
          //EXECUTION CONTROL////////////////////////////////////////////////////////
          //MessageBox(NULL, const char* text, const char* title, MB_OK); pauses execution of thread until a button is pressed.  Button codes can be found at http://msdn.microsoft.com/en-us/library/ms645505(VS.85).aspx
          //Sleep(int milliseconds) pauses execution of thread for specified time
          //exit(int status) terminates program, status is EXIT_SUCCESS or EXIT_FAILURE
          //system(string code) executes code as windows command script, sometimes wont work when run from ide but will work when run from executable

          public: static void messageBox(const string& message)
          {
               MessageBox(NULL,Util::stringToUnicode(message),Util::stringToUnicode(""),MB_OK);
          }

          public: static void handleFatalError(const string& errorMessage)
          {
               Util::messageBox("A fatal error occured.\n"+errorMessage+"\nPress OK to terminate program.");
               exit(EXIT_FAILURE);
          }

          //MISCELLANEOUS////////////////////////////////////////////////////////////
          //Returns: Time in seconds since 1/1/1970 12:00am
          public: static unsigned int secTime()
          {
               return (unsigned int)time(NULL);
          }
          //DEBUGGING////////////////////////////////////////////////////////////////
          //Pass in __LINE__ for lineNumber and __FILE__ for fileName.
     	public: static void assertTrue(const bool& condition, const string& errorMessage, const int& lineNumber, const string& fileName)
          {
               if(!condition)
               {
                    stringstream ss;
                    ss << "Argument on line " << lineNumber << " of file " << fileName << " was false but expected to be true." << "\n" << errorMessage;
                    handleFatalError(ss.str());
               }
     	}

     	public: static void runTestCases()
     	{
     	     Util::assertTrue(Util::isType<int>("-1000"),"An error occured while testing utility functions.",__LINE__,__FILE__);
               Util::assertTrue(Util::toType<int>("-1000") == -1000,"An error occured while testing utility functions.",__LINE__,__FILE__);
               Util::assertTrue(Util::isType<double>("-1000.5"),"An error occured while testing utility functions.",__LINE__,__FILE__);
               Util::assertTrue(Util::toType<double>("-1000.5") == -1000.5,"An error occured while testing utility functions.",__LINE__,__FILE__);
               Util::assertTrue(Util::isType<int>("001"),"An error occured while testing utility functions.",__LINE__,__FILE__);
               Util::assertTrue(!Util::isType<int>(""),"An error occured while testing utility functions.",__LINE__,__FILE__);
               Util::assertTrue(!Util::isType<int>("0 "),"An error occured while testing utility functions.",__LINE__,__FILE__);
               Util::assertTrue(!Util::isType<int>(" 0"),"An error occured while testing utility functions.",__LINE__,__FILE__);
               Util::assertTrue(!Util::isType<int>("zero"),"An error occured while testing utility functions.",__LINE__,__FILE__);
               Util::assertTrue(!Util::isType<int>("1 5"),"An error occured while testing utility functions.",__LINE__,__FILE__);
     	}
     	//MATH//////////////////////////////////////////////////////////////////////
     	//pow(double base, double exp) returns base^exp
     	//abs(arg) returns absolute value of arg
     	
     	//Requires: a <= b
     	//Returns: A random number in [a,b].
     	public: static double randomDouble(const double& a, const double& b)
     	{
               static bool seeded = false;
               if(!seeded)
               {
                    srand((unsigned int)time(NULL));
                    seeded = true;
               }
               double uniform = (1.0*rand())/RAND_MAX;
               return a+(b-a)*uniform;
          }
          
          //Requires: a <= b
          //Returns: A random number in {a,a+1,a+2,...,b}
          public: static int randomInt(const int& a, const int& b)
          {
               return (int)floor(randomDouble(a,b+0.999999));
          }
     };
     //FILES/////////////////////////////////////////////////////////////////////////
     class MyFileWriter
     {
          private: fstream fileWriter;
                   
          //File is created if it does not already exist.  Escape \ in filePath with \.
          public: MyFileWriter(const string& filePath, const bool& append) :
          fileWriter(filePath.c_str(), append ? ios::out|ios::app : ios::out)
          {
          }

          public: void write(const string& text)
          {
               Util::assertTrue(fileWriter.is_open() && fileWriter.good(),"Cannot write to file.",__LINE__,__FILE__);
               fileWriter << text;
          }

          public: void close()
          {
               Util::assertTrue(fileWriter.is_open(),"Tried to close a file that is not open.",__LINE__,__FILE__);
               fileWriter.close();
               Util::assertTrue(!fileWriter.is_open(),"File is still open after it was closed.",__LINE__,__FILE__);
          }
     };

     class MyFileReader
     {
          private: fstream fileReader;
          
          //Escape \ in filePath with \.
          public: MyFileReader(const string& filePath) :
          fileReader(filePath.c_str(), ios::in)
          {
          }
          
          public: bool isOpen()
          {
               return fileReader.is_open();
          }
          
          public: bool hasNextLine()
          {
               return !fileReader.eof();
          }
          
          public: string readLine()
          {
               Util::assertTrue(isOpen() && fileReader.good() && hasNextLine(),"Cannot read line from file.",__LINE__,__FILE__);
               string currLine;
               getline(fileReader,currLine);
               return currLine;
          }
          
          public: void close()
          {
               Util::assertTrue(isOpen(),"Tried to close a file that is not open.",__LINE__,__FILE__);
               fileReader.close();
               Util::assertTrue(!isOpen(),"File is still open after it was closed.",__LINE__,__FILE__);
          }
     };
     //STRINGS///////////////////////////////////////////////////////////////////////
     class LessThanString
     {
	     public:
               //Returns: true iff a comes before b lexicographically.
		     bool operator()(const string& a, const string& b) const
		     {
			     return (a.compare(b) < 0);
		     }
     };
     //DLL/////////////////////////////////////////////////////////////////////////////
     class DLL
     {
          //Requires: dllName is the name of a DLL in the same directory as the executable.  functionName is the name of a function in the DLL.
          public: static FARPROC loadFunction(const string& dllName, const string& functionName)
          {
               //Load the dll.
               HINSTANCE dll = ::LoadLibrary(Util::stringToUnicode(dllName));
               Util::assertTrue(dll != NULL,"Could not load the DLL "+dllName,__LINE__,__FILE__);
               //Load the function.
               FARPROC function = ::GetProcAddress(dll,functionName.c_str());
               Util::assertTrue(function != NULL,"Could not load the function "+functionName+" from the DLL "+dllName,__LINE__,__FILE__);
               return function;
          }
     };
     //XML/////////////////////////////////////////////////////////////////////////////
     class XMLParser
     {
          //ex. "a 0\\b 3\\c 2" is a path to an element interpreted as follows: the root is named a, the 4th element child of a named b, the 3rd element child of b named c
           
          private:
               map<string,int> counts;
               map<string,string> texts;
               string filePath;
               TiXmlDocument file;

               void loadHelper(TiXmlElement* elem, const string& elemPath)
               {
                      string base = elemPath+" "+Util::toString(count(elemPath));
                      counts[elemPath] = count(elemPath)+1;
                      if(elem->FirstChild() != NULL && elem->FirstChild()->ToText() != NULL)
                      {
                           //Base case
                           texts[base] = elem->FirstChild()->ToText()->Value();
                      }
                      else
                      {
                           //Recursive case
                           for(TiXmlElement* child = elem->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
                           {
                                loadHelper(child,base+"\\"+child->Value());
                           }
                      }
               }
               
          public:
               string load(const string& filePath)
               {
                    if(!file.LoadFile(filePath.c_str()))
                    {
                         return "Could not open "+filePath+".";
                    }
                    else
                    {
                         TiXmlElement* root(file.RootElement());
                         if(root == NULL)
                         {
                                 return "Could not find root element.";
                         }
                         counts.clear();
                         texts.clear();
                         this->filePath = filePath;
                         loadHelper(root,root->Value());
                         return "";
                    }
               }
               
               //Returns: The text of elemPath or "" if elemPath is not text.
               string text(const string& elemPath)
               {
                    map<string,string>::iterator i = texts.find(elemPath);
                    if(i == texts.end())
                    {
                         return "";                         
                    }
                    else
                    {
                         return i->second;
                    }        
               }
               
               //ex. count("a 0\\b 3\\ c") is the number of element children of "a 0\\b 3" named c
               unsigned int count(const string& elemPath)
               {
                    map<string,int>::iterator i = counts.find(elemPath);
                    if(i == counts.end())
                    {
                         return 0;                         
                    }
                    else
                    {
                         return i->second;
                    }      
               }
               
     };

}

using namespace PROJECT_NAMESPACE;

#endif //ndef UTIL_H
