// DIALOG_COLOR_TO_BINARY.cpp : ʵ���ļ�
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

// DIALOG_COLOR_TO_BINARY �Ի���

IMPLEMENT_DYNAMIC(DIALOG_COLOR_TO_BINARY, CDialogEx)

DIALOG_COLOR_TO_BINARY::DIALOG_COLOR_TO_BINARY(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COLOR_TO_BINARY, pParent)

	, CursorPos(_T("̧���\nx=000\ny=000"))
	, CursorPosDown(_T("���µ�\nx=000\ny=000"))
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

BOOL DIALOG_COLOR_TO_BINARY::OnInitDialog()//DoModal���һ��ִ�еĺ���
{
	CDialogEx::OnInitDialog();

	DrawPic(srcImage);

	return true;
}

BEGIN_MESSAGE_MAP(DIALOG_COLOR_TO_BINARY, CDialogEx)//ϵͳ��Ӧ����
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
	//��������С���ͼ���䵽���������
}
int DIALOG_COLOR_TO_BINARY::GetPicHeight ()
{
	return PicHeight;
}

bool DIALOG_COLOR_TO_BINARY::ImageCopy ( const CImage &srcImage, CImage &destImage )
{
	int i, j;//ѭ������
	if (srcImage.IsNull ())
		return FALSE;
	//Դͼ�����
	BYTE* srcPtr = (BYTE*)srcImage.GetBits ();
	int srcBitsCount = srcImage.GetBPP ();
	int srcWidth = srcImage.GetWidth ();
	int srcHeight = srcImage.GetHeight ();
	int srcPitch = srcImage.GetPitch ();
	//����ԭ��ͼ��
	if (!destImage.IsNull ())
	{
		destImage.Destroy ();
	}
	//������ͼ��
	if (srcBitsCount == 32)   //֧��alphaͨ��
	{
		destImage.Create ( srcWidth, srcHeight, srcBitsCount, 1 );
	}
	else
	{
		destImage.Create ( srcWidth, srcHeight, srcBitsCount, 0 );
	}
	//���ص�ɫ��
	if (srcBitsCount <= 8 && srcImage.IsIndexed ())//��Ҫ��ɫ��
	{
		RGBQUAD pal[256];
		int nColors = srcImage.GetMaxColorTableEntries ();
		if (nColors>0)
		{
			srcImage.GetColorTable ( 0, nColors, pal );
			destImage.SetColorTable ( 0, nColors, pal );//���Ƶ�ɫ�����
		}
	}
	//Ŀ��ͼ�����
	BYTE *destPtr = (BYTE*)destImage.GetBits ();

	int destPitch = destImage.GetPitch ();
	//����ͼ������
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


// DIALOG_COLOR_TO_BINARY ��Ϣ�������


void DIALOG_COLOR_TO_BINARY::OnView()
{
	double Avg = 0;
	//���⴦���޷��ɹ���ȡm_CImage���ڴ��ַ�Ȳ���,����Mat����
	//int nrow = m_CImage.GetPitch();//���m_CImageÿһ�����ص�RGB��ռ�õĴ洢�ռ�Ĵ�С
	//BYTE *psrc = (BYTE *)m_CImage.GetBits();//���m_CImage���һ�е����ص�ַ

	int minx = min(start.x, end.x);
	int maxx = max(start.x, end.x);
	int miny = min(start.y, end.y);
	int maxy = max(start.y, end.y);

	if (maxy > PicHeight || maxx > PicWidth)
	{
		MessageBox(TEXT("ѡ�����򳬳���Χ,�����¿�ʼ��"), TEXT("����"));
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
//��ʼ��ť
void DIALOG_COLOR_TO_BINARY::OnBnClickedButton1 ()
{
	//��ʼ��ť
	if (!FlagCancel)
		return;
	Flag = true;
	FlagCancel = false;
	/*SetCursor(LoadCursor(NULL,IDC_CROSS));*/
	/*MessageBox(TEXT("���ת���ɹ�"));*/
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void DIALOG_COLOR_TO_BINARY::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnLButtonDown(nFlags, point);
	if (!Flag)
		return;
	FlagDraw = TRUE;
	start.x = point.x;
	start.y = point.y; 
	g_rectangle = Rect(point.x, point.y, 0, 0);
	CursorPosDown.Format(TEXT("���µ�\nx=%.3d\ny=%.3d"), point.x, point.y);
	UpdateData(FALSE);
	//CString str;
	//str.Format(TEXT("x= %d,y= %d"), point.x, point.y);
	//MessageBox(str);
	
}


void DIALOG_COLOR_TO_BINARY::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnMouseMove(nFlags, point);
	//���������ʲô�ã�

	if (!Flag)
		return;
	if (!FlagDraw)
		return;
	Mat tempImage = srcImage.clone();
	g_rectangle.width = point.x - g_rectangle.x;
	g_rectangle.height = point.y - g_rectangle.y;
	rectangle(tempImage, g_rectangle.tl(), g_rectangle.br(), Scalar(0, 0, 255));
	DrawPic(tempImage);
	
	CursorPos.Format(TEXT("̧���\nx=%.3d\ny=%.3d"), point.x, point.y);
	
	UpdateData(FALSE);



}

//��������
void DIALOG_COLOR_TO_BINARY::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��������
	DrawPic(srcImage);
	FlagCancel = true;
	Flag = false;
}


void DIALOG_COLOR_TO_BINARY::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();

	
}


void DIALOG_COLOR_TO_BINARY::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();

}

//
BOOL DIALOG_COLOR_TO_BINARY::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!Flag)
		return true;
	CPoint p;
	CRect pRect;
	GetClientRect(pRect);
	//�Ȼ�ȡ����������Ļ������
	GetCursorPos(&p);

	//���ѵ�ǰ��������ת��Ϊ�����rect������
	ScreenToClient(&p);
	CRect rect;
	GetDlgItem(IDC_STATIC_SEQ_COLOR_TO_BINARY)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	if (rect.PtInRect(p))
	{
		//��ָ���ؼ����������������ʽΪʮ��
		SetCursor(LoadCursor(NULL, IDC_CROSS));
		return TRUE;
	}
	else
	{
		//������뿪ָ������ָ�Ϊ��ͷ��ʽ��ʾ
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return TRUE;
	}

	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}



BOOL DIALOG_COLOR_TO_BINARY::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CDialogEx::OnEraseBkgnd(pDC);//����ƫ�ҵı���
	//return TRUE;//���ش���ɫ�ı���
}

//���ر�����ˢΪ��
HBRUSH DIALOG_COLOR_TO_BINARY::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	//// TODO:  �ڴ˸��� DC ���κ�����

	//// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	//return hbr;

	//����͸������ģʽ
	/*pDC->SetBkMode(TRANSPARENT);*/
	//���ñ�����ˢΪ��
	if (FirstTime)
	{
		FirstTime--;
		HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
		return hbr;
	}
	else return (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
}
