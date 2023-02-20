#pragma once
#include "afxdialogex.h"
#include "DialogRect.h"
#include "CDialogXRayStatus.h"
#include "CInspectionClass.h"
#include "CDialogBlobsMasking.h"

// CDialogTeachinBlobs 대화 상자

class CDialogTeachingBlobs : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogTeachingBlobs)

public:
	CDialogTeachingBlobs(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogTeachingBlobs();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INSPECTION_TEACHING_BLOBS };
#endif

protected:
	CDialogXRayStatus* m_dlgXRayStatus;
	CDialogBlobsMasking* m_dlgMasking;
	CString m_strCurrentRecipeName;
	BLOB_INSPEC_PARA m_InspPara;

	vector< BLOB_INSPEC_PARA> m_vInspPara;

	CMilDisplay* m_MilDisplay;
	CMilBuffer* m_MilDispBuff;
	CMilBuffer* m_Mil16bitBuff;
	CMilBuffer* m_MilResultDispBuff;

	CRect m_DispRect[2];
	//CDialogRect* m_DlgRect;

	CBtnenhctrl m_ctrTeachingPara[30];
	CInspectionClass* m_InspectionClass;

	CSize m_ImageSize;
	double m_dRectRet[2];
	CPoint m_pImagePoint;
	double m_dPixelSizeX;
	double m_dPixelSizeY;

	typedef void(*CALLBACK_FUNCTION_Event)(int);
	CRect m_DispPanImg;
	BOOL m_bPanDisp;
	CPoint m_PanPoint;

	BOOL m_bGrabStart;
private:
	BOOL LoadTeachingPara();
	BOOL SaveTeachingPara();
	void UpdatePara(BOOL isUpdate);
	void MaskingImageView();

public:
	void SetRecipeName(CString strRecipe) { m_strCurrentRecipeName.Format(_T("%s"), strRecipe); LoadTeachingPara(); };
	void SetImageSize(int nWidth, int nHeight) { m_ImageSize.SetSize(nWidth, nHeight); };
	void SetCallBack(CALLBACK_FUNCTION_Event event) { m_dlgXRayStatus->SetCallBack(event); };
	HWND GetTeachingDlgHwnd() { return this->m_hWnd; };
	BLOB_INSPEC_PARA GetInspectionPara() { LoadTeachingPara(); return m_InspPara; };
	void SetPixelSize(double dX, double dY) { m_dPixelSizeX = dX; m_dPixelSizeY = dY; };

	int GetInspTypCnt() { return (int)m_vInspPara.size(); };
	BLOB_INSPEC_PARA GetInspectionPara(int nTypeIndex) { return m_vInspPara[nTypeIndex]; };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonImgLoad();
	afx_msg void OnBnClickedButtonImgSave();
	afx_msg void OnBnClickedButtonImgGrab();
	afx_msg void OnBnClickedButtonDisplayOriginalImage();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonClose();
	DECLARE_EVENTSINK_MAP()
	void SelectParameterNumDlgShowInt();
	void SelectParameterNumDlgShowDouble();
	void SelectParameterCheck();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonBlobsSearch();
	afx_msg LRESULT OnImageViewToTeachingMessage(WPARAM para0, LPARAM para1);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnBnClickedButtonBlobsPreProcess();
	afx_msg void OnBnClickedButtonSetScanSpeed();
	afx_msg void OnBnClickedButtonBlobsMasking();
	afx_msg void OnBnClickedButtonBlobsSearchAreaStart();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_slider_Min;
	CSliderCtrl m_slider_Max;
	void ClickBtnenhctrlMinGrayHisto();
	void ClickBtnenhctrlMaxGrayHisto();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonInspTypeDown();
	afx_msg void OnBnClickedButtonInspTypeUp();
	int m_edit_nInspTypeIndex;
};
