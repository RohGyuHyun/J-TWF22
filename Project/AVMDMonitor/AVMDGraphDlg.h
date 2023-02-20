#pragma once
#include "DrawingHelper.h"
#include "afxwin.h"

// CAVMDGraphDlg ��ȭ �����Դϴ�.

class CAVMDGraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAVMDGraphDlg)

public:
	CAVMDGraphDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CAVMDGraphDlg(CString strDegree, int nInputType);// nIsCompareGraph = 0 : Org Graph, 1: Compare Graph
	virtual ~CAVMDGraphDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAPH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
private:
	int m_iStandardValue;
	int m_icsrMAxValue;
	int m_iScrClickNum;
	bool bReEnter;
	CRect m_ClienRect;
	CRect m_OldClientRect;
	SCROLLINFO m_scrInfo;
	bool m_bIsDownClick;

public:
	CDC m_MemDC;
	CDC* m_pDC;
	CBitmap* pOldBitmap;
	CDrawingHelper m_drawHelper;
	CString m_strGraphType;
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	void MakeGraph();
	void DrawView();
	BOOL m_bIsReSizing;
	BOOL m_bToRepaint;
	void MakeReviewGraph();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
	int m_iMouseStart;
	int m_iMouseEnd;
	int m_nInputType; // 0 : Org Graph, 1: Compare Graph
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
