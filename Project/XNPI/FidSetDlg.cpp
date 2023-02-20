// FidSetDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SCInspector.h"
#include "FidSetDlg.h"
#include "FidAlgo.h"
#include "Status.h"
#include "SCInspectorView.h"
#include "Defines.h"
#include "OpPanelDlg.h"
#include <UniUtility.h>

// CFidSetDlg ��ȭ �����Դϴ�.

CFidSetDlg* CFidSetDlg::m_pInstance=NULL;
int CFidSetDlg::m_StopOn=FALSE;
CStarTestApp *CFidSetDlg::m_pGetMainApp=NULL;
IMPLEMENT_DYNAMIC(CFidSetDlg, CDialog)

CFidSetDlg::CFidSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFidSetDlg::IDD, pParent)
	, m_InspAngle(_T(""))
	, m_InspXOfs(_T(""))
	, m_InspYOfs(_T(""))
	, m_bFidType(FALSE)
{
	m_fIsInitialized=FALSE;

	Init();
}

CFidSetDlg::~CFidSetDlg()
{
}

void CFidSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB_FID, m_tabFidReg);
	DDX_Control(pDX, IDC_IMAGE, m_FidImgView);
	DDX_Text(pDX, IDC_EDT_SCORE, m_iScore);
	DDX_Text(pDX, IDC_EDT_SCALE, m_iScaleFactor);
	DDX_Check(pDX, IDC_CHK_USE_SCALE, m_UseScaleFactor);
	DDX_Text(pDX, IDC_EDT_FIDSIZEW, m_uiFidSizeWidth);
	DDX_Text(pDX, IDC_EDT_FROIW, m_uiROISizeWidth);
	DDX_Text(pDX, IDC_EDT_FIDSIZEH, m_uiFidSizeHeight);
	DDX_Text(pDX, IDC_EDT_FROIH, m_uiROISizeHeight);
	DDX_Control(pDX, IDC_SP_FIDW, m_spFidWidth);
	DDX_Control(pDX, IDC_SP_FIDH, m_spFidHeight);
	DDX_Control(pDX, IDC_SP_FROIW, m_spROIWidth);
	DDX_Control(pDX, IDC_SP_FROIH, m_spROIHeight);
	DDX_Check(pDX, IDC_CHK_ANGLE, m_bUseAngleFactor);
	DDX_Text(pDX, IDC_INSP_ANGLE, m_InspAngle);
	DDX_Text(pDX, IDC_INSP_XOFS, m_InspXOfs);
	DDX_Text(pDX, IDC_INSP_YOFS, m_InspYOfs);
	DDX_Radio(pDX, IDC_RA_FIDTYPE, m_bFidType);
}


BEGIN_MESSAGE_MAP(CFidSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_FIDREG, &CFidSetDlg::OnBnClickedBtnFidreg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_OK, &CFidSetDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CFidSetDlg::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_TRIAL, &CFidSetDlg::OnBnClickedBtnTrial)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FID, &CFidSetDlg::OnTcnSelchangeTabFid)
	ON_BN_CLICKED(IDC_BTN_TEST, &CFidSetDlg::OnBnClickedBtnTest)
	ON_MESSAGE(WM_SHOW_TRIALRST, &CFidSetDlg::OnShowTrialResult)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDT_FIDSIZEW, &CFidSetDlg::OnEnChangeEdtFidsize)
	ON_EN_CHANGE(IDC_EDT_FIDSIZEH, &CFidSetDlg::OnEnChangeEdtFidsize)
	ON_EN_CHANGE(IDC_EDT_FROIW, &CFidSetDlg::OnEnChangeEdtROISize)
	ON_EN_CHANGE(IDC_EDT_FROIH, &CFidSetDlg::OnEnChangeEdtROISize)
	ON_BN_CLICKED(IDC_RA_FIDTYPE, &CFidSetDlg::OnBnClickedRaFidtype)
	ON_BN_CLICKED(IDC_RA_FIDMOD, &CFidSetDlg::OnBnClickedRaFidtype)
	END_MESSAGE_MAP()


// CFidSetDlg �޽��� ó�����Դϴ�.

BOOL CFidSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_tabFidReg.InsertItem(0, _T("Fid 1"));
	m_tabFidReg.InsertItem(1, _T("Fid 2"));
	CMachineInfo::Instance()->LoadCFG();

	m_pGetMainApp =pGetMainApp;
	m_pApp=(CStarTestApp *)AfxGetApp();

#ifdef _USE_MATROX
	m_pMilFidDisp=m_pGetMainApp->m_pMilVLib->NewDisplay();

    
	MakeDefaultFid();
#endif
	m_fIsInitialized=TRUE;
   

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


