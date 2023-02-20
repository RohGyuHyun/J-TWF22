// DialogRect.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"

#include "DialogRect.h"





// CDialogRect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogRect, CDialog)

CDialogRect::CDialogRect(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogRect::IDD, pParent)
{
	m_iDlgIdx = 0;
	m_dZoomRatio = 0.;
	m_dDefaultZoomRatio = 0.;
	m_iImgWd = 0;
	m_iImgHt = 0;

	m_iDlgIdx = 0;

	m_iTrackerNum = 0;

	m_lPanSX = 0;
	m_lPanSY = 0;
	m_lPanEX = 0;
	m_lPanEY = 0;
	m_bPanDN = FALSE;
	
	m_dPX = 0.;
	m_dPY = 0.;

	m_dZoomRat = 0.;
	m_dZoomRatOld = 0.;
	m_dZoomSX = 0.;
	m_dZoomSY = 0.;

	m_bActive = FALSE;
	m_iActiveIdx = -1;

	m_bMainDlg = FALSE;

	m_curColor = RGB(0, 255, 0);
	m_MainHwnd = NULL;
	m_MessageID = NULL;
	m_bDbClickUse = TRUE;
}

CDialogRect::~CDialogRect()
{
}

void CDialogRect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogRect, CDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_MESSAGE(USER_MSG_IMG_RECT, OnImageRectMessage)
	ON_WM_CONTEXTMENU()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDialogRect 메시지 처리기입니다.

BOOL CDialogRect::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogRect::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int i, tracker_num;
	C_CtrlTracker* tracker;
	long s_x, s_y, e_x, e_y;
	double r_l, r_t, r_r, r_b;

	tracker_num = (int)m_Tracker.GetCount();
	for (i = 0; (i < tracker_num) && m_bActive; i++)
	{
		POSITION pos = m_Tracker.FindIndex(i);
		if (NULL != pos)
		{
			tracker = &m_Tracker.GetAt(pos);
			switch (tracker->GetType())
			{
			case 0:
			case 1:


				//Limit
				tracker->GetRect(&r_l, &r_t, &r_r, &r_b);
				tracker->GetZoom(&s_x, &s_y, &e_x, &e_y);
				if (m_iImgWd < (s_x + r_r * ((m_lPanEX - m_lPanSX) / (double)m_WinRect.Width())))
				{
					r_r = ((double)(m_iImgWd - s_x)) / ((m_lPanEX - m_lPanSX) / (double)m_WinRect.Width()) - 2;
				}
				tracker->SetRect(r_l, r_t, r_r, r_b);

				tracker->OnMouseMove(nFlags, point);
				m_Tracker.SetAt(pos, *tracker);
				break;
			default:
				AfxMessageBox(_T("Error : Invalid index!"));
				break;
			}
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CDialogRect::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	int i, tracker_num;
	C_CtrlTracker* tracker;

	//if (m_bUseTracker)
	{
		//검사용
		tracker_num = (int)m_Tracker.GetCount();
		for (i = 0; (i < tracker_num) && m_bActive; i++)
		{
			POSITION pos = m_Tracker.FindIndex(i);
			if (NULL != pos)
			{
				tracker = &m_Tracker.GetAt(pos);
				switch (tracker->GetType())
				{
				case 0:
				case 1:
					tracker->OnLButtonDown(nFlags, point);
					m_Tracker.SetAt(pos, *tracker);
					break;
				default:
					AfxMessageBox(_T("Error : Invalid index!"));
					break;
				}
			}
		}
	}


	CDialog::OnLButtonDown(nFlags, point);
}

void CDialogRect::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int i, tracker_num;
	C_CtrlTracker* tracker;
	int nTrackerIdx = 0;

	//if (m_bUseTracker)
	{
		//검사용
		tracker_num = (int)m_Tracker.GetCount();
		for (i = 0; (i < tracker_num) && m_bActive; i++)
		{
			POSITION pos = m_Tracker.FindIndex(i);
			if (NULL != pos)
			{
				tracker = &m_Tracker.GetAt(pos);
				switch (tracker->GetType())
				{
				case 0:
				case 1:
					tracker->OnLButtonUp(nFlags, point);
					m_Tracker.SetAt(pos, *tracker);
					if (tracker->GetEnable())
						nTrackerIdx = i;
					break;
				default:
					AfxMessageBox(_T("Error : invalid index!"));
					break;
				}
			}
		}
	}

	Invalidate();
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CDialogRect::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!m_bDbClickUse)
	{
		CDialog::OnLButtonDblClk(nFlags, point);
	}
	int i, tracker_num;
	C_CtrlTracker* tracker;
	CRect rect;
	BOOL check = FALSE;

	//20080803
	if (m_bActive)
	{
		m_iActiveIdx = -1;
	}

	//검사용
	tracker_num = (int)m_Tracker.GetCount();
	for (i = 0; (i < tracker_num) && m_bActive; i++)
	{

		POSITION pos = m_Tracker.FindIndex(i);
		if (NULL != pos)
		{
			tracker = &m_Tracker.GetAt(pos);
			switch (tracker->GetType())
			{
			case 0:
			case 1:
				//if (((!check) && tracker->OnLButtonDblClk(nFlags, point)) || (tracker->GetGroup()))
				if(!tracker->GetEnable())
				{
					check = TRUE;
					m_iActiveIdx = i;
					tracker->SetActive(TRUE);
					tracker->SetEnable(TRUE);
				}
				else
				{
					tracker->SetActive(FALSE);
					tracker->SetEnable(FALSE);
					::SendMessage(m_MainHwnd, m_MessageID, 0, 0);
				}
				m_Tracker.SetAt(pos, *tracker);
				break;
			default:
				AfxMessageBox(_T("Error : Invalid index!"));
				break;
			}
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CDialogRect::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 0:
		KillTimer(nIDEvent);
		//Tracker
		for(int i = 0; i < TRACKER_MAX_NUM; i++)
		{
			//m_Tracker[i].Draw(i);
		}
		break;
	}


	CDialog::OnTimer(nIDEvent);
}

