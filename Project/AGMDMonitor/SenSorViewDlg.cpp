// SenSorViewDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AGMDMonitor.h"
#include "SenSorViewDlg.h"
#include "afxdialogex.h"


// CSenSorViewDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSenSorViewDlg, CDialogEx)

CSenSorViewDlg::CSenSorViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SENSORVIEWDLG, pParent)
{
	CStatus::Instance()->SetSensorViewDlg(this);
}

CSenSorViewDlg::~CSenSorViewDlg()
{
}

void CSenSorViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSenSorViewDlg, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(WM_UIUPDATE, &CSenSorViewDlg::OnUpdateSensorView)
END_MESSAGE_MAP()


// CSenSorViewDlg �޽��� ó�����Դϴ�.


void CSenSorViewDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	if (CStatus::Instance()->m_bReEnter)
		return;
	CStatus::Instance()->m_bReEnter = TRUE;
	m_pDC = GetDC();
	//CDC MemDC;
	CRect rectClient;
	GetClientRect(&rectClient);

	//	CBitmap * pOldBitmap;
	CBitmap bmp;
	CBrush backBrush(GetSysColor(COLOR_3DFACE));

	m_MemDC.CreateCompatibleDC(m_pDC);
	bmp.CreateCompatibleBitmap(m_pDC, rectClient.Width(), rectClient.Height());
	m_pOldBitmap = (CBitmap *)m_MemDC.SelectObject(&bmp);
	CBrush* pOldBrush = m_MemDC.SelectObject(&backBrush);
	m_MemDC.PatBlt(0, 0, rectClient.Width(), rectClient.Height(), WHITENESS);
	//
	m_drawHelper.SetDC(&m_MemDC);
	m_drawHelper.DrawingInitialize(rectClient);
	m_drawHelper.DrawingGap();
	//
	m_pDC->BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &m_MemDC, 0, 0, SRCCOPY);

	m_MemDC.SelectObject(m_pOldBitmap);

	m_MemDC.DeleteDC();
	ReleaseDC(m_pDC);

	CStatus::Instance()->m_bReEnter = FALSE;
}
LRESULT CSenSorViewDlg::OnUpdateSensorView(WPARAM wParam, LPARAM lParam)
{
	
	Invalidate(FALSE);
	return 0;
}

BOOL CSenSorViewDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN))
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
