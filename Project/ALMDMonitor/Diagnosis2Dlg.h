#pragma once


// CDiagnosis2Dlg ��ȭ �����Դϴ�.

class CDiagnosis2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiagnosis2Dlg)

public:
	CDiagnosis2Dlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDiagnosis2Dlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
