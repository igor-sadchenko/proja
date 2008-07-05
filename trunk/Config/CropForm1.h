#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

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
	public ref class CropForm : public Form
	{

	public:
		CropForm(int* varSettings)
		{
			m_varSettings = varSettings;
			InitializeComponent();
			instance = this;
			m_bitmap0 = gcnew Bitmap("0.jpg");
			m_bitmap1 = gcnew Bitmap("1.jpg");
			m_bitmap2 = gcnew Bitmap("2.jpg");
			m_bitmap3 = gcnew Bitmap("3.jpg");

		}
	public: System::Windows::Forms::CheckBox^  checkBox1;
	public:static CropForm^ instance;

	public: System::Windows::Forms::PictureBox^  pictureBox1;
	public: System::Windows::Forms::TextBox^  textBox1;
	public: System::Windows::Forms::PictureBox^  pictureBox2;

	public: 
		System::Drawing::Bitmap^ m_bitmap0;
		System::Drawing::Bitmap^ m_bitmap1;
		System::Drawing::Bitmap^ m_bitmap2;
		System::Drawing::Bitmap^ m_bitmap3;
	public: 

	public: 

	public: 


		int* m_varSettings;
		
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
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			this->SuspendLayout();
			// 
			// checkBox1
			// 
			this->checkBox1->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(407, 43);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(72, 23);
			this->checkBox1->TabIndex = 0;
			this->checkBox1->Text = L"Calibrate...";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &CropForm::checkBox1_CheckedChanged);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(1, 118);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(348, 260);
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(1, 12);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox1->Size = System::Drawing::Size(348, 100);
			this->textBox1->TabIndex = 2;
			// 
			// pictureBox2
			// 
			this->pictureBox2->Location = System::Drawing::Point(372, 118);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(135, 135);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox2->TabIndex = 3;
			this->pictureBox2->TabStop = false;
			// 
			// CropForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(519, 379);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->checkBox1);
			this->Name = L"CropForm";
			this->Text = L"CropForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 g_Crop.m_isCropMode = checkBox1->Checked;
				 if(checkBox1->Checked)
				 {
					 checkBox1->Text = "Ready...";
					 pictureBox2->Image = m_bitmap0;
					 textBox1->AppendText("Touch the top left corner of the screen\r\n");
				 }
				 else 
					 checkBox1->Text = "Calibrate";

			 }
	};
}
