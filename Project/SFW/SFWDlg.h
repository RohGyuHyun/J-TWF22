
// SFWDlg.h : ��� ����
//

#pragma once
#include "CamViewDlg.h"
#include "TabContainerDlg.h"
#include "MapDlg.h"
#include "InfoPanelDlg.h"
#include "XrayPanelDlg.h"


// CSFWDlg ��ȭ ����
class CSFWDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CSFWDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SFW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int MakeChildWnd();
	CCamViewDlg *m_pCamViewDlg;
	CTabContainerDlg *m_pTabContainerDlg;
	CMapDlg *m_pMapDlg;
	CInfoPanelDlg *m_pInfoPanelDlg;
	CXrayPanelDlg *m_pXrayPanelDlg;
};
