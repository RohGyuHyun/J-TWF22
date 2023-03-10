
// UniUntilityTestDlg.cpp : 구현 파일
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


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CUniUntilityTestDlg 대화 상자

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


// CUniUntilityTestDlg 메시지 처리기

BOOL CUniUntilityTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_ImgBtn.SetBitmap((HBITMAP)IDB_BMPBTN);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CUniUntilityTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CUniUntilityTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CUniUntilityTestDlg::OnBnClickedButton1()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}


void CUniUntilityTestDlg::OnBnClickedBxmltest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CXMLTestDlg xmlTestDlg;
	xmlTestDlg.DoModal();
}


void CUniUntilityTestDlg::OnBnClickedBtnCfg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CUniUntilityTestDlg::OnBnClickedBtnImg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CUniUntilityTestDlg::OnBnClickedBtnParamparser()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
