// ExtractHolesAndCracks.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "ExtractHolesAndCracks.h"
#include "afxdialogex.h"


// ExtractHolesAndCracks 对话框

IMPLEMENT_DYNAMIC(ExtractHolesAndCracks, CDialogEx)

ExtractHolesAndCracks::ExtractHolesAndCracks(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXT_HOLES_AND_CRACKS, pParent)
	, EditThresholdValue(_T(""))
{

}

ExtractHolesAndCracks::~ExtractHolesAndCracks()
{
}

void ExtractHolesAndCracks::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PicView, Img_Static);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider1);
	DDX_Text(pDX, IDC_EDIT1, EditThresholdValue);
}

void ExtractHolesAndCracks::SendImage(Mat img)
{
	img.copyTo(srcImage);
}

void ExtractHolesAndCracks::DrawPic(Mat tempImage)
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

	HBITMAP hBmp = m_CImage.Detach();

	Img_Static.SetBitmap(hBmp);
	Img_Static.SetWindowPos(NULL,
		0,
		0,
		PicWidth,
		PicHeight,
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
}

void ExtractHolesAndCracks::DoThresholdSeg()
{
	Mat tempImage = FixSizeImage.clone();
	for (int i = 0; i != PicHeight; i++)
		for (int j = 0; j != PicWidth; j++)
		{
			if ((tempImage.at<Vec3b>(i, j)[0] + tempImage.at<Vec3b>(i, j)[1] + tempImage.at<Vec3b>(i, j)[2]) < ThresholdValue*3)
			{
				tempImage.at<Vec3b>(i, j)[0] = 255;
				tempImage.at<Vec3b>(i, j)[1] = 0;
				tempImage.at<Vec3b>(i, j)[2] = 0;
			}
			else
			{
				
			}
		}
	DrawPic(tempImage);
}


BEGIN_MESSAGE_MAP(ExtractHolesAndCracks, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &ExtractHolesAndCracks::OnNMCustomdrawSlider1)
	ON_WM_HSCROLL()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_THRESHOLDVALUE_FINISH, &ExtractHolesAndCracks::OnBnClickedThresholdvalueFinish)
	ON_BN_CLICKED(IDC_BUTTON1, &ExtractHolesAndCracks::OnBnClickedFileSaveAs)
END_MESSAGE_MAP()


// ExtractHolesAndCracks 消息处理程序


BOOL ExtractHolesAndCracks::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//滑块初始化
	m_Slider1.SetRange(0, 255);
	m_Slider1.SetPos(120);
	//图片显示初始化
	Mat dstimage(PicHeight, PicWidth, CV_8UC3);
	cv::resize(srcImage, dstimage, dstimage.size());
	FixSizeImage = dstimage.clone();
	DrawPic(FixSizeImage);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void ExtractHolesAndCracks::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	ThresholdValue = m_Slider1.GetPos();
	EditThresholdValue.Format(TEXT("%.3d"), ThresholdValue);
	UpdateData(FALSE);
	DoThresholdSeg();
	*pResult = 0;
	
}




HBRUSH ExtractHolesAndCracks::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	//初始化部分的背景刷新
	//if (FirstTime)
	//{
	//	FirstTime--;
	//	return hbr;
	//}
	////移动滑动条时的背景刷新
	//if (SecondTime)
	//{
	//	SecondTime--;
	//	return (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	//}
	//else
	//{
	//	SecondTime = 5;
	//	return hbr;
	//}
	return hbr;
}


