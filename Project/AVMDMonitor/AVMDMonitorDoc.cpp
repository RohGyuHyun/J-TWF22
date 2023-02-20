
// AVMDMonitorDoc.cpp : CAVMDMonitorDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "AVMDMonitor.h"
#include "Status.h"
#endif

#include "AVMDMonitorDoc.h"
#include "MainFrm.h"
#include <UniUtility.h>
#include <propkey.h>
#include "AVMDMonitorView.h"
#include "ProgressBarDlg.h"
#include "Status.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAVMDMonitorDoc

IMPLEMENT_DYNCREATE(CAVMDMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CAVMDMonitorDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CAVMDMonitorDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_LOADCOMPAREDATA, &CAVMDMonitorDoc::OnFileLoadcomparedata)
	ON_COMMAND(ID_FILE_UPDATE, &CAVMDMonitorDoc::OnFileUpdate)
	ON_COMMAND(ID_FILE_SAVEAS, &CAVMDMonitorDoc::OnFileSaveas)
	ON_COMMAND(ID_SAVE_SETTINGS_AS, &CAVMDMonitorDoc::OnSaveSettingsAs)
	ON_COMMAND(ID_FILE_LOADSETTINGSFROM, &CAVMDMonitorDoc::OnFileLoadsettingsfrom)
	ON_COMMAND(ID_FILE_SAVECURRENTFILTEREDCSV, &CAVMDMonitorDoc::OnFileSavecurrentfilteredcsv)
END_MESSAGE_MAP()


// CAVMDMonitorDoc ����/�Ҹ�

CAVMDMonitorDoc::CAVMDMonitorDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CAVMDMonitorDoc::~CAVMDMonitorDoc()
{
}

BOOL CAVMDMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CAVMDMonitorDoc serialization

void CAVMDMonitorDoc::Serialize(CArchive& ar)
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
void CAVMDMonitorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CAVMDMonitorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAVMDMonitorDoc::SetSearchContent(const CString& value)
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

// CAVMDMonitorDoc ����

#ifdef _DEBUG
void CAVMDMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAVMDMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAVMDMonitorDoc ���

void CAVMDMonitorDoc::OnFileOpen()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog FileLoad(TRUE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG|Files(*.*)|*.*|"));
	if (FileLoad.DoModal() == IDOK)
	{
		CStatus::Instance()->ReviewDataClear(TRUE, FALSE);
		m_Repository.ReadFromLogFile(FileLoad.GetPathName(), 0);
		//CStatus::Instance()->SetFilterType(FALSE, FALSE, FALSE);
		//CStatus::Instance()->m_strFilterStatus = _T("disable");
		CStatus::Instance()->SetReviewDragReset(0);
		CStatus::Instance()->UpdateReviewComboBox(0);

		CString strFileName = FileLoad.GetFileName();
		CStatus::Instance()->m_strCurLogFile = strFileName;
		this->SetTitle(strFileName);
		CStatus::Instance()->SetMarkClickEvent(FALSE, 0);

		if (CStatus::Instance()->IsFilterUse())
		{
			CStatus::Instance()->SetProgressBarMode(CStatus::tProgressBarMode::FILTER);
			CStatus::Instance()->m_nProgressBarCount = 0;
			CProgressBarDlg *pProgressDlg = new CProgressBarDlg;
			CStatus::Instance()->ReviewDataClear(TRUE, TRUE);
			CStatus::Instance()->ReviewDataFiltering();
			pProgressDlg->DoModal();
		}
		CStatus::Instance()->UpdateScrn();
	}
}

