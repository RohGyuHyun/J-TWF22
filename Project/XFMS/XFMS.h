
// XFMS.h : SCInspector ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
/**
@mainpage XFMS Project
@section intro �Ұ�
- �Ұ�      :   X-Ray�� �̿��Ͽ� ���� �������¸� �����ϱ� ���� App.���� ����@n
@section   Program XFMS
- ���α׷���		:  XFMS.EXE
- ���α׷�����	:  1���� ��Ʈ��Ÿ���� Xray ���ǵ����� �����̽��� �������� ������ �ϰ� �̶� �����µ��� �׷����� ���δ�.
@section  INOUTPUT I/O�ڷ�
- INPUT(��Ʈ��)   	:	Modulbus 2�� ����Ʈ �µ����� R7E-TS4_INST
- ���(Serial)		:	Ʃ��
- Camera			:   Viewworks
- Grabber			:	network type
@section  CREATEINFO �ۼ�����
- �ۼ���     		:	������
- �ۼ���		 	:	20/10/17
- ������			:	23/11/17
@section  MODIFYINFO    ��������
- ������/������  	:	��������
- ������/13/11/17	:	Commit Git Server
- 
*/
class CRepository;

// CXFMSApp:
// �� Ŭ������ ������ ���ؼ��� SCInspector.cpp�� �����Ͻʽÿ�.
//
#include "ModelData.h"

class CXFMSApp : public CWinAppEx
{
public:
	CXFMSApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	ULONG gdiplusToken;
	CRepository * m_pRepository;

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	int InitVision();
	virtual void OnClosingMainFrame(CFrameImpl* pFrameImpl);
	CModelData m_ModelData;
	int InitEnvInstance();
	BOOL InitGDIPlus();
};

extern CXFMSApp theApp;
