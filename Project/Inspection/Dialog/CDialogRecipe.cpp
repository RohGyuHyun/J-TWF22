// CDialogRecipe.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogRecipe.h"
#include "GetTextDlg.h"
#include "GetNumDlg.h"
#include "InspectionDlg.h"

// CDialogRecipe 대화 상자

IMPLEMENT_DYNAMIC(CDialogRecipe, CDialogEx)

CDialogRecipe::CDialogRecipe(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RECIPE, pParent)
	, m_edit_CurrentRecipeName(_T(""))
{
	m_LoginType = LOGIN_USER;
	m_dlgTeachingPattern = NULL;
	memset(m_strCurrentRecipeName, NULL, sizeof(wchar_t) * 256);
	m_nInspectionType = 0;
}

CDialogRecipe::~CDialogRecipe()
{
	//if (m_nInspectionType == 0)
	{
		if (m_dlgTeachingPattern != NULL)
		{
			m_dlgTeachingPattern->DestroyWindow();
			delete m_dlgTeachingPattern;
			m_dlgTeachingPattern = NULL;
		}
	}
	//else if (m_nInspectionType == 1)
	{
		if (m_dlgTeachingBlobs != NULL)
		{
			m_dlgTeachingBlobs->DestroyWindow();
			delete m_dlgTeachingBlobs;
			m_dlgTeachingBlobs = NULL;
		}
	}

}

void CDialogRecipe::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SELECT_RECIPE, m_edit_SelectRecipe);
	DDX_Control(pDX, IDC_LIST_RECIPE, m_list_Recipe);
	DDX_Text(pDX, IDC_EDIT_CURRENT_RECIPE, m_edit_CurrentRecipeName);
}


BEGIN_MESSAGE_MAP(CDialogRecipe, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_INSPECTION_TEACHING, &CDialogRecipe::OnBnClickedButtonInspectionTeaching)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDialogRecipe::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_NEW_RECIPE, &CDialogRecipe::OnBnClickedButtonNewRecipe)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_RECIPE, &CDialogRecipe::OnBnClickedButtonDeleteRecipe)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_RECIPE, &CDialogRecipe::OnBnClickedButtonChangeRecipe)
	ON_LBN_SELCHANGE(IDC_LIST_RECIPE, &CDialogRecipe::OnLbnSelchangeListRecipe)
END_MESSAGE_MAP()


// CDialogRecipe 메시지 처리기


BOOL CDialogRecipe::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LoadRecipeInfo();

	m_dlgTeachingPattern = new CDialogTeachingPattern();
	m_dlgTeachingPattern->Create(IDD_DIALOG_INSPECTION_TEACHING_PATTERN, this);
	m_dlgTeachingPattern->SetRecipeName(this->m_strCurrentRecipeName);
	m_dlgTeachingBlobs = new CDialogTeachingBlobs();
	m_dlgTeachingBlobs->Create(IDD_DIALOG_INSPECTION_TEACHING_BLOBS, this);
	m_dlgTeachingBlobs->SetRecipeName(this->m_strCurrentRecipeName);
	
	InitRecipeList();
	m_edit_CurrentRecipeName.Format(_T("%s"), m_strCurrentRecipeName);
	m_edit_SelectRecipe.Format(_T(""));
	m_dlgTeachingPattern->SetRecipeName(m_strCurrentRecipeName);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogRecipe::OnBnClickedButtonInspectionTeaching()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_nInspectionType == 0)
	{
		m_dlgTeachingPattern->SetRecipeName(m_strCurrentRecipeName);
		m_dlgTeachingPattern->ShowWindow(SW_SHOW);
	}
	else if (m_nInspectionType == 1)
	{
		m_dlgTeachingBlobs->SetRecipeName(m_strCurrentRecipeName);
		m_dlgTeachingBlobs->ShowWindow(SW_SHOW);
	}
}
void CDialogRecipe::SetImageSize(int nWidth, int nHeight)
{
	if (m_nInspectionType == 0)
	{
		m_dlgTeachingPattern->SetImageSize(nWidth, nHeight);
	}
	else
	{
		m_dlgTeachingBlobs->SetImageSize(nWidth, nHeight);
	}
};
void CDialogRecipe::SetCallBack(CALLBACK_FUNCTION_Event event)
{
	if (m_nInspectionType == 0)
	{
		m_dlgTeachingPattern->SetCallBack(event);
	}
	else
	{
		m_dlgTeachingBlobs->SetCallBack(event);
	}
}

HWND CDialogRecipe::GetTeachingDlgHwnd()
{
	if (m_nInspectionType == 0)
	{
		return m_dlgTeachingPattern->GetTeachingDlgHwnd();
	}
	else
	{
		return m_dlgTeachingBlobs->GetTeachingDlgHwnd();
	}
}

