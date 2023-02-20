
// DemoDeviceDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "DemoDevice.h"
#include "DemoDeviceDlg.h"
#include "afxdialogex.h"
#include "resource.h"

#include <atlbase.h>
#include <atlconv.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDT_ONE_SEC		0
#define MAX_PACKETNUM 256

#define WM_MMTIMER WM_USER+1


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.
#pragma comment(lib, "winmm.lib")
#include "Mmsystem.h"

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDemoDeviceDlg ��ȭ ����



CDemoDeviceDlg::CDemoDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMODEVICE_DIALOG, pParent)
	, m_AVMDPacket(m_serial)
	, m_sTemperature(0)
	, m_byBattery(0)
	, m_strCalDate(_T("160404"))
	, m_strSerialNo(_T("3P70016"))
	, m_bStart(FALSE)
	, m_bTimerOn(FALSE)
	, m_iXvalue(0)
	, m_iYvalue(0)
	, m_iZvalue(0)
	, m_iValueMode(0)
	, m_nPacketNum(0)
	, m_mmTimerID(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_itempXValue[15] = { 0, };
	m_itempYValue[15] = { 0, };
	m_itempZValue[15] = { 0, };
}

void CDemoDeviceDlg::UpdateAvailableComPort()
{
	m_wndComPorts.ResetContent();

	HKEY hKey;

	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	TCHAR szName[100];
	TCHAR szData[20];

	DWORD dwIndex = 0;
	DWORD dwSize = 100;
	DWORD dwSize2 = 20;
	DWORD dwType = REG_SZ;

	while (ERROR_SUCCESS == RegEnumValue(hKey, dwIndex, szName, &dwSize, NULL, NULL, NULL, NULL))
	{
		if (RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2) != ERROR_SUCCESS)
		{
			break;
		}

		TCHAR* pszComPort = new TCHAR[sizeof(szData) + 4];

		_stprintf_s(pszComPort, sizeof(szData) + 4, _T("\\\\.\\%s"), szData);
		if (CSerial::CheckPort(pszComPort) == CSerial::EPortAvailable)
		{
			CString strItem;
			strItem.Format(_T("%s (%s)"), szData, szName);

			int nIndex = m_wndComPorts.AddString(strItem);
			m_wndComPorts.SetItemData(nIndex, (DWORD_PTR)pszComPort);
		}
		else
		{
			delete[] pszComPort;
		}

		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 100;
		dwSize2 = 20;

		dwIndex++;
	}

	RegCloseKey(hKey);

	if (m_wndComPorts.GetCount() > 0)
	{
		m_wndComPorts.SetCurSel(0);
	}
}

void CDemoDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEMPERATURE, m_sTemperature);
	DDX_Text(pDX, IDC_BATTERY, m_byBattery);
	DDX_Control(pDX, IDC_STATE, m_wndState);
	DDX_Text(pDX, IDC_CAL_DATE, m_strCalDate);
	DDV_MaxChars(pDX, m_strCalDate, 6);
	DDX_Text(pDX, IDC_SERIAL_NO, m_strSerialNo);
	DDV_MaxChars(pDX, m_strCalDate, 8);
	DDX_Control(pDX, IDC_COMPORTS, m_wndComPorts);
	DDX_Control(pDX, IDC_START, m_wndStart);
	DDX_Control(pDX, IDC_CB_KOWAFER, m_cKindOfWafer);
	DDX_Control(pDX, IDC_SP_XVALUE, m_spXValue);
	DDX_Control(pDX, IDC_SP_YVALUE, m_spYValue);
	DDX_Control(pDX, IDC_SP_ZVALUE, m_spZValue);
	DDX_Control(pDX, IDC_CB_KOWAFER, m_cKindOfWafer);
	DDX_Control(pDX, IDC_SLID_XVALUE, m_cSldXValue);
	DDX_Control(pDX, IDC_SLID_YVALUE, m_cSldYValue);
	DDX_Control(pDX, IDC_SLID_ZVALUE, m_cSldZValue);
	DDX_Text(pDX, IDC_XVALUE, m_iXvalue);
	DDX_Text(pDX, IDC_YVALUE, m_iYvalue);
	DDX_Text(pDX, IDC_ZVALUE, m_iZvalue);
	DDX_Radio(pDX, IDC_RMANUAL, m_iValueMode);
}

