// NumEdit.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "NumEdit.h"


// CNumEdit

IMPLEMENT_DYNAMIC(CNumEdit, CEdit)

CNumEdit::CNumEdit()
{

}

CNumEdit::~CNumEdit()
{
}


BEGIN_MESSAGE_MAP(CNumEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CNumEdit �޽��� ó�����Դϴ�.




void CNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (
		nChar == VK_RETURN || nChar == VK_BACK ||  // �齺���̽� ���
		(nChar == _T('.') || (_T('0') <= nChar && nChar <= _T('9'))) && // '-'��ȣ�� 0 ~ 9 ���� ���
		GetWindowTextLength() <= 8  // ���� 3���� ����
		)
	{
		if (nChar == _T('-')) // '-' �ߺ�����
		{
			CString str;
			GetWindowText(str);
			if (str.GetLength() >= 1 && str[0] == _T('-')) return;
		}
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}
