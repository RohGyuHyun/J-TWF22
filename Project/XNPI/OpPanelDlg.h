#pragma once
#include "afxwin.h"
#include "..\Library\SerialLib\SerialMFC.h"	// �ʼ�
#include "..\Library\SerialLib\XFMSPacket.h"	// �ʼ�
#include "SCButton.h"
#include "SCGroupBox.h"
#include "NumEdit.h"
#include "afxdtctl.h"

// COpPanelDlg ��ȭ �����Դϴ�.
struct tXFMSRecord;
class CToolPanelDlg;


//�ð����� �ڵ����忡 �ʿ��� �ɼ�
struct tAutoSaveTime
{
	CTime	tmStart;
	CTime	tmEnd;
	int iSecTimeTotal;
	int iMinTimeInterval;
	int iMinTimeDuration;
};

// �µ����� �ڵ����忡 �ʿ��� �ɼ�
struct tAutoSaveTemp
{
	int iTempLimitLow;
	int iTempLimitHigh;
	int iSensorNo;
	int iTempDelta;
	int iMinTimeDuration;
};

class COpPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpPanelDlg)

public:
	COpPanelDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~COpPanelDlg();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_OPPANEL
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CSerialMFC		m_serial;				// �ʼ�
	CXFMSPacket 	m_XFMSPacket;			// �ʼ�
	CComboBox m_ctrlSelCam;
	virtual BOOL OnInitDialog();
	//	afx_msg void OnCbnSelchangeSelcam();
	//	afx_msg void OnBnClickedBtnLoadimage();
	afx_msg void OnBnClickedSetenv();
	int InitLight();
	BOOL m_bReceived;
	//int SerialCheckNInit();
	void ProcessCMD(int nEvent);
	void SetFlagReceived();
	BOOL IsCMDReceived();
	int RcvSerialMsg(WPARAM wParam);
	int CheckoutPacket();
	void UpdateDriveModeInfo(); //device list Update;
	//	afx_msg void OnBnClickedBtnTeaching();
	//	afx_msg void OnBnClickedBtnLoadjobfile();
	//	afx_msg void OnBnClickedBtnSavejobfile();
	afx_msg void OnBnClickedChkactiveallcam();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//	afx_msg void OnBnClickedAistart();
	//	afx_msg void OnBnClickedBtnSaveas();
	//	afx_msg void OnBnClickedBtntrial();
	//	afx_msg void OnBnClickedBtnOptest();
	void DeleteOldFile();
	BOOL IsDate(CString strName, CTime *pTime);
	int DiffDate(CTime * pDate);
	void DelFolder(CString strFolder);
	CSCButton m_cRunStop;
	CSCButton m_cSaveImage;
	CSCButton m_cLoadImage;
	CSCButton m_cLoadJobFile;
	CSCButton m_cSaveJobFile;
	CSCButton m_cSaveAs;
	CSCButton m_cTeaching;
	CSCButton m_cTrial;
	CSCButton m_cSetEnv;
	CSCButton m_cFinish;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	static UINT ThreadTest(LPVOID pParam);

	CFont m_BigFont;
	//int m_selFocalSpotSize;
	//	afx_msg void OnBnClickedSelfocalspot();
	afx_msg void OnBnClickedBtnXrayon();
	afx_msg void OnBnClickedBtnXrayoff();
	afx_msg void OnCbnSelchangeComboDriveMode();
	CSCButton m_cBtnXrayON;
	CSCButton m_cBtnXrayOFF;
	CString m_strMakCondition;
	CSCButton m_cMarkAdd;
	CString GetManualFileName();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSCButton m_cTempSave;
	CSCButton m_cTempCSVSave;
	BOOL m_bWriteCSV;
	CString m_strFullCSVFileName;
	CNumEdit m_cVol;
	CNumEdit m_cCur;
	void CloseProcess();
	float GetFpsValueFromDeviceSetting();  //kiy
	void SetFpsValue(float fpsVal); //kiy
	// �ڵ� �̹��� ������� �µ���ȭ�� ����
	CToolPanelDlg *m_pToolPanelDlg;
	CString m_strPipeInfoComp;
	CString m_strPipeInfoSEPC;
	CString m_strPipeInfoSize;
	CString m_strPipeInfoPipeNo;
	CString m_strPipeInfoSerial;
	CComboBox m_cbDriveMode;
	CString m_strFpsValue;
	CString m_strKeV;
	CString m_strMA;
	afx_msg void OnBnClickedButtonApply();

	//kiy ------------------------------------------------------
	//BOOL m_bCamIsActivated; //! active cam On, Off ����
	CStdioFile m_infoFileManager;
	afx_msg void OnDestroy();
	BOOL m_bTimeDisplay;
	BOOL m_bCamIsActivated; //! active cam On, Off ����
	BOOL m_bPipeInfoDisplay;
	BOOL m_bSectionNumDisplay;
	BOOL m_bDistanceDisplay;
	CString m_strInfoLine1;
	CString m_strInfoLine2;
	SYSTEMTIME m_tCur_time; //190327 - ����ð� �и��� ������ ���ϱ�(�α� ���λ翡 ���� �뵵)
	//----------------------------------------------------------
};
