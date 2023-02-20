#include "StdAfx.h"
#include "MilBuffer.h"
#include <math.h>
#include "atlimage.h"
int CMilBuffer::m_milIDBuffCounter=0;

CMilBuffer::CMilBuffer(void):m_iFileFormat(M_BMP), m_iDepth(8+M_UNSIGNED), m_BufAttr(M_IMAGE+M_DISP)
, m_pHWndSelected(NULL)
, m_nChannel(3)
{
	m_pMilDisplay=NULL;
	m_milIDBuffCounter++;
	m_bAVIClosed = TRUE;
	m_milIDSystem = CMilSystem::Instance()->GetSystemID();
}

CMilBuffer::CMilBuffer(CSize ImageSize):m_iFileFormat(M_BMP), m_iDepth(8+M_UNSIGNED), m_BufAttr(M_DISP)
{
	m_pMilDisplay=NULL;
	m_milIDSystem=CMilSystem::Instance()->GetSystemID();
	MbufAlloc2d(m_milIDSystem, ImageSize.cx, ImageSize.cy, m_iDepth, m_BufAttr, &m_milIDBuffer);
}


CMilBuffer::~CMilBuffer(void)
{
	Close();	
}

/*! @fn int CMilBuffer::GetMilCounter(void)
 *  @brief �Ҵ�� Mil����ī��Ʈ�� �Ѵ�.(Child����)
 *  @todo ���� ����� �� ���� �����̸� �����Ͽ� �Ϸ��Ͽ��� �Ѵ�.
 *  @return �Ҵ�� ī��Ʈ������ ��ȯ�Ѵ�.
 */
int CMilBuffer::GetMilCounter(void)
{
	return m_milIDBuffCounter;
}


/*! @fn int CMilBuffer::Restore(WCHAR * strFileName)
 *  @brief ������ �о� MilBuffer�� �ִ´�.
 @n   Buffer Alloc�� �ڵ����� �̷������ File���䵵 �ڵ����� Ȯ���Ͽ� �о�´�
 *  @param strFileName �о�� ������ �̸�
 *  @return 0�̸� ���� �ٸ� ���� �����̴�
 */
int CMilBuffer::Restore(CString strFileName)
{
	tImageProperties ImageProperties;
//	GetFileExInfoStandard();
	CString strResult = _T("");
	strResult = PathFindExtension(strFileName.GetBuffer());
	strResult.MakeUpper();
	int rv = 0;
	if (strResult == _T(".JPG"))
	{
		rv = (int)RestoreJPG(strFileName.GetBuffer(), m_milIDSystem, &m_milIDBuffer);
	}else	rv= (int)MbufRestore(strFileName.GetBuffer(), m_milIDSystem, &m_milIDBuffer);
	if(rv==0)return -1;
	GetImageProperties(&ImageProperties);
	m_BufAttr=ImageProperties.Attr;
	m_iDepth=ImageProperties.Depth;
	return 0;
}

int CMilBuffer::RestoreJPG(CString strFileName, MIL_ID milIDSystem, MIL_ID * pMilIDBuffter)
{
	CImage tempImg;
	if (tempImg.Load(strFileName) != S_OK)return 0;

	int cx = tempImg.GetWidth();
	int cy = tempImg.GetHeight();

	MbufAllocColor(milIDSystem, 3, cx, cy, 8 + M_UNSIGNED, M_IMAGE+M_DISP, pMilIDBuffter);

	for (int i = 0; i < cy; i++)
	{
		MbufPutColor2d(*pMilIDBuffter, M_PACKED + M_BGR24, M_ALL_BANDS, 0, i, cx, 1, tempImg.GetPixelAddress(0, i));
	}

	return *pMilIDBuffter;
}


/*! @fn void CMilBuffer::Select(CMilDisplay * pMilDisplay)
 *  @brief Mil���۸� ���� Display�� �����Ѵ�.
 *  @param pMilDisplay �����ų MilDisplay(�Ҵ�Ǿ� �־����)
 */
void CMilBuffer::Select(CMilDisplay * pMilDisplay)
{
	MdispSelect(pMilDisplay->GetDisplayID(), m_milIDBuffer);
}

/*! @fn void CMilBuffer::SelectWindow(HWND ClientWindowHandle)
 *  @brief Mil���۸� ���� Dialog�Ͱ��� �����츦 �����ϴ� �Լ�
 *  @param ClientWindowHandle �����ų �������� �ڵ�
 */
void CMilBuffer::SelectWindow(HWND ClientWindowHandle)
{
	MdispSelectWindow(m_pMilDisplay->GetDisplayID(), m_milIDBuffer, ClientWindowHandle);
}

/*! @fn void CMilBuffer::UnSelectDisp(void)
 *  @brief Mil Buffer�� Display�� ����Ȱ��� �����Ѵ�.
 */