/*! @fn void CFidSetDlg::OnBnClickedBtnFidreg()
	@brief ����ġ���� ������ũ�� ��Ͻ�Ŵ
*/
void CFidSetDlg::OnBnClickedBtnFidreg()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CAxisPos CurPos;

    pGetMainApp->m_pMilVLib->GetDigitizer(0)->m_pGrabChannel->SetAverageCounterSoft(0);

    m_pGetMainApp->m_pMilVLib->StopGrabContinuous();
    Sleep(100);
	m_pGetMainApp->m_pMilVLib->StopGrab();
	Sleep(100);
    
	int xrayFidVol = CCtrlPanelDlg::Instance()->X_ray_value_Vol;
	int xrayFidCur = CCtrlPanelDlg::Instance()->X_ray_value_Cur;

	int fidCur=m_tabFidReg.GetCurSel();

	m_pRegFid->m_pMilBuffer[fidCur]->Close();
	m_pRegFid->m_pMilBuffer[fidCur]->SetDepth(16);
	m_pRegFid->m_pMilBuffer[fidCur]->SetAttribute(M_IMAGE+M_PROC+M_DISP);
	m_pRegFid->m_pMilBuffer[fidCur]->Alloc2D(CSize(m_pMainDlg->m_pViewDlg->m_FidRect.Width()+1,m_pMainDlg->m_pViewDlg->m_FidRect.Height()+1));
	m_pRegFid->m_pMilBuffer[fidCur]->Clear();
	m_pRegFid->m_pMilBuffer[fidCur]->SetDisplay(m_pMilFidDisp);
	m_pRegFid->m_pMilBuffer[fidCur]->GetDisplay()->Control(M_FILL_DISPLAY,M_ENABLE);
	m_pRegFid->m_pMilBuffer[fidCur]->GetDisplay()->SetViewAutoScale();
	m_pRegFid->m_xrayCdt[fidCur].m_dblVol=xrayFidVol;
	m_pRegFid->m_xrayCdt[fidCur].m_dblCur=xrayFidCur;
	m_pRegFid->m_CamStatus[fidCur].bImgEnhance = CMachineInfo::Instance()->GetImageEnhance();
	m_pRegFid->m_CamStatus[fidCur].bnState = CMachineInfo::Instance()->GetBrightNormalize();
	m_pRegFid->m_CamStatus[fidCur].bScrnNormalize = CMachineInfo::Instance()->GetScrnNormalize();

	m_pMainDlg->m_pMilCenterView->CopyColor2D(m_pRegFid->m_pMilBuffer[fidCur], m_pMainDlg->m_pViewDlg->m_FidRect);
	SetFidStatusDisp(fidCur);
	GetMotionAxisPos(&CurPos);
	m_pRegFid->RemoveAllModels(fidCur);
	m_pRegFid->Reg(fidCur, m_pRegFid->m_pMilBuffer[fidCur], &CurPos);
}

void CFidSetDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if(!m_pRegFid)
	{
		MakeDefaultFid();
	}
	GetFidParam(0);

	if(m_pGetMainApp->m_ModelData.IsEmpty())
	{
		m_uiFidSizeWidth = m_pMainDlg->m_pViewDlg->m_FidRect.Width();
		m_uiFidSizeHeight = m_pMainDlg->m_pViewDlg->m_FidRect.Height();
		m_uiROISizeWidth = m_pMainDlg->m_pViewDlg->m_FidROI.Width();
		m_uiROISizeHeight = m_pMainDlg->m_pViewDlg->m_FidROI.Height();

	}else
	{
		OnEnChangeEdtFidsize();	
	}

	SetFidStatusDisp(0);

	UpdateData(FALSE);
	m_prevTeachingMode=CStatusInfo::Instance()->GetTeachingMode();
	CStatusInfo::Instance()->SetTeachingMode(CStatusInfo::TEACHING_FID);
}

/*! @fn void CFidSetDlg::GetMotionAxisPos(CAxisPos *pPos)
	@brief ����ġ������ �κ���ġ�� �д´�(������ũ ��Ͽ� ���
*/
void CFidSetDlg::GetMotionAxisPos(CAxisPos *pPos)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	double Xpos,Ypos,Zpos,Tpos;  ///�ప�������ϱ����� �ӽ� ����
	//CMotionCtrl::Instance()->GetPosition(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_X,&Xpos);
	//CMotionCtrl::Instance()->GetPosition(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_Y,&Ypos);
	//CMotionCtrl::Instance()->GetPosition(CMotionCtrl::Instance()->m_MOTION_AXIS_Z,&Zpos);
	//CMotionCtrl::Instance()->GetPosition(CMotionCtrl::Instance()->m_MOTION_AXIS_T,&Tpos);
     Xpos=CMotionInfoView::GetPosX_Stage_um;
	 Ypos=CMotionInfoView::GetPosY_Stage_um;
	 Zpos=CMotionInfoView::GetPosZ_um;
	 Tpos=CMotionInfoView::GetPosT_um;
	pPos->x = (int)Xpos;
	pPos->y = (int)Ypos;
	pPos->z = (int)Zpos;
	pPos->t = (int)Tpos;
}

