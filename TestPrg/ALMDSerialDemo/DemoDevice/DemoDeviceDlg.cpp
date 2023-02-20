
// DemoDeviceDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "DemoDevice.h"
#include "DemoDeviceDlg.h"
#include "afxdialogex.h"
#include "resource.h"

#include <atlbase.h>
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDT_ONE_SEC		0

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


// CDemoDeviceDlg ��ȭ ����



CDemoDeviceDlg::CDemoDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMODEVICE_DIALOG, pParent)
	, m_ALMDPacket(m_serial)
	, m_sTemperature(0)
	, m_byBattery(0)
	, m_strCalDate(_T("160404"))
	, m_strSerialNo(_T("3P70016"))
	, m_bStart(FALSE)
	, m_bTimerOn(FALSE)
	, m_sRoll(0)
	, m_sPitch(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
	//DDX_Control(pDX, IDC_SP_ROLL, m_spRoll);
	//DDX_Control(pDX, IDC_SP_PITCH, m_spPitch);
	DDX_Control(pDX, IDC_CB_KOWAFER, m_cKindOfWafer);
	DDX_Text(pDX, IDC_ROLL, m_sRoll);
	DDX_Text(pDX, IDC_PITCH, m_sPitch);
	DDX_Control(pDX, IDC_SLD_ROLL, m_cSldRoll);
	DDX_Control(pDX, IDC_SLD_PITCH, m_cSldPitch);
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
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLD_ROLL, &CDemoDeviceDlg::OnTRBNThumbPosChangingSldRoll)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLD_PITCH, &CDemoDeviceDlg::OnTRBNThumbPosChangingSldPitch)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_ROLL, &CDemoDeviceDlg::OnNMReleasedcaptureSldRoll)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_PITCH, &CDemoDeviceDlg::OnNMReleasedcaptureSldPitch)
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
	((CSpinButtonCtrl*)GetDlgItem(IDC_SP_ROLL))->SetRange32(-30000, 300000);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SP_PITCH))->SetRange32(-30000, 300000);
	//m_spRoll.SetRange32(-30000, 30000);
	//m_spPitch.SetRange32(-30000, 30000);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_BATTERY))->SetRange32(0, 255);

	UpdateAvailableComPort();

	OnCbnSelendokComports();		// Initial COM port open.

	m_cKindOfWafer.AddString(_T("150mm"));
	m_cKindOfWafer.AddString(_T("200mm"));
	m_cKindOfWafer.AddString(_T("300mm"));
	m_cKindOfWafer.AddString(_T("450mm"));
	m_cKindOfWafer.SetCurSel(2);
	m_sRoll = 1500;
	m_sPitch = 2500;
	m_sTemperature = -10;
	m_byBattery = 31;
	m_cSldRoll.SetRange(-30000, 30000);
	m_cSldRoll.SetPos(1500);
	m_cSldPitch.SetRange(-30000, 30000);
	m_cSldPitch.SetPos(2500);
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

void CDemoDeviceDlg::OnTimer(UINT_PTR nIDEvent)
{
	static DWORD dwCount = 0;
	static int nMTCount = 0;

	if (nIDEvent == IDT_ONE_SEC)
	{
		if (m_serial.IsOpen())
		{
			UpdateData(true);

			if (m_bStart)
			{
				m_ALMDPacket.SendMeasureData(m_sRoll, m_sPitch, m_sTemperature, m_byBattery);
			}

			CString strState;

			m_wndState.SetWindowText(strState);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
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
	UpdateData(true);
}

LRESULT CDemoDeviceDlg::OnSerialMsg(WPARAM wParam, LPARAM lParam)
{
	CALMDPacket::EPacketType eType = m_ALMDPacket.CheckoutPacket();
	USES_CONVERSION;
	switch (eType)
	{
	case CALMDPacket::SensorInfo:
		{
			m_ALMDPacket.SendSensorInfo(m_cKindOfWafer.GetCurSel()+1, CT2A(m_strCalDate), CT2A(m_strSerialNo));
			m_bStart = FALSE;
			m_wndStart.SetWindowText(_T("����"));
			KillTimer(IDT_ONE_SEC);
	}
		break;

	case CALMDPacket::Measure:
		{
			UpdateData(TRUE);
			m_ALMDPacket.SendMeasureData(m_sRoll, m_sPitch, m_sTemperature, m_byBattery);
			m_bStart = TRUE;
			m_wndStart.SetWindowText(_T("����"));
			SetTimer(IDT_ONE_SEC, 50, NULL);
	}
		break;

	default:
		break;
	}

	return 0;
}



void CDemoDeviceDlg::OnTRBNThumbPosChangingSldRoll(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �� ����� ����Ϸ��� Windows Vista �̻��� �־�� �մϴ�.
	// _WIN32_WINNT ��ȣ�� 0x0600���� ũ�ų� ���ƾ� �մϴ�.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void CDemoDeviceDlg::OnTRBNThumbPosChangingSldPitch(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �� ����� ����Ϸ��� Windows Vista �̻��� �־�� �մϴ�.
	// _WIN32_WINNT ��ȣ�� 0x0600���� ũ�ų� ���ƾ� �մϴ�.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void CDemoDeviceDlg::OnNMReleasedcaptureSldRoll(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_sRoll = m_cSldRoll.GetPos();
	UpdateData(FALSE);
	*pResult = 0;
}


void CDemoDeviceDlg::OnNMReleasedcaptureSldPitch(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_sPitch = m_cSldPitch.GetPos();
	UpdateData(FALSE);
	*pResult = 0;
}
