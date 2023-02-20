#pragma once
#include "SCSocket.h"

// CCamRemoteDlg ��ȭ �����Դϴ�.

class CCamRemoteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCamRemoteDlg)

public:
	CCamRemoteDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCamRemoteDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMREMOTE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CMilBuffer *m_pMilCamView;
	CMilDisplay *m_pMilDisp;
	int m_iCamBuildNo;
	CString m_strCamName;
	CSCSocket *m_pSockContainer;
	static int m_iRemoteIndex;
	LRESULT OnReceiveCMD(WPARAM wparam, LPARAM lparam);
	void LoadFile(CString strRemoteFileName);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitRemoteCam();
	LRESULT OnSockInitListen(WPARAM wparam, LPARAM lparam);
	void InitSockListen();
	void CreateSockListen();
};
