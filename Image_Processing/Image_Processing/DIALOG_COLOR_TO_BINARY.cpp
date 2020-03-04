// DIALOG_COLOR_TO_BINARY.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DIALOG_COLOR_TO_BINARY.h"
#include "afxdialogex.h"
#include "winuser.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

// DIALOG_COLOR_TO_BINARY 对话框

IMPLEMENT_DYNAMIC(DIALOG_COLOR_TO_BINARY, CDialogEx)

DIALOG_COLOR_TO_BINARY::DIALOG_COLOR_TO_BINARY(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COLOR_TO_BINARY, pParent)

	, CursorPos(_T("抬起点\nx=000\ny=000"))
	, CursorPosDown(_T("按下点\nx=000\ny=000"))
	, ThresholdValueView(_T("000"))
{

}

DIALOG_COLOR_TO_BINARY::~DIALOG_COLOR_TO_BINARY()
{
}

void DIALOG_COLOR_TO_BINARY::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SEQ_COLOR_TO_BINARY, Img_Static);
	DDX_Text(pDX, IDC_CursorPos, CursorPos);
	DDX_Text(pDX, IDC_CursorPosDown, CursorPosDown);
	DDX_Text(pDX, IDC_SHRESHOLD_VALUE, ThresholdValueView);
}

BOOL DIALOG_COLOR_TO_BINARY::OnInitDialog()//DoModal后第一个执行的函数
{
	CDialogEx::OnInitDialog();

	DrawPic(srcImage);

	return true;
}

BEGIN_MESSAGE_MAP(DIALOG_COLOR_TO_BINARY, CDialogEx)//系统响应函数
	ON_BN_CLICKED ( IDC_BUTTON1, &DIALOG_COLOR_TO_BINARY::OnBnClickedButton1 )
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON2, &DIALOG_COLOR_TO_BINARY::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &DIALOG_COLOR_TO_BINARY::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DIALOG_COLOR_TO_BINARY::OnBnClickedCancel)
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
void DIALOG_COLOR_TO_BINARY::SendImage ( Mat img )
{
	img.copyTo(srcImage);
	//将调整大小后的图像传输到这个类里面
}
int DIALOG_COLOR_TO_BINARY::GetPicHeight ()
{
	return PicHeight;
}

bool DIALOG_COLOR_TO_BINARY::ImageCopy ( const CImage &srcImage, CImage &destImage )
{
	int i, j;//循环变量
	if (srcImage.IsNull ())
		return FALSE;
	//源图像参数
	BYTE* srcPtr = (BYTE*)srcImage.GetBits ();
	int srcBitsCount = srcImage.GetBPP ();
	int srcWidth = srcImage.GetWidth ();
	int srcHeight = srcImage.GetHeight ();
	int srcPitch = srcImage.GetPitch ();
	//销毁原有图像
	if (!destImage.IsNull ())
	{
		destImage.Destroy ();
	}
	//创建新图像
	if (srcBitsCount == 32)   //支持alpha通道
	{
		destImage.Create ( srcWidth, srcHeight, srcBitsCount, 1 );
	}
	else
	{
		destImage.Create ( srcWidth, srcHeight, srcBitsCount, 0 );
	}
	//加载调色板
	if (srcBitsCount <= 8 && srcImage.IsIndexed ())//需要调色板
	{
		RGBQUAD pal[256];
		int nColors = srcImage.GetMaxColorTableEntries ();
		if (nColors>0)
		{
			srcImage.GetColorTable ( 0, nColors, pal );
			destImage.SetColorTable ( 0, nColors, pal );//复制调色板程序
		}
	}
	//目标图像参数
	BYTE *destPtr = (BYTE*)destImage.GetBits ();

	int destPitch = destImage.GetPitch ();
	//复制图像数据
	for (i = 0; i<srcHeight; i++)
	{
		memcpy ( destPtr + i*destPitch, srcPtr + i*srcPitch, abs ( srcPitch ) );
	}

	return TRUE;
}
int DIALOG_COLOR_TO_BINARY::GetPicWidth ()
{
	return PicWidth;
}

void DIALOG_COLOR_TO_BINARY::DrawPic (Mat tempImage)
{
	if (!m_CImage.IsNull())
		m_CImage.Destroy();
	m_CImage.Create(PicWidth, PicHeight, 24);
	uchar* ps;
	uchar* pimg = (uchar*)m_CImage.GetBits();
	int step = m_CImage.GetPitch();
	for (int i = 0; i < PicHeight; ++i)
	{
		ps = (tempImage.ptr<uchar>(i));
		for (int j = 0; j < PicWidth; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				*(pimg + i*step + j * 3 + k) = ps[j * 3 + k];
			}
		}
	}

	HBITMAP hBmp = m_CImage.Detach ();

	Img_Static.SetBitmap ( hBmp );
	Img_Static.SetWindowPos ( NULL,
		0,
		0,
		PicWidth,
		PicHeight,
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER );
}


// DIALOG_COLOR_TO_BINARY 消息处理程序


