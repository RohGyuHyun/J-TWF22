// CDialogAlarmLogView.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogAlarmLogView.h"

int _GetFindCharCount(CString parm_string, char parm_find_char)
{
	int nLen = parm_string.GetLength();
	int find_cnt = 0;

	for (int i = 0; i < nLen; i++)
	{
		if (parm_string[i] == parm_find_char)
		{
			find_cnt++;
		}
	}

	return find_cnt;
}

// CDialogAlarmLogView 대화 상자

IMPLEMENT_DYNAMIC(CDialogAlarmLogView, CDialogEx)

CDialogAlarmLogView::CDialogAlarmLogView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ALARM_LOG_VIEW, pParent)
{
	for (int i = 0; i < 8; i++)
	{
		m_nMonthAlarmCount[i] = 0;
		m_nDayAlarmCount[i] = 0;
	}

	m_pLogReadThread = NULL;
	m_bLogReadThreadEnd = FALSE;
	m_nSelectManu = 0;
}

CDialogAlarmLogView::~CDialogAlarmLogView()
{
	if (GetLogReadThreadEnd())
	{
		if (m_pLogReadThread)
			delete m_pLogReadThread;
		m_pLogReadThread = NULL;
	}
}

void CDialogAlarmLogView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MONTH_ALARM, m_ctr_list_Month_Alarm);
	DDX_Control(pDX, IDC_LIST_DAY_ALARM, m_ctr_list_Day_Alarm);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DAY_SELECT, m_ctrl_DaySelect);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DAY_SELECT2, m_ctrl_DaySelect2);
	DDX_Control(pDX, IDC_LIST_DAY_ALARM_LOG, m_ctr_list_AlarmLog);
	DDX_Control(pDX, IDC_BTNENHCTRL_SELECT_MANU_0, m_ctrlSelectMenu[0]);
	DDX_Control(pDX, IDC_BTNENHCTRL_SELECT_MANU_1, m_ctrlSelectMenu[1]);
	DDX_Control(pDX, IDC_LIST_LOG_LIST, m_ctr_list_SeqLogList);
}


