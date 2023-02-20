// ComSetDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "XNPI.h"
#include "ComSetDlg.h"
#include "afxdialogex.h"
#include "Status.h"

// CComSetDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CComSetDlg, CDialogEx)

CPtrList * CComSetDlg::m_pLstComsName = NULL;
CPtrList * CComSetDlg::m_pLstComsNo = NULL;

CComSetDlg::CComSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMSET, pParent)
	, m_iBaudRate(0)
	, m_strPortNo(_T(""))
{
	m_pLstComsName = new(CPtrList);
	m_pLstComsNo = new(CPtrList);
}

CComSetDlg::~CComSetDlg()
{
}

void CComSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBBAUDRATE, m_cBaudRate);
	DDX_CBIndex(pDX, IDC_CBBAUDRATE, m_iBaudRate);
	DDX_Control(pDX, IDC_CBPORTNO, m_cPortNo);
	DDX_CBString(pDX, IDC_CBPORTNO, m_strPortNo);
	DDX_Control(pDX, IDOK, m_cSave);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
}


BEGIN_MESSAGE_MAP(CComSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CComSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CComSetDlg �޽��� ó�����Դϴ�.


BOOL CComSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	int i = 0;

	GetComPortEnum();

	m_cBaudRate.AddString(_T("9600"));
	m_cBaudRate.AddString(_T("19200"));
	m_cBaudRate.AddString(_T("38400"));
	m_cBaudRate.AddString(_T("115200"));

	CString strComportNo = CStatus::Instance()->GetComportNo();
	if ((i = FindIndex(strComportNo.GetBuffer(), &m_cPortNo)) >= 0)
	{
		m_cPortNo.SetCurSel(i);
	}

	CString strBaudRate;
	strBaudRate.Format(_T("%d"), CStatus::Instance()->GetBaudRate());
	if ((i = FindIndex(strBaudRate.GetBuffer(), &m_cBaudRate)) >= 0)
	{
		m_cBaudRate.SetCurSel(i);
	}
	UpdateData(TRUE);

	ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

BOOL CALLBACK CComSetDlg::SerialEnum(LPVOID lpCallbackValue, LISTPORTS_PORTINFO* lpPortInfo)
{
	CString *pComString;
	pComString = new(CString);
	*(pComString) = lpPortInfo->lpPortName;
	m_pLstComsNo->AddTail(pComString);
	pComString = new(CString);
	*(pComString) = lpPortInfo->lpFriendlyName;
	m_pLstComsName->AddTail(pComString);
	return TRUE;
}

CString CComSetDlg::GetComPortEnum()
{
	POSITION pos;
	CString *pString;
	ListPorts(SerialEnum, NULL);
	Sleep(10);

	pos = m_pLstComsNo->GetHeadPosition();
	if (pos == NULL) return CString();
	int i = 0;
	do {
		pString = (CString *)m_pLstComsNo->GetAt(pos);
		m_cPortNo.AddString(*pString);
		m_pLstComsNo->GetNext(pos);

	} while (pos != NULL);

	return CString();
}

int CComSetDlg::FindIndex(TCHAR *szBaudRate, CComboBox * pComboBox)
{
	int i = pComboBox->FindString(0, szBaudRate);
	return i;
}


void CComSetDlg::OnBnClickedOk()
{
	CString strBaudRate;
	long iBaudRate;
	CString strComportNo;
	m_cBaudRate.GetLBText(m_cBaudRate.GetCurSel(), strBaudRate);
	m_cPortNo.GetLBText(m_cPortNo.GetCurSel(), strComportNo);

	iBaudRate = _wtoi(strBaudRate.GetBuffer());
	CStatus::Instance()->SetBaudRate(iBaudRate);
	CStatus::Instance()->SetComportNo(strComportNo.GetBuffer());
	CStatus::Instance()->SaveCFG();

	CDialogEx::OnOK();
}
