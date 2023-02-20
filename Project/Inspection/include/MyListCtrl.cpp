#pragma once
#include "pch.h"
#include "MyListCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl

CMyListCtrl::CMyListCtrl()
{
	m_iControlID = -1;
	m_dlgNum = new CGetNumDlg();
	m_dlgNum->Create(IDD_DIALOG_NUMBER_BOX, this);
	m_bInputNum = FALSE;
	m_dlgNum->GetClientRect(m_NumDlgRect);
	m_NumDlgRect.bottom += 30;
	m_NumDlgRect.right += 10;
	m_bDBClicNumDlg = FALSE;
	m_bEnter = FALSE;
	m_bEditDbClick = FALSE;
}

CMyListCtrl::~CMyListCtrl()
{
	delete m_dlgNum;
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl message handlers
void CMyListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_ctrEidt.DestroyWindow();

	CListCtrl::OnLButtonDown(nFlags, point);
}

void CMyListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(m_bEditDbClick)
	{
		m_ctrEidt.DestroyWindow();
		POSITION pos = GetFirstSelectedItemPosition();    // ������ ��ġ�� �˾Ƴ�.
		if( pos != NULL)                // ���� ���� ���ý�
		{
			LVHITTESTINFO lvhti;
			lvhti.pt = point;
			SubItemHitTest(&lvhti);
			if (lvhti.flags & LVHT_ONITEMLABEL)
			{
				m_nItem = lvhti.iItem;        // ������ ��ġ�� ã�Ƴ�.
				m_nSubItem = lvhti.iSubItem;     // �÷� ��ġ�� ã�Ƴ�.    
				if(m_nSubItem > 0)
					CreateEditBox();        // ���� �����϶� ����Ʈ �ڽ� ����
			}
		}
	}
	
	CListCtrl::OnLButtonDblClk(nFlags, point);
}


void CMyListCtrl::CreateEditBox()
{
	m_dlgNum->ShowWindow(SW_HIDE);
	CRect rect, rect1, dlgNumRect;
	CString strGetText=_T("");	
	GetSubItemRect(m_nItem, m_nSubItem, LVIR_LABEL, rect);   // Ŭ���� ���� �׵θ� ���� rect�� ����	
	InvalidateRect(&rect);
	
	m_ctrEidt.Create(ES_LEFT | ES_AUTOHSCROLL | WS_BORDER | WS_CHILD , rect, this, 0); // ���� rect ������ ����Ʈ �ڽ� ����
	
	strGetText = GetItemText(m_nItem, m_nSubItem);       //������ ������
	
	m_ctrEidt.MoveWindow(rect);
	m_ctrEidt.SetWindowText(strGetText);	
	
	m_ctrEidt.Invalidate();	
	m_ctrEidt.ShowWindow(TRUE);	
	m_ctrEidt.SetSel(0,-1);	
	m_ctrEidt.SetFocus(); // ��Ŀ�� �ֱ�

	if(m_bDBClicNumDlg)
	{
		m_dlgNum->SetstrNum(strGetText);
		m_dlgNum->GetClientRect(rect1);
		this->GetWindowRect(rect1);

		dlgNumRect.left = rect1.left + rect.right;
		dlgNumRect.top = rect1.top + rect.top;
		dlgNumRect.right = dlgNumRect.left + m_NumDlgRect.Width();
		dlgNumRect.bottom = dlgNumRect.top + m_NumDlgRect.Height();

		if(dlgNumRect.right > m_MainWindowRect.right)
		{
			dlgNumRect.left = rect1.left + rect.left - m_NumDlgRect.Width();
		}

		if(dlgNumRect.bottom > m_MainWindowRect.bottom)
		{
			dlgNumRect.top = rect1.top + rect.bottom - m_NumDlgRect.Height();
		}

		dlgNumRect.right = dlgNumRect.left + m_NumDlgRect.Width();
		dlgNumRect.bottom = dlgNumRect.top + m_NumDlgRect.Height();

		m_dlgNum->MoveWindow(dlgNumRect.left, dlgNumRect.top, m_NumDlgRect.Width(), m_NumDlgRect.Height());
		//if(dlg->DoModal() != IDOK) return;
		m_dlgNum->ShowWindow(SW_SHOW);
		m_bInputNum = TRUE;
	}
}


void CMyListCtrl::EndModify()
{
	if(m_ctrEidt.m_hWnd != NULL)
	{
		CString sGetText;
		if(m_bDBClicNumDlg)
		{
			if(m_bEnter)
			{
				m_bEnter = FALSE;
				m_ctrEidt.GetWindowText(sGetText);
			}
			else
			{
				sGetText.Format(_T("%s"), m_dlgNum->GetstrNum());
			}

			if(m_dlgNum->IsWindowVisible())
			{
				m_dlgNum->ShowWindow(SW_HIDE);
			}
			
			m_ctrEidt.SetWindowTextW(sGetText);
		}
		m_ctrEidt.GetWindowText(sGetText); //������ ������ ������
		if(sGetText == _T(""))	// 150428 khyoo
		{
			sGetText = GetItemText(m_nItem, m_nSubItem); 
			if(sGetText == _T(""))
				return;
		}

		SetItemText(m_nItem, m_nSubItem, sGetText);
		m_ctrEidt.DestroyWindow();
	}
	Invalidate(TRUE);
	UpdateData(FALSE);
	m_bInputNum = FALSE;
	return;
}

BOOL CMyListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN &&  pMsg->wParam ==VK_RETURN)
    {
		m_bEnter = TRUE;
		POSITION pos = GetFirstSelectedItemPosition();    // ������ ��ġ�� �˾Ƴ�.
		if(pos == NULL) {
			return TRUE;
		}
		EndModify();
		UpdateData(FALSE);	
		return TRUE;
	}	

	//if(pMsg->wParam == VK_DELETE) {
	//	DeleteData();
	//}
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CMyListCtrl::SetControlID(int nID)
{
	m_iControlID = nID;
}

void CMyListCtrl::DeleteData()
{
	int nCount = GetSelectedCount();
	int nItem = -1;
	for(int i = 0 ; i < nCount ; i++) {
		nItem = GetNextItem(nItem, LVNI_SELECTED);
		if (nItem == -1)
			break;
		DeleteItem(nItem);
		nItem = -1;
	}
	ResetIndex();
}

void CMyListCtrl::ResetIndex()
{
	int iTotal = GetItemCount();
	CString sIndex;
	for(int i = 0; i < iTotal; i++) {
		sIndex.Format(_T("%d"), i + 1);
		SetItemText(i, 0, sIndex);
	}
}
