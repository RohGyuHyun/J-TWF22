
// APMDMonitorDoc.h : CAPMDMonitorDoc Ŭ������ �������̽�
//


#pragma once


class CAPMDMonitorDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CAPMDMonitorDoc();
	DECLARE_DYNCREATE(CAPMDMonitorDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:
	CRepository m_Repository;

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
	virtual ~CAPMDMonitorDoc();
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
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnLoadComparedata();
};
