#pragma once
#include "afxwin.h"


// CDiagnosis4Dlg ��ȭ �����Դϴ�.

class CDiagnosis4Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiagnosis4Dlg)

public:
	CDiagnosis4Dlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDiagnosis4Dlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void HideResult();
	CStatic m_cGResult;
	CStatic m_cRPResult;
	CStatic m_cRRResult;
	CStatic m_cPPResult;
	void ShowResult();
	void SetResult(double dblRPResult, double dblRRResult, double dblPPResult);
	CString m_strRPResult;
	CString m_strRRResult;
	CString m_strPPResult;
};
