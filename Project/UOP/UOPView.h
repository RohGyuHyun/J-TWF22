
// UOPView.h : CUOPView Ŭ������ �������̽�
//

#pragma once


class CUOPView : public CView
{
protected: // serialization������ ��������ϴ�.
	CUOPView();
	DECLARE_DYNCREATE(CUOPView)

// Ư���Դϴ�.
public:
	CUOPDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CUOPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // UOPView.cpp�� ����� ����
inline CUOPDoc* CUOPView::GetDocument() const
   { return reinterpret_cast<CUOPDoc*>(m_pDocument); }
#endif