void CFidSetDlg::OnBnClickedBtnOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!m_pGetMainApp->GetModelData()->IsExist(m_pRegFid))
		m_pGetMainApp->GetModelData()->AddElementHead(m_pRegFid);

	CStatusInfo::Instance()->SetTeachingMode(m_prevTeachingMode);
	CViewDlg::Instance()->NewRectData(); // �ǵ�� ���̵������ ����� .. ���� 052412
	CViewDlg::Instance()->ClearOverlay();
	pGetMainApp->m_pMilVLib->GetDigitizer(0)->m_pGrabChannel->SetAverageCounterSoft(CMachineInfo::m_iAvgSoft);
	CMainDlg::fidModeOn=false;
	OnOK();
}

void CFidSetDlg::OnBnClickedBtnCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CViewDlg::Instance()->NewRectData(); // �ǵ�� ���̵������ ����� .. ���� 052412
	CStatusInfo::Instance()->SetTeachingMode(m_prevTeachingMode);
	CMainDlg::fidModeOn=false;
	CViewDlg::Instance()->ClearOverlay();
	pGetMainApp->m_pMilVLib->GetDigitizer(0)->m_pGrabChannel->SetAverageCounterSoft(CMachineInfo::m_iAvgSoft);// ����Ʈ���� ���������� ���� 12 06 15
	OnCancel();
}

/*! @fn CRect CFidSetDlg::GetROIRect(int index)
	@brief ��ϵǾ��ִ� ROI�� �����
	@param index ������ũ ��ȣ
	@return ROI�� CRect���� ��ȯ��
*/
CRect CFidSetDlg::GetROIRect(int index)
{
	CSize ImageSize;
	m_uiROISizeWidth	=	m_pRegFid->m_upROI[index].cx;
	m_uiROISizeHeight	=	m_pRegFid->m_upROI[index].cy;
	m_pGetMainApp->m_pMilVLib->GetDigitizer(0)->GetResolution(ImageSize);
	return CRect(ImageSize.cx/2-m_uiROISizeWidth/2, ImageSize.cy/2-m_uiROISizeHeight/2, ImageSize.cx/2+m_uiROISizeWidth/2, ImageSize.cy/2+m_uiROISizeWidth/2);
}

/*! @fn CMilBuffer *CFidSetDlg::GetROIChild(int index)
	@brief ��ϵǾ��ִ� ROI�� Child Mil Buffer�� �����Ͽ� ��ȯ�Ѵ�
	@param index ������ũ ��ȣ
	@return Child������ Mil Buffer�� ��ȯ�Ѵ�
*/
CMilBuffer *CFidSetDlg::GetROIChild(int index)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CRect FidROI = GetROIRect(index);

	return m_pMainDlg->m_pMilCenterView->Child2D(&FidROI);
}

