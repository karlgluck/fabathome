#ifndef FORM1_H
#define FORM1_H


#include "FabAtHomePrinter.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;







//variables for controller
bool correctModule;


//thread safe variables
double posDisplayPeriod = 1; //Number of seconds between position updates in user interface.
bool enablePosDisplay;
string pathDisplayText;
int maxProgress = 0;

//timer variables
int numTicks = 0;


FabAtHomePrinter printer;



/// <summary> 
/// Summary for Form1
///
/// WARNING: If you change the name of this class, you will need to change the 
///          'Resource File Name' property for the managed resource compiler tool 
///          associated with all .resx files this class depends on.  Otherwise,
///          the designers will not be able to interact properly with localized
///          resources associated with this form.
/// </summary>
public __gc class Form1 : public System::Windows::Forms::Form
{	

public:
	Form1(void)
	{
		InitializeComponent();
	}

protected:
	void Dispose(Boolean disposing)
	{
		if (disposing && components)
		{
			components->Dispose();
		}
		__super::Dispose(disposing);
	}

private: System::Windows::Forms::Button*  InitButton;

private: System::Windows::Forms::Button*  MoveButton;

private: System::Windows::Forms::Button*  ExitButton;



private: System::Windows::Forms::TextBox*  XDisplay;
private: System::Windows::Forms::TextBox*  YDisplay;
private: System::Windows::Forms::TextBox*  ZDisplay;

private: System::Windows::Forms::Button*  ExecuteButton;
private: System::Windows::Forms::Label*  XLabel;
private: System::Windows::Forms::Label*  YLabel;
private: System::Windows::Forms::Label*  ZLabel;



private: System::Windows::Forms::Button*  ResetPosButton;
private: System::Windows::Forms::Button*  LoadFileButton;


public: System::Windows::Forms::TextBox*  ExecutePathLabel;
private: System::Windows::Forms::TextBox*  bayVelocity0;
public: 
























private: System::ComponentModel::BackgroundWorker*  FabricationThread;
private: System::Windows::Forms::TextBox*  bayVelocity1;





private: System::Windows::Forms::Button*  ForceStopButton;


private: System::Windows::Forms::Panel*  panel1;
private: System::Windows::Forms::Button*  PauseFabButton;

private: System::Windows::Forms::Button*  CancelFabButton;

private: System::Windows::Forms::Button*  RedoPathButton;
private: System::Windows::Forms::ProgressBar*  PathProgressBar;



private: System::Windows::Forms::Timer*  Timer;

private: System::Windows::Forms::Panel*  panel2;





private: System::ComponentModel::BackgroundWorker*  PosDisplayThread;
private: System::Windows::Forms::Label*  label2;
private: System::Windows::Forms::Label*  IncrementLabel;
private: System::Windows::Forms::TextBox*  ZIncrementField;
private: System::Windows::Forms::TextBox*  YIncrementField;

private: System::Windows::Forms::TextBox*  XIncrementField;
private: System::Windows::Forms::Label*  CommandedLabel;
private: System::Windows::Forms::Label*  RealTimeLabel;
private: System::Windows::Forms::TextBox*  ZCommandedField;
private: System::Windows::Forms::TextBox*  YCommandedField;
private: System::Windows::Forms::TextBox*  XCommandedField;
private: System::Windows::Forms::VScrollBar*  xScroll;
private: System::Windows::Forms::VScrollBar*  yScroll;
private: System::Windows::Forms::VScrollBar*  zScroll;
private: System::Windows::Forms::TextBox*  bayCommandedPosition1;
private: System::Windows::Forms::TextBox*  bayCommandedPosition0;









private: System::Windows::Forms::Label*  bayLabel0;
private: System::Windows::Forms::Label*  bayLabel1;
private: System::Windows::Forms::VScrollBar*  bayScroll1;









private: System::Windows::Forms::VScrollBar*  bayScroll0;



private: System::Windows::Forms::Label*  label4;
private: System::Windows::Forms::Panel*  panel3;
private: System::Windows::Forms::Panel*  panel4;
private: System::Windows::Forms::TextBox*  bayPositionIncrement1;


private: System::Windows::Forms::TextBox*  bayPositionIncrement0;
private: System::Windows::Forms::TextBox*  bayMotorPosition1;


private: System::Windows::Forms::TextBox*  bayMotorPosition0;
private: System::Windows::Forms::TextBox*  XVelocityField;



private: System::Windows::Forms::TextBox*  YVelocityField;
private: System::Windows::Forms::TextBox*  ZVelocityField;
private: System::Windows::Forms::TextBox*  XAccelerationField;
private: System::Windows::Forms::TextBox*  YAccelerationField;
private: System::Windows::Forms::TextBox*  ZAccelerationField;
private: System::Windows::Forms::TextBox*  bayAcceleration1;

private: System::Windows::Forms::TextBox*  bayAcceleration0;






private: System::Windows::Forms::Label*  label1;
private: System::Windows::Forms::ComboBox*  bayMaterialCalibration1;

private: System::Windows::Forms::ComboBox*  bayMaterialCalibration0;

private: System::Windows::Forms::Label*  label5;
private: System::Windows::Forms::ComboBox*  bayMaterialCalibration2;
private: System::Windows::Forms::TextBox*  bayAcceleration2;
private: System::Windows::Forms::TextBox*  bayVelocity2;
private: System::Windows::Forms::TextBox*  bayMotorPosition2;
private: System::Windows::Forms::TextBox*  bayPositionIncrement2;
private: System::Windows::Forms::VScrollBar*  bayScroll2;

private: System::Windows::Forms::TextBox*  bayCommandedPosition2;
private: System::Windows::Forms::Label*  bayLabel2;
private: System::Windows::Forms::Panel*  fs;
private: System::Windows::Forms::OpenFileDialog*  openFileDialog1;























private: System::ComponentModel::IContainer*  components;




private:
	/// <summary>
	/// Required designer variable.
	/// </summary>


	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	void InitializeComponent(void)
	{
          this->components = (new System::ComponentModel::Container());
          this->InitButton = (new System::Windows::Forms::Button());
          this->MoveButton = (new System::Windows::Forms::Button());
          this->ExitButton = (new System::Windows::Forms::Button());
          this->XLabel = (new System::Windows::Forms::Label());
          this->YLabel = (new System::Windows::Forms::Label());
          this->ZLabel = (new System::Windows::Forms::Label());
          this->XDisplay = (new System::Windows::Forms::TextBox());
          this->YDisplay = (new System::Windows::Forms::TextBox());
          this->ZDisplay = (new System::Windows::Forms::TextBox());
          this->ExecuteButton = (new System::Windows::Forms::Button());
          this->ResetPosButton = (new System::Windows::Forms::Button());
          this->LoadFileButton = (new System::Windows::Forms::Button());
          this->ExecutePathLabel = (new System::Windows::Forms::TextBox());
          this->bayVelocity0 = (new System::Windows::Forms::TextBox());
          this->label4 = (new System::Windows::Forms::Label());
          this->bayVelocity1 = (new System::Windows::Forms::TextBox());
          this->bayScroll1 = (new System::Windows::Forms::VScrollBar());
          this->bayScroll0 = (new System::Windows::Forms::VScrollBar());
          this->bayLabel1 = (new System::Windows::Forms::Label());
          this->bayLabel0 = (new System::Windows::Forms::Label());
          this->bayCommandedPosition1 = (new System::Windows::Forms::TextBox());
          this->bayCommandedPosition0 = (new System::Windows::Forms::TextBox());
          this->FabricationThread = (new System::ComponentModel::BackgroundWorker());
          this->ForceStopButton = (new System::Windows::Forms::Button());
          this->panel1 = (new System::Windows::Forms::Panel());
          this->PathProgressBar = (new System::Windows::Forms::ProgressBar());
          this->CancelFabButton = (new System::Windows::Forms::Button());
          this->PauseFabButton = (new System::Windows::Forms::Button());
          this->RedoPathButton = (new System::Windows::Forms::Button());
          this->Timer = (new System::Windows::Forms::Timer(this->components));
          this->panel2 = (new System::Windows::Forms::Panel());
          this->bayMaterialCalibration2 = (new System::Windows::Forms::ComboBox());
          this->bayAcceleration2 = (new System::Windows::Forms::TextBox());
          this->bayVelocity2 = (new System::Windows::Forms::TextBox());
          this->bayMotorPosition2 = (new System::Windows::Forms::TextBox());
          this->bayPositionIncrement2 = (new System::Windows::Forms::TextBox());
          this->bayScroll2 = (new System::Windows::Forms::VScrollBar());
          this->bayCommandedPosition2 = (new System::Windows::Forms::TextBox());
          this->bayLabel2 = (new System::Windows::Forms::Label());
          this->label5 = (new System::Windows::Forms::Label());
          this->bayMaterialCalibration1 = (new System::Windows::Forms::ComboBox());
          this->bayMaterialCalibration0 = (new System::Windows::Forms::ComboBox());
          this->XAccelerationField = (new System::Windows::Forms::TextBox());
          this->YAccelerationField = (new System::Windows::Forms::TextBox());
          this->ZAccelerationField = (new System::Windows::Forms::TextBox());
          this->bayAcceleration1 = (new System::Windows::Forms::TextBox());
          this->bayAcceleration0 = (new System::Windows::Forms::TextBox());
          this->label1 = (new System::Windows::Forms::Label());
          this->XVelocityField = (new System::Windows::Forms::TextBox());
          this->YVelocityField = (new System::Windows::Forms::TextBox());
          this->ZVelocityField = (new System::Windows::Forms::TextBox());
          this->bayMotorPosition1 = (new System::Windows::Forms::TextBox());
          this->bayMotorPosition0 = (new System::Windows::Forms::TextBox());
          this->bayPositionIncrement1 = (new System::Windows::Forms::TextBox());
          this->bayPositionIncrement0 = (new System::Windows::Forms::TextBox());
          this->zScroll = (new System::Windows::Forms::VScrollBar());
          this->yScroll = (new System::Windows::Forms::VScrollBar());
          this->xScroll = (new System::Windows::Forms::VScrollBar());
          this->CommandedLabel = (new System::Windows::Forms::Label());
          this->RealTimeLabel = (new System::Windows::Forms::Label());
          this->ZIncrementField = (new System::Windows::Forms::TextBox());
          this->YIncrementField = (new System::Windows::Forms::TextBox());
          this->XIncrementField = (new System::Windows::Forms::TextBox());
          this->label2 = (new System::Windows::Forms::Label());
          this->IncrementLabel = (new System::Windows::Forms::Label());
          this->ZCommandedField = (new System::Windows::Forms::TextBox());
          this->YCommandedField = (new System::Windows::Forms::TextBox());
          this->XCommandedField = (new System::Windows::Forms::TextBox());
          this->PosDisplayThread = (new System::ComponentModel::BackgroundWorker());
          this->panel3 = (new System::Windows::Forms::Panel());
          this->panel4 = (new System::Windows::Forms::Panel());
          this->fs = (new System::Windows::Forms::Panel());
          this->openFileDialog1 = (new System::Windows::Forms::OpenFileDialog());
          this->panel1->SuspendLayout();
          this->panel2->SuspendLayout();
          this->panel3->SuspendLayout();
          this->panel4->SuspendLayout();
          this->fs->SuspendLayout();
          this->SuspendLayout();
          // 
          // InitButton
          // 
          this->InitButton->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
               (System::Byte)0));
          this->InitButton->Location = System::Drawing::Point(0, 0);
          this->InitButton->Name = S"InitButton";
          this->InitButton->Size = System::Drawing::Size(115, 50);
          this->InitButton->TabIndex = 0;
          this->InitButton->Text = S"Initialize";
          this->InitButton->Click += new System::EventHandler(this, &Form1::InitButton_Click);
          // 
          // MoveButton
          // 
          this->MoveButton->Enabled = false;
          this->MoveButton->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
               (System::Byte)0));
          this->MoveButton->Location = System::Drawing::Point(307, 184);
          this->MoveButton->Name = S"MoveButton";
          this->MoveButton->Size = System::Drawing::Size(201, 48);
          this->MoveButton->TabIndex = 3;
          this->MoveButton->Text = S"Move To Position ";
          this->MoveButton->Click += new System::EventHandler(this, &Form1::MoveButton_Click);
          // 
          // ExitButton
          // 
          this->ExitButton->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
               (System::Byte)0));
          this->ExitButton->Location = System::Drawing::Point(0, 69);
          this->ExitButton->Name = S"ExitButton";
          this->ExitButton->Size = System::Drawing::Size(164, 70);
          this->ExitButton->TabIndex = 5;
          this->ExitButton->Text = S"Exit";
          this->ExitButton->Click += new System::EventHandler(this, &Form1::ExitButton_Click);
          // 
          // XLabel
          // 
          this->XLabel->Location = System::Drawing::Point(8, 31);
          this->XLabel->Name = S"XLabel";
          this->XLabel->Size = System::Drawing::Size(63, 16);
          this->XLabel->TabIndex = 7;
          this->XLabel->Text = S"Not Loaded";
          this->XLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
          // 
          // YLabel
          // 
          this->YLabel->Location = System::Drawing::Point(8, 57);
          this->YLabel->Name = S"YLabel";
          this->YLabel->Size = System::Drawing::Size(63, 16);
          this->YLabel->TabIndex = 8;
          this->YLabel->Text = S"Not Loaded";
          this->YLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
          // 
          // ZLabel
          // 
          this->ZLabel->Location = System::Drawing::Point(8, 82);
          this->ZLabel->Name = S"ZLabel";
          this->ZLabel->Size = System::Drawing::Size(63, 16);
          this->ZLabel->TabIndex = 11;
          this->ZLabel->Text = S"Not Loaded";
          this->ZLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
          // 
          // XDisplay
          // 
          this->XDisplay->Enabled = false;
          this->XDisplay->Location = System::Drawing::Point(429, 30);
          this->XDisplay->Name = S"XDisplay";
          this->XDisplay->ReadOnly = true;
          this->XDisplay->Size = System::Drawing::Size(100, 20);
          this->XDisplay->TabIndex = 13;
          this->XDisplay->Text = S"0.000000";
          this->XDisplay->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // YDisplay
          // 
          this->YDisplay->Enabled = false;
          this->YDisplay->Location = System::Drawing::Point(429, 56);
          this->YDisplay->Name = S"YDisplay";
          this->YDisplay->ReadOnly = true;
          this->YDisplay->Size = System::Drawing::Size(100, 20);
          this->YDisplay->TabIndex = 14;
          this->YDisplay->Text = S"0.000000";
          this->YDisplay->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // ZDisplay
          // 
          this->ZDisplay->Enabled = false;
          this->ZDisplay->Location = System::Drawing::Point(429, 83);
          this->ZDisplay->Name = S"ZDisplay";
          this->ZDisplay->ReadOnly = true;
          this->ZDisplay->Size = System::Drawing::Size(100, 20);
          this->ZDisplay->TabIndex = 17;
          this->ZDisplay->Text = S"0.000000";
          this->ZDisplay->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // ExecuteButton
          // 
          this->ExecuteButton->Enabled = false;
          this->ExecuteButton->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
               (System::Byte)0));
          this->ExecuteButton->Location = System::Drawing::Point(123, 0);
          this->ExecuteButton->Name = S"ExecuteButton";
          this->ExecuteButton->Size = System::Drawing::Size(117, 40);
          this->ExecuteButton->TabIndex = 19;
          this->ExecuteButton->Text = S"Execute";
          this->ExecuteButton->Click += new System::EventHandler(this, &Form1::ExecuteButton_Click);
          // 
          // ResetPosButton
          // 
          this->ResetPosButton->Enabled = false;
          this->ResetPosButton->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
               (System::Byte)0));
          this->ResetPosButton->Location = System::Drawing::Point(514, 184);
          this->ResetPosButton->Name = S"ResetPosButton";
          this->ResetPosButton->Size = System::Drawing::Size(168, 48);
          this->ResetPosButton->TabIndex = 20;
          this->ResetPosButton->Text = S"Reset Position";
          this->ResetPosButton->Click += new System::EventHandler(this, &Form1::ResetPosButton_Click);
          // 
          // LoadFileButton
          // 
          this->LoadFileButton->Enabled = false;
          this->LoadFileButton->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
               (System::Byte)0));
          this->LoadFileButton->Location = System::Drawing::Point(0, 0);
          this->LoadFileButton->Name = S"LoadFileButton";
          this->LoadFileButton->Size = System::Drawing::Size(117, 41);
          this->LoadFileButton->TabIndex = 21;
          this->LoadFileButton->Text = S"Load";
          this->LoadFileButton->Click += new System::EventHandler(this, &Form1::LoadFileButton_Click);
          // 
          // ExecutePathLabel
          // 
          this->ExecutePathLabel->Location = System::Drawing::Point(247, 10);
          this->ExecutePathLabel->Name = S"ExecutePathLabel";
          this->ExecutePathLabel->ReadOnly = true;
          this->ExecutePathLabel->Size = System::Drawing::Size(458, 20);
          this->ExecutePathLabel->TabIndex = 24;
          // 
          // bayVelocity0
          // 
          this->bayVelocity0->Enabled = false;
          this->bayVelocity0->Location = System::Drawing::Point(77, 106);
          this->bayVelocity0->Name = S"bayVelocity0";
          this->bayVelocity0->Size = System::Drawing::Size(100, 20);
          this->bayVelocity0->TabIndex = 28;
          this->bayVelocity0->Text = S"1.000000";
          this->bayVelocity0->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // label4
          // 
          this->label4->AutoSize = true;
          this->label4->Location = System::Drawing::Point(74, 12);
          this->label4->Name = S"label4";
          this->label4->Size = System::Drawing::Size(79, 13);
          this->label4->TabIndex = 47;
          this->label4->Text = S"Velocity (mm/s)";
          // 
          // bayVelocity1
          // 
          this->bayVelocity1->Enabled = false;
          this->bayVelocity1->Location = System::Drawing::Point(77, 132);
          this->bayVelocity1->Name = S"bayVelocity1";
          this->bayVelocity1->Size = System::Drawing::Size(100, 20);
          this->bayVelocity1->TabIndex = 32;
          this->bayVelocity1->Text = S"1.000000";
          this->bayVelocity1->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // bayScroll1
          // 
          this->bayScroll1->AccessibleRole = System::Windows::Forms::AccessibleRole::ScrollBar;
          this->bayScroll1->Enabled = false;
          this->bayScroll1->LargeChange = 1;
          this->bayScroll1->Location = System::Drawing::Point(663, 132);
          this->bayScroll1->Maximum = 100000000;
          this->bayScroll1->Minimum = -100000000;
          this->bayScroll1->Name = S"bayScroll1";
          this->bayScroll1->Size = System::Drawing::Size(19, 20);
          this->bayScroll1->TabIndex = 46;
          this->bayScroll1->Scroll += new System::Windows::Forms::ScrollEventHandler(this, &Form1::Tool2Scroll_Scroll);
          // 
          // bayScroll0
          // 
          this->bayScroll0->AccessibleRole = System::Windows::Forms::AccessibleRole::ScrollBar;
          this->bayScroll0->Enabled = false;
          this->bayScroll0->LargeChange = 1;
          this->bayScroll0->Location = System::Drawing::Point(663, 106);
          this->bayScroll0->Maximum = 100000000;
          this->bayScroll0->Minimum = -100000000;
          this->bayScroll0->Name = S"bayScroll0";
          this->bayScroll0->Size = System::Drawing::Size(19, 20);
          this->bayScroll0->TabIndex = 45;
          this->bayScroll0->Scroll += new System::Windows::Forms::ScrollEventHandler(this, &Form1::Tool1Scroll_Scroll);
          // 
          // bayLabel1
          // 
          this->bayLabel1->Anchor = System::Windows::Forms::AnchorStyles::Right;
          this->bayLabel1->Location = System::Drawing::Point(8, 133);
          this->bayLabel1->Name = S"bayLabel1";
          this->bayLabel1->Size = System::Drawing::Size(63, 16);
          this->bayLabel1->TabIndex = 36;
          this->bayLabel1->Text = S"Not Loaded";
          this->bayLabel1->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
          // 
          // bayLabel0
          // 
          this->bayLabel0->Anchor = System::Windows::Forms::AnchorStyles::Right;
          this->bayLabel0->Location = System::Drawing::Point(8, 107);
          this->bayLabel0->Name = S"bayLabel0";
          this->bayLabel0->Size = System::Drawing::Size(63, 16);
          this->bayLabel0->TabIndex = 35;
          this->bayLabel0->Text = S"Not Loaded";
          this->bayLabel0->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
          // 
          // bayCommandedPosition1
          // 
          this->bayCommandedPosition1->Enabled = false;
          this->bayCommandedPosition1->Location = System::Drawing::Point(535, 132);
          this->bayCommandedPosition1->Name = S"bayCommandedPosition1";
          this->bayCommandedPosition1->Size = System::Drawing::Size(128, 20);
          this->bayCommandedPosition1->TabIndex = 34;
          this->bayCommandedPosition1->Text = S"0.000000";
          this->bayCommandedPosition1->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // bayCommandedPosition0
          // 
          this->bayCommandedPosition0->Enabled = false;
          this->bayCommandedPosition0->Location = System::Drawing::Point(535, 106);
          this->bayCommandedPosition0->Name = S"bayCommandedPosition0";
          this->bayCommandedPosition0->Size = System::Drawing::Size(128, 20);
          this->bayCommandedPosition0->TabIndex = 33;
          this->bayCommandedPosition0->Text = S"0.000000";
          this->bayCommandedPosition0->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // FabricationThread
          // 
          this->FabricationThread->WorkerReportsProgress = true;
          this->FabricationThread->DoWork += new System::ComponentModel::DoWorkEventHandler(this, &Form1::FabricationThread_DoWork);
          this->FabricationThread->RunWorkerCompleted += new System::ComponentModel::RunWorkerCompletedEventHandler(this, &Form1::FabricationThread_WorkerCompleted);
          this->FabricationThread->ProgressChanged += new System::ComponentModel::ProgressChangedEventHandler(this, &Form1::FabricationThread_ProgressChanged);
          // 
          // ForceStopButton
          // 
          this->ForceStopButton->Enabled = false;
          this->ForceStopButton->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
               (System::Byte)0));
          this->ForceStopButton->Location = System::Drawing::Point(473, 3);
          this->ForceStopButton->Name = S"ForceStopButton";
          this->ForceStopButton->Size = System::Drawing::Size(231, 46);
          this->ForceStopButton->TabIndex = 34;
          this->ForceStopButton->Text = S"Force Stop";
          this->ForceStopButton->UseVisualStyleBackColor = true;
          this->ForceStopButton->Click += new System::EventHandler(this, &Form1::ForceStopButton_Click);
          // 
          // panel1
          // 
          this->panel1->Controls->Add(this->PathProgressBar);
          this->panel1->Controls->Add(this->CancelFabButton);
          this->panel1->Controls->Add(this->PauseFabButton);
          this->panel1->Controls->Add(this->ForceStopButton);
          this->panel1->Location = System::Drawing::Point(9, 364);
          this->panel1->Name = S"panel1";
          this->panel1->Size = System::Drawing::Size(717, 92);
          this->panel1->TabIndex = 37;
          // 
          // PathProgressBar
          // 
          this->PathProgressBar->Location = System::Drawing::Point(5, 55);
          this->PathProgressBar->Maximum = 1000;
          this->PathProgressBar->Name = S"PathProgressBar";
          this->PathProgressBar->Size = System::Drawing::Size(697, 23);
          this->PathProgressBar->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
          this->PathProgressBar->TabIndex = 39;
          // 
          // CancelFabButton
          // 
          this->CancelFabButton->Enabled = false;
          this->CancelFabButton->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
               (System::Byte)0));
          this->CancelFabButton->Location = System::Drawing::Point(246, 3);
          this->CancelFabButton->Name = S"CancelFabButton";
          this->CancelFabButton->Size = System::Drawing::Size(219, 46);
          this->CancelFabButton->TabIndex = 39;
          this->CancelFabButton->Text = S"Cancel";
          this->CancelFabButton->UseVisualStyleBackColor = true;
          this->CancelFabButton->Click += new System::EventHandler(this, &Form1::CancelFabButton_Click);
          // 
          // PauseFabButton
          // 
          this->PauseFabButton->Enabled = false;
          this->PauseFabButton->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
               (System::Byte)0));
          this->PauseFabButton->Location = System::Drawing::Point(5, 3);
          this->PauseFabButton->Name = S"PauseFabButton";
          this->PauseFabButton->Size = System::Drawing::Size(235, 46);
          this->PauseFabButton->TabIndex = 38;
          this->PauseFabButton->Text = S"Pause";
          this->PauseFabButton->UseVisualStyleBackColor = true;
          this->PauseFabButton->Click += new System::EventHandler(this, &Form1::PauseFabButton_Click);
          // 
          // RedoPathButton
          // 
          this->RedoPathButton->Enabled = false;
          this->RedoPathButton->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
               (System::Byte)0));
          this->RedoPathButton->Location = System::Drawing::Point(0, 0);
          this->RedoPathButton->Name = S"RedoPathButton";
          this->RedoPathButton->Size = System::Drawing::Size(164, 63);
          this->RedoPathButton->TabIndex = 38;
          this->RedoPathButton->Text = S"Redo Path";
          this->RedoPathButton->UseVisualStyleBackColor = true;
          this->RedoPathButton->Click += new System::EventHandler(this, &Form1::RedoPathButton_Click);
          // 
          // Timer
          // 
          this->Timer->Tick += new System::EventHandler(this, &Form1::Timer_Tick);
          // 
          // panel2
          // 
          this->panel2->Controls->Add(this->bayMaterialCalibration2);
          this->panel2->Controls->Add(this->bayAcceleration2);
          this->panel2->Controls->Add(this->bayVelocity2);
          this->panel2->Controls->Add(this->bayMotorPosition2);
          this->panel2->Controls->Add(this->bayPositionIncrement2);
          this->panel2->Controls->Add(this->ResetPosButton);
          this->panel2->Controls->Add(this->MoveButton);
          this->panel2->Controls->Add(this->bayScroll2);
          this->panel2->Controls->Add(this->bayCommandedPosition2);
          this->panel2->Controls->Add(this->bayLabel2);
          this->panel2->Controls->Add(this->label5);
          this->panel2->Controls->Add(this->bayMaterialCalibration1);
          this->panel2->Controls->Add(this->bayMaterialCalibration0);
          this->panel2->Controls->Add(this->XAccelerationField);
          this->panel2->Controls->Add(this->YAccelerationField);
          this->panel2->Controls->Add(this->ZAccelerationField);
          this->panel2->Controls->Add(this->bayAcceleration1);
          this->panel2->Controls->Add(this->bayAcceleration0);
          this->panel2->Controls->Add(this->label1);
          this->panel2->Controls->Add(this->XVelocityField);
          this->panel2->Controls->Add(this->YVelocityField);
          this->panel2->Controls->Add(this->ZVelocityField);
          this->panel2->Controls->Add(this->bayVelocity1);
          this->panel2->Controls->Add(this->label4);
          this->panel2->Controls->Add(this->bayVelocity0);
          this->panel2->Controls->Add(this->bayMotorPosition1);
          this->panel2->Controls->Add(this->bayMotorPosition0);
          this->panel2->Controls->Add(this->bayPositionIncrement1);
          this->panel2->Controls->Add(this->bayPositionIncrement0);
          this->panel2->Controls->Add(this->bayScroll1);
          this->panel2->Controls->Add(this->zScroll);
          this->panel2->Controls->Add(this->bayScroll0);
          this->panel2->Controls->Add(this->yScroll);
          this->panel2->Controls->Add(this->bayCommandedPosition1);
          this->panel2->Controls->Add(this->xScroll);
          this->panel2->Controls->Add(this->bayCommandedPosition0);
          this->panel2->Controls->Add(this->bayLabel1);
          this->panel2->Controls->Add(this->CommandedLabel);
          this->panel2->Controls->Add(this->bayLabel0);
          this->panel2->Controls->Add(this->RealTimeLabel);
          this->panel2->Controls->Add(this->ZIncrementField);
          this->panel2->Controls->Add(this->YIncrementField);
          this->panel2->Controls->Add(this->XIncrementField);
          this->panel2->Controls->Add(this->label2);
          this->panel2->Controls->Add(this->IncrementLabel);
          this->panel2->Controls->Add(this->ZCommandedField);
          this->panel2->Controls->Add(this->YCommandedField);
          this->panel2->Controls->Add(this->XCommandedField);
          this->panel2->Controls->Add(this->XLabel);
          this->panel2->Controls->Add(this->YLabel);
          this->panel2->Controls->Add(this->ZLabel);
          this->panel2->Controls->Add(this->XDisplay);
          this->panel2->Controls->Add(this->YDisplay);
          this->panel2->Controls->Add(this->ZDisplay);
          this->panel2->Location = System::Drawing::Point(9, 63);
          this->panel2->Name = S"panel2";
          this->panel2->Size = System::Drawing::Size(887, 248);
          this->panel2->TabIndex = 42;
          // 
          // bayMaterialCalibration2
          // 
          this->bayMaterialCalibration2->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
          this->bayMaterialCalibration2->Enabled = false;
          this->bayMaterialCalibration2->FormattingEnabled = true;
          this->bayMaterialCalibration2->Location = System::Drawing::Point(700, 158);
          this->bayMaterialCalibration2->Name = S"bayMaterialCalibration2";
          this->bayMaterialCalibration2->Size = System::Drawing::Size(182, 21);
          this->bayMaterialCalibration2->TabIndex = 68;
          this->bayMaterialCalibration2->SelectedIndexChanged += new System::EventHandler(this, &Form1::bayMaterialCalibration2_SelectedIndexChanged);
          // 
          // bayAcceleration2
          // 
          this->bayAcceleration2->Enabled = false;
          this->bayAcceleration2->Location = System::Drawing::Point(183, 158);
          this->bayAcceleration2->Name = S"bayAcceleration2";
          this->bayAcceleration2->Size = System::Drawing::Size(108, 20);
          this->bayAcceleration2->TabIndex = 67;
          this->bayAcceleration2->Text = S"100.000000";
          this->bayAcceleration2->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // bayVelocity2
          // 
          this->bayVelocity2->Enabled = false;
          this->bayVelocity2->Location = System::Drawing::Point(77, 158);
          this->bayVelocity2->Name = S"bayVelocity2";
          this->bayVelocity2->Size = System::Drawing::Size(100, 20);
          this->bayVelocity2->TabIndex = 61;
          this->bayVelocity2->Text = S"1.000000";
          this->bayVelocity2->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // bayMotorPosition2
          // 
          this->bayMotorPosition2->Enabled = false;
          this->bayMotorPosition2->Location = System::Drawing::Point(429, 158);
          this->bayMotorPosition2->Name = S"bayMotorPosition2";
          this->bayMotorPosition2->ReadOnly = true;
          this->bayMotorPosition2->Size = System::Drawing::Size(100, 20);
          this->bayMotorPosition2->TabIndex = 65;
          this->bayMotorPosition2->Text = S"0.000000";
          this->bayMotorPosition2->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // bayPositionIncrement2
          // 
          this->bayPositionIncrement2->Enabled = false;
          this->bayPositionIncrement2->Location = System::Drawing::Point(307, 158);
          this->bayPositionIncrement2->Name = S"bayPositionIncrement2";
          this->bayPositionIncrement2->Size = System::Drawing::Size(116, 20);
          this->bayPositionIncrement2->TabIndex = 66;
          this->bayPositionIncrement2->Text = S"0.100000";
          this->bayPositionIncrement2->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // bayScroll2
          // 
          this->bayScroll2->AccessibleRole = System::Windows::Forms::AccessibleRole::ScrollBar;
          this->bayScroll2->Enabled = false;
          this->bayScroll2->LargeChange = 1;
          this->bayScroll2->Location = System::Drawing::Point(663, 158);
          this->bayScroll2->Maximum = 100000000;
          this->bayScroll2->Minimum = -100000000;
          this->bayScroll2->Name = S"bayScroll2";
          this->bayScroll2->Size = System::Drawing::Size(19, 20);
          this->bayScroll2->TabIndex = 64;
          this->bayScroll2->Scroll += new System::Windows::Forms::ScrollEventHandler(this, &Form1::bayScroll2_Scroll);
          // 
          // bayCommandedPosition2
          // 
          this->bayCommandedPosition2->Enabled = false;
          this->bayCommandedPosition2->Location = System::Drawing::Point(535, 158);
          this->bayCommandedPosition2->Name = S"bayCommandedPosition2";
          this->bayCommandedPosition2->Size = System::Drawing::Size(128, 20);
          this->bayCommandedPosition2->TabIndex = 62;
          this->bayCommandedPosition2->Text = S"0.000000";
          this->bayCommandedPosition2->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // bayLabel2
          // 
          this->bayLabel2->Anchor = System::Windows::Forms::AnchorStyles::Right;
          this->bayLabel2->Location = System::Drawing::Point(8, 159);
          this->bayLabel2->Name = S"bayLabel2";
          this->bayLabel2->Size = System::Drawing::Size(63, 16);
          this->bayLabel2->TabIndex = 63;
          this->bayLabel2->Text = S"Not Loaded";
          this->bayLabel2->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
          // 
          // label5
          // 
          this->label5->AutoSize = true;
          this->label5->Location = System::Drawing::Point(697, 12);
          this->label5->Name = S"label5";
          this->label5->Size = System::Drawing::Size(96, 13);
          this->label5->TabIndex = 60;
          this->label5->Text = S"Material Calibration";
          // 
          // bayMaterialCalibration1
          // 
          this->bayMaterialCalibration1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
          this->bayMaterialCalibration1->Enabled = false;
          this->bayMaterialCalibration1->FormattingEnabled = true;
          this->bayMaterialCalibration1->Location = System::Drawing::Point(700, 132);
          this->bayMaterialCalibration1->Name = S"bayMaterialCalibration1";
          this->bayMaterialCalibration1->Size = System::Drawing::Size(182, 21);
          this->bayMaterialCalibration1->TabIndex = 59;
          this->bayMaterialCalibration1->SelectedIndexChanged += new System::EventHandler(this, &Form1::bayMaterialCalibration1_SelectedIndexChanged);
          // 
          // bayMaterialCalibration0
          // 
          this->bayMaterialCalibration0->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
          this->bayMaterialCalibration0->Enabled = false;
          this->bayMaterialCalibration0->FormattingEnabled = true;
          this->bayMaterialCalibration0->Location = System::Drawing::Point(700, 105);
          this->bayMaterialCalibration0->Name = S"bayMaterialCalibration0";
          this->bayMaterialCalibration0->Size = System::Drawing::Size(182, 21);
          this->bayMaterialCalibration0->TabIndex = 58;
          this->bayMaterialCalibration0->SelectedIndexChanged += new System::EventHandler(this, &Form1::bayMaterialCalibration0_SelectedIndexChanged);
          // 
          // XAccelerationField
          // 
          this->XAccelerationField->Enabled = false;
          this->XAccelerationField->Location = System::Drawing::Point(183, 30);
          this->XAccelerationField->Name = S"XAccelerationField";
          this->XAccelerationField->Size = System::Drawing::Size(108, 20);
          this->XAccelerationField->TabIndex = 57;
          this->XAccelerationField->Text = S"100.000000";
          this->XAccelerationField->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // YAccelerationField
          // 
          this->YAccelerationField->Enabled = false;
          this->YAccelerationField->Location = System::Drawing::Point(183, 56);
          this->YAccelerationField->Name = S"YAccelerationField";
          this->YAccelerationField->Size = System::Drawing::Size(108, 20);
          this->YAccelerationField->TabIndex = 56;
          this->YAccelerationField->Text = S"100.000000";
          this->YAccelerationField->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // ZAccelerationField
          // 
          this->ZAccelerationField->Enabled = false;
          this->ZAccelerationField->Location = System::Drawing::Point(183, 81);
          this->ZAccelerationField->Name = S"ZAccelerationField";
          this->ZAccelerationField->Size = System::Drawing::Size(108, 20);
          this->ZAccelerationField->TabIndex = 55;
          this->ZAccelerationField->Text = S"100.000000";
          this->ZAccelerationField->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // bayAcceleration1
          // 
          this->bayAcceleration1->Enabled = false;
          this->bayAcceleration1->Location = System::Drawing::Point(183, 132);
          this->bayAcceleration1->Name = S"bayAcceleration1";
          this->bayAcceleration1->Size = System::Drawing::Size(108, 20);
          this->bayAcceleration1->TabIndex = 54;
          this->bayAcceleration1->Text = S"100.000000";
          this->bayAcceleration1->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // bayAcceleration0
          // 
          this->bayAcceleration0->Enabled = false;
          this->bayAcceleration0->Location = System::Drawing::Point(183, 106);
          this->bayAcceleration0->Name = S"bayAcceleration0";
          this->bayAcceleration0->Size = System::Drawing::Size(108, 20);
          this->bayAcceleration0->TabIndex = 53;
          this->bayAcceleration0->Text = S"100.000000";
          this->bayAcceleration0->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // label1
          // 
          this->label1->AutoSize = true;
          this->label1->Location = System::Drawing::Point(180, 12);
          this->label1->Name = S"label1";
          this->label1->Size = System::Drawing::Size(111, 13);
          this->label1->TabIndex = 52;
          this->label1->Text = S"Acceleration (mm/s/s)";
          // 
          // XVelocityField
          // 
          this->XVelocityField->Enabled = false;
          this->XVelocityField->Location = System::Drawing::Point(77, 30);
          this->XVelocityField->Name = S"XVelocityField";
          this->XVelocityField->Size = System::Drawing::Size(100, 20);
          this->XVelocityField->TabIndex = 51;
          this->XVelocityField->Text = S"100.000000";
          this->XVelocityField->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // YVelocityField
          // 
          this->YVelocityField->Enabled = false;
          this->YVelocityField->Location = System::Drawing::Point(77, 56);
          this->YVelocityField->Name = S"YVelocityField";
          this->YVelocityField->Size = System::Drawing::Size(100, 20);
          this->YVelocityField->TabIndex = 50;
          this->YVelocityField->Text = S"100.000000";
          this->YVelocityField->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // ZVelocityField
          // 
          this->ZVelocityField->Enabled = false;
          this->ZVelocityField->Location = System::Drawing::Point(77, 81);
          this->ZVelocityField->Name = S"ZVelocityField";
          this->ZVelocityField->Size = System::Drawing::Size(100, 20);
          this->ZVelocityField->TabIndex = 49;
          this->ZVelocityField->Text = S"100.000000";
          this->ZVelocityField->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // bayMotorPosition1
          // 
          this->bayMotorPosition1->Enabled = false;
          this->bayMotorPosition1->Location = System::Drawing::Point(429, 132);
          this->bayMotorPosition1->Name = S"bayMotorPosition1";
          this->bayMotorPosition1->ReadOnly = true;
          this->bayMotorPosition1->Size = System::Drawing::Size(100, 20);
          this->bayMotorPosition1->TabIndex = 47;
          this->bayMotorPosition1->Text = S"0.000000";
          this->bayMotorPosition1->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // bayMotorPosition0
          // 
          this->bayMotorPosition0->Enabled = false;
          this->bayMotorPosition0->Location = System::Drawing::Point(429, 106);
          this->bayMotorPosition0->Name = S"bayMotorPosition0";
          this->bayMotorPosition0->ReadOnly = true;
          this->bayMotorPosition0->Size = System::Drawing::Size(100, 20);
          this->bayMotorPosition0->TabIndex = 46;
          this->bayMotorPosition0->Text = S"0.000000";
          this->bayMotorPosition0->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // bayPositionIncrement1
          // 
          this->bayPositionIncrement1->Enabled = false;
          this->bayPositionIncrement1->Location = System::Drawing::Point(307, 132);
          this->bayPositionIncrement1->Name = S"bayPositionIncrement1";
          this->bayPositionIncrement1->Size = System::Drawing::Size(116, 20);
          this->bayPositionIncrement1->TabIndex = 48;
          this->bayPositionIncrement1->Text = S"0.100000";
          this->bayPositionIncrement1->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // bayPositionIncrement0
          // 
          this->bayPositionIncrement0->Enabled = false;
          this->bayPositionIncrement0->Location = System::Drawing::Point(307, 106);
          this->bayPositionIncrement0->Name = S"bayPositionIncrement0";
          this->bayPositionIncrement0->Size = System::Drawing::Size(116, 20);
          this->bayPositionIncrement0->TabIndex = 47;
          this->bayPositionIncrement0->Text = S"0.100000";
          this->bayPositionIncrement0->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // zScroll
          // 
          this->zScroll->AccessibleRole = System::Windows::Forms::AccessibleRole::ScrollBar;
          this->zScroll->Enabled = false;
          this->zScroll->LargeChange = 1;
          this->zScroll->Location = System::Drawing::Point(663, 81);
          this->zScroll->Maximum = 100000000;
          this->zScroll->Minimum = -100000000;
          this->zScroll->Name = S"zScroll";
          this->zScroll->Size = System::Drawing::Size(19, 20);
          this->zScroll->TabIndex = 46;
          this->zScroll->Scroll += new System::Windows::Forms::ScrollEventHandler(this, &Form1::ZScroll_Scroll);
          // 
          // yScroll
          // 
          this->yScroll->AccessibleRole = System::Windows::Forms::AccessibleRole::ScrollBar;
          this->yScroll->Enabled = false;
          this->yScroll->LargeChange = 1;
          this->yScroll->Location = System::Drawing::Point(663, 56);
          this->yScroll->Maximum = 100000000;
          this->yScroll->Minimum = -100000000;
          this->yScroll->Name = S"yScroll";
          this->yScroll->Size = System::Drawing::Size(19, 20);
          this->yScroll->TabIndex = 45;
          this->yScroll->Scroll += new System::Windows::Forms::ScrollEventHandler(this, &Form1::YScroll_Scroll);
          // 
          // xScroll
          // 
          this->xScroll->AccessibleRole = System::Windows::Forms::AccessibleRole::ScrollBar;
          this->xScroll->Enabled = false;
          this->xScroll->LargeChange = 1;
          this->xScroll->Location = System::Drawing::Point(663, 30);
          this->xScroll->Maximum = 100000000;
          this->xScroll->Minimum = -100000000;
          this->xScroll->Name = S"xScroll";
          this->xScroll->Size = System::Drawing::Size(19, 20);
          this->xScroll->TabIndex = 44;
          this->xScroll->Scroll += new System::Windows::Forms::ScrollEventHandler(this, &Form1::XScroll_Scroll);
          // 
          // CommandedLabel
          // 
          this->CommandedLabel->AutoSize = true;
          this->CommandedLabel->Location = System::Drawing::Point(532, 12);
          this->CommandedLabel->Name = S"CommandedLabel";
          this->CommandedLabel->Size = System::Drawing::Size(131, 13);
          this->CommandedLabel->TabIndex = 36;
          this->CommandedLabel->Text = S"Commanded Position (mm)";
          // 
          // RealTimeLabel
          // 
          this->RealTimeLabel->AutoSize = true;
          this->RealTimeLabel->Location = System::Drawing::Point(426, 12);
          this->RealTimeLabel->Name = S"RealTimeLabel";
          this->RealTimeLabel->Size = System::Drawing::Size(99, 13);
          this->RealTimeLabel->TabIndex = 35;
          this->RealTimeLabel->Text = S"Motor Position (mm)";
          // 
          // ZIncrementField
          // 
          this->ZIncrementField->Enabled = false;
          this->ZIncrementField->Location = System::Drawing::Point(307, 83);
          this->ZIncrementField->Name = S"ZIncrementField";
          this->ZIncrementField->Size = System::Drawing::Size(116, 20);
          this->ZIncrementField->TabIndex = 34;
          this->ZIncrementField->Text = S"0.500000";
          this->ZIncrementField->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // YIncrementField
          // 
          this->YIncrementField->Enabled = false;
          this->YIncrementField->Location = System::Drawing::Point(307, 56);
          this->YIncrementField->Name = S"YIncrementField";
          this->YIncrementField->Size = System::Drawing::Size(116, 20);
          this->YIncrementField->TabIndex = 33;
          this->YIncrementField->Text = S"1.500000";
          this->YIncrementField->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // XIncrementField
          // 
          this->XIncrementField->Enabled = false;
          this->XIncrementField->Location = System::Drawing::Point(308, 30);
          this->XIncrementField->Name = S"XIncrementField";
          this->XIncrementField->Size = System::Drawing::Size(115, 20);
          this->XIncrementField->TabIndex = 32;
          this->XIncrementField->Text = S"1.500000";
          this->XIncrementField->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // label2
          // 
          this->label2->AutoSize = true;
          this->label2->Enabled = false;
          this->label2->Location = System::Drawing::Point(456, 18);
          this->label2->Name = S"label2";
          this->label2->Size = System::Drawing::Size(0, 13);
          this->label2->TabIndex = 31;
          // 
          // IncrementLabel
          // 
          this->IncrementLabel->AutoSize = true;
          this->IncrementLabel->Location = System::Drawing::Point(304, 12);
          this->IncrementLabel->Name = S"IncrementLabel";
          this->IncrementLabel->Size = System::Drawing::Size(119, 13);
          this->IncrementLabel->TabIndex = 30;
          this->IncrementLabel->Text = S"Position Increment (mm)";
          // 
          // ZCommandedField
          // 
          this->ZCommandedField->Enabled = false;
          this->ZCommandedField->Location = System::Drawing::Point(535, 81);
          this->ZCommandedField->Name = S"ZCommandedField";
          this->ZCommandedField->Size = System::Drawing::Size(128, 20);
          this->ZCommandedField->TabIndex = 29;
          this->ZCommandedField->Text = S"0.000000";
          this->ZCommandedField->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // YCommandedField
          // 
          this->YCommandedField->Enabled = false;
          this->YCommandedField->Location = System::Drawing::Point(535, 56);
          this->YCommandedField->Name = S"YCommandedField";
          this->YCommandedField->Size = System::Drawing::Size(128, 20);
          this->YCommandedField->TabIndex = 28;
          this->YCommandedField->Text = S"0.000000";
          this->YCommandedField->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // XCommandedField
          // 
          this->XCommandedField->Enabled = false;
          this->XCommandedField->Location = System::Drawing::Point(535, 30);
          this->XCommandedField->Name = S"XCommandedField";
          this->XCommandedField->Size = System::Drawing::Size(128, 20);
          this->XCommandedField->TabIndex = 27;
          this->XCommandedField->Text = S"0.000000";
          this->XCommandedField->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
          // 
          // PosDisplayThread
          // 
          this->PosDisplayThread->WorkerReportsProgress = true;
          this->PosDisplayThread->DoWork += new System::ComponentModel::DoWorkEventHandler(this, &Form1::PosDisplayThread_DoWork);
          this->PosDisplayThread->ProgressChanged += new System::ComponentModel::ProgressChangedEventHandler(this, &Form1::PosDisplayThread_ProgressChanged);
          // 
          // panel3
          // 
          this->panel3->Controls->Add(this->InitButton);
          this->panel3->Location = System::Drawing::Point(9, 7);
          this->panel3->Name = S"panel3";
          this->panel3->Size = System::Drawing::Size(887, 50);
          this->panel3->TabIndex = 43;
          // 
          // panel4
          // 
          this->panel4->Controls->Add(this->ExecutePathLabel);
          this->panel4->Controls->Add(this->LoadFileButton);
          this->panel4->Controls->Add(this->ExecuteButton);
          this->panel4->Location = System::Drawing::Point(9, 317);
          this->panel4->Name = S"panel4";
          this->panel4->Size = System::Drawing::Size(717, 41);
          this->panel4->TabIndex = 44;
          // 
          // fs
          // 
          this->fs->Controls->Add(this->ExitButton);
          this->fs->Controls->Add(this->RedoPathButton);
          this->fs->Location = System::Drawing::Point(732, 317);
          this->fs->Name = S"fs";
          this->fs->Size = System::Drawing::Size(164, 139);
          this->fs->TabIndex = 45;
          // 
          // openFileDialog1
          // 
          this->openFileDialog1->FileName = S"openFileDialog1";
          this->openFileDialog1->InitialDirectory = S"C:\\Documents and Settings\\FabAdmin\\Desktop\\SVN- FAB@Home.org\\software\\projects\\printers\\fabathome-model2\\Fab@Home Interpreter\\Printers";
          // 
          // Form1
          // 
          this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
          this->ClientSize = System::Drawing::Size(904, 465);
          this->Controls->Add(this->fs);
          this->Controls->Add(this->panel4);
          this->Controls->Add(this->panel3);
          this->Controls->Add(this->panel2);
          this->Controls->Add(this->panel1);
          this->Name = S"Form1";
          this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
          this->Text = S"Fab@Home Interpreter";
          this->Closing += new System::ComponentModel::CancelEventHandler(this, &Form1::Form1_Closing);
          this->panel1->ResumeLayout(false);
          this->panel2->ResumeLayout(false);
          this->panel2->PerformLayout();
          this->panel3->ResumeLayout(false);
          this->panel4->ResumeLayout(false);
          this->panel4->PerformLayout();
          this->fs->ResumeLayout(false);
          this->ResumeLayout(false);

     }	










































	//***************************Form General Functions*******************************

