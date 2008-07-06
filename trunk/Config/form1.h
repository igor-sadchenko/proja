#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "stdafx.h"
namespace config {

	/// <summary>
	/// Summary for CropForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		FilterForm^ m_formMonochrome;
		FilterForm^ m_formNoise;
		CropForm^ m_formCrop;
		int preview_index ;
	private: System::Windows::Forms::CheckBox^  chkFlipX;
	public: 
	private: System::Windows::Forms::CheckBox^  chkFlipY;
	private: System::Windows::Forms::TrackBar^  trackBar1;
	private: System::Windows::Forms::Label^  label1;
			 int sample_index ;

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;




	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->chkFlipX = (gcnew System::Windows::Forms::CheckBox());
			this->chkFlipY = (gcnew System::Windows::Forms::CheckBox());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pictureBox1->Location = System::Drawing::Point(12, 51);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(310, 254);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pictureBox1_MouseDown);
			this->pictureBox1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pictureBox1_MouseMove);
			this->pictureBox1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pictureBox1_MouseUp);
			// 
			// chkFlipX
			// 
			this->chkFlipX->AutoSize = true;
			this->chkFlipX->Location = System::Drawing::Point(12, 5);
			this->chkFlipX->Name = L"chkFlipX";
			this->chkFlipX->Size = System::Drawing::Size(49, 17);
			this->chkFlipX->TabIndex = 4;
			this->chkFlipX->Text = L"flip X";
			this->chkFlipX->UseVisualStyleBackColor = true;
			this->chkFlipX->CheckedChanged += gcnew System::EventHandler(this, &MainForm::chkFlipX_CheckedChanged);
			// 
			// chkFlipY
			// 
			this->chkFlipY->AutoSize = true;
			this->chkFlipY->Location = System::Drawing::Point(12, 28);
			this->chkFlipY->Name = L"chkFlipY";
			this->chkFlipY->Size = System::Drawing::Size(49, 17);
			this->chkFlipY->TabIndex = 5;
			this->chkFlipY->Text = L"flip Y";
			this->chkFlipY->UseVisualStyleBackColor = true;
			this->chkFlipY->CheckedChanged += gcnew System::EventHandler(this, &MainForm::chkFlipY_CheckedChanged);
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(102, 3);
			this->trackBar1->Maximum = 255;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(175, 42);
			this->trackBar1->TabIndex = 6;
			this->trackBar1->Value = 145;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &MainForm::trackBar1_Scroll);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(287, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(0, 13);
			this->label1->TabIndex = 7;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(334, 317);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->chkFlipY);
			this->Controls->Add(this->chkFlipX);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"MainForm";
			this->Text = L"Capture (1)";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &MainForm::CropForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void CropForm_Load(System::Object^  sender, System::EventArgs^  e)
			 {
				 preview_index = 0;
				 if( FAILED(g_video.CaptureFromDevice(0)))
				 {
					 MessageBox::Show(L"camera device init failed\r\n");
					 return;
				 }

				 if (FAILED(g_video.AppendSampleGrabber( &g_Listner )))
				 {
					 MessageBox::Show(L"Couldnt append the Hi-Pass filter"); // edit the filter name
					 return;
				 }

				 if(FAILED(g_video.AppendPreview( (HWND)pictureBox1->Handle.ToPointer())))
				 {
					 MessageBox::Show(L"window 1 init failed");
					 return;
				 }

				 // Monochrome ...
				 if (FAILED(g_video.AppendSampleGrabber( &g_Monochrome)))
				 {
					 MessageBox::Show(L"Couldnt append the Monochrome filter"); // edit the filter name
					 return;
				 }
				 m_formMonochrome = gcnew FilterForm(g_settings.m_valNoise, L"Monochrome");
				 m_formMonochrome->Show(this);

				 if(FAILED(g_video.AppendPreview( (HWND)m_formMonochrome->pictureBox1->Handle.ToPointer())))
				 {
					 MessageBox::Show(L"window 2 init failed");
					 return;
				 }

				 // Noise Removal ...
				 if (FAILED(g_video.AppendSampleGrabber( &g_Noise)))
				 {
					 MessageBox::Show(L"Couldnt append the Hi-Pass filter"); // edit the filter name
					 return;
				 }
				 m_formNoise = gcnew FilterForm(g_settings.m_valNoise, L"Noise Removal");
				 m_formNoise->Show(this);

				 if(FAILED(g_video.AppendPreview( (HWND)m_formNoise->pictureBox1->Handle.ToPointer())))
				 {
					 MessageBox::Show(L"window 2 init failed");
					 return;
				 }

				 // Cropping ...
				 if (FAILED(g_video.AppendSampleGrabber( &g_Crop)))
				 {
					 MessageBox::Show(L"Couldnt append the Cropping filter"); // edit the filter name
					 return;
				 }
				 m_formCrop = gcnew CropForm(NULL);
				 m_formCrop->Show(this);

				 if(FAILED(g_video.AppendPreview( (HWND)m_formCrop->pictureBox1->Handle.ToPointer())))
				 {
					 MessageBox::Show(L"Crop Form init failed");
					 return;
				 }

				 if(FAILED(g_video.Play()))
				 {
					 MessageBox::Show(L"play failed");
					 return;
				 }				


			 }
	private: System::Void pictureBox1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

				 g_settings.m_Crop.left = e->X;
				 g_settings.m_Crop.top = e->Y;
				 g_settings.m_Crop.right = g_settings.m_Crop.left;
				 g_settings.m_Crop.bottom = g_settings.m_Crop.top;

			 }
	private: System::Void pictureBox1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

				 if(e->Button == ::MouseButtons::Left)
				 {
					 g_settings.m_Crop.right = e->X;
					 g_settings.m_Crop.bottom = e->Y;
				 }

			 }
	private: System::Void pictureBox1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 }

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 // 			 if(FAILED(g_video.SetCrop(g_settings.m_Crop)))
				 // 			 {
				 // 				 MessageBox::Show("crop failed");
				 // 			 }


			 }
	private: System::Void chkFlipX_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 g_settings.m_x_flip = chkFlipX->Checked;

			 }
private: System::Void chkFlipY_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 g_settings.m_y_flip = chkFlipY->Checked;
		 }
private: System::Void MainForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			 System::Windows::Forms::DialogResult res = System::Windows::Forms::MessageBox::Show(this,"Configuration Changed","Save changes ?",MessageBoxButtons::YesNoCancel,MessageBoxIcon::Question,MessageBoxDefaultButton::Button1);
			 if(res == System::Windows::Forms::DialogResult::Yes)
			 {
				 ofstream fout("config.txt");
				 fout<<g_settings;
			 }
			 else if (res == System::Windows::Forms::DialogResult::Cancel)
			 {
				 e->Cancel = true;
			 }
		 }
private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) {
			 label1->Text = trackBar1->Value.ToString();
			 g_settings.m_threshold = trackBar1->Value;
			 BlobDetector::m_Threshold = trackBar1->Value;
		 }
};
}
