// Dialog_Gray_Level_Down.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "Dialog_Gray_Level_Down.h"
#include "afxdialogex.h"


// Dialog_Gray_Level_Down 对话框

IMPLEMENT_DYNAMIC(Dialog_Gray_Level_Down, CDialogEx)

Dialog_Gray_Level_Down::Dialog_Gray_Level_Down(CWnd* pParent /*=NULL*/)
	: CDialogEx(Dialog_Gray_Level_Down::IDD, pParent)
	, Obj_Level(0)
{

}

Dialog_Gray_Level_Down::~Dialog_Gray_Level_Down()
{
}

void Dialog_Gray_Level_Down::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT_GRAY_LEVEL, Obj_Level);
}
int Dialog_Gray_Level_Down::GetObjLevel()
{
	return Obj_Level;
}

BEGIN_MESSAGE_MAP(Dialog_Gray_Level_Down, CDialogEx)
	ON_BN_CLICKED(IDOK, &Dialog_Gray_Level_Down::OnBnClickedOk)
END_MESSAGE_MAP()


// Dialog_Gray_Level_Down 消息处理程序


void Dialog_Gray_Level_Down::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
