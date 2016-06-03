#pragma once

#include "DeviceData.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BbDevBufVertex : public BbDevData
{
public:
	static BbDevBufVertex* Create(BbDevBufVertexDesc* desc);

public:
	BbDevBufVertex(ID3D11Buffer* buf, UINT stride, bool imm);
	
	ID3D11Buffer*	Buf();
	UINT			Stride();
private:
	ID3D11Buffer*	m_Buffer;
	UINT				m_Stride;
	bool					m_Immutable;
};

//////////////////////////////////////////////////////////////////////////

class BbDevBufIndex : public BbDevData
{
public:
	BbDevBufIndex();

	void Init( BbDevBufIndexDesc* bid );
	void Destroy();
	void Active( BBUI off );
	BbDevDataTypeE Type() const	{ return BB_DEV_DAT_BUFFER_INDEX; }

	ID3D11Buffer*	Buf();
	DXGI_FORMAT		Format();

private:
	ID3D11Buffer*	m_Buffer;
	DXGI_FORMAT		m_Format;
	bool			m_Immutable;
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

class BbDevLayout : public BbDevData
{
public:
	BbDevLayout();

	void Init( ID3D11InputLayout* il );
	void Destroy();
	void Active();

	ID3D11InputLayout*	Layout();

private:
	ID3D11InputLayout*	m_Layout;
};