// LedONOFF.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "APMDMonitor.h"
#include "LedONOFF.h"


// CLedONOFF

IMPLEMENT_DYNAMIC(CLedONOFF, CProgressCtrl)

CLedONOFF::CLedONOFF()
{

}

CLedONOFF::~CLedONOFF()
{
}


BEGIN_MESSAGE_MAP(CLedONOFF, CProgressCtrl)
END_MESSAGE_MAP()



// CLedONOFF �޽��� ó�����Դϴ�.




void CLedONOFF::Set(bool bLedStatus)
{
	SetWindowTheme(GetSafeHwnd(), L"", L"");
	if(bLedStatus)
		SetBarColor(RGB(100, 200, 0));
	else
		SetBarColor(RGB(255, 0, 0));

	SetRange(0, 1);
	SetPos(1);

}


BOOL CLedONOFF::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	SetWindowTheme(GetSafeHwnd(), L"", L"");
	SetBarColor(RGB(255, 0, 0));
	SetBkColor(RGB(0, 255, 255));
	SetRange(0, 1);
	SetPos(1);

	return CProgressCtrl::Create(dwStyle, rect, pParentWnd, nID);

}
