
// Image_ProcessingView.cpp : CImage_ProcessingView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Image_Processing.h"
#endif

#include "Image_ProcessingDoc.h"
#include "Image_ProcessingView.h"
#include "DrawHis.h"
#include "Image_Diffrence.h"
#include "WINDOW_Median.h"
#include"Dialog_Gray_Level_Down.h"
#include "FREQ_TRA_FILTER.h"
#include "DIALOG_COLOR_TO_BINARY.h"
#include "ExtractHolesAndCracks.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace cv;
using namespace std;
//全局变量声明
Rect g_rectangle;
bool g_bDrawingBox = false;
bool WindowOpen = FALSE;
int StartPointx=0;
int StartPointy=0;
int EndPointx = 0;
int EndPointy = 0;
void on_MouseHandle(int event, int x, int y, int flags, void * param);
void DrawRectangle(cv::Mat & img, cv::Rect box);
// CImage_ProcessingView

IMPLEMENT_DYNCREATE(CImage_ProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CImage_ProcessingView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImage_ProcessingView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	//ON_WM_LBUTTONDOWN()//鼠标左键按下
	//ON_WM_MOUSEMOVE()//鼠标移动
	//ON_WM_LBUTTONUP()//鼠标左键抬起
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_OPEN, &CImage_ProcessingView::OnFileOpen)
	ON_COMMAND(IDM_DRAWLINE, &CImage_ProcessingView::OnDrawline)
	ON_COMMAND(ID_FILE_SAVE_AS, &CImage_ProcessingView::OnFileSaveAs)
//	ON_COMMAND(ID_VIEW_TOOLBAR, &CImage_ProcessingView::OnViewToolbar)
ON_COMMAND(ID_imgdown2, &CImage_ProcessingView::Onimgdown2)
ON_COMMAND(ID_imgdown4, &CImage_ProcessingView::Onimgdown4)
ON_COMMAND(ID_imgdiffrence, &CImage_ProcessingView::Onimgdiffrence)
ON_COMMAND(ID_changetogray, &CImage_ProcessingView::Onchangetogray)
ON_COMMAND(ID_denoise_by_add, &CImage_ProcessingView::Ondenoisebyadd)
ON_COMMAND(ID_FILE_OPEN_ALL, &CImage_ProcessingView::OnFileOpenAll)
ON_COMMAND(ID_meansfilter, &CImage_ProcessingView::Onmeansfilter)
ON_COMMAND(ID_medianfilter, &CImage_ProcessingView::Onmedianfilter)
ON_COMMAND(ID_His_View, &CImage_ProcessingView::OnHisView)
ON_COMMAND(ID_His_equalization, &CImage_ProcessingView::OnHisequalization)
ON_COMMAND(ID_His_normalized, &CImage_ProcessingView::OnHisnormalized)
ON_COMMAND(ID_Gray_Level_Down, &CImage_ProcessingView::OnGrayLevelDown)
ON_COMMAND(ID_FFT_2D, &CImage_ProcessingView::OnFft2d)
ON_COMMAND(ID_TRADITIONAL_FILTER, &CImage_ProcessingView::OnTraditionalFilter)
ON_COMMAND ( ID_SHOWRED, &CImage_ProcessingView::OnShowred )
ON_COMMAND ( ID_SHOWGREEN, &CImage_ProcessingView::OnShowgreen )
ON_COMMAND ( ID_SHOWBLUE, &CImage_ProcessingView::OnShowblue )
ON_COMMAND ( ID_RGB_TO_HSI, &CImage_ProcessingView::OnRgbToHsi )
ON_COMMAND ( ID_RGB_EQUALIZATION, &CImage_ProcessingView::OnRgbEqualization )
ON_COMMAND ( ID_COLOR_EQUALIZATION, &CImage_ProcessingView::OnColorEqualization )
ON_COMMAND ( ID_COLOR_SEG_TO_BINARY, &CImage_ProcessingView::OnColorSegToBinary )
ON_COMMAND(ID_COLOR_SEG_TO_BINARY_MFC, &CImage_ProcessingView::OnColorSegToBinaryMfc)
ON_COMMAND(ID_BAS_GLOBAL_THRESHOLD_SEG, &CImage_ProcessingView::OnBasGlobalThresholdSeg)
ON_COMMAND(ID_EXTRACT_HOLES_AND_CRACKS, &CImage_ProcessingView::OnExtractHolesAndCracks)
END_MESSAGE_MAP()

// CImage_ProcessingView 构造/析构

CImage_ProcessingView::CImage_ProcessingView()
{
	// TODO: 在此处添加构造代码
}

CImage_ProcessingView::~CImage_ProcessingView()
{
}

BOOL CImage_ProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

CImage CImage_ProcessingView::MattoCImage(Mat img)
{
	/*************************************/
	//1.读入Mat矩阵(cvMat一样),Mat img=imread("*.*");//cvLoadImage
	//确保转换前矩阵中的数据都是uchar(0~255)类型(不是的话量化到此区间)，这样才能显示。(初学者，包括我经常忘了此事)
	//2.根据矩阵大小创建(CImage::Create)新的的CImage类
	CImage CI;
	int w = img.cols;//宽
	int h = img.rows;//高
	int chinnels = img.channels();//通道数
	CI.Destroy();//创建前，最好使用它，防止重复创建，程序崩溃
	CI.Create(w, h, 8 * chinnels);

	//3.下来就是对CI进行赋值了，这里是最核心的地方，分二类讨论
	//	(1)如果是1个通道的图像(灰度图像) DIB格式才需要对调色板设置
	//	CImage中内置了调色板，我们要对他进行赋值：
	RGBQUAD* ColorTable;
	int MaxColors = 256;
	//这里可以通过CI.GetMaxColorTableEntries()得到大小(如果你是CI.Load读入图像的话)
	ColorTable = new RGBQUAD[MaxColors];
	CI.GetColorTable(0, MaxColors, ColorTable);//这里是取得指针
	for (int i = 0; i<MaxColors; i++)
	{
		ColorTable[i].rgbBlue = (BYTE)i;
		//BYTE和uchar一回事，但MFC中都用它
		ColorTable[i].rgbGreen = (BYTE)i;
		ColorTable[i].rgbRed = (BYTE)i;
	}
	CI.SetColorTable(0, MaxColors, ColorTable);
	delete[]ColorTable;
	//然后就是数据拷贝了(这里的矩阵表示方法，根据需要(cvMat or Mat)修改)：
	if (chinnels == 1)
	{//灰度图像	
		uchar *pS;
		uchar *psrc = (uchar *)CI.GetBits();
		int nrow = CI.GetPitch();
		for (int i = 0; i<h; i++)
		{
			pS = img.ptr<uchar>(i);
			for (int j = 0; j<w; j++)
			{
				*(psrc + i*nrow + j) = pS[j];
			}
		}
	}
	//(2)如果是3个通道(彩色图像)
	//没有调色板，直接赋值
	if (chinnels == 3)
	{//彩色图像
		uchar *pS;
		BYTE *psrc = (BYTE *)CI.GetBits();//得到CImage数据区地址
		int nrow = CI.GetPitch();
		//这个是一行像素站的存储空间w*3，并且结果是4的倍数(这个不用关注，到底是不是4的倍数有待考证)
		for (int i = 0; i<h; i++)
		{
			pS = img.ptr<BYTE>(i);
			for (int j = 0; j<w; j++)
			{
				for (int k = 0; k<3; k++)
					*(psrc + i*nrow + j * 3 + k) = pS[j * 3 + k];
				//注意到这里的nrow不用乘以3
			}
		}
	}
	return CI;
}
// CImage_ProcessingView 绘制

void CImage_ProcessingView::OnDraw(CDC* pDC)
{
	CImage_ProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (!m_Image.IsNull())
	{
		m_Image.Draw(pDC->m_hDC,0,0); 
	}
	return;
}

void CImage_ProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CImage_ProcessingView 打印


void CImage_ProcessingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImage_ProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImage_ProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImage_ProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CImage_ProcessingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImage_ProcessingView::OnLButtonDown(UINT /*Flag*/, CPoint point)
{
	/*if(Flag)*/
}

void CImage_ProcessingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImage_ProcessingView 诊断

#ifdef _DEBUG
void CImage_ProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImage_ProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImage_ProcessingDoc)));
	return (CImage_ProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CImage_ProcessingView 消息处理程序


void CImage_ProcessingView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(TRUE);    //为打开文件创建一个变量
	if(IDOK == dlg.DoModal())    //调用函数打开一个对话框，并判断是否打开成功
	{
		if (!m_Image.IsNull())
		{
			m_Image.Destroy();//判断是否已经有图片，有的话进行清除
		}
		m_Image.Load(dlg.GetPathName());//获得图片的地址，并且加载图片
		m_Image.PathName = dlg.GetPathName ();
		//获得图片的大小，并按其大小设置滚动条的初始窗口大小等参数
		CSize sizeTotal;
		int w=m_Image.GetWidth();
		int h=m_Image.GetHeight();
		sizeTotal.cx =w;
		sizeTotal.cy =h;
		SetScrollSizes(MM_TEXT, sizeTotal);

		//Window_Image_w=m_Image.GetWidth();//获得图片的初始大小，为放大缩小功能做准备
		//Window_Image_h=m_Image.GetHeight();//


		Invalidate(1); //强制调用ONDRAW函数
	}
}


void CImage_ProcessingView::OnDrawline()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) return;
	
	int w=m_Image.GetWidth();
	int h=m_Image.GetHeight();

	for (int k=0;k<w;k++)
	{
		m_Image.m_pBits[0][10][k]=0;//B
		m_Image.m_pBits[1][10][k]=0;//G
		m_Image.m_pBits[2][10][k]=0;//R


	}

	Invalidate(1); //强制调用ONDRAW函数

}


void CImage_ProcessingView::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) {
		MessageBox(_T("你还没有打开一个要保存的图像文件！"));
		return;
	}
	CString strFilter;
	//strFilter ="位图文件|*.bmp|JPEG 图像文件|*.jpg|GIF 图像文件|*.gif|PNG 图像文件|*.png||";   //
	strFilter ="所有文件|*.*||";   //

	CFileDialog dlg(FALSE,NULL,NULL,NULL,strFilter);
	//CFileDialog dlg(FALSE,NULL,NULL,NULL);

	if ( IDOK != dlg.DoModal()) 
		return;
	// 如果用户没有指定文件扩展名，则为其添加一个
	CString strFileName;
	//CString strExtension;
	strFileName = dlg.m_ofn.lpstrFile;   //  获得文件名
	//if(dlg.m_ofn.nFileExtension == 0)     //获得文件扩展名
	//{
	//	switch (dlg.m_ofn.nFilterIndex)
	//	{
	//	case 1:
	//		strExtension = "bmp"; break;
	//	case 2:
	//		strExtension = "jpg"; break;
	//	case 3:
	//		strExtension = "gif"; break;
	//	case 4:
	//		strExtension = "png"; break;
	//	default:
	//		break;
	//	}
	//	strFileName = strFileName + _T(".") + strExtension;
	//	//strFileName="C:\\Users\\Lenovo\\Desktop\\QSanguosha-Qingming\\abc.bmp";
	//}
	// 图像保存
	HRESULT hResult = m_Image.Save(strFileName);
	if (FAILED(hResult))
	{
		MessageBox(_T("保存图像文件失败！"));
	}

}



//void CImage_ProcessingView::OnViewToolbar()
//{
//	// TODO: 在此添加命令处理程序代码
//}


void CImage_ProcessingView::Onimgdown2()
{
	
	if(m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"));
		 return;
	}
	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度
	// --------------------------------------------------------------------------------------------------
	//		描述：初始化动态数组picture
	//-------------------------------------------------------------------------------------------------
	int*** picture;
	picture = (int***)new   int**[3]();
	for (int i = 0; i<3; i++)
	{
		picture[i] = (int**)new  int*[h]();
	}
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<h; j++)
		{
			picture[i][j] = new int[w]();
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：用picture对原始数据下采样,
	//-------------------------------------------------------------------------------------------------
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h / 2; ++j)
		{
			for (int k = 0; k < w / 2; ++k)
			{
				picture[i][j][k] = m_Image.m_pBits[i][2 * j][2 * k];
			}
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：背景色设置为白色255
	//-------------------------------------------------------------------------------------------------
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < w; ++k)
			{
				m_Image.m_pBits[i][j][k] = 255;
			}
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：picture加载到m_pBits，用于显示
	//-------------------------------------------------------------------------------------------------
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h / 2; ++j)
		{
			for (int k = 0; k < w / 2; ++k)
			{
				m_Image.m_pBits[i][j][k] = picture[i][j][k];
			}
		}
	}