BEGIN_MESSAGE_MAP(CDialogAlarmLogView, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(DTN_CLOSEUP, IDC_DATETIMEPICKER_DAY_SELECT, &CDialogAlarmLogView::OnDtnCloseupDatetimepickerDaySelect)
	ON_NOTIFY(DTN_CLOSEUP, IDC_DATETIMEPICKER_DAY_SELECT2, &CDialogAlarmLogView::OnDtnCloseupDatetimepickerDaySelect2)
	ON_MESSAGE(USE_MSG_LOG_READ_END_AALARM_LOG_VIEW_DIALOG, &CDialogAlarmLogView::OnLogReadEndMessage)
END_MESSAGE_MAP()


// CDialogAlarmLogView 메시지 처리기
void CDialogAlarmLogView::InitList()
{
	m_ctr_list_Month_Alarm;
	m_ctr_list_Day_Alarm;


	CString strCol;
	LV_COLUMN m_lvColumn;
	m_lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;
	for (int i = 0; i < 2; i++)
	{
		switch (i)
		{
		case 0:
			strCol.Format(_T("알람 이름"));
			m_lvColumn.cx = 230;
			break;
		case 1:
			strCol.Format(_T("발생 수"));
			m_lvColumn.cx = 50;
			break;
		}
		m_lvColumn.pszText = strCol.GetBuffer();
		m_lvColumn.iSubItem = i;
		
		m_ctr_list_Month_Alarm.InsertColumn(i, &m_lvColumn);
	}

	for (int i = 0; i < 2; i++)
	{
		switch (i)
		{
		case 0:
			strCol.Format(_T("알람 이름"));
			m_lvColumn.cx = 230;
			break;
		case 1:
			strCol.Format(_T("발생 수"));
			m_lvColumn.cx = 50;
			break;
		}
		m_lvColumn.pszText = strCol.GetBuffer();
		m_lvColumn.iSubItem = i;
		m_ctr_list_Day_Alarm.InsertColumn(i, &m_lvColumn);
	}

	m_lvColumn.cx = 390;
	strCol.Format(_T("Log"));
	m_lvColumn.pszText = strCol.GetBuffer();
	m_lvColumn.iSubItem = 0;
	m_ctr_list_AlarmLog.InsertColumn(0, &m_lvColumn);

	m_lvColumn.cx = 900;
	strCol.Format(_T("Log"));
	m_lvColumn.pszText = strCol.GetBuffer();
	m_lvColumn.iSubItem = 0;
	m_ctr_list_SeqLogList.InsertColumn(0, &m_lvColumn);

	m_ctr_list_Month_Alarm.EnableWindow(FALSE);
	m_ctr_list_Day_Alarm.EnableWindow(FALSE);

	int nIdx = 0;
	
	m_ctr_list_Month_Alarm.InsertItem(nIdx, ALARM_MSG_EMG_INTER_LOCK);
	m_ctr_list_Month_Alarm.SetItemText(nIdx++, 1, _T("0"));
	m_ctr_list_Month_Alarm.InsertItem(nIdx, ALARM_MSG_KEY_SW_INTER_LOCK);
	m_ctr_list_Month_Alarm.SetItemText(nIdx++, 1, _T("0"));
	m_ctr_list_Month_Alarm.InsertItem(nIdx, ALARM_MSG_DOOR_INTER_LOCK);
	m_ctr_list_Month_Alarm.SetItemText(nIdx++, 1, _T("0"));
	m_ctr_list_Month_Alarm.InsertItem(nIdx, ALARM_MSG_INVERTER_ALARM_INTER_LOCK);
	m_ctr_list_Month_Alarm.SetItemText(nIdx++, 1, _T("0"));
	m_ctr_list_Month_Alarm.InsertItem(nIdx, ALARM_MSG_TUBE_ALARM_INTER_LOCK);
	m_ctr_list_Month_Alarm.SetItemText(nIdx++, 1, _T("0"));
	m_ctr_list_Month_Alarm.InsertItem(nIdx, ALARM_MSG_X_RAY_LAMP_ALARM_INTER_LOCK);
	m_ctr_list_Month_Alarm.SetItemText(nIdx++, 1, _T("0"));
	m_ctr_list_Month_Alarm.InsertItem(nIdx, ALARM_MSG_IN_SENSER_INTER_LOCK);
	m_ctr_list_Month_Alarm.SetItemText(nIdx++, 1, _T("0"));
	m_ctr_list_Month_Alarm.InsertItem(nIdx, ALARM_MSG_OUT_SENSER_INTER_LOCK);
	m_ctr_list_Month_Alarm.SetItemText(nIdx++, 1, _T("0"));

	nIdx = 0;

	m_ctr_list_Day_Alarm.InsertItem(nIdx, ALARM_MSG_EMG_INTER_LOCK);
	m_ctr_list_Day_Alarm.SetItemText(nIdx++, 1, _T("0"));
	m_ctr_list_Day_Alarm.InsertItem(nIdx, ALARM_MSG_KEY_SW_INTER_LOCK);
	m_ctr_list_Day_Alarm.SetItemText(nIdx++, 1, _T("0"));
	m_ctr_list_Day_Alarm.InsertItem(nIdx, ALARM_MSG_DOOR_INTER_LOCK);
	m_ctr_list_Day_Alarm.SetItemText(nIdx++, 1, _T("0"));
	m_ctr_list_Day_Alarm.InsertItem(nIdx, ALARM_MSG_INVERTER_ALARM_INTER_LOCK);
	m_ctr_list_Day_Alarm.SetItemText(nIdx++, 1, _T("0"));
	m_ctr_list_Day_Alarm.InsertItem(nIdx, ALARM_MSG_TUBE_ALARM_INTER_LOCK);
	m_ctr_list_Day_Alarm.SetItemText(nIdx++, 1, _T("0"));
	m_ctr_list_Day_Alarm.InsertItem(nIdx, ALARM_MSG_X_RAY_LAMP_ALARM_INTER_LOCK);
	m_ctr_list_Day_Alarm.SetItemText(nIdx++, 1, _T("0"));
	m_ctr_list_Day_Alarm.InsertItem(nIdx, ALARM_MSG_IN_SENSER_INTER_LOCK);
	m_ctr_list_Day_Alarm.SetItemText(nIdx++, 1, _T("0"));
	m_ctr_list_Day_Alarm.InsertItem(nIdx, ALARM_MSG_OUT_SENSER_INTER_LOCK);
	m_ctr_list_Day_Alarm.SetItemText(nIdx++, 1, _T("0"));
}

BOOL CDialogAlarmLogView::ReadSeqLog()
{
	CString strFindeFileName;
	CFileFind ff;
	CString strPath;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	strPath.Format(_T("%s\\%sSequence\\*.*"), strTemp, LOG_FILE_PATH);
	deque<CString> queLogList;
	SYSTEMTIME	lpSystemTime = m_SeletelpSystemTime2;
	//GetLocalTime(&lpSystemTime);

	m_strRawSeqLog.clear();


	BOOL rslt = ff.FindFile(strPath);
	if (rslt)
	{
		CString Filename;
		BOOL bFlag = true;
		while (bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if (ff.IsDots())
			{
				continue;
			}
			if (ff.IsDirectory())
			{
				continue;
			}


			Filename.Format(_T("%s"), ff.GetFileName());
			if (_wtoi(Filename.Mid(0, 4)) == lpSystemTime.wYear && _wtoi(Filename.Mid(4, 2)) == lpSystemTime.wMonth && _wtoi(Filename.Mid(6, 2)) == lpSystemTime.wDay)
			{
				ReadSeqLogFile(ff.GetFilePath());// +ff.GetFileName());
			}
		}
	}

	ff.Close();

	return rslt;
}

BOOL CDialogAlarmLogView::ReadSeqLogFile(CString strFilePath)
{
	CStdioFile pFile;
	BOOL bOpen = pFile.Open(strFilePath, CFile::shareDenyRead | CFile::modeRead);
	CString tempStr1, tempStr2;

	if (bOpen)
	{
		while (pFile.ReadString(tempStr1))
		{
			m_strRawSeqLog.push_back(tempStr1);
		}
	}

	pFile.Close();

	return bOpen;
}

void CDialogAlarmLogView::ViewSeqLogList()
{
	m_ctr_list_SeqLogList.DeleteAllItems();
	int nStart = (m_nCurLogIdx - 1) * LOG_MAX_LIST_COUNT, nEnd = m_nCurLogIdx * LOG_MAX_LIST_COUNT;
	if (m_strRawSeqLog.size() < LOG_MAX_LIST_COUNT)
	{
		nEnd = m_strRawSeqLog.size();
	}
	if (nEnd > m_strRawSeqLog.size())
		nEnd = m_strRawSeqLog.size();
	int j = LOG_MAX_LIST_COUNT;
	for (int i = 0; i < (nEnd - nStart); i++)
	{
		m_ctr_list_SeqLogList.InsertItem(0, m_strRawSeqLog[nEnd - 1 - i]);
	}
}

BOOL CDialogAlarmLogView::ReadAlarmLog(int nType)
{
	CString strFindeFileName;
	CFileFind ff;
	CString strPath;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	strPath.Format(_T("%s\\%sAlarm\\*.*"), strTemp, LOG_FILE_PATH);
	deque<CString> queLogList;
	SYSTEMTIME	lpSystemTime = m_SeletelpSystemTime;
	//GetLocalTime(&lpSystemTime);
	m_strRawDayAlarmLog.clear();
	if (nType == 0)
		m_strMonthAlarmLog.clear();
	else
		m_strDayAlarmLog.clear();

	BOOL rslt = ff.FindFile(strPath);
	if (rslt)
	{
		CString Filename;
		BOOL bFlag = true;
		while (bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if (ff.IsDots())
			{
				continue;
			}
			if (ff.IsDirectory())
			{
				continue;
			}

			if (nType == 0)
			{
				Filename.Format(_T("%s"), ff.GetFileName());

				if (_wtoi(Filename.Mid(0, 4)) == lpSystemTime.wYear && _wtoi(Filename.Mid(4, 2)) == lpSystemTime.wMonth)
				{
					ReadAlarmLogFile(0, ff.GetFilePath());// +ff.GetFileName());
					AlarmCountCheck(0);
				}
			}
			else
			{
				Filename.Format(_T("%s"), ff.GetFileName());
				if (_wtoi(Filename.Mid(0, 4)) == lpSystemTime.wYear && _wtoi(Filename.Mid(4, 2)) == lpSystemTime.wMonth && _wtoi(Filename.Mid(6, 2)) == lpSystemTime.wDay)
				{
					ReadAlarmLogFile(1, ff.GetFilePath());// +ff.GetFileName());
					AlarmCountCheck(1);
				}
			}
		}
	}

	ff.Close();

	return rslt;
}

void CDialogAlarmLogView::ViewAlarmCount()
{
	CString strText;

	for (int i = 0; i < 8; i++)
	{
		strText.Format(_T("%d"), m_nMonthAlarmCount[i]);
		m_ctr_list_Month_Alarm.SetItemText(i, 1, strText);

		strText.Format(_T("%d"), m_nDayAlarmCount[i]);
		m_ctr_list_Day_Alarm.SetItemText(i, 1, strText);
	}
}

BOOL CDialogAlarmLogView::ReadAlarmLogFile(int nType, CString strFilePath)
{
	CStdioFile pFile;
	BOOL bOpen = pFile.Open(strFilePath, CFile::shareDenyRead | CFile::modeRead);
	CString tempStr1, tempStr2;

	if (bOpen)
	{
		while (pFile.ReadString(tempStr1))
		{
			AfxExtractSubString(tempStr2, tempStr1, 1, '-');
			if(nType == 0)
				m_strMonthAlarmLog.push_back(tempStr2.Mid(1, tempStr2.GetLength() - 1));
			else
			{
				m_strDayAlarmLog.push_back(tempStr2.Mid(1, tempStr2.GetLength() - 1));
				m_strRawDayAlarmLog.push_back(tempStr1);
			}
		}
	}

	if(bOpen)
		pFile.Close();

	return bOpen;
}

void CDialogAlarmLogView::AlarmCountCheck(int nType)
{
	if (nType == 0)
	{
		for (int j = 0; j < m_strMonthAlarmLog.size(); j++)
		{
			CString strText;
			strText.Format(_T("%s"), m_strMonthAlarmLog[j].Mid(10, 2));
			m_nMonthAlarmCount[_wtoi(strText) - 1]++;
		}
	}
	else
	{	
		for (int j = 0; j < m_strDayAlarmLog.size(); j++)
		{
			CString strText;
			strText.Format(_T("%s"), m_strDayAlarmLog[j].Mid(10, 2));
			m_nDayAlarmCount[_wtoi(strText) - 1]++;
		}
	}
}

void CDialogAlarmLogView::ViewAlarmLogList()
{
	//m_ctr_list_AlarmLog.DeleteAllItems();
	//for (int i = 0; i < m_strRawDayAlarmLog.size(); i++)
	//{
	//	m_ctr_list_AlarmLog.InsertItem(0, m_strRawDayAlarmLog[m_strRawDayAlarmLog.size() - i - 1]);
	//}

	m_ctr_list_AlarmLog.DeleteAllItems();
	int nStart = (m_nCurLogIdx - 1) * LOG_MAX_LIST_COUNT, nEnd = m_nCurLogIdx * LOG_MAX_LIST_COUNT;
	if (m_strRawDayAlarmLog.size() < LOG_MAX_LIST_COUNT)
	{
		nEnd = m_strRawDayAlarmLog.size();
	}
	if (nEnd > m_strRawDayAlarmLog.size())
		nEnd = m_strRawDayAlarmLog.size();
	int j = LOG_MAX_LIST_COUNT;
	for (int i = 0; i < (nEnd - nStart); i++)
	{
		m_ctr_list_AlarmLog.InsertItem(0, m_strRawDayAlarmLog[nEnd - 1 - i]);
	}
}

BOOL CDialogAlarmLogView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogAlarmLogView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	CDialogEx::OnTimer(nIDEvent);
}


