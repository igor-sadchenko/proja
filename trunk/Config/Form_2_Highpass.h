#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace config {

	/// <summary>
	/// Summary for Form_2_Highpass
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form_2_Highpass : public System::Windows::Forms::Form
	{
	private:
		int* m_varSettings;
		System::String^ m_formName;

	public:
		Form_2_Highpass(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		Form_2_Highpass(int* varSettings, System::String^ name)
		{
			m_varSettings = varSettings;
			m_formName = name;
			InitializeComponent();
			trackBar1->Value = *(g_settings.m_valNoise);
			this->Text = m_formName + " - Val: " + (trackBar1->Value).ToString();

			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form_2_Highpass()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::PictureBox^  pictureBox1;
	protected: 

	protected: 
	private: System::Windows::Forms::TrackBar^  trackBar1;


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
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(12, 68);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(295, 254);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(5, 12);
			this->trackBar1->Maximum = 200;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(308, 45);
			this->trackBar1->TabIndex = 1;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &Form_2_Highpass::trackBar1_Scroll);
			// 
			// Form_2_Highpass
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(318, 334);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"Form_2_Highpass";
			this->Text = L"Form_2_Highpass";
			this->Load += gcnew System::EventHandler(this, &Form_2_Highpass::Form_2_Highpass_Load);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form_2_Highpass::Form_2_Highpass_FormClosing);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) {
				 *m_varSettings = trackBar1->Value;
			this->Text = m_formName + " - Val: " + (trackBar1->Value).ToString();
			 }
	private: System::Void Form_2_Highpass_Load(System::Object^  sender, System::EventArgs^  e) {

				
			 }
	
	private: System::Void Form_2_Highpass_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {

			 }
	};
}
