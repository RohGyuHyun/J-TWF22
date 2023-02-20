#pragma once
#include "afxdialogex.h"
#include "MyListCtrl.h"

// CDialogNgView 대화 상자

class CDialogNgView : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogNgView)

public:
	CDialogNgView(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogNgView();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NG_VIEW };
#endif
protected:
	CMilDisplay* m_milMainDisplay;
	CMilBuffer* m_milMainImage;
	CMilDisplay* m_milZoomDisplay;
	CMilBuffer* m_milZoomImage;

	CMyListCtrl m_list_Ng;

	vector<BLOB_RESULT_PARA> m_ResultData;
	CSize m_ImageSize;
	vector<CString> m_ResultList;
	int m_nSelectResultDataIdx;
	int m_nSelectResultZoomDataIdx;
	CString m_strSelectResultFileNmae;

	BOOL m_bResult;
	double m_dPixelSizeX;
	double m_dPixelSizeY;
	CString m_strCurrentRecipe;
private:
	void ReadNgData();
	BOOL ReadData(CString strFilePath);
	void ReadImage(CString strFilePath);
	void ResultDisplay();
	void ZoomResultDisplay(int nIdx);
	void InitNgList();
public:
	void SetImageSize(CSize size_image) { m_ImageSize = size_image; };
	void SetInsertResultList(CString strResultName);
	void SetPixelSize(double dX, double dY) { m_dPixelSizeX = dX; m_dPixelSizeY = dY; };
	void SetRecipeName(CString strRecipe) { m_strCurrentRecipe.Format(_T("%s"), strRecipe); };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnNMClickListNg(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonClose();
	CListCtrl m_list_ResultList;
	afx_msg void OnNMClickListResult(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnKeydownListResult(NMHDR* pNMHDR, LRESULT* pResult);
};