void CMilBuffer::UnSelectDisp(void)
{
	MdispSelect(m_pMilDisplay->GetDisplayID(), NULL);
}

BOOL CMilBuffer::IsSelectDisp(void)
{
	MIL_ID IDBuffer;
	if(!m_pMilDisplay)return FALSE;
	MIL_ID IDDisp = m_pMilDisplay->GetDisplayID();
	if (!IDDisp)return FALSE;
	MdispInquire(m_pMilDisplay->GetDisplayID(), M_SELECTED, &IDBuffer);
	if (IDBuffer == M_NULL)return FALSE; else return TRUE;
}

/*! @fn void CMilBuffer::SelectWindow(CMilDisplay * pMilDisplay, HWND ClientWindowHandle)
 *  @brief �Ķ���ͷ� ������ Display�� ������ �ڵ�� MilBuffer�� �����Ѵ�.
 *	@param pMilDisplay ������ Display buffer
 *  @param ClientWindowHandle ǥ���� Dialog�� ���� 
 */
void CMilBuffer::SelectWindow(CMilDisplay * pMilDisplay, HWND ClientWindowHandle)
{
	MdispSelectWindow(pMilDisplay->GetDisplayID(), m_milIDBuffer, ClientWindowHandle);
}

/*! @fn void CMilBuffer::SelectWindow(CWnd *pHWindow)
 *  @brief Mil Buffer�� ������ �ڵ��� �����Ų��(ȭ�鿡 ���̱�����)
 *  @param pHWindow Mil Buffer�� ���� ������ �ڵ� Class
 */
void CMilBuffer::SelectWindow(CWnd *pHWindow)
{
	m_pHWndSelected = pHWindow;
	MdispSelectWindow(m_pMilDisplay->GetDisplayID(), m_milIDBuffer, pHWindow->GetSafeHwnd());
}

/*! @fn void CMilBuffer::SelectWindow(CMilDisplay * pMilDisplay, CWnd *pHWindow)
 *  @brief Mil Buffer�� ������ �ڵ��� �����Ų��(ȭ�鿡 ���̱�����)
 *  @param pHWindow Mil Buffer�� ���� ������ �ڵ� Class
 */
void CMilBuffer::SelectWindow(CMilDisplay * pMilDisplay, CWnd *pHWindow, BOOL bRemove)
{
	m_pMilDisplay = pMilDisplay;
	if(bRemove)
		MdispSelectWindow(m_pMilDisplay->GetDisplayID(), M_NULL, M_NULL);
	else
		MdispSelectWindow(m_pMilDisplay->GetDisplayID(), m_milIDBuffer, pHWindow->GetSafeHwnd());
}

/*! @fn BOOL CMilBuffer::IsDispAllocated(void)
 *  @brief Mil Buffer�� Display buffer�� ����Ǿ� �ִ��� Ȯ���� �� �ִ� �Լ�
 *  @return TRUE:�Ҵ�Ǿ� ���� FALSE:�Ҵ�� Display�� ����
 */
BOOL CMilBuffer::IsDispAllocated(void)
{
	if(m_pMilDisplay) return TRUE;
	else return FALSE;
}

/*! @fn int CMilBuffer::Close(void)
 *  @brief Mil Buffer�� ������
 *  @return ���� ������ 0���� ����
 */
int CMilBuffer::Close(void)
{
	if (m_milIDGraph != M_NULL)
	{
		CMilGraph::Free();
	}

	if(m_milIDBuffer!=M_NULL)
	{
		m_milIDBuffCounter--;
		MbufFree(m_milIDBuffer);
		m_milIDBuffer=M_NULL;
	}
	return 0;
}

/*! @fn BOOL CMilBuffer::IsAlloced(void)
 *  @brief Mil Buffer�� �Ҵ�Ǿ� �ִ��� Ȯ��
 *  @return 1:�Ҵ�Ǿ� ���� 0:�Ҵ�Ǿ� ���� ����
 */
BOOL CMilBuffer::IsAlloced(void)
{
	if(m_milIDBuffer)
		return 1;
	else 
		return 0;
}

/*! @fn void CMilBuffer::ConvertToBMPFmt(CMilBuffer *pBMPBuffer, MIL_INT64 BufferAttribute)
 *  @brief Mil Buffer�� BMP���� ����� �� �ִ� ������ ���۷� �������
 *  @n 16bit->8bit�� Gray value�� leveling��
 *	@n buffer�Ҵ���� �̷����
 *  @param pBMPBuffer ��ȯ�� ���۰� ����� Mil buffer(Alloc�� ���� ���� ����)
 *  @param BufferAttribute ��ȯ�� ������ Attribute
 */
