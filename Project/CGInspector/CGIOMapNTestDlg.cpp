// CGIOMapNTestDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CGInspector.h"
#include "CGIOMapNTestDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "CGInspIO.h"


// CCGIOMapNTestDlg 대화 상자입니다.

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


// CCGIOMapNTestDlg 메시지 처리기입니다.


BOOL CCGIOMapNTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CCGIOMapNTestDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CCGInspIO::Instance()->m_nConveyor.InSensor = m_cCBInSensor.GetCurSel();
	CCGInspIO::Instance()->m_nConveyor.OutSensor = m_cCBOutSensor.GetCurSel();
	CCGInspIO::Instance()->SavePortNo();

	CDialogEx::OnOK();
}
