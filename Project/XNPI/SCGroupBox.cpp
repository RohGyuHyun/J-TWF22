// SCGroupBox.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "XNPI.h"
#include "SCGroupBox.h"


// CSCGroupBox

IMPLEMENT_DYNAMIC(CSCGroupBox, CStatic)

CSCGroupBox::CSCGroupBox()
{

}

CSCGroupBox::~CSCGroupBox()
{
}


BEGIN_MESSAGE_MAP(CSCGroupBox, CStatic)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CSCGroupBox �޽��� ó�����Դϴ�.




BOOL CSCGroupBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rect;
	GetClientRect(rect);

	CBrush brushBlue(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&brushBlue);

	pDC->Rectangle(rect);
	pDC->SelectObject(pOldBrush);

	return TRUE;
	return CStatic::OnEraseBkgnd(pDC);
}
