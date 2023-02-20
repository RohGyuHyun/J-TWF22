// SnapshotDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CamDlg.h"
#include "SnapshotDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Status.h"
#include "TabDetailLogDlg.h"
#include "Repository.h"
#include <list>
#include <iostream>
#include <gdiplus.h> //! ������ ������ gdiplus �Լ� ���

#define	THUMBNAIL_WIDTH		190
#define	THUMBNAIL_HEIGHT	190
#define	THUMBNAIL_GAP		10

//PC -> PLC
#define RESET			0001
#define PROC_DONE		0002
#define TRIGGER_REPEAT	0003
#define GO_TO_SECTION	0004

#define SNAPSHOT_SUBMENU_MOVETOPOS_ID 20000

// CSnapshotDlg ��ȭ �����Դϴ�.
IMPLEMENT_DYNAMIC(CSnapshotDlg, CDialogEx)

CSnapshotDlg::CSnapshotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SNAPSHOT_PANEL, pParent)
	, m_strPipeSectorNum(_T(""))
	, m_strImgSaveFolder(_T(""))
	, m_bIsNewPipe(TRUE)
{
}

CSnapshotDlg::~CSnapshotDlg()
{
}

BOOL CSnapshotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ImageListThumb.Create(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, ILC_COLOR32, 0, 1);
	m_ListCtrlSnapShot.SetImageList(&m_ImageListThumb, LVSIL_NORMAL);

	m_ListCtrlSnapShot.SetBkColor(RGB(234, 239, 245));

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pView = (CXNPIView *)pFrame->GetActiveView();

	m_vtImgNameList.reserve(150);	//! ������ ���� �Կ� 150���� ����Ұ����� �����ؼ� �Ҵ� - ���� ����ɼ� ����.

	m_Menu.CreatePopupMenu();	//! ������ ��Ŭ�� �˾��޴� - �ش� �������� ���� ��ġ�� PLC���� �̵��Ҽ� �ִ� �ɼ� ����
	m_Menu.AppendMenu(MF_STRING, SNAPSHOT_SUBMENU_MOVETOPOS_ID, _T("Move to Position"));

	m_mdManager = CModbusManager::GetInstance();


	//ó�� �Ӷ� ������ �˻� ���� �ε� - ���� �Ⱦ�
	CString lastFolderPath = CStatus::Instance()->m_strLastOpendFolder;
	//LoadInspectionFolder(lastFolderPath);

	return TRUE;
}

void CSnapshotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCTRL_SNAPSHOT, m_ListCtrlSnapShot);
	DDX_Control(pDX, IDC_BUTTON_RETAKE, m_btnRetake);
	DDX_Control(pDX, IDC_BUTTON_NEWPIPE, m_btnNewPipe);
	DDX_Text(pDX, IDC_EDIT_NUMER, m_strPipeSectorNum);
	DDX_Control(pDX, IDC_BUTTON_TAKE_IMAGE, m_btnTakeImage);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_btnReset);
	DDX_Control(pDX, IDC_BUTTON_TRIGGER_REPEAT, m_btnTriggerRepeat);
}

BEGIN_MESSAGE_MAP(CSnapshotDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_TAKE_IMAGE, &CSnapshotDlg::OnBnClickedButtonTakeImage)
	ON_BN_CLICKED(IDC_BUTTON_RETAKE, &CSnapshotDlg::OnBnClickedButtonRetake)
	ON_BN_CLICKED(IDC_BUTTON_NEWPIPE, &CSnapshotDlg::OnBnClickedButtonNewpipe)
	ON_NOTIFY(NM_CLICK, IDC_LISTCTRL_SNAPSHOT, &CSnapshotDlg::OnNMClickListctrlSnapshot)
	ON_EN_SETFOCUS(IDC_EDIT_NUMER, &CSnapshotDlg::OnEnSetfocusEditNumer)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CSnapshotDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_TRIGGER_REPEAT, &CSnapshotDlg::OnBnClickedButtonTriggerRepeat)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_BUTTON2, &CSnapshotDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

// CSnapshotDlg �޽��� ó�����Դϴ�.
BOOL CSnapshotDlg::PreTranslateMessage(MSG* pMsg)
{
	//Enter Closing ����
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			int nID = GetFocus()->GetDlgCtrlID();

			if (nID == IDC_EDIT_NUMER)
			{
				UpdateData(TRUE);

				int selNum = _ttoi(m_strPipeSectorNum);

				//selNum = selNum * 2 - 1;

				CString sctName;
				sctName.Format(_T("%d"), selNum);
				int index = FindSnapshotIndexByPartOfString(sctName);

				if (index != -1)
				{
					m_ListCtrlSnapShot.EnsureVisible(index, FALSE);

					m_ListCtrlSnapShot.SetFocus();
					m_ListCtrlSnapShot.SetItemState(index,
						LVIS_SELECTED | LVIS_FOCUSED,
						LVIS_SELECTED | LVIS_FOCUSED);
				}
			}

			return TRUE;
		}

		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//Design
