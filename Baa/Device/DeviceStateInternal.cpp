#include "DeviceStateInternal.h"
#include "DeviceDataMgr.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BbDevStateBlendItl::BbDevStateBlendItl()
	:	m_BlendState(nullptr)
	,	m_SampleMask(0xFFFFFFFF)
	,	m_Dirty( false )
{
	SetType( BB_DEV_DATA_STATE_BLEND );
	m_BlendFactor[0]=m_BlendFactor[1]=m_BlendFactor[2]=m_BlendFactor[3]=0.0f;
	memset( &m_Desc, 0, sizeof(D3D11_BLEND_DESC) );
}

void BbDevStateBlendItl::Create( D3D11_BLEND_DESC* desc )
{
	assert(nullptr==m_BlendState);
	memcpy_s( &m_Desc, sizeof(m_Desc), desc, sizeof(D3D11_BLEND_DESC) );
	BbDeviceDataMgr::Get()->Device()->CreateBlendState( &m_Desc, &m_BlendState );
}

void BbDevStateBlendItl::Destroy()
{
	BB_SAFE_RELEASE(m_BlendState);
}

void BbDevStateBlendItl::DestroyNull()
{
// 	if ( 0==RefNum() )
// 	{
// 		Destroy();
// 	}
}

void BbDevStateBlendItl::SetBlendFactor( float f0, float f1, float f2, float f3 )
{
	m_BlendFactor[0]=f0;
	m_BlendFactor[1]=f1;
	m_BlendFactor[2]=f2;
	m_BlendFactor[3]=f3;
	m_Dirty = true;
}

void BbDevStateBlendItl::SetSampleMask( BBUL mask )
{
	if ( m_SampleMask!=mask )
	{
		m_SampleMask = mask;
		m_Dirty = true;
	}	
}

ID3D11BlendState* BbDevStateBlendItl::State()
{
	m_Dirty = false;
	return m_BlendState;
}

float* BbDevStateBlendItl::BlendFactor()
{
	return m_BlendFactor;
}

BBUL BbDevStateBlendItl::SampleMask()
{
	return m_SampleMask;
}

bool BbDevStateBlendItl::Dirty()
{
	return m_Dirty;
}

bool BbDevStateBlendItl::Equal( D3D11_BLEND_DESC* desc )
{
	return 0==memcpy_s( &m_Desc, sizeof(m_Desc), desc, sizeof(D3D11_BLEND_DESC) );
}

bool BbDevStateBlendItl::Equal( BbDevStateBlendItl* rhs )
{
	return this==rhs;
}

//////////////////////////////////////////////////////////////////////////

BbDevStateDepthStencilItl::BbDevStateDepthStencilItl()
	:	m_DepthStencilState(nullptr)
	,	m_StencilRef(1)	
	,	m_Dirty(false)
{
	SetType( BB_DEV_DATA_STATE_DEPTH_STENCIL );
	memset( &m_Desc, 0, sizeof(D3D11_DEPTH_STENCIL_DESC) );
}

void BbDevStateDepthStencilItl::Create( D3D11_DEPTH_STENCIL_DESC* desc )
{
	assert(nullptr==m_DepthStencilState);
	memcpy_s( &m_Desc, sizeof(m_Desc), desc, sizeof(D3D11_DEPTH_STENCIL_DESC) );
	BbDeviceDataMgr::Get()->Device()->CreateDepthStencilState( &m_Desc, &m_DepthStencilState );
}

void BbDevStateDepthStencilItl::Destroy()
{
	BB_SAFE_RELEASE(m_DepthStencilState);
}

void BbDevStateDepthStencilItl::DestroyNull()
{
// 	if ( 0==RefNum() )
// 	{
// 		Destroy();
// 	}
}

void BbDevStateDepthStencilItl::SetStencilRef( BBUL ref )
{
	if ( ref!=m_StencilRef )
	{
		m_Dirty = true;
		m_StencilRef = ref;
	}
}

