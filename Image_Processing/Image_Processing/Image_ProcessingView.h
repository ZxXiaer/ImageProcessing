
// Image_ProcessingView.h : CImage_ProcessingView ��Ľӿ�
//

#pragma once
#include "MyImage_.h"
#include "DIALOG_COLOR_TO_BINARY.h"
using namespace cv;

class CImage_ProcessingView : public CScrollView
{
protected: // �������л�����
	CImage_ProcessingView();
	DECLARE_DYNCREATE(CImage_ProcessingView)

// ����%
public:
	CImage_ProcessingDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	

// ʵ��
public:
	virtual ~CImage_ProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT Flag, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
private:
	/*DIALOG_COLOR_TO_BINARY dlg_color_to_binary;*/
public:
	MyImage_ m_Image; 
	CImage MattoCImage(Mat img);
	afx_msg void OnDrawline();
	afx_msg void OnFileSaveAs();
//	afx_msg void OnViewToolbar();
	afx_msg void Onimgdown2();
	afx_msg void Onimgdown4();
	afx_msg void Onimgdiffrence();
	afx_msg void Onchangetogray();
	afx_msg void Ondenoisebyadd();
	afx_msg void OnFileOpenAll();
	afx_msg void Onmeansfilter();
	afx_msg void Onmedianfilter();
	afx_msg void OnHisView();
	afx_msg void OnHisequalization();
	afx_msg void OnHisnormalized();
	afx_msg void OnGrayLevelDown();
	afx_msg void OnFft2d();
	afx_msg void OnTraditionalFilter();
	afx_msg void OnShowred ();
	afx_msg void OnShowgreen ();
	afx_msg void OnShowblue ();
	afx_msg void OnRgbToHsi ();
	afx_msg void OnRgbEqualization ();
	afx_msg void OnColorEqualization ();
	afx_msg void OnColorSegToBinary ();


	afx_msg void OnColorSegToBinaryMfc();
	afx_msg void OnBasGlobalThresholdSeg();
	afx_msg void OnExtractHolesAndCracks();
};

#ifndef _DEBUG  // Image_ProcessingView.cpp �еĵ��԰汾
inline CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const
   { return reinterpret_cast<CImage_ProcessingDoc*>(m_pDocument); }
#endif

