/**
	@file LoginDlg.h
	@brief DB �� �����Ͽ� ���� ���̵�� �н����带 Ȯ���Ͽ� �α����Ѵ�. ���� ������ ���� ��� ����� ���ѵȴ�.
	@author jubaking
	@date 2020-10-01
*/

#pragma once
#include "afxwin.h"
#include "UniUtility.h"
#include "UserControl.h"

// CLoginDlg dialog

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

// Constructor
public:
	enum tLongType {liLogin=IDOK,liExit=IDCANCEL,liConfig=IDCANCEL+1};
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDlg();

// Dialog Data
	enum { IDD = IDD_LOGIN };

// Attributes
protected:
	CString m_strcontrol;

public:
	void GetControlString(CString& strControl) { strControl = m_strcontrol; }

// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Operations

// Implementation
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLogin();	///< �Է��� ���� ������, ���̵�, �н����带 �޾ƿ� DB ������ ���Ͽ� �α��� ������ �Ǻ�.
	afx_msg void OnBnClickedBtnExit();	///< ���� ��ư.
	afx_msg void OnBnClickedBtnConfig();///< ConfigDlg ���̾�α׸� �ҷ��´�.
	CComboBox m_cbUserID;		///< ���� ������ �����ϴ� �޺��ڽ� ��Ʈ�� ����.
	int m_nLevel;				///< ������ ���� ������ �޾ƿ��� �޺��ڽ� ����.
	virtual BOOL OnInitDialog();
	CString m_strId;			///< �Է��� �������̵� string ����.
	CString m_strPassword;		///< �Է��� �н����� string ����.
	BOOL m_bSelectMode;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
