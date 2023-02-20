// GraphDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "XFMS.h"
#include "GraphDlg.h"
#include "afxdialogex.h"


// CGraphDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CGraphDlg, CDialogEx)

CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GRAPH, pParent)
{

}

CGraphDlg::~CGraphDlg()
{
}

void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGraphDlg �޽��� ó�����Դϴ�.

void CGraphDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CDC* pDC = GetDC();
	CRect rectClient;
	GetClientRect(&rectClient);
	CDC MemDC;
	CBitmap * pOldBitmap;
	CBitmap bmp;
	MemDC.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rectClient.Width(), rectClient.Height());
	pOldBitmap = (CBitmap *)MemDC.SelectObject(&bmp);
	MemDC.PatBlt(0, 0, rectClient.Width(), rectClient.Height(), WHITENESS);

	CBrush myBrush;
	CBrush *pOldBrush = NULL;

	BOOL bIsStable = FALSE;
	if (!bIsStable)
	{
		myBrush.CreateSolidBrush(RGB(228, 228, 228));
		pOldBrush = MemDC.SelectObject(&myBrush);
	}

	m_LiveChart.SetDC(&MemDC);
	m_LiveChart.SetRect(rectClient);
	m_LiveChart.Calc();
	m_LiveChart.DrawBG();
	m_LiveChart.Draw();
	pDC->BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &MemDC, 0, 0, SRCCOPY);

	if (!bIsStable)
	{
		MemDC.SelectObject(pOldBrush);
		myBrush.DeleteObject();
	}
	MemDC.DeleteDC();
	ReleaseDC(pDC);
				   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
}


int CGraphDlg::AddTempDataToDraw(double * pDblTemp)
{

	return 0;
}