Invalidate(); //强制调用ONDRAW函数，ONDRAW会绘制图像
// --------------------------------------------------------------------------------------------------
//		描述：释放动态数组picture
//-------------------------------------------------------------------------------------------------
for (int i = 0; i<2; i++)
{
	for (int j = 0; j<h; j++)
	{
		delete[] picture[i][j];
		picture[i][j] = NULL;
	}
}
for (int i = 0; i<2; i++)
{
	delete[] picture[i];
	picture[i] = NULL;
}
delete[]  picture;
picture = NULL;
}


void CImage_ProcessingView::Onimgdown4()
{
	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"));
		return;
	}
	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度

	// --------------------------------------------------------------------------------------------------
	//		描述：初始化动态数组picture
	//-------------------------------------------------------------------------------------------------
	int*** picture;
	picture = (int***)new   int**[3]();
	for (int i = 0; i<3; i++)
	{
		picture[i] = (int**)new  int*[h]();
	}
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<h; j++)
		{
			picture[i][j] = new int[w]();
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：用picture对原始数据下采样,
	//-------------------------------------------------------------------------------------------------
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h / 4; ++j)
		{
			for (int k = 0; k < w / 4; ++k)
			{
				picture[i][j][k] = m_Image.m_pBits[i][4 * j][4 * k];
			}
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：背景色设置为白色255
	//-------------------------------------------------------------------------------------------------
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < w; ++k)
			{
				m_Image.m_pBits[i][j][k] = 255;
			}
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：picture加载到m_pBits，用于显示
	//-------------------------------------------------------------------------------------------------
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h / 4; ++j)
		{
			for (int k = 0; k < w / 4; ++k)
			{
				m_Image.m_pBits[i][j][k] = picture[i][j][k];
			}
		}
	}

	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
	// --------------------------------------------------------------------------------------------------
	//		描述：释放动态数组picture
	//-------------------------------------------------------------------------------------------------
	for (int i = 0; i<2; i++)
	{
		for (int j = 0; j<h; j++)
		{
			delete[] picture[i][j];
			picture[i][j] = NULL;
		}
	}
	for (int i = 0; i<2; i++)
	{
		delete[] picture[i];
		picture[i] = NULL;
	}
	delete[]  picture;
	picture = NULL;
}


void CImage_ProcessingView::Onimgdiffrence()
{
	// --------------------------------------------------------------------------------------------------
	//		描述：从控件获取CImage
	//-------------------------------------------------------------------------------------------------
	MyImage_ srcImage_back;
		Image_Diffrence dlg;
		dlg.DoModal();
		if (!m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
		{
			m_Image.Destroy();
		}
		if (!srcImage_back.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
		{
			srcImage_back.Destroy();
		}
		CString PathName = dlg.GetPathName();
		CString PathName_back = dlg.GetPathName_back();
		if (PathName.IsEmpty() || PathName_back.IsEmpty())
		{
			MessageBox(L"图片选择不正确，请重新选择！", L"警告");
			return;
		}
		m_Image.Load(PathName);
		srcImage_back.Load(PathName_back);
		int w = m_Image.GetWidth();
		int h = m_Image.GetHeight();
		int w_back = srcImage_back.GetWidth();
		int h_back = srcImage_back.GetHeight();
		 if (w != w_back || h != h_back)
		{
			MessageBox(L"两幅图像大小不一致，请重新选择！",L"警告");
			return;
		}
		
	
	// --------------------------------------------------------------------------------------------------
	//		描述：得到各种参数
	//-------------------------------------------------------------------------------------------------

	// --------------------------------------------------------------------------------------------------
	//		描述：第一幅图像与第二幅图像对应像素点值相减(RGB)
	//-------------------------------------------------------------------------------------------------
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < w; ++k)
			{
				m_Image.m_pBits[i][j][k] = abs(m_Image.m_pBits[i][j][k] - srcImage_back.m_pBits[i][j][k]);
			}
		}
	} 
	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}


void CImage_ProcessingView::Onchangetogray()
{
	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"));
		return;
	}
	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度
	// --------------------------------------------------------------------------------------------------
	//		描述：将彩色图像转换为灰度图像处理
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = (299 * m_Image.m_pBits[2][j][k] + 587 * m_Image.m_pBits[1][j][k] + 114 * m_Image.m_pBits[0][j][k] + 500) / 1000;
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：灰度一致
	//-------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[1][j][k] = m_Image.m_pBits[0][j][k];
			m_Image.m_pBits[2][j][k] = m_Image.m_pBits[0][j][k];
		}
	}
	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}


void CImage_ProcessingView::Ondenoisebyadd()
{

	CFileDialog fDlgGetTxt(TRUE);//打开文件对话框
	//fDlgGetTxt.m_ofn.lpstrFilter = "(*.txt)\0*.txt\0\0";//只显示txt文件格式：显示内容\0 过滤条件\0 ... 最后以\0结尾
	//fDlgGetTxt.m_ofn.lpstrTitle = "打开txt文件";
	fDlgGetTxt.m_ofn.Flags |= OFN_ALLOWMULTISELECT;//允许选择多个文件
	//fDlgGetTxt.m_ofn.nMaxFile = 20 * 101;//最多可以打开20个文件，每个文件名的字符数<=100
	//设定一个文件名缓存，因为CFileDialog内置的文件名缓存长度只有200，但是很多时候，
	//文件的路径远大于这个数，为了保险起见，所以要自己设定一个文件名缓存
	//char fileNameBuffer[20 * 101] = { 0 };//初始化为零，否则会发生意外错误
	//fDlgGetTxt.m_ofn.lpstrFile = fileNameBuffer;//设定一个文件名缓存

	CStringArray strArrFilePaths;//将文件路径保存到CStringArray对象中
	int size = 0;
	// --------------------------------------------------------------------------------------------------
	//		描述：获取选取所有图像的path保存到strArrFilePaths
	//-------------------------------------------------------------------------------------------------
	if (fDlgGetTxt.DoModal() == IDOK)
	{
		POSITION pos = fDlgGetTxt.GetStartPosition();//获取第一个文件名的位置

		while (pos != NULL) //GetNextPathName()返回当前pos的文件名，并将下一个文件名的位置保存到pos中
		{
			strArrFilePaths.Add(fDlgGetTxt.GetNextPathName(pos));
		}
	}
	size = strArrFilePaths.GetSize();
	if (size == 0)
	{
		return;
	}
	if (!m_Image.IsNull())
	{
		m_Image.Destroy();//判断是否已经有图片，有的话保存并进行清除
	}

	m_Image.Load(strArrFilePaths[0]);
	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度
	int count = 0;
	// --------------------------------------------------------------------------------------------------
	//		描述：定义动态数组存图
	//-------------------------------------------------------------------------------------------------
	int*** picture;
	picture = (int***)new   int**[3]() ;
	for (int i = 0; i<3; i++)
	{
		picture[i] = (int**)new  int*[h]() ;
	}
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<h; j++)
		{
			picture[i][j] = new int[w]() ;
		}
	}

	// --------------------------------------------------------------------------------------------------
	//		描述：所有图像叠加去除（减弱）噪声
	//-------------------------------------------------------------------------------------------------
	while (count<size)
	{
		m_Image.Destroy();
		m_Image.Load(strArrFilePaths[count]);
		for (int j = 0; j<h; ++j)
		{
			for (int k = 0; k<w; ++k)
			{
				picture[0][j][k] = picture[0][j][k] + m_Image.m_pBits[0][j][k];//B
				picture[1][j][k] = picture[1][j][k] + m_Image.m_pBits[1][j][k];//G
				picture[2][j][k] = picture[2][j][k] + m_Image.m_pBits[2][j][k];//R
			}
		}
		count++;
	}
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			picture[0][j][k] = picture[0][j][k] / size;
			picture[1][j][k] = picture[1][j][k] / size;
			picture[2][j][k] = picture[2][j][k] / size;
		}
	}
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{ 
			m_Image.m_pBits[0][j][k] = picture[0][j][k];
			m_Image.m_pBits[1][j][k] = picture[1][j][k];
			m_Image.m_pBits[2][j][k] = picture[2][j][k];
		}
	}
	Invalidate(1); //强制调用ONDRAW函数
	// --------------------------------------------------------------------------------------------------
	//		描述：释放动态数组
	//-------------------------------------------------------------------------------------------------
	for (int i = 0; i<2; i++)
	{
		for (int j = 0; j<h; j++)
		{
			delete[] picture[i][j];
			picture[i][j] = NULL;
		}
	}
	for (int i = 0; i<2; i++)
	{
		delete[] picture[i];
		picture[i] = NULL;
	}
	delete[]  picture;
	picture = NULL;
}


void CImage_ProcessingView::OnFileOpenAll()
{
	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"));
		return;
	}
	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度
	// --------------------------------------------------------------------------------------------------
	//		描述：将彩色图像转换为灰度图像处理
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = (299 * m_Image.m_pBits[2][j][k] + 587 * m_Image.m_pBits[1][j][k] + 114 * m_Image.m_pBits[0][j][k] + 500) / 1000;
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：定义直方图数组，统计直方图
	//---------------------------------------------------------------------------------------------------
	int size;
	WINDOW_Median dlg;
	size=dlg.GetWindowSize();
	dlg.DoModal();
}


void CImage_ProcessingView::Onmeansfilter()
{
	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"));
		return;
	}
	int size=3;//定义窗口大小
	
	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度
	// --------------------------------------------------------------------------------------------------
	//		描述：将彩色图像转换为灰度图像处理
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = (299 * m_Image.m_pBits[2][j][k] + 587 * m_Image.m_pBits[1][j][k] + 114 * m_Image.m_pBits[0][j][k] + 500) / 1000;
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：定义转存数组picture，转存图像
	//-------------------------------------------------------------------------------------------------
	int** picture;
	picture = new int*[h];
	for (int j = 0; j < h; ++j)
	{
		picture[j] = new int[w];
	}

	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			picture[j][k] = m_Image.m_pBits[0][j][k];
		}
	}

	// --------------------------------------------------------------------------------------------------
	//		描述：均值操作
	//-------------------------------------------------------------------------------------------------
	for (int j = 1; j < h-1; ++j)
	{
		for (int k = 1; k < w-1; ++k)
		{
			m_Image.m_pBits[0][j][k] =
				(   picture[j - 1][k - 1] + picture[j - 1][k] + picture[j - 1][k + 1] +
			    	picture[j][k-1] + picture[j][k] + picture[j][k+1] +
			    	picture[j + 1][k - 1] + picture[j + 1][k] + picture[j + 1][k+1] ) / 9;
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：释放转存数组picture
	//-------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; j++)
	{
		delete[] picture[j];
	}
	delete[] picture;
	// --------------------------------------------------------------------------------------------------
	//		描述：灰度一致
	//-------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[1][j][k] = m_Image.m_pBits[0][j][k];
			m_Image.m_pBits[2][j][k] = m_Image.m_pBits[0][j][k];
		}
	}
	Invalidate(1); //强制调用ONDRAW函数

}


