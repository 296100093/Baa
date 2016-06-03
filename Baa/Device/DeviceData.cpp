#include "DeviceData.h"
#include "DeviceDataMgr.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////

BbDeviceDataMgr* BbDevData::s_BbDdMgr = nullptr;
bool BbDevData::s_Run = false;

void BbDevData::InitMgr( BbDeviceDataMgr* mgr )
{	
	s_BbDdMgr = mgr;
	s_Run = true;
}

void BbDevData::DestroyMgr()
{
	s_Run = false;
	s_BbDdMgr = nullptr;
}

//////////////////////////////////////////////////////////////////////////

BbDevData::BbDevData()
	:	m_Ref(0)
	,	m_Type( BB_DEV_DAT_UNKNOWN )
{

}

void BbDevData::RefAdd()
{
	++m_Ref;
}

void BbDevData::Release()
{
	--m_Ref;
	if ( s_Run && 0==m_Ref )
	{
		s_BbDdMgr->SetRefNull( this );
	}
}

BBUC BbDevData::RefNum()
{
	return m_Ref;
}

BbDevDataTypeE BbDevData::Type() const
{
	return m_Type;
}

//////////////////////////////////////////////////////////////////////////

BbDevSrv::BbDevSrv()
{
	m_SlotSrv[BB_PIPE_TYPE_VS] =
	m_SlotSrv[BB_PIPE_TYPE_HS] =
	m_SlotSrv[BB_PIPE_TYPE_GS] =
	m_SlotSrv[BB_PIPE_TYPE_DS] =
	m_SlotSrv[BB_PIPE_TYPE_PS] = BB_DEV_SLOT_NULL;
}

void BbDevSrv::RefSlotAdd( BBUC pipe, BBUC slot )
{
	assert( BB_DEV_SLOT_NULL==m_SlotSrv[pipe] );
	m_SlotSrv[pipe] = slot;
	RefAdd();
}

void BbDevSrv::RefSlotSub( BBUC pipe, BBUC slot )
{
	assert( slot==m_SlotSrv[pipe] );
	m_SlotSrv[pipe] = BB_DEV_SLOT_NULL;
	Release();
}

void BbDevSrv::RefSlotClear()
{
	for ( BBUC i(0); i<BB_PIPE_TYPE_COUNT; ++i )
	{
		if ( BB_DEV_SLOT_NULL!=m_SlotSrv[i] )
		{

		}
	}
}