private: void Form1_Closing(System::Object *  sender, System::ComponentModel::CancelEventArgs *  e)
		 {
			 printer.cleanUp();
		 }
		 //////////////////////////////////////////////////////////////////////////////////
		 //***************************Main List Action Functions******************************
private: void InitButton_Click(System::Object *  sender, System::EventArgs *  e)
		 {

                if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                {
                     string filePath = (char*)(void*)Marshal::StringToHGlobalAnsi(openFileDialog1->FileName);
                     string result = printer.initialize(filePath);
			      if(result.compare("") == 0)
			      {
                          
				       correctModule = true;

                           //Show labels.
                           
                           XLabel->Text = new String("X");
                           YLabel->Text = new String("Y");
                           ZLabel->Text = new String("Z");
			            int numLoadedBays = printer.numLoadedBays();
                           if(numLoadedBays >= 1)
                           {
                                bayLabel0->Text = new String("Bay 0");
                           }
                           if(numLoadedBays >= 2)
                           {
                                bayLabel1->Text = new String("Bay 1");
                           }
                           if(numLoadedBays >= 3)
                           {
                                bayLabel2->Text = new String("Bay 2");
                           }
                           


				       //Modify buttons.
				       //InitButton->Enabled = false;
				       LoadFileButton->Focus();
			            
                           enablePositionDisplay(); //Start live position display
			      }
			      else
			      {
                          Util::messageBox(result);
			      }
                }
		 }
		 ////////////////////////////////////////////////////////////////////////////////////
