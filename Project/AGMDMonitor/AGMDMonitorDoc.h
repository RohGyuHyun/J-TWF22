
// AGMDMonitorDoc.h : CAGMDMonitorDoc Ŭ������ �������̽�
//


#pragma once
#include "Repository.h"

class CAGMDMonitorDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CAGMDMonitorDoc();
	DECLARE_DYNCREATE(CAGMDMonitorDoc)

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
	virtual ~CAGMDMonitorDoc();
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
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

	afx_msg void OnFileSaveaslogfile();
};