BOOL CSnapshotDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(234, 239, 245));

	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

HBRUSH CSnapshotDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(90, 107, 135));  // static text ���ڻ� ����

		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}


//Button
void CSnapshotDlg::OnBnClickedButtonTakeImage()
{
	TakeImage();
}

void CSnapshotDlg::OnBnClickedButtonNewpipe()
{
	NewPipeInspectionStart();
}

void CSnapshotDlg::OnBnClickedButtonRetake()
{
	int index = m_ListCtrlSnapShot.GetNextItem(-1, LVIS_SELECTED);
	if (index == -1)
		return;

	RetakeImage(index);
}

void CSnapshotDlg::OnBnClickedButtonReset()
{
	SendResetCmd();
}

void CSnapshotDlg::OnBnClickedButtonTriggerRepeat()
{
	SendTriggerRepeatCmd();
}


//Send Modbus CMD
/*!
@fn void CSnapshotDlg::SendResetCmd()
@brief Reset Ŀ�ǵ� Send
*/
void CSnapshotDlg::SendResetCmd()	//Reset
{
	//log
	{
		GetLocalTime(&m_tCur_time);

		CString strTime;
		strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
			m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

		CString log = strTime + _T("[CMD SEND] Send [RESET] CMD");
		CTabDetailLogDlg::Instance()->SetTextLog(log, RGB(50, 50, 50), FALSE);
	}

	m_mdManager->WriteCmdData(RESET, 1);
	SetTimer(RESET, 1000, NULL);
}

/*!
	@fn void CSnapshotDlg::SendProcDondCmd()
	@brief Proc_Done Ŀ�ǵ� Send
*/
void CSnapshotDlg::SendProcDondCmd()	//Proc_Done
{
	//log
	{
		GetLocalTime(&m_tCur_time);

		CString strTime;
		strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
			m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

		CString log = strTime + _T("[CMD SEND] Send [PROC_DONE] CMD");
		CTabDetailLogDlg::Instance()->SetTextLog(log, RGB(50, 50, 50), FALSE);
	}

	m_mdManager->WriteCmdData(PROC_DONE, 1);
	SetTimer(PROC_DONE, 1000, NULL);
}

/*!
	@fn void CSnapshotDlg::SendTriggerRepeatCmd()
	@brief Trigger Repeat Ŀ�ǵ� Send
*/
void CSnapshotDlg::SendTriggerRepeatCmd()	//Trigger Repeat
{
	//log
	{
		GetLocalTime(&m_tCur_time);

		CString strTime;
		strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
			m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

		CString log = strTime + _T("[CMD SEND] Send [TRIGGER_REPEAT] CMD");
		CTabDetailLogDlg::Instance()->SetTextLog(log, RGB(50, 50, 50), FALSE);
	}

	m_mdManager->WriteCmdData(TRIGGER_REPEAT, 1);
	SetTimer(TRIGGER_REPEAT, 1000, NULL);
}

void CSnapshotDlg::SendGoToSection(int sectionIndex)
{
	//log
	{
		GetLocalTime(&m_tCur_time);

		CString strTime;
		strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
			m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

		CString log;
		log.Format(_T("[CMD SEND] Send [GO_TO_SECTION] CMD : %d"), sectionIndex);
		log = strTime + log;

		CTabDetailLogDlg::Instance()->SetTextLog(log, RGB(50, 50, 50), FALSE);
	}

	m_mdManager->WriteCmdData(GO_TO_SECTION, sectionIndex);
	SetTimer(GO_TO_SECTION, 1000, NULL);
}