private: void toggleAxes(bool enabled)
         {
               XVelocityField->Enabled = enabled;
               XAccelerationField->Enabled = enabled;
               XIncrementField->Enabled = enabled;
               XDisplay->Enabled = enabled;
               XCommandedField->Enabled = enabled;
               xScroll->Enabled = enabled;
               YVelocityField->Enabled = enabled;
               YAccelerationField->Enabled = enabled;
               YIncrementField->Enabled = enabled;
               YDisplay->Enabled = enabled;
               YCommandedField->Enabled = enabled;
               yScroll->Enabled = enabled;
               ZVelocityField->Enabled = enabled;
               ZAccelerationField->Enabled = enabled;
               ZIncrementField->Enabled = enabled;
               ZDisplay->Enabled = enabled;
               ZCommandedField->Enabled = enabled;
               zScroll->Enabled = enabled;
          }
          ////////////////////////////////////////////////////////////////////////////////////
private: void toggleBays(bool enabled)
         {
              int numLoadedBays = printer.numLoadedBays();
              if(numLoadedBays >= 1)
              {
                   bayVelocity0->Enabled = enabled;
                   bayAcceleration0->Enabled = enabled;
                   bayPositionIncrement0->Enabled = enabled;
                   bayMotorPosition0->Enabled = enabled;
                   bayCommandedPosition0->Enabled = enabled;
                   bayMaterialCalibration0->Enabled = enabled;
                   bayScroll0->Enabled = enabled;
              }
              if(numLoadedBays >= 2)
              {
                   bayVelocity1->Enabled = enabled;
                   bayAcceleration1->Enabled = enabled;
                   bayPositionIncrement1->Enabled = enabled;
                   bayMotorPosition1->Enabled = enabled;
                   bayCommandedPosition1->Enabled = enabled;
                   bayMaterialCalibration1->Enabled = enabled;
                   bayScroll1->Enabled = enabled;
              }
              if(numLoadedBays >= 3)
              {
                   bayVelocity2->Enabled = enabled;
                   bayAcceleration2->Enabled = enabled;
                   bayPositionIncrement2->Enabled = enabled;
                   bayMotorPosition2->Enabled = enabled;
                   bayCommandedPosition2->Enabled = enabled;
                   bayMaterialCalibration2->Enabled = enabled;
                   bayScroll2->Enabled = enabled;
              }
         }
		 ////////////////////////////////////////////////////////////////////////////////////
