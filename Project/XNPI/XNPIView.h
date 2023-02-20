
// XFMSView.h : CXNPIView Ŭ������ �������̽�
//

#pragma once
#include "atltypes.h"
#include "ModbusManager.h"

class CCamDlg;
class COpPanelDlg;
class CTeachingDlg;
class CStatusPanelDlg;
class CGraphDlg;
class CHistoLevellingDlg;
class CToolPanelDlg;
class CSnapshotDlg;

class CXNPIView : public CView
{
protected: // serialization������ ��������ϴ�.
	CXNPIView();
	DECLARE_DYNCREATE(CXNPIView)

	// Ư���Դϴ�.
public:
	CXNPIDoc* GetDocument() const;

	// �۾��Դϴ�.
public:

	// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// �����Դϴ�.
public:
	virtual ~CXNPIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	int MakeChildWnd();
	CCamDlg *m_pCamDlg[4];
	COpPanelDlg *m_pOpPanelDlg;
	CTeachingDlg *m_pTeachingDlg;
	int WndRepositioning();
	CRect CameraDlgPos(int nPos, CRect rectMain, int widthCam, int heightCam, int xOffset = 0);
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetOPMode(int OPMode);
	BOOL IsTeachingMode();
	CRect m_rectCamFull;
	int m_nCountOfCam;
	CStatusPanelDlg *m_pStatusPanelDlg;
	void DoTrial();
	int LoadDefaultJob();
	int CloseAll();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//CGraphDlg *m_pGraphDlg;
	CHistoLevellingDlg *m_pHistoLevellingDlg;
	afx_msg void OnToolHistowindowlevel();
	afx_msg void OnUpdateToolHistowindowlevel(CCmdUI *pCmdUI);
	CToolPanelDlg *m_pToolPanelDlg;

	//kiy ------------------------------------------------------
	CSnapshotDlg* m_pSnapshotDlg;
	CStdioFile m_infoFileManager;
	CModbusManager* m_mdManager;

	int m_nCurrentRetryCnt;
	void SocketOnReceive(int transactionID, byte functionCode, int rcvData);

	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	HANDLE m_hReceiveDataFromPLCEvent;
	int m_nPlcSendcmdNum;
	
	BOOL m_bStartCmdFlag;
	BOOL m_bEndCmdFlag;
	BOOL m_bTriggerCmdFlag;

	clock_t m_tRStartCMDStart, m_tRStartCMDEnd;
	clock_t m_tREndCMDStart, m_tREndCMDEnd;
	clock_t m_tRTriggerCMDStart, m_tRTriggerCMDEnd;

	int m_nPrevTriggerNum;
	SYSTEMTIME m_tCur_time; //190327 - ����ð� �и��� ������ ���ϱ�(�α� ���λ翡 ���� �뵵)
	int m_nInvalidInterval_ms; //190327 - ���� CMD_INVAILD_INTERVAL_MS ��� - config���Ͽ��� �о���� �����߱� �����̴�.
	//----------------------------------------------------------
};

#ifndef _DEBUG  // SCInspectorView.cpp�� ����� ����
inline CXNPIDoc* CXNPIView::GetDocument() const
{
	return reinterpret_cast<CXNPIDoc*>(m_pDocument);
}
#endif

