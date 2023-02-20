#pragma once
#include "DrawingHelper.h"

// CAVMDTimeDlg ��ȭ �����Դϴ�.

class CAVMDTimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAVMDTimeDlg)

public:
	CAVMDTimeDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CAVMDTimeDlg(CString strDegree, double unit, BOOL bIsFFTDlg, int nInputType);
	virtual ~CAVMDTimeDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TIME};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
private:
	CString strDegree;
	double dblUnit;
	BOOL bIsFFT;
public:
	CDrawingHelper m_drawHelper;
	
	CString strFFTdegree;
	afx_msg void OnPaint();
private:
	int m_nInputType;
	CDC m_MemDC;
	CDC* m_pDC;
	CBitmap* pOldBitmap;
	CRect m_ClienRect;
	CRect m_OldClientRect;
	BOOL m_bToRepaint;
public:
	void MakeTimeUnit();
	void DrawView();
};
