#pragma once
#include "afxwin.h"
#include "afxcoll.h"
#include "listports.h"


// CComSetDlg ��ȭ �����Դϴ�.
//typedef tSerialEnum static BOOL CALLBACK SerialEnum(LPVOID lpCallbackValue, LISTPORTS_PORTINFO* lpPortInfo)
class CComSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CComSetDlg)

public:
	CComSetDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CComSetDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMSET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCbportno();
	static BOOL CALLBACK SerialEnum(LPVOID lpCallbackValue, LISTPORTS_PORTINFO* lpPortInfo);
	//! Port�� ��ȣ�� �ƴ������ϰ�� ����� �� ����.
	CString m_strPortNo;
	int m_iBaudRate;
	CComboBox m_cPortNo;
	CComboBox m_cBaudRate;
	virtual BOOL OnInitDialog();
	CString GetComPortEnum();
	static CPtrList *m_pLstComsName;
	static CPtrList *m_pLstComsNo;
	afx_msg void OnBnClickedOk();
	int FindIndex(TCHAR *szBaudRate, CComboBox * pComboBox);
};
