// RenameModelData.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "XNPI.h"
#include "RenameModelDataDlg.h"

#include "OpenModelDataDlg.h"


// CRenameModelDataDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CRenameModelDataDlg, CDialog)

CRenameModelDataDlg::CRenameModelDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRenameModelDataDlg::IDD, pParent)
	, m_strDirName(_T(""))
{

}

CRenameModelDataDlg::~CRenameModelDataDlg()
{
}

void CRenameModelDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RENAME, m_strDirName);
	DDX_Control(pDX, IDC_RENAME, m_cModify);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
}


BEGIN_MESSAGE_MAP(CRenameModelDataDlg, CDialog)
	ON_BN_CLICKED(IDC_RENAME, &CRenameModelDataDlg::OnBnClickedRename)
END_MESSAGE_MAP()


// CRenameModelDataDlg �޽��� ó�����Դϴ�.

void CRenameModelDataDlg::OnBnClickedRename()
{
	UpdateData(true);
	USES_CONVERSION; 

	CString strOldName; //������ ���� �̸�
	CString strNewName; //������ ���� �̸�
	CString strRepository = CStatus::Instance()->GetModelRepository();
	strOldName = strRepository + _T("\\")+ COpenModelDataDlg::m_strModelName;
	strOldName.Format(_T("%s"), strOldName.GetBuffer());
	strNewName.Format(_T("%s\\%s"), strRepository.GetBuffer(), m_strDirName.GetBuffer() );

	char* filename_old = W2A(strOldName); //char*�� ����ȯ
	char* filename_new = W2A(strNewName); //char*�� ����ȯ

	if ( rename(filename_old, filename_new) != 0 ) 
	{
		CString str;
		str.LoadStringW(IDS_RENAME_FAIL);
		AfxMessageBox(str);
	}
	else 
	{
		CString str;
		str.LoadStringW( IDS_RENAME_COMPLETE);
		AfxMessageBox(str);
	}

	OnOK();
}
