#pragma once

// CDiagnosis1Dlg ��ȭ �����Դϴ�.

class CDiagnosis1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiagnosis1Dlg)

public:
	CDiagnosis1Dlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDiagnosis1Dlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
