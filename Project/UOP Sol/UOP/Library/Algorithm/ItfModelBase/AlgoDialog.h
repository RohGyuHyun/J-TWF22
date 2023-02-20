#pragma once
#include "AlgorithmBase.h"
#include "ParamsInterface.h"

// CAlgoDialog ��ȭ �����Դϴ�.

/*!
	@brief �˰����۾��ڰ� ���� �۾��� ���̾�α��� ����Ŭ������ ��ϵ��� ����ڰ� ���� ���� �ʴ� ��Ȳ���� �� �� �ֵ��� �Ѵ�.
*/
class ITFBASEDLLDECL CAlgoDialog : public CDialog
{
	DECLARE_DYNAMIC(CAlgoDialog)

public:
	CAlgoDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAlgoDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_resID;
	CParamsInterface * m_pParamsInterface;
	CPtrList m_lstTemplete;
	virtual LRESULT OnScrnUpdate(WPARAM wparam, LPARAM lparam);
	virtual int GetParam(CAlgorithmBase * pAlgorithm);
	virtual int SetParam(CAlgorithmBase * pAlgorithm);
	virtual CAlgorithmBase * NewAlgoObject(void);
	void SetResID(int resID);
	virtual BOOL IsChecked(void);
	virtual int RegCaller(CParamsInterface * pParamsInterface);
	virtual int CompareAlgoName(TCHAR * szAlgoName);
	virtual int DoTrial(void);
	virtual int DoCompoTrial(CRect inspRect, CInspResult * pResult);
	virtual int Inspect(void);
	virtual void SetInput(CMilBuffer * pInputImage);
	CMilBuffer *m_pInputImg;
	CAlgorithmBase *m_pAlgorithm;
	virtual CString GetAlgoName(void);
	virtual int GetAlgoType(void);
	virtual void SetCheck(bool bStatus);
	virtual int DoAutoTeaching(void);
	virtual int ReportResult(CInspResult * pInspResult);
};