void CDialogAlarmLogView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		ClickBtnenhctrlSelectManu0();
		SYSTEMTIME	lpSystemTime;
		GetLocalTime(&lpSystemTime);
		m_ctrl_DaySelect.SetTime(lpSystemTime);
		m_SeletelpSystemTime = lpSystemTime;

		for (int i = 0; i < 8; i++)
		{
			m_nMonthAlarmCount[i] = 0;
			m_nDayAlarmCount[i] = 0;
		}
		m_nCurLogIdx = 1;
		ReadAlarmLog(0);
		ReadAlarmLog(1);
		ViewAlarmCount();
		ViewAlarmLogList();
	}
	else
	{
		m_strMonthAlarmLog.clear();
		m_strDayAlarmLog.clear();
		m_strRawDayAlarmLog.clear();
		m_strRawSeqLog.clear();
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CDialogAlarmLogView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogAlarmLogView::OnDtnCloseupDatetimepickerDaySelect(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	SYSTEMTIME	lpSystemTime;
	m_ctrl_DaySelect.GetTime(&m_SeletelpSystemTime);
	GetLocalTime(&lpSystemTime);

	for (int i = 0; i < 8; i++)
		m_nMonthAlarmCount[i] = 0;
	
	for (int i = 0; i < 8; i++)
		m_nDayAlarmCount[i] = 0;

	InitLogReadThread();
}

BEGIN_EVENTSINK_MAP(CDialogAlarmLogView, CDialogEx)
	ON_EVENT(CDialogAlarmLogView, IDC_BTNENHCTRL_SELECT_MANU_0, DISPID_CLICK, CDialogAlarmLogView::ClickBtnenhctrlSelectManu0, VTS_NONE)
	ON_EVENT(CDialogAlarmLogView, IDC_BTNENHCTRL_SELECT_MANU_1, DISPID_CLICK, CDialogAlarmLogView::ClickBtnenhctrlSelectManu1, VTS_NONE)
	ON_EVENT(CDialogAlarmLogView, IDC_BTNENHCTRL_SEQ_LOG_LIST_M, DISPID_CLICK, CDialogAlarmLogView::ClickBtnenhctrlSeqLogListM, VTS_NONE)
	ON_EVENT(CDialogAlarmLogView, IDC_BTNENHCTRL_SEQ_LOG_LIST_P, DISPID_CLICK, CDialogAlarmLogView::ClickBtnenhctrlSeqLogListP, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogAlarmLogView::ClickBtnenhctrlSelectManu0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBtnenhctrl* pBtn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_SELECT_MANU_0);
	pBtn->SetValue(TRUE);
	pBtn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_SELECT_MANU_1);
	pBtn->SetValue(FALSE);
	SelectMenu(0);
}


