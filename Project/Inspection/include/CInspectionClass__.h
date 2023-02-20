#pragma once
#include "DialogRect.h"
#define DEFAULT_TIMEOUT					3000
#define EACH_INSP_THREAD_CNT			3

#define NEAR_BY_VAL_WIDTH_HEIGHT		2

#define PRE_PROCESS_CNT					1

#define INSP_RET						1
#define CLIP_INSP_RET					3

#ifdef MIL_USE
class CInspectionClass;

class CInspectionPattern
{
protected:
	RESULT_DATA m_ResultData;
	INSPEC_PARA m_InspPara;

	CMilBuffer* m_milInspImage[EACH_INSP_THREAD_CNT];

	CString m_strCurrentRecipe;
	int m_nPatternModelIndex;
	DWORD m_dwPatternResultColor[5];

	BOOL m_bEachInspectionThreadEnd[EACH_INSP_THREAD_CNT];
	BOOL m_bEachInspectionStart[EACH_INSP_THREAD_CNT];
	BOOL m_bEachInspecting[EACH_INSP_THREAD_CNT];
	BOOL m_bEachInspectionEnd[EACH_INSP_THREAD_CNT];
	CWinThread* m_pThreadEachInsp[EACH_INSP_THREAD_CNT];
	UINT static EachInspThread1(LPVOID pParam);
	UINT static EachInspThread2(LPVOID pParam);
	UINT static EachInspThread3(LPVOID pParam);
	BOOL ReleaseEachThread();
	MIL_ID m_milModelFile[EACH_INSP_THREAD_CNT];
	RESULT_DATA m_EachResultData[EACH_INSP_THREAD_CNT];
	
	CSize m_ImageSize;
	BOOL m_bResultDisplay;

	BOOL m_bMainLogWrite;

	

private:
	BOOL PatternAddFunc(MIL_ID milGraphicList, MIL_ID milPatternBuff, MIL_ID milMaskBuff, int nPtnIdx, BOOL isMask = FALSE);
	void ResultDisplay(CMilDisplay* milDisplay, CDialogRect *DlgRect, CListCtrl* list_ctrl = NULL);
	BOOL PatternModelLoad();
	BOOL PatternModelSave();
	BOOL OverlapResultPoint(double dX, double dY, int nIdx);
	BOOL OverlapResultPoint(double dX, double dY);

	BOOL PatternSearchFunc(CMilBuffer* milOrgImageBuff, int nIdx);
public:
	CInspectionPattern();
	~CInspectionPattern();

	void InitImage(long nWidth, long nHeight, BOOL isInit = TRUE);

	

	void SetInspPara(MIL_ID milSystem, INSPEC_PARA InspPara, CString strCurrentRecipe, int nPatternModelIndex)
	{
		m_InspPara = InspPara;
		
		m_strCurrentRecipe.Format(_T("%s"), strCurrentRecipe);
		m_nPatternModelIndex = nPatternModelIndex;
		if (m_InspPara.bInspUse)
		{
			PatternModelLoad();
		}
	};

	void InitResultData() {
		m_ResultData.stEachResultData.clear(); m_ResultData.stEachResultData.resize(0);
		m_ResultData.isAllResult = TRUE;
		m_ResultData.isCountResult = TRUE, m_ResultData.isOverlapResult = FALSE;
	};
	BOOL GetAllResult() { return m_ResultData.isAllResult; };
	BOOL GetCountResult() { return m_ResultData.isCountResult; };
	BOOL GetOverlapResult() { return m_ResultData.isOverlapResult; };
	int GetSearchPatternCnt() { return (int)m_ResultData.stEachResultData.size(); };
	int GetOverlapPatternCnt() { return (int)m_ResultData.nOverlapCnt; };
	EACH_RESULT_DATA GetEachResultData(int nIdx) { return m_ResultData.stEachResultData[nIdx]; };
	RESULT_DATA GetResultData() { return m_ResultData; };

	BOOL SetPatternAddFunc(MIL_ID milGraphicList, MIL_ID milPatternBuff, MIL_ID milMaskBuff, int nPtnIdx, BOOL isMask = FALSE)
	{
		return PatternAddFunc(milGraphicList, milPatternBuff, milMaskBuff, nPtnIdx, isMask);
	}

	void SetResultDisplay(CMilDisplay* milDisplay, CDialogRect *dlgRect, CListCtrl* list_ctrl = NULL) {
		ResultDisplay(milDisplay, dlgRect, list_ctrl);
	}


	BOOL StartInspectionThread(CMilBuffer* milImageBuff);

	void SetEachInspectionThread(int nIdx);
	BOOL WaitEachInspThreadEnd(int nIdx);
	BOOL SetPatternSearchFunc(CMilBuffer* milImageBuff, int nIdx)
	{
		return PatternSearchFunc(milImageBuff, nIdx);
	}
	BOOL GetAllInspectionEnd();

