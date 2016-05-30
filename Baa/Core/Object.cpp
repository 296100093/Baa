#include "Object.h"
#include "String.h"
#include "ObjectMgr.h"

//////////////////////////////////////////////////////////////////////////

unsigned long BbMod::GetModId()
{
	static const BbStringHash sh("UnknownMod");
	return sh.Value();
}

BbMod* BbMod::GetMod( unsigned long id )
{
	return BbObjectMgr::Instance().GetMod( id );
}

//////////////////////////////////////////////////////////////////////////

BbObject::BbObject()
	:	m_objListen( nullptr )
	,	m_objId( 0 )
{

}

BbObject::~BbObject()
{

}

void BbObject::OnEvent( BbObject* sender, int evid, void* data )
{
	sender;
	evid;
	data;
}

void BbObject::InitObj( BbObject* lis, int id )
{
	m_objListen = lis;
	m_objId = id;
}

int BbObject::GetId()
{
	return m_objId;
}

void BbObject::PostEvent( int evid, void* data /* = nullptr */ )
{
	if ( nullptr!=m_objListen )
	{
		m_objListen->OnEvent( this, evid, data );
	}
}