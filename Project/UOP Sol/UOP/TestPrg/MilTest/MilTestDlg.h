
// MilTestDlg.h : ��� ����
//

#pragma once
#include <Matrox.h>
#include <UniUtility.h>

#include "ViewDlg.h"
#include "afxwin.h"
#include "ScrnHistoDlg.h"
#include "afxcmn.h"
// CMilTestDlg ��ȭ ����
class CMilTestDlg : public CDialog
{
// �����Դϴ�.
public:
	CMilTestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MILTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

public:
	CMilBuffer *m_pImage;
	CMilBuffer *m_pAddImages;
	CMilBuffer *pImgData;
	CMilContext ctxtFirst;

	CMilVLib myMilTest;
	CMilDisplay *m_pMyDisplay;
	CMilDisplay *m_pMySecondDisplay;
	CMilIm *pMyImageProcess;
	CMilApplication m_miIDApplication;
	CViewDlg *m_pViewDlg;
	double *m_pOrgHistoData;
	double *m_pDestHistoData;

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnLoadimage();
	afx_msg void OnBnClickedBtnSelect();
	afx_msg void OnBnClickedBtnSelwnd();
	afx_msg void OnBnClickedBtnBlob();
	afx_msg void OnBnClickedBtnImclose();
	afx_msg void OnBnClickedBtnProc();
	afx_msg void OnBnClickedBtnBin();
	afx_msg void OnBnClickedBtnGetattr();
	afx_msg void OnBnClickedBtnResize();
	CStatic m_ImageView;
	afx_msg void OnBnClickedBtnPut();
	short *m_pRawData;
	afx_msg void OnBnClickedBtnHistogram();
	CScrnHistoDlg* m_pScrnHistoDlg;
	afx_msg void OnBnClickedBtnGmf();
	afx_msg void OnBnClickedBtnClipimg();
	CStatic m_imgSpplement;
	afx_msg void OnBnClickedBtnFind();
	afx_msg void OnBnClickedAvr();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnlvlwnd();
	afx_msg void OnBnClickedBtnhistotest();
	afx_msg void OnBnClickedBtnGetmaxmin();
	afx_msg void OnBnClickedGmfSave();
	afx_msg void OnBnClickedImport();
	afx_msg void OnBnClickedZoom();
	afx_msg void OnBnClickedBtnPan();
	afx_msg void OnBnClickedBtnShowcontext();
	BOOL m_bUseAutoScale;
	afx_msg void OnBnClickedChkAutoscale();
	CEdit m_ctlBinValue;
	UINT m_uiBinValue;
	CSpinButtonCtrl m_ctlSPBinValue;
	BOOL m_bFillWindow;
	afx_msg void OnBnClickedChkFillwnd();
	afx_msg void OnBnClickedCalcord();
	afx_msg void OnBnClickedBtnjpgsave();
	afx_msg void OnBnClickedBtnLoadrawimage();
	int m_SaveType;
};
