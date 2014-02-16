#include "stdafx.h"
#include "Form1.h"

[STAThread]

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
     System::Threading::Thread::CurrentThread->ApartmentState = System::Threading::ApartmentState::STA;
     Util::runTestCases(); //Test helper functions for bugs.
	Application::Run(new Form1());
	return 0;
}
