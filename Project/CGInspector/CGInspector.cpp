
// CGInspector.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "CGInspector.h"
#include "MainFrm.h"

#include "CGInspectorDoc.h"
#include "CGInspectorView.h"
#include "CGInspIO.h"
#include "LoginDlg.h"
#include "Status.h"
#include "ConveyorCtrl.h"
#include "repository.h"
#include <UniUtility.h>
#include <UniMotion.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCGInspectorApp

BEGIN_MESSAGE_MAP(CCGInspectorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CCGInspectorApp::OnAppAbout)
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ǥ�� �μ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CCGInspectorApp ����

CCGInspectorApp::CCGInspectorApp()
	: m_bOnline(FALSE)
{
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���� ���α׷��� ���� ��� ��Ÿ�� ������ ����Ͽ� ������ ���(/clr):
	//     1) �� �߰� ������ �ٽ� ���� ������ ������ ����� �۵��ϴ� �� �ʿ��մϴ�.
	//     2) ������Ʈ���� �����Ϸ��� System.Windows.Forms�� ���� ������ �߰��ؾ� �մϴ�.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("CGInspector.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CCGInspectorApp ��ü�Դϴ�.

CCGInspectorApp theApp;


// CCGInspectorApp �ʱ�ȭ

BOOL CCGInspectorApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.


	// ���� ���α׷��� ���� ���ø��� ����մϴ�.  ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCGInspectorDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CCGInspectorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	InitEnvInstance();

	CLoginDlg LogInDlg;
	if (LogInDlg.DoModal() == CLoginDlg::liExit)
	{
		return FALSE;
	}
	//	CConfigInfo::Instance()->Init();///Option���� ������ ������ �����´�.
	CCGInspIO::Instance()->Init();

	if (CMotionCtrl::Instance()->Init() != CMotionCtrl::XaMotionOK)
	{
		CStatus::Instance()->m_bIsOffLine = TRUE;
		m_bOnline = FALSE;
	}
	else
	{
		CStatus::Instance()->m_bIsOffLine = FALSE;
		m_bOnline = TRUE;
	}

	BOOL bValue = CStatus::Instance()->m_bIsOffLine;
	CConveyorCtrl::Instance()->LoadCFG();
	bValue = CStatus::Instance()->m_bIsOffLine;
	InitVision();

	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();


	return TRUE;
}

int CCGInspectorApp::ExitInstance()
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	AfxOleTerm(FALSE);

	//if (!CStatus::Instance()->m_bIsOffLine)
	//{
	//	int nCountOfCam = CStatus::Instance()->m_nCountOfCam;
	//	for (int i = 0; i < nCountOfCam; i++)
	//	{
	//		CRepository::Instance()->m_Grabber[i].Close();
	//	}
	//}

	Sleep(100);

	return CWinApp::ExitInstance();
}

// CCGInspectorApp �޽��� ó����


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CCGInspectorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CCGInspectorApp �޽��� ó����





int CCGInspectorApp::InitVision()
{
	if (CRepository::Instance()->m_MilLib.DefaultSetting())
	{
		AfxMessageBox(_T("Cann't open default setting"));
		return -1;
	}

	if (!CStatus::Instance()->m_bIsOffLine)
	{
		int nCountOfCam = CStatus::Instance()->m_nCountOfCam;
		for (int i = 0; i < nCountOfCam; i++)
		{
			if (CRepository::Instance()->m_Grabber[i].Init())
			{
				CStatus::Instance()->m_bIsOffLine = FALSE;
			}
		}
	}

	return 0;
}


void CCGInspectorApp::InitEnvInstance()
{
	CCfgFile *pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
	CStatus::Instance()->LoadCFG();
}
