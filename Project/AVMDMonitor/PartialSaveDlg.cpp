// PartialSaveDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "PartialSaveDlg.h"
#include "afxdialogex.h"
#include "Repository.h"
#include "Status.h"
#include "ProgressBarDlg.h"

// CPartialSaveDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPartialSaveDlg, CDialogEx)

int CPartialSaveDlg::m_iStartPartialNo = 0;
int CPartialSaveDlg::m_iEndPartialNo = 0;
CPartialSaveDlg::CPartialSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PARTIALSAVE, pParent)
	, m_nStep(0)
	//, m_iStartPartialNo(0)
	//, m_iEndPartialNo(0)
	, m_pRepository(NULL)
	, m_strLabelPartialSave(_T(""))
{

}

CPartialSaveDlg::~CPartialSaveDlg()
{
}

void CPartialSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LSELPOS, m_strLabelPartialSave);
}


BEGIN_MESSAGE_MAP(CPartialSaveDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CPartialSaveDlg::OnBnClickedBtnNext)
END_MESSAGE_MAP()


// CPartialSaveDlg �޽��� ó�����Դϴ�.


void CPartialSaveDlg::OnBnClickedBtnNext()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnStepNext();
}


int CPartialSaveDlg::OnStepNext()
{
	m_pRepository = CStatus::Instance()->GetRepository();
	switch(m_nStep)
	{
	case 0://Start Pos ����
		this->SetWindowTextW(_T("Set End Position"));
		m_iStartPartialNo = CStatus::Instance()->m_iStartPosText;
		m_strLabelPartialSave = _T("Move to the end position and push next button");
		UpdateData(FALSE);
		break;
	case 1://End Po ����
		m_strLabelPartialSave = _T("To set the partial Log File Name, push next button");
		UpdateData(FALSE);
		m_iEndPartialNo = CStatus::Instance()->m_iStartPosText;
		break;
	case 2:	//Save
	{
		CFileDialog dlg(FALSE);
		if (dlg.DoModal() == IDOK)
		{
			CStatus::Instance()->m_iPartialSaveCount = m_iEndPartialNo - m_iStartPartialNo;
			CStatus::Instance()->SetProgressBarMode(CStatus::tProgressBarMode::PRATICAL_SAVE);
			CStatus::Instance()->m_nProgressBarCount = 0;
			CProgressBarDlg *pProgressDlg = new CProgressBarDlg;
			CString strFilePath = dlg.GetPathName();
			AfxBeginThread(PartialSaveThread, &strFilePath);
			pProgressDlg->DoModal();
			//m_pRepository->ExportDataFile(dlg.GetPathName(), m_iStartPartialNo, m_iEndPartialNo);
		}
	}
	this->ShowWindow(SW_HIDE);
	break;
	case 3:
		break;
	}

	m_nStep++;
	m_nStep %= 4;
	return 0;
}


BOOL CPartialSaveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_strLabelPartialSave = _T("Move to the start position and push the next button");
	this->SetWindowTextW(_T("Set Start Position"));
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CPartialSaveDlg::Init()
{
	m_nStep = 0;
	m_strLabelPartialSave = _T("Move to the start position and push the next button");
	this->SetWindowTextW(_T("Set Start Position"));
	UpdateData(FALSE);
}

UINT CPartialSaveDlg::PartialSaveThread(LPVOID lParam)
{
	//m_pRepository->ExportDataFile(dlg.GetPathName(), m_iStartPartialNo, m_iEndPartialNo);
	CString *strFilePath = (CString *)lParam;
	CStatus::Instance()->GetRepository()->ExportDataFile(*strFilePath, m_iStartPartialNo, m_iEndPartialNo);
	CStatus::Instance()->m_bIsThreadStop = FALSE;
	return 0;
}