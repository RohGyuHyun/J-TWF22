#pragma once
#include "afxdialogex.h"


// CDialogResultReport 대화 상자

class CDialogResultReport : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogResultReport)

public:
	CDialogResultReport(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogResultReport();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RESULT_REPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