void CImage_ProcessingView::Onmedianfilter()
{
	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"),L"警告");
		return;
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：弹出输入窗口，重复直到输入合法
	//---------------------------------------------------------------------------------------------------
	int size=3;//定义窗口大小
	int Size_Min = 3;
	int Size_Max = 21;//设定最大窗口
	int Size_Max_Half = 10;
	int halfsize;
	int flag_sort = 1;//选择哪种算法，0为冒泡排序，1为直方图排序
	long time = 0; //算法运行时间
	bool FlagAuto=false;
	WINDOW_Median dlg;
	if (dlg.DoModal() == IDOK)
	{ 
		
		if (!dlg.Flag_auto)
		{
			size = dlg.GetWindowSize();
			halfsize = size / 2;
			if (size < 3)
			{
				MessageBox(_T("输入窗口大小有误，不能小于3，请重新输入！"), L"警告");
				return;
			}
			else if (size % 2 == 0)
			{
				MessageBox(_T("输入窗口大小有误，不能为偶数，请重新输入！"), L"警告");
				return;
			}
			else if (size > 21)
			{
				MessageBox(_T("输入窗口大小有误，窗口太大，请重新输入！"), L"警告");
				return;
			}
		}
		else FlagAuto = true;
		flag_sort = dlg.GetSortType();
	}
	if (!dlg.flag_success)
		return;

	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度

	// --------------------------------------------------------------------------------------------------
	//		描述：将彩色图像转换为灰度图像处理
	//---------------------------------------------------------------------------------------------------
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < w; ++k)
			{
				m_Image.m_pBits[0][j][k] = (299 * m_Image.m_pBits[2][j][k] + 587 * m_Image.m_pBits[1][j][k] + 114 * m_Image.m_pBits[0][j][k] + 500) / 1000;
			}
		}

	// --------------------------------------------------------------------------------------------------
	//		描述：定义转存数组picture,转存图像
	//-------------------------------------------------------------------------------------------------
	long time_start = GetTickCount();//获得开始时间
	unsigned char** picture;
	if (FlagAuto)//自适应
	{
		size = Size_Max;
		halfsize = Size_Max_Half;
	}
		picture = new unsigned char*[h + size - 1];
		for (int j = 0; j < (h + size - 1); ++j)
		{
			picture[j] = new unsigned char[w + size - 1];
		}
		for (int j = halfsize; j < h + size - 1 - halfsize; ++j)
		{
			for (int k = halfsize; k < w + size - 1 - halfsize; ++k)
			{
				picture[j][k] = m_Image.m_pBits[0][j - halfsize][k - halfsize];
			}
		}


	// --------------------------------------------------------------------------------------------------
	//		描述：对边界进行扩展（外圈镜像扩展），赋值
	//---------------------------------------------------------------------------------------------------

	for (int j = 0; j < h+size-1; ++j)
	{
		for (int k = 0; k < w+size-1; ++k)
		{
			if (j >= 0 && j < halfsize&&k >= 0 && k < halfsize)//左上角部分
			{
				picture[j][k] = picture[2 * halfsize - j - 1][2 * halfsize - k - 1];
			}
			else if (j >= 0 && j < halfsize && k >= w + halfsize&&k <w + size - 1)//右上角部分
			{
				picture[j][k] = picture[2 * halfsize - j - 1][2 * (halfsize + w) - k - 1];
			}
			else if (j >= h + halfsize && j < h + size - 1 && k >= 0 && k < halfsize)//左下角部分
			{
				picture[j][k] = picture[2 * (halfsize + h) - j - 1][2 * halfsize - k - 1];
			}
			else if (j >= h + halfsize && j < h + size - 1 && k >= w + halfsize&&k < w + size - 1)//右下角部分
			{
				picture[j][k] = picture[2 * (halfsize + h) - j - 1][2 * (halfsize + w) - k - 1];
			}
			else if (j>=halfsize&&j<h+halfsize && k>=0&&k<halfsize)//左边部分
			{
				picture[j][k] = picture[j][2 * halfsize - k - 1];
			}
			else if (j >= halfsize&&j<h + halfsize &&k >= w + halfsize&&k <w + size - 1)//右边部分
			{
				picture[j][k] = picture[j][2 * (w + halfsize) - k - 1];
			}
			else if (j >= 0 && j < halfsize&& k>=halfsize&&k<w+halfsize)//上边部分
			{
				picture[j][k] = picture[2 * halfsize - j - 1][k];
			}
			else if (j >= h + halfsize && j < h + size - 1 && k >= halfsize&&k<w + halfsize)//下边部分
			{
				picture[j][k] = picture[2 * (h + halfsize) - j - 1][k];
			}
			else break;
		}
	}

	
	// --------------------------------------------------------------------------------------------------
	//		描述：中值操作(冒泡排序)
	//-------------------------------------------------------------------------------------------------
	if ((flag_sort == 0)&& FlagAuto)
	{
		int size_auto = Size_Min;
		int Local_min = 255;
		int Local_max = 0;
		int Local_med = 0;
		bool FlagAddWnd=true;
		int * list = new int[size*size];
		for (int j = halfsize; j < h + size - 1 - halfsize; ++j)
		{
			for (int k = halfsize; k < w + size - 1 - halfsize; ++k)
			{ 
				size_auto = Size_Min;
				while (FlagAddWnd)
				{ 
					if (size_auto > 21)
						break;
					int i = 0;
					for (int m = j - size_auto / 2; m <= j + size_auto / 2; m++)
					{
						for (int n = k - size_auto / 2; n <= k + size_auto / 2; n++)
						{
							list[i] = picture[m][n];
							i++;
						}
					}
					int T = 0;
					for (int m = 0; m < (size_auto*size_auto) / 2 + 1; m++)
					{
						for (int n = 0; n < size_auto*size_auto - 1 - m; n++)
						{
							if (list[n]>list[n + 1])
							{
								T = list[n];
								list[n] = list[n + 1];
								list[n + 1] = T;
							}
						}
					}
					Local_min = list[0];
					Local_max = list[size_auto*size_auto - 1];
					Local_med = list[(size_auto*size_auto) / 2];
					if ((Local_med > Local_min) && (Local_med < Local_max))
						break;
					else size_auto += 2;
				}
				if ((m_Image.m_pBits[0][j - halfsize][k - halfsize]>Local_min) && (m_Image.m_pBits[0][j - halfsize][k - halfsize] < Local_max))
					continue;
				else m_Image.m_pBits[0][j - halfsize][k - halfsize] = Local_med;
			}
		}
		delete[] list;
	}

	if ((flag_sort == 0) && (!FlagAuto))
	{
		int * list = new int[size*size];
		for (int j = halfsize; j < h + size - 1 - halfsize; ++j)
		{
			for (int k = halfsize; k < w + size - 1 - halfsize; ++k)
			{
				int i = 0;
				for (int m = j - halfsize; m <= j + halfsize; m++)
				{
					for (int n = k - halfsize; n <= k + halfsize; n++)
					{
						list[i] = picture[m][n];
						i++;
					}
				}
				int T = 0;
				for (int m = 0; m < (size*size) / 2 + 1; m++)
				{
					for (int n = 0; n < size*size - 1 - m; n++)
					{
						if (list[n]>list[n + 1])
						{
							T = list[n];
							list[n] = list[n + 1];
							list[n + 1] = T;
						}
					}
				}
				m_Image.m_pBits[0][j - halfsize][k - halfsize] = list[(size*size) / 2];
			}
		}
		delete[] list;
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：中值操作(直方图排序)
	//-------------------------------------------------------------------------------------------------
	if ((flag_sort == 1) && FlagAuto)//自适应，（未完成）
	{
		int His[256] = { 0 };
		int Cumpiexl = 0;
		//对图像做Z字形遍历
		for (int j = halfsize; j < h + size - 1 - halfsize; ++j)
		{
			if ((j - halfsize) % 2 == 0)//第奇数个遍历行，从左到右遍历
			{
				for (int k = halfsize; k < w + size - 1 - halfsize; ++k)
				{
					if (k == halfsize)
					{
						if (j == halfsize)//统计第一行，横向第一个块的直方图
						{
							for (int m = j - halfsize; m <= j + halfsize; m++)
							{
								for (int n = k - halfsize; n <= k + halfsize; n++)
								{
									His[picture[m][n]]++;
								}
							}
						}
						else //非第一行，横向第一个块
						{
							//添加最下行size个点进直方图
							for (int i = k - halfsize; i <= k + halfsize; i++)
								His[picture[j + halfsize][i]]++;
						}

						//从直方图中找到中值
						Cumpiexl = 0;
						for (int i = 0; i < 256; i++)
						{
							Cumpiexl += His[i];
							if (Cumpiexl >(size*size) / 2)
							{
								m_Image.m_pBits[0][j - halfsize][k - halfsize] = i;
								break;
							}
						}
						//从直方图去除最左列size个点
						for (int i = j - halfsize; i <= j + halfsize; i++)
							His[picture[i][k - halfsize]]--;
					}
					else//非第一行
					{
						//添加最右列size个点进直方图
						for (int i = j - halfsize; i <= j + halfsize; i++)
							His[picture[i][k + halfsize]]++;
						//从直方图中找到中值
						Cumpiexl = 0;
						for (int i = 0; i < 256; i++)
						{
							Cumpiexl += His[i];
							if (Cumpiexl >(size*size) / 2)
							{
								m_Image.m_pBits[0][j - halfsize][k - halfsize] = i;
								break;
							}
						}
						//从直方图去除前size个点
						if (k < w + size - 2 - halfsize)//不是这一行最后一个块
						{
							//去除最左列size个点
							for (int i = j - halfsize; i <= j + halfsize; i++)
								His[picture[i][k - halfsize]]--;
						}
						else//这一行最后一个块
						{
							//去除最上行size个点
							for (int i = k - halfsize; i <= k + halfsize; i++)
								His[picture[j - halfsize][i]]--;
						}
					}
				}
			}
			else //第偶数个遍历行，从右到左遍历
			{
				for (int k = w + size - 2 - halfsize; k >= halfsize; --k)
				{
					if (k == w + size - 2 - halfsize)//本行第一个块
					{
						//添加最下行size个点进直方图
						for (int i = k - halfsize; i <= k + halfsize; i++)
							His[picture[j + halfsize][i]]++;
						//从直方图中找到中值
						Cumpiexl = 0;
						for (int i = 0; i < 256; i++)
						{
							Cumpiexl += His[i];
							if (Cumpiexl >(size*size) / 2)
							{
								m_Image.m_pBits[0][j - halfsize][k - halfsize] = i;
								break;
							}
						}
						//去除最右列size个点
						for (int i = j - halfsize; i <= j + halfsize; i++)
							His[picture[i][k + halfsize]]--;
					}
					else//本行非第一个块
					{
						//添加最左列size个点进直方图
						for (int i = j - halfsize; i <= j + halfsize; i++)
							His[picture[i][k - halfsize]]++;
						//从直方图中找到中值
						Cumpiexl = 0;
						for (int i = 0; i < 256; i++)
						{
							Cumpiexl += His[i];
							if (Cumpiexl >(size*size) / 2)
							{
								m_Image.m_pBits[0][j - halfsize][k - halfsize] = i;
								break;
							}
						}
						if (k != halfsize)//本行非最后一个块
						{
							//去除最右列size个点
							for (int i = j - halfsize; i <= j + halfsize; i++)
								His[picture[i][k + halfsize]]--;
						}
						else//本行最后一个块
						{
							//去除最上行size个点
							for (int i = k - halfsize; i <= k + halfsize; i++)
								His[picture[j - halfsize][i]]--;
						}
					}
				}
			}
		}
	}
	if ((flag_sort == 1)&&(!FlagAuto))
	{
		int His[256] = { 0 };
		int Cumpiexl = 0;
		//对图像做Z字形遍历
		for (int j = halfsize; j < h + size - 1 - halfsize; ++j)
		{
			if ((j - halfsize) % 2 == 0)//第奇数个遍历行，从左到右遍历
			{
				for (int k = halfsize; k < w + size - 1 - halfsize; ++k)
				{
					if (k == halfsize)
					{
						if (j == halfsize)//统计第一行，横向第一个块的直方图
						{
							for (int m = j - halfsize; m <= j + halfsize; m++)
							{
								for (int n = k - halfsize; n <= k + halfsize; n++)
								{
									His[picture[m][n]]++;
								}
							}
						}
						else //非第一行，横向第一个块
						{
							//添加最下行size个点进直方图
							for (int i = k - halfsize; i <= k + halfsize; i++)
								His[picture[j + halfsize][i]]++;
						}

						//从直方图中找到中值
						Cumpiexl = 0;
						for (int i = 0; i < 256; i++)
						{
							Cumpiexl += His[i];
							if (Cumpiexl >(size*size) / 2)
							{
								m_Image.m_pBits[0][j - halfsize][k - halfsize] = i;
								break;
							}
						}
						//从直方图去除最左列size个点
						for (int i = j - halfsize; i <= j + halfsize; i++)
							His[picture[i][k - halfsize]]--;
					}
					else//非第一行
					{
						//添加最右列size个点进直方图
						for (int i = j - halfsize; i <= j + halfsize; i++)
							His[picture[i][k + halfsize]]++;
						//从直方图中找到中值
						Cumpiexl = 0;
						for (int i = 0; i < 256; i++)
						{
							Cumpiexl += His[i];
							if (Cumpiexl >(size*size) / 2)
							{
								m_Image.m_pBits[0][j - halfsize][k - halfsize] = i;
								break;
							}
						}
						//从直方图去除前size个点
						if (k < w + size - 2 - halfsize)//不是这一行最后一个块
						{
							//去除最左列size个点
							for (int i = j - halfsize; i <= j + halfsize; i++)
								His[picture[i][k - halfsize]]--;
						}
						else//这一行最后一个块
						{
							//去除最上行size个点
							for (int i = k - halfsize; i <= k + halfsize; i++)
								His[picture[j - halfsize][i]]--;
						}
					}
				}
			}
			else //第偶数个遍历行，从右到左遍历
			{
				for (int k = w + size - 2 - halfsize; k >= halfsize; --k)
				{
					if (k == w + size - 2 - halfsize)//本行第一个块
					{
						//添加最下行size个点进直方图
						for (int i = k - halfsize; i <= k + halfsize; i++)
							His[picture[j + halfsize][i]]++;
						//从直方图中找到中值
						Cumpiexl = 0;
						for (int i = 0; i < 256; i++)
						{
							Cumpiexl += His[i];
							if (Cumpiexl >(size*size) / 2)
							{
								m_Image.m_pBits[0][j - halfsize][k - halfsize] = i;
								break;
							}
						}
						//去除最右列size个点
						for (int i = j - halfsize; i <= j + halfsize; i++)
							His[picture[i][k + halfsize]]--;
					}
					else//本行非第一个块
					{
						//添加最左列size个点进直方图
						for (int i = j - halfsize; i <= j + halfsize; i++)
							His[picture[i][k - halfsize]]++;
						//从直方图中找到中值
						Cumpiexl = 0;
						for (int i = 0; i < 256; i++)
						{
							Cumpiexl += His[i];
							if (Cumpiexl >(size*size) / 2)
							{
								m_Image.m_pBits[0][j - halfsize][k - halfsize] = i;
								break;
							}
						}
						if (k != halfsize)//本行非最后一个块
						{
							//去除最右列size个点
							for (int i = j - halfsize; i <= j + halfsize; i++)
								His[picture[i][k + halfsize]]--;
						}
						else//本行最后一个块
						{
							//去除最上行size个点
							for (int i = k - halfsize; i <= k + halfsize; i++)
								His[picture[j - halfsize][i]]--;
						}
					}
				}
			}
		}
	}
	
	// --------------------------------------------------------------------------------------------------
	//		描述：释放转存数组picture
	//-------------------------------------------------------------------------------------------------
	for (int j = 0; j < h + size - 1; j++)
	{
		delete[] picture[j];
	}
	delete[] picture;
	// --------------------------------------------------------------------------------------------------
	//		描述：灰度一致
	//-------------------------------------------------------------------------------------------------
	long time_end = GetTickCount();//获得结束时间
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[1][j][k] = m_Image.m_pBits[0][j][k];
			m_Image.m_pBits[2][j][k] = m_Image.m_pBits[0][j][k];
		}
	}
	Invalidate(1); //强制调用ONDRAW函数
	// --------------------------------------------------------------------------------------------------
	//		描述：计算运行时间
	//-------------------------------------------------------------------------------------------------
	CString str;
	time = time_end - time_start;
	str.Format(L"运行时间time = %f秒（s）", time/1000.0);
	MessageBox(str, L"运行时间");
}


