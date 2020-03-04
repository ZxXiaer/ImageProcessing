// WINDOW_Median.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "WINDOW_Median.h"
#include "afxdialogex.h"


// WINDOW_Median �Ի���

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
	((CButton *)GetDlgItem(IDC_RADIO_SORT_Bubble))->SetCheck(TRUE); //ѡ��
	((CButton *)GetDlgItem(IDC_RADIO2_SORT_His))->SetCheck(FALSE);//��ѡ��
	((CButton *)GetDlgItem(IDC_RADIO3_SORT_AUTO))->SetCheck(FALSE);//��ѡ��
}


BEGIN_MESSAGE_MAP(WINDOW_Median, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &WINDOW_Median::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &WINDOW_Median::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &WINDOW_Median::OnBnClickedCancel)
END_MESSAGE_MAP()


// WINDOW_Median ��Ϣ�������
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void WINDOW_Median::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//UpdateData(TRUE);//����һ��DoDataExchange;TRUE��ʾ�ؼ�ֵ����������FALSE��ʾ����ֵ������ؼ�����DoDataExchange�����ڹ����Ŀؼ��ͱ�����
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	flag_success = FALSE;
	CDialogEx::OnCancel();
}
