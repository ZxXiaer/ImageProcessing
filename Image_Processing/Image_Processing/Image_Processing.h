
// Image_Processing.h : Image_Processing 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "math.h"
#include "string.h"
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc\imgproc.hpp"
//excel
#include "CApplication.h"
#include "CFont0.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"

// CImage_ProcessingApp:
// 有关此类的实现，请参阅 Image_Processing.cpp
//

class CImage_ProcessingApp : public CWinAppEx
{
public:
	CImage_ProcessingApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CImage_ProcessingApp theApp;
