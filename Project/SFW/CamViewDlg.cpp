// CamViewDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SFW.h"
#include "CamViewDlg.h"
#include "afxdialogex.h"


// CCamViewDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCamViewDlg, CDialogEx)

CCamViewDlg::CCamViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCamViewDlg::IDD, pParent)
{

}

CCamViewDlg::~CCamViewDlg()
{
}

void CCamViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCAMSEL, m_tabCamSel);
}


BEGIN_MESSAGE_MAP(CCamViewDlg, CDialogEx)
END_MESSAGE_MAP()


// CCamViewDlg �޽��� ó�����Դϴ�.


BOOL CCamViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_tabCamSel.InsertItem(1, _T("Xray"));
	m_tabCamSel.InsertItem(2, _T("AOI"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