private: void MoveButton_Click(System::Object *  sender, System::EventArgs *  e)
		 {
			printer.axes["X"].motor->moveAbsolute(textBoxValue(XCommandedField), textBoxValue(XVelocityField), textBoxValue(XAccelerationField));	
			printer.axes["Y"].motor->moveAbsolute(textBoxValue(YCommandedField), textBoxValue(YVelocityField), textBoxValue(YAccelerationField));
			printer.axes["Z"].motor->moveAbsolute(textBoxValue(ZCommandedField), textBoxValue(ZVelocityField), textBoxValue(ZAccelerationField));
			int numLoadedBays = printer.numLoadedBays();
               if(numLoadedBays >= 1)
               {
			    printer.tool.bays["Bay 0"].motor->moveAbsolute(textBoxValue(bayCommandedPosition0), textBoxValue(bayVelocity0), textBoxValue(bayAcceleration0));
        		}
               if(numLoadedBays >= 2)
               {
                   printer.tool.bays["Bay 1"].motor->moveAbsolute(textBoxValue(bayCommandedPosition1), textBoxValue(bayVelocity1), textBoxValue(bayAcceleration1));
			
               }
               if(numLoadedBays >= 3)
               {
                   printer.tool.bays["Bay 2"].motor->moveAbsolute(textBoxValue(bayCommandedPosition2), textBoxValue(bayVelocity2), textBoxValue(bayAcceleration2));			
               }
         }
		 ////////////////////////////////////////////////////////////////////////////////////