BEGIN_MESSAGE_MAP(CDemoDeviceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_CBN_SELENDOK(IDC_COMPORTS, &CDemoDeviceDlg::OnCbnSelendokComports)
	ON_WM_SERIAL(OnSerialMsg)
	ON_BN_CLICKED(IDC_START, &CDemoDeviceDlg::OnBnClickedStart)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLID_XVALUE, &CDemoDeviceDlg::OnNMReleasedcaptureSlidXvalue)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLID_YVALUE, &CDemoDeviceDlg::OnNMReleasedcaptureSlidYvalue)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLID_ZVALUE, &CDemoDeviceDlg::OnNMReleasedcaptureSlidZvalue)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLID_XVALUE, &CDemoDeviceDlg::OnTRBNThumbPosChangingSlidXvalue)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLID_YVALUE, &CDemoDeviceDlg::OnTRBNThumbPosChangingSlidYvalue)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLID_ZVALUE, &CDemoDeviceDlg::OnTRBNThumbPosChangingSlidZvalue)
	ON_MESSAGE(WM_MMTIMER, &CDemoDeviceDlg::OnMMTimer)
	ON_BN_CLICKED(IDC_BUTTON, &CDemoDeviceDlg::OnBnClickedButton)
	ON_BN_CLICKED(IDC_RAUTO, &CDemoDeviceDlg::OnBnClickedRauto)
	ON_BN_CLICKED(IDC_RMANUAL, &CDemoDeviceDlg::OnBnClickedRmanual)
	ON_BN_CLICKED(IDC_RSQURE, &CDemoDeviceDlg::OnBnClickedRsqure)
END_MESSAGE_MAP()


// CDemoDeviceDlg �޽��� ó����

BOOL CDemoDeviceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TEMPERATURE))->SetRange32(SHRT_MIN, SHRT_MAX);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SP_XVALUE))->SetRange32(-1962, 1962);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SP_YVALUE))->SetRange32(-1962, 1962);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SP_ZVALUE))->SetRange32(-1962, 1962);

	m_spXValue.SetRange32(-1962, 1962);
	m_spYValue.SetRange32(-1962, 1962);
	m_spZValue.SetRange32(-1962, 1962);

	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_BATTERY))->SetRange32(0, 255);

	UpdateAvailableComPort();

	OnCbnSelendokComports();		// Initial COM port open.

	m_cKindOfWafer.AddString(_T("150mm"));
	m_cKindOfWafer.AddString(_T("200mm")); 
	m_cKindOfWafer.AddString(_T("300mm"));
	m_cKindOfWafer.AddString(_T("450mm"));
	m_cKindOfWafer.SetCurSel(2);
	m_nPacketNum = 0;

	m_iXvalue = 10;
	m_iYvalue = 10;
	m_iZvalue = 10;
	m_iValueMode = 0;
	m_sTemperature = -10;
	m_byBattery = 31;

	m_cSldXValue.SetRange(-1962, 1962);
	m_cSldXValue.SetPos(100);
	m_cSldYValue.SetRange(-1962, 1962);
	m_cSldYValue.SetPos(100);
	m_cSldZValue.SetRange(-1962, 1962);
	m_cSldZValue.SetPos(100);
	
	UpdateData(FALSE);
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CDemoDeviceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CDemoDeviceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2; 
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CDemoDeviceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDemoDeviceDlg::OnDestroy()
{
	if (m_serial.IsOpen())
		m_serial.Close();

	for (int i = 0; i < m_wndComPorts.GetCount(); i++)
	{
		delete[](TCHAR*)m_wndComPorts.GetItemData(i);
	}

	CDialogEx::OnDestroy();
}

void CDemoDeviceDlg::OnCbnSelendokComports()
{
	int nIndex = m_wndComPorts.GetCurSel();
	if (nIndex == CB_ERR)
	{
		return;
	}

	if (m_serial.IsOpen())
	{
		m_serial.Close();
	}

	TCHAR* pszComPort = (TCHAR*)m_wndComPorts.GetItemData(nIndex);

	if (m_serial.Open(pszComPort, this) == ERROR_SUCCESS)
	{
		VERIFY(m_serial.Setup(CSerial::EBaud115200, CSerial::EData8, CSerial::EParNone, CSerial::EStop1) == ERROR_SUCCESS);
		VERIFY(m_serial.SetMask(CSerial::EEventBreak | CSerial::EEventError | CSerial::EEventRecv) == ERROR_SUCCESS);
	}

	if (!m_serial.IsOpen())
	{
		AfxMessageBox(_T("Unable to open COM-port"), MB_ICONSTOP | MB_OK);
	}
}