//Take Image �޼���� ���� ȣ�� �޼����
void CSnapshotDlg::TakeImage()
{
	//---�ð� ����
	clock_t startTime, endTime;
	startTime = clock();

	m_pView->m_pOpPanelDlg->UpdateData(TRUE);
	m_pView->m_pToolPanelDlg->UpdateData(TRUE);

	//! pipe, �������� �ε�
	CString strTag = _T("");
	CString pipeNo = m_pView->m_pOpPanelDlg->m_strPipeInfoPipeNo;

	CString FPS = m_pView->m_pOpPanelDlg->m_strFpsValue;
	CString KV = m_pView->m_pOpPanelDlg->m_strKeV;
	CString MA = m_pView->m_pOpPanelDlg->m_strMA;
	CString avg = m_pView->m_pToolPanelDlg->m_strAvgVal;
	CTime curTime = CTime::GetCurrentTime();
	CString strTime = curTime.Format(_T("%Y.%m.%d_%H:%M:%S"));

	//! ���� ��� ������ ����
	CString resultPath;
	CString imgName;
	CString sectionName;

	sectionName.Format(_T("[%d-%d]"), pipeImgNum - 1, pipeImgNum);

	if (m_bIsNewPipe)
	{
		CreateInspectionImgFolder(curTime.Format(_T("%y_%m_%d")));
		CreatePipeForlder(pipeNo, curTime.Format(_T("%H%M%S")));
		m_bIsNewPipe = FALSE;
	}

	BOOL isSaveFormatJPG = CStatus::Instance()->m_bImgSaveFormatJPG;
	BOOL isSaveFormatTIF = CStatus::Instance()->m_bImgSaveFormatTIF;


	endTime = clock();
	printf("���� ���� ���� �ð�: %f\n", ((double)(endTime - startTime)) / CLOCKS_PER_SEC);
	//------------


	//---�ð� ����-------
	clock_t startTime2, endTime2;
	startTime2 = clock();

	BOOL saveWithRaw = CStatus::Instance()->m_bSaveWithRaw;

	if (isSaveFormatTIF)
	{
		imgName = CreateImgName(sectionName, curTime.Format(_T("%H%M%S")), _T("TIF"));

		resultPath = m_strImgSaveFolder + _T("\\");;
		resultPath += imgName;

		//Test check - 16��Ʈ �̹�������
		//SaveInspectionImg(resultPath, m_pView->m_pCamDlg[0]->m_pMilCamView, _T("TIF"));	//���� - 24��Ʈ�� ����

		if (saveWithRaw)	//190328
		{
			CString name = resultPath.Left(resultPath.ReverseFind(_T('.')));
			name += _T("_RAW.TIF");

			CMilBuffer *milBufRawTif = CRepository::Instance()->m_MilLib.NewImage(16);
			milBufRawTif->Alloc2D(CSize(CAM_HRES, CAM_VRES));

			//Test Check 190401 - ������ ������ ����ó��
			//��� ����ó������ milBufRawTif->Copy(m_pView->m_pCamDlg[0]->m_pMilTempBufAvgResult�ص� ������ �ʵ��׽�Ʈ ���� �ʾ� ���ǹ� �س���
			if (m_pView->m_pCamDlg[0]->m_pGrabChannel == NULL)
			{
				MessageBox(_T("ERROR: Grab Channel is NULL"), _T("In SnapshotDlg TakeImage() - Raw"),
					MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND | MB_TOPMOST);
				milBufRawTif->Copy(m_pView->m_pCamDlg[0]->m_pMilTempBufAvgResult);
			}
			else
			{
				milBufRawTif->Copy(m_pView->m_pCamDlg[0]->m_pGrabChannel->m_pMilTempBufAvgResult);
			}

			SaveInspectionImg(name, milBufRawTif, _T("TIF"));
			milBufRawTif->Close();
		}

		CMilBuffer *milBufTif = CRepository::Instance()->m_MilLib.NewImage(16);
		milBufTif->Alloc2D(CSize(CAM_HRES, CAM_VRES));

		//Test Check 190401 - ������ ������ ����ó��
		//��� ����ó������ milBufRawTif->Copy(m_pView->m_pCamDlg[0]->m_pMilTempBufAvgResult�ص� ������ �ʵ��׽�Ʈ ���� �ʾ� ���ǹ� �س���
		if (m_pView->m_pCamDlg[0]->m_pGrabChannel == NULL)
		{
			MessageBox(_T("ERROR: Grab Channel is NULL"), _T("In SnapshotDlg TakeImage() - Tif"),
				MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND | MB_TOPMOST);
			milBufTif->Copy(m_pView->m_pCamDlg[0]->m_pMilTempBufAvgResult_histo);
		}
		else
		{
			milBufTif->Copy(m_pView->m_pCamDlg[0]->m_pGrabChannel->m_pMilTempBufAvgResult_histo);
		}

		OutputDebugString(_T("\nSaved"));
		DrawInfosMgraText(milBufTif, pipeImgNum);
		SaveInspectionImg(resultPath, milBufTif, _T("TIF"));
		milBufTif->Close();
	}

	endTime2 = clock();
	printf("�̹��� ���� ���� �ð�1: %f\n", ((double)(endTime2 - startTime2)) / CLOCKS_PER_SEC);
	//------------


	//---�ð� ����-------
	clock_t startTime3, endTime3;
	startTime3 = clock();
	m_vtImgNameList.push_back(imgName);

	//log
	{
		GetLocalTime(&m_tCur_time);

		CString strTime;
		strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
			m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

		int sectionNum = pipeImgNum;
		CString log = strTime + _T("[SYS] Image Inserted");
		log.Format(_T("%s : [%d-%d]"), log, sectionNum - 1, sectionNum);

		CTabDetailLogDlg::Instance()->SetTextLog(log, RGB(50, 50, 50), FALSE);
	}

	pipeImgNum++;

	endTime3 = clock();
	printf("�̹��� ���� ���� �ð�2: %f\n", ((double)(endTime3 - startTime3)) / CLOCKS_PER_SEC);
	//------------

	DrawThumbnail(resultPath, sectionName);

	//! Tiff�� tag �߰�
	if (isSaveFormatTIF)
	{
		strTag += _T("Section: ") + sectionName + _T(", "); //kiy 0906 - �±׿� �������� �߰�
		CString line1 = m_pView->m_pOpPanelDlg->m_strInfoLine1;
		CString line2 = m_pView->m_pOpPanelDlg->m_strInfoLine2;

		strTag += _T("Line1: ") + line1 + _T(", ");
		strTag += _T("Line2: ") + line2 + _T(", ");

		strTag += _T("FPS: ") + FPS + _T(", ");
		strTag += _T("KV: ") + KV + _T(", ");
		strTag += _T("MA: ") + MA + _T(", ");
		strTag += _T("Avg: ") + avg + _T(", ");
		strTag += _T("Time: ") + strTime;

		AttachTag(resultPath, strTag);
	}
}