/*! @fn void CFidSetDlg::GetFidOffset()
	@brief ������ũ�� �о� ���� ��ġ Ʋ������ �����Ѵ�.
	@n ����� ,m_pRegFid->m_upOffset�� ����
*/
int CFidSetDlg::GetFidOffset()
{   

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	 CStarTestApp *m_pApp=(CStarTestApp *)AfxGetApp();

	 m_pRegFid->m_upOffset[0].x=0;
	 m_pRegFid->m_upOffset[0].y=0;
	 m_pRegFid->m_umOffset[0].x=0;
	 m_pRegFid->m_umOffset[0].y=0;
	 m_pRegFid->m_theta=0;

	if(!m_pRegFid->m_nFidCnt)
	{
		CString FiduLog;
		FiduLog.Format(_T("m_pRegFid->m_nFidCnt : %d "),m_pRegFid->m_nFidCnt);
		CTabDetailLogDlg::Instance()->SetTextLog(FiduLog,RGB(0,255,0),false);
	    return -1;
	}
		
	if(m_pRegFid->m_nFidCnt>=1)
	{
		for(int i=0;i<m_pRegFid->m_nFidCnt;i++)
		{   
			long xrayVol=m_pRegFid->m_xrayCdt[i].m_dblVol;
			long xrayCur=m_pRegFid->m_xrayCdt[i].m_dblCur;
			CCtrlPanelDlg::Instance()->XrayAutoSetVol(xrayVol);//
			CCtrlPanelDlg::Instance()->XrayAutoSetCur(xrayCur);//
			
			BackupCamStatus(i);
			ChangeCamToFidStatus(i);

			

			CMotionCtrl::Instance()->GoPositionMove(CMotionCtrl::Instance()->m_MOTION_AXIS_Z,m_pRegFid->m_lFidPos[i].z,CMachineInfo::m_FrameMoveZSpeed/4,200,200);					   
			CMotionCtrl::Instance()->GoPositionMove(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_X,m_pRegFid->m_lFidPos[i].x,CMachineInfo::m_FrameMoveZSpeed/4,200,200);					   
			CMotionCtrl::Instance()->GoPositionMove(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_Y,m_pRegFid->m_lFidPos[i].y,CMachineInfo::m_FrameMoveZSpeed/4,200,200);	
			while(CMotionCtrl::Instance()->AllMotionMoveDone());
			Sleep(90);// ����ȭ �ð� 

			
			WaitForXrayStable(xrayVol, xrayCur);
			 m_pApp->m_pMilVLib->SingleGrab();
			
			//�ȼ��������� 
			double Posz=0;
			//CMotionCtrl::Instance()->GetPosition(CMotionCtrl::Instance()->m_MOTION_AXIS_Z,&Posz);
			Posz = CMotionInfoView::GetPosZ_um;
			CPixelCalDlg::Instance()->CalpixelSize(Posz);
			CdblSize Pixelsize; 
			Pixelsize.dblCX=CPixelCalDlg::m_PixeclSizeX;
			Pixelsize.dblCY=CPixelCalDlg::m_PixeclSizeY;
		
			CMilBuffer *m_pMilROIArea = GetROIChild(i);
     
			if(m_pRegFid->Find(i, m_pMilROIArea))
			{   ///������ũ�� ã�� ��������
				//AfxMessageBox(IDS_FIDERROR, MB_OK);
				CString err;
				err.LoadStringW(IDS_FIDERROR);
				CTabDetailLogDlg::Instance()->SetTextLog(err,RGB(0,255,0),false);
				m_pMilROIArea->Close();
				RestoreCamStatus();
				return -2; 
				break;
			}else
			{	///������ũ�� ã������
				CRect ROIRect= GetROIRect(i);
				m_pRegFid->SetDrawOffset(CSize(-ROIRect.left,-ROIRect.top));
				m_pRegFid->Draw();
				m_pRegFid->SetDrawOffset();
			}
			m_pMilROIArea->Close();
			m_pRegFid->SetPixelScale(Pixelsize);
		}

		RestoreCamStatus();

		if(m_pRegFid->m_nFidCnt>=2)
			m_pRegFid->CalcTheta();
		CString FiduLog;
		FiduLog.Format(_T("FIDTest x:%f y:%f"),m_pRegFid->m_upOffset[0].x, m_pRegFid->m_upOffset[0].y);
		CTabDetailLogDlg::Instance()->SetTextLog(FiduLog,RGB(0,255,0),false);
		TRACE(_T("\nFIDTest x:%f y:%f"),m_pRegFid->m_upOffset[0].x, m_pRegFid->m_upOffset[0].y);
#ifdef _DEBUG
		CDbgFile::Instance()->Write(FiduLog.GetBuffer());
#endif
				//m_pRegFid->GetResultStr();
	}    

	 return 0;
}

/*! @fn void CFidSetDlg::OnBnClickedBtnTest()
	@brief ��ϰ������� ��ϵ� ������ũ�� ã�ƺ���(�� ��ġ���� ã�´�)
	@n ����� ȭ�鿡 �׸�
*/
void CFidSetDlg::OnBnClickedBtnTest()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(m_pRegFid->IsEmpty())
	{
		AfxMessageBox(IDS_FID_NOT_TEACHED, MB_OK);
		return;
	}

	CMilBuffer *m_pMilROIArea = GetROIChild(m_tabFidReg.GetCurSel());

	if(!m_pRegFid->Find(m_tabFidReg.GetCurSel(), m_pMilROIArea))
	{
		CRect ROIRect= GetROIRect(m_tabFidReg.GetCurSel());
		m_pRegFid->SetDrawOffset(CSize(-ROIRect.left,-ROIRect.top));
		m_pRegFid->Draw();
		m_pRegFid->SetDrawOffset();
	}
	m_pMilROIArea->Close();
}

