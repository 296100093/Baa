#include "DeviceBufferInternal.h"
#include "DeviceDataMgr.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BbDevBufVertexItl::BbDevBufVertexItl()
	: m_Buffer(nullptr)
	, m_Stride(0)
	, m_Immutable(false)
{
	SetType(BB_DEV_DATA_BUFFER_VERTEX);
}

void BbDevBufVertexItl::Create( void* data, BBUL stride, BBUL count )
{
	assert( nullptr==m_Buffer );
	assert( nullptr!=data );
	ID3D11Device* device = BbDeviceDataMgr::Get()->Device();
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA srd;
	HRESULT hr;

	m_Stride = stride;
	m_Immutable = true;

	bd.ByteWidth = stride * count;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	srd.pSysMem = data;
	srd.SysMemPitch = 0;
	srd.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&bd, &srd, &m_Buffer);
}

void BbDevBufVertexItl::Create( BBUL stride, BBUL count )
{
	assert( nullptr==m_Buffer );
	ID3D11Device* device = BbDeviceDataMgr::Get()->Device();
	D3D11_BUFFER_DESC bd;
	HRESULT hr;

	m_Stride = stride;
	m_Immutable = false;

	bd.ByteWidth = stride * count;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	hr = device->CreateBuffer(&bd, nullptr, &m_Buffer);
}

void BbDevBufVertexItl::Destroy()
{	
	BB_SAFE_RELEASE(m_Buffer);
	m_Stride = 0;
	m_Immutable = false;
}

void BbDevBufVertexItl::UpdateByNum( void* data, BBUL left, BBUL width )
{
	UpdateByByte( data, left * m_Stride, width * m_Stride );
}

void BbDevBufVertexItl::UpdateByByte( void* data, BBUL left, BBUL width )
{
	if ( m_Immutable )	return;

	ID3D11DeviceContext* context = BbDeviceDataMgr::Get()->Context();
	D3D11_BOX destRegion;
	destRegion.left = left;
	destRegion.right = destRegion.left + width;
	destRegion.top = 0;
	destRegion.bottom = 1;
	destRegion.front = 0;
	destRegion.back = 1;
	context->UpdateSubresource( m_Buffer, 0, &destRegion, data, 0, 0 );
}

ID3D11Buffer* BbDevBufVertexItl::Buf()
{
	return m_Buffer;
}

UINT BbDevBufVertexItl::Stride()
{
	return m_Stride;
}

//////////////////////////////////////////////////////////////////////////

BbDevBufIndexItl::BbDevBufIndexItl()
	: m_Buffer(nullptr)
	, m_Format(DXGI_FORMAT_UNKNOWN)
	, m_Stride(0)
	, m_Immutable(false)
{
	SetType( BB_DEV_DATA_BUFFER_INDEX );
}

void BbDevBufIndexItl::Create16( void* data, BBUL count )
{
	Create( data, count, DXGI_FORMAT_R16_UINT );
}

void BbDevBufIndexItl::Create32( void* data, BBUL count )
{
	Create( data, count, DXGI_FORMAT_R16_UINT );
}

void BbDevBufIndexItl::Create16( BBUL count )
{
	Create( nullptr, count, DXGI_FORMAT_R16_UINT );
}

void BbDevBufIndexItl::Create32( BBUL count )
{
	Create( nullptr, count, DXGI_FORMAT_R32_UINT );
}

void BbDevBufIndexItl::Destroy()
{
	BB_SAFE_RELEASE(m_Buffer);
	m_Format = DXGI_FORMAT_UNKNOWN;
	m_Stride = 0;
	m_Immutable = false;
}

ID3D11Buffer* BbDevBufIndexItl::Buf()
{
	return m_Buffer;
}

DXGI_FORMAT BbDevBufIndexItl::Format()
{
	return m_Format;
}

void BbDevBufIndexItl::Create( void* data, BBUL count, DXGI_FORMAT fmt )
{
	assert(nullptr==m_Buffer);
	ID3D11Device* device = BbDeviceDataMgr::Get()->Device();
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA srd;
	HRESULT hr;

	m_Format = fmt;
	m_Stride = fmt==DXGI_FORMAT_R16_UINT ? 2 : 4;
	m_Immutable = nullptr!=data;

	bd.ByteWidth = count * m_Stride;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = m_Immutable ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	srd.pSysMem = data;
	srd.SysMemPitch = 0;
	srd.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&bd, m_Immutable?&srd:nullptr, &m_Buffer);
}

void BbDevBufIndexItl::UpdateByNum( void* data, BBUL left, BBUL width )
{
	UpdateByByte( data, left * m_Stride, width * m_Stride );
}

void BbDevBufIndexItl::UpdateByByte( void* data, BBUL left, BBUL width )
{
	if ( m_Immutable )	return;

	ID3D11DeviceContext* context = BbDeviceDataMgr::Get()->Context();
	D3D11_BOX destRegion;
	destRegion.left = left;
	destRegion.right = destRegion.left + width;
	destRegion.top = 0;
	destRegion.bottom = 1;
	destRegion.front = 0;
	destRegion.back = 1;
	context->UpdateSubresource( m_Buffer, 0, &destRegion, data, 0, 0 );
}

//////////////////////////////////////////////////////////////////////////