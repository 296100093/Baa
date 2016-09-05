#include "DeviceDataInternal.h"
#include "DeviceDataMgr.h"
//#include <assert.h>

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

BbDevDataItl::BbDevDataItl()
	: m_Type( BB_DEV_DATA_UNKNOWN )
	, m_Idx(BBUL_MAX)
	, m_Ref(0)	
{

}

void BbDevDataItl::RefAdd()
{
	++m_Ref;
}

void BbDevDataItl::RefSub()
{
	--m_Ref;
	if ( 0==m_Ref )
	{
		BbDeviceDataMgr::Get()->SetRefNull( this );
	}
}

BBUS BbDevDataItl::RefNum()
{
	return m_Ref;
}

BbDevDataTypeE BbDevDataItl::Type() const
{
	return m_Type;
}

void BbDevDataItl::SetType(BbDevDataTypeE type)
{
	m_Type = type;
}

void BbDevDataItl::RefZero()
{
	m_Ref = 0;
}

void BbDevDataItl::RefMax()
{
	m_Ref = BBUS_MAX;
}

void BbDevDataItl::IdxSet( BBUL idx )
{
	m_Idx = idx;
}

BBUL BbDevDataItl::IdxGet()
{
	return m_Idx;
}

//////////////////////////////////////////////////////////////////////////

BbDevSrvItl::BbDevSrvItl()
	: m_ShaderResourceView(nullptr)
{

}

ID3D11ShaderResourceView* BbDevSrvItl::Srv()
{
	return m_ShaderResourceView;
}