#pragma once


// CDispView ��ȭ �����Դϴ�.

class CDispView : public CDialogEx
{
	DECLARE_DYNAMIC(CDispView)

public:
	CDispView(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDispView();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
