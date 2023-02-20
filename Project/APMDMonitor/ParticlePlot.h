#pragma once
#include "atltypes.h"
#include "Status.h"
#include "Repository.h"
#include "MarkToolTip.h"
// CParticlePlot

class CModeDlg;
class CParticlePlot : public CStatic
{
	DECLARE_DYNAMIC(CParticlePlot)

public:
	CParticlePlot();
	virtual ~CParticlePlot();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void DrawAxis(CDC *pUserDC=NULL);
	CDC *m_pUserDC;
	afx_msg void OnPaint();
	void SetRepository(CRepository * pRepository, int nKindOfData=0);
	CRepository *m_pRepository;
	CPtrList *m_pDispData;
	int CalcGrid();
	CRect m_PlotRect;
	CRect m_ClientRect;
	//! Index�� 0������ �����Ѵ�. 0���ε����� 1��
	int m_iXGridSpace; //! pixel������ ȭ��󿡼� X��������� �� pixel��������
	int m_iYGridSpace; //! pixel������ ȭ��󿡼� Y��������� �� pixel��������
	double m_dblXIndexSpace;  //!pixel������ Index���� �Ÿ��� X���� �� pixel��������
	// X������ ��� ������ �׸� �� �ִ���
	int m_iXGridCount;
	int m_iYGridCount;
//	int m_iXTotalIndexCount;
	tViewOptions m_ViewOptions;
	int m_iXStartIndexNo;  //! ȭ����� ���� ���� IndexNo
	int m_iXEndIndexNo;    //! ȭ����� ���� ���� IndexNo
	int m_iCountOfRecords; //! ���� ���ڵ尹�� �ִ� Index����
	int DrawMark(CDC *pUserDC = NULL);
	int PlotViewRect(int CurIndexNo=0, CDC *pUserDC=NULL);
	LRESULT RecordUpdated(WPARAM wparam, LPARAM lparam);
	int SetGridUnit(int iXGridUnit, int iYGridUnit);
	// ȭ��󿡼� X��������� �� �� �ִ� �ִ� Index����
	int m_iXScrnMaxIndexCount;
	int m_iXGridUnit;
	int GetMarkLinePos();
	int GetMarkLineIndex();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	bool m_bDrag;
	CPoint m_ptDragStart;
	int GetIndexNo(CPoint ptToGet);
	int m_iDragStartIndexNo;
	int IndexMoveToPt(int IndexNo, CPoint ptToMove);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void SetCurIndexNo(int curIndexNo);
	CModeDlg *m_pModeDlg;
	void SetModeDlg(CModeDlg * pModelDlg);
	int GetCurIndexNo();
protected:
	int m_curIndexNo;  // !���������� Index��-Navi�� ���� �����Ǿ����� ��

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int DrawMarkToolTip(CPoint curPoint);
	LRESULT OnMarkToolTip(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void MakeHorUnitName(int iSec, CString & strUnitName);
	int PlotCompareViewRect(int CurIndexNo, CDC * pUserDC, CPtrList * pLstData);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int m_nKindOfData;//! 0:Main 1:Compare��
};