void CDemoDeviceDlg::OnBnClickedStart()
{
	//m_bTimerOn = !m_bTimerOn;

	//if (m_bTimerOn)
	//{
	//	SetTimer(IDT_ONE_SEC, 100, NULL);
	//	m_wndStart.SetWindowText(_T("����"));
	//}
	//else
	//{
	//	KillTimer(IDT_ONE_SEC);
	//	m_wndStart.SetWindowText(_T("����"));
	//}
}

void  CALLBACK MMTimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CDemoDeviceDlg *pDlg = (CDemoDeviceDlg *)dwUser;
	pDlg->SendMessage(WM_MMTIMER);
	return;
}

LRESULT CDemoDeviceDlg::OnSerialMsg(WPARAM wParam, LPARAM lParam)
{
	CAVMDPacket::EPacketType eType = m_AVMDPacket.CheckoutPacket();

	USES_CONVERSION;
	switch (eType)
	{
	case CAVMDPacket::SensorInfo:
	{
		m_AVMDPacket.SendSensorInfo(m_cKindOfWafer.GetCurSel() + 1, CT2A(m_strCalDate), CT2A(m_strSerialNo));
		m_bStart = FALSE;
		m_wndStart.SetWindowText(_T("����"));
		m_nPacketNum = 0;
		timeKillEvent(m_mmTimerID);
	}
	break;

	case CAVMDPacket::Measure:
	{
		UpdateData(TRUE);
		m_AVMDPacket.SendMeasureData(m_itempXValue,m_itempYValue,m_itempZValue, m_sTemperature, m_byBattery, m_nPacketNum);
		IncPacketNum();
		m_bStart = TRUE;
		TIMECAPS timecaps;
		timeGetDevCaps(&timecaps, sizeof(TIMECAPS));
		m_wndStart.SetWindowText(_T("����"));
		m_mmTimerID = timeSetEvent(50, timecaps.wPeriodMin, MMTimerProc, (DWORD)this, TIME_PERIODIC);//jubatemp
		// sampling 1000 => 1�ʿ� packet 40�� �̹Ƿ� 1000 / 40 = 25
		// sampling 500 => 50
	}
		break;

	default:
		break;
	}

	return 0;
}

LRESULT CDemoDeviceDlg::OnMMTimer(WPARAM wParam, LPARAM lParam)
{
	static DWORD dwCount = 0;
	static int nMTCount = 0;
	static int nStep = 0;
	static int nIndex = 0;
	static int nAdd = 0;
	static int prevTime = 0;
	static int nCount = 0;
	//if (nCount > 100)return 0;
	//else nCount++;

	UpdateData(TRUE);
	if (prevTime == 0)prevTime = timeGetTime();
	int curTime = timeGetTime();
	printf("\nStep %d PacketNum %d:time:%d", nStep++, m_nPacketNum, curTime - prevTime);
	prevTime = curTime;
	if (m_serial.IsOpen())
	{
		UpdateData(true);

		if (m_bStart)
		{
			switch (m_iValueMode)
			{
			case 0:
				for (int i = 0; i < VALUESET_COUNT; i++)
				{
					m_itempXValue[i] = m_iXvalue;
					m_itempYValue[i] = m_iYvalue;
					m_itempZValue[i] = m_iZvalue;
				}
				break;
			case 1:
			{
				m_iXvalue = 10;
				m_iYvalue = 10;
				m_iZvalue = 10;

				for (int i = 0; i < VALUESET_COUNT; i++)
				{
					m_itempXValue[i] = 900 * sin((M_PI*nIndex) / 180);
					m_itempYValue[i] = 900 * sin((M_PI*(nIndex + 90)) / 180.);
					m_itempZValue[i] = 900 * sin((M_PI*(nIndex + 180)) / 180.);
					nIndex++;
					nAdd++;

					nAdd %= 1000;

					//if (nAdd > 900)
					//{
					//	m_itempXValue[i] = 0;
					//	m_itempYValue[i] = 0;
					//	m_itempZValue[i] = 0;
					//}
					nIndex %= 360;
				}
			}
				break;
			case 2:
				for (int i = 0; i < VALUESET_COUNT; i++)
				{
					if ((m_nPacketNum%2))
					{
						m_itempXValue[i] = 900;
						m_itempYValue[i] = 900;
						m_itempZValue[i] = 900;
					}
					else
					{
						m_itempXValue[i] = 0;
						m_itempYValue[i] = 0;
						m_itempZValue[i] = 0;
					}
				}
				break;

			}

			m_AVMDPacket.SendMeasureData(m_itempXValue, m_itempYValue, m_itempZValue, m_sTemperature, m_byBattery, m_nPacketNum);
			IncPacketNum();
		}
		CString strState;

		m_wndState.SetWindowText(strState);
	}
	return 0;
}

