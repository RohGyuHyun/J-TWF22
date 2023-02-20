// SaveModelData.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "XFMS.h"
#include "Status.h"
#include "SaveModelDataDlg.h"
#include "OpenModelDataDlg.h"

// CSaveModelDataDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSaveModelDataDlg, CDialog)

CSaveModelDataDlg::CSaveModelDataDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSaveModelDataDlg::IDD, pParent)
, m_strModelName(_T(""))
{

}

CSaveModelDataDlg::~CSaveModelDataDlg()
{
}

void CSaveModelDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_MODELNAME, m_strModelName);
	DDX_Control(pDX, IDOK, m_cOK);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
}


BEGIN_MESSAGE_MAP(CSaveModelDataDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSaveModelDataDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_BROWSER, &CSaveModelDataDlg::OnBnClickedBtnBrowser)
END_MESSAGE_MAP()


// CSaveModelDataDlg �޽��� ó�����Դϴ�.

void CSaveModelDataDlg::OnBnClickedOk()
{  
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CModelData *pModelData = CStatus::Instance()->GetModelData();
	int nCount = pModelData->GetList()->GetCount();
	if(nCount)
	{
		UpdateData();

		CString strFolderPath= pModelData->GetModelPath();
		pModelData->SetMDName(m_strModelName.GetBuffer());
		CStatus::Instance()->SetModelName(m_strModelName.GetBuffer());
		pModelData->SaveModelData();

		/////////////////// ������ ������ ������ ��� ������ ������ ��ξ����� ��°�� �ű�
		OnOK();
	}
	else
	{
		AfxMessageBox(IDS_NOT_FOUND_TEACHING_DATA);
	}
}

void CSaveModelDataDlg::CopyfileSetuptoModelFloder(CString OriginPath,CString NewPath)
{    
	
	if(AfxMessageBox(IDS_MODEL_SAVE_START,MB_OKCANCEL)==IDOK)
	{   
		CopyFile(OriginPath+_T("\\CameraPIP_other.ini"),NewPath+_T("\\CameraPIP_other.ini"),FALSE);  // ���Ϻ��� 
				
		CopyFile(OriginPath+_T("\\PCBImage\\AllImage.tif"),NewPath+_T("\\AllImage.tif"),FALSE);
		SHFILEOPSTRUCT fo;
		fo.wFunc=FO_COPY;
		fo.pFrom=_T("D:\\StarPrj\\Runtime\\Setup\\PCBImage\\ImageList"); // ������ü ���� 
		fo.pTo=NewPath;
		fo.fFlags=FOF_NOCONFIRMMKDIR;
		SHFileOperation(&fo);
	}
	else 
		return;

	AfxMessageBox(IDS_SAVE_OK);

}

void CSaveModelDataDlg::OnBnClickedBtnBrowser()
{
	CFolderDialog FloderDlg;
	CString strModelName = _T("");
	CString strFullName = _T("");

	switch(FloderDlg.DoModal())
	{
	case IDOK:
		{
		strFullName = FloderDlg.GetFolderPath();

			strFullName.MakeReverse();

			int i = strFullName.Find('\\');
			strModelName = strFullName.Left(i);

			strModelName.MakeReverse();

			m_strModelName = strModelName;
		};
		break;
	case IDCANCEL:
		m_strModelName = _T("NewModel");
		break;
	}

	UpdateData(FALSE);
}
BOOL CSaveModelDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if( CStatus::Instance()->GetModelName() == _T("") )
	{
		m_strModelName =_T("NewModel");
	}
	else
	{
		m_strModelName = CStatus::Instance()->GetModelName();
	}	

	UpdateData(FALSE);
	CStatus::Instance()->LoadCFG();
//    CString Dirstr = CStatus::Instance()->m_LengDir;
//	CChangeRes::Instance()->ChangeDialogCaption(Dirstr,this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