void CImage_ProcessingView::OnHisView()
{
	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"));
		return;
	}
	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度
	// --------------------------------------------------------------------------------------------------
	//		描述：定义直方图数组，统计直方图
	//---------------------------------------------------------------------------------------------------
	int His_Red[256] = { 0 };
	int His_Green[256] = { 0 };
	int His_Blue[256] = { 0 };

	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			His_Red[m_Image.m_pBits[2][j][k]]++;
			His_Green[m_Image.m_pBits[1][j][k]]++;
			His_Blue[m_Image.m_pBits[0][j][k]]++;

		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：创建窗口显示直方图
	//---------------------------------------------------------------------------------------------------
	DrawHis dlgHis;
	dlgHis.SetDatas(His_Red, His_Green, His_Blue); 
	dlgHis.SetTitle(L"红色分量", L"绿色分量", L"蓝色分量");
	dlgHis.DoModal();
	//Invalidate();
	// TODO:  在此添加命令处理程序代码
}


void CImage_ProcessingView::OnHisequalization()
{
	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"));
		return;
	}
	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度
	// --------------------------------------------------------------------------------------------------
	//		描述：将彩色图像转换为灰度图像处理
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = (299 * m_Image.m_pBits[2][j][k] + 587 * m_Image.m_pBits[1][j][k] + 114 * m_Image.m_pBits[0][j][k] + 500) / 1000;
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：定义直方图数组，统计直方图
	//--------------------------------------------------------------------------------------------------
	int NumPixel[256] = {0};
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			NumPixel[m_Image.m_pBits[0][j][k]]++;
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：计算亮度分布密度
	//---------------------------------------------------------------------------------------------------
	float ProbPixel[256] = {0};
	for (int i = 0; i < 256; i++)
	{
		ProbPixel[i] = (double)NumPixel[i] / (w*h*1.0);
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：计算累计直方图分布
	//---------------------------------------------------------------------------------------------------
	double CumuPixel[256] = {0};
	CumuPixel[0] = ProbPixel[0];
	for (int i = 1; i < 256; i++)
	{
			CumuPixel[i] = CumuPixel[i - 1] + ProbPixel[i];
	}

	// --------------------------------------------------------------------------------------------------
	//		描述：累计分布取整
	//---------------------------------------------------------------------------------------------------
	int NumPixel_equa[256] = {0};
	for (int i = 0; i < 256; i++)
	{
		NumPixel_equa[i] = (int)(CumuPixel[i] * 255.0 + 0.5);
	
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：对亮度值进行映射（均衡化)
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = NumPixel_equa[m_Image.m_pBits[0][j][k]];
		}
	}
	//---------------------------------------------------------------------------------------------------
	//		描述：灰度显示一致
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[1][j][k] = m_Image.m_pBits[0][j][k];
			m_Image.m_pBits[2][j][k] = m_Image.m_pBits[0][j][k];
		}
	}
	Invalidate(1); //强制调用ONDRAW函数
}


void CImage_ProcessingView::OnHisnormalized()
{
	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"));
		return;
	}
	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度

	// --------------------------------------------------------------------------------------------------
	//		描述：弹出消息框，要求打开目标参考图像
	//---------------------------------------------------------------------------------------------------
	MessageBox(_T("请打开一幅图像作为目标！"));
	// --------------------------------------------------------------------------------------------------
	//		描述：打开一个目标图像
	//---------------------------------------------------------------------------------------------------
	CFileDialog dlg(TRUE);    //为打开文件创建一个变量
	MyImage_ srcImage;
	//while ()
	if (IDOK == dlg.DoModal())    //调用函数打开一个对话框，并判断是否打开成功
	{
		if (!srcImage.IsNull())
		{
			srcImage.Destroy();//判断是否已经有图片，有的话进行清除
		}
	}
	CString PathName = dlg.GetPathName();
	if (PathName.IsEmpty())
	{
		MessageBox(_T("请打开一幅图像作为目标！"));
		return;
	}
	srcImage.Load(PathName);//获得图片的地址，并且加载图片
		int w_obj = srcImage.GetWidth();//获得图像的宽度
		int h_obj = srcImage.GetHeight();//获得图像的高度
	// --------------------------------------------------------------------------------------------------
	//		描述：RGB转换为灰度图
	//---------------------------------------------------------------------------------------------------
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < w; ++k)
			{
				m_Image.m_pBits[0][j][k] = (299 * m_Image.m_pBits[2][j][k] + 587 * m_Image.m_pBits[1][j][k] + 114 * m_Image.m_pBits[0][j][k]+500) / 1000;
			}
		}
		for (int j = 0; j < h_obj; ++j)
		{
			for (int k = 0; k < w_obj; ++k)
			{
				srcImage.m_pBits[0][j][k] = (299 * srcImage.m_pBits[2][j][k] + 587 * srcImage.m_pBits[1][j][k] + 114 * srcImage.m_pBits[0][j][k] + 500) / 1000;
			}
		}

	// --------------------------------------------------------------------------------------------------
	//		描述：定义直方图数组，统计直方图
	//---------------------------------------------------------------------------------------------------
		int NumPixel[256] = {0};
		int NumPixel_obj[256] = {0};

	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			NumPixel[m_Image.m_pBits[0][j][k]]++;
		}
	}
	for (int j = 0; j < h_obj; ++j)
	{
		for (int k = 0; k < w_obj; ++k)
		{
			NumPixel_obj[srcImage.m_pBits[0][j][k]] ++;
		}
	}
	
	// --------------------------------------------------------------------------------------------------
	//		描述：计算亮度分布密度
	//---------------------------------------------------------------------------------------------------
	double ProbPixel[256] = {0};
	double ProbPixel_obj[256] = { 0 };
	
	for (int i = 0; i < 256; i++)
	{
		ProbPixel_obj[i] = (double)NumPixel_obj[i] / (w_obj*h_obj*1.0);
		ProbPixel[i] = (double)NumPixel[i] / (w*h*1.0);
	}

	// --------------------------------------------------------------------------------------------------
	//		描述：计算累计直方图分布
	//---------------------------------------------------------------------------------------------------
	float CumuPixel[256] = { 0 };
	float CumuPixel_obj[256] = { 0 };

	CumuPixel[0] = ProbPixel[0];
	CumuPixel_obj[0] = ProbPixel_obj[0];
	
	for (int i = 1; i < 256; i++)
	{
		CumuPixel[i] = CumuPixel[i - 1] + ProbPixel[i];
		CumuPixel_obj[i] = CumuPixel_obj[i - 1] + ProbPixel_obj[i];
	
	}

	// --------------------------------------------------------------------------------------------------
	//		描述：累计分布取整
	//---------------------------------------------------------------------------------------------------
	int NumPixel_equa[256] = { 0 };
	int NumPixel_equa_obj[256] = { 0 };

	for (int i = 0; i < 256; i++)
	{
		NumPixel_equa[i] =(int) (CumuPixel[i] * 255.0+0.5 );
		NumPixel_equa_obj[i] = (int)(CumuPixel_obj[i] * 255.0 + 0.5);
	}
	
	//---------------------------------------------------------------------------------------------------
	//		描述：确定映射关系
	//---------------------------------------------------------------------------------------------------
	int value[256][256] = { 0 };
	int index[256] = { 0 };


	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			value[i][j] = abs(NumPixel_equa_obj[j] - NumPixel_equa[i]);
		}
	}
	//找出最小值
	int MIN[256];

	for (int i = 0; i < 256; i++)
	{
		MIN[i] = 256;
	}
	int flag = 0;
	
	for (int i = 0; i < 256; i++)
	{
		for (int j= 0; j< 256; j++)
		{
			if (value[i][j] < MIN[i])
			{
				MIN[i] = value[i][j ];
				flag = j;
			}
		}
		index[i] = flag;
	}

	//---------------------------------------------------------------------------------------------------
	//		描述：规格化
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = index[m_Image.m_pBits[0][j][k]];
		}
	}
	//---------------------------------------------------------------------------------------------------
	//		描述：灰度显示一致
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[1][j][k] =m_Image.m_pBits[0][j][k];
			m_Image.m_pBits[2][j][k] = m_Image.m_pBits[0][j][k];
		}
	}

	Invalidate(1);
	//---------------------------------------------------------------------------------------------------
	//		描述：显示目标图像直方图和规格化后直方图
	//---------------------------------------------------------------------------------------------------
	int His_final[256] = { 0 };

	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			His_final[m_Image.m_pBits[2][j][k]]++;
		}
	}
	DrawHis dlgHis;
	dlgHis.SetDatas(NumPixel, NumPixel_obj, His_final);
	dlgHis.SetTitle(L"原图直方图",L"目标图直方图",L"规格化直方图");
	dlgHis.DoModal();

	 //TODO:  在此添加命令处理程序代码
}