void CMilBuffer::ConvertToBMPFmt(CMilBuffer *pBMPBuffer, MIL_INT64 BufferAttribute)
{
	tImageProperties imgProperty;
	long GrayMax, GrayMin;
	GetImageProperties(&imgProperty);
	CMilIm::GetMinMax(this, &GrayMin, &GrayMax);
	pBMPBuffer->SetImageProperties(&imgProperty);
	pBMPBuffer->SetDepth(8+M_UNSIGNED);
	pBMPBuffer->SetAttribute(BufferAttribute);
	pBMPBuffer->Alloc2D(imgProperty.ImageSize);
	WndLvlTranform(pBMPBuffer, GrayMin, GrayMax, 0, 255);
}

/*! @fn void CMilBuffer::ExportBMP(WCHAR * strFileName)
 *  @brief Mil Buffer�� ������ BMP���Ϸ� ����
 *  @n 16bit->8bit�� Gray value�� leveling��
 *  @param strFileName ������ �����̸�
 */
void CMilBuffer::ExportBMP(CString strFileName)
{
	tImageProperties ImageProperty;
	GetImageProperties(&ImageProperty);
	if (ImageProperty.Depth != 8)
	{
		CMilBuffer newImage;
		ConvertToBMPFmt(&newImage);
		MbufExport(strFileName.GetBuffer(), M_BMP, newImage.GetIDBuffer());
		newImage.Close();
	}
	else
		MbufExport(strFileName.GetBuffer(), M_BMP, m_milIDBuffer);
}

/*! @fn void CMilBuffer::ExportJPG(WCHAR * strFileName)
*  @brief Mil Buffer�� ������ JPG���Ϸ� ����->�߰� Ű�� ���� ó���ϱ� ����
*  @n 16bit->8bit�� Gray value�� leveling�� ������ CImage�� �̿��Ͽ� JPG�� ����
*  @param strFileName ������ �����̸�
*/
void CMilBuffer::ExportJPG(CString strFileName)
{
	CImage tempImg;
//	CMilBuffer newImage;
//	ConvertToBMPFmt(&newImage);
	tImageProperties ImageProperty;
	GetImageProperties(&ImageProperty);
	int cx = ImageProperty.ImageSize.cx;
	int cy = ImageProperty.ImageSize.cy;

	tempImg.Create(cx, cy, 24);
	for (int i = 0; i < cy; i++)
	{
		GetColor2D(0, i, CSize(cx, 1), tempImg.GetPixelAddress(0, i), M_BGR24);
	}
	tempImg.Save(strFileName.GetBuffer());
//	newImage.Close();
}


int CMilBuffer::ExportAVIOpen(tExportAVI *pEA, int iOpenMode)
{
	m_pExportAVIInfo = pEA;
	if (!m_bAVIClosed)
	{
		ASSERT(TRUE);
		return -1;
	}
	pEA->FileFormat;
	pEA->iFrame = 0;
	MbufExportSequence(pEA->strFileName.GetBuffer(), pEA->FileFormat, M_NULL, M_NULL, pEA->dblFrameRate, iOpenMode);
	Sleep(10);

	m_bAVIClosed = FALSE;
	return 0;
}

int CMilBuffer::ExportAVI(tExportAVI *pEA)
{
	if (m_bAVIClosed)
	{
		ASSERT(TRUE);
		return -1;
	}
	if (pEA == NULL)pEA = m_pExportAVIInfo;
	if (pEA == NULL)
	{
		ASSERT(TRUE);
		return -1;
	}
	MbufExportSequence(pEA->strFileName.GetBuffer(), pEA->FileFormat, &m_milIDBuffer, 1, pEA->dblFrameRate, M_WRITE);
	(pEA->iFrame) += 1;
	return 0;
}


int CMilBuffer::ExportAVIClose(tExportAVI *pEA)
{
	if (m_bAVIClosed)
	{
		ASSERT(TRUE);
		return -1;
	}
	CString strFileName = pEA->strFileName;
	int FileFormat = pEA->FileFormat;
	m_bAVIClosed = TRUE;
	Sleep(10);
	MbufExportSequence(strFileName.GetBuffer(), pEA->FileFormat, M_NULL, 0, pEA->dblFrameRate, M_CLOSE);
	m_pExportAVIInfo = NULL;
	return 0;
}


int CMilBuffer::BufImportJPG(CString strFileName, MIL_ID milIDSystem, MIL_ID milIDBuffter)
{
	CImage tempImg;
	if (tempImg.Load(strFileName)!=S_OK)
	{
		return M_NULL;
	}

	int cx = tempImg.GetWidth();
	int cy = tempImg.GetHeight();

	for (int i = 0; i < cy; i++)
	{
		MbufPutColor2d(milIDBuffter, M_PACKED + M_BGR24, M_ALL_BANDS, 0,0, cx, 1, tempImg.GetPixelAddress(0, i));
	}

	return milIDBuffter;
}

