#pragma once


// FREQ_TRA_FILTER 对话框

class FREQ_TRA_FILTER : public CDialogEx
{
	DECLARE_DYNAMIC(FREQ_TRA_FILTER)

public:
	FREQ_TRA_FILTER(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FREQ_TRA_FILTER();

// 对话框数据
	enum { IDD = IDD_FREQ_TRA_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedOk();
	BOOL Flag_LOW;
	BOOL Flag_HIGH;
	BOOL Flag_IPF;
	BOOL Flag_BPF;
	BOOL Flag_GPF;
	int GetFilterSize();
	int GetFilterLevel();

	afx_msg void OnEnChangeEdit1();
	int Filter_Size;
	
	afx_msg void OnEnChangeEdit2();
	int Filter_Level;
};