void CImage_ProcessingView::OnGrayLevelDown()
{
	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"));
		return;
	}
	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度
	// --------------------------------------------------------------------------------------------------
	//		描述：将彩色图像转换为灰度图像处理
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = (299 * m_Image.m_pBits[2][j][k] + 587 * m_Image.m_pBits[1][j][k] + 114 * m_Image.m_pBits[0][j][k] + 500) / 1000;
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：获取处理目标灰度级
	//---------------------------------------------------------------------------------------------------
	int Obj_Level =0;
	Dialog_Gray_Level_Down dlg;
	while (1)
	{
		dlg.DoModal();
		Obj_Level = dlg.GetObjLevel();
		if (Obj_Level == 2 || Obj_Level == 4 || Obj_Level == 8 || Obj_Level == 16 || Obj_Level == 32 || Obj_Level == 64 || Obj_Level == 128 )
		{
			break;
		}
		else
		{
			MessageBox(L"目标灰度级错误，应为2的整数幂，请重新输入！", L"警告");
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：Obj_Level > 8的处理
	//---------------------------------------------------------------------------------------------------
	if (Obj_Level > 8)
	{
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < w; ++k)
			{
				m_Image.m_pBits[0][j][k] = m_Image.m_pBits[0][j][k] / (256 / Obj_Level)* (256 / Obj_Level);
			}
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：Obj_Level = 8的处理
	//---------------------------------------------------------------------------------------------------
	if (Obj_Level == 8)
	{
		//定义每个灰度级别的亮度
		int  level0 = 0, level1 = 37, level2 = 74, level3 = 111, level4 = 148, level5 = 184, level6 = 220, level7 = 255;
		//降低灰度级操作
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < w; ++k)
			{

				if (32 <= m_Image.m_pBits[0][j][k])
					if (64 <= m_Image.m_pBits[0][j][k])
						if (96 <= m_Image.m_pBits[0][j][k])
							if (128 <= m_Image.m_pBits[0][j][k])
								if (160 <= m_Image.m_pBits[0][j][k])
									if (192 <= m_Image.m_pBits[0][j][k])
										if (224 <= m_Image.m_pBits[0][j][k])
											m_Image.m_pBits[0][j][k] = level7;
										else m_Image.m_pBits[0][j][k] = level6;
									else m_Image.m_pBits[0][j][k] = level5;
								else m_Image.m_pBits[0][j][k] = level4;
							else m_Image.m_pBits[0][j][k] = level3;
						else m_Image.m_pBits[0][j][k] = level2;
					else m_Image.m_pBits[0][j][k] = level1;
				else m_Image.m_pBits[0][j][k] = level0;
			}
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：Obj_Level = 4的处理
	//---------------------------------------------------------------------------------------------------
	if (Obj_Level == 4)
	{
		//定义每个灰度级别的亮度
		int  level0 = 0, level1 = 85, level2 = 170, level3 = 255;
		//降低灰度级操作
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < w; ++k)
			{

				if (64 <= m_Image.m_pBits[0][j][k])
					if (128 <= m_Image.m_pBits[0][j][k])
						if (192 <= m_Image.m_pBits[0][j][k])
							m_Image.m_pBits[0][j][k] = level3;
						else m_Image.m_pBits[0][j][k] = level2;
					else m_Image.m_pBits[0][j][k] = level1;
				else m_Image.m_pBits[0][j][k] = level0;
			}
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：Obj_Level = 2的处理
	//---------------------------------------------------------------------------------------------------
	if (Obj_Level == 2)
	{
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < w; ++k)
			{
				m_Image.m_pBits[0][j][k] = m_Image.m_pBits[0][j][k] / (256 / Obj_Level)* 255;
			}
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：灰度一致, 显示
	//-------------------------------------------------------------------------------------------------
	for (int j = 0; j<h; ++j)
	{
		for (int k = 0; k<w; ++k)
		{
			m_Image.m_pBits[1][j][k] = m_Image.m_pBits[0][j][k];//G
			m_Image.m_pBits[2][j][k] = m_Image.m_pBits[0][j][k];//R
		}
	}

	Invalidate(1);
}


void CImage_ProcessingView::OnFft2d()
{
	// --------------------------------------------------------------------------------------------------
	//		描述：弹出消息框，要求打开图像
	//---------------------------------------------------------------------------------------------------
	MessageBox(_T("请打开一幅图像！"));
	// --------------------------------------------------------------------------------------------------
	//		描述：打开一幅图像
	//---------------------------------------------------------------------------------------------------
	CString strfilter = _T("所有图像文件(*.bmp; *.jpg; *.tif; *.png)|*.bmp; *.jpg; *.tif;*.png|Windows 位图(*.bmp)|*.bmp|JPEG 文件(*.jpg)|*.jpg|TIFF 文件(*.tif)|*.tif|All Files(*.*)|*.*||");
	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT,
		//"Suport Files(*.BMP;*.JPG;*.TIF;*.txt)|*.BMP;*.JPG;*.TIF;*.txt|BMP Files(*.BMP)|*.BMP|Jpeg Files(*.JPG)|*.JPG|Tif Files(*.TIF)|*.TIF|TXT Files(*.TXT)|*.TXT||" );
		strfilter);
	Mat mat_Image;
	CImage C_Image;
	//while ()
	CString PathName;
	if (FileDlg.DoModal()==IDOK)    //调用函数打开一个对话框，并判断是否打开成功
	{
		 PathName = FileDlg.GetPathName();
	}
	if (PathName.IsEmpty())
	{
		MessageBox(_T("请打开一幅图像作为目标！"), L"警告");
		return;
	}
	if (!m_Image.IsNull())
		m_Image.Destroy();
	m_Image.Load(PathName);
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	// --------------------------------------------------------------------------------------------------
	//		描述：RGB转换为灰度图
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = (299 * m_Image.m_pBits[2][j][k] + 587 * m_Image.m_pBits[1][j][k] + 114 * m_Image.m_pBits[0][j][k] + 500) / 1000;
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：灰度一致
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[1][j][k] = m_Image.m_pBits[0][j][k];
			m_Image.m_pBits[2][j][k] = m_Image.m_pBits[0][j][k];
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：在MFC主窗口显示原图
	//---------------------------------------------------------------------------------------------------
	Invalidate(1);
	// --------------------------------------------------------------------------------------------------
	//		描述：加载进Mat做后续操作
	//---------------------------------------------------------------------------------------------------
	string str_path;
	str_path = CT2A(PathName.GetString());
	mat_Image = imread(str_path,0);
	if (!mat_Image.data)
	{
		MessageBox(L"读入图像有误，请检查文件", L"警告");
		return ;
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：做FFT变换，中心化，对数变换
	//---------------------------------------------------------------------------------------------------
	//将输入图像拓展到最佳的尺寸2^n，边界使用0进行补充
	int m = getOptimalDFTSize(mat_Image.rows);
	int n = getOptimalDFTSize(mat_Image.cols);
	//将添加的像素初始化为0
	Mat padded;
	copyMakeBorder(mat_Image, padded, m - mat_Image.rows, 0, n - mat_Image.cols, 0, BORDER_CONSTANT, Scalar::all(0));

	//为傅里叶变换的结果分配存储空间(分为实部和虚部)
	//将planes数组合并成为一个多通道的数组complexI
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);
	//就地进行离散傅里叶变换
	dft(complexI, complexI);

	//将负数转换为幅值，即为：log(1+sqrt(Re(DFT(I))^2+Im(DFT(I))^2))
	split(complexI, planes);	//将多通道数组分为两个单通道数组
	//planes[0] = Re(DFT(I)),planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);
	Mat MagnitudeImage = planes[0];
	// --------------------------------------------------------------------------------------------------
	//		描述：对数变换
	//---------------------------------------------------------------------------------------------------
	//进行对数尺度放缩
	MagnitudeImage += Scalar::all(1);
	log(MagnitudeImage, MagnitudeImage);	//求自然对数
	// --------------------------------------------------------------------------------------------------
	//		描述：频谱作中心化
	//---------------------------------------------------------------------------------------------------
	//剪切和重新分布幅度图的象限
	//若有奇数行和奇数列，进行频谱剪裁
	MagnitudeImage = MagnitudeImage(Rect(0, 0, MagnitudeImage.cols &-2, MagnitudeImage.rows &-2));
	//这句中的 & -2 是位操作，因为-2的二进制为除最后一位为0外其余均为1，因此与之后的结果为将最低位的1与掉
	//实现将原来的奇数变为偶数

	//重新排列傅里叶图像中的象限，使得原点位原图像中心
	int cx = MagnitudeImage.cols / 2;
	int cy = MagnitudeImage.rows / 2;
	Mat q0(MagnitudeImage, Rect(0, 0, cx, cy));		//ROI区域的左上方
	Mat q1(MagnitudeImage, Rect(cx, 0, cx, cy));	//ROI区域的右上方
	Mat q2(MagnitudeImage, Rect(0, cy, cx, cy));	//ROI区域的左下方
	Mat q3(MagnitudeImage, Rect(cx, cy, cx, cy));	//ROI区域的右下方

	//交换象限，左上与右下交换
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	//交换象限，右上与左下进行交换
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
	//归一化，用0~1之间的浮点数将矩阵变换为可视图的格式
	normalize(MagnitudeImage, MagnitudeImage, 0, 1, CV_MINMAX);
	// --------------------------------------------------------------------------------------------------
	//		描述：显示频谱图
	//---------------------------------------------------------------------------------------------------
	imshow("频谱幅值", MagnitudeImage);
}