void CDialogAlarmLogView::ClickBtnenhctrlSelectManu1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBtnenhctrl* pBtn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_SELECT_MANU_0);
	pBtn->SetValue(FALSE);
	pBtn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_SELECT_MANU_1);
	pBtn->SetValue(TRUE);
	SelectMenu(1);
}

void CDialogAlarmLogView::SelectMenu(int nType)
{
	SYSTEMTIME	lpSystemTime;
	m_nSelectManu = nType;
	switch (nType)
	{
	case 0:
		//show
		GetDlgItem(IDC_BTNENHCTRL_ALARM_LOG_VIEW_LABEL_1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DATETIMEPICKER_DAY_SELECT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LIST_MONTH_ALARM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LIST_DAY_ALARM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LIST_DAY_ALARM_LOG)->ShowWindow(SW_SHOW);

		//hide
		GetDlgItem(IDC_DATETIMEPICKER_DAY_SELECT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LIST_LOG_LIST)->ShowWindow(SW_HIDE);

		/*GetDlgItem(IDC_STATIC_0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_SEQ_LOG_LIST_M)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_SEQ_LOG_LIST_P)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_SEQ_LOG_LIST_CUR_IDX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_SEQ_LOG_LIST_MAX_IDX)->ShowWindow(SW_HIDE);*/

		GetDlgItem(IDC_BTNENHCTRL_ALARM_LOG_VIEW_LABEL_0)->SetWindowText(_T("월별"));

		
		GetLocalTime(&lpSystemTime);
		m_ctrl_DaySelect.SetTime(lpSystemTime);
		m_SeletelpSystemTime = lpSystemTime;

		for (int i = 0; i < 8; i++)
		{
			m_nMonthAlarmCount[i] = 0;
			m_nDayAlarmCount[i] = 0;
		}

		
		InitLogReadThread();
		break;
	case 1:
		//hide
		GetDlgItem(IDC_BTNENHCTRL_ALARM_LOG_VIEW_LABEL_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DATETIMEPICKER_DAY_SELECT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LIST_MONTH_ALARM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LIST_DAY_ALARM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LIST_DAY_ALARM_LOG)->ShowWindow(SW_HIDE);

		//show
		GetDlgItem(IDC_DATETIMEPICKER_DAY_SELECT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LIST_LOG_LIST)->ShowWindow(SW_SHOW);

		/*GetDlgItem(IDC_STATIC_0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_SEQ_LOG_LIST_M)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_SEQ_LOG_LIST_P)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_SEQ_LOG_LIST_CUR_IDX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_SEQ_LOG_LIST_MAX_IDX)->ShowWindow(SW_SHOW);*/

		GetDlgItem(IDC_BTNENHCTRL_ALARM_LOG_VIEW_LABEL_0)->SetWindowText(_T("로그"));

		GetLocalTime(&lpSystemTime);
		m_ctrl_DaySelect2.SetTime(lpSystemTime);
		m_SeletelpSystemTime2 = lpSystemTime;
		m_ctr_list_SeqLogList.DeleteAllItems();
		
		InitLogReadThread();
		break;
	}
}

