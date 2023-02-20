#pragma once
#include "SCButton.h"
#include <Matrox.h>

// CToolPanelDlg ��ȭ �����Դϴ�.
struct tManSaveDuration
{
	CTime	tmStart;
	int iMinTimeDuration;
};

class CToolPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CToolPanelDlg)

public:
	CToolPanelDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CToolPanelDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLPANEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnMansaveimg();
	afx_msg void OnBnClickedBtnMansavavi();
	CSCButton m_cManSavImg;
	CSCButton m_cManSavAVI;
	tExportAVI m_ExportAVITag;
	tExportAVI m_ExportAutoAVITag;
	afx_msg void OnBnClickedBtnDevsave();
	CSCButton m_cTiffSave;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	LRESULT OnImgAutoSaveTime(WPARAM wparam, LPARAM lparam);
	LRESULT OnImgAutoSaveTemp(WPARAM wparam, LPARAM lparam);
	LRESULT OnAVIAutoSaveStart(WPARAM wparam, LPARAM lparam);
	LRESULT OnAVIAutoSaveStop(WPARAM wparam, LPARAM lparam);
	LRESULT OnAVIAutoSaveReStart(WPARAM wparam, LPARAM lparam);
	LRESULT OnAVIManSaveStart(WPARAM wparam, LPARAM lparam);
	LRESULT OnAVIManSaveStop(WPARAM wparam, LPARAM lparam);
	static UINT ThreadAVIManSave(LPVOID pParam);
	void AVIManSaveStarter();
	virtual BOOL OnInitDialog();
	BOOL m_bAVIMan;
	tManSaveDuration m_AVIManSaveDuration;
};
