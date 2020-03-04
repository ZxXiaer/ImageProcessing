// FREQ_TRA_FILTER.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "FREQ_TRA_FILTER.h"
#include "afxdialogex.h"


// FREQ_TRA_FILTER 对话框

IMPLEMENT_DYNAMIC(FREQ_TRA_FILTER, CDialogEx)

FREQ_TRA_FILTER::FREQ_TRA_FILTER(CWnd* pParent /*=NULL*/)
	: CDialogEx(FREQ_TRA_FILTER::IDD, pParent)
	, Filter_Size(0)
	, Filter_Level(0)
{
	Filter_Size = 20;
	Filter_Level = 1;
}

FREQ_TRA_FILTER::~FREQ_TRA_FILTER()
{
}

void FREQ_TRA_FILTER::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//设置RADIO BUTTUN初始状态
	((CButton *)GetDlgItem(IDC_RADIO_LOW))->SetCheck(TRUE); //选上
	((CButton *)GetDlgItem(IDC_RADIO_HIGH))->SetCheck(FALSE);//不选上
	((CButton *)GetDlgItem(IDC_RADIO_IPF))->SetCheck(TRUE); //选上
	((CButton *)GetDlgItem(IDC_RADIO_BPF))->SetCheck(FALSE);//不选上
	((CButton *)GetDlgItem(IDC_RADIO_GPF))->SetCheck(FALSE);//不选上
	DDX_Text(pDX, IDC_EDIT1, Filter_Size);
	DDX_Text(pDX, IDC_EDIT2, Filter_Level);
	DDV_MinMaxInt(pDX, Filter_Level, 0, 10);
}


BEGIN_MESSAGE_MAP(FREQ_TRA_FILTER, CDialogEx)
	
	ON_BN_CLICKED(IDOK, &FREQ_TRA_FILTER::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &FREQ_TRA_FILTER::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &FREQ_TRA_FILTER::OnEnChangeEdit2)
END_MESSAGE_MAP()


// FREQ_TRA_FILTER 消息处理程序



void FREQ_TRA_FILTER::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//获取RADIO BUTTUN状态
	Flag_LOW=((CButton *)GetDlgItem(IDC_RADIO_LOW))->GetCheck();
	Flag_HIGH = ((CButton *)GetDlgItem(IDC_RADIO_HIGH))->GetCheck();
	Flag_IPF = ((CButton *)GetDlgItem(IDC_RADIO_IPF))->GetCheck();
	Flag_BPF = ((CButton *)GetDlgItem(IDC_RADIO_BPF))->GetCheck();
	Flag_GPF = ((CButton *)GetDlgItem(IDC_RADIO_GPF))->GetCheck();

	CDialogEx::OnOK();
}

int FREQ_TRA_FILTER::GetFilterSize()
{
	return Filter_Size;
}

int FREQ_TRA_FILTER::GetFilterLevel()
{
	return Filter_Level;
}

void FREQ_TRA_FILTER::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void FREQ_TRA_FILTER::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
