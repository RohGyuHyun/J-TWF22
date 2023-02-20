#pragma once
#include <Matrox.h>
#include "afxwin.h"
#include <UniUtility.h>
#include "atltypes.h"
#include <vidi_runtime.h>
#include <vidi_runtime_legacy.h>

class CCompo;
class CCompoTracker;
class CSCFidAlgo;
class CLineEquation;
class CSCResult;

#ifndef _NOVIDI
#pragma comment(lib,"vidi_41.lib")
#endif

// CCamDlg ��ȭ �����Դϴ�.

struct tScoreSet
{
	double dblThresholdHight;
	double dblThresholdLow;
	double dblScore;
	int nDefect;
};

class CCamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCamDlg)

public:
	enum tCam { CAMTYPE_BOTTOM, CAMTYPE_TOP, CAMTYPE_LEFTSIDE, CAMTYPE_RIGHTSIDE };
	TCHAR m_szChName[4][20] = { _T("BOTTOM"), _T("TOP"), _T("LEFTSIDE"), _T("RIGHTSIDE") };
	char m_szStream[4][20] = { "CGBOTTOM", "CGTOP0", "CGTOP1","CGTOP2" };
	CCamDlg(tCam nTypeCam, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
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
	VIDI_IMAGE m_vidiCamImg[3];  //splitImage�� vidi�̹����� ���ս�Ŵ

	//CMilBuffer *m_pMilCamDraw; //Graph�۾����� �ϱ� ����
	CMilBuffer *m_pMilCamProc;  //�۾������� �ӽÿ�
	CMilBuffer *m_ppSplitLineImg[3]; //������ ��������, �������, �ݻ翵���� ����� Mil Buffer

	//CMilBuffer *m_pMilPenetration;	
	//CMilBuffer *m_pMilScaterring;	 
	//CMilBuffer *m_pMilReflection;	 
	CMilDisplay *m_pMilDisp;
	CMilDisplay *m_pMilDispDefault;
	CString m_strCamName;
	CString m_strCamTag;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	// �ϳ��� �ɸ��°��� ������ ��� �ɸ��°��� �ϳ��� ������ ����
	int CamViewHitTest(CPoint Point, CCompo * &pFindedCompo);
	BOOL IsTeachingMode();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	CDC *m_pOverlayDC;
	BOOL m_bInitialized;
	CCompo *m_pSelectedCompo;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CdblSize m_dblsizeZoomFactor;
//	CEuresysChannel *m_pEuresysChannel;
	int m_iCamBuildNo;
	int LoadBufImage(CString strFileName, MIL_INT FileFormat=M_BMP);
	int LoadBufImage(CMilBuffer *pImage, int nLight=-1);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	int Inspect(CSCResult *pResult=NULL, int nLight=-1);
	void RefreshView();
	void DrawToView();
	int SaveImage(BOOL bIsNG=TRUE);
	int Grab();
	BOOL IsGrabDone();
	int DoGrab();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	void ClearView();
	void DrawCross(CMilBuffer *pDrawBuffer);
	CRect m_rectFidSearchRange;
	BOOL IsPixelCalibrated();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	static BOOL m_bRun;
	BOOL m_bRequestReconnect;
	int m_nTypeCam;
	void SetCamType(tCam nTypeCam);
	void SetImageSize(int cx, int cy);
	int m_iCAM_HRES;
	int m_iCAM_VRES;
	CGrabChannel *m_pMatroxChannel;
	void SwitchLightImage(int nSelectedLight);
	void SetDataAquMode(BOOL bMode);
	// //�������� 0:�ݻ� 1:��� 2:����
	int m_nSelectedLight;
	void SetRunStop(int OPMode);
	int ParseResult(TCHAR * szData, tScoreSet & dblScore, CPtrArray &listPointGroup);
	void GetPointList(CString strPoints, CPtrList *plistPoint, CdblPoint& dblOffset);
	void GetViewOffset(CString strPoints, CdblPoint& dblOffset);
	void ViDiClose();
	void CombineViDi();
	LRESULT OnReqInsp(WPARAM wparam, LPARAM lparam);
	void SetTopSideInspType(int nInspType);
	int m_nTopSideInspType;
};