private: void ResetPosButton_Click(System::Object*  sender, System::EventArgs*  e) 
		 {
		 	 //Reset position of all motors.
          	 printer.axes["X"].motor->resetPosition();
     		 printer.axes["Y"].motor->resetPosition();
			 printer.axes["Z"].motor->resetPosition();
                XCommandedField->set_Text("0.000000");
			 YCommandedField->set_Text("0.000000");
			 ZCommandedField->set_Text("0.000000");
                int numLoadedBays = printer.numLoadedBays();
                if(numLoadedBays >= 1)
                {
			     printer.tool.bays["Bay 0"].motor->resetPosition();
                    bayCommandedPosition0->set_Text("0.000000");
        		 }
                if(numLoadedBays >= 2)
                {
                    printer.tool.bays["Bay 1"].motor->resetPosition();
                    bayCommandedPosition1->set_Text("0.000000");
                }
                if(numLoadedBays >= 3)
                {
                    printer.tool.bays["Bay 2"].motor->resetPosition();
                    bayCommandedPosition2->set_Text("0.000000");
                }
		 }
		 ////////////////////////////////////////////////////////////////////////////////////
private: void LoadFileButton_Click(System::Object *  sender, System::EventArgs *  e)
		 {
                if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                {
                     string filePath = (char*)(void*)Marshal::StringToHGlobalAnsi(openFileDialog1->FileName);
                     string result = printer.loadFabFile(filePath);
                     if(result.compare("") == 0)
			      {
				      //Update display.
                          string info = Util::toString<int>(printer.numLoadedPaths())+" paths loaded from file "+filePath;
				      
				      maxProgress = printer.numLoadedPaths(); //sets the max progress to the number of paths
                          ExecutePathLabel->set_Text(info.c_str());

                          //Show the loaded material calibrations.
                          vector<string> result;
                          printer.loadedMaterialCalibrations(result);
                          bayMaterialCalibration0->Items->Clear();
                          bayMaterialCalibration1->Items->Clear();
                          bayMaterialCalibration0->Items->Add(new System::String(""));
                          bayMaterialCalibration1->Items->Add(new System::String(""));
                          for(vector<string>::iterator i = result.begin(); i != result.end(); ++i)
                          {
                               int numLoadedBays = printer.numLoadedBays();
                               if(numLoadedBays >= 1)
                               {
                                   bayMaterialCalibration0->Items->Add(new System::String(i->c_str()));
                               }
                               if(numLoadedBays >= 2)
                               {
                                   bayMaterialCalibration1->Items->Add(new System::String(i->c_str()));
                               }
                               if(numLoadedBays >= 3)
                               {
                                   bayMaterialCalibration2->Items->Add(new System::String(i->c_str()));
                               }
                           }
                                               
				      ExecuteButton->Enabled = true;
				      ExecuteButton->Focus();
			      }
			      else
			      {
                          Util::messageBox(result);
			      }
                }
		 }
		 ////////////////////////////////////////////////////////////////////////////////////
