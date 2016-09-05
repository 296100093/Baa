#pragma once

#include "DeviceDef.h"


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BbDevStateBlend
{
public:
	BB_DEV_SMART_REF_DECL(BbDevStateBlend);

	void Create(D3D11_BLEND_DESC* desc);
	void SetBlendFactor(float f0, float f1, float f2, float f3);
	void SetSampleMask(BBUL mask);
	void Active();
private:
	BBUL	m_Idx;
};

//////////////////////////////////////////////////////////////////////////

class BbDevStateDepthStencil
{
public:
	BB_DEV_SMART_REF_DECL(BbDevStateDepthStencil);

	void Create( D3D11_DEPTH_STENCIL_DESC* desc );
	void SetStencilRef(BBUL ref);
	void Active();
private:
	BBUL	m_Idx;
};

//////////////////////////////////////////////////////////////////////////

class BbDevStateRasterizer
{
public:
	BB_DEV_SMART_REF_DECL(BbDevStateRasterizer);

	void Create( D3D11_RASTERIZER_DESC* desc );
	void Active();
private:
	BBUL	m_Idx;
};

//////////////////////////////////////////////////////////////////////////

class BbDevStateSampler
{
public:
	BB_DEV_SMART_REF_DECL(BbDevStateSampler);

	void Create( D3D11_SAMPLER_DESC* desc );
	BBUL Data() const;
private:
	BBUL	m_Idx;
};

//////////////////////////////////////////////////////////////////////////

class BbDevLayoutDesc
{
	typedef std::vector<D3D11_INPUT_ELEMENT_DESC>	EleDescArray;
	typedef std::list<BbString>		StrBuf;
public:
	void Add( const char* name, BbLayoutFmtTypeE fmt );
	D3D11_INPUT_ELEMENT_DESC* Buf();
	BBUL Count();

private:	
	DXGI_FORMAT TurnFmt( BbLayoutFmtTypeE fmt );
	UINT		TurnSize( DXGI_FORMAT fmt );
	UINT		CalcOffset();

public:
	EleDescArray		m_Desc;
	StrBuf				m_StrBuf;
};

//////////////////////////////////////////////////////////////////////////

class BbDevViewPort
{
public:
	BbDevViewPort();
	void Init( BBUL width, BBUL height );
	void Active();

private:
	D3D11_VIEWPORT	m_ViewPort;
};

//////////////////////////////////////////////////////////////////////////

class BbDevToplogy
{
public:
	BbDevToplogy();
	void Init( D3D11_PRIMITIVE_TOPOLOGY topo );
	void Active();

private:
	D3D11_PRIMITIVE_TOPOLOGY	m_Topo;
};