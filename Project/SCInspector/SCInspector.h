
// SCInspector.h : SCInspector ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
/**
@mainpage SCInspector Project
@section intro �Ұ�
- �Ұ�      :   Shield Cap�˻縦 ���� App.���� ����@n
@section   Program SCInspector
- ���α׷���		:  SCInspector.EXE
- ���α׷�����	:  4���� ī�޶� �������̽��� ���� ���α׷��̴�.
Area Camera�� ����ϴ� ���� �˻�� Align�˻縦 ���Ѵ�.
2�� PC���� ��ġ�Ǿ� No1PC ���̰˻� 2��, Align 1�� No2PC���̰˻� 3��
@section  INOUTPUT I/O�ڷ�
- INPUT          	:	�������� PCI-DB64R Description.xls����
- OUTPUT      		:   �������� PCI-DB64R Description.xls����
- Camera			:   Sentec STC-ANB521PCL
- Grabber			:	Euresys Multicam
@section  CREATEINFO �ۼ�����
- �ۼ���     		:	������
- �ۼ���		 	:	09/08/17
- ������			:	23/08/17
@section  MODIFYINFO    ��������
- ������/������  	:	��������
- ������/13/08/17	:	Commit Git Server
- 
*/
class CRepository;

// CSCInspectorApp:
// �� Ŭ������ ������ ���ؼ��� SCInspector.cpp�� �����Ͻʽÿ�.
//
#include "ModelData.h"

class CSCInspectorApp : public CWinAppEx
{
public:
	CSCInspectorApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	ULONG gdiplusToken;

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	int InitVision();
	virtual void OnClosingMainFrame(CFrameImpl* pFrameImpl);
	CRepository * m_pRepository;
	CModelData m_ModelData;
	int InitEnvInstance();
	BOOL InitGDIPlus();
};

extern CSCInspectorApp theApp;
