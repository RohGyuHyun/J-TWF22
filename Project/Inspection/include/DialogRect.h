#pragma once
#include "Inspection.h"
#include "_CtrlTracker.h"

#define		TRACKER_MAX_NUM				10
#define		USER_MSG_IMG_RECT			WM_USER+100


#define DEFAULT_ZOOM_RATIO	0.045//0.162//0.045
#define MAIN_ZOOM_RATIO	1.317//0.248
// CDialogRect 대화 상자입니다.

class CDialogRect : public CDialog
{
	DECLARE_DYNAMIC(CDialogRect)

public:
	CDialogRect(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogRect();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RECT };

private:
	CList<C_CtrlTracker, C_CtrlTracker&> m_Tracker;

	HWND m_MainHwnd;
	UINT m_MessageID;

public:
	BOOL m_bDbClickUse;
	BOOL m_bActive;
	CRect m_WinRect;
	int m_iActiveIdx;
	double m_dZoomRatio;
	double m_dDefaultZoomRatio;
	COLORREF m_curColor;

	int m_iImgWd;
	int m_iImgHt;

	int m_iDlgIdx;

	int m_iTrackerNum;

	long m_lPanSX;
	long m_lPanSY;
	long m_lPanEX;
	long m_lPanEY;
	BOOL m_bPanDN;

	double m_dPX;
	double m_dPY;

	double m_dZoomRat;
	double m_dZoomRatOld;
	double m_dZoomSX;
	double m_dZoomSY;

	BOOL m_bMainDlg;

	int GetEnable();
	void SetEnable(int idx, BOOL enable);
	void SetTrackerNum(int i);
	void SetDlgIdx(int idx);
	double GetZoom();
	void SetZoom(double ratio, long img_wd, long img_ht);
	HWND GetWinHwnd();
	void GetTracker(int idx, double *left, double *top, double *right, double *bottom);
	void SetTracker(int idx, double left, double top, double right, double bottom);
	void SetActiveTracker(int idx, BOOL active);
	void ZoomDisplay();
	void ZoomTracker(CRect *rect, double o_rat);

	//Image display
	LRESULT OnImageRectMessage(WPARAM para0, LPARAM para1);
	BOOL CheckTracker(int idx, CPoint point);

	void SetActiveImageRect(BOOL active) { m_bActive = active; };
	void DeleteAll();
	void AddTracker(int type, CRect rect, COLORREF color, char* cap);
	void SetImageSize(int nWidth, int nHeight) { m_lPanEX = nWidth; m_lPanEY = nHeight; };
	void SetMainMessage(HWND mainHwnd, UINT nMessgeID) { m_MainHwnd = mainHwnd; m_MessageID = nMessgeID; };
	void SetRect(CRect rect) { m_WinRect = rect; };
#ifdef MIL_USE
	void SetPointGrayValueDraw(CMilDisplay* milDisplay, CPoint pPoint, int nVal);
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