void CDemoDeviceDlg::OnNMReleasedcaptureSlidXvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iXvalue = m_cSldXValue.GetPos();
	UpdateData(FALSE);
	*pResult = 0;
}


void CDemoDeviceDlg::OnNMReleasedcaptureSlidYvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iYvalue = m_cSldYValue.GetPos();
	UpdateData(FALSE);
	*pResult = 0;
}


void CDemoDeviceDlg::OnNMReleasedcaptureSlidZvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iZvalue = m_cSldZValue.GetPos();
	UpdateData(FALSE);
	*pResult = 0;
}


void CDemoDeviceDlg::OnTRBNThumbPosChangingSlidXvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �� ����� ����Ϸ��� Windows Vista �̻��� �־�� �մϴ�.
	// _WIN32_WINNT ��ȣ�� 0x0600���� ũ�ų� ���ƾ� �մϴ�.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void CDemoDeviceDlg::OnTRBNThumbPosChangingSlidYvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �� ����� ����Ϸ��� Windows Vista �̻��� �־�� �մϴ�.
	// _WIN32_WINNT ��ȣ�� 0x0600���� ũ�ų� ���ƾ� �մϴ�.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void CDemoDeviceDlg::OnTRBNThumbPosChangingSlidZvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �� ����� ����Ϸ��� Windows Vista �̻��� �־�� �մϴ�.
	// _WIN32_WINNT ��ȣ�� 0x0600���� ũ�ų� ���ƾ� �մϴ�.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void CDemoDeviceDlg::OnBnClickedButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_AVMDPacket.SendMeasureData(m_itempXValue, m_itempYValue, m_itempZValue, m_sTemperature, m_byBattery, m_nPacketNum);
	IncPacketNum();
	m_bStart = TRUE;
	TIMECAPS timecaps;
	timeGetDevCaps(&timecaps, sizeof(TIMECAPS));
	m_wndStart.SetWindowText(_T("����"));
	m_mmTimerID = timeSetEvent(20, timecaps.wPeriodMin, MMTimerProc, (DWORD)this, TIME_PERIODIC);




	//UpdateData(TRUE);
	//m_AVMDPacket.SendMeasureData(m_itempXValue, m_itempYValue, m_itempZValue, m_sTemperature, m_byBattery, m_nPacketNum);
	//IncPacketNum();
	//m_bStart = TRUE;
}


void CDemoDeviceDlg::OnBnClickedRauto()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	GetDlgItem(IDC_XVALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_YVALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_ZVALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLID_XVALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLID_YVALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLID_ZVALUE)->EnableWindow(FALSE);
}


void CDemoDeviceDlg::OnBnClickedRmanual()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	GetDlgItem(IDC_XVALUE)->EnableWindow(TRUE);
	GetDlgItem(IDC_YVALUE)->EnableWindow(TRUE);
	GetDlgItem(IDC_ZVALUE)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLID_XVALUE)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLID_YVALUE)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLID_ZVALUE)->EnableWindow(TRUE);
}


void CDemoDeviceDlg::IncPacketNum()
{
	m_nPacketNum++;
	m_nPacketNum %= MAX_PACKETNUM;
}



void CDemoDeviceDlg::OnBnClickedRsqure()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	GetDlgItem(IDC_XVALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_YVALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_ZVALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLID_XVALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLID_YVALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLID_ZVALUE)->EnableWindow(FALSE);
}
