#include "StdAfx.h"
using namespace config;
using namespace System;

void FrameListner::OnSampleArrived( BYTE*pdata,long size )
{
	m_WidthBytes = m_bmpinfo.biWidth*3;
	m_bmpBits = pdata;
}



void MonochromeListner::OnSampleArrived( BYTE*pdata,long size )
{
	ComponentLabelingBlobDetector detector(&m_bmpinfo);
	detector.InitializeBitmap(pdata);
	detector.ApplyMonochrome();
}

void NoiseRemovalListner::OnSampleArrived( BYTE*pdata,long size )
{
	ComponentLabelingBlobDetector detector(&m_bmpinfo);
	detector.InitializeBitmap(pdata);
	detector.ApplyGaussianFilter(g_settings.getNoise());
}

void CroppingListner::OnSampleArrived( BYTE*pdata,long size )
{
	list <twBlob> blobs ;
	if (m_isCropMode)
	{
		ComponentLabelingBlobDetector detector(&m_bmpinfo);
		detector.InitializeBitmap(pdata);
		detector.DetectBlobs(pdata,blobs);

		

		if(blobs.size() == 1)
		{
			list<twBlob>::iterator it = blobs.begin();
			g_settings.m_trapozoid[m_cropPoints][0] = it->m_center.m_x;
			g_settings.m_trapozoid[m_cropPoints][1] = it->m_center.m_y;
			m_cropPoints++;

			config::CropForm::instance->textBox1->AppendText(it->m_center.m_x + " " + it->m_center.m_y  + "\r\n");


			if(m_cropPoints == 1)
			{
				CropForm::instance->pictureBox2->Image = CropForm::instance->m_bitmap1;
				config::CropForm::instance->textBox1->AppendText("Touch the top right corner of the screen\r\n");
			}
			else if(m_cropPoints == 2)
			{
				CropForm::instance->pictureBox2->Image = CropForm::instance->m_bitmap2;
				config::CropForm::instance->textBox1->AppendText("Touch the bottom right corner of the screen\r\n");
			}
			else if(m_cropPoints == 3)
			{
				CropForm::instance->pictureBox2->Image = CropForm::instance->m_bitmap3;
				config::CropForm::instance->textBox1->AppendText("Touch the bottm left corner of the screen\r\n");
			}
			else if(m_cropPoints == 4)
			{
				CropForm::instance->pictureBox2->Image = nullptr;
				config::CropForm::instance->textBox1->AppendText("Don, you dont have to flip \r\n");
				m_cropPoints = 0;
				m_isCropMode = false;
				config::CropForm::instance->checkBox1->Checked = false;
			}
		}
	}
}

// void CroppingListner::OnSampleArrived( BYTE*pdata,long size )
// {
// 	list <twBlob> blobs ;
// 	if (m_isCropMode)
// 	{
// 		ComponentLabelingBlobDetector detector(&m_bmpinfo);
// 		detector.InitializeBitmap(pdata);
// 		detector.DetectBlobs(pdata,blobs);
// 
// 		list<twBlob>::iterator it;
// 		int xmin = INT_MAX;
// 		int xmax = INT_MIN;
// 		int ymin = INT_MAX;
// 		int ymax = INT_MIN;
// 		config::CropForm::instance->textBox1->Text = "";
// 		if(blobs.size() != 2)
// 		{
// 			config::CropForm::instance->textBox1->Text = "use just two fingers please\r\n";
// 			return;
// 		}
// 		for (it = blobs.begin(); it != blobs.end(); it++)
// 		{
// 			if(it->m_center.m_x < xmin)
// 				xmin = it->m_center.m_x;
// 			if(it->m_center.m_y < ymin)
// 				ymin = it->m_center.m_y;
// 			if(it->m_center.m_x > xmax)
// 				xmax = it->m_center.m_x;
// 			if(it->m_center.m_y > ymax)
// 				ymax = it->m_center.m_y;
// 			config::CropForm::instance->textBox1->Text += it->m_center.m_x + " " + it->m_center.m_y  + "\r\n";
// 		}
// 		if(blobs.size())
// 		{
// 			config::CropForm::instance->checkBox1->Checked = false;
// 			
// 			config::CropForm::instance->textBox1->Text += xmin+ " " +ymin+ " "+xmax+ " "+ymax;
// 			g_settings.m_Crop.left = xmin;
// 			g_settings.m_Crop.top = ymin;
// 			g_settings.m_Crop.right = xmax;
// 			g_settings.m_Crop.bottom = ymax;
// 		}
// 
// 	}

