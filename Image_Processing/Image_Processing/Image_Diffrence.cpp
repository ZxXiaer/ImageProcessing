// Image_Diffrence.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "Image_Diffrence.h"
#include "afxdialogex.h"


// Image_Diffrence 对话框

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


// Image_Diffrence 消息处理程序
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
	// TODO:  在此添加控件通知处理程序代码
	if (m_CImage.IsNull() || m_CImage_back.IsNull())
	{
		MessageBox(L"图片选择不正确，请重新选择！", L"警告");
		return;
	}

	CDialogEx::OnOK();
}


void Image_Diffrence::OnBnClickedButton1()
{
	
	CFileDialog dlg(TRUE);    //为打开文件创建一个变量
	if (IDOK == dlg.DoModal())    //调用函数打开一个对话框，并判断是否打开成功
	{
		if (!m_CImage.IsNull())
		{
			m_CImage.Destroy();//判断是否已经有图片，有的话保存并进行清除
		}
	}
	PathName=dlg.GetPathName();//获得图片的地址
	if (PathName.IsEmpty())
	{
		MessageBox(L"图片选择不正确，请重新选择！", L"警告");
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
	// TODO:  在此添加控件通知处理程序代码
}


void Image_Diffrence::OnBnClickedButton2()
{
	CFileDialog dlg(TRUE);    //为打开文件创建一个变量
	if (IDOK == dlg.DoModal())    //调用函数打开一个对话框，并判断是否打开成功
	{
		if (!m_CImage_back.IsNull())
		{
			m_CImage_back.Destroy();//判断是否已经有图片，有的话保存并进行清除
		}
	}
	PathName_back=dlg.GetPathName();//获得图片的地址，并且加载图片
	if (PathName_back.IsEmpty())
	{
		MessageBox(L"图片选择不正确，请重新选择！", L"警告");
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