void CAVMDMonitorDoc::OnFileLoadcomparedata()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog FileLoad(TRUE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG|Files(*.*)|*.*|"));
	if (FileLoad.DoModal() == IDOK)
	{
		CStatus::Instance()->ReviewDataClear(FALSE, TRUE);
		m_Repository.ReadFromLogFile(FileLoad.GetPathName(), 1);
		
		//CStatus::Instance()->SetFilterType(FALSE, FALSE, FALSE);
		CStatus::Instance()->SetReviewDragReset(1);
		CStatus::Instance()->UpdateReviewComboBox(1);
		CString strPathName = FileLoad.GetPathName();
		CString strFileName = FileLoad.GetFileName();
		strPathName.TrimRight(_T("\\") + strFileName);
		CString strFullName;

		strFullName = strPathName + _T("\\") + strFileName;
		CStatus::Instance()->m_strCompareDataFileName = strFullName;
		CStatus::Instance()->SetMarkClickEvent(FALSE, 1);

		if (CStatus::Instance()->IsFilterUse())
		{
			CStatus::Instance()->SetProgressBarMode(CStatus::tProgressBarMode::COMPARE_FILTER);
			CStatus::Instance()->m_nProgressBarCount = 0;
			CProgressBarDlg *pProgressDlg = new CProgressBarDlg;
			CStatus::Instance()->ReviewDataClear(TRUE, TRUE);
			CStatus::Instance()->ReviewDataFiltering();
			pProgressDlg->DoModal();

		}
		//CStatus::Instance()->UpdateScrn();
		CMainFrame  *pFrame = (CMainFrame*)AfxGetMainWnd();
		CheckMenuItem(::GetMenu(pFrame->m_hWnd), ID_VIEW_COMPAREDATA, MF_CHECKED);
		CStatus::Instance()->SetCompareCondition(TRUE);
		CStatus::Instance()->GraphUpdate();

		CStatus::Instance()->UpdateScrn();
	}
}


BOOL CAVMDMonitorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CDocument::OnSaveDocument(lpszPathName);
}


void CAVMDMonitorDoc::OnFileUpdate()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_Repository.ReSaveTagFile(0);
}




void CAVMDMonitorDoc::OnFileSaveas()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog dlg(FALSE);

	if (dlg.DoModal()==IDOK)
	{
		CStatus::Instance()->SetProgressBarMode(CStatus::tProgressBarMode::SAVE_AS);
		CStatus::Instance()->m_nProgressBarCount = 0;
		CProgressBarDlg *pProgressDlg = new CProgressBarDlg;
		CString strFilePath = dlg.GetPathName();
		AfxBeginThread(SaveAsThread, &strFilePath);
		pProgressDlg->DoModal();
		//m_Repository.ExportDataFile(&strFilePath);
	}
}

void CAVMDMonitorDoc::OnSaveSettingsAs()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog dlg(FALSE, _T("*.CFG"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CFG File|*.CFG|Files(*.*)|*.*|"));

	if (dlg.DoModal() == IDOK)
	{
		CStatus::Instance()->ExportSetting(dlg.GetPathName());
	}
}


void CAVMDMonitorDoc::OnFileLoadsettingsfrom()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog dlg(TRUE, _T("*.CFG"), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T("CFG File|*.CFG|Files(*.*)|*.*|"));

	if (dlg.DoModal() == IDOK)
	{
		CStatus::Instance()->GetSettingFrom(dlg.GetPathName());
	}

}


void CAVMDMonitorDoc::OnFileSavecurrentfilteredcsv()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog dlg(FALSE, _T("*.CSV"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CSV File|*.CSV|Files(*.*)|*.*|"));
	{
		if (dlg.DoModal() == IDOK)
		{
			CStatus::Instance()->SetProgressBarMode(CStatus::tProgressBarMode::EXPORT_EXCEL);
			CStatus::Instance()->m_nProgressBarCount = 0;
			CProgressBarDlg *pProgressDlg = new CProgressBarDlg;
			CString strFilePath = dlg.GetPathName();
			AfxBeginThread(ProcessThread, &strFilePath);
			pProgressDlg->DoModal();
		}
	}
}

UINT CAVMDMonitorDoc::ProcessThread(LPVOID lParam)
{
	CString *strFileName = (CString*)lParam;
	if (CStatus::Instance()->IsFilterUse())
	{
		CStatus::Instance()->GetRepository()->ExportFilteredCSVFile(*strFileName, 0);
	}
	else
	{
		CStatus::Instance()->GetRepository()->ExportCSVFile(*strFileName, -1);
	}
	CStatus::Instance()->m_bIsThreadStop = FALSE;
	return 0;
}

UINT CAVMDMonitorDoc::SaveAsThread(LPVOID lParam)
{
	CString *strFilePath = (CString *)lParam;
	CStatus::Instance()->GetRepository()->ExportDataFile(*strFilePath);
	CStatus::Instance()->m_bIsThreadStop = FALSE;
	return 0;
}