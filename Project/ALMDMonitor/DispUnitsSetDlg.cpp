// DispUnitsSetDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "DispUnitsSetDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "UnitConversion.h"

#define ROUNDING(x, dig)    ( floor((x) * pow(float(10), dig) + 0.5f) / pow(float(10), dig) )

// CDispUnitsSetDlg ��ȭ �����Դϴ�.
#define TOLERANCE_LOW 0.03
#define TOLERANCE_HIGH 5.0
IMPLEMENT_DYNAMIC(CDispUnitsSetDlg, CDialogEx)

CDispUnitsSetDlg::CDispUnitsSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DISPUNITSET, pParent)
	, m_iCoordinateSystem(0)
	, m_iInclinationType(CStatus::IT_ANGLE)
	, m_dblGoNoGoDegree(0)
	, m_strGoNoGoUnit(_T(""))
	, m_strDispTolerance(_T(""))
	, m_dblDispTolerance(0)
	, m_strGoNoGoRange(_T(""))
	, m_dblToleranceLow(0)
	, m_dblToleranceHigh(0)
{

}

CDispUnitsSetDlg::~CDispUnitsSetDlg()
{
}

void CDispUnitsSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RDISPTYPE, m_iCoordinateSystem);
	DDX_Radio(pDX, IDC_RDISPUNIT, m_iInclinationType);
	DDX_Control(pDX, IDC_CB_KOFANGLE, m_cKofAngle);
	DDX_Control(pDX, IDC_CB_KOFROR, m_cKofROR);
	DDX_Text(pDX, IDC_EDT_TOL, m_strDispTolerance);
	DDX_Text(pDX, IDC_LGONOGOUNIT, m_strGoNoGoUnit);
	DDX_Text(pDX, IDC_LGONOGO, m_strGoNoGoRange);
	DDX_Control(pDX, IDC_EDT_TOL, m_cTolValue);
}


BEGIN_MESSAGE_MAP(CDispUnitsSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDispUnitsSetDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_CB_KOFANGLE, &CDispUnitsSetDlg::OnCbnSelchangeCbKofangle)
	ON_CBN_SELCHANGE(IDC_CB_KOFROR, &CDispUnitsSetDlg::OnCbnSelchangeCbKofror)
	ON_BN_CLICKED(IDC_RDISPTYPE, &CDispUnitsSetDlg::OnBnClickedRdisptype)
	ON_BN_CLICKED(IDC_RDISPUNIT, &CDispUnitsSetDlg::OnBnClickedRdispunit)
	ON_BN_CLICKED(IDC_RROR, &CDispUnitsSetDlg::OnBnClickedRror)
	ON_BN_CLICKED(IDC_RMAGDIR, &CDispUnitsSetDlg::OnBnClickedRmagdir)
	ON_EN_UPDATE(IDC_EDT_TOL, &CDispUnitsSetDlg::OnEnUpdateEdtTol)
END_MESSAGE_MAP()


// CDispUnitsSetDlg �޽��� ó�����Դϴ�.


BOOL CDispUnitsSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_iCoordinateSystem = CStatus::Instance()->GetCoordinateSystem();
	m_iInclinationType = CStatus::Instance()->GetInclinationType();
	m_cKofAngle.AddString(_T("Degrees"));
	m_cKofAngle.AddString(_T("MilRad"));
	m_cKofROR.AddString(_T("mm(diameter)"));
	m_cKofROR.AddString(_T("mm(radius)"));
	m_cKofAngle.SetCurSel(CStatus::Instance()->GetAngleType());
	m_cKofROR.SetCurSel(CStatus::Instance()->GetRORType());

	m_dblGoNoGoDegree = CStatus::Instance()->GetTolerance();
	GetGoNoGoDispInitData();
	SetGoNoGoLabel();
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDispUnitsSetDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();
	if (GoNoGoEdtChange() == FALSE)return;
	CStatus::Instance()->SetCoordinateSystem( (CStatus::tCoordinateSystem) m_iCoordinateSystem);
	CStatus::Instance()->SetInclinationType( (CStatus::tIncType) m_iInclinationType);

	switch (m_cKofAngle.GetCurSel())
	{
	case 0:
		CStatus::Instance()->SetAngleType(CStatus::AT_DEGREE);
		break;
	case 1:
		CStatus::Instance()->SetAngleType(CStatus::AT_RADIAN);
		break;
	}
	switch (m_cKofROR.GetCurSel())
	{
	case 0:
		CStatus::Instance()->SetRORType(CStatus::RT_MMD);
		break;
	case 1:
		CStatus::Instance()->SetRORType(CStatus::RT_MMR);
		break;
	}
	UpdateData(true);
	CStatus::Instance()->SetTolerance(m_dblGoNoGoDegree);

	CDialogEx::OnOK();
}


