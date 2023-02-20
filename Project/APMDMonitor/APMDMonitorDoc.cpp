
// APMDMonitorDoc.cpp : CAPMDMonitorDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "APMDMonitor.h"
#endif

#include "APMDMonitorDoc.h"

#include <propkey.h>

#include "MainFrm.h"
#include "APMDMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAPMDMonitorDoc

IMPLEMENT_DYNCREATE(CAPMDMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CAPMDMonitorDoc, CDocument)
	ON_COMMAND(ID_LOAD_COMPAREDATA, &CAPMDMonitorDoc::OnLoadComparedata)
END_MESSAGE_MAP()


// CAPMDMonitorDoc ����/�Ҹ�

CAPMDMonitorDoc::CAPMDMonitorDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CAPMDMonitorDoc::~CAPMDMonitorDoc()
{
}

BOOL CAPMDMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CAPMDMonitorDoc serialization

void CAPMDMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
//?		m_Repository.ReadFromLogFile(&ar);
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CAPMDMonitorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CAPMDMonitorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAPMDMonitorDoc::SetSearchContent(const CString& value)
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

// CAPMDMonitorDoc ����

#ifdef _DEBUG
void CAPMDMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAPMDMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAPMDMonitorDoc ���


BOOL CAPMDMonitorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	m_Repository.ReadFromLogFile(lpszPathName);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CAPMDMonitorView *pView = (CAPMDMonitorView *)pFrame->GetActiveView();

	pView->m_pGraphDlg->m_cNavi.SetRangeMax(m_Repository.m_DataRecords[0].nCountOfRecords, true);
	pView->m_pGraphDlg->m_cNavi.SetPos(1);
	pView->m_pGraphDlg->m_cParticlePlot.SetCurIndexNo(0);
	pView->m_pGraphDlg->PosTextOut();
	pView->m_pViewOptionDlg->UpdateMarkView();
	pView->Invalidate();

	return FALSE;
//	return TRUE;//UI�� ������ �ӽ÷� �ٲ�
}


BOOL CAPMDMonitorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
//	m_Repository.TestFileWrite();

	m_Repository.ExportCSVFile(lpszPathName);
	return true;
//	return CDocument::OnSaveDocument(lpszPathName);
}


void CAPMDMonitorDoc::OnLoadComparedata()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog cmpFileLoad(TRUE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG;*.CSV;|Files(*.*)|*.*|"));

	if (cmpFileLoad.DoModal() == IDOK)
	{
		m_Repository.LoadCompareData(cmpFileLoad.GetPathName());
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CAPMDMonitorView *pView = (CAPMDMonitorView *)pFrame->GetActiveView();
		pView->m_pGraphDlg->m_cComparePlot.SetCurIndexNo(0);
		pView->Invalidate();
		pView->DemendComparedataView();
	}
}
