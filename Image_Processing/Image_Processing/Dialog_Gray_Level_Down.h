#pragma once


// Dialog_Gray_Level_Down �Ի���

class Dialog_Gray_Level_Down : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog_Gray_Level_Down)

public:
	Dialog_Gray_Level_Down(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Dialog_Gray_Level_Down();

// �Ի�������
	enum { IDD = IDD_DIALOG_GRAY_LEVEL_DOWN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int Obj_Level;
	int GetObjLevel();
};
