#pragma once
#include "atltypes.h"

#define WM_HISTO_UPDATE WM_USER+45

// CHistoHandle ��ȭ �����Դϴ�.
class CHistoHandle : public CButton
{
	DECLARE_DYNAMIC(CHistoHandle)

public:
	CHistoHandle();   // ǥ�� �������Դϴ�.
	virtual ~CHistoHandle();


protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	CPoint m_centerPos;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetHistoHDLRect(CRect * pHistoHDLRect);
	CRect m_HistoHDLRect;
	void SetPos(int iPos);
	int GetPos();
};