void CSnapshotDlg::DrawInfosMgraText(CMilBuffer * srcBuf, int num)
{
	CView *view = ((CMainFrame *)AfxGetMainWnd())->GetActiveView();
	CXNPIView* pView = (CXNPIView *)view;

	CTime curTime = CTime::GetCurrentTime();
	CString strTime = curTime.Format(_T("%Y.%m.%d_%H:%M:%S"));

	CString strTagPipeInfo_1 = _T("");
	CString strTagPipeInfo_2 = _T("");
	CString strTagTimeInfo = _T("");

	BOOL timeDisplay = pView->m_pOpPanelDlg->m_bTimeDisplay;
	BOOL pipeInfoDisplay = pView->m_pOpPanelDlg->m_bPipeInfoDisplay;

	MIL_DOUBLE TextPosX = 50.0;
	MIL_DOUBLE TextPosY = 30.0;
	MIL_DOUBLE StepYOffset = 38.0;

	MIL_DOUBLE TextLeftSectPosY = 1000.0;
	MIL_DOUBLE TextRightSectPosY = 1000.0;

	//
	double eachSectionConstOffset = CStatus::Instance()->m_pipeSectEachDistance_mm;

	double xPixelSize = CStatus::Instance()->m_dPixelXSize_mm;
	double yPixelSize = CStatus::Instance()->m_dPixelYSize_mm;

	double xa = eachSectionConstOffset / xPixelSize;
	double ya = eachSectionConstOffset / yPixelSize;

	MIL_DOUBLE TextLeftSectPosX = (CAM_VRES - xa) / 2;
	MIL_DOUBLE TextRightSectPosX = CAM_VRES - TextLeftSectPosX;
	//

	//Comp, SEPC, Size, Pipe No���� ǥ�þ���
	if (pipeInfoDisplay)
	{
		//1��° ��
		strTagPipeInfo_1 += CStatus::Instance()->m_strInfoLine1;

		//2��° ��
		strTagPipeInfo_2 += CStatus::Instance()->m_strInfoLine2;
	}

	if (timeDisplay)
	{
		//3��° ��
		strTagTimeInfo += _T("Time: ") + strTime;
	}

	MgraFontScale(M_DEFAULT, 1.7, 1.7);

	MgraText(M_DEFAULT, srcBuf->GetIDBuffer(), TextPosX, TextPosY, strTagPipeInfo_1);
	MgraText(M_DEFAULT, srcBuf->GetIDBuffer(), TextPosX, TextPosY + StepYOffset, strTagPipeInfo_2);
	MgraText(M_DEFAULT, srcBuf->GetIDBuffer(), TextPosX, TextPosY + StepYOffset * 2, strTagTimeInfo);

	CString frontPiptImgNum, backPiptImgNum;
	frontPiptImgNum.Format(_T("%d"), num - 1);
	backPiptImgNum.Format(_T("%d"), num);

	BOOL isSectDisp = m_pView->m_pOpPanelDlg->m_bSectionNumDisplay;
	BOOL isDistanceDisp = m_pView->m_pOpPanelDlg->m_bDistanceDisplay;

	if (isSectDisp)
	{
		MgraText(M_DEFAULT, srcBuf->GetIDBuffer(), TextLeftSectPosX, TextLeftSectPosY, frontPiptImgNum);
		MgraText(M_DEFAULT, srcBuf->GetIDBuffer(), TextRightSectPosX, TextRightSectPosY, backPiptImgNum);
	}

	if (isDistanceDisp)
	{
		double startPos = CStatus::Instance()->m_pipeStartPos_mm;
		double eachDist = CStatus::Instance()->m_pipeSectEachDistance_mm;

		CString curPos;
		curPos.Format(_T("%.2f"), (num - 1)* eachDist + startPos);
		curPos += _T(" mm");
		MgraText(M_DEFAULT, srcBuf->GetIDBuffer(), TextLeftSectPosX, TextLeftSectPosY + StepYOffset, curPos);
	}
}

void CSnapshotDlg::CreateInspectionImgFolder(CString time)
{
	CString strImgSaveFolder = CStatus::Instance()->m_strIMGSaveFolder;
	strImgSaveFolder += _T("\\Pipe Inspection");

	m_strImgSaveFolder = strImgSaveFolder + _T("\\") + time;

	CFileInfo::CreateFolder(m_strImgSaveFolder.GetBuffer());
}