void ExtractHolesAndCracks::OnBnClickedThresholdvalueFinish()
{
	// TODO: 在此添加控件通知处理程序代码
	//本部分功能：1. 在原图上分割出目标
	//				2. 统计裂缝，孔洞

	// --------------------------------------------------------------------------------------------------
	//		描述：分割出原图的目标
	//---------------------------------------------------------------------------------------------------
	//rows行数，高；cols列数，宽
	ListHoles.clear();
	ListCracks.clear();
	Mat BackImage = Mat::zeros(srcImage.size(), CV_8UC3);
	for (int i = 0; i != BackImage.rows; i++)
		for (int j = 0; j != BackImage.cols; j++)
		{
			if ((srcImage.at<Vec3b>(i, j)[0] + srcImage.at<Vec3b>(i, j)[1] + srcImage.at<Vec3b>(i, j)[2]) < ThresholdValue * 3)
			{
				BackImage.at<Vec3b>(i, j)[0] = 255;
				BackImage.at<Vec3b>(i, j)[1] = 0;
				BackImage.at<Vec3b>(i, j)[2] = 0;
			}
			else
			{
				BackImage.at<Vec3b>(i, j)[0] = 0;
				BackImage.at<Vec3b>(i, j)[1] = 0;
				BackImage.at<Vec3b>(i, j)[2] = 0;
			}
		}
	/*imshow("",BackImage);*/
	// --------------------------------------------------------------------------------------------------
	//		描述：查找连通域（8邻域）
	//---------------------------------------------------------------------------------------------------
	Mat TempImage = Mat::zeros(BackImage.size(), CV_8UC3);

	int NumberHoles = 0;//孔洞的数目
	int NumberAll = 0;//连通域的面积
	int NumberNow = 0;//当前为哪个点
	BOOL Flag_BackGround = TRUE;//背景连通域不纳入统计
	vector<vector<Point>> ListPoints;//按孔洞保存所有的点
	vector<Point> TempPoint;

	for (int i = 0; i != BackImage.rows; i++)
	{
		for (int j = 0; j != BackImage.cols; j++)
		{
			if (BackImage.at<Vec3b>(i, j)[0] == 255)
			{
				NumberAll = 0;
				NumberNow = 0;
				TempImage.at<Vec3b>(i, j)[0] = 255;
				TempPoint.push_back(Point(i, j));
				NumberAll++;
				NumberNow++;
				for (int m = max(i - 1,0); m <= min(i + 1, BackImage.rows-1); m++)
					for (int n = max(j - 1,0); n <= min(j + 1, BackImage.cols-1); n++)
					{
						if ((BackImage.at<Vec3b>(m, n)[0] == 255) && (TempImage.at<Vec3b>(m, n)[0] != 255))
						{
							TempImage.at<Vec3b>(m, n)[0] = 255;
							TempPoint.push_back(Point(m, n));
							NumberAll++;
						}
					}
				while (NumberAll != NumberNow)
				{
					for (int m = max(0,TempPoint[NumberNow].x - 1); m <= min(BackImage.rows-1,TempPoint[NumberNow].x + 1); m++)
						for (int n = max(0,TempPoint[NumberNow].y - 1); n <= min(BackImage.cols-1,TempPoint[NumberNow].y + 1); n++)
						{
							if ((BackImage.at<Vec3b>(m, n)[0] == 255) && (TempImage.at<Vec3b>(m, n)[0] != 255))
							{
								TempImage.at<Vec3b>(m, n)[0] = 255;
								TempPoint.push_back(Point(m, n));
								NumberAll++;
							}
						}
					NumberNow++;
				}
				for (int m = 0; m < NumberAll; m++)
				{
					BackImage.at<Vec3b>(TempPoint[m].x, TempPoint[m].y)[0] = 0;
				}
				if(Flag_BackGround)
				{
					Mat InputImage = srcImage.clone();
					for (int i = 0; i != BackImage.rows; i++)
					{
						for (int j = 0; j != BackImage.cols; j++)
						{
							if (BackImage.at<Vec3b>(i, j)[0] == 255)
							{
								InputImage.at<Vec3b>(i, j)[0] = 255;
								InputImage.at<Vec3b>(i, j)[1] = 0;
								InputImage.at<Vec3b>(i, j)[2] = 0;
							}
						}
					}
					imwrite("result1.BMP", InputImage);
				}
				if (!Flag_BackGround)
				{
					ListHoles.push_back(NumberAll);//将面积保存进表里
					NumberHoles++;
				}
				Flag_BackGround = FALSE;
				ListPoints.push_back(TempPoint);
				TempPoint.clear();
			}
		}
	}
	/*imshow("", TempImage);*/

	// --------------------------------------------------------------------------------------------------
	//		描述：算周长，找到裂缝,寻找边界点的数量，四邻域中存在0点，则为边界点
	//---------------------------------------------------------------------------------------------------
	
	int TempLength = 0;
	vector<int>::iterator del = ListHoles.begin();
	
	for (int i = 1; i < NumberHoles; i++)
	{
		for (int j = 0; j < ListPoints[i].size(); j++)
		{
			if ((TempImage.at<Vec3b>(ListPoints[i][j].x - 1, ListPoints[i][j].y)[0] == 0) ||
				/*(TempImage.at<Vec3b>(ListPoints[i][j].x - 1, ListPoints[i][j].y - 1)[0] == 0) ||
				(TempImage.at<Vec3b>(ListPoints[i][j].x - 1, ListPoints[i][j].y + 1)[0] == 0) ||
				(TempImage.at<Vec3b>(ListPoints[i][j].x + 1, ListPoints[i][j].y - 1)[0] == 0) ||
				(TempImage.at<Vec3b>(ListPoints[i][j].x + 1, ListPoints[i][j].y + 1)[0] == 0) ||*/
				(TempImage.at<Vec3b>(ListPoints[i][j].x + 1, ListPoints[i][j].y)[0] == 0) ||
				(TempImage.at<Vec3b>(ListPoints[i][j].x, ListPoints[i][j].y - 1)[0] == 0) ||
				(TempImage.at<Vec3b>(ListPoints[i][j].x, ListPoints[i][j].y + 1)[0] == 0))
			{
				TempLength++;
			}
		}
		
		if (TempLength > srcImage.rows)
		{
			ListCracks.push_back(Point(ListHoles[i-1], TempLength));
			ListHoles.erase(del);
		}
		del++;
		TempLength = 0;
	}
	// --------------------------------------------------------------------------------------------------
	//		描述：文件输出及格式转换  (弃用，改为输出Excel文档)
	//---------------------------------------------------------------------------------------------------
	/*ofstream FileOut;
	string name = "统计结果.txt";
	FileOut.open(name);
	CString tempname;
	tempname.Format(TEXT("本样品一共%d个裂缝，%d个孔洞，采用的分割阈值为：%d。"), ListCracks.size(), ListHoles.size(),ThresholdValue);
	CStringA temp;
	temp = tempname;
	char* obj = temp.GetBuffer();
	FileOut << obj << endl;
	for (int i = 0; i < ListCracks.size(); i++)
	{
		int Area = ListCracks[i].x;
		int Length = ListCracks[i].y *0.5;
		tempname.Format(TEXT("第%d个裂缝的面积为：%d，长度为%d"), i, Area, Length);
		CStringA temp;
		temp = tempname;
		char* obj = temp.GetBuffer();
		FileOut << obj << endl;
	}
	for (int i = 0; i < ListHoles.size(); i++)
	{
		tempname.Format(TEXT("第%d个孔洞的面积为：%d"), i, ListHoles[i]);
		CStringA temp;
		temp = tempname;
		char* obj = temp.GetBuffer();
		FileOut << obj << endl;
	}*/

	MessageBox(_T("处理完成！"), L"提示");
	
}


