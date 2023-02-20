#pragma once
#include "afxdialogex.h"
#include "CATDetector.h"
#include "CDialogTeachingPattern.h"
#include "CDialogTeachingBlobs.h"

// CDialogRecipe 대화 상자

class CDialogRecipe : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogRecipe)

public:
	CDialogRecipe(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogRecipe();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RECIPE };
#endif
protected:
	CDialogTeachingPattern* m_dlgTeachingPattern;
	CDialogTeachingBlobs* m_dlgTeachingBlobs;
	wchar_t m_strCurrentRecipeName[MAX_CHAR_LENG];
	LOGIN_TYPE m_LoginType;
	typedef void(*CALLBACK_FUNCTION_Event)(int);
	int m_nInspectionType;
	int m_nClose;
private:
	BOOL SaveRecipeInfo();
	BOOL LoadRecipeInfo();
	void ChangeRecipe(CString strBeforeName, CString strAfterName);
	void InitRecipeList();
	void DeleteRecipeList();
	void InsertRecipeList();
public:
	CString GetCurrentRecipeInfo();
	INSPEC_PARA GetInspectionPara(int nPtnIndex) { return m_dlgTeachingPattern->GetInspectionPara(nPtnIndex); };
	BLOB_INSPEC_PARA GetInspectionPara() { return m_dlgTeachingBlobs->GetInspectionPara(); };
	//void SetDetector(CAtDetector* AtDetector) { m_dlgTeachingPattern->SetDetector(AtDetector); };
	HWND GetTeachingDlgHwnd();
	//void SetXray(CXRayClass* xray) { m_dlgTeachingPattern->SetXray(xray); };
	void SetLoginType(LOGIN_TYPE login_type) { m_LoginType = login_type; };
	void SetImageSize(int nWidth, int nHeight);
	void SetCallBack(CALLBACK_FUNCTION_Event event);
	void SetInspectionType(int nType) { m_nInspectionType = nType; };
	void SetPixelSize(double dX, double dY) { m_dlgTeachingBlobs->SetPixelSize(dX, dY); };
	int GetClose() { return m_nClose; };

	int GetInspTypCnt() { return m_dlgTeachingBlobs->GetInspTypCnt(); };
	BLOB_INSPEC_PARA GetInspectionTypePara(int nTypeIndex) { return m_dlgTeachingBlobs->GetInspectionPara(nTypeIndex); };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonInspectionTeaching();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonClose();
	//CListCtrl m_list_Recipe;
	afx_msg void OnBnClickedButtonNewRecipe();
	afx_msg void OnBnClickedButtonDeleteRecipe();
	afx_msg void OnBnClickedButtonChangeRecipe();

	CString m_edit_SelectRecipe;
	CListBox m_list_Recipe;
	CString m_edit_CurrentRecipeName;
	afx_msg void OnLbnSelchangeListRecipe();
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlNewRecipe();
	void ClickBtnenhctrlDeleteRecipe();
	void ClickBtnenhctrlChangeRecipe();
	void ClickBtnenhctrlInspTeaching();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