void CSnapshotDlg::CreatePipeForlder(CString pipeNo, CString time)
{
	CString pipeName = pipeNo;
	CString pipeFolderName;
	pipeFolderName.Format(_T("[%s]_[%s]"), pipeName, time);

	m_strImgSaveFolder += _T("\\") + pipeFolderName;
	CFileInfo::CreateFolder(m_strImgSaveFolder.GetBuffer());


	CStatus::Instance()->m_strLastOpendFolder = m_strImgSaveFolder;
	CStatus::Instance()->SaveLastOpendFolder();	//kiy 0911 - ������ ������ ��� ����
}

CString CSnapshotDlg::CreateImgName(CString sectionName, CString time, CString format)
{
	if (format != _T("JPG") &&
		format != _T("TIF"))
	{
		AfxMessageBox(_T("Image Save Format not jpg or TIF - CSnapshotDlg::CreateImgName"));
		return NULL;
	}

	CString strImgName;
	strImgName.Format(_T("[%d-%d]_[%s].%s"), pipeImgNum - 1, pipeImgNum, time, format);
	//strImgName.Format(_T("[%d-%d]_[%s]"), pipeImgNum - 1, pipeImgNum, time);

	return strImgName;
}

void CSnapshotDlg::SaveInspectionImg(CString path, CMilBuffer* img, CString format)
{
	if (format != _T("JPG") && format != _T("TIF"))
	{
		AfxMessageBox(_T("Image Save Format not jpg or TIF - CSnapshotDlg::SaveInspectionImg"));
		return;
	}

	CString strExt = path;
	//CString strExt = path + _T(".") + format;

	if (format == _T("JPG"))
		img->Export(strExt, M_JPEG_LOSSY);

	if (format == _T("TIF"))
		img->Export(strExt, M_TIFF);
}

void CSnapshotDlg::DrawThumbnail(CString orgPath, CString itemName)
{
	//---�ð� ����
	clock_t startTime, endTime;
	startTime = clock();

	CBitmap*    pImage = NULL;
	HBITMAP		hBmp = NULL;
	POINT		pt;
	CString		strPath;


	//190308 ��ο� �̹��� �����ϴ��� Ȯ�� gdiplus���� ����� �̹����� �����Ҷ�,
	//�ش� �̹����� ���� ���� �ʾƵ� �׷����⶧���� ����ó��

	CFileStatus status;
	bool imgExistInPath = CFile::GetStatus(orgPath, status);

	if (!imgExistInPath)
	{
		//log
		{
			GetLocalTime(&m_tCur_time);

			CString strTime;
			strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
				m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

			CString log;
			log.Format(_T("[SYS ERR] Can't Create Thumbnail, Image Not Exist : %s"), orgPath);
			log = strTime + log;
			CTabDetailLogDlg::Instance()->SetTextLog(log, RGB(50, 50, 50), FALSE);
		}
	}


	//std::wcout << orgPath.GetBuffer() << ", " << itemName.GetBuffer() << std::endl;
	strPath = orgPath;

	Gdiplus::Bitmap img(strPath);
	Gdiplus::Bitmap* pThumbnail
		= static_cast<Gdiplus::Bitmap*>(img.GetThumbnailImage(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, NULL, NULL));

	pThumbnail->GetHBITMAP(NULL, &hBmp);
	pImage = new CBitmap();
	BOOL attachSuccess = pImage->Attach(hBmp);

	m_ImageListThumb.Add(pImage, RGB(0, 0, 0));

	m_ListCtrlSnapShot.InsertItem(m_nItemNum, itemName, m_nItemNum);

	m_ListCtrlSnapShot.GetItemPosition(m_nItemNum, &pt);

	pt.x = THUMBNAIL_GAP + m_nItemNum * (THUMBNAIL_WIDTH + THUMBNAIL_GAP);
	m_ListCtrlSnapShot.SetItemPosition(m_nItemNum, pt);
	m_nItemNum++;

	//���� ��ũ��
	int nCount = m_ListCtrlSnapShot.GetItemCount();
	if (nCount > 0)
		m_ListCtrlSnapShot.EnsureVisible(nCount - 1, FALSE);

	delete pImage;
	delete pThumbnail;

	endTime = clock();
	printf("DrawThumbnail ���� �ð�: %f\n", ((double)(endTime - startTime)) / CLOCKS_PER_SEC);
	//------------
}

void CSnapshotDlg::ReplaceThumbnail(CString orgPath, int index)
{
	CBitmap*    pImage = NULL;
	HBITMAP		hBmp = NULL;
	CString		fullPath;

	fullPath = m_strImgSaveFolder + _T("\\");
	fullPath += orgPath;

	Gdiplus::Bitmap img(fullPath);
	Gdiplus::Bitmap* pThumbnail
		= static_cast<Gdiplus::Bitmap*>(img.GetThumbnailImage(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, NULL, NULL));

	pThumbnail->GetHBITMAP(NULL, &hBmp);
	pImage = new CBitmap();
	BOOL attachSuccess = pImage->Attach(hBmp);

	int i = m_ImageListThumb.Replace(index, pImage, RGB(0, 0, 0));

	Invalidate();

	delete pImage;
	delete pThumbnail;
}