/*! @fn void CFidSetDlg::OnBnClickedBtnTrial()
	@brief ��ϰ������� ��ϵ� ������ũ�� ã�ƺ���(�κ��̵����� �Ͽ� ã�´�)
	@n ����� ȭ�鿡 �׸�
*/
void CFidSetDlg::OnBnClickedBtnTrial()
{  

	CMoveButtonDlg::Instance()->ButtonCtrlOff(true);///>��ư��Ʈ�Ѹ� ������Ŵ 
	CViewDlg::Instance()->NewRectData();
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	m_pGetMainApp->m_pMilVLib->StopGrabContinuous();

	double FOV_H,PixelSize,FOV_V;
	double AxisZ=(m_pRegFid->m_lFidPos[0].z);
	FOV_H=((CMachineInfo::Instance()->m_Tanx*((CMachineInfo::Instance()->m_MahcineFID-AxisZ*(CMachineInfo::Instance()->m_mm_Per_pulseZ*1000))))*2)/1000;
	PixelSize = FOV_H/CMachineInfo::Instance()->m_CameraPicxel.cx;
	FOV_V = (PixelSize*CMachineInfo::Instance()->m_CameraPicxel.cy);
	m_pRegFid->SetPixelScale(CdblSize(PixelSize,PixelSize));		

	Sleep(50);
	ApplyFidParam(m_tabFidReg.GetCurSel());
	AfxBeginThread(TrialThread, NULL, NULL);

}

/*! @fn int CFidSetDlg::LoadXml(void)
	@brief ������ũ������ ������ CFG���Ͽ��� �о�´�
*/
int CFidSetDlg::LoadXml(void)
{
	CCfgFile *pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir()+_T("\\..\\Setup\\")+CFG_FILE);
	pMyCfg->SetDepth(2);													
	if(!pMyCfg->Open())return -1;															

	m_FidMoveSpeed	= pMyCfg->ReadDouble(250000, _T("FidSet"), _T("RobotSpeed"));
	m_FidZPos	= pMyCfg->ReadDouble(0, _T("FidSet"), _T("RobotZPos"));
	pMyCfg->Flush();	
	
	return 0;
}

/*! @fn void CFidSetDlg::OnTcnSelchangeTabFid(NMHDR *pNMHDR, LRESULT *pResult)
	@brief TAB�� �ٲ� ȭ�鰻�� 
*/
void CFidSetDlg::OnTcnSelchangeTabFid(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SwitchFidStatusDisp(m_tabFidReg.GetCurSel());
	m_pMainDlg->m_pViewDlg->DrawFidTeachRect();

	*pResult = 0;
}

/*! @fn void CFidSetDlg::SetFidStatusDisp(int num)
	@brief ������ũ������ �̹����� ���Ž�Ų�� 
*/
void CFidSetDlg::SetFidStatusDisp(int num)
{
	if(m_pMilFidDisp->IsSelected())
	{
		m_pMilFidDisp->UnSelectDisp();
	}

	m_pRegFid->m_pMilBuffer[num]->SetDisplay(m_pMilFidDisp);
	m_pRegFid->m_pMilBuffer[num]->SelectWindow(m_FidImgView);
	m_pRegFid->m_pMilBuffer[num]->GetDisplay()->SetViewAutoScale();
	ApplyFidParam(num);
}

/*! @fn void CFidSetDlg::SwitchFidStatusDisp(int num)
	@brief num��ȣ�� �´� ������ũ�� �̹��� �� �Ķ���͸� ���Ž�Ų�� 
*/
void CFidSetDlg::SwitchFidStatusDisp(int num)
{
	if(!m_pRegFid->m_pMilBuffer[num])
	{
		MakeDefaultFid(num);		
	}

	if(m_pMilFidDisp->IsSelected())
	{
		m_pMilFidDisp->UnSelectDisp();
	}

	m_pRegFid->m_pMilBuffer[num]->SetDisplay(m_pMilFidDisp);
	m_pRegFid->m_pMilBuffer[num]->SelectWindow(m_FidImgView);
	m_pRegFid->m_pMilBuffer[num]->GetDisplay()->SetViewAutoScale();

	switch(num)
	{
	case 0:
		ApplyFidParam(1);
		GetFidParam(0);
		break;
	case 1:
		ApplyFidParam(0);
		GetFidParam(1);
		break;
	}
}

/*! @fn void CFidSetDlg::GetFidParam(int FidNum)
	@brief ������ũ���� �Ķ���͸� ������ 
*/
void CFidSetDlg::GetFidParam(int FidNum)
{
	m_iScore = m_pRegFid->m_iMatRate;
	m_iScaleFactor = m_pRegFid->m_iScaleRate;
	m_UseScaleFactor = m_pRegFid->m_iUseScale;
	m_bUseAngleFactor = m_pRegFid->m_iUseAngle;
	m_uiFidSizeWidth =m_pRegFid->m_upFid[FidNum].cx;
	m_uiFidSizeHeight =m_pRegFid->m_upFid[FidNum].cy;
	m_uiROISizeWidth =m_pRegFid->m_upROI[FidNum].cx;
	m_uiROISizeHeight =m_pRegFid->m_upROI[FidNum].cy;
	UpdateData(FALSE);
}

