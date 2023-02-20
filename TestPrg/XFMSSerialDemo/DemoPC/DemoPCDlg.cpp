
// DemoPCDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "DemoPC.h"
#include "DemoPCDlg.h"
#include "afxdialogex.h"

#include <atlbase.h>
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

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


// CDemoPCDlg ��ȭ ����



CDemoPCDlg::CDemoPCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMOPC_DIALOG, pParent)
	, m_XFMSPacket(m_serial)	//kiy
	, m_strCount01(_T(""))
	, m_strCount03(_T(""))
	, m_strCount05(_T(""))
	, m_strTemperature(_T(""))
	, m_strPressure(_T(""))
	, m_strBattery(_T(""))
	, m_strHumidity(_T(""))
	, m_strCalDate(_T(""))
	, m_strSerialNo(_T(""))
	, m_strState(_T(""))
	, m_strTime(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoPCDlg::UpdateAvailableComPort()
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

void CDemoPCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COUNT_01, m_strCount01);
	DDX_Text(pDX, IDC_COUNT_03, m_strCount03);
	DDX_Text(pDX, IDC_COUNT_05, m_strCount05);
	DDX_Text(pDX, IDC_TEMPERATURE, m_strTemperature);
	DDX_Text(pDX, IDC_PRESSURE, m_strPressure);
	DDX_Text(pDX, IDC_BATTERY, m_strBattery);
	DDX_Text(pDX, IDC_HUMIDITY, m_strHumidity);
	DDX_Text(pDX, IDC_CAL_DATE, m_strCalDate);
	DDX_Text(pDX, IDC_SERIAL_NO, m_strSerialNo);
	DDX_Text(pDX, IDC_STATE, m_strState);
	DDX_Text(pDX, IDC_TIME, m_strTime);
	DDX_Control(pDX, IDC_COMPORTS, m_wndComPorts);
	DDX_Control(pDX, IDC_PURGE, m_wndPruge);
	DDX_Control(pDX, IDC_MEASURE, m_wndMeasure);
	DDX_Control(pDX, IDC_KV, m_wndKV);
	DDX_Control(pDX, IDC_MA, m_wndMA);

}

BEGIN_MESSAGE_MAP(CDemoPCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_START, &CDemoPCDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CDemoPCDlg::OnBnClickedStop)
	ON_CBN_SELENDOK(IDC_COMPORTS, &CDemoPCDlg::OnCbnSelendokComports)
	ON_WM_SERIAL(OnSerialMsg)		// �ʼ�
	ON_BN_CLICKED(IDC_XRAY_ON, &CDemoPCDlg::OnBnClickedXrayOn)
	ON_BN_CLICKED(IDC_XRAY_OFF, &CDemoPCDlg::OnBnClickedXrayOff)
	ON_BN_CLICKED(IDC_FOCAL_STANDARD, &CDemoPCDlg::OnBnClickedFocalStandard)
	ON_BN_CLICKED(IDC_FOCAL_SMALL, &CDemoPCDlg::OnBnClickedFocalSmall)
	ON_BN_CLICKED(IDC_SET_KV, &CDemoPCDlg::OnBnClickedSetKv)
	ON_BN_CLICKED(IDC_SET_MA, &CDemoPCDlg::OnBnClickedSetMa)
END_MESSAGE_MAP()


// CDemoPCDlg �޽��� ó����

BOOL CDemoPCDlg::OnInitDialog()
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

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_wndPruge.SetWindowText(_T("20"));
	m_wndMeasure.SetWindowText(_T("30"));

	UpdateAvailableComPort();		

	OnCbnSelendokComports();		// �ʼ� Initial COM port open.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CDemoPCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDemoPCDlg::OnPaint()
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
HCURSOR CDemoPCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDemoPCDlg::OnDestroy()
{
	if (m_serial.IsOpen())
		m_serial.Close();

	for (int i = 0; i < m_wndComPorts.GetCount(); i++)
	{
		delete[] (TCHAR*)m_wndComPorts.GetItemData(i);
	}

	CDialogEx::OnDestroy();
}

void CDemoPCDlg::OnBnClickedStart()
{
	UpdateData(TRUE);

	CString strText;

	m_wndPruge.GetWindowText(strText);

	WORD wPurgeTime = _tstoi(strText);

	m_wndMeasure.GetWindowText(strText);

	WORD wMeasureTime = _tstoi(strText);

//	m_APMDPacket.SendStartCommand(wPurgeTime, wMeasureTime);
}


void CDemoPCDlg::OnBnClickedStop()
{
//	m_APMDPacket.SendStopCommand();
}

