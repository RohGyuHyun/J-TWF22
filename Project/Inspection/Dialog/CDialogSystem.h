#pragma once
#include "afxdialogex.h"


// CDialogSystem 대화 상자

class CDialogSystem : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSystem)

public:
	CDialogSystem(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogSystem();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SYSTEM };
#endif
protected:
	SYSTEM_PARA	m_stSystemPara;
	CString m_strCalibrationFileName;
	LOGIN_TYPE m_LoginType;
	CString m_strRecipe;
	BOOL m_bInverterMove;
private:
	void InitCombo();
	BOOL LoadSystemPara();
	BOOL SaveSystemPara();
	void UpdatePara(BOOL isUpdate);
	void LoginTypeBtnEnable();
public:
	void SetCalFileName(CString strFileName);
	SYSTEM_PARA GetSystemPara() { return m_stSystemPara; };
	CSize GetInspImageSize() { return CSize(m_stSystemPara.stDetector.lCamHeight, m_stSystemPara.stDetector.ICamWidth); };
	void SetXrayEndTimeWrite(TIME_DATE cEndTime);
	TIME_DATE GetXrayEndTimeRead();
	void SetSystemPara(SYSTEM_PARA sys) { m_stSystemPara = sys; };
	BOOL SetSaveSystemPara() { return SaveSystemPara(); };
	void SetLoginType(LOGIN_TYPE login_type) { m_LoginType = login_type; };
	void GetPixelSize(double* dX, double* dY) { *dX = m_stSystemPara.dPixelSizeX; *dY = m_stSystemPara.dPixelSizeY; };
	void SetRecipe(CString strRecipe) { m_strRecipe.Format(_T("%s"), strRecipe); };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSave();
	BOOL m_chk_SysPara0;
	BOOL m_chk_SysPara1;
	BOOL m_chk_SysPara2;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlSystemParaString();
	void ClickBtnenhctrlSystemParaInt();
	void ClickBtnenhctrlSystemParaDouble();
	afx_msg void OnBnClickedCheckSystemPara0();
	afx_msg void OnBnClickedCheckSystemPara1();
	afx_msg void OnBnClickedCheckSystemPara2();
	afx_msg void OnBnClickedCheckSystemPara3();
	afx_msg void OnBnClickedCheckSystemPara4();
	CComboBox m_combo_SystemPara0;
	CComboBox m_combo_SystemPara1;
	CComboBox m_combo_SystemPara2;
	CComboBox m_combo_SystemPara3;
	CComboBox m_combo_SystemPara4;
	CComboBox m_combo_SystemPara5;
	CComboBox m_combo_SystemPara6;
	CComboBox m_combo_SystemPara7;
	BOOL m_chk_SysPara3;
	BOOL m_chk_SysPara4;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1();
	BOOL m_chk_SysPara5;
	afx_msg void OnBnClickedCheckSystemPara5();
};
