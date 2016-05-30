#include "ObjectMgr.h"
#include "Object.h"


void BbObjectMgr::Destroy()
{
	for ( ModMgrMap::iterator iter(m_mModMgr.begin()), end(m_mModMgr.end()); iter!=end; ++iter )
	{
		delete iter->second;
	}
	m_mModMgr.clear();
}

BbMod* BbObjectMgr::GetMod( unsigned long id )
{
	ModMgrMap::iterator iter = m_mModMgr.find( id );
	return m_mModMgr.end()==iter?nullptr:iter->second;
}

void BbObjectMgr::RegisterMod( BbMod* a, unsigned long id )
{
	m_mModMgr.insert( ModMgrMap::value_type( id, a ) );
}
