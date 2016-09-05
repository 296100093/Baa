#include "DeviceTexture.h"
#include "DeviceTextureInternal.h"
#include "DeviceDataMgr.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BB_DEV_SMART_REF_IMPL(BbDevTex1D,GetTex1DItl);

void BbDevTex1D::Create(const wchar_t* str)
{
	assert( BBUL_MAX==m_Idx );
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	m_Idx = mgr->CreateTex1D( str );
}

void BbDevTex1D::Create(const BbDevTextureDesc* desc)
{
	assert( BBUL_MAX==m_Idx );
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	m_Idx = mgr->CreateTex1D( desc );
}

BBUL BbDevTex1D::Data() const
{
	return m_Idx;
}

//////////////////////////////////////////////////////////////////////////

BB_DEV_SMART_REF_IMPL(BbDevTex2D,GetTex2DItl);

void BbDevTex2D::Create(const wchar_t* str)
{
	assert( BBUL_MAX==m_Idx );
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	m_Idx = mgr->CreateTex2D( str );
}

void BbDevTex2D::Create(const BbDevTextureDesc* desc)
{
	assert( BBUL_MAX==m_Idx );
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	m_Idx = mgr->CreateTex2D( desc );
}

BBUL BbDevTex2D::Data() const
{
	return m_Idx;
}

//////////////////////////////////////////////////////////////////////////

BB_DEV_SMART_REF_IMPL(BbDevTex3D,GetTex3DItl);

void BbDevTex3D::Create(const BbDevTextureDesc* desc)
{
	assert( BBUL_MAX==m_Idx );
	desc;
}

BBUL BbDevTex3D::Data() const
{
	return m_Idx;
}

//////////////////////////////////////////////////////////////////////////

BB_DEV_SMART_REF_IMPL(BbDevRenderTarget,GetRenderTargetItl);

void BbDevRenderTarget::Create(const BbDevTextureDesc* desc)
{
	assert( BBUL_MAX==m_Idx );
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	m_Idx = mgr->CreateRenderTarget();
	mgr->GetRenderTargetItl( m_Idx )->Create( desc );
}

BBUL BbDevRenderTarget::Data() const
{
	return m_Idx;
}

//////////////////////////////////////////////////////////////////////////

BB_DEV_SMART_REF_IMPL(BbDevDepthStencil,GetDepthStencilItl);

void BbDevDepthStencil::Create(const BbDevTextureDesc* desc)
{
	assert( BBUL_MAX==m_Idx );
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	m_Idx = mgr->CreateDepthStencil();
	mgr->GetDepthStencilItl( m_Idx )->Create( desc );
}

BBUL BbDevDepthStencil::Data() const
{
	return m_Idx;
}

//////////////////////////////////////////////////////////////////////////

BbDevRenderPad::BbDevRenderPad()
{
	SetDefault();
}

BbDevRenderPad* BbDevRenderPad::Default()
{
	SetDefault();
	return this;
}

BbDevRenderPad* BbDevRenderPad::Add( const BbDevRenderTarget& rhs )
{
	SetDefault();
	m_Idx[0] = rhs.Data();
	return this;
}

BbDevRenderPad* BbDevRenderPad::Append( const BbDevRenderTarget& rhs )
{
	if ( m_Count<8 )
	{
		m_Idx[m_Count++] = rhs.Data();
	}
	return this;
}

BbDevRenderPad* BbDevRenderPad::Add( const BbDevDepthStencil& rhs )
{
	m_Idx[8] = rhs.Data();
	return this;
}

void BbDevRenderPad::Active()
{

}

bool BbDevRenderPad::Equal( BbDevRenderPad* rhs )
{
	return rhs->Count()==m_Count && 0==memcpy_s( rhs->Data(), sizeof(BBUL) * 8, m_Idx, sizeof(BBUL) * 8 );
}

BBUL* BbDevRenderPad::Data()
{
	return m_Idx;
}

BBUL BbDevRenderPad::Count()
{
	return m_Count;
}

void BbDevRenderPad::SetDefault()
{
	memset( m_Idx, 0xFF, sizeof(BBUL) * 8 );
	m_Count = 1;
}