private: void ExecuteButton_Click(System::Object*  sender, System::EventArgs*  e) {
			 FabricationThread->RunWorkerAsync(); //Executes fabrication thread
		 }
		 ////////////////////////////////////////////////////////////////////////////////////
private: void ExitButton_Click(System::Object *  sender, System::EventArgs *  e) {		
			 disablePositionDisplay();
			 Close();
		 }
		 //////////////////////////////////////////////////////////////////////////////////////////////
private: double textBoxValue(System::Windows::Forms::TextBox* textBox)
		 {
			 return Util::toType<double>((char*)(void*)Marshal::StringToHGlobalAnsi(textBox->get_Text()));
		 }
		 //////////////////////////////////////////////////////////////////////////////////////////////
private: void incrementCommanded(System::Windows::Forms::TextBox* incrementField, System::Windows::Forms::ScrollEventArgs* e, System::Windows::Forms::TextBox* commandedField)
		 {
			 string incrementString = (char*)(void*)Marshal::StringToHGlobalAnsi(incrementField->get_Text());
                double delta = (e->get_OldValue() - e->get_NewValue()) * Util::toType<double>(incrementString);

			 string commandedString = (char*)(void*)Marshal::StringToHGlobalAnsi(commandedField->get_Text());
                double commanded = Util::toType<double>(commandedString);

			 double newCommanded = commanded + delta;

                string newCommandedString = Util::toString<double>(newCommanded);
                commandedField->set_Text(newCommandedString.c_str());
			 commandedField->Update();
		 }
		 //////////////////////////////////////////////////////////////////////////////////////////////
		 //************************************Scroll Buttons******************************************
