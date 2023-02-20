
// AVMDMonitorDoc.h : CAVMDMonitorDoc Ŭ������ �������̽�
//

#include "Repository.h"
#pragma once

class CAVMDMonitorDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CAVMDMonitorDoc();
	DECLARE_DYNCREATE(CAVMDMonitorDoc)

// Ư���Դϴ�.
public:
	
// �۾��Դϴ�.
public:
// �������Դϴ�.
	
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CAVMDMonitorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	CRepository m_Repository;
	afx_msg void OnFileOpen();
	afx_msg void OnFileLoadcomparedata();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileUpdate();
	afx_msg void OnFileSaveas();
	afx_msg void OnSaveSettingsAs();
	afx_msg void OnFileLoadsettingsfrom();
	afx_msg void OnFileSavecurrentfilteredcsv();
	static UINT ProcessThread(LPVOID lParam);
	static UINT SaveAsThread(LPVOID lParam);
};
