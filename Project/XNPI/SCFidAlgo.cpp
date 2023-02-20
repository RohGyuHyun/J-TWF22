#include "stdafx.h"
#include "SCFidAlgo.h"
#include "Repository.h"


CSCFidAlgo::CSCFidAlgo()
	: m_nFidCnt(0)
	, m_upOffset(0)
	, m_umOffset(0)
	, m_iUseScale(0)
	, m_iUseAngle(0)
	, m_iAngleRate(7)
	, m_iScaleRate()
	, m_iMatRate(60)
	, m_nCamNo(0)
	, m_pMilBuffer(0)
	, m_ptFidROICenterOffset(0)
{
}


CSCFidAlgo::~CSCFidAlgo()
{
}

/*! @fn int CFidAlgo::Reg(CMilBuffer* pMilBuffer, CRect rectFid)
@brief pMilBuffer�� ������ ������ũ�� ��Ͻ�Ų��.
@param pMilBuffer ����� MilBuffer
@param pPos	����� ������ũ ��ġ */
int CSCFidAlgo::Reg(CMilBuffer * pMilBuffer, CRect rectFid)
{
	int FidIndex = 0;
	m_modFinder.Close();
	m_modFinder.SetBuffer(pMilBuffer);
	m_modFinder.SetContext(&m_MilContext);
	m_modFinder.SetResult(&m_milResult);
	m_modFinder.Alloc();
	m_modFinder.GetFeature();
	m_modFinder.PreProcess();
	m_nFidCnt = 1;
	return 0;
}

int CSCFidAlgo::RemoveAllModels()
{
	return m_MilContext.DelAllModel();
}

BOOL CSCFidAlgo::IsEmpty()
{
	if (!m_nFidCnt)return TRUE; else return FALSE;
}

/*! @fn int CSCFidAlgo::LoadFid(TCHAR * szModelPath)
@brief ������ũ�����͸� �𵨵��������������� ����Ѵ�
@param szModelPath  �𵨵����� ���� */
int CSCFidAlgo::SaveFid(TCHAR * szModelPath, int index)
{
	CString FidName;
	FidName.Format(_T("%s\\%d.ctx"), szModelPath, index);
	m_MilContext.SaveContext(FidName.GetBuffer());

	return 0;
}

/*! @fn int CSCFidAlgo::LoadFid(TCHAR * szModelPath)
@brief ������ũ�� �𵨵����ͷκ��� �о�´�
@param szModelPath data������ ���� */
int CSCFidAlgo::LoadFid(TCHAR * szModelPath, int index)
{
	CString FidName;
	FidName.Format(_T("%s\\%d.ctx"), szModelPath, index);
	if (PathFileExists(FidName))
		m_MilContext.RestoreContext(FidName.GetBuffer());
	else
	{
		AfxMessageBox(_T("Fiducial is not registrated!!!"));
		return -1;
	}
	m_MilContext.PreProcess();
	CSize sizeImage;
	m_MilContext.GetImageSize(&sizeImage);
	

	if (m_pMilBuffer)
	{
		m_pMilBuffer->Close();
		delete m_pMilBuffer;
		m_pMilBuffer= NULL;
	}
	m_pMilBuffer = CRepository::Instance()->m_MilLib.NewImage();
	m_pMilBuffer->SetDepth(8);
	m_pMilBuffer->SetAttribute(M_IMAGE + M_PROC + M_DISP);
	m_pMilBuffer->Alloc2D(CSize(sizeImage.cx, sizeImage.cy));
	m_MilContext.GetImage(m_pMilBuffer);
	m_nFidCnt = 1;
	return 0;
}

