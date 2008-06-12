#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "stdafx.h"
namespace config2 {

	/// <summary>
	/// Summary for CropForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CropForm : public System::Windows::Forms::Form
	{
	public:
		CropForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		Form_2_Highpass^ m_f1;
		int preview_index ;
		int sample_index ;

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CropForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label1;
			 
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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pictureBox1->Location = System::Drawing::Point(0, 55);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(295, 254);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &CropForm::pictureBox1_MouseMove);
			this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &CropForm::pictureBox1_MouseDown);

			this->pictureBox1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &CropForm::pictureBox1_MouseUp);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(205, 26);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"start";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &CropForm::button1_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(0, 29);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(157, 20);
			this->textBox1->TabIndex = 2;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(56, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Crop Area";
			// 
			// CropForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 308);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"CropForm";
			this->Text = L"CropForm-1";
			this->Load += gcnew System::EventHandler(this, &CropForm::CropForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
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
				 }
			
				 if (FAILED(g_video.AppendSampleGrabber( &g_Listner )))
				 {
					 MessageBox::Show(L"Couldnt append the Hi-Pass filter"); // edit the filter name
				 }

				 if(FAILED(g_video.AppendPreview( (HWND)pictureBox1->Handle.ToPointer())))
				 {
					 MessageBox::Show(L"window 1 init failed");
				 }
				 if (FAILED(g_video.AppendSampleGrabber( &g_Highpass)))
				 {
					 MessageBox::Show(L"Couldnt append the Hi-Pass filter"); // edit the filter name
				 }

			
				 m_f1 = gcnew Form_2_Highpass();
				 m_f1->Show(this);
				 
				 if(FAILED(g_video.AppendPreview( (HWND)m_f1->pictureBox1->Handle.ToPointer())))
				 {
					 MessageBox::Show(L"window 2 init failed");
				 }
				 if(FAILED(g_video.Play()))
				 {
					 MessageBox::Show(L"play failed");
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
			 
			 textBox1->Text = g_settings.m_Crop.left+ "," + g_settings.m_Crop.top + "," + g_settings.m_Crop.right + "," + g_settings.m_Crop.bottom;
		 }

private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
// 			 if(FAILED(g_video.SetCrop(g_settings.m_Crop)))
// 			 {
// 				 MessageBox::Show("crop failed");
// 			 }
			

		 }
};
}