void CDialogRecipe::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		m_nClose = 0;
		LoadRecipeInfo();
		InitRecipeList();
		m_edit_CurrentRecipeName.Format(_T("%s"), m_strCurrentRecipeName);
		m_edit_SelectRecipe.Format(_T(""));
		UpdateData(FALSE);

		if (m_LoginType == LOGIN_USER)
		{
			GetDlgItem(IDC_BTNENHCTRL_NEW_RECIPE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTNENHCTRL_DELETE_RECIPE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTNENHCTRL_INSP_TEACHING)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BTNENHCTRL_NEW_RECIPE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTNENHCTRL_DELETE_RECIPE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTNENHCTRL_INSP_TEACHING)->EnableWindow(TRUE);
		}
	}
	else
	{
		if (m_nInspectionType == 0)
		{
			if (m_dlgTeachingPattern->IsWindowVisible())
				m_dlgTeachingPattern->ShowWindow(SW_HIDE);
		}
		else if (m_nInspectionType == 1)
		{
			if (m_dlgTeachingBlobs->IsWindowVisible())
				m_dlgTeachingBlobs->ShowWindow(SW_HIDE);
			
		}
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDialogRecipe::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->ShowWindow(SW_HIDE);
}

BOOL CDialogRecipe::SaveRecipeInfo()
{
	CString key, dat;
	CString file_path, pat_path, extention, master_path, cur_path;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);

	file_path.Format(_T("%s\\Recipe\\%s"), strTemp, RECIPE_STR_PARA_FILENAME);

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(file_path);
	CString strNode, strKey, strLabel;

	strNode.Format(_T("CURRENT_RECIPE_PARAMETER"));
	strKey.Format(_T("CURRENT_RECIPE_NAME"));
	cDataIf.SetString(strNode, strKey, m_strCurrentRecipeName);

	return TRUE;
}

BOOL CDialogRecipe::LoadRecipeInfo()
{
	CString key, dat;
	CString file_path, pat_path, extention, master_path, cur_path;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);

	file_path.Format(_T("%s\\Recipe\\%s"), strTemp, RECIPE_STR_PARA_FILENAME);

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(file_path);
	CString strNode, strKey, strLabel;

	strNode.Format(_T("CURRENT_RECIPE_PARAMETER"));
	strKey.Format(_T("CURRENT_RECIPE_NAME"));
	strLabel = cDataIf.GetString(strNode, strKey, _T("NoName"));
	memcpy(m_strCurrentRecipeName, strLabel.GetBuffer(), sizeof(wchar_t) * (strLabel.GetLength() + 1));
	m_edit_CurrentRecipeName = strLabel;

	return TRUE;
}

void CDialogRecipe::InitRecipeList()
{
	if(m_list_Recipe.GetCount() > 0)
		m_list_Recipe.ResetContent();

	CString strFilePath, strName, strIdx;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	strFilePath.Format(_T("%s\\Recipe\\*.*"), strTemp);
	CFileFind cFFind;
	int nCount = 0;

	TRY{
		BOOL bIsFile = cFFind.FindFile(strFilePath, 0);
		while (bIsFile)
		{
			bIsFile = cFFind.FindNextFile();
			strName = cFFind.GetFileName();
			if (strName != _T(".") && strName != _T("..") && strName != _T("Recipe.ini"))
			{
				//CString strTemp2;
				//int nIndex = strName.Find(_T("."));
				//strTemp2.Format(_T("%s"), strName.Mid(0, nIndex));
				m_list_Recipe.AddString(strName);
			}
			Delay(2,TRUE);
		}
	}
	CATCH(CException, e)
	{
		cFFind.Close();
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

	cFFind.Close();
}

void CDialogRecipe::DeleteRecipeList()
{
	CString str;
	CString strMsg;

	CString strFilePath;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);

	if (m_edit_SelectRecipe == m_edit_CurrentRecipeName)
	{
		//g_cMsgView.OkMsgBox(_T("현재 사용 중인 Model은 삭제할 수 없습니다."));
		AfxMessageBox(_T("Use Model Not Delete."));
		return;
	}

	strMsg.Format(_T("Select Model Name < %s > is Delete?"), m_edit_SelectRecipe);
	if (AfxMessageBox(strMsg, MB_YESNO) != IDYES)
	{
		return;
	}

	TRY
	{
		strFilePath.Format(_T("%s\\Recipe\\%s"), strTemp, m_edit_SelectRecipe);
		DeleteFolderAndFile(strFilePath);
	}
	CATCH(CFileException, e)
	{
		return;
	}
	END_CATCH

	InitRecipeList();
}