void CDemoPCDlg::OnCbnSelendokComports()
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

	// �ʼ� -
	if (m_serial.Open(pszComPort, this) == ERROR_SUCCESS)
	{
		VERIFY(m_serial.Setup(CSerial::EBaud9600, CSerial::EData8, CSerial::EParNone, CSerial::EStop1) == ERROR_SUCCESS);
		VERIFY(m_serial.SetMask(CSerial::EEventBreak | CSerial::EEventError | CSerial::EEventRecv) == ERROR_SUCCESS);
	}
	// - �ʼ�

	if (!m_serial.IsOpen())
	{
		AfxMessageBox(_T("Unable to open COM-port"), MB_ICONSTOP | MB_OK);
	}
}

LRESULT CDemoPCDlg::OnSerialMsg(WPARAM wParam, LPARAM lParam)	// �ʼ�
{
	USES_CONVERSION;

	//CAPMDPacket::EPacketType eType = m_APMDPacket.CheckoutPacket();
	//
	//switch (eType)
	//{
	//case CAPMDPacket::Measure:
	//{
	//	DVC_PACKET_MEASURE* pPacket = (DVC_PACKET_MEASURE*)m_APMDPacket.GetPacket();

	//	float fTemperature = (float)pPacket->sTemperature / 100.F;
	//	float fPressure = (float)pPacket->wPressure / 10.F;

	//	m_strTime.Format(_T("%u sec"), pPacket->wSec);
	//	m_strCount01.Format(_T("%u"), pPacket->wCount01);
	//	m_strCount03.Format(_T("%u"), pPacket->wCount03);
	//	m_strCount05.Format(_T("%u"), pPacket->wCount05);
	//	m_strTemperature.Format(_T("%.2f degree"), fTemperature);
	//	m_strPressure.Format(_T("%.1f mbar"), fPressure);
	//	m_strBattery.Format(_T("%u %%"), pPacket->byBattery);

	//	if (pPacket->byHumidity <= 100)
	//	{
	//		m_strHumidity.Format(_T("%u %%"), pPacket->byHumidity);
	//	}
	//	else
	//	{
	//		m_strHumidity.Format(_T("----"));
	//	}

	//	switch (m_APMDPacket.CheckDeviceState(pPacket))
	//	{
	//	case CAPMDPacket::StateStandby:
	//		m_strState = _T("STANDBY");
	//		break;

	//	case CAPMDPacket::StatePurge:
	//		m_strState = _T("PURGE");
	//		break;

	//	case CAPMDPacket::StateMeasure:
	//		m_strState = _T("MEASURE");
	//		break;

	//	default:
	//		m_strState = _T("UNKNOWN");
	//		break;
	//	}

	//	UpdateData(FALSE);
	//}
	//break;

	//case CAPMDPacket::State:
	//{
	//	DVC_PACKET_STATE* pPacket = (DVC_PACKET_STATE*)m_APMDPacket.GetPacket();

	//	char szCalDate[MAX_CAL_DATE + 1] = { 0 };
	//	char szSerialNo[MAX_SERIAL_NO + 1] = { 0 };

	//	::CopyMemory(szCalDate, pPacket->chCalDate, MAX_CAL_DATE);
	//	::CopyMemory(szSerialNo, pPacket->chSerialNo, MAX_SERIAL_NO);

	//	m_strCalDate = A2CT(szCalDate);
	//	m_strSerialNo = A2CT(szSerialNo);

	//	UpdateData(FALSE);
	//}
	//break;

	//default:
	//	break;
	//}
	//
	return 0;
}

//kiy------------------------------------------------------
void CDemoPCDlg::OnBnClickedXrayOn()
{
	m_XFMSPacket.SendSetXrayOnOff(TRUE);
}


void CDemoPCDlg::OnBnClickedXrayOff()
{
	m_XFMSPacket.SendSetXrayOnOff(FALSE);
}


void CDemoPCDlg::OnBnClickedFocalStandard()
{
	m_XFMSPacket.SendSetFocalSpot(TRUE);
}


void CDemoPCDlg::OnBnClickedFocalSmall()
{
	m_XFMSPacket.SendSetFocalSpot(FALSE);
}

void CDemoPCDlg::OnBnClickedSetKv()
{
	CString str;
	m_wndKV.GetWindowText(str);
	double nNum = _wtof(str);

	m_XFMSPacket.SendSetKvCMD(nNum);
}

void CDemoPCDlg::OnBnClickedSetMa()
{
	CString str;
	m_wndMA.GetWindowText(str);
	double nNum = _wtof(str);

	m_XFMSPacket.SendSetMaCmd(nNum);
}
//---------------------------------------------------------
