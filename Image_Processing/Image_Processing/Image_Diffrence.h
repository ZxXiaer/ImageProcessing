#pragma once
#include "afxwin.h"


// Image_Diffrence 对话框

class Image_Diffrence : public CDialogEx
{
	DECLARE_DYNAMIC(Image_Diffrence)

public:
	Image_Diffrence(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Image_Diffrence();

// 对话框数据
	enum { IDD = IDD_IMAGE_DIFFRENCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString  PathName;
	CString PathName_back;
	CString GetPathName();
	CString GetPathName_back();
	CImage m_CImage_back;
	CImage m_CImage;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CStatic Picture_Back;
	CStatic Picture_Front;
};
