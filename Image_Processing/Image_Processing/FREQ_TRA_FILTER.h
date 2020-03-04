#pragma once


// FREQ_TRA_FILTER �Ի���

class FREQ_TRA_FILTER : public CDialogEx
{
	DECLARE_DYNAMIC(FREQ_TRA_FILTER)

public:
	FREQ_TRA_FILTER(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FREQ_TRA_FILTER();

// �Ի�������
	enum { IDD = IDD_FREQ_TRA_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
