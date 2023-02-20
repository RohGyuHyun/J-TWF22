// TabDetailResultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SCInspector.h"
#include "TabDetailResultDlg.h"
//#include "BridgeAlgo.h"
//#include "InspResult.h"


// CTabDetailResultDlg dialog
CTabDetailResultDlg* CTabDetailResultDlg::m_pInstance=NULL;

IMPLEMENT_DYNAMIC(CTabDetailResultDlg, CPropertyPage)

CTabDetailResultDlg::CTabDetailResultDlg()
	: CPropertyPage(CTabDetailResultDlg::IDD)
{

}

CTabDetailResultDlg::~CTabDetailResultDlg()
{
}

void CTabDetailResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
}


BEGIN_MESSAGE_MAP(CTabDetailResultDlg, CPropertyPage)
	ON_BN_CLICKED(IDOK, &CTabDetailResultDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTabDetailResultDlg message handlers

void CTabDetailResultDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CTabDetailResultDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// 	CMachineInfo::Instance()->LoadCFG();
 //   CString Dirstr = CMachineInfo::Instance()->m_LengDir;
	//CChangeRes::Instance()->ChangeDialogCaption(Dirstr,this);
	//return TRUE;  // return TRUE unless you set the focus to a control

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
 
void CTabDetailResultDlg::SetResultDisplay(CInspResult *pInspResult)
{
//	UpdateData(true);
//	if(pInspResult->GetType()==_T("CBridgeAlgo"))
//	{  //��ü Trial ǥ��
//		int nColumnCount = m_listResult.GetHeaderCtrl()->GetItemCount();
//		for(int i=0;i<nColumnCount;i++)
//		{
//			m_listResult.DeleteColumn(0);
//			m_listResult.DeleteAllItems();
//		}
//
//		CString strIndex, strArea, strCntrX, strCntrY, strAR;
//		CString strCount;
//		strCount.Format(_T("%d"),pInspResult->GetCount());
//
//		m_listResult.InsertColumn(0, _T("No"),		LVCFMT_RIGHT,30);
//		m_listResult.InsertColumn(1, _T("Area"),	LVCFMT_RIGHT,60);
//		m_listResult.InsertColumn(2, _T("CntrX"),	LVCFMT_RIGHT,60);
//		m_listResult.InsertColumn(3, _T("CntrY"),	LVCFMT_RIGHT,60);
//		m_listResult.InsertColumn(4, _T("ARatio"),	LVCFMT_RIGHT,60);
//
//		m_listResult.SetExtendedStyle(LVS_EX_FULLROWSELECT);
//
//		if(!pInspResult->GetCount())return;
//
//		tBridgeResult *pResult = (tBridgeResult *)(pInspResult->m_pResult);
//
//		for(int i=0;i<pInspResult->GetCount();i++)
//		{
//			strIndex.Format(_T("%d"),i);
//			strArea.Format(_T("%d"), (pResult+i)->Area);
//			strCntrX.Format(_T("%d"), (pResult+i)->CenterX);
//			strCntrY.Format(_T("%d"), (pResult+i)->CenterY);
//			strAR.Format(_T("%5.2f"), (pResult+i)->AspectRate);
//
//			m_listResult.InsertItem(i, strIndex);
//			m_listResult.SetItemText(i ,1 ,strArea);
//			m_listResult.SetItemText(i ,2 ,strCntrX);
//			m_listResult.SetItemText(i ,3 ,strCntrY);
//			m_listResult.SetItemText(i ,4 ,strAR);
//		}
//	}
//
//	if(pInspResult->GetType()==_T("CChipAlgo"))
//	{  //1��Trial��ǥ��
///*		int nColumnCount = m_listResult.GetHeaderCtrl()->GetItemCount();
//		for(int i=0;i<nColumnCount;i++)
//		{
//			m_listResult.DeleteColumn(0);
//			m_listResult.DeleteAllItems();
//		}
//
//		CString strIndex, strColPos, strRowPos, strColDist, strRowDist;
//		CString strCount;
//		strCount.Format(_T("%d"),pInspResult->GetCount());
//
//		m_listResult.InsertColumn(0, _T("No"),		LVCFMT_RIGHT,30);
//		m_listResult.InsertColumn(1, _T("COL:0"),	LVCFMT_RIGHT,60);
//		m_listResult.InsertColumn(2, _T("CntrX"),	LVCFMT_RIGHT,60);
//		m_listResult.InsertColumn(3, _T("CntrY"),	LVCFMT_RIGHT,60);
//		m_listResult.InsertColumn(4, _T("ARatio"),	LVCFMT_RIGHT,60);
//
//		m_listResult.SetExtendedStyle(LVS_EX_FULLROWSELECT);
//
//		if(!pInspResult->GetCount())return;
//
//		tBridgeResult *pResult = (tBridgeResult *)(pInspResult->m_pResult);
//
//		for(int i=0;i<4;i++)
//		{
//			strIndex.Format(_T("ColPos[%d]"),i);
//			m_listResult.InsertItem(i, strIndex);
//			strColPos.Format(_T("%d"), (pResult+i)->ColPos);
//			m_listResult.SetItemText(i ,1 ,strColPos);
//		}
//
//		for(int i=0;i<4;i++)
//		{
//			strIndex.Format(_T("RowPos[%d]"),i);
//			m_listResult.InsertItem(i+4, strIndex);
//			strColPos.Format(_T("%d"), (pResult+i)->ColPos);
//			m_listResult.SetItemText(i+4 ,1 ,strColPos);
//		}
//
//		for(int i=0;i<2;i++)
//		{
//			strIndex.Format(_T("ColDist[%d]"),i);
//			m_listResult.InsertItem(i+4+4, strIndex);
//			strColPos.Format(_T("%d"), (pResult+i)->ColPos);
//			m_listResult.SetItemText(i+4+4, 1 ,strColPos);
//		}
//
//		for(int i=0;i<2;i++)
//		{
//			strIndex.Format(_T("ColDist[%d]"),i);
//			m_listResult.InsertItem(i+4+4+2, strIndex);
//			strColPos.Format(_T("%d"), (pResult+i)->ColPos);
//			m_listResult.SetItemText(i+4+4+2, 1 ,strColPos);
//		}
//
//		strIndex.Format(_T("Mean"),i);
//		m_listResult.InsertItem(i+4+4+2+2, strIndex);
//		strColPos.Format(_T("%d"), (pResult+i)->ColPos);
//		m_listResult.SetItemText(i+4+4+2+2, 1 ,strColPos);
//*/
//
//	}
//

	UpdateData(FALSE);
	
}


CTabDetailResultDlg * CTabDetailResultDlg::Instance(void)
{
	if(!m_pInstance)m_pInstance=new CTabDetailResultDlg; 

	return m_pInstance;
}
