#include "StdAfx.h"
using namespace config;

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

		list<twBlob>::iterator it;
		int xmin = INT_MAX;
		int xmax = INT_MIN;
		int ymin = INT_MAX;
		int ymax = INT_MIN;
		config::CropForm::instance->textBox1->Text = "";
		if(blobs.size() != 2)
		{
			config::CropForm::instance->textBox1->Text = "use just two fingers please\r\n";
			return;
		}
		for (it = blobs.begin(); it != blobs.end(); it++)
		{
			if(it->m_center.m_x < xmin)
				xmin = it->m_center.m_x;
			if(it->m_center.m_y < ymin)
				ymin = it->m_center.m_y;
			if(it->m_center.m_x > xmax)
				xmax = it->m_center.m_x;
			if(it->m_center.m_y > ymax)
				ymax = it->m_center.m_y;
			config::CropForm::instance->textBox1->Text += it->m_center.m_x + " " + it->m_center.m_y  + "\r\n";
		}
		if(blobs.size())
		{
			config::CropForm::instance->checkBox1->Checked = false;
			
			config::CropForm::instance->textBox1->Text += xmin+ " " +ymin+ " "+xmax+ " "+ymax;
			g_settings.m_Crop.left = xmin;
			g_settings.m_Crop.top = ymin;
			g_settings.m_Crop.right = xmax;
			g_settings.m_Crop.bottom = ymax;
		}

	}

}
