#pragma once
#include "DrawingHelper.h"

// CSenSorViewDlg ��ȭ �����Դϴ�.

class CSenSorViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSenSorViewDlg)

public:
	CSenSorViewDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSenSorViewDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENSORVIEWDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CDC m_MemDC;
	CDC* m_pDC;
	CBitmap* m_pOldBitmap;
	afx_msg void OnPaint();
	CDrawingHelper m_drawHelper;
	LRESULT afx_msg OnUpdateSensorView(WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
