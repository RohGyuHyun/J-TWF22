// Broadcaster.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Broadcaster.h"
#include <list>
using namespace std;


// CBroadcaster

CBroadcaster::CBroadcaster()
{
}

CBroadcaster::~CBroadcaster()
{
}


// CBroadcaster ��� �Լ�

void CBroadcaster::RegObserver(CObserver* pObserver)
{
	m_ListObservers.push_front(pObserver);
}

void CBroadcaster::UnRegObserver(CObserver* pObserver)
{
	list<CObserver*>::iterator iter;
	for(iter=m_ListObservers.begin();iter != m_ListObservers.end(); iter++)
		if(*iter == pObserver)iter=m_ListObservers.erase(iter);
}

void CBroadcaster::Notify(void)
{
	list<CObserver*>::iterator iter;
	for(iter=m_ListObservers.begin();iter != m_ListObservers.end(); iter++)
		(*iter)->Update();
}
