#pragma once
#include "afxdialogex.h"
#include "CATDetector.h"
#include "CXRayClass.h"
#include "CDialogXRayStatus.h"
#include "CInspectionClass.h"
#include "DialogRect.h"
#include "GetNumDlg.h"
// CDialogTeachingPattern 대화 상자

class CDialogTeachingPattern : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogTeachingPattern)

public:
	CDialogTeachingPattern(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogTeachingPattern();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INSPECTION_TEACHING_PATTERN};
#endif
protected:
	CDialogXRayStatus* m_dlgXRayStatus;
#ifdef MIL_USE
	CMilDisplay* m_MilDisplay;
	CMilBuffer* m_MilDispBuff;
	CMilBuffer* m_MilResultDispBuff;

	CMilDisplay* m_MilPatternDisplay;
	CMilBuffer* m_MilPatternDispBuff;
	CMilBuffer* m_MilPatternResultDispBuff;

	CMilDisplay* m_MilPatternMaskDisplay;
	CMilBuffer* m_MilPatternMaskDispBuff;
#endif
	INSPEC_PARA m_InspPara[MAX_PATTERN_ADD_CNT];
	RESULT_DATA m_ResultData;
	CRect m_DispRect[2];

	BOOL m_bLBMouseDown;
	BOOL m_bRBMouseDown;
	BOOL m_bMouseMove;

	BOOL m_bPatternAdd;
	BOOL m_bPatternDel;
#ifdef MIL_USE
	MIL_ID m_GraphicList;
	CInspectionClass *m_InspectionClass[MAX_PATTERN_ADD_CNT];
#endif
	CDialogRect *m_DlgRect;
	BOOL m_isDlgShow;

	CString m_strCurrentRecipeName;

	CBtnenhctrl m_ctrTeachingPara[MAX_RECT_CNT];
	
	int m_nPtnAddIndex;

	//CAtDetector* m_AtDetector;

	CRect m_DefaultRect[2];
	BOOL m_bInitDisplay;

	CSize m_ImageSize;
	double m_dRectRet[2];
	CPoint m_pImagePoint;
	typedef void(*CALLBACK_FUNCTION_Event)(int);
private:
	BOOL LoadTeachingPara();
	BOOL SaveTeachingPara();
	void UpdatePara(BOOL isUpdate);

	void PatternAddFunc();
	BOOL PatternSearchFunc();

	void PatternAddImageCenAverageVal();

	void SavePatternAddParaImage();
	void LoadPatternAddParaImage();
	void InspButtonEnable(BOOL isEnable);
public:
	void SetRecipeName(CString strRecipe) { m_strCurrentRecipeName.Format(_T("%s"), strRecipe); LoadTeachingPara(); };
	void SetInsertRecipeFile(CString strRecipe){ m_strCurrentRecipeName.Format(_T("%s"), strRecipe); SaveTeachingPara(); };
	//void SetDetector(CAtDetector* AtDetector) { m_AtDetector = AtDetector; };
	INSPEC_PARA GetInspectionPara(int nPtnIndex) { LoadTeachingPara(); return m_InspPara[nPtnIndex]; };
	HWND GetTeachingDlgHwnd() { return this->m_hWnd; };
	//void SetXray(CXRayClass* xray) { m_dlgXRayStatus->SetXrayClass(xray); };
	void SetImageSize(int nWidth, int nHeight) { m_ImageSize.SetSize(nWidth, nHeight); };
	void SetCallBack(CALLBACK_FUNCTION_Event event) { m_dlgXRayStatus->SetCallBack(event); };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonImgLoad();
	afx_msg void OnBnClickedButtonImgSave();
	afx_msg void OnBnClickedButtonImgGrab();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonSetRoi();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonPatternAdd();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonPatternSearch();
	afx_msg void OnBnClickedButtonPatternDel();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	LRESULT OnImageViewToTeachingMessage(WPARAM para0, LPARAM para1);
	afx_msg void OnBnClickedButtonSave();
	DECLARE_EVENTSINK_MAP()
	void SelectParameterNumDlgShowInt();
	void SelectParameterNumDlgShowDouble();
	void ClickBtnenhctrlTeachingMinus();
	void ClickBtnenhctrlTeachingPlus();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ClickBtnenhctrlTeachingPara12();
	CSliderCtrl m_slider_Para0;
	CSliderCtrl m_slider_Para1;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonDisplayOriginalImage();
};
