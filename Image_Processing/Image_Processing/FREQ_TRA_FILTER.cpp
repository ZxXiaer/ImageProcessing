// FREQ_TRA_FILTER.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "FREQ_TRA_FILTER.h"
#include "afxdialogex.h"


// FREQ_TRA_FILTER �Ի���

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
	//����RADIO BUTTUN��ʼ״̬
	((CButton *)GetDlgItem(IDC_RADIO_LOW))->SetCheck(TRUE); //ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_HIGH))->SetCheck(FALSE);//��ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_IPF))->SetCheck(TRUE); //ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_BPF))->SetCheck(FALSE);//��ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_GPF))->SetCheck(FALSE);//��ѡ��
	DDX_Text(pDX, IDC_EDIT1, Filter_Size);
	DDX_Text(pDX, IDC_EDIT2, Filter_Level);
	DDV_MinMaxInt(pDX, Filter_Level, 0, 10);
}


BEGIN_MESSAGE_MAP(FREQ_TRA_FILTER, CDialogEx)
	
	ON_BN_CLICKED(IDOK, &FREQ_TRA_FILTER::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &FREQ_TRA_FILTER::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &FREQ_TRA_FILTER::OnEnChangeEdit2)
END_MESSAGE_MAP()


// FREQ_TRA_FILTER ��Ϣ�������



void FREQ_TRA_FILTER::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ȡRADIO BUTTUN״̬
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void FREQ_TRA_FILTER::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
