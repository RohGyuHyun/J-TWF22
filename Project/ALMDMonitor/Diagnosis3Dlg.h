#pragma once


// CDiagnosis3Dlg 대화 상자입니다.

class CDiagnosis3Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiagnosis3Dlg)

public:
	CDiagnosis3Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDiagnosis3Dlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