void CSnapshotDlg::AttachTag(CString tifPath, CString tag)
{
	int tagLen = tag.GetLength();
	int resultPathLen = tifPath.GetLength();

	char* pszTag = new char[tagLen + 1];
	char* pszResultPath = new char[resultPathLen + 1];

	wcstombs(pszTag, tag, tagLen + 1);
	wcstombs(pszResultPath, tifPath, resultPathLen + 1);

	double xSize = CStatus::Instance()->m_dPixelXSize_mm;
	double ySize = CStatus::Instance()->m_dPixelYSize_mm;

	// (10/ size)�� ���־�� ����� ���ż� ����. TiffTag�� centemeter�� �ȼ��� �����̹Ƿ�.
	m_tiffTag.AttachTagFromFile(pszResultPath, pszTag, 10.0f / xSize, 10.0f / ySize);

	delete[] pszTag;
	delete[] pszResultPath;
}

//etc
/*!
	@fn void CSnapshotDlg::NewPipeInspectionStart()
	@brief �������� �˻� ����, ������ ����� �ʱ�ȭ�Ѵ�.
*/
void CSnapshotDlg::NewPipeInspectionStart()
{
	if (m_ListCtrlSnapShot.GetItemCount() != 0)
		m_ListCtrlSnapShot.DeleteAllItems();

	m_nItemNum = 0;
	pipeImgNum = 1;

	m_bIsNewPipe = TRUE;
	m_vtImgNameList.clear();

	//������ ����Ʈ �ʱ�ȭ - ��°������ Remove()�� ���� ���� �̹����� ���´�.
	m_ImageListThumb.DeleteImageList();
	m_ImageListThumb.Create(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, ILC_COLOR32, 0, 1);

	//log
	{
		GetLocalTime(&m_tCur_time);

		CString strTime;
		strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
			m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);

		CString log = strTime + _T("[SYS] New Pipe Inspection");
		CTabDetailLogDlg::Instance()->SetTextLog(log, RGB(50, 50, 50), FALSE);
	}
}

void CSnapshotDlg::OnNMClickListctrlSnapshot(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int index = pNMItemActivate->iItem;
	CString name = m_ListCtrlSnapShot.GetItemText(index, 0);

	BOOL camActivated = m_pView->m_pOpPanelDlg->m_bCamIsActivated;
	//Cam Off�����϶� Ŭ���ϸ� �̹��� �ҷ�����
	if ((!camActivated) && (index >= 0))
	{
		CString fullPath;
		CString itemName = m_vtImgNameList[index];
		fullPath.Format(_T("%s\\%s"), m_strImgSaveFolder, itemName);
		m_pView->m_pCamDlg[0]->m_pMilCamView->BufLoad(fullPath);
	}

	//Number: �� ������ index ����
	if ((index >= 0))
	{
		int temp = 0;
		temp = GetIndexBySectionName(name);
		m_strPipeSectorNum.Format(_T("%d"), temp);
	}
	else
	{
		m_strPipeSectorNum = _T("");
	}

	UpdateData(FALSE);

	*pResult = 0;
}

void CSnapshotDlg::OnEnSetfocusEditNumer()
{
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NUMER);
	pEdit->SetSel(-1, 0);
}

void CSnapshotDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case RESET:
		m_mdManager->WriteCmdData(RESET, 0);
		KillTimer(RESET);
		break;
	case PROC_DONE:
		m_mdManager->WriteCmdData(PROC_DONE, 0);
		KillTimer(PROC_DONE);
		break;
	case TRIGGER_REPEAT:
		m_mdManager->WriteCmdData(TRIGGER_REPEAT, 0);
		KillTimer(TRIGGER_REPEAT);
		break;
	case GO_TO_SECTION:
		m_mdManager->WriteCmdData(GO_TO_SECTION, 0);
		KillTimer(GO_TO_SECTION);
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CSnapshotDlg::TakeImage(int sectionNum)
{
	BOOL sectionAlreadyExist = FALSE;

	CString sectionName;
	sectionName.Format(_T("[%d-%d]"), sectionNum - 1, sectionNum);

	int index;
	for (index = 0; index < m_vtImgNameList.size(); index++)	//�̹� �����ϴ� �������� �˻�.
	{
		CString name = m_vtImgNameList[index].Left(m_vtImgNameList[index].ReverseFind(_T('_')));

		if (name == sectionName)
		{
			sectionAlreadyExist = TRUE;
			break;
		}
	}

	if (sectionAlreadyExist)
		RetakeImage(index);
	else
	{
		pipeImgNum = sectionNum;
		TakeImage();
	}

	SendProcDondCmd(); //Test Check
}

