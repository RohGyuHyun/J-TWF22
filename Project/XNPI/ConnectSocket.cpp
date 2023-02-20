// ConnectSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ConnectSocket.h"

CConnectSocket::CConnectSocket()
	: m_nCurrentRetryCnt(0)
	, m_bReceived(FALSE)
	, m_nTestCnt(0)
{
}

CConnectSocket::~CConnectSocket()
{
}

void CConnectSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	if (m_pfOnclose != NULL)
		m_pfOnclose();
}

void CConnectSocket::OnReceive(int nErrorCode)
{
	m_nTestCnt--;

	int rcvValueResult = 0;
	byte rcvBuffer[30];
	::ZeroMemory(rcvBuffer, sizeof(rcvBuffer));

	int received = Receive(rcvBuffer, sizeof(rcvBuffer));
	if (received > 0)
	{
		//! 11�� PLC�� input register �����͸� �б� ��û�������� ����, 12��  holding register�� ���������� ����  - kiy
		if (received == 11 || received == 12)
		{

			WORD trID = MAKEWORD(rcvBuffer[1], rcvBuffer[0]);
			byte fCode = rcvBuffer[7];

			if (m_pfOnReceive != NULL)
			{
				if (fCode == 0x04 && received == 11) //! input register Function Code == 0x04  - kiy
				{
					byte len = rcvBuffer[8]; //! ���� �������� ���̰� ������ �迭  - kiy

					for (int i = 0; i < len; i++)
						rcvValueResult += rcvBuffer[8 + i + 1] * (int)(pow(16, (len - (i * 2))));

					m_pfOnReceive(trID, fCode, rcvValueResult);
				}
				else if (fCode == 0x06 && received == 12) //! holding register Function Code == 0x06  - kiy
				{
					//! PLC�� �� �����ʹ� Ȯ���� �ʿ� ������ ������� ���� ���ܵ�  - kiy
					rcvValueResult = rcvBuffer[11];
					m_pfOnReceive(trID, fCode, rcvValueResult);
				}
				else
				{
					//! 4, 6���� ���� ���Դٸ� ������ �ִ°� [4, 6�� Function Code�� �ǹ�]  - kiy
					CString str;
					str.Format(_T("ERR OnReceive FCODE : %d"), fCode);
					AfxMessageBox(str);
				}
			}
		}
		//! 11, 12�� �ƴѰ��� ������, 23���� �ö� ����, �������� ��쿡�� �ش� Receive�� �ǳʶ�.  - kiy
		else
		{
			std::wcout << "ERR OnReceive : " << received << std::endl;
		}
	}

	m_bReceived = TRUE;
	CSocket::OnReceive(nErrorCode);
}

void CConnectSocket::SetOnCloseCallback(tfSocketClosed funcOnClose)
{
	m_pfOnclose = std::move(funcOnClose);
}

void CConnectSocket::SetOnReceiveCallback(tfOnReceive funcOnReceive)
{
	m_pfOnReceive = std::move(funcOnReceive);
}