void CDialogRect::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	int i, tracker_num;
	C_CtrlTracker* tracker;

	//검사용
	tracker_num = (int)m_Tracker.GetCount();
	for (i = 0; (i < tracker_num); i++)
	{
		POSITION pos = m_Tracker.FindIndex(i);
		if (NULL != pos)
		{
			tracker = &m_Tracker.GetAt(pos);
			switch (tracker->GetType())
			{
			case 0:
			case 1:
				if(tracker->GetActive())
					tracker->Draw(i);
				break;
			default:
				AfxMessageBox(_T("Error : Invalid index!"));
				break;
			}
		}
	}

	if(m_bMainDlg)
		::SendMessage(m_MainHwnd, m_MessageID, 999, 0);
}

int CDialogRect::GetEnable()
{


	return -1;
}

void CDialogRect::SetEnable(int idx, BOOL enable)
{

}

void CDialogRect::SetTrackerNum(int i)
{
	m_iTrackerNum = i;
}

void CDialogRect::SetDlgIdx(int idx)
{
	m_iDlgIdx = idx;
}

double CDialogRect::GetZoom()
{
	return m_dZoomRatio;
}

void CDialogRect::SetZoom(double ratio, long img_wd, long img_ht)
{
	m_iImgWd = img_wd;
	m_iImgHt = img_ht;
	m_dZoomRatio = ratio;	
}

void CDialogRect::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//for(int i = 0; i < TRACKER_MAX_NUM; i++)
	//{
	//	m_Tracker[i] = C_CtrlTracker(this, 0, 4, 2);
	//	m_Tracker[i].SetColor(RGB(0, 255, 0));
	//	//m_Tracker[i].SetZoom(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT);
	//	m_Tracker[i].SetZoom(0, 0, m_iImgWd, m_iImgHt);
	//	m_Tracker[i].SetRect(100, 100, 200, 200);
	//	m_Tracker[i].SetCaption(" ");
	//	m_Tracker[i].SetActive(FALSE);
	//}
	CRect rect;
	GetClientRect(&rect);

	m_dDefaultZoomRatio = (double)rect.Height() / m_iImgHt;
	m_dDefaultZoomRatio = (1000000 * m_dDefaultZoomRatio) / 1000000.;


	if(bShow)
		Invalidate();
}


