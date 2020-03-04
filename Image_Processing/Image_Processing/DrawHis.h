#pragma once


// DrawHis 对话框

class DrawHis : public CDialogEx
{
	DECLARE_DYNAMIC(DrawHis)

public:
	DrawHis(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DrawHis();

// 对话框数据
	enum { IDD = IDD_DrawHis };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
	void SetDatas(int his_1[],int his_2[],int his_3[]);
	void SetDatas(int his[]);
	void SetTitle(CString title1, CString title2, CString title3);
	void SetTitle(CString title);
	CString Title;
	CString Title1;
	CString Title2;
	CString Title3;
	int* His=NULL;
	int* His_1=NULL;
	int* His_2=NULL;
	int* His_3=NULL;

};
