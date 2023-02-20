#pragma once
#include "stdafx.h"
#include <mil.h>
#include "MilClass.h"
#include "MilGraph.h"
#include <UniUtility.h>

/*!
\brief 
* �̹����Ӽ��� �޾ƿö����(CMilBuffer::GetImageProperties)
@author ������
*/
struct tImageProperties
{
	CSize ImageSize;
	int Depth;
	int BandSize;
	int Sign;
	int Type;
	MIL_INT64 Attr;

};

struct tExportAVI
{
	CString strFileName;
	int	iFrame;
	double dblFrameRate;
	MIL_INT64 FileFormat;
};

class CMilDisplay;
/*!
\brief 
* Mil�� MilBuffer����  �Լ����� Classȭ�Ͽ���
@author ������
*/
class MIL_DLLDECL CMilBuffer:public CMilGraph
{
public:
//	enum tMilAttr{BUF_ARRAY=M_ARRAY, BUF_IMAGE=M_IMAGE, BUF_KERNEL=M_KERNEL, BUF_LUT=M_LUT, BUF_DISP=M_DISP, BUF_PROC=M_PROC, BUF_DIB=M_DIB};
//	enum tMilDepth{DEP_1=1, DEP_8=8, DEP_16=16, DEP_32=32, DEP_UNSIGNED=M_UNSIGNED, DEP_SIGNED=M_SIGNED, DEP_FLOAT=M_FLOAT};
	static int m_milIDBuffCounter;
	CMilBuffer(void);
	CMilBuffer(CSize ImageSize);
	~CMilBuffer(void);
    
	int GetMilCounter(void);
	int Restore(CString strFileName);
	int RestoreJPG(CString strFileName, MIL_ID milIDSystem, MIL_ID * pMilIDBuffter);
	void BufLoad(CString strFileName);
	int BufImport(CString strFileName, MIL_INT FileFormat=M_BMP, MIL_INT Operation=M_LOAD);
	//! ���÷��̿� ���۸� ���� ��Ų��.
	int Close(void);
	BOOL IsAlloced(void);
	void ConvertToBMPFmt(CMilBuffer *pBMPBuffer, MIL_INT64 BufferAttribute=M_IMAGE+M_PROC);
	void ExportBMP(CString strFileName);
	void ExportJPG(CString strFileName);
	void Export(CString strFileName, int m_iFileFormat = M_TIFF);
	void SetFileFormat(MIL_INT iFileFormat);
	CMilBuffer* operator=(CMilBuffer &  LValue);
	MIL_INT64 SetAttribute(MIL_INT64 BufferAttribute);
	MIL_INT64 GetAttribute(void);
	void Select(CMilDisplay * pMilDisplay);
	void UnSelectDisp(void);
	BOOL IsSelectDisp(void);
	void SelectWindow(HWND ClientWindowHandle);
	void SelectWindow(CMilDisplay * pMilDisplay,  HWND ClientWindowHandle);
	void SelectWindow(CWnd *pHWindow);
	void SelectWindow(CMilDisplay * pMilDisplay, CWnd *pHWindow, BOOL bRemove=FALSE);
	BOOL IsDispAllocated(void);
	void GetImageProperties(tImageProperties *pImageProperty);
    void ResizeBuf(CMilBuffer *pMilDest, CdblSize imageSize);

	MIL_INT m_iDepth;
	MIL_INT64 m_BufAttr;
	MIL_INT m_iFileFormat;
	tExportAVI* m_pExportAVIInfo;

	int Alloc2D(CSize imageSize);
	int AllocColor2D(CSize imageSize);
	void Put2D(CSize offsetPos, CSize sizeImage, void *pUserArrayPtr);
	CMilBuffer * Child2D(CRect *pRect);
	CMilBuffer * Child2D(int offsetX, int offsetY, int childWidth, int childHeight);
	int Create2D(CSize sizeImage, void *pAddr, int nDepth=8L+M_UNSIGNED, int Attr=M_IMAGE+M_PROC);
	void * GetAddr(void);
	void SetDepth(int Depth);
	void SetChannelCount(int nChannel);
	int GetDepth(void);
	CWnd *m_pHWndSelected;
	CMilBuffer* DuplicateBuff(void);
	void Clear(MIL_DOUBLE ClearColor=M_COLOR_BLACK);
	int Copy(CMilBuffer* pMilSrc);
	int CopyClip(CMilBuffer* pMilClipBuffer, CPoint Offset);
	int Get2D(void * pUserArray, CRect rect);
	int GetColor(void * pUserArray);
	int SetImageProperties(tImageProperties* pImageProperties);
	int SetDisplay(CMilDisplay* pMilDisplay);
	void CopyColor2D(CMilBuffer *pBufDest, CRect rectCopy, CPoint ofsDest=CPoint(0,0), int bandDest=M_ALL_BANDS , int bandSrc=M_ALL_BANDS);
	CMilDisplay* GetDisplay();
	int WndLvlTranform(CMilBuffer *pBufDest, int OrgStart, int OrgEnd, int DestStart, int DestEnd);
private:
	CMilDisplay *m_pMilDisplay;
public:
	int GetIDBuffer(void);
	void SetIDBuffer(int milID);
	int GetPixelGray(int x, int y, int * vGray);
	int GetPixelGray(CPoint posPixel, int * vGray);
	int GetPixelColor(int x, int y, COLORREF * vColor, int typeColor= M_RGB24);
	int GetPixelColor(CPoint posPixel, COLORREF * vColor);
	int GetColor2D(int x, int y, CSize sizeImage, void * pBuffer, int typeColor = M_RGB24);
	int LoadRawFile(CString strFileName, CSize rawSize);
	void Put(void * pUserArrayPtr);
	int BufImportJPG(CString strFileName, MIL_ID milIDSystem, MIL_ID milIDBuffter);
	int ExportAVIOpen(tExportAVI *pEA, int iOpenMode = M_OPEN);
	int ExportAVI(tExportAVI *pEA=NULL);
	int ExportAVIClose(tExportAVI *pEA);
	BOOL m_bAVIClosed;
	void Lock();
	void Unlock();
	int m_nChannel;
};