HWND CDialogRect::GetWinHwnd()
{

	return this->m_hWnd;
}

void CDialogRect::GetTracker(int idx, double *left, double *top, double *right, double *bottom)
{
	C_CtrlTracker tracker;
	double rect_l, rect_t, rect_r, rect_b;

	if (!m_Tracker.IsEmpty())
	{
		POSITION pos = m_Tracker.FindIndex(idx);
		if (NULL != pos)
		{
			tracker = m_Tracker.GetAt(pos);
			switch (tracker.GetType())
			{
			case 0:
			case 1:
				tracker.GetRect(&rect_l, &rect_t, &rect_r, &rect_b);

				/*
				rect_l = (rect_l - m_lZoomStartX) / ((double)m_WinRect.Width()/(m_lZoomEndX - m_lZoomStartX));
				rect_t = (rect_t - m_lZoomStartY) / ((double)m_WinRect.Height()/(m_lZoomEndY - m_lZoomStartY));

				rect_r = (rect_r - m_lZoomStartX) / ((double)m_WinRect.Width()/(m_lZoomEndX - m_lZoomStartX));
				rect_b = (rect_b - m_lZoomStartY) / ((double)m_WinRect.Height()/(m_lZoomEndY - m_lZoomStartY));
				*/

				//2090924
				rect_l = m_lPanSX + (((double)rect_l) * ((m_lPanEX - m_lPanSX) / (double)m_WinRect.Width()));
				rect_t = m_lPanSY + (((double)rect_t) * ((m_lPanEY - m_lPanSY) / (double)m_WinRect.Height()));
				rect_r = m_lPanSX + (((double)rect_r) * ((m_lPanEX - m_lPanSX) / (double)m_WinRect.Width()));
				rect_b = m_lPanSY + (((double)rect_b) * ((m_lPanEY - m_lPanSY) / (double)m_WinRect.Height()));


				*left = rect_l + 0.5;
				*top = rect_t + 0.5;
				*right = rect_r + 0.5;
				*bottom = rect_b + 0.5;

				//*rect = CRect(rect_l, rect_t, rect_r, rect_b);
				break;
			default:
				AfxMessageBox(_T("Error : invalid index!"));
				break;
			}

			return;// TRUE;
		}
	}

	return;// FALSE;
}

void CDialogRect::SetTracker(int idx, double left, double top, double right, double bottom)
{
	C_CtrlTracker tracker;

	double rect_l, rect_t, rect_r, rect_b;

	if (!m_Tracker.IsEmpty())
	{
		POSITION pos = m_Tracker.FindIndex(idx);
		if (NULL != pos)
		{
			tracker = m_Tracker.GetAt(pos);
			switch (tracker.GetType())
			{
			case 0:
			case 1:
				rect_l = left;
				rect_t = top;
				rect_r = right;
				rect_b = bottom;

				rect_l = (rect_l - m_lPanSX) * m_dZoomRatio;// ((double)m_WinRect.Width() / (m_lPanEX - m_lPanSX));
				rect_t = (rect_t - m_lPanSY) * m_dZoomRatio;//((double)m_WinRect.Height() / (m_lPanEY - m_lPanSY));

				rect_r = (rect_r - m_lPanSX) * m_dZoomRatio;//((double)m_WinRect.Width() / (m_lPanEX - m_lPanSX));
				rect_b = (rect_b - m_lPanSY) * m_dZoomRatio;//((double)m_WinRect.Height() / (m_lPanEY - m_lPanSY));

				tracker.SetRect(rect_l, rect_t, rect_r, rect_b);
				tracker.SetZoom(m_lPanSX, m_lPanSY, m_lPanEX, m_lPanEY);

				idx = (int)m_Tracker.GetCount();
				//m_Tracker.AddTail(tracker);
				m_Tracker.SetAt(pos, tracker);
				break;
			default:
				AfxMessageBox(_T("Error : invalid index!"));
				break;
			}

			return;// TRUE;
		}
	}
}

void CDialogRect::SetActiveTracker(int idx, BOOL active)
{
	C_CtrlTracker tracker;

	if (!m_Tracker.IsEmpty())
	{
		POSITION pos = m_Tracker.FindIndex(idx);
		if (NULL != pos)
		{
			tracker = m_Tracker.GetAt(pos);
			tracker.SetActive(active);
		}
	}
}

