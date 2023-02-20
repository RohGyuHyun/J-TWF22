// XMLTestDlg.cpp : ���� �����Դϴ�.
//
#include "stdafx.h"
#include "UniUntilityTest.h"
#include "XMLTestDlg.h"
#include "afxdialogex.h"
#include <UniUtility.h>

// CXMLTestDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CXMLTestDlg, CDialog)

CXMLTestDlg::CXMLTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXMLTestDlg::IDD, pParent)
{

}

CXMLTestDlg::~CXMLTestDlg()
{
}

void CXMLTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CXMLTestDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_XML_READ, &CXMLTestDlg::OnBnClickedBtnXmlRead)
	ON_BN_CLICKED(IDC_BTN_XML_WRITE, &CXMLTestDlg::OnBnClickedBtnXmlWrite)
	ON_BN_CLICKED(IDC_BTN_SINGLE, &CXMLTestDlg::OnBnClickedBtnSingle)
END_MESSAGE_MAP()


// CXMLTestDlg �޽��� ó�����Դϴ�.


void CXMLTestDlg::OnBnClickedBtnXmlRead()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CCfgFile MyXMLCfg;
	MyXMLCfg.SetPath(CAppInfo::GetAppDir() + _T("\\My.Cfg"));
	MyXMLCfg.SetDepth(2);
	MyXMLCfg.Open();

	long iValue;
	double dblValue;
	CString strValue;
	dblValue	= MyXMLCfg.Get(10.5, _T("Setup"), _T("FirstDouble"));
	iValue = MyXMLCfg.Get(15L, _T("Setup"), _T("FirstLong"));
	strValue = MyXMLCfg.Get(_T("TESTstring"), _T("Setup"), _T("FirstString"));

}


void CXMLTestDlg::OnBnClickedBtnXmlWrite()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CCfgFile MyXMLCfg;
	MyXMLCfg.SetPath(CAppInfo::GetAppDir() + _T("\\My.Cfg"));
	//! depth�� 2��  default�׷��� 2�� ��� SetDepth�ʿ����
	MyXMLCfg.SetDepth(2);
	MyXMLCfg.Open();

	//! Tag�� 2�� �̻� ���� ������ �����߻��մϴ�. 
	MyXMLCfg.Set(55.7, _T("Setup"), _T("FirstDouble"));
	MyXMLCfg.Set(15L, _T("Setup"), _T("FirstLong"));
	MyXMLCfg.Set(_T("testString"), _T("Setup"), _T("FirstString"));

	MyXMLCfg.Flush();
}


void CXMLTestDlg::OnBnClickedBtnSingle()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//Read 
	//CCfgFile *pMyCfg= CCfgFile::Instance();
	//pMyCfg->Open();
	//pMyCfg->SetPath(CAppInfo::GetAppDir() + _T("Test.xml"));
	//pMyCfg->Get()
	//pMyCfg->Flush();

	//Write
	CCfgFile *pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir() + _T("\\Test.xml"));
	pMyCfg->SetDepth(2);
	pMyCfg->Open();
	pMyCfg->Set(_T("TEST"), _T("Setup"), _T("TagFirst"));
	pMyCfg->Set(100L, _T("Setup"), _T("TagSecond"));
	pMyCfg->Set(10.5, _T("Setup"), _T("TagThird"));
	pMyCfg->Flush();
}
