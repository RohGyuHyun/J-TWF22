
// IWBFrameView.cpp : CIWBFrameView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "IWBFrame.h"
#endif

#include "IWBFrameDoc.h"
#include "IWBFrameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIWBFrameView

IMPLEMENT_DYNCREATE(CIWBFrameView, CView)

BEGIN_MESSAGE_MAP(CIWBFrameView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CIWBFrameView ����/�Ҹ�

CIWBFrameView::CIWBFrameView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CIWBFrameView::~CIWBFrameView()
{
}

BOOL CIWBFrameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CIWBFrameView �׸���

void CIWBFrameView::OnDraw(CDC* /*pDC*/)
{
	CIWBFrameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CIWBFrameView �μ�

BOOL CIWBFrameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CIWBFrameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CIWBFrameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CIWBFrameView ����

#ifdef _DEBUG
void CIWBFrameView::AssertValid() const
{
	CView::AssertValid();
}

void CIWBFrameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIWBFrameDoc* CIWBFrameView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIWBFrameDoc)));
	return (CIWBFrameDoc*)m_pDocument;
}
#endif //_DEBUG


// CIWBFrameView �޽��� ó����
