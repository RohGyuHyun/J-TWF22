#pragma once
#include <Matrox.h>
#include "afxwin.h"
#include <UniUtility.h>
#include "atltypes.h"
#include "SCSocket.h"

class CCompo;
class CCompoTracker;
class CSCFidAlgo;
class CLineEquation;
class CSCResult;


// CCamDlg ��ȭ �����Դϴ�.

class CCamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCamDlg)

public:
	CCamDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCamDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
//	CMilBuffer m_milView;
	virtual BOOL OnInitDialog();
	CMilBuffer *m_pMilCamView; //���̱� ���� ���۷� Select�Ȱ���  �׷����۾����� �ϸ� ������ CamDraw�� ī���ؼ� ���̴� �뵵�θ� ����ؾߵ� 
	CMilBuffer *m_pMilCamDraw; //Graph�۾����� �ϱ� ����
	CMilBuffer *m_pMilCamProc;
	CMilDisplay *m_pMilDisp;
	CMilDisplay *m_pMilDispDefault;
	CString m_strCamName;
	int m_inspStage;
	CString m_strCamTag;
	CSCSocket * m_pSockContainer;
	static UINT ThreadSendRemote(LPVOID pParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	// �ϳ��� �ɸ��°��� ������ ��� �ɸ��°��� �ϳ��� ������ ����
	int CamViewHitTest(CPoint Point, CCompo * &pFindedCompo);
	LRESULT OnSendFN2Remote(WPARAM wparam, LPARAM lparam);
	LRESULT OnRequstReconnect(WPARAM wparam, LPARAM lparam);
	CPtrList * GetCamList();
	BOOL IsTeachingMode();
	void AddCompoForView(CPoint Point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void DrawAllTrack(CDC * pDC);
	void DrawAllTrack();
	int ClearOverlay();
	CDC *m_pOverlayDC;
	CPtrList * AddCamPart();
	void FreeOVLDC();
	void SetOVLDC();
	BOOL m_bInitialized;
	CCompo *m_pSelectedCompo;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void DeleteSelCompo();
	CdblSize m_dblsizeZoomFactor;
	CPoint GetMapedPoint(CPoint point);
	void GetMapedRect(CRect *pOrgRect);
	int Draw(CDC * pDC, CCompoTracker *pCompoTracker);
	CEuresysChannel *m_pEuresysChannel;
	int m_iCamBuildNo;
	int LoadBufImage(CString strFileName, MIL_INT FileFormat=M_BMP);
	int LoadBufImage(CMilBuffer *pImage);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	int Inspect(CSCResult *pResult=NULL);
	void RefreshView();
	BOOL IsThereFid();
	void DrawToView();
	int SaveImage(BOOL bIsNG=TRUE);
	int Grab();
	BOOL IsGrabDone();
	int DoGrab();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	int FindOffset(CdblPoint * pDblOffset, CMilBuffer *pMilBuffProc, CMilBuffer *pMilBuffDraw);
	CCompo * FindFid();
	int GetFidROI(CRect *rectFidROI);
	CMilBuffer * GetROIChild(CSCFidAlgo *pFidAlgo);
	void ClearView();
	BOOL IsThereHEDet();
	int FindHorizontalEdge(CLineEquation *pLineEquation, CdblPoint * pDblOffset, CMilBuffer *pMilBuffProc, CMilBuffer *pMilBuffDraw);
	CCompo * FindHEdge();
	void DrawCross(CMilBuffer *pDrawBuffer);
	CRect m_rectFidSearchRange;
	BOOL IsPixelCalibrated();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int SendFN2Remote(CString strFileName);
	static BOOL m_bRun;
	int InitRemoteCam();
	int ReconnectRemoteCam();
	BOOL m_bRequestReconnect;
};
