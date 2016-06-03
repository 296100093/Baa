#include "DeviceState.h"
#include "DeviceDirect.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BbDevStateBlend* BbDevStateBlend::Create( D3D11_BLEND_DESC* desc )
{
	return s_BbDdMgr->CreateStateBlend( desc );
}

BbDevStateBlend::BbDevStateBlend( ID3D11BlendState* state )
	:	m_BlendState(state)
	,	m_SampleMask(0xFFFFFFFF)
	,	m_Dirty(false)
{
	m_Type = BB_DEV_DAT_STATE_BLEND;
	m_BlendFactor[0]=m_BlendFactor[1]=m_BlendFactor[2]=m_BlendFactor[3]=0.0f;
}

void BbDevStateBlend::SetBlendFactor( float f0, float f1, float f2, float f3 )
{
	m_BlendFactor[0]=f0;
	m_BlendFactor[1]=f1;
	m_BlendFactor[2]=f2;
	m_BlendFactor[3]=f3;
	m_Dirty = true;
}

void BbDevStateBlend::SetSampleMask( UINT mask )
{
	m_SampleMask = mask;
	m_Dirty = true;
}

float* BbDevStateBlend::BlendFactor()
{
	return m_BlendFactor;
}

UINT BbDevStateBlend::SampleMask()
{
	return m_SampleMask;
}

ID3D11BlendState* BbDevStateBlend::State()
{
	m_Dirty = false;
	return m_BlendState;
}

//////////////////////////////////////////////////////////////////////////

BbDevStateDepthStencil* BbDevStateDepthStencil::Create( D3D11_DEPTH_STENCIL_DESC* desc )
{
	return s_BbDdMgr->CreateStateDepthStencil( desc );
}

BbDevStateDepthStencil::BbDevStateDepthStencil( ID3D11DepthStencilState* state )
	:	m_StencilRef(1)
	,	m_DepthStencilState(state)
{
	m_Type = BB_DEV_DAT_STATE_DEPTH_STENCIL;
}

void BbDevStateDepthStencil::SetStencilRef( UINT ref )
{
	m_StencilRef = ref;
}

UINT BbDevStateDepthStencil::StencilRef()
{
	return m_StencilRef;
}

ID3D11DepthStencilState* BbDevStateDepthStencil::State()
{
	return m_DepthStencilState;
}

//////////////////////////////////////////////////////////////////////////

BbDevStateRasterizer* BbDevStateRasterizer::Create( D3D11_RASTERIZER_DESC* desc )
{
	return s_BbDdMgr->CreateStateRasterizer( desc );
}

BbDevStateRasterizer::BbDevStateRasterizer( ID3D11RasterizerState* state )
	:	m_RasterizerState(state)
{
	m_Type = BB_DEV_DAT_STATE_RASTERIZER;
}

ID3D11RasterizerState* BbDevStateRasterizer::State()
{
	return m_RasterizerState;
}

//////////////////////////////////////////////////////////////////////////

BbDevStateSampler* BbDevStateSampler::Create( D3D11_SAMPLER_DESC* desc )
{
	return s_BbDdMgr->CreateStateSampler( desc );
}

BbDevStateSampler::BbDevStateSampler( ID3D11SamplerState* state )
	:	m_SamplerState(state)
	,	m_AttachSlot(0)
{
	m_Slot[BB_PIPE_TYPE_VS] =
	m_Slot[BB_PIPE_TYPE_HS] =
	m_Slot[BB_PIPE_TYPE_GS] =
	m_Slot[BB_PIPE_TYPE_DS] =
	m_Slot[BB_PIPE_TYPE_PS] = BBUC_MAX;
	m_Type = BB_DEV_DAT_STATE_SAMPLER;
}

void BbDevStateSampler::RefSlotAdd( BBUC pipe, BBUC slot )
{
	if ( m_Slot[pipe]!=slot )
	{
		if ( BBUC_MAX!=m_Slot[pipe] )
		{
			s_BbDdMgr->Device()->DeActiveSrv(pipe, m_Slot[pipe]);
		}		
	}
	m_Slot[pipe] = slot;
	++m_AttachSlot;
	RefAdd();
}

void BbDevStateSampler::RefSlotSub( BBUC pipe, BBUC slot )
{
	if ( slot==m_Slot[pipe] )
	{
		m_Slot[pipe] = BBUC_MAX;
		--m_AttachSlot;
		Release();
	}
}

void BbDevStateSampler::RefSlotClear()
{
	if ( 0<m_AttachSlot )
	{
		for ( BBUC i(0); i < BB_PIPE_TYPE_COUNT; ++i )
		{
			if ( BBUC_MAX!=m_Slot[i] )
			{
				s_BbDdMgr->Device()->DeActiveSrv(pipe, m_Slot[pipe]);
			}
		}
	}
}

ID3D11SamplerState* BbDevStateSampler::State()
{
	return m_SamplerState;
}