void CSnapshotDlg::RetakeImage(int snapshotIndex)
{
	BOOL isSaveFormatJPG = CStatus::Instance()->m_bImgSaveFormatJPG;
	BOOL isSaveFormatTIF = CStatus::Instance()->m_bImgSaveFormatTIF;

	BOOL saveWithRaw = CStatus::Instance()->m_bSaveWithRaw;

	int index = snapshotIndex;
	if (index >= 0)
	{
		CString fullPath;
		CString selectedImgPath = m_vtImgNameList[index];

		fullPath = m_strImgSaveFolder + _T("\\");
		fullPath += selectedImgPath;

		if (isSaveFormatTIF)
		{
			if (saveWithRaw)
			{
				CString name = fullPath.Left(fullPath.ReverseFind(_T('.')));
				name += _T("_RAW.TIF");

				CMilBuffer *milBufRawTif = CRepository::Instance()->m_MilLib.NewImage(16);
				milBufRawTif->Alloc2D(CSize(CAM_HRES, CAM_VRES));

				//Test Check 190401 - ������ ������ ����ó��
				//��� ����ó������ milBufRawTif->Copy(m_pView->m_pCamDlg[0]->m_pMilTempBufAvgResult�ص� ������ �ʵ��׽�Ʈ ���� �ʾ� ���ǹ� �س���
				if (m_pView->m_pCamDlg[0]->m_pGrabChannel == NULL)
				{
					MessageBox(_T("ERROR: Grab Channel is NULL"), _T("In SnapshotDlg RetakeImage() - Raw"),
						MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND | MB_TOPMOST);
					milBufRawTif->Copy(m_pView->m_pCamDlg[0]->m_pMilTempBufAvgResult);
				}
				else
				{
					milBufRawTif->Copy(m_pView->m_pCamDlg[0]->m_pGrabChannel->m_pMilTempBufAvgResult);
				}

				SaveInspectionImg(name, milBufRawTif, _T("TIF"));
				milBufRawTif->Close();
			}

			CMilBuffer *milBufTif = CRepository::Instance()->m_MilLib.NewImage(16);
			milBufTif->Alloc2D(CSize(CAM_HRES, CAM_VRES));

			//Test Check 190401 - ������ ������ ����ó��
			//��� ����ó������ milBufRawTif->Copy(m_pView->m_pCamDlg[0]->m_pMilTempBufAvgResult�ص� ������ �ʵ��׽�Ʈ ���� �ʾ� ���ǹ� �س���
			if (m_pView->m_pCamDlg[0]->m_pGrabChannel == NULL)
			{
				MessageBox(_T("ERROR: Grab Channel is NULL"), _T("In SnapshotDlg RetakeImage() - Tif"),
					MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND | MB_TOPMOST);
				milBufTif->Copy(m_pView->m_pCamDlg[0]->m_pMilTempBufAvgResult_histo);
			}
			else
			{
				milBufTif->Copy(m_pView->m_pCamDlg[0]->m_pGrabChannel->m_pMilTempBufAvgResult_histo);
			}

			DrawInfosMgraText(milBufTif, index + 1);
			SaveInspectionImg(fullPath, milBufTif, _T("TIF"));
			milBufTif->Close();
			//tempMilBuf->Close();


			//Tag
			CString strTag = _T("");
			CString line1 = m_pView->m_pOpPanelDlg->m_strInfoLine1;
			CString line2 = m_pView->m_pOpPanelDlg->m_strInfoLine2;

			CString FPS = m_pView->m_pOpPanelDlg->m_strFpsValue;
			CString KV = m_pView->m_pOpPanelDlg->m_strKeV;
			CString MA = m_pView->m_pOpPanelDlg->m_strMA;
			CString avg = m_pView->m_pToolPanelDlg->m_strAvgVal;
			CTime curTime = CTime::GetCurrentTime();
			CString strTime = curTime.Format(_T("%Y.%m.%d_%H:%M:%S"));

			strTag += _T("Line1: ") + line1 + _T(", ");
			strTag += _T("Line2: ") + line2 + _T(", ");

			strTag += _T("FPS: ") + FPS + _T(", ");
			strTag += _T("KV: ") + KV + _T(", ");
			strTag += _T("MA: ") + MA + _T(", ");
			strTag += _T("Avg: ") + avg + _T(", ");
			strTag += _T("Time: ") + strTime;

			AttachTag(fullPath, strTag);
		}

		ReplaceThumbnail(selectedImgPath, index);


		//log
		{
			GetLocalTime(&m_tCur_time);

			CString strTime;
			strTime.Format(_T("[%02d:%02d:%02d.%03d] - "),
				m_tCur_time.wHour, m_tCur_time.wMinute, m_tCur_time.wSecond, m_tCur_time.wMilliseconds);


			CString log = strTime + _T("[SYS] Image Retaked : ");
			log += m_vtImgNameList[index].Left(m_vtImgNameList[index].ReverseFind(_T('_')));
			CTabDetailLogDlg::Instance()->SetTextLog(log, RGB(50, 50, 50), FALSE);
		}
	}
}