void CImage_ProcessingView::OnTraditionalFilter()
{
	// --------------------------------------------------------------------------------------------------
	//		描述：弹出消息框，要求打开图像
	//---------------------------------------------------------------------------------------------------
	MessageBox(_T("请打开一幅图像！"), L"提示");
	// --------------------------------------------------------------------------------------------------
	//		描述：打开一幅图像
	//---------------------------------------------------------------------------------------------------
	CString strfilter = _T("所有图像文件(*.bmp; *.jpg; *.tif; *.png)|*.bmp; *.jpg; *.tif;*.png|Windows 位图(*.bmp)|*.bmp|JPEG 文件(*.jpg)|*.jpg|TIFF 文件(*.tif)|*.tif|All Files(*.*)|*.*||");
	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT,
		//"Suport Files(*.BMP;*.JPG;*.TIF;*.txt)|*.BMP;*.JPG;*.TIF;*.txt|BMP Files(*.BMP)|*.BMP|Jpeg Files(*.JPG)|*.JPG|Tif Files(*.TIF)|*.TIF|TXT Files(*.TXT)|*.TXT||" );
		strfilter);
	Mat mat_srcImage;//输入图像
	Mat outImage;//输出图像
	//while ()
	CString PathName;
	if (FileDlg.DoModal() == IDOK)    //调用函数打开一个对话框，并判断是否打开成功
	{
		PathName = FileDlg.GetPathName();
	}
	if (PathName.IsEmpty())
	{
		MessageBox(_T("打开图像有误，请重新开始！"), L"警告");
		return;
	}
	if (!m_Image.IsNull())
		m_Image.Destroy();
	m_Image.Load(PathName);
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	// --------------------------------------------------------------------------------------------------
	//		描述：RGB转换为灰度图
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = (299 * m_Image.m_pBits[2][j][k] + 587 * m_Image.m_pBits[1][j][k] + 114 * m_Image.m_pBits[0][j][k] + 500) / 1000;
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：灰度一致
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[1][j][k] = m_Image.m_pBits[0][j][k];
			m_Image.m_pBits[2][j][k] = m_Image.m_pBits[0][j][k];
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：在MFC主窗口显示原图
	//---------------------------------------------------------------------------------------------------
	Invalidate(1);
	// --------------------------------------------------------------------------------------------------
	//		描述：加载进Mat做后续操作
	//---------------------------------------------------------------------------------------------------
	string str_path;
	str_path = CT2A(PathName.GetString());
	mat_srcImage = imread(str_path, 0);
	if (!mat_srcImage.data)
	{
		MessageBox(L"读入图像有误，请检查文件",L"警告");
		return;
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：做FFT变换，中心化，对数变换
	//---------------------------------------------------------------------------------------------------
	//将输入图像拓展到最佳的尺寸2^n，边界使用0进行补充
	int m = getOptimalDFTSize(mat_srcImage.rows);
	int n = getOptimalDFTSize(mat_srcImage.cols);
	//将添加的像素初始化为0
	Mat padded;
	copyMakeBorder(mat_srcImage, padded, m - mat_srcImage.rows, 0, n - mat_srcImage.cols, 0, BORDER_CONSTANT, Scalar::all(0));

	//为傅里叶变换的结果分配存储空间(分为实部和虚部)
	//将planes数组合并成为一个多通道的数组complexI
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);
	//就地进行离散傅里叶变换
	dft(complexI, complexI);
	//将复数转换为幅值，即为：log(1+sqrt(Re(DFT(I))^2+Im(DFT(I))^2))
	split(complexI, planes);	//将多通道数组分为两个单通道数组
	//planes[0] = Re(DFT(I)),planes[1] = Im(DFT(I))

	//剪切和重新分布幅度图的象限
	//若有奇数行和奇数列，进行频谱剪裁
	//MagnitudeImage = MagnitudeImage(Rect(0, 0, MagnitudeImage.cols &-2, MagnitudeImage.rows &-2));
	//这句中的 & -2 是位操作，因为-2的二进制为除最后一位为0外其余均为1，因此与之后的结果为将最低位的1与掉
	//实现将原来的奇数变为偶数

	// --------------------------------------------------------------------------------------------------
	//		描述：频谱(实部和虚部)作中心化
	//---------------------------------------------------------------------------------------------------
	//重新排列傅里叶图像中的象限，使得原点位原图像中心
	int cx = planes[0].cols / 2;
	int cy = planes[0].rows / 2;
	Mat q0_re(planes[0], Rect(0, 0, cx, cy));		//ROI区域的左上方
	Mat q1_re(planes[0], Rect(cx, 0, cx, cy));	//ROI区域的右上方
	Mat q2_re(planes[0], Rect(0, cy, cx, cy));	//ROI区域的左下方
	Mat q3_re(planes[0], Rect(cx, cy, cx, cy));	//ROI区域的右下方

	//交换象限，左上与右下交换
	Mat tmp;
	q0_re.copyTo(tmp);
	q3_re.copyTo(q0_re);
	tmp.copyTo(q3_re);
	//交换象限，右上与左下进行交换
	q1_re.copyTo(tmp);
	q2_re.copyTo(q1_re);
	tmp.copyTo(q2_re);

	Mat q0_im(planes[1], Rect(0, 0, cx, cy));		//ROI区域的左上方
	Mat q1_im(planes[1], Rect(cx, 0, cx, cy));	//ROI区域的右上方
	Mat q2_im(planes[1], Rect(0, cy, cx, cy));	//ROI区域的左下方
	Mat q3_im(planes[1], Rect(cx, cy, cx, cy));	//ROI区域的右下方

	//交换象限，左上与右下交换
	q0_im.copyTo(tmp);
	q3_im.copyTo(q0_im);
	tmp.copyTo(q3_im);
	//交换象限，右上与左下进行交换
	q1_im.copyTo(tmp);
	q2_im.copyTo(q1_im);
	tmp.copyTo(q2_im);
	
	// --------------------------------------------------------------------------------------------------
	//		描述：显示原频谱图，尺度放缩
	//---------------------------------------------------------------------------------------------------
	Mat MagnitudeImage;
	magnitude(planes[0], planes[1], MagnitudeImage);
	MagnitudeImage += Scalar::all(1);
	log(MagnitudeImage, MagnitudeImage);	//求自然对数
	//归一化，用0~1之间的浮点数将矩阵变换为可视图的格式
	normalize(MagnitudeImage, MagnitudeImage, 1, 0, CV_MINMAX);
	// --------------------------------------------------------------------------------------------------
	//		描述：频域滤波，选择滤波器
	//---------------------------------------------------------------------------------------------------
	FREQ_TRA_FILTER Fliterdlg;
	if (Fliterdlg.DoModal() == IDOK)
	{

	}
	Mat outplanes_re,outplanes_im; //滤波后频域图
	Mat planesFilter;//滤波矩阵
	string name;//窗口显示的滤波器名字
	planesFilter.create(planes[0].size(), CV_32FC1);
	int Rows = planes[0].rows;
	int Cols = planes[0].cols;
	int FilterSize=Fliterdlg.GetFilterSize();
	int Level = Fliterdlg.GetFilterLevel();
	if (FilterSize<1 || FilterSize>min(Rows, Cols) / 2)
	{
		MessageBox(L"输入滤波器半径有误，请重新开始！",L"警告");
		return;
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：选择理想低通滤波器
	//---------------------------------------------------------------------------------------------------
	if (Fliterdlg.Flag_LOW && Fliterdlg.Flag_IPF)
	{
		float Flag_R = FilterSize;//半径越小，模糊越大；半径越大，模糊越小
		for (int i = 0; i<Rows; i++)
		{
			for (int j = 0; j<Cols; j++)
			{
				double r = sqrt(pow((i - Rows / 2), 2) + pow((j - Cols / 2), 2));//分子,计算pow必须为float型
				if (r <= Flag_R)
				{
					planesFilter.at<float>(i, j) = 1;
				}
				else
				{
					planesFilter.at<float>(i, j) = 0;
				}
			}
		}
		 name = "理想低通滤波器 半径R=" + std::to_string(FilterSize);
		
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：选择布特沃斯低通滤波器
	//---------------------------------------------------------------------------------------------------
	if (Fliterdlg.Flag_LOW && Fliterdlg.Flag_BPF)
	{
		float Flag_R = FilterSize;//半径越小，模糊越大；半径越大，模糊越小
		for (int i = 0; i<Rows; i++)
		{
			for (int j = 0; j<Cols; j++)
			{
				double r = sqrt(pow((i - Rows / 2), 2) + pow((j - Cols / 2), 2));//分子,计算pow必须为float型
				planesFilter.at<float>(i, j) = 1.0 / (1 + pow(r / Flag_R, 2 * Level));
			}
		}
		 name = "布特沃斯低通滤波器 半径R=" + std::to_string(FilterSize) + "阶数L=" + std::to_string(Level);
		
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：选择高斯低通滤波器
	//---------------------------------------------------------------------------------------------------
	if (Fliterdlg.Flag_LOW && Fliterdlg.Flag_GPF)
	{
		float Flag_R = FilterSize;//半径越小，模糊越大；半径越大，模糊越小
		for (int i = 0; i<Rows; i++)
		{
			for (int j = 0; j<Cols; j++)
			{
				double r = sqrt(pow((i - Rows / 2), 2) + pow((j - Cols / 2), 2));//分子,计算pow必须为float型
				planesFilter.at<float>(i, j) = expf(-r / Flag_R);//expf为以e为底求幂（必须为float型）
			}
		}
		 name = "高斯低通滤波器 半径R=" + std::to_string(FilterSize);
		
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：选择理想高通滤波器
	//---------------------------------------------------------------------------------------------------
	if (Fliterdlg.Flag_HIGH && Fliterdlg.Flag_IPF)
	{
		float Flag_R = FilterSize;//半径越小，模糊越大；半径越大，模糊越小
		for (int i = 0; i<Rows; i++)
		{
			for (int j = 0; j<Cols; j++)
			{
				double r = sqrt(pow((i - Rows / 2), 2) + pow((j - Cols / 2), 2));//分子,计算pow必须为float型
				if (r <= Flag_R)
				{
					planesFilter.at<float>(i, j) = 0;
				}
				else
				{
					planesFilter.at<float>(i, j) = 1;
				}
			}
		}
		 name = "理想高通滤波器 半径R=" + std::to_string(FilterSize);
	
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：选择布特沃斯高通滤波器
	//---------------------------------------------------------------------------------------------------
	if (Fliterdlg.Flag_HIGH && Fliterdlg.Flag_BPF)
	{
		float Flag_R = FilterSize;//半径越小，模糊越大；半径越大，模糊越小
		for (int i = 0; i<Rows; i++)
		{
			for (int j = 0; j<Cols; j++)
			{
				double r = sqrt(pow((i - Rows / 2), 2) + pow((j - Cols / 2), 2));//分子,计算pow必须为float型
				planesFilter.at<float>(i, j) = 1.0 / (1 + pow(Flag_R / r, 2 * Level));
			}
		}
		 name = "布特沃斯高通滤波器 半径R=" + std::to_string(FilterSize)+"阶数L="+std::to_string(Level);
		
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：选择高斯高通滤波器
	//---------------------------------------------------------------------------------------------------
	if (Fliterdlg.Flag_HIGH && Fliterdlg.Flag_GPF)
	{
		float Flag_R = FilterSize;//半径越小，模糊越大；半径越大，模糊越小
		for (int i = 0; i<Rows; i++)
		{
			for (int j = 0; j<Cols; j++)
			{
				double r = sqrt(pow((i - Rows / 2), 2) + pow((j - Cols / 2), 2));//分子,计算pow必须为float型
				planesFilter.at<float>(i, j) = 1-expf(-r / Flag_R);//expf为以e为底求幂（必须为float型）
			}
		}
		 name = "高斯高通滤波器 半径R=" + std::to_string(FilterSize);
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：滤波器模板和频谱相乘
	//---------------------------------------------------------------------------------------------------
	multiply(planes[0], planesFilter, outplanes_re);
	multiply(planes[1], planesFilter, outplanes_im);
	Mat outplanes[] = { outplanes_re, outplanes_im };
	Mat complexO;
	merge(outplanes, 2, complexO);
	// --------------------------------------------------------------------------------------------------
	//		描述：傅里叶反变换
	//---------------------------------------------------------------------------------------------------
	idft(complexO, complexO);
	split(complexO, planes);
	magnitude(planes[0], planes[1], planes[0]);
	normalize(planes[0], planes[0], 1, 0, CV_MINMAX);
	outImage = planes[0];
	imshow("原图", mat_srcImage);
	imshow("原图频谱幅值", MagnitudeImage);
	imshow(name, planesFilter);
	imshow("滤波后图像", outImage);
}


void CImage_ProcessingView::OnShowred ()
{
	if (m_Image.IsNull ()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox ( _T ( "请先打开一幅图像！" ), L"提示" );
		return;
	}
	int w = m_Image.GetWidth ();//获得图像的宽度
	int h = m_Image.GetHeight ();//获得图像的高度
	// --------------------------------------------------------------------------------------------------
	//		描述：返回源图
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = m_Image.srcm_pBits[0][j][k];
			m_Image.m_pBits[1][j][k] = m_Image.srcm_pBits[1][j][k];
			m_Image.m_pBits[2][j][k] = m_Image.srcm_pBits[2][j][k];
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：显示红色分量
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = m_Image.m_pBits[2][j][k];
			m_Image.m_pBits[1][j][k] = m_Image.m_pBits[2][j][k];
			m_Image.m_pBits[2][j][k] = m_Image.m_pBits[2][j][k];
		}
	}
	Invalidate ( 1 );
}


void CImage_ProcessingView::OnShowgreen ()
{
	if (m_Image.IsNull ()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox ( _T ( "请先打开一幅图像！" ), L"提示" );
		return;
	}
	int w = m_Image.GetWidth ();//获得图像的宽度
	int h = m_Image.GetHeight ();//获得图像的高度
 // --------------------------------------------------------------------------------------------------
//		描述：返回源图
 //---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = m_Image.srcm_pBits[0][j][k];
			m_Image.m_pBits[1][j][k] = m_Image.srcm_pBits[1][j][k];
			m_Image.m_pBits[2][j][k] = m_Image.srcm_pBits[2][j][k];
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：显示绿色分量
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = m_Image.m_pBits[1][j][k];
			m_Image.m_pBits[1][j][k] = m_Image.m_pBits[1][j][k];
			m_Image.m_pBits[2][j][k] = m_Image.m_pBits[1][j][k];
		}
	}
	Invalidate ( 1 );
}


void CImage_ProcessingView::OnShowblue ()
{
	if (m_Image.IsNull ()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox ( _T ( "请先打开一幅图像！" ), L"提示" );
		return;
	}
	int w = m_Image.GetWidth ();//获得图像的宽度
	int h = m_Image.GetHeight ();//获得图像的高度
 // --------------------------------------------------------------------------------------------------
 //		描述：返回源图
//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = m_Image.srcm_pBits[0][j][k];
			m_Image.m_pBits[1][j][k] = m_Image.srcm_pBits[1][j][k];
			m_Image.m_pBits[2][j][k] = m_Image.srcm_pBits[2][j][k];
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：显示蓝色分量
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = m_Image.m_pBits[0][j][k];
			m_Image.m_pBits[1][j][k] = m_Image.m_pBits[0][j][k];
			m_Image.m_pBits[2][j][k] = m_Image.m_pBits[0][j][k];
		}
	}
	Invalidate ( 1 );
}


