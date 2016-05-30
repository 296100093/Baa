#pragma once

#include "CoreDef.h"

class BbMod;

class BbObjectMgr : public Singleton<BbObjectMgr>
{
	typedef std::map<unsigned long,BbMod*>	ModMgrMap;
public:
	template<class T> void RegisterMod();
	template<class T> T* GetMod();
	void Destroy();	
	
	BbMod* GetMod( unsigned long id );
private:
	void RegisterMod( BbMod* a, unsigned long id );
	

private:
	ModMgrMap	m_mModMgr;
};

template<class T>
void BbObjectMgr::RegisterMod()
{
	RegisterMod( new T, T::GetModId() );
}

template<class T>
T* BbObjectMgr::GetMod()
{
	return static_cast<T*>( GetMod( T::GetModId() ) );
};