	void SetPatternModelSave(int nPtnIndex);

	BOOL SetReleaseEachThread() { return ReleaseEachThread(); };

	void SetResultDisplay(BOOL isDisplay) { m_bResultDisplay = isDisplay; };

	void SetInitEachInspectionThread();
};

#define X_NEGATIVE_FREQUENCY_POSITION   63
#define X_POSITIVE_FREQUENCY_POSITION  191
#define Y_FREQUENCY_POSITION           127

class CInspectionBlobs
{
protected:
	BLOB_INSPEC_PARA m_stBlobsInspPara;
	vector<BLOB_RESULT_PARA> m_stBlobsResult;
	CMilBuffer* m_milBlobsInspImage;
	CString m_strBlobsCurrentRecipe;
	DWORD m_dwBlobsResultColor;
	

	BOOL m_bBlobsInspectionThreadEnd;
	BOOL m_bBlobsInspectionStart;
	BOOL m_bBlobsInspecting;
	BOOL m_bBlobsInspectionEnd;
	CWinThread* m_pThreadBlobsInsp;
	UINT static BlobsInspThread(LPVOID pParam);
	CSize m_BlobsImageSize;

	double m_dAreaAngle;
	double m_dAreaCenX;
	double m_dAreaCenY;

	double m_dPixelSizeX;
	double m_dPixelSizeY;

	int m_nClipBlobsThreshHold;
private:
	BOOL ReleaseBlobsThread();
	void InitBlobsThread();
	void PreProProcess(CMilBuffer* srcImage, CMilBuffer* dstImage);
	void AutoSearchArea(CMilBuffer* srcImage, CMilBuffer* dstMaskImage, CMilDisplay *milDisplay = NULL);
	void GainOffset(BYTE* bySrc, BYTE* byDst, int nSrcPitch, int nSrcHeight, int nDstPitch, int nDstHeight, float fGain = 2., int nOffset = 0, int nAddOffsetVal = 0);
	void BlobsSearch(CMilBuffer* bin_Image, CMilBuffer* gray_Image, CMilBuffer* search_area_image, BOOL isSearch_Area);
	void MaskingImage(CMilBuffer* src, CMilBuffer* dst);
	void IlluminateFunc(CMilBuffer* src, CMilBuffer* dst, int nMaskWidthHeight, double dFactor);
	void EmpahsizeFunc(CMilBuffer* src, CMilBuffer* dst, int nMaskWidthHeight, double dFactor);
	void FFT(CMilBuffer* src, CMilBuffer* dst);
public:
	CInspectionBlobs();
	~CInspectionBlobs();

	void SetPixelSize(double dX, double dY) { m_dPixelSizeX = dX; m_dPixelSizeY = dY; };
	void InitBlobsImage(long nWidth, long nHeight, BOOL isInit = TRUE);
	void SetBlobsInspectionThread();
	void SetInspBlobsPara(BLOB_INSPEC_PARA stInspPara) { m_stBlobsInspPara = stInspPara; };
	int GetBlobsResultCount() { return (int)m_stBlobsResult.size(); };
	BLOB_RESULT_PARA GetBlobsResultData(int nIdx) { return m_stBlobsResult[nIdx]; };
	void SetAutoSearchArea(CMilBuffer* srcImage, CMilBuffer* dstMaskImage, CMilDisplay *milDisplay = NULL) {
		AutoSearchArea(srcImage, dstMaskImage, milDisplay);
	}

	void SetStartBlobsThread(CMilBuffer* milImage);
	void SetBlobsInspection();
	BOOL GetBlobsThreadEnd();
	void SetBlobsResultDisplay(CMilDisplay* milDisplay, CListCtrl* list_ctrl = NULL);
	BOOL GetBlobsInspectionEnd() {return m_bBlobsInspectionEnd; };

	long GetThreshHoldArea(CMilBuffer* srcImage, CRect rect, int nThreshHold);

	void SetPreProcess(CMilBuffer* srcImage, CMilBuffer* dstImage);
	void SetRecipeName(CString strRecipe) { m_strBlobsCurrentRecipe.Format(_T("%s"), strRecipe); };

	void GetSearchAreaCenterXY(CMilBuffer* srcImage, double* dCenX, double* dCenY);
};


class CInspectionClass : public CInspectionPattern, public CInspectionBlobs
{
protected:
	HWND m_hWndMain;
	BOOL m_bMainLog;
	BOOL m_bInspectionLog;
private:


public:
	CInspectionClass();
	~CInspectionClass();

	void SetMainHwnd(HWND hMain, BOOL isMainLog, BOOL isInspectionLog) { m_hWndMain = hMain; m_bMainLog = isMainLog; m_bInspectionLog = isInspectionLog; };
	void WriteLogMessage(CString strLog);
};

#endif