/*! @fn void CMilBuffer::Export(WCHAR * strFileName, int m_iFileFormat)
 *  @brief Mil Buffer�� ���������� ���Ϸ� ���� M_BMP, M_JPEG, M_TIFF
 *  @n BMP�� ��� 16bit->8bit�� Gray value�� leveling��
 *  @param strFileName ������ �����̸�
 *	@param m_iFileFormat ������ ���� ����
 *	@n M_TIFF, M_BMP
 *	@todo jpeg�� ��� ���̼����� ���� ������ �� ����. ���̼����� 10�������̻��ϱ� ������ jpeg�� ������ �� �ִ� ����� ���� �����ؾ���
 */
void CMilBuffer::Export(CString strFileName, int m_iFileFormat)
{
	switch(m_iFileFormat)
	{
	case M_BMP:
		ExportBMP(strFileName);
		break;
	case M_JPEG_LOSSY:
		ExportJPG(strFileName);
//		MbufExport(strFileName, M_JPEG_LOSSY, m_milIDBuffer);
		break;
	default:
		MbufExport(strFileName, m_iFileFormat, m_milIDBuffer); 
		break;
	}

}


/*! @fn void CMilBuffer::SetFileFormat(MIL_INT iFileFormat)
 *  @brief Mil Buffer�� ���������� ���Ϸ� ���� M_BMP, M_JPEG, M_TIFF
 *	@param m_iFileFormat ������ ���� ����
 *	@n M_BMP,M_JPEG_LOSSLESS, M_JPEG2000_LOSSLESS, M_JPEG_LOSSLESS_INTERLACED, M_JPEG2000_LOSSLESS_JP2
 *	@n M_JPEG_LOSSY,M_JPEG2000_LOSSY,M_JPEG_LOSSY_INTERLACED,M_JPEG2000_LOSSY_JP2,M_JPEG_LOSSY_RGB
 *	@n M_MIL+, M_RAW, M_TIFF+ 
 */
void CMilBuffer::SetFileFormat(MIL_INT iFileFormat)
{
	m_iFileFormat = iFileFormat;
}

CMilBuffer* CMilBuffer::operator=(CMilBuffer &  LValue)
{

	return NULL;
}

/*! @fn MIL_INT64 CMilBuffer::SetAttribute(MIL_INT64 BufferAttribute)
 *  @brief �Ҵ��� Mil Buffer�� Atrribute�� �����Ѵ�
 *  @n �̹��Ҵ��Ѱ��� �����ϴ°��� �ƴϴ�.
 *  @param BufferAttribute ������ �Ӽ���
 *  @n M_IMAGE, M_DISP, M_GRAB, M_PROC, M_DIB
 */
MIL_INT64 CMilBuffer::SetAttribute(MIL_INT64 BufferAttribute)
{
	return m_BufAttr  = BufferAttribute;
}

/*! @fn MIL_INT64 CMilBuffer::GetAttribute(void)
 *  @brief �Ҵ��Ҷ� ����� Attribute�� ������
 *  @n �Ҵ�Ȱ��� Ȯ���ϴ°��� GetImageProperties���
 *  @return �Ӽ� ��
 *  @n M_IMAGE, M_DISP, M_GRAB, M_PROC, M_DIB
 */
MIL_INT64 CMilBuffer::GetAttribute(void)
{
	return m_BufAttr;
}

/*! @fn void CMilBuffer::BufLoad(WCHAR * strFileName)
 *  @brief �̹� �Ҵ�Ǿ��ִ� ���ۿ� ������ �о� �ִ´�
 *  @n �̹� ũ�Ⱑ �Ҵ�Ǿ� �ִ� ���۸� ����ؾ� �Ѵ�.
 *  @param strFileName �о���� ���ϸ�
 */
void CMilBuffer::BufLoad(CString strFileName)
{
	return MbufLoad(strFileName.GetBuffer(), m_milIDBuffer);
}

/*! @fn int CMilBuffer::BufImport(WCHAR * strFileName, MIL_INT FileFormat, MIL_INT Operation)
 *  @brief �ٿ뵵�� �ε���ų�� �ִ� ����� �������ش�.
 *  @n ������ �����Ͽ� �ε��ϸ�, 
 *  @param strFileName �о���� ���ϸ�
 *  @param FileFormat �о���� ������ �������� �������ش� 
 *  @param Operation �о���̴� ����� �����Ѵ�
 *  @n M_LOAD, M_RESTORE ���۸� �Ҵ����� �ƴϸ� �Ҵ�� ���۸� �������.
    *return M_RESOTRE�� ��쿡�� �Ҵ�� ID��ȯ �Ҵ������ ��� M_NULL��ȯ
 */
