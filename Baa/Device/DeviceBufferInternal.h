#pragma once

#include "DeviceDataInternal.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BbDevBufVertexItl : public BbDevDataItl
{
public:
	BbDevBufVertexItl();

	void	Create( void* data, BBUL stride, BBUL count );
	void	Create( BBUL stride, BBUL count );
	void	Destroy();
	
	void	UpdateByNum( void* data, BBUL left, BBUL width );
	void	UpdateByByte( void* data, BBUL left, BBUL width );
	ID3D11Buffer*	Buf();
	UINT			Stride();
private:
	ID3D11Buffer*	m_Buffer;
	BBUL			m_Stride;
	bool			m_Immutable;
};

//////////////////////////////////////////////////////////////////////////

class BbDevBufIndexItl : public BbDevDataItl
{
public:
	BbDevBufIndexItl();

	void Create16( void* data, BBUL count );
	void Create32( void* data, BBUL count );
	void Create16( BBUL count );
	void Create32( BBUL count );
	void Destroy();

	void	UpdateByNum( void* data, BBUL left, BBUL width );
	void	UpdateByByte( void* data, BBUL left, BBUL width );
	ID3D11Buffer*	Buf();
	DXGI_FORMAT		Format();

private:
	void Create( void* data, BBUL count, DXGI_FORMAT fmt );

private:
	ID3D11Buffer*	m_Buffer;
	DXGI_FORMAT		m_Format;
	BBUL			m_Stride;
	bool			m_Immutable;
};

//////////////////////////////////////////////////////////////////////////
