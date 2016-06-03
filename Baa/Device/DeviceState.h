#pragma once

#include "DeviceData.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BbDevStateBlend : public BbDevData
{
public:
	static BbDevStateBlend* Create( D3D11_BLEND_DESC* desc );

public:
	BbDevStateBlend( ID3D11BlendState* state );

	void SetBlendFactor( float f0, float f1, float f2, float f3 );
	void SetSampleMask( UINT mask );
	float*	BlendFactor();
	UINT	SampleMask();
	ID3D11BlendState*	State();
private:
	float							m_BlendFactor[4];
	ID3D11BlendState*	m_BlendState;
	UINT						m_SampleMask;
	bool							m_Dirty;
};

//////////////////////////////////////////////////////////////////////////

class BbDevStateDepthStencil : public BbDevData
{
public:
	static BbDevStateDepthStencil* Create( D3D11_DEPTH_STENCIL_DESC* desc );

public:
	BbDevStateDepthStencil( ID3D11DepthStencilState* state );
	
	void SetStencilRef( UINT ref );
	UINT StencilRef();
	ID3D11DepthStencilState*	State();

private:	
	ID3D11DepthStencilState*	m_DepthStencilState;
	UINT	m_StencilRef;
};

//////////////////////////////////////////////////////////////////////////

class BbDevStateRasterizer : public BbDevData
{
public:
	static BbDevStateRasterizer* Create( D3D11_RASTERIZER_DESC* desc );

public:
	BbDevStateRasterizer( ID3D11RasterizerState* state );
	
	ID3D11RasterizerState*	State();

private:
	ID3D11RasterizerState*	m_RasterizerState;
};

//////////////////////////////////////////////////////////////////////////

class BbDevStateSampler : public BbDevData
{
public:
	static BbDevStateSampler* Create( D3D11_SAMPLER_DESC* desc );

public:
	BbDevStateSampler( ID3D11SamplerState* state );

	void RefSlotAdd( BBUC pipe, BBUC slot );
	void RefSlotSub( BBUC pipe, BBUC slot );
	void RefSlotClear();
	ID3D11SamplerState*		State();
private:
	ID3D11SamplerState*		m_SamplerState;
	BBUC		m_Slot[BB_PIPE_TYPE_COUNT];
	BBUC		m_AttachSlot;
};

//////////////////////////////////////////////////////////////////////////