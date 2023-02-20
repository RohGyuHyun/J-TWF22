#include "StdAfx.h"
#include "UserControl.h"

CUserControl::CUserControl(void)
{

}

CUserControl::~CUserControl(void)
{
}

void CUserControl::LEVEL0(CString data)
{
		////ToolBar 버튼 컨트롤
	//CToolBarCtrl& MainBar = ((CMainDlg*)AfxGetMainWnd())->m_ToolBar.GetToolBarCtrl();

	//MainBar.EnableButton(ID_TOOLBTN_SAVE, true);
	//MainBar.EnableButton(ID_TOOLBTN_OPEN, true);
	//MainBar.EnableButton(ID_TOOLBTN_CONVAYOR, true);
	//MainBar.EnableButton(ID_TOOLBTN_FIDUCIAL, true);
	//MainBar.EnableButton(ID_TOOLBTN_PIPMAPMAKE, true);
	//MainBar.EnableButton(ID_TOOLBTN_JOYPAD, true);
	//MainBar.EnableButton(ID_TOOLBTN_MODE, true);
	//MainBar.EnableButton(ID_TOOLBTN_AUTOMOVE, true);
	//MainBar.EnableButton(ID_TOOLBTN_AUTOSTOP, true);
	//MainBar.EnableButton(ID_TOOLBTN_MOVESETT, true);
	//MainBar.EnableButton(ID_PCBIN, true);

	//////ContainerDlg 탭 컨트롤
	//CWnd* pWnd_Container = ((CMainDlg*)AfxGetMainWnd())->m_pContainerDlg;

	//pWnd_Container->EnableWindow(true);

	//////SysOpPanelDlg 버튼 컨트롤
	//CWnd* pWnd_Main = ((CMainDlg*)AfxGetMainWnd())->m_pSysOpPanelDlg;

	//ASSERT(pWnd_Main != NULL && pWnd_Main->GetSafeHwnd());

	//pWnd_Main->GetDlgItem(IDC_DF_TRIAL)->ShowWindow(SW_SHOW);
	//pWnd_Main->GetDlgItem(IDC_DF_SAVE)->ShowWindow(SW_SHOW);
	//pWnd_Main->GetDlgItem(IDC_BTN_MODE)->ShowWindow(SW_SHOW);
	//pWnd_Main->GetDlgItem(IDC_BTN_TEACHING)->ShowWindow(SW_SHOW);
	//pWnd_Main->GetDlgItem(IDC_BTN_FIDUCIAL)->ShowWindow(SW_SHOW);
	//pWnd_Main->GetDlgItem(IDC_BTN_MOTION)->ShowWindow(SW_SHOW);
	//pWnd_Main->GetDlgItem(IDC_BTN_IO)->ShowWindow(SW_SHOW);

	//////DetecViewDlg 버튼 컨트롤
	//CWnd* pWnd_DetecView = ((CMainDlg*)AfxGetMainWnd())->m_pDetecView;

	//pWnd_DetecView->GetDlgItem(IDC_BTND1VIEW)->ShowWindow(SW_SHOW);
	//pWnd_DetecView->GetDlgItem(IDC_BTND2VIEW)->ShowWindow(SW_SHOW);
	//pWnd_DetecView->GetDlgItem(IDC_D1_SETTINGBTN)->ShowWindow(SW_SHOW);
	//pWnd_DetecView->GetDlgItem(IDC_D2_SETTINGBTN)->ShowWindow(SW_SHOW);


}

void CUserControl::LEVEL1(CString data)
{

};

