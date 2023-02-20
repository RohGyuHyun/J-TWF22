
// UniUntilityTestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "UniUntilityTest.h"
#include "UniUntilityTestDlg.h"
#include "afxdialogex.h"
#include "XMLTestDlg.h"
#include "UniUtility.h"
#include "Markup.h"
#include <afxcoll.h>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUniUntilityTestDlg ��ȭ ����

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif


CUniUntilityTestDlg::CUniUntilityTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUniUntilityTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUniUntilityTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_IMG, m_ImgBtn);
}

BEGIN_MESSAGE_MAP(CUniUntilityTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_BUTTON1, &CUniUntilityTestDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BXMLTEST, &CUniUntilityTestDlg::OnBnClickedBxmltest)
ON_BN_CLICKED(IDC_BTN_CFG, &CUniUntilityTestDlg::OnBnClickedBtnCfg)
ON_BN_CLICKED(IDC_BTN_IMG, &CUniUntilityTestDlg::OnBnClickedBtnImg)
ON_BN_CLICKED(IDC_BTN_PARAMPARSER, &CUniUntilityTestDlg::OnBnClickedBtnParamparser)
END_MESSAGE_MAP()


// CUniUntilityTestDlg �޽��� ó����

BOOL CUniUntilityTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_ImgBtn.SetBitmap((HBITMAP)IDB_BMPBTN);
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CUniUntilityTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CUniUntilityTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CUniUntilityTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CUniUntilityTestDlg::OnBnClickedButton1()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//}


void CUniUntilityTestDlg::OnBnClickedBxmltest()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CXMLTestDlg xmlTestDlg;
	xmlTestDlg.DoModal();
}


void CUniUntilityTestDlg::OnBnClickedBtnCfg()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CUniUntilityTestDlg::OnBnClickedBtnImg()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CUniUntilityTestDlg::OnBnClickedBtnParamparser()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMarkup xml;
	xml.Load(_T("d:\\result.xml"));
	
	CString csXML = xml.GetDoc();

	wprintf(_T("%s"), csXML);

	double dblScore;
	CPtrList listPoint;
	GetResult(csXML.GetBuffer(), dblScore, listPoint);

}


int CUniUntilityTestDlg::GetResult(TCHAR * szData, double & dblScore, CPtrList& listPoint)
{
	CString strPoints;
	CMarkup xml;
	xml.SetDoc(szData);

	if (xml.FindElem(_T("sample")))
	{
		if (xml.FindChildElem(_T("image")))
		{
			xml.IntoElem();
			if (xml.FindChildElem(_T("marking")))
			{
				xml.IntoElem();
				if (xml.FindChildElem(_T("view")))
				{
					xml.IntoElem();
					if (xml.FindChildElem(_T("red")))
					{
						xml.IntoElem();
						if (xml.FindChildElem(_T("region")))
						{
							xml.IntoElem();
							CString strScore = xml.GetAttrib(_T("score"));
							double dblScore = _wtof(strScore);
							strPoints = xml.GetAttrib(_T("points"));
						}

					}
				}

			}
		}
	}


	CString strToken;
	int pos = 0;
	int iIndex = 0;
	
	CdblPoint *pDblPoint = NULL;

	while ((strToken = strPoints.Tokenize(_T(" ,"), pos)) != _T(""))
	{
		if( (iIndex%2)==0 )
		{
			pDblPoint = new(CdblPoint);
			pDblPoint->dblPosX = _wtof(strToken);
		}else
		{ 
			pDblPoint->dblPosY = _wtof(strToken);
			listPoint.AddTail(pDblPoint);
		}
		//		wprintf(_T("\n%d:%s"), iIndex, strToken.GetBuffer());
		iIndex++;

	}

	return 0;
}
