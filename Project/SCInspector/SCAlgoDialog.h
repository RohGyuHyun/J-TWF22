#pragma once

class CSCAlgoBase;
// CSCAlgoDialog ��ȭ �����Դϴ�.

class CSCAlgoDialog : public CDialog
{
	DECLARE_DYNAMIC(CSCAlgoDialog)

public:
	CSCAlgoDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSCAlgoDialog();

// ��ȭ ���� �������Դϴ�.

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void SetAlgo(CSCAlgoBase * pAlgo);
	CSCAlgoBase *m_pSCAlgo;
	virtual void LoadParam(CSCAlgoBase * pAlgo);
};