void CUserControl::LEVEL2(CString data)
{
	////ToolBar 버튼 컨트롤
//	CToolBarCtrl& MainBar = ((CMainDlg*)AfxGetMainWnd())->m_ToolBar.GetToolBarCtrl();
//
//	MainBar.EnableButton(ID_TOOLBTN_SAVE, false);
//	MainBar.EnableButton(ID_TOOLBTN_OPEN, false);
//	MainBar.EnableButton(ID_TOOLBTN_CONVAYOR, false);
//	MainBar.EnableButton(ID_TOOLBTN_FIDUCIAL, false);
//	MainBar.EnableButton(ID_TOOLBTN_PIPMAPMAKE, false);
//	MainBar.EnableButton(ID_TOOLBTN_JOYPAD, false);
//	MainBar.EnableButton(ID_TOOLBTN_MODE, false);
//	MainBar.EnableButton(ID_TOOLBTN_AUTOMOVE, false);
//	MainBar.EnableButton(ID_TOOLBTN_AUTOSTOP, false);
//	MainBar.EnableButton(ID_TOOLBTN_MOVESETT, false);
//	MainBar.EnableButton(ID_PCBIN, false);
//
//	////ContainerDlg 탭 컨트롤
//	CWnd* pWnd_Container = ((CMainDlg*)AfxGetMainWnd())->m_pContainerDlg;
//
//	pWnd_Container->EnableWindow(false);
//
//	////SysOpPanelDlg 버튼 컨트롤
//	CWnd* pWnd_Main = ((CMainDlg*)AfxGetMainWnd())->m_pSysOpPanelDlg;
//
//	ASSERT(pWnd_Main != NULL && pWnd_Main->GetSafeHwnd());
//
//	pWnd_Main->GetDlgItem(IDC_DF_TRIAL)->ShowWindow(SW_HIDE);
//	pWnd_Main->GetDlgItem(IDC_DF_SAVE)->ShowWindow(SW_HIDE);
//	pWnd_Main->GetDlgItem(IDC_BTN_MODE)->ShowWindow(SW_HIDE);
//	pWnd_Main->GetDlgItem(IDC_BTN_TEACHING)->ShowWindow(SW_HIDE);
//	pWnd_Main->GetDlgItem(IDC_BTN_FIDUCIAL)->ShowWindow(SW_HIDE);
//	pWnd_Main->GetDlgItem(IDC_BTN_MOTION)->ShowWindow(SW_HIDE);
//	pWnd_Main->GetDlgItem(IDC_BTN_IO)->ShowWindow(SW_HIDE);
//
//
//	////DetecViewDlg 버튼 컨트롤
//	CWnd* pWnd_DetecView = ((CMainDlg*)AfxGetMainWnd())->m_pDetecView;
//
//	pWnd_DetecView->GetDlgItem(IDC_BTND1VIEW)->ShowWindow(SW_HIDE);
//	pWnd_DetecView->GetDlgItem(IDC_BTND2VIEW)->ShowWindow(SW_HIDE);
//	pWnd_DetecView->GetDlgItem(IDC_D1_SETTINGBTN)->ShowWindow(SW_HIDE);
//	pWnd_DetecView->GetDlgItem(IDC_D2_SETTINGBTN)->ShowWindow(SW_HIDE);
//
///*
//	if( .GetAt(0) == _T('1'))
//	{
//		pWnd->GetDlgItem(IDC_DF_OPEN)->EnableWindow(false);
//	}
//	if( .GetAt(1) == _T('1'))
//	{
//		pWnd->GetDlgItem(IDC_DF_SAVE)->EnableWindow(false);
//	}
//	if( data.GetAt(2) == _T('1'))
//	{
//		pWnd->GetDlgItem(IDC_BTN_MODE)->EnableWindow(false);
//	}
//	if( data.GetAt(3) == _T('1'))
//	{
//		pWnd->GetDlgItem(IDC_BTN_TEACHING)->EnableWindow(false);
//	}
//	if( data.GetAt(4) == _T('1'))
//	{
//		pWnd->GetDlgItem(IDC_BTN_FIDUCIAL)->EnableWindow(false);
//	}
//	if( data.GetAt(5) == _T('1'))
//	{
//		pWnd->GetDlgItem(IDC_BTN_MOTION)->EnableWindow(false);
//	}
//	if( data.GetAt(6) == _T('1'))
//	{
//		pWnd->GetDlgItem(IDC_BTN_IO)->EnableWindow(false);
//	}
//	if( data.GetAt(7) == _T('1'))
//	{
//		pWnd->GetDlgItem(IDC_DF_OPEN)->EnableWindow(false);
//	}
//	if( data.GetAt(8) == _T('1'))
//	{
//		pWnd->GetDlgItem(IDC_DF_OPEN)->EnableWindow(false);
//	}
//	if( data.GetAt(9) == _T('1'))
//	{
//		pWnd->GetDlgItem(IDC_DF_OPEN)->EnableWindow(false);
//	}
//*/

}