void CImage_ProcessingView::OnRgbToHsi ()
{
	if (m_Image.IsNull ()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox ( _T ( "请先打开一幅图像！" ), L"提示" );
		return;
	}
	int w = m_Image.GetWidth ();//获得图像的宽度
	int h = m_Image.GetHeight ();//获得图像的高度
	Mat srcImage;
	string str_path;

	str_path=CT2A(m_Image.PathName.GetBuffer());
	srcImage = imread ( str_path,1 );

	Mat H_Array ( h, w, CV_8UC1,Scalar::all(0) );
	Mat S_Array ( h, w, CV_8UC1, Scalar::all ( 0 ) );
	Mat I_Array ( h, w, CV_8UC1, Scalar::all ( 0 ) );
	double temp=0;
	// --------------------------------------------------------------------------------------------------
	//		描述：转换到HSI空间
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			//H

			temp = acos ( ( srcImage.at<Vec3b> ( j, k )[2]+ srcImage.at<Vec3b> ( j, k )[2] - srcImage.at<Vec3b> ( j, k )[1] - srcImage.at<Vec3b> ( j, k )[0] )*0.5 /
				sqrt ( pow ( (srcImage.at<Vec3b> ( j, k )[2] - srcImage.at<Vec3b> ( j, k )[1]), 2 )
					+ (srcImage.at<Vec3b> ( j, k )[2] - srcImage.at<Vec3b> ( j, k )[0])*(srcImage.at<Vec3b> ( j, k )[1] - srcImage.at<Vec3b> ( j, k )[0]) ));

			if (srcImage.at<Vec3b> ( j, k )[0]>srcImage.at<Vec3b> ( j, k )[1])
				H_Array.at<uchar> ( j, k ) =( (1 - (temp*0.5/ 3.1415926))*255+0.5);
			else
			{
				H_Array.at<uchar> ( j, k ) = ( (temp*0.5/3.1415926 )*255+0.5);
			}
			//S
			S_Array.at<uchar> ( j, k ) =255* (1 - 3.0 * min(min ( srcImage.at<Vec3b> ( j, k )[0], srcImage.at<Vec3b> ( j, k )[1] ), srcImage.at<Vec3b> ( j, k )[2] ) / (srcImage.at<Vec3b> ( j, k )[0] + srcImage.at<Vec3b> ( j, k )[1] + srcImage.at<Vec3b> ( j, k )[2] ));
			//I
			I_Array.at<uchar> ( j, k ) =  (srcImage.at<Vec3b> ( j, k )[0] + srcImage.at<Vec3b> ( j, k )[1] + srcImage.at<Vec3b> ( j, k )[2])/3+0.5 ;
		}
	}

	imshow ( "H分量", H_Array );
	imshow ( "S分量", S_Array );
	imshow ( "I分量", I_Array );
}


void CImage_ProcessingView::OnRgbEqualization ()
{
	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"));
		return;
	}
	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度
	
	// --------------------------------------------------------------------------------------------------
	//		描述：定义直方图数组，统计直方图
	//--------------------------------------------------------------------------------------------------
	int NumPixel_R[256] = {0};
	int NumPixel_G[256] = { 0 };
	int NumPixel_B[256] = { 0 };
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			NumPixel_R[m_Image.m_pBits[2][j][k]]++;
			NumPixel_G[m_Image.m_pBits[1][j][k]]++;
			NumPixel_B[m_Image.m_pBits[0][j][k]]++;
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：计算亮度分布密度
	//---------------------------------------------------------------------------------------------------
	float ProbPixel_R[256] = {0};
	float ProbPixel_G[256] = { 0 };
	float ProbPixel_B[256] = { 0 };
	for (int i = 0; i < 256; i++)
	{
		ProbPixel_R[i] = (double)NumPixel_R[i] / (w*h*1.0);
		ProbPixel_G[i] = (double)NumPixel_G[i] / (w*h*1.0);
		ProbPixel_B[i] = (double)NumPixel_B[i] / (w*h*1.0);
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：计算累计直方图分布
	//---------------------------------------------------------------------------------------------------
	double CumuPixel_R[256] = {0};
	double CumuPixel_G[256] = { 0 };
	double CumuPixel_B[256] = { 0 };
	CumuPixel_R[0] = ProbPixel_R[0];
	CumuPixel_G[0] = ProbPixel_G[0];
	CumuPixel_B[0] = ProbPixel_B[0];
	for (int i = 1; i < 256; i++)
	{
			CumuPixel_R[i] = CumuPixel_R[i - 1] + ProbPixel_R[i];
			CumuPixel_G[i] = CumuPixel_G[i - 1] + ProbPixel_G[i];
			CumuPixel_B[i] = CumuPixel_B[i - 1] + ProbPixel_B[i];
	}

	// --------------------------------------------------------------------------------------------------
	//		描述：累计分布取整
	//---------------------------------------------------------------------------------------------------
	int NumPixel_equa_R[256] = {0};
	int NumPixel_equa_G[256] = { 0 };
	int NumPixel_equa_B[256] = { 0 };
	for (int i = 0; i < 256; i++)
	{
		NumPixel_equa_R[i] = (int)(CumuPixel_R[i] * 255.0 + 0.5);
		NumPixel_equa_G[i] = (int)(CumuPixel_G[i] * 255.0 + 0.5);
		NumPixel_equa_B[i] = (int)(CumuPixel_B[i] * 255.0 + 0.5);
	
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：对亮度值进行映射（均衡化)
	//---------------------------------------------------------------------------------------------------
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[2][j][k] = NumPixel_equa_R[m_Image.m_pBits[2][j][k]];
			m_Image.m_pBits[1][j][k] = NumPixel_equa_G[m_Image.m_pBits[1][j][k]];
			m_Image.m_pBits[0][j][k] = NumPixel_equa_B[m_Image.m_pBits[0][j][k]];
		}
	}

	Invalidate(1); //强制调用ONDRAW函数
}


void CImage_ProcessingView::OnColorEqualization ()
{
	if (m_Image.IsNull ()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox ( _T ( "请先打开一幅图像！" ), L"提示" );
		return;
	}
	int w = m_Image.GetWidth ();//获得图像的宽度
	int h = m_Image.GetHeight ();//获得图像的高度
	Mat srcImage;
	Mat outImage(h,w, CV_8UC3, Scalar::all ( 0 ) );
	string str_path;
	str_path = CT2A ( m_Image.PathName.GetBuffer () );
	srcImage = imread ( str_path, 1 );
	
	Mat H_Array ( h, w, CV_64FC1, Scalar::all ( 0 ) );
	Mat S_Array ( h, w, CV_64FC1, Scalar::all ( 0 ) );
	Mat I_Array ( h, w, CV_8UC1, Scalar::all ( 0 ) );
	double temp = 0;
	int tempint = 0;

	// --------------------------------------------------------------------------------------------------
	//		描述：RGB转换到HSI空间
	//---------------------------------------------------------------------------------------------------

	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			//H
			if ((srcImage.at<Vec3b> ( j, k )[2] == srcImage.at<Vec3b> ( j, k )[1]) && (srcImage.at<Vec3b> ( j, k )[1] == srcImage.at<Vec3b> ( j, k )[0]))
				temp = 3.1415926*0.5;
			else
			{
				temp = acos ( (srcImage.at<Vec3b> ( j, k )[2] + srcImage.at<Vec3b> ( j, k )[2] - srcImage.at<Vec3b> ( j, k )[1] - srcImage.at<Vec3b> ( j, k )[0])*0.5 /
					sqrt ( pow ( (srcImage.at<Vec3b> ( j, k )[2] - srcImage.at<Vec3b> ( j, k )[1]), 2 )
						+ (srcImage.at<Vec3b> ( j, k )[2] - srcImage.at<Vec3b> ( j, k )[0])*(srcImage.at<Vec3b> ( j, k )[1] - srcImage.at<Vec3b> ( j, k )[0]) ) );
			}

			if (srcImage.at<Vec3b> ( j, k )[0]>srcImage.at<Vec3b> ( j, k )[1])
				H_Array.at<double> ( j, k ) = 2 * 3.1415926 - temp;
			else
			{
				H_Array.at<double> ( j, k ) = temp;
			}
			//S
			S_Array.at<double> ( j, k ) =  (1 - 3.0 * min ( min ( srcImage.at<Vec3b> ( j, k )[0], srcImage.at<Vec3b> ( j, k )[1] ), srcImage.at<Vec3b> ( j, k )[2] ) / (srcImage.at<Vec3b> ( j, k )[0] + srcImage.at<Vec3b> ( j, k )[1] + srcImage.at<Vec3b> ( j, k )[2]));
			//I
			I_Array.at<uchar> ( j, k ) = (srcImage.at<Vec3b> ( j, k )[0] + srcImage.at<Vec3b> ( j, k )[1] + srcImage.at<Vec3b> ( j, k )[2] ) / 3+0.5;
		}
	}

	// --------------------------------------------------------------------------------------------------
	//		描述：对I分量均衡化
	//--------------------------------------------------------------------------------------------------
	int NumPixel[256] = { 0 };
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			NumPixel[I_Array.at<uchar> ( j, k )]++;
		}
	}

	float ProbPixel[256] = { 0 };
	for (int i = 0; i < 256; i++)
	{
		ProbPixel[i] = (double)NumPixel[i] / (w*h*1.0);
	}
	
	double CumuPixel[256] = { 0 };
	CumuPixel[0] = ProbPixel[0];
	for (int i = 1; i < 256; i++)
	{
		CumuPixel[i] = CumuPixel[i - 1] + ProbPixel[i];
	}


	int NumPixel_equa[256] = { 0 };
	for (int i = 0; i < 256; i++)
	{
		NumPixel_equa[i] = (int)(CumuPixel[i] * 255.0 + 0.5);
	}

	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			I_Array.at<uchar> ( j, k ) = NumPixel_equa[I_Array.at<uchar> ( j, k )];
		}
	}

	// --------------------------------------------------------------------------------------------------
	//		描述：HSI转换到RGB空间
	//---------------------------------------------------------------------------------------------------
	//double test;
	Mat tempImage(h,w, CV_64FC3, Scalar::all ( 0 ) );
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			if (H_Array.at<double>(j,k) < 2* 3.1415926/3.0)
			{
			/*	test = cos ( H_Array.at<double> ( j, k ) );
				test= cos ( 3.1415926 / 3.0 - H_Array.at<double> ( j, k ) );
				test = I_Array.at<uchar> ( j, k );
				test= I_Array.at<uchar> ( j, k )*(1 - S_Array.at<double> ( j, k ));
				test= I_Array.at<uchar> ( j, k )*(1 + S_Array.at<double> ( j, k ) *cos ( H_Array.at<double> ( j, k ) ) / cos ( 3.1415926 / 3.0 - H_Array.at<double> ( j, k ) ));
				test= 3 * I_Array.at<uchar> ( j, k ) - tempImage.at<Vec3d> ( j, k )[0] - tempImage.at<Vec3d> ( j, k )[2];*/
				tempImage.at<Vec3d> ( j, k )[0] = I_Array.at<uchar> ( j, k )*(1 - S_Array.at<double> ( j, k ));
				tempImage.at<Vec3d> ( j, k )[2] = I_Array.at<uchar> ( j, k )*(1 + S_Array.at<double> ( j, k ) *cos ( H_Array.at<double> ( j, k ) ) / cos  ( 3.1415926 / 3.0 -H_Array.at<double> ( j, k ) ) );
				tempImage.at<Vec3d> ( j, k )[1] = 3 * I_Array.at<uchar> ( j, k ) - tempImage.at<Vec3d> ( j, k )[0] - tempImage.at<Vec3d> ( j, k )[2];
			}
			else if ((H_Array.at<double> ( j, k ) >= 2 * 3.1415926 / 3.0 ) && H_Array.at<double> ( j, k ) < 4 * 3.1415926 / 3.0)
			{
				H_Array.at<double> ( j, k ) = H_Array.at<double> ( j, k ) - 2 * 3.1415926 / 3.0;
				tempImage.at<Vec3d> ( j, k )[2] = I_Array.at<uchar> ( j, k )*(1 - S_Array.at<double> ( j, k ));
				tempImage.at<Vec3d> ( j, k )[1] = I_Array.at<uchar> ( j, k )*(1 + S_Array.at<double> ( j, k )*cos ( H_Array.at<double> ( j, k ) ) / cos  ( 3.1415926 / 3.0 - H_Array.at<double> ( j, k ) ) );
				tempImage.at<Vec3d> ( j, k )[0] = 3 * I_Array.at<uchar> ( j, k ) - tempImage.at<Vec3d> ( j, k )[1] - tempImage.at<Vec3d> ( j, k )[2];
			
			}
			else
			{

			/*	test = H_Array.at<double> ( j, k );
				test = H_Array.at<double> ( j, k ) - 4 * 3.1415926 / 3.0;
				test= I_Array.at<uchar> ( j, k )*(1 - S_Array.at<double> ( j, k ));
				test= I_Array.at<uchar> ( j, k )*(1 + S_Array.at<double> ( j, k ) *cos ( H_Array.at<double> ( j, k ) ) / cos ( 3.1415926 / 3.0 - H_Array.at<double> ( j, k ) ));
				test= 3 * I_Array.at<uchar> ( j, k ) - tempImage.at<Vec3d> ( j, k )[0] - tempImage.at<Vec3d> ( j, k )[1];*/
				H_Array.at<double> ( j, k ) = H_Array.at<double> ( j, k ) - 4 * 3.1415926 / 3.0;
				tempImage.at<Vec3d> ( j, k )[1] = I_Array.at<uchar> ( j, k )*(1 - S_Array.at<double> ( j, k ));
				tempImage.at<Vec3d> ( j, k )[0] = I_Array.at<uchar> ( j, k )*(1 + S_Array.at<double> ( j, k ) *cos ( H_Array.at<double> ( j, k ) ) / cos  ( 3.1415926 / 3.0 - H_Array.at<double> ( j, k ) ) );
				tempImage.at<Vec3d> ( j, k )[2] = 3 * I_Array.at<uchar> ( j, k ) - tempImage.at<Vec3d> ( j, k )[0] - tempImage.at<Vec3d> ( j, k )[1];
			}
		}
	}
	normalize ( tempImage , tempImage,0,255, NORM_MINMAX );
	tempImage.convertTo(outImage,CV_8UC3);
	/*imshow ( "处理后的图", outImage );*/
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			m_Image.m_pBits[0][j][k] = outImage.at<Vec3b>(j,k)[0];
			m_Image.m_pBits[1][j][k] = outImage.at<Vec3b>(j,k)[1];
			m_Image.m_pBits[2][j][k] = outImage.at<Vec3b>(j,k)[2];
		}
	}
	Invalidate ( 1 );
}