int CSCFidAlgo::Find(CMilBuffer * pMilBuffer)
{
	CMilBuffer newImage;
	tImageProperties imgProperty;
	tModResult myResult;
	if (IsEmpty())return -2;


	pMilBuffer->ConvertToBMPFmt(&newImage, M_IMAGE + M_PROC + M_DISP);
	m_upOffset.SetPoint(0, 0);
	m_umOffset.SetPoint(0, 0);


	m_modFinder.SetBuffer(&newImage);
	m_modFinder.SetContext(&m_MilContext);
	ApplyParamToAlgo();
	m_modFinder.SetResult(&m_milResult);
	m_modFinder.Alloc();
	m_modFinder.PreProcess();
	m_modFinder.Find(&myResult);
	GetOffset(&myResult);
#ifdef _DEBUG
	CDbgFile::Instance()->Write(_T("Fid Result:%ld"), myResult.NumResults);
#endif
	///Result�� �׸� ���۷� ġȯ��
	m_modFinder.SetBuffer(pMilBuffer);
//	m_modFinder.Close();
	newImage.Close();
	if (!myResult.NumResults)return -1;
	return 0;
}


void CSCFidAlgo::GetOffset(tModResult * pResult)
{
	double centerX = pResult->ImageSize.cx / 2;
	double centerY = pResult->ImageSize.cy / 2;
	centerX -= m_ptFidROICenterOffset.dblPosX;
	centerY -= m_ptFidROICenterOffset.dblPosY;
	m_upOffset.dblPosX = pResult->Pos.dblPosX - centerX;
	m_upOffset.dblPosY = pResult->Pos.dblPosY - centerY;

	m_umOffset.dblPosX = (m_upOffset.dblPosX*m_dblPixelScale.dblCX) * 1000;
	if (abs(m_umOffset.dblPosX)>22000)m_umOffset.dblPosX = 0;///�������� �Ҵ� FOV���� Ŭ��� ������ �Ǵ��Ͽ� 0���� ����
	m_umOffset.dblPosY = (m_upOffset.dblPosY*m_dblPixelScale.dblCY) * 1000;
	if (abs(m_umOffset.dblPosY)>22000)m_umOffset.dblPosY = 0;///�������� �Ҵ� FOV���� Ŭ��� ������ �Ǵ��Ͽ� 0���� ����
#ifdef _DEBUG
	CDbgFile::Instance()->Write(_T("Fid:Pixel x=%f, y%f, um x=%f, y=%f"), m_upOffset.dblPosX, m_upOffset.dblPosY, m_umOffset.dblPosX, m_umOffset.dblPosY);
#endif

}


int CSCFidAlgo::Draw(BOOL bIsFinded)
{
	CRect rectOffSet(m_rectInsp);
	CRect rectSearchRage(m_rectFidSearchRange);
	rectOffSet.OffsetRect(CPoint(m_upOffset.dblPosX, m_upOffset.dblPosY));
	if(bIsFinded)
		m_pMilBuffView->SetColor(M_COLOR_GREEN);
	else
		m_pMilBuffView->SetColor(M_COLOR_RED);

	for (int i = 0; i < 4; i++)
	{
		m_pMilBuffView->Rect(rectOffSet);
		m_pMilBuffView->Rect(m_rectFidSearchRange);
		rectOffSet.InflateRect(1, 1);
		m_rectFidSearchRange.InflateRect(1, 1);

	}

//	m_modFinder.DrawResult();
	return 0;
}


int CSCFidAlgo::SetDrawOffset(CSize Offset)
{
	return m_modFinder.SetDrawOffset(Offset);
}

void CSCFidAlgo::ApplyParamToAlgo(void)
{
	//	if (m_iUseAngle)m_modFinder.SetSearchAngleRange(m_iAngleRate); else m_modFinder.SetSearchAngleRange();
	//	if (m_iUseScale)m_modFinder.SetSearchScaleRange(m_iScaleRate); else m_modFinder.SetSearchScaleRange();
	//	if (m_iUseAngle)m_modFinder.SetSearchAngleRange(m_iAngleRate); else m_modFinder.SetSearchAngleRange();
	//	if (m_iUseScale)m_modFinder.SetSearchScaleRange(m_iScaleRate); else m_modFinder.SetSearchScaleRange();
	m_modFinder.SetAccptanceScore(m_iMatRate);
}

void CSCFidAlgo::CloseMod()
{
	m_modFinder.Close();
}