int CMilBuffer::BufImport(CString strFileName, MIL_INT FileFormat, MIL_INT Operation)
{
	switch(FileFormat)
	{
	case M_JPEG2000:
		return BufImportJPG(strFileName.GetBuffer(), m_milIDSystem, m_milIDBuffer);
		break;
	default:
		return (int)MbufImport(strFileName.GetBuffer(), FileFormat, Operation, m_milIDSystem, &m_milIDBuffer);
		break;
	}
	return -1;
}

/*! @fn void CMilBuffer::GetImageProperties(tImageProperties *pImageProperty)
 *  @brief mil buffer�� �Ӽ��� �˷��ش�.
 *  @param pImageProperty tImageProperties����ü�� �Ӽ��� �ݳ��Ѵ�.
 */
void CMilBuffer::GetImageProperties(tImageProperties *pImageProperty)
{
	MIL_INT64 attr=M_NULL;
	long cx=0, cy=0, Depth=0, BandSize=0;
    cx = MbufInquire(m_milIDBuffer, M_SIZE_X, M_NULL);
	cy = MbufInquire(m_milIDBuffer, M_SIZE_Y, M_NULL);
	pImageProperty->Type = MbufInquire(m_milIDBuffer, M_TYPE, M_NULL);
	pImageProperty->Depth = MbufInquire(m_milIDBuffer, M_SIZE_BIT, M_NULL);
	pImageProperty->BandSize = MbufInquire(m_milIDBuffer, M_SIZE_BAND, M_NULL);
	MbufInquire(m_milIDBuffer, M_EXTENDED_ATTRIBUTE, &attr);
	pImageProperty->Attr = attr; 
	pImageProperty->Sign = MbufInquire(m_milIDBuffer, M_SIGN, M_NULL);
	pImageProperty->ImageSize=CSize(cx,cy);
}

/*! @fn int CMilBuffer::Alloc2D(CSize imageSize)
 *  @brief Mil Buffer�� �Ҵ��Ѵ�
 *  @n ������ �Ӽ��� ������ �̹� �����Ǿ� �־���Ѵ�(SetAttribute, SetDepth��)
 *  @param imageSize �Ҵ��� ������ ũ�⸦ �������ش�
 */
int CMilBuffer::Alloc2D(CSize imageSize)
{
	return (int)MbufAlloc2d(m_milIDSystem, imageSize.cx, imageSize.cy, m_iDepth, m_BufAttr, &m_milIDBuffer);
}

/*! @fn int CMilBuffer::AllocColor2D(CSize imageSize)
*  @brief Color Mil Buffer�� �Ҵ��Ѵ�
*  @n ������ �Ӽ��� ������ �̹� �����Ǿ� �־���Ѵ�(SetAttribute, SetDepth��)
*  @param imageSize �Ҵ��� ������ ũ�⸦ �������ش�
*/
int CMilBuffer::AllocColor2D(CSize imageSize)
{
	return (int)MbufAllocColor(m_milIDSystem, m_nChannel, imageSize.cx, imageSize.cy, m_iDepth, m_BufAttr, &m_milIDBuffer);
}

/*! @fn ResizeBuf(CMilBuffer *pMilDest,CdblSize imageSize)
 *  @brief �̹����� ũ�⸦ �����Ѵ�.
 *  @n ������ ũ��� ������� ����
 *  @param pMilDest ������ �̹����� ����� MilBuffer
 *  @param imageSize �̹��� ũ�����
 *  @n ��)cx=1.5,cy=2�̸� x������ 1.5�� y������ 2��� ����°���
 *	@return 0�� �ƴϸ� Error��
 */
void CMilBuffer::ResizeBuf(CMilBuffer *pMilDest,CdblSize imageSize)
{
	MimResize(m_milIDBuffer, pMilDest->m_milIDBuffer, imageSize.dblCX, imageSize.dblCY, M_DEFAULT);
}

/*! @fn void CMilBuffer::Put2D(CSize offsetPos, CSize sizeImage, void *pUserArrayPtr)
 *  @brief ����� �迭�� pUserArrayPtr�� �ִ� �����͸� Mil Buffer�� �����Ѵ�
 *  @param offsetPos Copy������ġ
 *  @param sizeImage offsetPos��ġ���� sizeImageũ������
 *  @param pUserArrayPtr �̹����� �ִ� �迭�� ��ġ
 */
void CMilBuffer::Put2D(CSize offsetPos, CSize sizeImage, void *pUserArrayPtr)
{
	MbufPut2d(m_milIDBuffer, offsetPos.cx, offsetPos.cy, sizeImage.cx, sizeImage.cy, pUserArrayPtr);
}

/*! @fn CMilBuffer * CMilBuffer::Child2D(CRect *pRect)
 *  @brief Child���۸� �����.
 *  @n Child�� C�� �����Ͱ������� Child���� ����Ȱ��� �������� ������ ��ģ��. ���� ������ �����ϸ� Child���� ������ ��ģ��.
 *  @param pRect �������� pRect��ġ�� Child�� �����
 *  @return Child Buffer�� ��ȯ�Ѵ�
 */