ID3D11DepthStencilState* BbDevStateDepthStencilItl::State()
{
	m_Dirty = false;
	return m_DepthStencilState;
}

BBUL BbDevStateDepthStencilItl::StencilRef()
{
	return m_StencilRef;
}

bool BbDevStateDepthStencilItl::Dirty()
{
	return m_Dirty;
}

bool BbDevStateDepthStencilItl::Equal( D3D11_DEPTH_STENCIL_DESC* desc )
{
	return 0==memcpy_s( &m_Desc, sizeof(m_Desc), desc, sizeof(D3D11_DEPTH_STENCIL_DESC) );
}

bool BbDevStateDepthStencilItl::Equal( BbDevStateDepthStencilItl* rhs )
{
	return this==rhs;
}

//////////////////////////////////////////////////////////////////////////

BbDevStateRasterizerItl::BbDevStateRasterizerItl()
	:	m_RasterizerState(nullptr)
{
	SetType(BB_DEV_DATA_STATE_RASTERIZER);
	memset( &m_Desc, 0, sizeof(D3D11_RASTERIZER_DESC) );
}

void BbDevStateRasterizerItl::Create( D3D11_RASTERIZER_DESC* desc )
{
	assert(nullptr==m_RasterizerState);
	memcpy_s( &m_Desc, sizeof(m_Desc), desc, sizeof(D3D11_RASTERIZER_DESC) );
	BbDeviceDataMgr::Get()->Device()->CreateRasterizerState( &m_Desc, &m_RasterizerState );
}

void BbDevStateRasterizerItl::Destroy()
{
	BB_SAFE_RELEASE(m_RasterizerState);
}

void BbDevStateRasterizerItl::DestroyNull()
{
// 	if ( 0==RefNum() )
// 	{
// 		Destroy();
// 	}
}

ID3D11RasterizerState* BbDevStateRasterizerItl::State()
{
	return m_RasterizerState;
}

bool BbDevStateRasterizerItl::Equal( D3D11_RASTERIZER_DESC* desc )
{
	return 0==memcpy_s( &m_Desc, sizeof(m_Desc), desc, sizeof(D3D11_RASTERIZER_DESC) );
}

bool BbDevStateRasterizerItl::Equal( BbDevStateRasterizerItl* rhs )
{
	return this==rhs;
}

//////////////////////////////////////////////////////////////////////////

BbDevStateSamplerItl::BbDevStateSamplerItl()
	:	m_SamplerState(nullptr)
{
	SetType(BB_DEV_DATA_STATE_SAMPLER);
	memset( &m_Desc, 0, sizeof(D3D11_SAMPLER_DESC) );
}

void BbDevStateSamplerItl::Create( D3D11_SAMPLER_DESC* desc )
{
	assert(nullptr==m_SamplerState);
	memcpy_s( &m_Desc, sizeof(m_Desc), desc, sizeof(D3D11_SAMPLER_DESC) );
	BbDeviceDataMgr::Get()->Device()->CreateSamplerState( &m_Desc, &m_SamplerState );
}

void BbDevStateSamplerItl::Destroy()
{
	BB_SAFE_RELEASE(m_SamplerState);
}

void BbDevStateSamplerItl::DestroyNull()
{
// 	if ( 0==RefNum() )
// 	{
// 		Destroy();
// 	}
}

ID3D11SamplerState* BbDevStateSamplerItl::State()
{
	return m_SamplerState;
}

bool BbDevStateSamplerItl::Equal( D3D11_SAMPLER_DESC* desc )
{
	return 0==memcpy_s( &m_Desc, sizeof(m_Desc), desc, sizeof(D3D11_SAMPLER_DESC) );
}

bool BbDevStateSamplerItl::Equal( BbDevStateSamplerItl* rhs )
{
	return this==rhs;
}

//////////////////////////////////////////////////////////////////////////