void CDialogAlarmLogView::OnDtnCloseupDatetimepickerDaySelect2(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	m_ctrl_DaySelect2.GetTime(&m_SeletelpSystemTime2);
	m_ctr_list_SeqLogList.DeleteAllItems();
	
	InitLogReadThread();
}

UINT CDialogAlarmLogView::LogReadThread(LPVOID pParam)
{
	CDialogAlarmLogView* pDlg = (CDialogAlarmLogView*)pParam;
	pDlg->ThreadLogRead();

	return 0;
}

void CDialogAlarmLogView::ThreadLogRead()
{
	m_bLogReadThreadEnd = FALSE;
	
	if (m_nSelectManu == 0)
	{
		ReadAlarmLog(0);
		ReadAlarmLog(1);
		ViewAlarmCount();
	}
	else
		ReadSeqLog();

	SendMessage(USE_MSG_LOG_READ_END_AALARM_LOG_VIEW_DIALOG, 0, 0);
	
	m_bLogReadThreadEnd = TRUE;
}

void CDialogAlarmLogView::InitLogReadThread()
{
	while (TRUE)
	{
		if (GetLogReadThreadEnd())
		{
			if(m_pLogReadThread)
				delete m_pLogReadThread;
			m_pLogReadThread = NULL;
			break;
		}

		Sleep(1);
	}

	m_pLogReadThread = AfxBeginThread(LogReadThread, this, THREAD_PRIORITY_NORMAL);
	m_pLogReadThread->m_bAutoDelete = FALSE;
}