CMilBuffer * CMilBuffer::Child2D(CRect *pRect)
{
	return Child2D(pRect->left,pRect->top, pRect->Width(), pRect->Height());
}

/*! @fn CMilBuffer * CMilBuffer::Child2D(int offsetX, int offsetY, int childWidth, int childHeight)
 *  @brief OffsetX, OffsetY���� ChildWidth��, childHeight���̷� Child�� �����
 */
CMilBuffer * CMilBuffer::Child2D(int offsetX, int offsetY, int childWidth, int childHeight)
{
	CMilBuffer *pBuffer=new CMilBuffer;
	pBuffer->m_pMilDisplay = m_pMilDisplay;
	pBuffer->m_iDepth = m_iDepth;
	pBuffer->m_BufAttr = m_BufAttr;
	pBuffer->m_milIDSystem = m_milIDSystem;
	pBuffer->m_milIDBuffer=MbufChild2d(m_milIDBuffer, offsetX, offsetY, childWidth, childHeight, M_NULL);
	return pBuffer;
}

/*! @fn int CMilBuffer::Create2D(CSize sizeImage,void * pAddr, int nDepth, int Attr)
 *  @brief sizeImageũ��� pAddr�� Host Address�� �ϴ� ���۸� �����Ѵ�.
 *  @param nDepth ������ ���� depth 
 *  @param Attr ������ ������ Attribute 
 *  @return return 0�� �ƴϸ� Error��
 */
int CMilBuffer::Create2D(CSize sizeImage,void * pAddr, int nDepth, int Attr)
{
	return (int)MbufCreate2d(m_milIDSystem, sizeImage.cx, sizeImage.cy, nDepth, Attr, M_HOST_ADDRESS, M_DEFAULT,  pAddr, &m_milIDBuffer);
}

/*! @fn void * CMilBuffer::GetAddr(void)
 *  @brief ������ �޸��ּҸ� ��´�.
 *  @return �޸𸮹����� �ּҸ� ��ȯ�Ѵ� 0�̸� ã�� ����(Error)
 */
void * CMilBuffer::GetAddr(void)
{
	void * pAddr=NULL;
	MbufInquire(m_milIDBuffer, M_HOST_ADDRESS, &pAddr);
	return pAddr;
}

void CMilBuffer::Lock()
{
	MbufControl(m_milIDBuffer, M_LOCK, M_DEFAULT);
}
void CMilBuffer::Unlock()
{
	MbufControl(m_milIDBuffer, M_UNLOCK, M_DEFAULT);
}

/*! @fn void CMilBuffer::SetDepth(int Depth)
 *  @brief �Ҵ��� ������ Depth����
 *	@n 1,8,16,32 M_FLOAT, M_SIGNED, M_UNSIGNED
 */
void CMilBuffer::SetDepth(int Depth)
{
	m_iDepth=Depth;
}

/*! @fn CMilBuffer* CMilBuffer::DuplicateBuff(void)
 *  @brief ������ �Ӽ��� ũ���� ���۸� �����Ͽ� ��ȯ
 *  @return ������ Mil buffer
 */
CMilBuffer* CMilBuffer::DuplicateBuff(void)
{
	tImageProperties ImageProperties;
	GetImageProperties(&ImageProperties);

	CMilBuffer * pMilBuffer = new CMilBuffer;
	pMilBuffer->m_pMilDisplay = m_pMilDisplay;
	pMilBuffer->m_iDepth = m_iDepth;
	pMilBuffer->m_BufAttr = ImageProperties.Attr;
	pMilBuffer->m_milIDSystem = m_milIDSystem;
	pMilBuffer->Alloc2D(ImageProperties.ImageSize);
	return pMilBuffer;
}

/*! @fn void CMilBuffer::Clear(MIL_DOUBLE ClearColor)
 *  @brief ���۸� Clear�Ѵ�
 *  @n �ʱ������ �����Ⱚ���� ���ﶧ Ȥ�� �ִ� �̹����� ���ﶧ
 *  @param ClearColor ȭ���� ���ﶧ ����(���������� ��� �̻������� ü������
 *  @M_RGB888(MIL_INT Red, MIL_INT Green, MIL_INT Blue), M_COLOR_BLACK, M_COLOR_RED... 
 */
void CMilBuffer::Clear(MIL_DOUBLE ClearColor)
{
	MbufClear(m_milIDBuffer, ClearColor);
}

/*! @fn int CMilBuffer::Copy(CMilBuffer* pMilSrc)
 *  @brief pMilSrc���� �̹����� �����ؿ´�
 */
int CMilBuffer::Copy(CMilBuffer* pMilSrc)
{
	pMilSrc->m_pMilDisplay = m_pMilDisplay;
	pMilSrc->m_iDepth = m_iDepth;
	pMilSrc->m_BufAttr = m_BufAttr;
	MbufCopy(pMilSrc->m_milIDBuffer, m_milIDBuffer);
	return 0;
}

