#pragma once
#include "afxwin.h"
#include "afxcmn.h"
using namespace cv;
using namespace std;

// ExtractHolesAndCracks �Ի���

class ExtractHolesAndCracks : public CDialogEx
{
	DECLARE_DYNAMIC(ExtractHolesAndCracks)

public:
	ExtractHolesAndCracks(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ExtractHolesAndCracks();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXT_HOLES_AND_CRACKS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CImage m_CImage;
	Mat srcImage;
	Mat FixSizeImage;
public:
	vector<Point> ListCracks;//�����ѷ��������ܳ�
	vector<int> ListHoles;//�ܱ�
	int ThresholdValue=120;
	int FirstTime = 7;
	int SecondTime = 5;
	const int PicHeight = 400;
	const int PicWidth = 400;
	void SendImage(Mat img);
	void DrawPic(Mat tempImage);
	void DoThresholdSeg();
	CStatic Img_Static;
	virtual BOOL OnInitDialog();

	// ��ǰ�����ֵ��Ϊ��ֵ�ָ����ֵ

	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_Slider1;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString EditThresholdValue;
	afx_msg void OnBnClickedThresholdvalueFinish();
	afx_msg void OnBnClickedFileSaveAs();
};