/*! @fn void CFidSetDlg::ApplyFidParam(int FidNum)
	@brief UI�� �Ķ���͸� ������ũ�� ������ 
*/
void CFidSetDlg::ApplyFidParam(int FidNum)
{
	UpdateData(TRUE);
	m_pRegFid->m_iMatRate = m_iScore;
	m_pRegFid->m_iScaleRate = m_iScaleFactor;
	m_pRegFid->m_iUseScale = m_UseScaleFactor;
	m_pRegFid->m_iUseAngle = m_bUseAngleFactor;

	m_pRegFid->m_upFid[FidNum].cx = m_uiFidSizeWidth;
	m_pRegFid->m_upFid[FidNum].cy = m_uiFidSizeHeight;
	m_pRegFid->m_upROI[FidNum].cx = m_uiROISizeWidth;
	m_pRegFid->m_upROI[FidNum].cy = m_uiROISizeHeight;
}

CFidSetDlg * CFidSetDlg::Instance(void) 
{
	if(!m_pInstance)m_pInstance=new CFidSetDlg; 

	return m_pInstance;
}


void CFidSetDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	delete m_pRegFid;
	m_pRegFid=NULL;
	CDialog::OnClose();
}

void CFidSetDlg::OnEnChangeEdtFidsize()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!m_fIsInitialized)return;

	UpdateData(TRUE);
	m_uiFidSizeWidth=(m_uiFidSizeWidth>=(m_uiROISizeWidth-10))?(m_uiROISizeWidth-10):m_uiFidSizeWidth;
	m_uiFidSizeHeight=(m_uiFidSizeHeight>=(m_uiROISizeHeight-10))?(m_uiROISizeHeight-10):m_uiFidSizeHeight;
	UpdateData(FALSE);

	CSize FidSize=CSize(m_uiFidSizeWidth, m_uiFidSizeHeight);

	m_pMainDlg->m_pViewDlg->ApplyFidSize();
	m_pMainDlg->m_pViewDlg->DrawFidTeachRect();
}

/*! @fn void CFidSetDlg::OnEnChangeEdtROISize()
	@brief UI�� ROI�� �׸��� BOX�� ũ�⸦ �����Ͽ� �帰��.
*/
void CFidSetDlg::OnEnChangeEdtROISize()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!m_fIsInitialized)return;

	UpdateData(TRUE);
	m_uiROISizeWidth=(m_uiFidSizeWidth>=(m_uiROISizeWidth-10))?(m_uiFidSizeWidth+10):m_uiROISizeWidth;
	m_uiROISizeHeight=(m_uiFidSizeHeight>=(m_uiROISizeHeight-10))?(m_uiFidSizeHeight+10):m_uiROISizeHeight;
	UpdateData(FALSE);

	CSize FidROISize=CSize(m_uiROISizeWidth, m_uiROISizeHeight);
	m_pMainDlg->m_pViewDlg->ApplyFidROISize();
	m_pMainDlg->m_pViewDlg->DrawFidTeachRect();
}

int CFidSetDlg::RemoveAll(void)
{
	for(int i=0;i<2;i++)
	{
		if(!m_pRegFid->m_pMilBuffer[i])continue;
		if(!m_pRegFid->m_pMilBuffer[i]->GetDisplay())continue;
		if(m_pRegFid->m_pMilBuffer[i]->GetDisplay()->IsSelected())
		{
			m_pRegFid->m_pMilBuffer[i]->UnSelectDisp();
		}
	}
	
	Init();
	return 0;
}