private: void XScroll_Scroll(System::Object*  sender, System::Windows::Forms::ScrollEventArgs*  e) {
			 incrementCommanded(XIncrementField, e, XCommandedField);
			 printer.axes["X"].motor->moveAbsolute(textBoxValue(XCommandedField), textBoxValue(XVelocityField), textBoxValue(XAccelerationField));
		 }
		 //////////////////////////////////////////////////////////////////////////////////////////////
private: void YScroll_Scroll(System::Object*  sender, System::Windows::Forms::ScrollEventArgs*  e) {
			 incrementCommanded(YIncrementField, e, YCommandedField);
			 printer.axes["Y"].motor->moveAbsolute(textBoxValue(YCommandedField), textBoxValue(YVelocityField), textBoxValue(YAccelerationField));
		 }
		 //////////////////////////////////////////////////////////////////////////////////////////////
private: void ZScroll_Scroll(System::Object*  sender, System::Windows::Forms::ScrollEventArgs*  e) {
			 incrementCommanded(ZIncrementField, e, ZCommandedField);
			 printer.axes["Z"].motor->moveAbsolute(textBoxValue(ZCommandedField), textBoxValue(ZVelocityField), textBoxValue(ZAccelerationField));
		 }
		 //////////////////////////////////////////////////////////////////////////////////////////////
private: void Tool1Scroll_Scroll(System::Object*  sender, System::Windows::Forms::ScrollEventArgs*  e) {
			 incrementCommanded(bayPositionIncrement0, e, bayCommandedPosition0);
			 printer.tool.bays["Bay 0"].motor->moveAbsolute(textBoxValue(bayCommandedPosition0), textBoxValue(bayVelocity0), textBoxValue(bayAcceleration0));
		 }
		 ///////////////////////////////////////////////////////////////////////////////////////////////////
private: void Tool2Scroll_Scroll(System::Object*  sender, System::Windows::Forms::ScrollEventArgs*  e) {
			 incrementCommanded(bayPositionIncrement1, e, bayCommandedPosition1);
			 printer.tool.bays["Bay 1"].motor->moveAbsolute(textBoxValue(bayCommandedPosition1), textBoxValue(bayVelocity1), textBoxValue(bayAcceleration1));
		 }	
		 ///////////////////////////////////////////////////////////////////////////////////////////////////
private: System::Void bayScroll2_Scroll(System::Object*  sender, System::Windows::Forms::ScrollEventArgs*  e) {
               incrementCommanded(bayPositionIncrement2, e, bayCommandedPosition2);
			printer.tool.bays["Bay 2"].motor->moveAbsolute(textBoxValue(bayCommandedPosition2), textBoxValue(bayVelocity2), textBoxValue(bayAcceleration2));
         }
		 ///////////////////////////////////////////////////////////////////////////////////////////////////
		 //**************************************Stop/Cancel/Pause Buttons***************************************