//! ������ ��Ŭ���� �޴�����
void CSnapshotDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CPoint hitPoint = point;
	m_ListCtrlSnapShot.ScreenToClient(&hitPoint);

	UINT uFlags = 0;
	m_ListCtrlSnapShot.HitTest(hitPoint, &uFlags);
	if (uFlags & LVHT_NOWHERE)
		return;

	m_Menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

BOOL CSnapshotDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == 0)
	{
		int id = LPARAM(wParam);

		switch (id)
		{
			//! ������ ��Ŭ�� -> Move To Position
		case SNAPSHOT_SUBMENU_MOVETOPOS_ID:
			int index = m_ListCtrlSnapShot.GetNextItem(-1, LVIS_SELECTED);
			if (index == -1)
				break;

			CString name = m_ListCtrlSnapShot.GetItemText(index, 0);
			SendGoToSection(GetIndexBySectionName(name));
			break;
		}
	}

	return CDialogEx::OnCommand(wParam, lParam);
}

int CSnapshotDlg::GetIndexBySectionName(CString sectionName)
{
	CString name = sectionName;

	//�糡 ���ȣ ����
	name.Delete(0);
	name.Delete(name.GetLength() - 1);

	int limitCnt = 30;
	int curCnt = 0;

	while (name[0] != '-')
	{
		name.Delete(0);
		curCnt++;

		if (curCnt > limitCnt)
		{
			MessageBox(_T("ERROR: Failed Image Load - Check Image Name"), _T("Image Load Error"),
				MB_ICONERROR | MB_OK | MB_SETFOREGROUND | MB_TOPMOST);
			break;
		}
	}

	name.Delete(0);

	int rst = _ttoi(name);

	return rst;
}

int CSnapshotDlg::FindSnapshotIndexByPartOfString(CString sectionName)
{
	CString name = sectionName;

	int cnt = m_ListCtrlSnapShot.GetItemCount();
	int foundIndex = -1;

	for (int i = 0; i < cnt; i++)
	{
		CString temp = m_ListCtrlSnapShot.GetItemText(i, 0);

		//�糡 ���ȣ ����
		temp.Delete(0);
		temp.Delete(temp.GetLength() - 1);

		temp = temp + '-';

		CString l, r;
		AfxExtractSubString(l, temp, 0, '-');
		AfxExtractSubString(r, temp, 1, '-');

		if (l == sectionName ||
			r == sectionName)
		{
			foundIndex = i;
			break;
		}
	}

	return foundIndex;
}


void CSnapshotDlg::OnBnClickedButton2()
{
	SendProcDondCmd();
}

//���������� �����Ҷ� ���
void CSnapshotDlg::LoadInspectionFolder(CString path)
{
	m_vLastOpendFolderFileList = GetFileNamesInDirectory(path);

	int fileCnt = m_vLastOpendFolderFileList.size();

	if (fileCnt <= 0)
		return;

	m_bIsNewPipe = FALSE;	//������ �������ϰ� ������ �˻� ������ �̾ �ֱ����ؼ� ����

	CString dirPath = CStatus::Instance()->m_strLastOpendFolder;
	m_strImgSaveFolder = dirPath;

	for (int i = 0; i < fileCnt; i++)
	{
		CString str = m_vLastOpendFolderFileList[i];
		CString fullPath = dirPath + _T("\\") + str;


		//_RAW ������ �׳� �ǳʶٵ��� ó�� - 0919
		int nFirst = fullPath.ReverseFind(_T('_')) + 1;
		int nLast = 3;
		CString name = fullPath.Mid(nFirst, nLast);

		if (name == _T("RAW"))
			continue;


		CString sctName;
		AfxExtractSubString(sctName, str, 0, '_');

		DrawThumbnail(fullPath, sctName);

		pipeImgNum = GetIndexBySectionName(sctName);

		m_vtImgNameList.push_back(str);
	}
	pipeImgNum++;	//Take Image �Ҷ� SctName����� ���̱�����.
}

vector<CString> CSnapshotDlg::GetFileNamesInDirectory(CString directory)
{
	vector<CString> files;
	WIN32_FIND_DATA fileData;
	HANDLE hFind;

	directory += "\\*";	//���� ������ ���� ����

	if (!((hFind = FindFirstFile(directory, &fileData)) == INVALID_HANDLE_VALUE))
	{
		while (FindNextFile(hFind, &fileData))
		{
			//�ش� ������ ���丮��� skip
			if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
				continue;

			//��� �������� �����ϴ� ����� "."�� ".."�� ��ŵ
			CString ecep = fileData.cFileName;
			if (!(!(strcmp((CStringA)ecep, ".")) || !(strcmp((CStringA)ecep, ".."))))
			{
				files.push_back(fileData.cFileName);
			}
		}
	}

	FindClose(hFind);
	return files;
}
