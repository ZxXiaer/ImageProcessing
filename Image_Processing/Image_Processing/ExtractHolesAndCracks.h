#pragma once
#include "afxwin.h"
#include "afxcmn.h"
using namespace cv;
using namespace std;

// ExtractHolesAndCracks 对话框

class ExtractHolesAndCracks : public CDialogEx
{
	DECLARE_DYNAMIC(ExtractHolesAndCracks)

public:
	ExtractHolesAndCracks(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ExtractHolesAndCracks();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXT_HOLES_AND_CRACKS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CImage m_CImage;
	Mat srcImage;
	Mat FixSizeImage;
public:
	vector<Point> ListCracks;//保存裂缝的面积，周长
	vector<int> ListHoles;//总表
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

	// 当前滑块的值做为二值分割的阈值

	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_Slider1;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString EditThresholdValue;
	afx_msg void OnBnClickedThresholdvalueFinish();
	afx_msg void OnBnClickedFileSaveAs();
};