int CFidSetDlg::MakeDefaultFid(void)
{
	m_pMainDlg =pGetMainDlg;
	m_pRegFid = new CFidAlgo;
	m_spFidWidth.SetRange(60, 250);
	m_spFidHeight.SetRange(60, 250);
	m_spROIWidth.SetRange(60, 300);
	m_spROIHeight.SetRange(60, 300);


	for(int i=0;i<4;i++)
	{
		ApplyFidParam(i);
	}

	m_pRegFid->m_pMilBuffer[0] = m_pGetMainApp->m_pMilVLib->NewImage();
	m_pRegFid->m_pMilBuffer[0]->SetDepth(16);
	m_pRegFid->m_pMilBuffer[0]->SetAttribute(M_IMAGE+M_PROC+M_DISP);
	m_pRegFid->m_pMilBuffer[0]->Alloc2D(CSize(30,30));
	m_pRegFid->m_pMilBuffer[0]->Clear();
	m_pRegFid->m_pMilBuffer[0]->SetDisplay(m_pMilFidDisp);
	m_pRegFid->m_pMilBuffer[0]->GetDisplay()->Control(M_FILL_DISPLAY,M_ENABLE);
	m_pRegFid->m_pMilBuffer[0]->GetDisplay()->SetViewAutoScale();

	m_pRegFid->m_pMilBuffer[1] = m_pGetMainApp->m_pMilVLib->NewImage();
	m_pRegFid->m_pMilBuffer[1]->SetDepth(16);
	m_pRegFid->m_pMilBuffer[1]->SetAttribute(M_IMAGE+M_PROC+M_DISP);
	m_pRegFid->m_pMilBuffer[1]->Alloc2D(CSize(30,30));
	m_pRegFid->m_pMilBuffer[1]->Clear();
	m_pRegFid->m_pMilBuffer[1]->SetDisplay(m_pMilFidDisp);
	m_pRegFid->m_pMilBuffer[1]->GetDisplay()->Control(M_FILL_DISPLAY,M_ENABLE);
	m_pRegFid->m_pMilBuffer[1]->GetDisplay()->SetViewAutoScale();
	m_pRegFid->m_pMilBuffer[1]->SelectWindow(m_FidImgView);
	///m_pRegFid�� ROI������ ����ȭ��Ű������ ����
	for(int i=0;i<4;i++)
		m_pROI[i]=&m_pRegFid->m_upROI[i];

	LoadXml();
	CString Dirstr = CMachineInfo::Instance()->m_LengDir;
	CChangeRes::Instance()->ChangeDialogCaption(Dirstr,this);

	return 0;
}

int CFidSetDlg::MakeDefaultFid(int num)
{
	m_spFidWidth.SetRange(60, 250);
	m_spFidHeight.SetRange(60, 250);
	m_spROIWidth.SetRange(60, 300);
	m_spROIHeight.SetRange(60, 300);

	ApplyFidParam(num);

	m_pRegFid->m_pMilBuffer[num] = m_pGetMainApp->m_pMilVLib->NewImage();
	m_pRegFid->m_pMilBuffer[num]->SetDepth(16);
	m_pRegFid->m_pMilBuffer[num]->SetAttribute(M_IMAGE+M_PROC+M_DISP);
	m_pRegFid->m_pMilBuffer[num]->Alloc2D(CSize(30,30));
	m_pRegFid->m_pMilBuffer[num]->Clear();
	m_pRegFid->m_pMilBuffer[num]->SetDisplay(m_pMilFidDisp);
	m_pRegFid->m_pMilBuffer[num]->GetDisplay()->Control(M_FILL_DISPLAY,M_ENABLE);
	m_pRegFid->m_pMilBuffer[num]->GetDisplay()->SetViewAutoScale();

	///m_pRegFid�� ROI������ ����ȭ��Ű������ ����
	m_pROI[num]=&m_pRegFid->m_upROI[num];

	return 0;
}

int CFidSetDlg::Init(void)
{
	m_iScore			=70;
	m_iScaleFactor		=0;
	m_UseScaleFactor	=FALSE;
	m_FidZPos			=0;
	m_pRegFid			=NULL;
	m_uiFidSizeWidth	=60;
	m_uiROISizeWidth	=180;
	m_uiFidSizeHeight	=60;
	m_uiROISizeHeight	=180;
	m_bUseAngleFactor	=FALSE;
	m_pRegFid			=NULL;

	memset(&m_backCamStatus, 0, sizeof(m_backCamStatus));
	return 0;
}

void CFidSetDlg::ShowInspResult(void)
{
	m_InspAngle.Format(_T("%3.1f"), m_pRegFid->m_theta);
	m_InspXOfs.Format(_T("%5.1f"), m_pRegFid->m_upOffset[0].x);
	m_InspYOfs.Format(_T("%5.1f"), m_pRegFid->m_upOffset[0].y);
	UpdateData(FALSE);
}

UINT CFidSetDlg::TrialThread(LPVOID pParam)
{   
	pGetMainApp->m_pMilVLib->GetDigitizer(0)->m_pGrabChannel->SetAverageCounterSoft(0);  // ����Ʈ���� �������� ������ 12 06 15 ����
	static BOOL reEnter=FALSE;
	if(reEnter)return 0;
	reEnter=TRUE;
	//m_pGetMainApp->m_pMilVLib->StopGrab();
	if(!Instance()->GetFidOffset())Instance()->PostMessage(WM_SHOW_TRIALRST);
//		ShowInspResult();
	Sleep(100);
	m_pGetMainApp->m_pMilVLib->GrabContinuous();
	reEnter=FALSE;
	pGetMainApp->m_pMilVLib->GetDigitizer(0)->m_pGrabChannel->SetAverageCounterSoft(CMachineInfo::m_iAvgSoft);// ����Ʈ���� �������� �� -12 -06 15
	return 0;
}

