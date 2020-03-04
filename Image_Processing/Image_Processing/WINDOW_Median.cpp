// WINDOW_Median.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "WINDOW_Median.h"
#include "afxdialogex.h"


// WINDOW_Median 对话框

IMPLEMENT_DYNAMIC(WINDOW_Median, CDialogEx)

WINDOW_Median::WINDOW_Median(CWnd* pParent /*=NULL*/)
	: CDialogEx(WINDOW_Median::IDD, pParent)
	, WINDOW_SIZE(0)
{
	WINDOW_SIZE = 3;
	
}

WINDOW_Median::~WINDOW_Median()
{
}

void WINDOW_Median::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, WINDOW_SIZE);
	((CButton *)GetDlgItem(IDC_RADIO_SORT_Bubble))->SetCheck(TRUE); //选上
	((CButton *)GetDlgItem(IDC_RADIO2_SORT_His))->SetCheck(FALSE);//不选上
	((CButton *)GetDlgItem(IDC_RADIO3_SORT_AUTO))->SetCheck(FALSE);//不选上
}


BEGIN_MESSAGE_MAP(WINDOW_Median, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &WINDOW_Median::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &WINDOW_Median::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &WINDOW_Median::OnBnClickedCancel)
END_MESSAGE_MAP()


// WINDOW_Median 消息处理程序
int WINDOW_Median::GetWindowSize()
{
	return WINDOW_SIZE;
}

int WINDOW_Median::GetSortType()
{
	return Flag_sort;
}

void WINDOW_Median::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void WINDOW_Median::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//UpdateData(TRUE);//调用一次DoDataExchange;TRUE表示控件值读到变量，FALSE表示变量值输出到控件（在DoDataExchange函数内关联的控件和变量）
	if (((CButton *)GetDlgItem(IDC_RADIO_SORT_Bubble))->GetCheck())
	{
		Flag_sort = 0;
	}
	if (((CButton *)GetDlgItem(IDC_RADIO2_SORT_His))->GetCheck())
	{
		Flag_sort = 1;
	}
	if (((CButton *)GetDlgItem(IDC_RADIO3_SORT_AUTO))->GetCheck())
	{
		Flag_auto = TRUE;
	}
	flag_success = TRUE;
	CDialogEx::OnOK();
}



void WINDOW_Median::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	flag_success = FALSE;
	CDialogEx::OnCancel();
}
