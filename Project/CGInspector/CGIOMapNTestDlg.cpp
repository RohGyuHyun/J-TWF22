// CGIOMapNTestDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CGInspector.h"
#include "CGIOMapNTestDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "CGInspIO.h"


// CCGIOMapNTestDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCGIOMapNTestDlg, CDialogEx)

CCGIOMapNTestDlg::CCGIOMapNTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CGIOMAPNTEST, pParent)
{

}

CCGIOMapNTestDlg::~CCGIOMapNTestDlg()
{
}

void CCGIOMapNTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cCBInSensor);
	DDX_Control(pDX, IDC_COMBO2, m_cCBOutSensor);
}


BEGIN_MESSAGE_MAP(CCGIOMapNTestDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCGIOMapNTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCGIOMapNTestDlg �޽��� ó�����Դϴ�.


BOOL CCGIOMapNTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CString strNum;
	for (int i = 0; i < 10; i++)
	{
		strNum.Format(_T("%2d"), i);
		m_cCBInSensor.AddString(strNum);
		m_cCBOutSensor.AddString(strNum);
	}

	m_cCBInSensor.SetCurSel(CCGInspIO::Instance()->m_nConveyor.InSensor);
	m_cCBOutSensor.SetCurSel(CCGInspIO::Instance()->m_nConveyor.OutSensor);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CCGIOMapNTestDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CCGInspIO::Instance()->m_nConveyor.InSensor = m_cCBInSensor.GetCurSel();
	CCGInspIO::Instance()->m_nConveyor.OutSensor = m_cCBOutSensor.GetCurSel();
	CCGInspIO::Instance()->SavePortNo();

	CDialogEx::OnOK();
}
