#pragma once
#include "afxwin.h"


// WINDOW_Median �Ի���

class WINDOW_Median : public CDialogEx
{
	DECLARE_DYNAMIC(WINDOW_Median)

public:
	WINDOW_Median(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~WINDOW_Median();

// �Ի�������
	enum { IDD = IDD_WINDOW_Median };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
    int GetWindowSize();
	int GetSortType();
	int WINDOW_SIZE;
	int Flag_sort = 0;
	bool Flag_auto = 0;
	bool flag_success= FALSE;
	afx_msg void OnBnClickedOk();
	
	afx_msg void OnBnClickedCancel();
};
