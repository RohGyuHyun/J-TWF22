
// IWBFrameView.h : CIWBFrameView Ŭ������ �������̽�
//

#pragma once


class CIWBFrameView : public CView
{
protected: // serialization������ ��������ϴ�.
	CIWBFrameView();
	DECLARE_DYNCREATE(CIWBFrameView)

// Ư���Դϴ�.
public:
	CIWBFrameDoc* GetDocument() const;

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
	virtual ~CIWBFrameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // IWBFrameView.cpp�� ����� ����
inline CIWBFrameDoc* CIWBFrameView::GetDocument() const
   { return reinterpret_cast<CIWBFrameDoc*>(m_pDocument); }
#endif

