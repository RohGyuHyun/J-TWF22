#pragma once
#include <UniUtility.h>
#include "ITFBase.h"

/// CModelBase ��� ����Դϴ�.
class ITFBASEDLLDECL CModelBase;

typedef CModelBase* (*tGetModelClass)(TCHAR * szModelName);

class ITFBASEDLLDECL CModelBase : public CWnd
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
	///ModelData�� �Լ��ιٲ۴�.
	static tGetModelClass pGetModelClass;
	CModelBase *m_pParentModel;
public:
	CPtrList m_ListElement;
	CModelBase * GetParentModel(void);
	void SetParentModel(CModelBase * pParentModel);
	CString GetParentModelName(void);
};


