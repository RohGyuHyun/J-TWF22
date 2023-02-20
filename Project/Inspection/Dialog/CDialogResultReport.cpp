// CDialogResultReport.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogResultReport.h"


// CDialogResultReport 대화 상자

IMPLEMENT_DYNAMIC(CDialogResultReport, CDialogEx)

CDialogResultReport::CDialogResultReport(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RESULT_REPORT, pParent)
{

}

CDialogResultReport::~CDialogResultReport()
{
}

void CDialogResultReport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogResultReport, CDialogEx)
END_MESSAGE_MAP()


// CDialogResultReport 메시지 처리기
