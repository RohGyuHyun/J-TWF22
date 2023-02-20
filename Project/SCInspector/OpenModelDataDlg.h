#pragma once
#include "afxcmn.h"
#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <string.h>
#include "afxwin.h"
#include "RenameModelDataDlg.h"
#include "SCInspector.h"
#include "Status.h"


// COpenModelDataDlg ��ȭ �����Դϴ�.

class COpenModelDataDlg : public CDialog
{
	DECLARE_DYNAMIC(COpenModelDataDlg)

public:
	COpenModelDataDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~COpenModelDataDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OPEN_MODELDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	CStatic m_image;
	static CString m_strModelName;
	static CString m_strAllName;

	CMilBuffer *SavePCBModelBuff;

	virtual BOOL OnInitDialog();

	CMilDisplay *m_pMilDisplay; //Mil ���� �Լ�
	CMilBuffer *m_pDisplayMap; //Mil ���� �Լ�

	void FindModelData(void);
	BOOL DeleteDirectory(CString szDir, int recur);		// recur �� 0�̸� �ڱ�� ����������, 1�̸� �ڱ���� ����
	afx_msg void OnNMClickModelList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnDelete();

	afx_msg void OnBnClickedBtnRename();
	afx_msg void OnBnClickedBtnSelect();

	CStatus m_pStatus;
	
    static COpenModelDataDlg* Instance(void);                 //<�̱��� �ν��Ͻ� �κ�(.)
  	static COpenModelDataDlg* m_pInstance;
	int m_imageWidth;
	int m_imageHeight;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CStatic m_static_NoImage;
};
