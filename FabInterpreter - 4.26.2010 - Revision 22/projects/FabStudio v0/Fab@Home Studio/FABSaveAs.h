#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Windows::Forms;

public ref class CExercise : public Form
{
	SaveFileDialog ^ sfd;
public:
	CExercise(void)
	{
		InitializeComponent();
		sfd = gcnew SaveFileDialog();
	}

private:
	void InitializeComponent()
	{
		this->Text = L"File Processing";
	}
};