void ExtractHolesAndCracks::OnBnClickedFileSaveAs()
{
	// TODO: 在此添加控件通知处理程序代码
	// --------------------------------------------------------------------------------------------------
	//		描述：对话框选择保存目录
	//---------------------------------------------------------------------------------------------------
	CString strFilter;
	//strFilter ="位图文件|*.bmp|JPEG 图像文件|*.jpg|GIF 图像文件|*.gif|PNG 图像文件|*.png||";   //
	strFilter ="Excel (*.xlsx)|*.*||";   //
	CFileDialog dlg(FALSE,NULL,NULL,NULL,strFilter);
	if ( IDOK != dlg.DoModal()) 
		return;
	// 如果用户没有指定文件扩展名，则为其添加一个
	CString strFileName;
	//CString strExtension;
	//strFileName = dlg.m_ofn.lpstrFile;   //  获得文件名
	strFileName = dlg.GetPathName();
	// --------------------------------------------------------------------------------------------------
	//		描述：//excel 接口部分
	//---------------------------------------------------------------------------------------------------
	_excel::CApplication app;
	_excel::CFont0 font;
	_excel::CRange range;
	_excel::CRange col;
	_excel::CWorkbook book;
	_excel::CWorkbooks books;
	_excel::CWorksheet sheet;
	_excel::CWorksheets sheets;
	CoInitialize(NULL);//初始化COM，最后还有CoUninitialize
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	if (!app.CreateDispatch(_T("EXCEL.application"))) //启动excel
	{
		AfxMessageBox(_T("居然你连OFFICE都没有安装吗?"));
		return;
	}
	app.put_Visible(TRUE);//设置表可见性
	app.put_DisplayFullScreen(FALSE);//设置全屏显示
	app.put_DisplayAlerts(FALSE);//屏蔽警告

	books = app.get_Workbooks();
	book = books.Add(vtMissing);//新建一个book
	
	//获取工作表worksheet(三种方法)
	sheet = book.get_ActiveSheet();
	sheets = book.get_Sheets();
	//worksheet = worksheets.get_Item((_variant_t("sheet2")));
	//worksheet = worksheets.get_Item((_variant_t(1)));

	// --------------------------------------------------------------------------------------------------
	//		描述：编写文件内容
	//---------------------------------------------------------------------------------------------------
	CString TempStr;
	range = sheet.get_Range(_variant_t("A1"), _variant_t("G1"));
	range.Merge(_variant_t((long)0));
	TempStr.Format(TEXT("本样品一共%d个裂缝，%d个孔洞，采用的分割阈值为：%d"), ListCracks.size(), ListHoles.size(), ThresholdValue);
	range.put_Value2(_variant_t(TempStr));//参数是二维数组可以批量设置单元格值
	range = sheet.get_Range(_variant_t("A2"), _variant_t("A2"));
	range.put_Value2(_variant_t("孔洞编号"));
	range = sheet.get_Range(_variant_t("B2"), _variant_t("B2"));
	range.put_Value2(_variant_t("面积"));
	range = sheet.get_Range(_variant_t("D2"), _variant_t("D2"));
	range.put_Value2(_variant_t("面积"));
	range = sheet.get_Range(_variant_t("C2"), _variant_t("C2"));
	range.put_Value2(_variant_t("裂缝编号"));
	range = sheet.get_Range(_variant_t("E2"), _variant_t("E2"));
	range.put_Value2(_variant_t("长度"));
	//
	//int* TempNumber;
	//TempNumber = new int[ListHoles.size()];
	//本来尝试用二维数组批量赋值，但是尝试失败
	for (int i = 0; i <  ListHoles.size(); i++)
	{
		int Temp = i + 3;
		TempStr.Format(TEXT("A%d"), Temp);
		range = sheet.get_Range(_variant_t(TempStr), _variant_t(TempStr));
		TempStr.Format(TEXT("孔%d"), i);
		range.put_Value2(_variant_t(TempStr));
		TempStr.Format(TEXT("B%d"), Temp);
		range = sheet.get_Range(_variant_t(TempStr), _variant_t(TempStr));
		range.put_Value2(_variant_t(ListHoles[i]));
	}
	for (int i = 0; i < ListCracks.size(); i++)
	{
		int Temp = i + 3;
		TempStr.Format(TEXT("C%d"), Temp);
		range = sheet.get_Range(_variant_t(TempStr), _variant_t(TempStr));
		TempStr.Format(TEXT("裂%d"), i);
		range.put_Value2(_variant_t(TempStr));
		TempStr.Format(TEXT("D%d"), Temp);
		range = sheet.get_Range(_variant_t(TempStr), _variant_t(TempStr));
		range.put_Value2(_variant_t(ListCracks[i].x));
		TempStr.Format(TEXT("E%d"), Temp);
		range = sheet.get_Range(_variant_t(TempStr), _variant_t(TempStr));
		int length = ListCracks[i].y*0.5;
		range.put_Value2(_variant_t(length));
	}
	
	//设置单元格格式
	//range = sheet.get_Range(_variant_t("F2"), _variant_t("G5"));
	////range.put_HorizontalAlignment(_variant_t(XlHAlign::xlHAlignRight));//水平对齐
	////range.put_VerticalAlignment(_variant_t(XlVAlign::xlVAlignBottom));//竖直对齐
	//range.put_ColumnWidth(_variant_t(35));//单位不明
	//range.put_RowHeight(_variant_t(24));//磅
	//font = range.get_Font();//字体
	//font.put_Italic(_variant_t(1));//斜体
	//interior = range.get_Interior();//底色
	/*interior.put_Color(_variant_t(76));*/

	////获取单元格内容
	//CString getdata;//定义字符串变量
	//_variant_t rValue;
	//range = sheet.get_Range(_variant_t("A1"), _variant_t("A1"));
	//rValue = range.get_Value2();
	//getdata = rValue.bstrVal;

	//未添加线框类
	////线框，border会对range中所有单元格都做设置，
	//range = worksheet.get_Range(_variant_t("A5"), _variant_t("C9"));
	//borders = range.get_Borders();
	//borders.put_LineStyle(_variant_t(XlLineStyle::xlDouble));
	//range = worksheet.get_Range(_variant_t("A12"), _variant_t("C13"));
	//range.BorderAround(_variant_t(XlLineStyle::xlDashDot), 2, 8, vtMissing);//外框线

	//另存为
	covOptional = COleVariant((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	COleVariant varZero((short)0);
	COleVariant varTrue(short(1), VT_BOOL);
	COleVariant varFalse(short(0), VT_BOOL);

	book.SaveAs(COleVariant(strFileName), vtMissing, vtMissing, vtMissing,
		vtMissing, vtMissing, (long)0, vtMissing,
		vtMissing, vtMissing, vtMissing, vtMissing, vtMissing);
	//workbook.Save();

	/*AfxMessageBox(_T(""));*/

	//所有东西都需要ReleaseDispatch，否则会报错
	/*borders.ReleaseDispatch();*/

	font.ReleaseDispatch();
	/*interior.ReleaseDispatch();*/
	range.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	//app.Quit();//Excel退出
	app.ReleaseDispatch();
	CoUninitialize();//对应CoInitialize
	/*OnOK();*///退出子对话框
}