BOOL CDialogRect::CheckTracker(int idx, CPoint point)
{
	/*CRect rect, win_rect;
	POINT pnt;

	rect = m_Tracker[idx].GetRect();

	GetClientRect(&win_rect);
	win_rect.left = win_rect.left + 10;
	win_rect.top = win_rect.top + 10;
	win_rect.right = win_rect.right - 10;
	win_rect.bottom = win_rect.bottom - 10;

	if(!win_rect.PtInRect(point))
	{
		return FALSE;
	}

	pnt.x = rect.left;
	pnt.y = rect.top;
	if(!win_rect.PtInRect(pnt))
	{
		return FALSE;
	}

	pnt.x = rect.right;
	pnt.y = rect.bottom;
	if(!win_rect.PtInRect(pnt))
	{
		return FALSE;
	}*/

	return TRUE;
}

void CDialogRect::DeleteAll()
{
	if (m_bActive && (!m_Tracker.IsEmpty()))
	{
		m_Tracker.RemoveAll();
		m_iActiveIdx = -1;
	}
}

void CDialogRect::AddTracker(int type, CRect rect, COLORREF color, char* cap)
{

	C_CtrlTracker tracker;
	int idx;
	double rect_l, rect_t, rect_r, rect_b;

	if (m_bActive)
	{
		m_curColor = color;
		switch (type)
		{
		case 0:
		case 1:
			tracker = C_CtrlTracker(this, type, 4, 2);
			tracker.SetColor(m_curColor);

			rect_l = rect.left;
			rect_t = rect.top;
			rect_r = rect.right;
			rect_b = rect.bottom;

			rect_l = (rect_l - m_lPanSX) * m_dZoomRatio;// ((double)m_WinRect.Width() / (m_lPanEX - m_lPanSX));
			rect_t = (rect_t - m_lPanSY) * m_dZoomRatio;// ((double)m_WinRect.Height() / (m_lPanEY - m_lPanSY));

			rect_r = (rect_r - m_lPanSX) * m_dZoomRatio;// ((double)m_WinRect.Width() / (m_lPanEX - m_lPanSX));
			rect_b = (rect_b - m_lPanSY) * m_dZoomRatio;// ((double)m_WinRect.Height() / (m_lPanEY - m_lPanSY));

			tracker.SetRect(rect_l, rect_t, rect_r, rect_b);
			tracker.SetZoom(m_lPanSX, m_lPanSY, m_lPanEX, m_lPanEY);
			tracker.SetCaption(cap);
			idx = (int)m_Tracker.GetCount();
			m_Tracker.AddTail(tracker);
			break;
		default:
			AfxMessageBox(_T("Error : invalid index!"));
			break;
		}

	}

}

LRESULT CDialogRect::OnImageRectMessage(WPARAM para0, LPARAM para1)
{

	////Tracker
	//for(int i = 0; i < TRACKER_MAX_NUM; i++)
	//{
	//	m_Tracker[i].Draw(i);
	//}

	////Tracker
	//SetTimer(0, 1, NULL);

	return 0;
}
void CDialogRect::OnContextMenu(CWnd* pWnd, CPoint point)
{

}


void CDialogRect::ZoomDisplay()
{
	
}


void CDialogRect::ZoomTracker(CRect *rect, double o_rat)
{

}	


//다이얼로그 배경색 변경
HBRUSH CDialogRect::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
		return (HBRUSH)GetStockObject(BLACK_BRUSH);
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}
#ifdef MIL_USE
void CDialogRect::SetPointGrayValueDraw(CMilDisplay* milDisplay, CPoint pPoint, int nVal)
{
	CString strText;
	strText.Format(_T("Point X : %d, Point Y : %d, Gray Value : %d"), pPoint.x, pPoint.y, nVal);
	milDisplay->FontSize(M_FONT_DEFAULT_LARGE);
	milDisplay->ClearOverlay();
	milDisplay->OutputText(0, 1000, strText.GetBuffer());
}
#endif

BOOL CDialogRect::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
