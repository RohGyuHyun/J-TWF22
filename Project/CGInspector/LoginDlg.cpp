// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoginDlg.h"
#include "resource.h"
#include "ConfigDlg.h"

// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
: CDialog(CLoginDlg::IDD, pParent)
, m_nLevel(0)
, m_strId(_T(""))
, m_strPassword(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_USERTYPE, m_cbUserID);
	DDX_CBIndex(pDX, IDC_COMBO_USERTYPE, m_nLevel);
	DDX_Text(pDX, IDC_EDT_PSWD2, m_strId);
	DDX_Text(pDX, IDC_EDT_PSWD, m_strPassword);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CLoginDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CLoginDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_CONFIG, &CLoginDlg::OnBnClickedBtnConfig)
END_MESSAGE_MAP()


// CLoginDlg message handlers

void CLoginDlg::OnBnClickedBtnLogin()
{
	UpdateData(true);

	CUserControl m_CControl;

//	m_fireDB.UserSelectLogin(m_strId);	///< �Էµ� ���̵�� DB �� ������ ������ ���� �غ� �Ѵ�.

	CString strlevel;

	strlevel.Format(_T("%d"),m_nLevel);	///< ���������� string ������ �ٲ۴�.

	//if( strlevel == m_fireDB.m_User.strUSER_level )
	//{
	//	if( m_strId == m_fireDB.m_User.strUSER_id )
	//	{
	//		if( m_strPassword == pGetMainDlg->GetPasswordStr(m_fireDB.m_User.strUSER_password) )
	//		{
	//			m_fireDB.UserLoginTime();

	//			pGetMainDlg->m_strCControl = m_fireDB.m_User.strUSER_control;	///< ���� �������� ������ ��Ʈ�� ��� ������ MainDlg �� �����Ѵ�.
	//			pGetMainDlg->m_strCLevel = m_fireDB.m_User.strUSER_level;		///< ���� �������� ������ ���� ������ MainDlg �� �����Ѵ�.
	//			pGetMainDlg->m_strCId = m_fireDB.m_User.strUSER_id;				///< ���� �������� ������ ���̵� MainDlg �� �����Ѵ�. 

	//			CUserControl m_user_control;

	//			if( !m_bSelectMode )
	//			{
	//				if( strlevel == _T("0"))
	//				{
	//					m_user_control.LEVEL0(m_fireDB.m_User.strUSER_control);
	//				}
	//				else if( strlevel == _T("1"))
	//				{
	//					m_user_control.LEVEL1(m_fireDB.m_User.strUSER_control);
	//				}
	//				else if( strlevel == _T("2"))
	//				{
	//					m_user_control.LEVEL2(m_fireDB.m_User.strUSER_control);
	//				}
	//			}

	//			EndDialog(liLogin);		///< ���� ������ ��ġ�� ��� Login ȭ���� ����Ǹ鼭 ���α׷��� ����ȴ�.
	//		}
	//		else
	//		{
	//			m_strId.Empty();
	//			m_strPassword.Empty();
	//			CString str;
	//			str.LoadStringW(IDS_CHECKIDPASSWORD);
	//			AfxMessageBox(str);
	//		}
	//	}
	//	else
	//	{
	//		CString str;
	//		str.LoadStringW(IDS_LOGIN_CHECK_INFO);
	//		m_strId.Empty();
	//		m_strPassword.Empty();
	//		AfxMessageBox(str);
	//	}
	//}
	//else
	//{
	//	CString str;
	//	str.LoadStringW(IDS_LOGIN_CHECK_INFO);
	//	m_strId.Empty();
	//	m_strPassword.Empty();
	//	AfxMessageBox(str);
	//}

	//	EndDialog(liLogin);
}

void CLoginDlg::OnBnClickedBtnExit()
{
	EndDialog(liExit);
}

void CLoginDlg::OnBnClickedBtnConfig()
{
	CConfigDlg dlg;
	dlg.DoModal();
}

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cbUserID.SetCurSel(0);

	//	control.LEVEL1();

	//m_fireDB.ClassInit(_T(""), _T("..\\..\\Runtime\\DB\\DB_9800A.FDB"), _T("d:\\DB_9800A.BAK"), _T("xavis"), _T("xavis"));
	//m_fireDB.ConnectDB();
   
	return TRUE;  
	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == VK_RETURN )
	{
		//		OnBnClickedBtnLogin();
	}

	return CDialog::PreTranslateMessage(pMsg);
}