void DIALOG_COLOR_TO_BINARY::OnView()
{
	double Avg = 0;
	//问题处，无法成功获取m_CImage的内存地址等参数,改用Mat类型
	//int nrow = m_CImage.GetPitch();//获得m_CImage每一行像素的RGB所占用的存储空间的大小
	//BYTE *psrc = (BYTE *)m_CImage.GetBits();//获得m_CImage最后一行的像素地址

	int minx = min(start.x, end.x);
	int maxx = max(start.x, end.x);
	int miny = min(start.y, end.y);
	int maxy = max(start.y, end.y);

	if (maxy > PicHeight || maxx > PicWidth)
	{
		MessageBox(TEXT("选择区域超出范围,请重新开始！"), TEXT("错误"));
		DrawPic(srcImage);
		FlagCancel = true;
		Flag = false;
		return;
	}
	Mat tempImage = srcImage.clone();
	for (int i = miny; i != maxy; i++)
	{
		for (int j = minx; j != maxx; j++)
		{
			/*Avg += (psrc[i*nrow + j * 3] + psrc[i*nrow + j * 3 + 1] + psrc[i*nrow + j * 3 + 2]) / (1.0*(maxx - minx)*(maxy - miny));*/
			Avg += (tempImage.at<Vec3b>(i, j)[0] + tempImage.at<Vec3b>(i, j)[1] + tempImage.at<Vec3b>(i, j)[2]) / (1.0*(maxx - minx)*(maxy - miny));
		}
	}
	ThresholdValue = Avg / 3 ;
	for (int i = 0; i != PicHeight; i++)
		for (int j = 0; j != PicWidth; j++)
		{
				if ((tempImage.at<Vec3b>(i, j)[0] + tempImage.at<Vec3b>(i, j)[1] + tempImage.at<Vec3b>(i, j)[2]) > Avg)
				{
					tempImage.at<Vec3b>(i, j)[0] = 255;
					tempImage.at<Vec3b>(i, j)[1] = 255;
					tempImage.at<Vec3b>(i, j)[2] = 255;
				}
				else
				{
					tempImage.at<Vec3b>(i, j)[0] = 0;
					tempImage.at<Vec3b>(i, j)[1] = 0;
					tempImage.at<Vec3b>(i, j)[2] = 0;
				}
		}
	ThresholdValueView.Format(TEXT("%.3d"), ThresholdValue);
	DrawPic(tempImage);
	UpdateData(FALSE);
}

int DIALOG_COLOR_TO_BINARY::GetThresholdValue()
{
	return ThresholdValue;
}
//开始按钮
void DIALOG_COLOR_TO_BINARY::OnBnClickedButton1 ()
{
	//开始按钮
	if (!FlagCancel)
		return;
	Flag = true;
	FlagCancel = false;
	/*SetCursor(LoadCursor(NULL,IDC_CROSS));*/
	/*MessageBox(TEXT("鼠标转换成功"));*/
	// TODO: 在此添加控件通知处理程序代码
}


void DIALOG_COLOR_TO_BINARY::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnLButtonDown(nFlags, point);
	if (!Flag)
		return;
	FlagDraw = TRUE;
	start.x = point.x;
	start.y = point.y; 
	g_rectangle = Rect(point.x, point.y, 0, 0);
	CursorPosDown.Format(TEXT("按下点\nx=%.3d\ny=%.3d"), point.x, point.y);
	UpdateData(FALSE);
	//CString str;
	//str.Format(TEXT("x= %d,y= %d"), point.x, point.y);
	//MessageBox(str);
	
}


void DIALOG_COLOR_TO_BINARY::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnLButtonUp(nFlags, point);
	if (!Flag)
		return;

	end.x = point.x;
	end.y = point.y;

	Flag = false;
	FlagDraw = false;
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	OnView();
}


void DIALOG_COLOR_TO_BINARY::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnMouseMove(nFlags, point);
	//这条语句有什么用？

	if (!Flag)
		return;
	if (!FlagDraw)
		return;
	Mat tempImage = srcImage.clone();
	g_rectangle.width = point.x - g_rectangle.x;
	g_rectangle.height = point.y - g_rectangle.y;
	rectangle(tempImage, g_rectangle.tl(), g_rectangle.br(), Scalar(0, 0, 255));
	DrawPic(tempImage);
	
	CursorPos.Format(TEXT("抬起点\nx=%.3d\ny=%.3d"), point.x, point.y);
	
	UpdateData(FALSE);



}

//撤销操作
void DIALOG_COLOR_TO_BINARY::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//撤销操作
	DrawPic(srcImage);
	FlagCancel = true;
	Flag = false;
}


void DIALOG_COLOR_TO_BINARY::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();

	
}


void DIALOG_COLOR_TO_BINARY::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();

}

//
BOOL DIALOG_COLOR_TO_BINARY::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!Flag)
		return true;
	CPoint p;
	CRect pRect;
	GetClientRect(pRect);
	//先获取鼠标相对于屏幕的坐标
	GetCursorPos(&p);

	//最后把当前鼠标的坐标转化为相对于rect的坐标
	ScreenToClient(&p);
	CRect rect;
	GetDlgItem(IDC_STATIC_SEQ_COLOR_TO_BINARY)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	if (rect.PtInRect(p))
	{
		//在指定控件区域设置鼠标光标样式为十字
		SetCursor(LoadCursor(NULL, IDC_CROSS));
		return TRUE;
	}
	else
	{
		//鼠标光标离开指定区域恢复为箭头样式显示
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return TRUE;
	}

	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}



BOOL DIALOG_COLOR_TO_BINARY::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialogEx::OnEraseBkgnd(pDC);//返回偏灰的背景
	//return TRUE;//返回纯白色的背景
}

//返回背景画刷为空
HBRUSH DIALOG_COLOR_TO_BINARY::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	//// TODO:  在此更改 DC 的任何特性

	//// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	//return hbr;

	//设置透明背景模式
	/*pDC->SetBkMode(TRANSPARENT);*/
	//设置背景画刷为空
	if (FirstTime)
	{
		FirstTime--;
		HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
		return hbr;
	}
	else return (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
}