LRESULT CFidSetDlg::OnShowTrialResult(WPARAM wParam, LPARAM lParam)
{
	ShowInspResult();
	return LRESULT();
}

int CFidSetDlg::GetFidVol(int nFid)
{
	int Vol;
	if(m_pRegFid)
	{
		Vol=m_pRegFid->m_xrayCdt[nFid].m_dblVol;
	}else
	{
		Vol=CCtrlPanelDlg::Instance()->X_ray_value_Vol;	
	}

	return Vol;
}

int CFidSetDlg::GetFidCur(int nFid)
{
	int Cur;
	if(m_pRegFid)
	{
		Cur=m_pRegFid->m_xrayCdt[nFid].m_dblCur;
	}else
	{
		Cur=CCtrlPanelDlg::Instance()->X_ray_value_Cur;	
	}

	return Cur;
}

int CFidSetDlg::WaitForXrayStable(long xrayVol, long xrayCur)
{
	//// �������̰��� ��ǥġ�� �����ߴ��� üũ�ϴ� ��ƾ
	int Xrayerrcount=0;
	m_StopOn=FALSE;
	while(1)
	{  
	 if(m_StopOn==1){break;}
	 CCtrlPanelDlg::Instance()->GetVotageCurrentXray();
	
	if (CCtrlPanelDlg::Instance()->X_ray_value_Vol==xrayVol && CCtrlPanelDlg::Instance()->X_ray_value_Cur==xrayCur)
	{ 
			CString err;
			err.Format(_T("X-ray Voltage:%d Current:%d Check"),CCtrlPanelDlg::Instance()->X_ray_value_Vol,CCtrlPanelDlg::Instance()->X_ray_value_Cur);
			CTabDetailLogDlg::Instance()->SetTextLog(err,RGB(0,255,0),false);
			err.Empty();
	   
	        break;  // ����� �������̰��� �����ϸ� ��������
	}
	
	
	    CMainDlg::XrayOffControltime=0;
		if(Xrayerrcount>=300)
		{   
		    //m_pApp->m_pMilVLib->StopGrab();
			//m_pApp->m_pMilVLib->StopGrabContinuous();
		   // Sleep(1000);
			//m_pApp->m_pMilVLib->GrabContinuous();
			JOYCTRL::Instance()->PIPStopModeJoy(true);
			CString err;
			err.Format(_T("X-ray Voltage&&Current err"));
			CTabDetailLogDlg::Instance()->SetTextLog(err,RGB(0,255,0),false);
			err.Empty();
		    m_StopOn=1;
			Xrayerrcount=0;
			break;
		}// �������̰������ð����� ��ǥġ�� ������ ������ ��������ž
	        Sleep(5);
	        Xrayerrcount=Xrayerrcount+1;
	}
   /// ��������Ȯ�� ��ƾ ��
	return 0;
}

void CFidSetDlg::BackupCamStatus(int nFidNo)
{
	m_backCamStatus.bImgEnhance =	m_pApp->m_pMilVLib->GetDigitizer(0)->m_pGrabChannel->GetImgEnhance();
	m_backCamStatus.bnState = m_pApp->m_pMilVLib->GetDigitizer(0)->m_pGrabChannel->GetBrightNormalize();
	m_backCamStatus.bScrnNormalize = m_pApp->m_pMilVLib->GetDigitizer(0)->m_pGrabChannel->GetScrnNormalize();
}

void CFidSetDlg::RestoreCamStatus()
{
	m_pApp->m_pMilVLib->GetDigitizer(0)->m_pGrabChannel->SetImgEnhance( m_backCamStatus.bImgEnhance);
	m_pApp->m_pMilVLib->GetDigitizer(0)->m_pGrabChannel->SetBrightNormalize( m_backCamStatus.bnState);
	m_pApp->m_pMilVLib->GetDigitizer(0)->m_pGrabChannel->SetScrnNormalize(m_backCamStatus.bScrnNormalize);
}

void CFidSetDlg::ChangeCamToFidStatus(int nFidNo)
{
	m_pApp->m_pMilVLib->GetDigitizer(0)->m_pGrabChannel->SetImgEnhance(m_pRegFid->m_CamStatus[nFidNo].bImgEnhance);
	m_pApp->m_pMilVLib->GetDigitizer(0)->m_pGrabChannel->SetBrightNormalize(m_pRegFid->m_CamStatus[nFidNo].bnState);
	m_pApp->m_pMilVLib->GetDigitizer(0)->m_pGrabChannel->SetScrnNormalize(m_pRegFid->m_CamStatus[nFidNo].bScrnNormalize);
}

void CFidSetDlg::OnBnClickedRaFidtype()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	int i=m_bFidType;
}
