/**
	@file LoginDlg.h
	@brief DB 에 연결하여 유저 아이디와 패스워드를 확인하여 로그인한다. 유저 레벨에 따라 사용 기능이 제한된다.
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
	afx_msg void OnBnClickedBtnLogin();	///< 입력한 유저 레벨과, 아이디, 패스워드를 받아와 DB 정보와 비교하여 로그인 유무를 판별.
	afx_msg void OnBnClickedBtnExit();	///< 종료 버튼.
	afx_msg void OnBnClickedBtnConfig();///< ConfigDlg 다이얼로그를 불러온다.
	CComboBox m_cbUserID;		///< 유저 레벨을 설정하는 콤보박스 컨트롤 변수.
	int m_nLevel;				///< 설정된 유저 레벨을 받아오는 콤보박스 변수.
	virtual BOOL OnInitDialog();
	CString m_strId;			///< 입력한 유저아이디 string 변수.
	CString m_strPassword;		///< 입력한 패스워드 string 변수.
	BOOL m_bSelectMode;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
