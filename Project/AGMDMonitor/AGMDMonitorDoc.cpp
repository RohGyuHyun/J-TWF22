
// AGMDMonitorDoc.cpp : CAGMDMonitorDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "AGMDMonitor.h"
#endif

#include "AGMDMonitorDoc.h"
#include "Status.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAGMDMonitorDoc

IMPLEMENT_DYNCREATE(CAGMDMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CAGMDMonitorDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CAGMDMonitorDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CAGMDMonitorDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVEASLOGFILE, &CAGMDMonitorDoc::OnFileSaveaslogfile)
END_MESSAGE_MAP()


// CAGMDMonitorDoc ����/�Ҹ�

CAGMDMonitorDoc::CAGMDMonitorDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CAGMDMonitorDoc::~CAGMDMonitorDoc()
{
}

BOOL CAGMDMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CAGMDMonitorDoc serialization

void CAGMDMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CAGMDMonitorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void CAGMDMonitorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAGMDMonitorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAGMDMonitorDoc ����

#ifdef _DEBUG
void CAGMDMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAGMDMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAGMDMonitorDoc ���


void CAGMDMonitorDoc::OnFileOpen()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog FileLoad(TRUE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG|Files(*.*)|*.*|"));
	if (FileLoad.DoModal() == IDOK)
	{
		m_Repository.ReadFromLogFile(FileLoad.GetPathName());
		tAGMDRecord *pMeaInfo = m_Repository.GetMeasureInfo(m_Repository.m_iDispIndex);
		CStatus::Instance()->SetMeasureData(pMeaInfo);
		CStatus::Instance()->OnInitBrowserUpdate();
		CStatus::Instance()->UpdateScrn();
	}
}

BOOL CAGMDMonitorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return FALSE;
	//return CDocument::OnSaveDocument(lpszPathName);
}



void CAGMDMonitorDoc::OnFileSaveaslogfile()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	int nCount = CStatus::Instance()->GetRepository()->m_DataRecords.m_AGMDRecords.GetSize();
	if (nCount == 0)
	{
		AfxMessageBox(_T("There is no Data."));
		return;
	}
		
	CFileDialog dlg(FALSE, _T("*.LOG"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("LOG File|*.LOG|Files(*.*)|*.*|"));
	//CFileDialog dlg(FALSE);
	if (dlg.DoModal() == IDOK)
	{	
		CStatus::Instance()->GetRepository()->NewMakeLogFile(dlg.GetPathName());
	}
}
