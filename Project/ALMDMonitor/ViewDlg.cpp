// ViewDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "ViewDlg.h"
#include "afxdialogex.h"
#include "PlotGage.h"
#include "ALMDMessages.h"
#include "Stabilization.h"
#include "Status.h"


// CViewDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CViewDlg, CDialogEx)

CViewDlg::CViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIEW, pParent)
{

}

CViewDlg::~CViewDlg()
{
}

void CViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewDlg, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(WM_UPDATE_STATINFO, &CViewDlg::OnUpdateStatusInfo)
END_MESSAGE_MAP()


// CViewDlg �޽��� ó�����Դϴ�.


BOOL CViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CViewDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
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
	
	BOOL bIsStable = CStatus::Instance()->GetCurMeasureData()->bStablization;
	if (!bIsStable)
	{
		myBrush.CreateSolidBrush(RGB(228, 228, 228));
		pOldBrush = MemDC.SelectObject(&myBrush);
	}

	m_Gage.SetDC(&MemDC);
	m_Gage.SetMode(CPlotGage::PLOT_DUAL);
//	m_Gage.SetMode(CPlotGage::PLOT_SINGLE);
	m_Gage.SetRect(rectClient);
	m_Gage.Calc();
	m_Gage.DrawBG();
	m_Gage.DrawBubble();
	m_Gage.Draw();
	pDC->BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap);
	if (!bIsStable)
	{
		MemDC.SelectObject(pOldBrush);
		myBrush.DeleteObject();
	}
	MemDC.DeleteDC();
	ReleaseDC(pDC);
}


BOOL CViewDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


LRESULT CViewDlg::OnUpdateStatusInfo(WPARAM wParam, LPARAM lParam)
{
	Invalidate(FALSE);
	return 0;
}
