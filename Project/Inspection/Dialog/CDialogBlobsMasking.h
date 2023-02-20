#pragma once
#include "afxdialogex.h"
#include "DialogRect.h"
#include "Repository.h"
#include "CInspectionClass.h"
// CDialogBlobsMasking 대화 상자

class CDialogBlobsMasking : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogBlobsMasking)

public:
	CDialogBlobsMasking(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogBlobsMasking();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BLOBS_MASK };
#endif

protected:
	CRect m_DispRect[2];
	//CDialogRect* m_DlgRect;
	double m_dRectRet;
	CSize m_ImageSize;
	CPoint m_pImagePoint;
	CMilDisplay* m_MilDisplay;
	CMilBuffer* m_MilDispBuff;
	CMilBuffer* m_MilResultDispBuff;
	CMilBuffer* m_MilMaskBuff;

	BOOL m_bLbtnMouseDown;
	BOOL m_bMaskRemove;
	CString m_strRecipeName;
	CInspectionClass *m_InspectionClass;
	int m_nMaskSize;
	double m_dSearchAreaCenX;
	double m_dSearchAreaCenY;

	MASKING_PARA m_stMaskingPara;
	BOOL m_bMaskingUse;
private:
	void ClearMaskOverlay();
	void DrawMaskOverlay(BOOL isLoadFile = TRUE);
	BOOL SaveMaskImage();
	BOOL LoadMaskImage();
	void UpdatePara(BOOL isUpdate);
public:
	void SetDispImage(CMilBuffer* src);
	void SetRecipeName(CString strRecipe) { m_strRecipeName.Format(_T("%s"), strRecipe); };
	void SetSearchAreaCenXY(double dX, double dY) { m_dSearchAreaCenX = dX; m_dSearchAreaCenY = dY; };
	void SetMaskingPara(MASKING_PARA para) { m_stMaskingPara = para; };
	MASKING_PARA GetMaskingPara() { return m_stMaskingPara; };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMaskingStart();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCheckMaskPara0();
	afx_msg void OnBnClickedCheckMaskPara1();
	BOOL m_chk_MaskPara0;
	BOOL m_chk_MaskPara1;
	CComboBox m_combo_MaskSize;
	afx_msg void OnBnClickedButtonMaskingDelete();
	afx_msg void OnCbnEditupdateComboMaskSize();
};