/*! @fn int CMilBuffer::CopyClip(CMilBuffer* pMilClipBuffer, CPoint Offset)
 *  @brief pMilClipBuffer�� Offset��ġ���� �̹����� �о� �����ؿ´�.
 */
int CMilBuffer::CopyClip(CMilBuffer* pMilClipBuffer, CPoint Offset)
{
	MbufCopyClip(pMilClipBuffer->m_milIDBuffer, m_milIDBuffer, Offset.x, Offset.y);

	return 0;
}

/*! @fn int CMilBuffer::Get2D(void * pUserArray, CRect rect)
 *  @brief Mil Buffer�� �̹����� ����� �迭�� pUserArray�ּҷ� rectũ�� �̹����� �о�´�
 *  @n Mil Buffer�� �̹����� �ٸ� ���α׷����� ����ϰų� Pixel������ �����ϱ� ���ػ��
 */
int CMilBuffer::Get2D(void * pUserArray, CRect rect)
{
	MbufGet2d(m_milIDBuffer, rect.left, rect.top, rect.Width(), rect.Height(), pUserArray);

	return 0;
}

/*! @fn int CMilBuffer::GetColor(void * pUserArray)
*  @brief Mil Buffer�� �̹����� ����� �迭�� pUserArray�ּҷ� �̹����� �о�´�
*  @n Mil Buffer�� �̹����� �ٸ� ���α׷����� ����ϰų� Pixel������ �����ϱ� ���ػ��
*/
int CMilBuffer::GetColor(void * pUserArray)
{
	MbufGetColor(m_milIDBuffer, M_PACKED+M_RGB24,  M_ALL_BANDS, pUserArray);

	return 0;
}

/*! @fn int CMilBuffer::SetImageProperties(tImageProperties* pImageProperties)
 *  @brief Alloc�� �Ҷ� �ٸ� ImagePorperties�� ����Ͽ� ������
 *  @n Alloc�� �ؾ߸� ������ ��ħ
 */
int CMilBuffer::SetImageProperties(tImageProperties* pImageProperties)
{
	m_iDepth = pImageProperties->Depth+pImageProperties->Sign;
	m_BufAttr = pImageProperties->Attr;
	return 0;
}

/*! @fn int CMilBuffer::SetDisplay(CMilDisplay* pMilDisplay)
 *  @brief Display���۸� �Ҵ���
 *  @n ������ ���� �ʰ� ����
 */
int CMilBuffer::SetDisplay(CMilDisplay* pMilDisplay)
{
	m_pMilDisplay = pMilDisplay;
	return 0;
}

/*! @fn CMilDisplay* CMilBuffer::GetDisplay()
 *  @brief �Ҵ�� Display�� �ּҸ� ��´�.
 *  @return CMilDisplay�� �ּҰ� ��ȯ�ȴ�.
 */
CMilDisplay* CMilBuffer::GetDisplay()
{
	return m_pMilDisplay;
}

/*! @fn int CMilBuffer::GetIDBuffer(void)
 *  @brief ������ Mil ID�� ��ȯ�Ѵ�.
 *  @return Mil Buffer�� Mil ID
 */
int CMilBuffer::GetIDBuffer(void)
{
	return (int)m_milIDBuffer;
}

void CMilBuffer::SetIDBuffer(int milID)
{
	m_milIDBuffer=milID;
}

/*! @fn void CMilBuffer::CopyColor2D(CMilBuffer *pBufDest, CRect rectCopy, CPoint ofsDest, int bandDest, int bandSrc)
 *  @brief ���۴� ���۷� �����Ѵ�
 *  @param pBufDest ����Ǵ� ������ CMilBuffer����Ʈ
 *  @param rectCopy ���������� ������ �����ġ�� ũ��
 *  @param ofsDest ����Ǵ� ������ ���� ������ġ
 *  @param bandDest ����Ǵ� ������ Band��ġ
 *  @param bandSrc  ������ Band ��ġ Default=M_ALL_BANDS
 */
void CMilBuffer::CopyColor2D(CMilBuffer *pBufDest, CRect rectCopy, CPoint ofsDest, int bandDest, int bandSrc)
{
	MbufCopyColor2d(m_milIDBuffer, pBufDest->GetIDBuffer(),  bandSrc, rectCopy.left, rectCopy.top, bandDest, ofsDest.x, ofsDest.y, rectCopy.Width(), rectCopy.Height());
}

/*! @fn int CMilBuffer::GetDepth(void)
 *  @brief �Ҵ��� �� ����� Depth��ȯ
 *  @n ���簪�� ��Ȯ�� �˰��� �Ҷ��� GetImageProperties���
 *  @return �Ҵ��� �� ����� Depth
 */
