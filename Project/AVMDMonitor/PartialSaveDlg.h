#pragma once

class CRepository;
// CPartialSaveDlg ��ȭ �����Դϴ�.

class CPartialSaveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPartialSaveDlg)

public:
	CPartialSaveDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPartialSaveDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARTIALSAVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnNext();
	int m_nStep;
	int OnStepNext();
	static int m_iStartPartialNo;
	static int m_iEndPartialNo;
	CRepository *m_pRepository;
	CString m_strLabelPartialSave;
	virtual BOOL OnInitDialog();
	void Init();
	static UINT PartialSaveThread(LPVOID lParam);
};