void CDispUnitsSetDlg::OnCbnSelchangeCbKofangle()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GoNoGoDispUpdate();
}


void CDispUnitsSetDlg::OnCbnSelchangeCbKofror()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GoNoGoDispUpdate();
}


void CDispUnitsSetDlg::OnBnClickedRdisptype()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GoNoGoDispUpdate();

}

void CDispUnitsSetDlg::OnBnClickedRdispunit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GoNoGoDispUpdate();
}

void CDispUnitsSetDlg::OnBnClickedRror()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GoNoGoDispUpdate();
}

void CDispUnitsSetDlg::OnBnClickedRmagdir()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GoNoGoDispUpdate();
}


void CDispUnitsSetDlg::GoNoGoDispUpdate()
{
	UpdateData(TRUE);
	GetDispData();
	SetGoNoGoLabel();
	UpdateData(false);
}


void CDispUnitsSetDlg::GetDispData()
{
	CStatus::tAngleType AngleType = CStatus::AT_DEGREE;
	switch (m_cKofAngle.GetCurSel())
	{
	case 0:
		AngleType = CStatus::AT_DEGREE;
		break;
	case 1:
		AngleType = CStatus::AT_RADIAN;
		break;
	}

	CStatus::tRORType RORType = CStatus::RT_MMD;
	switch (m_cKofROR.GetCurSel())
	{
	case 0:
		RORType = CStatus::RT_MMD;
		break;
	case 1:
		RORType = CStatus::RT_MMR;
		break;
	}

	double dispGoNoGoTolerance = _wtof(m_strDispTolerance.GetBuffer());
	double dblGoNoGoDegree;
	if (dispGoNoGoTolerance != m_dblDispTolerance)
	{
		CUnitConversion::Instance()->ConvertToDegree((CStatus::tCoordinateSystem)m_iCoordinateSystem, AngleType, (CStatus::tIncType)m_iInclinationType, RORType, &dblGoNoGoDegree, dispGoNoGoTolerance);
		if (dblGoNoGoDegree != m_dblGoNoGoDegree)
		{
		}
	}

	CUnitConversion::Instance()->ConvertToDest((CStatus::tCoordinateSystem)m_iCoordinateSystem, AngleType, (CStatus::tIncType)m_iInclinationType, RORType, &m_strGoNoGoUnit, &m_dblDispTolerance, m_dblGoNoGoDegree);
	if(CStatus::Instance()->IsDigit2Mode())
		m_strDispTolerance.Format(_T("%5.2f"), m_dblDispTolerance);
	else
		m_strDispTolerance.Format(_T("%5.3f"), m_dblDispTolerance);
}


void CDispUnitsSetDlg::GetGoNoGoDispInitData()
{
	CStatus::tAngleType AngleType = CStatus::AT_DEGREE;
	switch (m_cKofAngle.GetCurSel())
	{
	case 0:
		AngleType = CStatus::AT_DEGREE;
		break;
	case 1:
		AngleType = CStatus::AT_RADIAN;
		break;
	}

	CStatus::tRORType RORType = CStatus::RT_MMD;
	switch (m_cKofROR.GetCurSel())
	{
	case 0:
		RORType = CStatus::RT_MMD;
		break;
	case 1:
		RORType = CStatus::RT_MMR;
		break;
	}

	CUnitConversion::Instance()->ConvertToDest((CStatus::tCoordinateSystem)m_iCoordinateSystem, AngleType, (CStatus::tIncType)m_iInclinationType, RORType, &m_strGoNoGoUnit, &m_dblDispTolerance, m_dblGoNoGoDegree);
	if(CStatus::Instance()->IsDigit2Mode())
		m_strDispTolerance.Format(_T("%5.2f"), m_dblDispTolerance);
	else
		m_strDispTolerance.Format(_T("%5.3f"), m_dblDispTolerance);
}


