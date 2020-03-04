#pragma once
using namespace cv;
using namespace std;


// DIALOG_COLOR_TO_BINARY 对话框

class DIALOG_COLOR_TO_BINARY : public CDialogEx
{
	DECLARE_DYNAMIC(DIALOG_COLOR_TO_BINARY)

public:
	DIALOG_COLOR_TO_BINARY(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DIALOG_COLOR_TO_BINARY();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLOR_TO_BINARY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();//初始化
	DECLARE_MESSAGE_MAP()
private:
	CImage m_CImage;
	Mat srcImage;
public:
	bool Flag=false;//开启分割的开关
	bool FlagCancel=true;//是否点击撤销
	bool FlagDraw = false;//是否画矩形框
	/*bool IsFinished = false;*/ //没有用
	int FirstTime = 3;//控制返回的背景画刷
	CStatic Img_Static;
	void SendImage (Mat img);
	const int PicHeight = 400;
	const int PicWidth = 400;
	int GetPicHeight ();
	int GetPicWidth ();
	void DrawPic (Mat tempImage);
	bool ImageCopy ( const CImage &srcImage, CImage &destImage ) ;//未使用
	POINT start; //按下点
	POINT end; //抬起点
	Rect g_rectangle;
	void OnView();
	/*bool GetIsFinished();*/
	int GetThresholdValue();
	afx_msg void OnBnClickedButton1 ();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	// 窗口显示的分割阈值
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	int ThresholdValue;
	CString CursorPos;//当前点坐标
	CString CursorPosDown;//按下点坐标
	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString ThresholdValueView;
};
