#pragma once
#include "afxwin.h"


// Image_Diffrence �Ի���

class Image_Diffrence : public CDialogEx
{
	DECLARE_DYNAMIC(Image_Diffrence)

public:
	Image_Diffrence(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Image_Diffrence();

// �Ի�������
	enum { IDD = IDD_IMAGE_DIFFRENCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