BOOL CDialogAlarmLogView::GetLogReadThreadEnd()
{
	BOOL rslt = TRUE;
	if (m_pLogReadThread != NULL)
	{
		DWORD dwRet = WaitForSingleObject(m_pLogReadThread->m_hThread, 1000);

		if (dwRet == WAIT_FAILED)
		{
			rslt = FALSE;
		}
		else if (dwRet == WAIT_ABANDONED)
		{
			rslt = FALSE;
		}
		else if (dwRet == WAIT_TIMEOUT)
		{
			rslt = FALSE;
		}
	}

	return rslt;
}

LRESULT CDialogAlarmLogView::OnLogReadEndMessage(WPARAM para0, LPARAM para1)
{
	m_nCurLogIdx = 1;
	if(m_nSelectManu == 0)
		m_nMaxLogIdx = m_strRawDayAlarmLog.size() / 100 + 1;
	else
		m_nMaxLogIdx = m_strRawSeqLog.size() / 100 + 1;

	CString strText;
	strText.Format(_T("%d"), m_nCurLogIdx);
	SetDlgItemText(IDC_BTNENHCTRL_SEQ_LOG_LIST_CUR_IDX, strText);
	strText.Format(_T("%d"), m_nMaxLogIdx);
	SetDlgItemText(IDC_BTNENHCTRL_SEQ_LOG_LIST_MAX_IDX, strText);

	if (m_nSelectManu == 0)
		ViewAlarmLogList();
	else
		ViewSeqLogList();

	return 0;
}

void CDialogAlarmLogView::ClickBtnenhctrlSeqLogListM()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_nCurLogIdx--;
	if (m_nCurLogIdx < 1)
		m_nCurLogIdx = 1;

	CString strText;
	strText.Format(_T("%d"), m_nCurLogIdx);
	SetDlgItemText(IDC_BTNENHCTRL_SEQ_LOG_LIST_CUR_IDX, strText);
	if(m_nSelectManu == 0)
		ViewAlarmLogList();
	else
		ViewSeqLogList();
}


void CDialogAlarmLogView::ClickBtnenhctrlSeqLogListP()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_nCurLogIdx++;
	if (m_nCurLogIdx > m_nMaxLogIdx)
		m_nCurLogIdx = m_nMaxLogIdx;

	CString strText;
	strText.Format(_T("%d"), m_nCurLogIdx);
	SetDlgItemText(IDC_BTNENHCTRL_SEQ_LOG_LIST_CUR_IDX, strText);
	if(m_nSelectManu == 0)
		ViewAlarmLogList();
	else
		ViewSeqLogList();
}
