#include "stdafx.h"
#include "Image_Processing.h"
#include "DrawHis.h"
#include "afxdialogex.h"


// DrawHis 对话框

IMPLEMENT_DYNAMIC(DrawHis, CDialogEx)

DrawHis::DrawHis(CWnd* pParent /*=NULL*/)
	: CDialogEx(DrawHis::IDD, pParent)
{

}

DrawHis::~DrawHis()
{
}

void DrawHis::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DrawHis, CDialogEx)
	ON_BN_CLICKED(IDOK, &DrawHis::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// DrawHis 消息处理程序

void DrawHis::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
void DrawHis::SetDatas(int his[])
{
	His = his;
}
void DrawHis::SetDatas(int his_1[],int his_2[],int his_3[])
{
	His_1 = his_1;
	His_2 = his_2;
	His_3 = his_3;

}
void DrawHis::SetTitle(CString title)
{
	Title = title;
}
void DrawHis::SetTitle(CString title1, CString title2, CString title3)
{
	Title1 = title1;
	Title2 = title2;
	Title3 = title3;
}
void DrawHis::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	CDC *pDC = GetDC();//获取CDC
	CString str;
	if (His == NULL)
	{
		// --------------------------------------------------------------------------------------------------
		//		描述：定义三个矩形框
		//---------------------------------------------------------------------------------------------------
		pDC->Rectangle(100, 20, 620, 270);//矩形框，以矩形框为坐标轴 画直方图
		pDC->Rectangle(100, 295, 620, 545);//矩形框，以矩形框为坐标轴 画直方图
		pDC->Rectangle(100, 570, 620, 820);//矩形框，以矩形框为坐标轴 画直方图
		pDC->TextOut(20, 30, Title1);
		pDC->TextOut(20, 300, Title2);
		pDC->TextOut(20, 570, Title3);
		// --------------------------------------------------------------------------------------------------
		//		描述：矩形框绘制刻度
		//---------------------------------------------------------------------------------------------------
		for (int i = 0; i <= 5; i++)
		{	//绘制X轴刻度
			str.Format(_T("%d"), i * 50);
			pDC->TextOut(97 + 98 * i, 275, str);
			pDC->MoveTo(i * 100 + 100, 270);
			pDC->LineTo(i * 100 + 100, 265);

			pDC->TextOut(97 + 98 * i, 550, str);
			pDC->MoveTo(i * 100 + 100, 545);
			pDC->LineTo(i * 100 + 100, 540);

			pDC->TextOut(97 + 98 * i, 825, str);
			pDC->MoveTo(i * 100 + 100, 820);
			pDC->LineTo(i * 100 + 100, 815);
		}

		for (int i = 1; i <= 10; i++)
		{
			//绘制Y轴刻度
			pDC->MoveTo(100, 270 - 25 * i);
			pDC->LineTo(105, 270 - 25 * i);

			pDC->MoveTo(100, 545 - 25 * i);
			pDC->LineTo(105, 545 - 25 * i);

			pDC->MoveTo(100, 820 - 25 * i);
			pDC->LineTo(105, 820 - 25 * i);
		}

		// --------------------------------------------------------------------------------------------------
		//		描述：创建画笔
		//---------------------------------------------------------------------------------------------------
		CPen *pRedPen = new CPen(); //创建画笔对象，用来画直方图
		CPen *pGreenPen = new CPen(); //创建画笔对象，用来画直方图
		CPen *pBluePen = new CPen(); //创建画笔对象，用来画直方图

		pRedPen->CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); //红色画笔
		pGreenPen->CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); //绿色画笔
		pBluePen->CreatePen(PS_SOLID, 1, RGB(0, 0, 255)); //蓝色画笔
		// --------------------------------------------------------------------------------------------------
		//		描述：画第一个分量
		//---------------------------------------------------------------------------------------------------
		//CGdiObject *pOldPen = pDC->SelectObject(pRedPen);//选用红色画笔
		pDC->SelectObject(pRedPen);
		//统计最大值
		int max_1 = 0;
		for (int i = 0; i < 256; i++)
		{
			if (max_1 < His_1[i])
			{
				max_1 = His_1[i];
			}
		}

		//以最大值为基准绘制直方图
		for (int j = 0; j < 256; j++)
		{
			pDC->MoveTo(j * 2 + 105, 268);
			pDC->LineTo(j * 2 + 105, 268 - His_1[j] * 248 / max_1);
		}
		// --------------------------------------------------------------------------------------------------
		//		描述：画第二个分量
		//---------------------------------------------------------------------------------------------------
		pDC->SelectObject(pGreenPen);
		//统计最大值
		int max_2 = 0;
		for (int i = 0; i < 256; i++)
		{
			if (max_2 < His_2[i])
			{
				max_2 = His_2[i];
			}
		}

		//以最大值为基准绘制直方图
		for (int j = 0; j < 256; j++)
		{
			pDC->MoveTo(j * 2 + 105, 543);
			pDC->LineTo(j * 2 + 105, 543 - His_2[j] * 248 / max_2);
		}
		// --------------------------------------------------------------------------------------------------
		//		描述：画第三个分量
		//---------------------------------------------------------------------------------------------------
		pDC->SelectObject(pBluePen);
		//统计最大值
		int max_3 = 0;
		for (int i = 0; i < 256; i++)
		{
			if (max_3 < His_3[i])
			{
				max_3 = His_3[i];
			}
		}

		//以最大值为基准绘制直方图
		for (int j = 0; j < 256; j++)
		{
			pDC->MoveTo(j * 2 + 105, 818);
			pDC->LineTo(j * 2 + 105, 818 - His_3[j] * 248 / max_3);
		}
		delete pRedPen;
		delete pGreenPen;
		delete pBluePen;
		ReleaseDC(pDC);
	}
}