void DrawRectangle(cv::Mat & img, cv::Rect box)
{
	rectangle(img, box.tl(), box.br(), Scalar(0, 0, 255));
}

void CImage_ProcessingView::OnColorSegToBinary ()
{
	if (m_Image.IsNull ()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox ( _T ( "请先打开一幅图像！" ), L"提示" );
		return;
	}
	int w = m_Image.GetWidth ();//获得图像的宽度
	int h = m_Image.GetHeight ();//获得图像的高度
	g_rectangle = Rect(-1, -1, 0, 0);
	Mat srcImage;
	string str_path;
	str_path = CT2A(m_Image.PathName.GetBuffer());
	srcImage = imread(str_path);
	Mat tempImage;
	srcImage.copyTo(tempImage);
	namedWindow("请选择参考区域");
	setMouseCallback("请选择参考区域", on_MouseHandle, (void*)&tempImage);
	WindowOpen = true;
	while (WindowOpen)
	{
		srcImage.copyTo(tempImage);//清理鼠标抬起前画的矩形，只保留当前鼠标位置的矩形，与imshow的顺序不能换，换了就将当前鼠标位置的矩形也清理掉了
		if (g_bDrawingBox)DrawRectangle(tempImage, g_rectangle);
		imshow("请选择参考区域", tempImage);

		if (waitKey(10) == 27) break;//按下ESC退出
	}
	cv::destroyWindow("请选择参考区域");
	int minx = min(StartPointx, EndPointx);
	int maxx = max(StartPointx, EndPointx);
	int miny = min(StartPointy, EndPointy);
	int maxy = max(StartPointy, EndPointy);
	double Avg = 0;
	for (int i = minx; i != maxx; i++)
	{
		for (int j = miny; j != maxy; j++)
		{
			Avg += (srcImage.at<Vec3b>(i, j)[0] + srcImage.at<Vec3b>(i, j)[1] + srcImage.at<Vec3b>(i, j)[2]) / (1.0*(maxx-minx)*(maxy-miny));
		}
	}

	for (int i = 0; i != srcImage.rows; i++)
		for (int j = 0; j != srcImage.cols; j++)
		{
			if ((srcImage.at<Vec3b>(i, j)[0] + srcImage.at<Vec3b>(i, j)[1] + srcImage.at<Vec3b>(i, j)[2]) > Avg)
			{
				srcImage.at<Vec3b>(i, j)[0] = 255;
				srcImage.at<Vec3b>(i, j)[1] = 255;
				srcImage.at<Vec3b>(i, j)[2] = 255;
			}
			else
			{
				srcImage.at<Vec3b>(i, j)[0] = 0;
				srcImage.at<Vec3b>(i, j)[1] = 0;
				srcImage.at<Vec3b>(i, j)[2] = 0;
			}
		}
	imshow("二值化", srcImage);
}

void on_MouseHandle(int event, int x, int y, int flags, void * param)
{
	Mat& image = *(cv::Mat*)param;//创建一个Mat类型的引用;param存的是地址，*param存的是内容，（cv::Mat* ）将param类型转换为指向Mat类型的指针
	switch (event)
	{
	case EVENT_MOUSEMOVE://鼠标移动
	{
		if (g_bDrawingBox)
		{
			g_rectangle.width = x - g_rectangle.x;
			g_rectangle.height = y - g_rectangle.y;
		}
	}
	break;
	case EVENT_LBUTTONDOWN://左键按下
	{
		g_bDrawingBox = true;
		g_rectangle = Rect(x, y, 0, 0);
		StartPointx = x;
		StartPointy = y;
	}
	break;
	case EVENT_LBUTTONUP://左键抬起
	{
		g_bDrawingBox = false;
		//对宽和高小于0的处理
		if (g_rectangle.width < 0)
		{
			g_rectangle.x += g_rectangle.width;
			g_rectangle.width *= -1;
		}
		if (g_rectangle.height < 0)
		{
			g_rectangle.y += g_rectangle.height;
			g_rectangle.height *= -1;
		}
		//调用函数进行绘制
		DrawRectangle(image, g_rectangle);
		EndPointx = x;
		EndPointy = y;
		WindowOpen = FALSE;
	}
	default:
		break;
	}
}


void CImage_ProcessingView::OnColorSegToBinaryMfc()
{
	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"), L"提示");
		return;
	}
	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度

	Mat srcImage;
	CStringA temp;
	temp = m_Image.PathName;
	string str_path(temp.GetBuffer());
	srcImage = imread(str_path);
	
	/*string str_path;
	str_path = CT2A ( m_Image.PathName.GetBuffer () );
	srcImage = imread ( str_path);*/
	DIALOG_COLOR_TO_BINARY dlg;
	
	int PicWidth = dlg.GetPicWidth ();
	int PicHeight = dlg.GetPicHeight ();
	Mat dstImage ( PicHeight, PicWidth, CV_8UC3 );
	cv::resize ( srcImage, dstImage, dstImage.size () );

	/*dlg.DrawPic(dstImage);*/
	/*CImage C_Image;
	C_Image.Create( PicWidth , PicHeight, 24 );
	uchar* ps;
	uchar* pimg = (uchar*)C_Image.GetBits ();
	int step = C_Image.GetPitch ();
	for (int i = 0; i < PicHeight; ++i)
	{
		ps = (dstImage.ptr<uchar>(i));
		for (int j = 0; j < PicWidth; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				*(pimg + i*step + j * 3 + k) = ps[j * 3 + k];
			}
		}
	}*/
	dlg.SendImage (dstImage);

	if (IDOK == dlg.DoModal())
	{
		if (dlg.FlagCancel)//如果是撤销状态，则直接返回
			return;
		int ThresholdValue = dlg.GetThresholdValue();
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < w; ++k)
			{
				if ((m_Image.m_pBits[0][j][k] + m_Image.m_pBits[1][j][k] + m_Image.m_pBits[2][j][k])>3*ThresholdValue)
				{
					m_Image.m_pBits[0][j][k] = 255;
					m_Image.m_pBits[1][j][k] = 255;
					m_Image.m_pBits[2][j][k] = 255;
				}
				else
				{
					m_Image.m_pBits[0][j][k] = 0;
					m_Image.m_pBits[1][j][k] = 0;
					m_Image.m_pBits[2][j][k] = 0;
				}
			}
		}
	}

	Invalidate(1);
	/*SetCursor(LoadCursor(NULL, IDC_CROSS));*/
	/*if (IDOK == dlg.DoModal())
	{
		dlg.DrawPic();
	}*/
}


void CImage_ProcessingView::OnBasGlobalThresholdSeg()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"), L"提示");
		return;
	}
	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度
	// --------------------------------------------------------------------------------------------------
	//		描述：如果是彩色图，转换为灰度图来处理
	//---------------------------------------------------------------------------------------------------
	if (m_Image.GetBPP() == 24)
	{
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < w; ++k)
			{
				m_Image.m_pBits[0][j][k] = (299 * m_Image.m_pBits[2][j][k] + 587 * m_Image.m_pBits[1][j][k] + 114 * m_Image.m_pBits[0][j][k] + 500) / 1000;
			}
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：初始化及第一个阈值T的选取
	//---------------------------------------------------------------------------------------------------
	int tempmax = 0;
	int tempmin = 255;
	int Flag_T = 1;//迭代停止条件
	int T = 0;
	int T_new = 0;
	//灰度值大于T的参数
	long CumValue1 = 0;
	int num1 = 0;
	int AvgValue1 = 0;
	//灰度值小于T的参数
	long CumValue2 = 0;
	int num2 = 0;
	int AvgValue2 = 0;
	//根据图像亮度值选取第一个T
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			if (m_Image.m_pBits[0][j][k]>tempmax)
				tempmax = m_Image.m_pBits[0][j][k];
			if (m_Image.m_pBits[0][j][k] < tempmin)
				tempmin = m_Image.m_pBits[0][j][k];
		}
	}
	T_new = (int)((tempmax + tempmin) / 2.0 + 0.5);
	// --------------------------------------------------------------------------------------------------
	//		描述：迭代
	//---------------------------------------------------------------------------------------------------
	do {
		T = T_new;
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < w; ++k)
			{
				if (m_Image.m_pBits[0][j][k]>T)
				{
					CumValue1 += m_Image.m_pBits[0][j][k];
					num1++;
				}

				else
				{
					CumValue2 += m_Image.m_pBits[0][j][k];
					num2++;
				}
			}
		}
		AvgValue1 = CumValue1 / num1 + 0.5;
		AvgValue2 = CumValue2 / num2 + 0.5;
		T_new = (int)((AvgValue1 + AvgValue2) / 2.0 + 0.5);
	} while ((T_new - T) > Flag_T);
	// --------------------------------------------------------------------------------------------------
	//		描述：分割
	//---------------------------------------------------------------------------------------------------
	T = T_new;
	for (int j = 0; j < h; ++j)
	{
		for (int k = 0; k < w; ++k)
		{
			if (m_Image.m_pBits[0][j][k]>T)
			{
				m_Image.m_pBits[0][j][k] = 255;
			}
			else
			{
				m_Image.m_pBits[0][j][k] = 0;
			}
		}
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：如果是彩色图，则需要同步另外两个分量
	//---------------------------------------------------------------------------------------------------
	if (m_Image.GetBPP() == 24)
	{
		for (int j = 0; j < h; ++j)
		{
			for (int k = 0; k < w; ++k)
			{
				m_Image.m_pBits[1][j][k] = m_Image.m_pBits[0][j][k];
				m_Image.m_pBits[2][j][k] = m_Image.m_pBits[0][j][k];
			}
		}
	}
	Invalidate(1);
}


void CImage_ProcessingView::OnExtractHolesAndCracks()
{
	// TODO: 在此添加命令处理程序代码
	//问题描述：1. 提取图中孔洞和裂缝
	//			2. 计算所有孔洞和裂缝的面积，生成一张EXCEL表格
	//			3. 计算裂缝的长度

	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"), L"提示");
		return;
	}
	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度
	Mat srcImage;
	CStringA temp;
	temp = m_Image.PathName;
	string str_path(temp.GetBuffer());
	srcImage = imread(str_path);

	ExtractHolesAndCracks dlg;
	dlg.SendImage(srcImage);
	if (IDOK == dlg.DoModal())
	{

	}
}
