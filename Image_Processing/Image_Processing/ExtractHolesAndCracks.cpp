// ExtractHolesAndCracks.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "ExtractHolesAndCracks.h"
#include "afxdialogex.h"


// ExtractHolesAndCracks �Ի���

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
END_MESSAGE_MAP()


// ExtractHolesAndCracks ��Ϣ�������


BOOL ExtractHolesAndCracks::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//�����ʼ��
	m_Slider1.SetRange(0, 255);
	m_Slider1.SetPos(120);
	//ͼƬ��ʾ��ʼ��
	Mat dstimage(PicHeight, PicWidth, CV_8UC3);
	cv::resize(srcImage, dstimage, dstimage.size());
	FixSizeImage = dstimage.clone();
	DrawPic(FixSizeImage);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void ExtractHolesAndCracks::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ThresholdValue = m_Slider1.GetPos();
	EditThresholdValue.Format(TEXT("%.3d"), ThresholdValue);
	UpdateData(FALSE);
	DoThresholdSeg();
	*pResult = 0;
	
}




HBRUSH ExtractHolesAndCracks::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	//��ʼ�����ֵı���ˢ��
	//if (FirstTime)
	//{
	//	FirstTime--;
	//	return hbr;
	//}
	////�ƶ�������ʱ�ı���ˢ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�����ֹ��ܣ�1. ��ԭͼ�Ϸָ��Ŀ��
	//				2. ͳ���ѷ죬�׶�

	// --------------------------------------------------------------------------------------------------
	//		�������ָ��ԭͼ��Ŀ��
	//---------------------------------------------------------------------------------------------------
	//rows�������ߣ�cols��������
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
	//		������������ͨ��8����
	//---------------------------------------------------------------------------------------------------
	Mat TempImage = Mat::zeros(BackImage.size(), CV_8UC3);

	vector<int> ListHoles;//�ܱ�
	int NumberHoles = 0;//�׶�����Ŀ
	int NumberAll = 0;//��ͨ������
	int NumberNow = 0;//��ǰΪ�ĸ���
	BOOL Flag_BackGround = TRUE;//������ͨ������ͳ��
	vector<vector<Point>> ListPoints;//���׶��������еĵ�
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
					ListHoles.push_back(NumberAll);//��������������
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
	//		���������ܳ����ҵ��ѷ�,Ѱ�ұ߽����������������д���0�㣬��Ϊ�߽��
	//---------------------------------------------------------------------------------------------------
	vector<Point> ListCracks;//�����ѷ��������ܳ�
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
	//		�������ļ��������ʽת��
	//---------------------------------------------------------------------------------------------------
	ofstream FileOut;
	string name = "ͳ�ƽ��.txt";
	FileOut.open(name);
	CString tempname;
	tempname.Format(TEXT("����Ʒһ��%d���ѷ죬%d���׶������õķָ���ֵΪ��%d��"), ListCracks.size(), ListHoles.size(),ThresholdValue);
	CStringA temp;
	temp = tempname;
	char* obj = temp.GetBuffer();
	FileOut << obj << endl;
	for (int i = 0; i < ListCracks.size(); i++)
	{
		int Area = ListCracks[i].x;
		int Length = ListCracks[i].y *0.5;
		tempname.Format(TEXT("��%d���ѷ�����Ϊ��%d������Ϊ%d"), i, Area, Length);
		CStringA temp;
		temp = tempname;
		char* obj = temp.GetBuffer();
		FileOut << obj << endl;
	}
	for (int i = 0; i < ListHoles.size(); i++)
	{
		tempname.Format(TEXT("��%d���׶������Ϊ��%d"), i, ListHoles[i]);
		CStringA temp;
		temp = tempname;
		char* obj = temp.GetBuffer();
		FileOut << obj << endl;
	}

	/*MessageBox(_T("������ɣ�"), L"��ʾ");*/
	
}
