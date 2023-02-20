#pragma once
#include "afxcoll.h"
#include "atltypes.h"
#include <UniUtility.h>

class CModelBase;

/// CModelBase ��� ����Դϴ�.

class CModelBase : public CObject
{
	DECLARE_DYNAMIC(CModelBase)
public:
	enum enumType{typeBASE, typePART, typeALGO};
	CModelBase();
	virtual ~CModelBase();
	/// Component�� �����ϴ� Component Attribute�� ���� Frame���� Ư������ �����.
	CdblRect m_dblRect;
	/// Base, Part, Algorithm�� ����.
	enumType Type;
	/// Sub�� ����� OffSet�ʿ��ϸ� ���� Offset���� �о�, ���ؼ� �۾���.
	CRect m_Offset;
	virtual int LoadParam(CXmlNode* pNode);
	virtual int SaveParam(CXmlNode* pNode);
	CRect GetAccumulatedOffset(void);
	virtual int LoadAttr(void);
	virtual int SaveAttr(CXmlNode* pNode);
	virtual BOOL IsExist(CModelBase* pElement);
	virtual int GetBaseType(void){return typeBASE;}
	virtual CPtrList *GetList(void){return &m_ListElement;}
	virtual int Free(void);
public:
	CPtrList m_ListElement;
};


