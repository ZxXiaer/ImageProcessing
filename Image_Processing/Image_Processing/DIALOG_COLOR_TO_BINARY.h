#pragma once
using namespace cv;
using namespace std;


// DIALOG_COLOR_TO_BINARY �Ի���

class DIALOG_COLOR_TO_BINARY : public CDialogEx
{
	DECLARE_DYNAMIC(DIALOG_COLOR_TO_BINARY)

public:
	DIALOG_COLOR_TO_BINARY(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DIALOG_COLOR_TO_BINARY();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLOR_TO_BINARY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();//��ʼ��
	DECLARE_MESSAGE_MAP()
private:
	CImage m_CImage;
	Mat srcImage;
public:
	bool Flag=false;//�����ָ�Ŀ���
	bool FlagCancel=true;//�Ƿ�������
	bool FlagDraw = false;//�Ƿ񻭾��ο�
	/*bool IsFinished = false;*/ //û����
	int FirstTime = 3;//���Ʒ��صı�����ˢ
	CStatic Img_Static;
	void SendImage (Mat img);
	const int PicHeight = 400;
	const int PicWidth = 400;
	int GetPicHeight ();
	int GetPicWidth ();
	void DrawPic (Mat tempImage);
	bool ImageCopy ( const CImage &srcImage, CImage &destImage ) ;//δʹ��
	POINT start; //���µ�
	POINT end; //̧���
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
	// ������ʾ�ķָ���ֵ
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	int ThresholdValue;
	CString CursorPos;//��ǰ������
	CString CursorPosDown;//���µ�����
	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString ThresholdValueView;
};
