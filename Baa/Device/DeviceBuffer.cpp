#include "DeviceBuffer.h"
#include "DeviceBufferInternal.h"
#include "DeviceDataMgr.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BB_DEV_SMART_REF_IMPL(BbDevBufVertex,GetBufVertexItl);

void BbDevBufVertex::Create( void* data, BBUL stride, BBUL count )
{
	assert( BBUL_MAX==m_Idx );
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	m_Idx = mgr->CreateBufVertex();
	mgr->GetBufVertexItl( m_Idx )->Create( data, stride, count );
}

void BbDevBufVertex::Create( BBUL stride, BBUL count )
{
	assert( BBUL_MAX==m_Idx );
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	m_Idx = mgr->CreateBufVertex();
	mgr->GetBufVertexItl( m_Idx )->Create( stride, count );
}

void BbDevBufVertex::Update( void* data, BBUL left, BBUL width )
{
	assert( BBUL_MAX!=m_Idx );
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	mgr->GetBufVertexItl( m_Idx )->UpdateByNum( data, left, width );
}

//////////////////////////////////////////////////////////////////////////

BB_DEV_SMART_REF_IMPL(BbDevBufIndex,GetBufIndexItl);

void BbDevBufIndex::Create16( void* data, BBUL count )
{
	assert( BBUL_MAX==m_Idx );
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	m_Idx = mgr->CreateBufIndex();
	mgr->GetBufIndexItl(m_Idx)->Create16( data, count );
}

void BbDevBufIndex::Create32( void* data, BBUL count )
{
	assert( BBUL_MAX==m_Idx );
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	m_Idx = mgr->CreateBufIndex();
	mgr->GetBufIndexItl(m_Idx)->Create32( data, count );
}

void BbDevBufIndex::Create16( BBUL count )
{
	assert( BBUL_MAX==m_Idx );
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	m_Idx = mgr->CreateBufIndex();
	mgr->GetBufIndexItl(m_Idx)->Create16( count );
}

void BbDevBufIndex::Create32( BBUL count )
{
	assert( BBUL_MAX==m_Idx );
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	m_Idx = mgr->CreateBufIndex();
	mgr->GetBufIndexItl(m_Idx)->Create32( count );
}

void BbDevBufIndex::Update( void* data, BBUL left, BBUL width )
{
	assert( BBUL_MAX!=m_Idx );
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	mgr->GetBufIndexItl( m_Idx )->UpdateByNum( data, left, width );
}

//////////////////////////////////////////////////////////////////////////

BbDevBufConst::BbDevBufConst()
	:	m_Buffer(nullptr)
	,	m_Data( nullptr )
	,	m_Size( 0 )
	,	m_Update(false)
{
}

void BbDevBufConst::Create( BBUL size )
{
	ID3D11Device* device = BbDeviceDataMgr::Get()->Device();
	if ( 0<size )
	{
		m_Size = size;
		m_Data = ::malloc( m_Size );
		memset( m_Data, 0, m_Size );

		D3D11_BUFFER_DESC bd;	
		bd.ByteWidth			= m_Size;
		bd.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		bd.Usage				= D3D11_USAGE_DYNAMIC;
		bd.MiscFlags			= 0;
		bd.StructureByteStride	= 0;
		device->CreateBuffer( &bd, NULL, &m_Buffer );
	}
}

void BbDevBufConst::Destroy()
{
	BB_SAFE_RELEASE(m_Buffer);
	if ( nullptr != m_Data )
	{
		::free( m_Data );
		m_Data = nullptr;
	}
	m_Size = 0;
	m_Update = false;
	m_aBcstVar.clear();
}

ID3D11Buffer* BbDevBufConst::Buf()
{
	return m_Buffer;
}

void BbDevBufConst::Commit()
{
	if ( m_Update )
	{
		ID3D11DeviceContext* context = BbDeviceDataMgr::Get()->Context();
		D3D11_MAPPED_SUBRESOURCE mss;
		memset( &mss, 0, sizeof( D3D11_MAPPED_SUBRESOURCE ) );
		if ( S_OK == context->Map( m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mss ) )
		{
			memcpy_s( mss.pData, m_Size, m_Data, m_Size );
			context->Unmap( m_Buffer, 0 );
		}
		m_Update = false;
	}
}

void BbDevBufConst::SetValue( const void* src, BBUL size, BBUL off )
{
	byte* dest = (byte*)m_Data + off;
	if(0 != memcmp( dest, src, size ))
	{
		memcpy_s( dest, size, src, size );
		m_Update = true;
	}
}