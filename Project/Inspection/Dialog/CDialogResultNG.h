#pragma once
#include "afxdialogex.h"


// CDialogResultNG 대화 상자

class CDialogResultNG : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogResultNG)

public:
	CDialogResultNG(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogResultNG();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RESULT_NG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