int CMilBuffer::GetDepth(void)
{
	return (int)m_iDepth;
}

/*! @fn int CMilBuffer::WndLvlTranform(CMilBuffer *pBufDest, int OrgStart, int OrgEnd, int DestStart, int DestEnd)
 *  @brief Gray Value Windowingó���� �Ѵ�.
 *	@param pBufDest ó���� ����� �� ����
 *	@param OrgStart ������ ���� Gray value
 *	@param OrgEnd ������ �� Gray value
 *	@param DestStart ó���� ����� ���� Gray value
 *	@param DestEnd ó���� ����� �ִ� Gray value
 */
int CMilBuffer::WndLvlTranform(CMilBuffer *pBufDest, int OrgStart, int OrgEnd, int DestStart, int DestEnd)
{
	MIL_ID ImgLUT;
	double dblOrgStart=OrgStart, dblOrgEnd=OrgEnd;
	long sizeTable=2 << (GetDepth()-1);
	MIL_DOUBLE dblDestStart=DestStart;
	int MaxValue=CMilIm::GetMax(this);
	int LimitEnd=(OrgEnd<MaxValue)?OrgEnd:MaxValue;
	MbufAlloc1d(CMilSystem::Instance()->GetSystemID(), sizeTable, GetDepth()+M_UNSIGNED, M_LUT, &ImgLUT);
	MgenLutRamp(ImgLUT, 0, 0, OrgStart,0);
	MgenLutRamp(ImgLUT, OrgStart, DestStart, OrgEnd,DestEnd);
	MgenLutRamp(ImgLUT, LimitEnd, DestEnd, MaxValue, DestEnd);
	//if(MaxValue>4095)TRACE(_T("MaxError"));
	MimLutMap(GetIDBuffer(), pBufDest->GetIDBuffer(), ImgLUT);
	MbufFree(ImgLUT);
	return 0;
}

int CMilBuffer::GetPixelGray(int x, int y, int * vGray)
{
	Get2D(vGray, CRect(x,y, x+1, y+1));
	return 0;
}

int CMilBuffer::GetPixelGray(CPoint posPixel, int * vGray)
{
	Get2D(vGray, CRect(posPixel.x,posPixel.y, posPixel.x+1, posPixel.y+1));
	return 0;
}

int CMilBuffer::GetPixelColor(int x, int y, COLORREF * vColor, int typeColor)
{
	MbufGetColor2d(m_milIDBuffer, M_PACKED + typeColor, M_ALL_BANDS, x, y, 1, 1, (void *)vColor);
	return 0;
}

int CMilBuffer::GetColor2D(int x, int y, CSize sizeImage, void * pBuffer, int typeColor)
{
	MbufGetColor2d(m_milIDBuffer, M_PACKED + typeColor, M_ALL_BANDS, x, y, (MIL_INT)sizeImage.cx, (MIL_INT)sizeImage.cy, pBuffer);
//	MbufGetColor2d(m_milIDBuffer, M_RGB24, M_ALL_BANDS, x, y, (MIL_INT)sizeImage.cx, (MIL_INT)sizeImage.cy, pBuffer);
	return 0;
}
int CMilBuffer::GetPixelColor(CPoint posPixel, COLORREF * vColor)
{
	MbufGetColor2d(m_milIDBuffer, M_PACKED + M_RGB24, M_ALL_BANDS, posPixel.x, posPixel.y, 1, 1, (void *)vColor);
	return 0;
}

int CMilBuffer::LoadRawFile(CString strFileName, CSize rawSize)
{
	CFile rawFile;
	CFileException e;
	int sizeFile;
	if(!rawFile.Open(strFileName, CFile::modeRead, &e))return -1;

	sizeFile=(int)rawFile.SeekToEnd();
	rawFile.SeekToBegin();
	

	BYTE *pRawData= new BYTE[rawSize.cx * rawSize.cy *(m_iDepth/8)];
	rawFile.Read(pRawData, sizeFile);

	
	//MbufAlloc2d(m_milIDSystem, rawSize.cx, rawSize.cy, m_iDepth, m_BufAttr, &m_milIDBuffer);
	//MbufPut2d(m_milIDBuffer, 0, 0, rawSize.cx, rawSize.cy, pRawData);
	//delete pRawData;

	MbufCreate2d((MIL_ID)m_milIDSystem, rawSize.cx, rawSize.cy, m_iDepth+M_UNSIGNED, m_BufAttr, M_HOST_ADDRESS, M_DEFAULT, pRawData, &m_milIDBuffer);
  
	return 0;
}

void CMilBuffer::Put(void * pUserArrayPtr)
{
	MbufPut(m_milIDBuffer, pUserArrayPtr);
}

void CMilBuffer::SetChannelCount(int nChannel)
{
	m_nChannel = nChannel;
}


