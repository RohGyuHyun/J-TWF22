// AVMDFFTGraphDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "AVMDFFTGraphDlg.h"
#include "afxdialogex.h"
#include "DrawingHelper.h"
#include "Status.h"

// CAVMDFFTGraphDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAVMDFFTGraphDlg, CDialogEx)

CAVMDFFTGraphDlg::CAVMDFFTGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FFT_GRAPH, pParent)
	, m_strGraphType(_T(""))
	, m_nInputType(0)
{

}

CAVMDFFTGraphDlg::CAVMDFFTGraphDlg(CString strType, int nInputType /*=NULL*/)
	: m_strGraphType(_T(""))
{
	m_strGraphType = strType;
	m_nInputType = nInputType;
}

CAVMDFFTGraphDlg::~CAVMDFFTGraphDlg()
{
}

void CAVMDFFTGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAVMDFFTGraphDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CAVMDFFTGraphDlg �޽��� ó�����Դϴ�.

void CAVMDFFTGraphDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	
	if (!m_MemDC.m_hDC || !m_MemDC.m_hAttribDC)
		return;

	DrawView();
	ReleaseDC(m_pDC);
}

void CAVMDFFTGraphDlg::MakeGraph()
{
	if (m_strGraphType == "RMS")
		CStatus::Instance()->m_bFftRMSReEnter = TRUE;
	if (m_strGraphType == "X")
		CStatus::Instance()->m_bFftXReEnter = TRUE;
	if (m_strGraphType == "Y")
		CStatus::Instance()->m_bFftYReEnter = TRUE;
	if (m_strGraphType == "Z")
		CStatus::Instance()->m_bFftZReEnter = TRUE;

	m_bToRepaint = TRUE;
	CStatus::Instance()->m_iFFTShowStartPos = CStatus::Instance()->m_iFFTShowTempStartPos;
	m_pDC = GetDC();

	CBitmap bmp;
	CBrush backBrush(GetSysColor(COLOR_3DFACE));

	GetClientRect(m_ClientRect);
	m_OldClientRect = m_ClientRect;
	m_MemDC.CreateCompatibleDC(m_pDC);
	bmp.CreateCompatibleBitmap(m_pDC, m_ClientRect.Width(), m_ClientRect.Height());

	pOldBitmap = (CBitmap*)m_MemDC.SelectObject(&bmp);
	CBrush* pOldBrush = m_MemDC.SelectObject(&backBrush);

	m_MemDC.PatBlt(0, 0, m_ClientRect.Width(), m_ClientRect.Height(), PATCOPY);

	m_drawHelper.SetDC(&m_MemDC);
	m_drawHelper.InitFFTBackGround(m_ClientRect, m_strGraphType, m_nInputType);
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
	{
		int iError = 0;
		if (CStatus::Instance()->IsFilterUse())
		{
			iError = m_drawHelper.OnFilterFft2d(m_nInputType);
		}
		else
		{
			iError = m_drawHelper.OnFft2d(m_nInputType);
		}
		
		if(iError == 0)
			m_drawHelper.DrawFFTGraph(m_fftMeasurePos, m_nInputType,m_nInputType, FALSE);
	}
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		BOOL bIsOverlay = CStatus::Instance()->GetOverlayCondition();
		int iError = 0;
		if (m_nInputType == 0)
		{
			if (bIsOverlay)
			{
				if (CStatus::Instance()->IsFilterUse())
				{
					iError = m_drawHelper.OnReviewFilterFft2d(0);
					iError = m_drawHelper.OnReviewFilterFft2d(1);
				}
				else
				{
					iError = m_drawHelper.OnReviewFft2d(0);
					iError = m_drawHelper.OnReviewFft2d(1);
				}
				if (iError == 0)
				{
					m_drawHelper.DrawFFTGraph(m_fftMeasurePos, 1, 0, TRUE);
					m_drawHelper.DrawFFTGraph(m_fftMeasurePos, 0, 0, FALSE);
				}
			}
			else
			{
				if (CStatus::Instance()->IsFilterUse())
				{
					iError = m_drawHelper.OnReviewFilterFft2d(0);
				}
				else
				{
					iError = m_drawHelper.OnReviewFft2d(0);
				}
				if (iError == 0)
					m_drawHelper.DrawFFTGraph(m_fftMeasurePos, 0, 0, FALSE);			
			}
		}
		if (m_nInputType == 1)
		{
			if (CStatus::Instance()->IsFilterUse())
			{
				iError = m_drawHelper.OnReviewFilterFft2d(1);
			}
			else
			{
				iError = m_drawHelper.OnReviewFft2d(1);
			}
			if (iError == 0)
				m_drawHelper.DrawFFTGraph(m_fftMeasurePos, 1, 1, FALSE);
		}
	}
	Invalidate(FALSE);
}

void CAVMDFFTGraphDlg::DrawView()
{
	if (m_bToRepaint)
	{
		m_pDC->BitBlt(0, 0, m_ClientRect.Width(), m_ClientRect.Height(), &m_MemDC, 0, 0, SRCCOPY);
		if (m_strGraphType == "RMS")
			CStatus::Instance()->m_bFftRMSReEnter = FALSE;
		if (m_strGraphType == "X")
			CStatus::Instance()->m_bFftXReEnter = FALSE;
		if (m_strGraphType == "Y")
			CStatus::Instance()->m_bFftYReEnter = FALSE;
		if (m_strGraphType == "Z")
			CStatus::Instance()->m_bFftZReEnter = FALSE;
		if (m_strGraphType == "COMPARE")
			CStatus::Instance()->m_bCompareReEnter = FALSE;
	}
	//MemDC.SelectObject(pOldBrush);		//�ֶ��� ����
	if (m_bToRepaint)
	{
		if (!m_MemDC.m_hDC)
		{
			CStatus::Instance()->GetReEnter();
			m_bToRepaint = FALSE;
			return;
		}
		m_MemDC.SelectObject(pOldBitmap);
		m_MemDC.DeleteDC();
		m_MemDC.m_hDC = NULL;
		m_bToRepaint = FALSE;
	}

	CStatus::Instance()->GetReEnter();
}

void CAVMDFFTGraphDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_fftMeasurePos = point;
	CStatus::Instance()->UpdateScrn();
	CDialogEx::OnLButtonDown(nFlags, point);
}
