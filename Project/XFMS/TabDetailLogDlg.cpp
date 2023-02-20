// TabDetailLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XFMS.h"
#include "TabDetailLogDlg.h"
#include <UniUtility.h>

static BOOL Settext=false;

COLORREF m_color;

#define COLOR_BLACK RGB( 0, 0, 0)
#define COLOR_BLUE RGB( 0, 0,128)
#define COLOR_GREEN RGB( 0,128, 0)
#define COLOR_CYAN RGB( 0,128,128)
#define COLOR_RED RGB(128, 0, 0)
#define COLOR_MAGENTA RGB(128, 0,128)
#define COLOR_BROWN RGB(128,128, 0)
#define COLOR_DARKGRAY RGB(128,128,128)
#define COLOR_LIGHTGRAY RGB(192,192,192)
#define COLOR_LIGHTBLUE RGB( 0, 0,255)
#define COLOR_LIGHTGREEN RGB( 0,255, 0)
#define COLOR_LIGHTCYAN RGB( 0,255,255)
#define COLOR_LIGHTRED RGB(255, 0, 0)
#define COLOR_LIGHTMAGENTA RGB(255, 0,255)
#define COLOR_YELLOW RGB(255,255, 0)
#define COLOR_WHITE RGB(255,255,255)


CTabDetailLogDlg* CTabDetailLogDlg::m_pInstance=NULL;
// CTabDetailLogDlg dialog

IMPLEMENT_DYNAMIC(CTabDetailLogDlg, CPropertyPage)

CTabDetailLogDlg::CTabDetailLogDlg()
	: CPropertyPage(CTabDetailLogDlg::IDD)

	, m_StringErr(_T(""))
{
}

CTabDetailLogDlg::~CTabDetailLogDlg()
{
	
}

void CTabDetailLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_BTNLOGTEXTCEAR, m_ClearBtn);

	DDX_Text(pDX, IDC_EDITLOG, m_StringErr);
	DDX_Control(pDX, IDC_EDITLOG, m_LogdataEditControl);
}


BEGIN_MESSAGE_MAP(CTabDetailLogDlg, CPropertyPage)
//	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTNLOGTEXTCEAR, &CTabDetailLogDlg::OnBnClickedTextLogClear)
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CTabDetailLogDlg message handlers


/** \brief ���⼭ Log â�� ���� Text ������ �����մϴ�.*/
//HBRUSH CTabDetailLogDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
//
//
//   
//			HBRUSH brush;
//			if (pWnd->GetDlgCtrlID()==IDC_EDITLOG)
//			{
//			brush=CreateSolidBrush( COLOR_BLACK); // ����Ʈâ �� ���
//			pDC->SetBkMode(TRANSPARENT);
//					 // �ؽ�Ʈ�� ������ ������� �Ѵ�. 
//			pDC->SetTextColor(m_color);
//			
//			hbr=brush;
//			}
//			else
//			{
//			//brush=CreateSolidBrush(COLOR_WHITE);
//			//hbr=brush; // Ŭ���̾�Ʈ â�� ���
//			}
//
//            //DeleteObject(brush);    // �귯�� ��ü�� �����Ѵ�.
//
//
// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
//	return hbr;
//}



/** \brief �ٸ� Ŭ�������� LOG â�� �̿��� �� �ֵ��� �ϴ� instance �Լ�*/
CTabDetailLogDlg * CTabDetailLogDlg::Instance(void)
{
	if(!m_pInstance)m_pInstance=new CTabDetailLogDlg; 

	return m_pInstance;
}



/* \brief �α�â�� TEXT �� ��� �ϱ� ���� �Լ�
   ������ ���������� �ڵ����� �����ٷ� ���� �ϵ��� �Ͽ���.

   �߰��Ǵ� Text ������ ī�����Ͽ� 200 �̵Ǹ� �ڵ����� Clear �Ǿ� ����۵ǵ��� �Ͽ���.

   \param CString Text  Log â�� ������ CString
*/
int len=0; // �������ؽ�Ʈâ (�޸𸮴þ�⶧����)
void CTabDetailLogDlg::SetTextLog(CString Text,COLORREF RGBcolor,BOOL ERR)

