// SCButton.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SCButton.h"


// CSCButton
#define TIME_BLINK 124
IMPLEMENT_DYNAMIC(CSCButton, CButton)

CSCButton::CSCButton()
	: m_bStatHover(FALSE)
	, m_bBlink(FALSE)
	, m_iBlinkColor(FALSE)
{

}

CSCButton::~CSCButton()
{
}


BEGIN_MESSAGE_MAP(CSCButton, CButton)
	ON_WM_ERASEBKGND()
//	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
//	ON_NOTIFY_REFLECT(BCN_HOTITEMCHANGE, &CSCButton::OnBnHotItemChange)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CSCButton �޽��� ó�����Դϴ�.




BOOL CSCButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rectClient;
	GetClientRect(&rectClient);
	pDC->FillSolidRect(&rectClient, RGB(0, 255, 255));
	return CButton::OnEraseBkgnd(pDC);
//	return TRUE;
}

void CSCButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  ������ �׸��� �׸��� �ڵ带 �߰��մϴ�.
	CDC DC;
	RECT rect;
	DC.Attach(lpDrawItemStruct->hDC);   //  ��ư�� DC ���ϱ�
	GetClientRect(&rect);     // ��ư ���� ���ϱ�    
	DC.SetBkMode(TRANSPARENT); //��� ����
	DC.Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0)); // ��ư�� �ܰ��� �׸���

	UINT state = lpDrawItemStruct->itemState;  //  ��ư ���� ���ϱ�
	if ((state & ODS_SELECTED))
	{
		DC.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
	}
	else
	{
		DC.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
	}
	if (state & ODS_DISABLED)
	{
		DC.FillSolidRect(&rect, RGB(230, 230, 230));
		//		DC.SetBkColor(RGB(92, 126, 173)); // TEXT �� ��׶��� ����
		DC.SetTextColor(RGB(255, 255, 255));  // TEXT ����
	}
	else
	{
		if (m_bStatHover)
		{
			DC.FillSolidRect(&rect, RGB(92, 126, 173));
			//		DC.SetBkColor(RGB(92, 126, 173)); // TEXT �� ��׶��� ����
			DC.SetTextColor(RGB(255, 0, 0));  // TEXT ����
		}
		else
		{
			DC.FillSolidRect(&rect, RGB(90, 107, 135));
			//		DC.SetBkColor(RGB(0, 255, 0)); // TEXT �� ��׶��� ����
			DC.SetTextColor(RGB(255, 255, 255));  // TEXT ����
		}
	}

	CString strText;
	GetWindowText(strText);
	DC.DrawText(strText, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);// ��ư�� TEXT �ֱ�
	DC.Detach();  // Button DC Ǯ�� �ֱ�
}



void CSCButton::OnMouseLeave()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_bStatHover = FALSE;
	Invalidate();
	CButton::OnMouseLeave();
}


void CSCButton::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_bStatHover = TRUE;
	DRAWITEMSTRUCT drawItem;
	DrawItem(&drawItem);
	CButton::OnMouseHover(nFlags, point);
}

void CSCButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(m_bStatHover)
	{ 
	}else
	{
		m_bStatHover = TRUE;
		m_stTrackMouse.cbSize = sizeof(TRACKMOUSEEVENT);
		m_stTrackMouse.dwFlags = TME_LEAVE;
		m_stTrackMouse.dwHoverTime = HOVER_DEFAULT;
		m_stTrackMouse.hwndTrack = GetSafeHwnd();
		TrackMouseEvent(&m_stTrackMouse);
		Invalidate();
		//DRAWITEMSTRUCT drawItem;
		//DrawItem(&drawItem);
	}
	CButton::OnMouseMove(nFlags, point);
}


int CSCButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return 0;
}



void CSCButton::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (nIDEvent)
	{
	case TIME_BLINK:
		if(m_iBlinkColor)
		{
			BlinkText(m_iBlinkColor);
			m_iBlinkColor = 0;
		}
		else
		{
			BlinkText(m_iBlinkColor);
			m_iBlinkColor = 1;
		}
		break;
	}
	CButton::OnTimer(nIDEvent);
}


void CSCButton::SetBlink(bool bBlink)
{
	m_bBlink = bBlink;
	if (m_bBlink)
	{
		SetTimer(TIME_BLINK, 500, 0);
	}
	else
	{
		KillTimer(TIME_BLINK);
		BlinkText(0);
	}
}


void CSCButton::BlinkText(int iBlinkColor)
{
	CDC *pDC;
	RECT rect;
	pDC = GetDC();
//	DC.Attach(lpDrawItemStruct->hDC);   //  ��ư�� DC ���ϱ�
	GetClientRect(&rect);     // ��ư ���� ���ϱ�    
	pDC->SetBkMode(TRANSPARENT); //��� ����

	if (iBlinkColor)
	{
		pDC->FillSolidRect(&rect, RGB(92, 126, 173));
		pDC->SetTextColor(RGB(255, 0, 0));  // TEXT ����

	}
	else
	{
		pDC->FillSolidRect(&rect, RGB(90, 107, 135));
		pDC->SetTextColor(RGB(255, 255, 255));  // TEXT ����
	}
	CString strText;
	GetWindowText(strText);
	pDC->DrawText(strText, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);// ��ư�� TEXT �ֱ�
	ReleaseDC(pDC);
}


BOOL CSCButton::IsBlink()
{
	return m_bBlink;
}