void CDialogRecipe::InsertRecipeList()
{
	CGetTextDlg	dlg(30, _T(""), _T("Input New Model Name"), NULL, FALSE);

	if (dlg.DoModal() == IDCANCEL) return;
	CString strModelName = dlg.GetStringValue();
	CString strSrcFile, strDstFile;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);

	if (!m_edit_CurrentRecipeName.Compare(_T("")))
	{
		m_dlgTeachingPattern->SetInsertRecipeFile(strModelName);
		m_edit_CurrentRecipeName.Format(_T("%s"), strModelName);
		memcpy(m_strCurrentRecipeName, m_edit_CurrentRecipeName.GetBuffer(), sizeof(wchar_t) * (m_edit_CurrentRecipeName.GetLength() + 1));
		SaveRecipeInfo();
		//OnBnClickedButtonChangeRecipe();
		UpdateData(FALSE);
	}
	else
	{
		CString strTempPath;
		strTempPath.Format(_T("%s\\Recipe\\%s\\"), strTemp, strModelName);
		CreateDir(strTempPath);

		strSrcFile.Format(_T("%s\\Recipe\\%s\\Teaching.ini"), strTemp, m_edit_CurrentRecipeName);
		strDstFile.Format(_T("%s\\Recipe\\%s\\Teaching.ini"), strTemp, strModelName);
		CopyFile(strSrcFile, strDstFile, TRUE);
	}
	
	InitRecipeList();
}

CString CDialogRecipe::GetCurrentRecipeInfo()
{
	CString strText;
	strText.Format(_T("%s"), m_strCurrentRecipeName);
	return strText;
}


void CDialogRecipe::ChangeRecipe(CString strBeforeName, CString strAfterName)
{
	m_dlgTeachingPattern->SetRecipeName(strAfterName);
	m_edit_CurrentRecipeName.Format(_T("%s"), strAfterName);
	memcpy(m_strCurrentRecipeName, m_edit_CurrentRecipeName.GetBuffer(), sizeof(wchar_t) * (m_edit_CurrentRecipeName.GetLength() + 1));
	m_edit_SelectRecipe.Format(_T(""));
	UpdateData(FALSE);
	SaveRecipeInfo();
	CInspectionDlg* pdlg = (CInspectionDlg*)AfxGetApp()->m_pMainWnd;
	pdlg->SetMainRecipeName(strAfterName);
}


void CDialogRecipe::OnBnClickedButtonNewRecipe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	InsertRecipeList();
}


void CDialogRecipe::OnBnClickedButtonDeleteRecipe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_edit_SelectRecipe.IsEmpty())
		return;

	DeleteRecipeList();
}


void CDialogRecipe::OnBnClickedButtonChangeRecipe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_edit_SelectRecipe.IsEmpty())
		return;

	ChangeRecipe(m_edit_CurrentRecipeName, m_edit_SelectRecipe);
	m_nClose = 1;
}


void CDialogRecipe::OnLbnSelchangeListRecipe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str, strModel;

	int nIndex = m_list_Recipe.GetCurSel();

	if (nIndex >= 0)
	{
		m_list_Recipe.GetText(nIndex, str);

		m_edit_SelectRecipe = str;
		UpdateData(FALSE);
	}
}
BEGIN_EVENTSINK_MAP(CDialogRecipe, CDialogEx)
	ON_EVENT(CDialogRecipe, IDC_BTNENHCTRL_NEW_RECIPE, DISPID_CLICK, CDialogRecipe::ClickBtnenhctrlNewRecipe, VTS_NONE)
	ON_EVENT(CDialogRecipe, IDC_BTNENHCTRL_DELETE_RECIPE, DISPID_CLICK, CDialogRecipe::ClickBtnenhctrlDeleteRecipe, VTS_NONE)
	ON_EVENT(CDialogRecipe, IDC_BTNENHCTRL_CHANGE_RECIPE, DISPID_CLICK, CDialogRecipe::ClickBtnenhctrlChangeRecipe, VTS_NONE)
	ON_EVENT(CDialogRecipe, IDC_BTNENHCTRL_INSP_TEACHING, DISPID_CLICK, CDialogRecipe::ClickBtnenhctrlInspTeaching, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogRecipe::ClickBtnenhctrlNewRecipe()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnBnClickedButtonNewRecipe();
}


void CDialogRecipe::ClickBtnenhctrlDeleteRecipe()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnBnClickedButtonDeleteRecipe();
}


void CDialogRecipe::ClickBtnenhctrlChangeRecipe()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnBnClickedButtonChangeRecipe();
}


void CDialogRecipe::ClickBtnenhctrlInspTeaching()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnBnClickedButtonInspectionTeaching();
}


BOOL CDialogRecipe::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F5)
			ClickBtnenhctrlNewRecipe();
		else if (pMsg->wParam == VK_F6)
			ClickBtnenhctrlDeleteRecipe();
		else if (pMsg->wParam == VK_F7)
			ClickBtnenhctrlChangeRecipe();
		else if (pMsg->wParam == VK_F8)
			ClickBtnenhctrlInspTeaching();
		else if (pMsg->wParam == VK_F12)
			OnBnClickedButtonClose();
		else if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