private: void ForceStopButton_Click(System::Object*  sender, System::EventArgs*  e) {
			 //Stop all motors.
			 printer.axes["X"].motor->stop();
			 printer.axes["Y"].motor->stop();
			 printer.axes["Z"].motor->stop();
                int numLoadedBays = printer.numLoadedBays();
                if(numLoadedBays >= 1)
                {
			    printer.tool.bays["Bay 0"].motor->stop();
                }
                if(numLoadedBays >= 2)
                {
			    printer.tool.bays["Bay 1"].motor->stop();
                }
                if(numLoadedBays >= 3)
                {
			    printer.tool.bays["Bay 2"].motor->stop();
                }
		 }
		 ///////////////////////////////////////////////////////////////////////////////////////////////////
private: void RedoPathButton_Click(System::Object*  sender, System::EventArgs*  e) {
			 printer.redoPath();
		 }
		 ///////////////////////////////////////////////////////////////////////////////////////////////////
private: void PauseFabButton_Click(System::Object*  sender, System::EventArgs*  e) {
			 printer.pausePrint();
		 }
		 ///////////////////////////////////////////////////////////////////////////////////////////////////
private: void CancelFabButton_Click(System::Object*  sender, System::EventArgs*  e) {
			 printer.cancelPrint();
		 }
		 //*************************************Display Threads*************************************		 
private: void PosDisplayThread_DoWork(System::Object*  sender, System::ComponentModel::DoWorkEventArgs*  e) {
			 int c = 0;
			 while (enablePosDisplay)
			 {
				 c = (c == 0 ? 1 : 0);
				 this->PosDisplayThread->ReportProgress(c);
				 Sleep((DWORD)(posDisplayPeriod * 1000));
			 }
		 }

		 void enablePositionDisplay()
		 {
			 enablePosDisplay = true;
			 this->PosDisplayThread->RunWorkerAsync();
		 }

		 void disablePositionDisplay()
		 {
			 enablePosDisplay = false;
		 }
		 ///////////////////////////////////////////////////////////////////////////////////////////////////////
private: void PosDisplayThread_ProgressChanged(System::Object* sender, System::ComponentModel::ProgressChangedEventArgs* e) {
                static string prevState;
                string state = printer.state();

                if(state.compare("PAUSED") == 0)
                {
                     updatePosDisplay();
                     if(prevState.compare("PRINTING") == 0)
                     {
                          XCommandedField->set_Text(XDisplay->get_Text());
			           YCommandedField->set_Text(YDisplay->get_Text());
			           ZCommandedField->set_Text(ZDisplay->get_Text());
			           bayCommandedPosition0->set_Text(bayMotorPosition0->get_Text());
			           bayCommandedPosition1->set_Text(bayMotorPosition1->get_Text());
                     }
                     toggleBays(true);
                     toggleAxes(false);
                     ResetPosButton->Enabled = false;
                     MoveButton->Enabled = false;
                     LoadFileButton->Enabled = false;
                     ExecuteButton->Enabled = false;
                   	 this->PauseFabButton->Enabled=true;
		     	 this->CancelFabButton->Enabled=true;
	     		 this->ForceStopButton->Enabled=true;
     			 this->RedoPathButton->Enabled=true;
                }
                else if(state.compare("PRINTING") == 0)
                {
                     toggleBays(false);
                     toggleAxes(false);
                     ResetPosButton->Enabled = false;
                     MoveButton->Enabled = false;
                     LoadFileButton->Enabled = false;
                     ExecuteButton->Enabled = false;
			      this->PauseFabButton->Enabled=true;
		     	 this->CancelFabButton->Enabled=true;
	     		 this->ForceStopButton->Enabled=true;
     			 this->RedoPathButton->Enabled=true;
                }
                else if(state.compare("IDLE") == 0)
                {
                     updatePosDisplay();
                     if(prevState.compare("PRINTING") == 0)
                     {
                          XCommandedField->set_Text(XDisplay->get_Text());
			           YCommandedField->set_Text(YDisplay->get_Text());
			           ZCommandedField->set_Text(ZDisplay->get_Text());
			           bayCommandedPosition0->set_Text(bayMotorPosition0->get_Text());
			           bayCommandedPosition1->set_Text(bayMotorPosition1->get_Text());
                     }           
                     toggleBays(true);
                     toggleAxes(true);
                     ResetPosButton->Enabled = true;
                     MoveButton->Enabled = true;
                     LoadFileButton->Enabled = true;
			      this->PauseFabButton->Enabled=false;
		     	 this->CancelFabButton->Enabled=false;
	     		 this->ForceStopButton->Enabled=false;
     			 this->RedoPathButton->Enabled=false;
                }
                prevState = state;
		 }
		 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		 //*************************************Execution Threads************************************
private: void FabricationThread_DoWork(System::Object*  sender, System::ComponentModel::DoWorkEventArgs*  e) {
			 printer.print(FabricationThread, pathDisplayText);
		 }
		 ////////////////////////////////////////////////////////////////////////////////////////////
private: void FabricationThread_WorkerCompleted(System::Object*  sender, System::ComponentModel::RunWorkerCompletedEventArgs*  e) {
		 }
		 //////////////////////////////////////////////////////////////////////////////////////////////////
private: void FabricationThread_ProgressChanged(System::Object* sender, System::ComponentModel::ProgressChangedEventArgs* e) {
			 int progress = e->get_ProgressPercentage();

			 if (progress==0) this->PathProgressBar->set_Maximum(maxProgress);

			 this->ExecutePathLabel->set_Text(pathDisplayText.c_str());
			 this->ExecutePathLabel->Update();

			 this->PathProgressBar->set_Value(progress);
			 this->PathProgressBar->Update();
		 }
		 /////////////////////////////////////////////////////////////////////////////////////////////////////
private: void Timer_Tick(System::Object*  sender, System::EventArgs*  e) {
			 
		 }

		 //************************************I/O Helper Functions********************************
private: void updatePosDisplay() {
		      string temp;	 
                //Display the position data in text fields
                temp = Util::toString<double>(printer.axes["X"].motor->getPosition());
                XDisplay->set_Text(temp.c_str());
			 XDisplay->Update();
			 temp = Util::toString<double>(printer.axes["Y"].motor->getPosition());
                YDisplay->set_Text(temp.c_str());
			 YDisplay->Update();
			 temp = Util::toString<double>(printer.axes["Z"].motor->getPosition());
                ZDisplay->set_Text(temp.c_str());
			 ZDisplay->Update();
                
                int numLoadedBays = printer.numLoadedBays();
                if(numLoadedBays >= 1)
                {
                     temp = Util::toString<double>(printer.tool.bays["Bay 0"].motor->getPosition());
                     bayMotorPosition0->set_Text(temp.c_str());
		     	 bayMotorPosition0->Update();
			 }
                if(numLoadedBays >= 2)
                {
                     temp = Util::toString<double>(printer.tool.bays["Bay 1"].motor->getPosition());
                     bayMotorPosition1->set_Text(temp.c_str());
			      bayMotorPosition1->Update();
                }
                if(numLoadedBays >= 3)
                {
                     temp = Util::toString<double>(printer.tool.bays["Bay 2"].motor->getPosition());
                     bayMotorPosition2->set_Text(temp.c_str());
			      bayMotorPosition2->Update();
                }
                
		 }
		 ///////////////////////////////////////////////////////////////////////////////////////////////
private: System::Void bayMaterialCalibration0_SelectedIndexChanged(System::Object*  sender, System::EventArgs*  e)
         {
              string bayName = (char*)(void*)Marshal::StringToHGlobalAnsi(bayLabel0->Text);
              string materialCalibrationName = (char*)(void*)Marshal::StringToHGlobalAnsi(bayMaterialCalibration0->Text);
              printer.equipBay(bayName, materialCalibrationName);
         }
  	      ///////////////////////////////////////////////////////////////////////////////////////////////
private: System::Void bayMaterialCalibration1_SelectedIndexChanged(System::Object*  sender, System::EventArgs*  e)
         {
              string bayName = (char*)(void*)Marshal::StringToHGlobalAnsi(bayLabel1->Text);
              string materialCalibrationName = (char*)(void*)Marshal::StringToHGlobalAnsi(bayMaterialCalibration1->Text);
              printer.equipBay(bayName, materialCalibrationName);
         }
            ///////////////////////////////////////////////////////////////////////////////////////////////
private: System::Void bayMaterialCalibration2_SelectedIndexChanged(System::Object*  sender, System::EventArgs*  e) {
              string bayName = (char*)(void*)Marshal::StringToHGlobalAnsi(bayLabel2->Text);
              string materialCalibrationName = (char*)(void*)Marshal::StringToHGlobalAnsi(bayMaterialCalibration2->Text);
              printer.equipBay(bayName, materialCalibrationName);
         }
             ///////////////////////////////////////////////////////////////////////////////////////////////

};

#endif //ndef FORM1_H