{   
	//m_color=RGBcolor;
	
	if(ERR)
	{
	   ///////////////////LOG �����ϱ� ////////////////////////

		CString strDir;
		strDir.Format(_T("..\\Setup\\LOGDATA.txt"));
		
		CFile* pcFile = new CFile(strDir,CFile::modeCreate | CFile::modeWrite);
		if(pcFile == NULL)
		{}
		else{
				CStringA strTemp, strBuff;
				strBuff = _T("LogData:\r\n");
				char buff[100000] = {NULL,};

				strBuff +=_T("\r\n");        

				strTemp.Format("%S",m_StringErr);  
				strBuff +=  strTemp; 

				sprintf_s(buff, strBuff.GetLength()+1, "%s", strBuff);
				pcFile->Write(buff,(UINT)strlen(buff));
				pcFile->Close();
			}
			delete pcFile;
   
	
	}

	else if(len>500)                          ///500�� �̻��̸� �ڵ� Ŭ����
	{
     ///////////////////LOG �����ϱ� ////////////////////////
/*
    CString strDir;
	strDir.Format(_T("..\\Setup\\LOGDATA.txt"));
	
	CFile* pcFile = new CFile(strDir,CFile::modeCreate | CFile::modeWrite);
	if(pcFile == NULL)
	{}
	else{
		CStringA strTemp, strBuff;
		strBuff = _T("LogData:\r\n");
		char buff[100000] = {NULL,};

		strBuff +=_T("\r\n");;       // ��� Laminography

		strTemp.Format("%S",m_StringErr);  // Last Angle
		strBuff +=  strTemp; 

		sprintf_s(buff, strBuff.GetLength()+1, "%s", strBuff);
		pcFile->Write(buff,(UINT)strlen(buff));
		pcFile->Close();
	}
	delete pcFile;
*/

  ///////////////////////////////////////////
	m_StringErr.Empty();
	Settext=true;
	len=0;
	}

	else
	{
     m_StringErr+=Text;
     m_StringErr+=_T("\r\n");
	 Settext=true;
     len=len+1;
	}
 
  
 }


BOOL CTabDetailLogDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
   	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	 
      brush1 = new CBrush(RGB(255,0,0));
   
	SetTimer(0,50,NULL);
  
	m_ClearBtn.SetIcon(IDI_CLEAR);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

/** \brief ���������� Log â�� ��µ� Text �����͸� �����Ѵ�.*/
void CTabDetailLogDlg::OnBnClickedTextLogClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
   
    m_StringErr.Empty();
	Settext=true;
	
}


BOOL CTabDetailLogDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return CPropertyPage::OnCommand(wParam, lParam);
}

void CTabDetailLogDlg::OnRButtonDown(UINT nFlags, CPoint point)
{

	CPropertyPage::OnRButtonDown(nFlags, point);
}

void CTabDetailLogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
    
	if(Settext)
	{
		UpdateData(false);
		m_LogdataEditControl.SetSel(-2, -1);
		Settext = false;
	}
	CPropertyPage::OnTimer(nIDEvent);
}

HBRUSH CTabDetailLogDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.


	switch (nCtlColor) 
	{
 
 		case CTLCOLOR_EDIT:
 		pDC->SetTextColor(RGB(255, 255, 255));
 		pDC->SetBkColor(RGB(0, 0, 0));
 		return 0;

		case CTLCOLOR_STATIC:
 		pDC->SetTextColor(RGB(255, 255, 255));
 		pDC->SetBkColor(RGB(0, 0, 0));
 	 	return 0;
     
		
		default:
 		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
 	}

      return (HBRUSH)(brush1->GetSafeHandle());

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
}

BOOL CTabDetailLogDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
 
	CRect winRC;
	GetClientRect(winRC);
	
	CBrush bkBrush(RGB(0, 0, 0));
	CBrush* pOldBrush = pDC->SelectObject(&bkBrush);
	pDC->Rectangle(winRC);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}