BOOL CDispUnitsSetDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{
			if (pMsg->hwnd == GetDlgItem(IDC_EDT_TOL)->GetSafeHwnd())
			{
				GoNoGoEdtChange();
			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDispUnitsSetDlg::GoNoGoEdtChange()
{
	if (!IsInTheTolRange()) return FALSE;
	UpdateData(true);
	GetDispGoNoGo();
	UpdateData(false);
	return TRUE;
}


void CDispUnitsSetDlg::GetDispGoNoGo()
{
	CStatus::tAngleType AngleType = CStatus::AT_DEGREE;
	switch (m_cKofAngle.GetCurSel())
	{
	case 0:
		AngleType = CStatus::AT_DEGREE;
		break;
	case 1:
		AngleType = CStatus::AT_RADIAN;
		break;
	}

	CStatus::tRORType RORType = CStatus::RT_MMD;
	switch (m_cKofROR.GetCurSel())
	{
	case 0:
		RORType = CStatus::RT_MMD;
		break;
	case 1:
		RORType = CStatus::RT_MMR;
		break;
	}

	double dispGoNoGoTolerance = _wtof(m_strDispTolerance.GetBuffer());
	double dblGoNoGoDegree;
	CUnitConversion::Instance()->ConvertToDegree((CStatus::tCoordinateSystem)m_iCoordinateSystem, AngleType, (CStatus::tIncType)m_iInclinationType, RORType, &dblGoNoGoDegree, dispGoNoGoTolerance);
	if (dblGoNoGoDegree != m_dblGoNoGoDegree)
	{
			m_dblGoNoGoDegree = dblGoNoGoDegree;
	}
}


void CDispUnitsSetDlg::SetGoNoGoLabel()
{
	CStatus::tAngleType AngleType = CStatus::AT_DEGREE;
	switch (m_cKofAngle.GetCurSel())
	{
	case 0:
		AngleType = CStatus::AT_DEGREE;
		break;
	case 1:
		AngleType = CStatus::AT_RADIAN;
		break;
	}

	CStatus::tRORType RORType = CStatus::RT_MMD;
	switch (m_cKofROR.GetCurSel())
	{
	case 0:
		RORType = CStatus::RT_MMD;
		break;
	case 1:
		RORType = CStatus::RT_MMR;
		break;
	}


	CUnitConversion::Instance()->ConvertToDest((CStatus::tCoordinateSystem)m_iCoordinateSystem, AngleType, (CStatus::tIncType)m_iInclinationType, RORType, &m_strGoNoGoUnit, &m_dblToleranceLow, TOLERANCE_LOW);
	CUnitConversion::Instance()->ConvertToDest((CStatus::tCoordinateSystem)m_iCoordinateSystem, AngleType, (CStatus::tIncType)m_iInclinationType, RORType, &m_strGoNoGoUnit, &m_dblToleranceHigh, TOLERANCE_HIGH);

	if (CStatus::Instance()->IsDigit2Mode())
	{
		m_strDispTolerance.Format(_T("%5.2f"), m_dblDispTolerance);
		m_strGoNoGoRange.Format(_T("Enter the value from %5.2f to %5.2f"), m_dblToleranceLow, m_dblToleranceHigh);
	}
	else
	{
		m_strDispTolerance.Format(_T("%5.3f"), m_dblDispTolerance);
		m_strGoNoGoRange.Format(_T("Enter the value from %5.3f to %5.3f"), m_dblToleranceLow, m_dblToleranceHigh);
	}
}


BOOL CDispUnitsSetDlg::IsInTheTolRange()
{
	CString strNum;
	GetDlgItemText(IDC_EDT_TOL, strNum);
	double dblNum = _wtof(strNum.GetBuffer());
	if ((dblNum<(ROUNDING(m_dblToleranceLow, 4))) || (dblNum>(ROUNDING(m_dblToleranceHigh, 4))))
	{
		AfxMessageBox(m_strGoNoGoRange);
		return FALSE;
	}
	return TRUE;
}


void CDispUnitsSetDlg::OnEnUpdateEdtTol()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
