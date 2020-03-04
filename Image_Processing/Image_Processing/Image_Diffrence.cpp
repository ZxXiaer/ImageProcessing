// Image_Diffrence.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "Image_Diffrence.h"
#include "afxdialogex.h"


// Image_Diffrence �Ի���

IMPLEMENT_DYNAMIC(Image_Diffrence, CDialogEx)

Image_Diffrence::Image_Diffrence(CWnd* pParent /*=NULL*/)
	: CDialogEx(Image_Diffrence::IDD, pParent)
{

}

Image_Diffrence::~Image_Diffrence()
{
}

void Image_Diffrence::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC2, Picture_Back);
	DDX_Control(pDX, IDC_STATIC1, Picture_Front);
}


BEGIN_MESSAGE_MAP(Image_Diffrence, CDialogEx)
	ON_BN_CLICKED(IDOK, &Image_Diffrence::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &Image_Diffrence::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Image_Diffrence::OnBnClickedButton2)
END_MESSAGE_MAP()


// Image_Diffrence ��Ϣ�������
CString Image_Diffrence::GetPathName()
{
	return PathName;
}
CString Image_Diffrence::GetPathName_back()
{
	return PathName_back;
}
void Image_Diffrence::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_CImage.IsNull() || m_CImage_back.IsNull())
	{
		MessageBox(L"ͼƬѡ����ȷ��������ѡ��", L"����");
		return;
	}

	CDialogEx::OnOK();
}


void Image_Diffrence::OnBnClickedButton1()
{
	
	CFileDialog dlg(TRUE);    //Ϊ���ļ�����һ������
	if (IDOK == dlg.DoModal())    //���ú�����һ���Ի��򣬲��ж��Ƿ�򿪳ɹ�
	{
		if (!m_CImage.IsNull())
		{
			m_CImage.Destroy();//�ж��Ƿ��Ѿ���ͼƬ���еĻ����沢�������
		}
	}
	PathName=dlg.GetPathName();//���ͼƬ�ĵ�ַ
	if (PathName.IsEmpty())
	{
		MessageBox(L"ͼƬѡ����ȷ��������ѡ��", L"����");
		return;
	}
	m_CImage.Load(PathName);
	int w = m_CImage.GetWidth();
	int h = m_CImage.GetHeight();
	//CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC1);
	HBITMAP hBmp = m_CImage.Detach();
	Picture_Front.SetBitmap(hBmp);
	//Picture_Front.SetWindowPos(NULL,
	//	0,
	//	0,
	//	w,
	//	h,
	//	SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER );
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void Image_Diffrence::OnBnClickedButton2()
{
	CFileDialog dlg(TRUE);    //Ϊ���ļ�����һ������
	if (IDOK == dlg.DoModal())    //���ú�����һ���Ի��򣬲��ж��Ƿ�򿪳ɹ�
	{
		if (!m_CImage_back.IsNull())
		{
			m_CImage_back.Destroy();//�ж��Ƿ��Ѿ���ͼƬ���еĻ����沢�������
		}
	}
	PathName_back=dlg.GetPathName();//���ͼƬ�ĵ�ַ�����Ҽ���ͼƬ
	if (PathName_back.IsEmpty())
	{
		MessageBox(L"ͼƬѡ����ȷ��������ѡ��", L"����");
		return;
	}
	m_CImage_back.Load(PathName_back);
	int w_back = m_CImage_back.GetWidth();
	int h_back = m_CImage_back.GetHeight();
	//CStatic* pWnd_back = (CStatic*)GetDlgItem(IDC_STATIC2);
	HBITMAP hBmp_back = m_CImage_back.Detach();  
	Picture_Back.SetBitmap(hBmp_back);
	//Picture_Back.SetWindowPos(NULL,
	//	0,
	//	0,
	//	w_back,
	//	h_back,
	//	SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER );
}
