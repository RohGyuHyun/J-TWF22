// CDialogResultNG.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogResultNG.h"


// CDialogResultNG 대화 상자

IMPLEMENT_DYNAMIC(CDialogResultNG, CDialogEx)

CDialogResultNG::CDialogResultNG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RESULT_NG, pParent)
{

}

CDialogResultNG::~CDialogResultNG()
{
}

void CDialogResultNG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogResultNG, CDialogEx)
END_MESSAGE_MAP()


// CDialogResultNG 메시지 처리기
