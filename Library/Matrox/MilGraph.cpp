#include "StdAfx.h"
#include "MilGraph.h"
#include <UniUtility.h>
#include <mil.h>

CMilGraph::CMilGraph(void)
: m_milIDSystem(0)
, m_milIDGraph(0)
, m_milIDBuffer(0)
{
}

CMilGraph::~CMilGraph(void)
{
	Free();
}

//! color는  Mil Color를 사용 RGB(R,G,B)사용못함 M_COLOR_YELLOW, M_COLOR_RED등이나  M_RGB888(R,G,B)로 하여야함 RGB(255,0,0)과 M_RGB888(255,0,0)은 다른 값임
void CMilGraph::SetColor(MIL_DOUBLE color)
{
	MgraColor(m_milIDGraph, color);
}

void CMilGraph::Clear(void)
{
	MgraClear(m_milIDGraph, m_milIDBuffer);
}

int CMilGraph::EnableGraph(BOOL bState)
{
	if (bState)
	{
		return MgraAlloc(m_milIDSystem, (MIL_ID *)&m_milIDGraph);
	}
	else
	{
		Free();
	}
	return 0;
}

void CMilGraph::Free(void)
{
	if (m_milIDGraph)
	{
		MgraFree(m_milIDGraph);
		m_milIDGraph = NULL;
	}
}

void CMilGraph::Rect(double dblLeft, double dblTop, double dblRight, double dblBottom)
{
	MgraRect(m_milIDGraph, m_milIDBuffer,dblLeft, dblTop, dblRight, dblBottom);
}

void CMilGraph::Rect(CRect rectToDraw)
{
	MgraRect(m_milIDGraph, m_milIDBuffer, rectToDraw.left, rectToDraw.top, rectToDraw.right, rectToDraw.bottom);
}

void CMilGraph::Line(double dblLeft, double dblTop, double dblRight, double dblBottom)
{
	MgraLine(m_milIDGraph, m_milIDBuffer,dblLeft, dblTop, dblRight, dblBottom);
}

int CMilGraph::GetIDGraph(void)
{
	return m_milIDGraph;
}

void CMilGraph::Dot(double dblX, double dblY)
{
	MgraDot(m_milIDGraph, m_milIDBuffer, dblX, dblY);
}

void CMilGraph::OutputText(double dblX, double dblY, TCHAR * szText)
{
	MgraText(m_milIDGraph, m_milIDBuffer, dblX, dblY, szText);
}

void CMilGraph::FontScale(double dblXScale, double dblYScale)
{
	MgraFontScale(m_milIDGraph, dblXScale, dblYScale);
}

void CMilGraph::FontSize(int sizeFont)
{
	MgraFont(m_milIDGraph, sizeFont);
}

void CMilGraph::Arc(double dblXCenter, double dblYCenter, double dblXRad, double dblYRad, double dblStartAngle, double dblEndAngle)
{
	MgraArc(m_milIDGraph, m_milIDBuffer, dblXCenter, dblYCenter, dblXRad, dblYRad, dblStartAngle, dblEndAngle);
}

void CMilGraph::Ellipse(double dblXCenter, double dblYCenter, double dblXRad, double dblYRad)
{
	Arc(dblXCenter, dblYCenter, dblXRad, dblYRad, 0, 360);
}

CDC *CMilGraph::GetDC(void)
{
	HDC hDC;
	MbufControl(m_milIDBuffer, M_DC_ALLOC, M_DEFAULT);
	hDC = (HDC)MbufInquire(m_milIDBuffer, M_DC_HANDLE, M_NULL);
	if(hDC)
	{
		CDC *pDC = new(CDC);
		pDC->Attach(hDC);
		return pDC;
	}
	return NULL;
}

void CMilGraph::FreeDC(void)
{
     MbufControl(m_milIDBuffer, M_DC_FREE, M_DEFAULT);
}

int CMilGraph::Modified(void)
{
	MbufControl(m_milIDBuffer, M_MODIFIED, M_DEFAULT);
	return 0;
}

void CMilGraph::AddInteractiveRect()
{
	MdispControl(m_milIDBuffer, M_GRAPHIC_LIST_INTERACTIVE, M_ENABLE);
//	MgraRectAngle(m_milIDGraph, );
}


void CMilGraph::DrawPolygon(CPtrList *pList)
{
	int nCount = pList->GetCount();
	CdblPoint *pDblPointCurr=NULL;

	double *pDblPosX1 = new double[nCount];
	double *pDblPosY1 = new double[nCount];

	//pDblPosX1[0] = 10;
	//pDblPosY1[0] = 10;
	//pDblPosX1[1] = 1000;
	//pDblPosY1[1] = 10;
	//pDblPosX1[2] = 1000;
	//pDblPosY1[2] = 1000;
	//pDblPosX1[3] = 10;
	//pDblPosY1[3] = 1000;

	POSITION pos;
	pos = pList->GetHeadPosition();

	for (int i = 0; (i < nCount) && (pos != NULL) ; i++)
	{
		pDblPointCurr = (CdblPoint *)pList->GetAt(pos);
		pDblPosX1[i] = pDblPointCurr->dblPosX;
		pDblPosY1[i] = pDblPointCurr->dblPosY;
		pList->GetNext(pos);
	}

	MgraLines(m_milIDGraph, m_milIDBuffer, nCount, pDblPosX1, pDblPosY1, NULL, NULL, M_POLYGON + M_FILLED);
	delete pDblPosX1;
	delete pDblPosY1;
}
