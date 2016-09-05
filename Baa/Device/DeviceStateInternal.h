#pragma once

#include "DeviceDataInternal.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BbDevStateBlendItl : public BbDevDataItl
{
public:
	BbDevStateBlendItl();

	void	Create( D3D11_BLEND_DESC* desc );
	void	Destroy();
	void	DestroyNull();
	
	void	SetBlendFactor( float f0, float f1, float f2, float f3 );
	void	SetSampleMask( BBUL mask );
	
	ID3D11BlendState*	State();
	float*	BlendFactor();
	BBUL	SampleMask();	
	bool	Dirty();

	bool	Equal( D3D11_BLEND_DESC* desc );
	bool	Equal( BbDevStateBlendItl* rhs );

private:
	D3D11_BLEND_DESC		m_Desc;
	float					m_BlendFactor[4];
	ID3D11BlendState*		m_BlendState;
	BBUL					m_SampleMask;
	bool					m_Dirty;
};

//////////////////////////////////////////////////////////////////////////

class BbDevStateDepthStencilItl : public BbDevDataItl
{
public:
	BbDevStateDepthStencilItl();

	void	Create( D3D11_DEPTH_STENCIL_DESC* desc );
	void	Destroy();
	void	DestroyNull();

	void	SetStencilRef( BBUL ref );
		
	ID3D11DepthStencilState*	State();
	BBUL	StencilRef();
	bool	Dirty();

	bool	Equal( D3D11_DEPTH_STENCIL_DESC* desc );
	bool	Equal( BbDevStateDepthStencilItl* rhs );

private:
	D3D11_DEPTH_STENCIL_DESC	m_Desc;
	ID3D11DepthStencilState*	m_DepthStencilState;
	BBUL						m_StencilRef;
	bool						m_Dirty;
};

//////////////////////////////////////////////////////////////////////////

class BbDevStateRasterizerItl : public BbDevDataItl
{
public:
	BbDevStateRasterizerItl();

	void	Create( D3D11_RASTERIZER_DESC* desc );
	void	Destroy();
	void	DestroyNull();
	
	ID3D11RasterizerState*	State();

	bool	Equal( D3D11_RASTERIZER_DESC* desc );
	bool	Equal( BbDevStateRasterizerItl* rhs );

private:
	D3D11_RASTERIZER_DESC	m_Desc;
	ID3D11RasterizerState*	m_RasterizerState;
};

//////////////////////////////////////////////////////////////////////////

class BbDevStateSamplerItl : public BbDevDataItl
{
public:
	BbDevStateSamplerItl();

	void	Create( D3D11_SAMPLER_DESC* desc );
	void	Destroy();
	void	DestroyNull();

	ID3D11SamplerState*		State();

	bool	Equal( D3D11_SAMPLER_DESC* desc );
	bool	Equal( BbDevStateSamplerItl* rhs );

private:
	D3D11_SAMPLER_DESC		m_Desc;
	ID3D11SamplerState*		m_SamplerState;
};

//////////////////////////////////////////////////////////////////////////