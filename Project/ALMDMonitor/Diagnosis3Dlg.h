#pragma once


// CDiagnosis3Dlg ��ȭ �����Դϴ�.

class CDiagnosis3Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiagnosis3Dlg)

public:
	CDiagnosis3Dlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDiagnosis3Dlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
