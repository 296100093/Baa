#include "DevicePipe.h"
#include "DeviceDirect.h"


//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

BbDevTex2D::BbDevTex2D()
	:	m_Texture2D(nullptr)
	,	m_ShaderResourceView(nullptr)
{

}

void BbDevTex2D::Init( void* src, BbLoadFromTypeE type )
{
	Destroy();
	ID3D11Device* device = s_BbDd->Device();
	D3D11_TEXTURE2D_DESC td;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	D3DX11_IMAGE_LOAD_INFO ili;	
	ili.Usage = D3D11_USAGE_IMMUTABLE;
	ili.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	if ( BB_LOAD_FROM_FILE_A==type )
	{
		D3DX11CreateTextureFromFileA( device, (const char*)src, &ili, NULL, (ID3D11Resource**)&m_Texture2D, NULL );
	}
	else if ( BB_LOAD_FROM_RES==type )
	{
		D3DX11CreateTextureFromResourceA( device, GetModuleHandleA(NULL), (const char*)src, &ili, NULL,  (ID3D11Resource**)&m_Texture2D, NULL );
	}
	else if ( BB_LOAD_FROM_MEM==type )
	{
		Lot<char>* ptr = (Lot<char>*)src;
		D3DX11CreateTextureFromMemory( device, ptr->data, ptr->len, &ili, NULL,  (ID3D11Resource**)&m_Texture2D, NULL );
	}
	else if ( BB_LOAD_FROM_ZIP==type )
	{
	}
	else
	{
		m_Texture2D = NULL;
	}
	m_Texture2D->GetDesc( &td );
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = td.MipLevels;
	srvd.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView( m_Texture2D, &srvd, &m_ShaderResourceView );
}

void BbDevTex2D::Destroy()
{
	BbDevDataSrv::Destroy();
	BB_SAFE_RELEASE(m_ShaderResourceView);
	BB_SAFE_RELEASE(m_Texture2D);
}

ID3D11ShaderResourceView* BbDevTex2D::Srv() const
{
	return m_ShaderResourceView;
}


//////////////////////////////////////////////////////////////////////////

BbDevBufConst::BbDevBufConst()
	:	m_Buffer(nullptr)
	,	m_Data( nullptr )
	,	m_Size( 0 )
	,	m_AttachConst(0)
	,	m_Update(false)
	,	m_Immutable(false)
{
	m_SlotConst[BB_PIPE_TYPE_VS] =
	m_SlotConst[BB_PIPE_TYPE_HS] =
	m_SlotConst[BB_PIPE_TYPE_GS] =
	m_SlotConst[BB_PIPE_TYPE_DS] =
	m_SlotConst[BB_PIPE_TYPE_PS] = 0xFF;
}

void BbDevBufConst::Init( ID3D11ShaderReflectionConstantBuffer* buf )
{
	ID3D11Device* device = s_BbDd->Device();
	D3D11_SHADER_BUFFER_DESC sbd;
	buf->GetDesc( &sbd );
	if ( sbd.Size > 0 )
	{
		m_Size = sbd.Size;
		m_Data = ::malloc( sbd.Size );
		::RtlZeroMemory( m_Data, m_Size );
		D3D11_SHADER_VARIABLE_DESC svd;
		for ( UINT n(0); n < sbd.Variables; ++n )
		{
			buf->GetVariableByIndex(n)->GetDesc( &svd );
			m_mVar[svd.Name] = static_cast<UINT>( m_aVar.size() );
			m_aVar.push_back( var(svd.StartOffset, svd.Size) );			
		}
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DYNAMIC;
	bd.ByteWidth			= m_Size;
	bd.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags			= 0;
	bd.StructureByteStride	= 0;
	device->CreateBuffer( &bd, NULL, &m_Buffer );
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
	m_Immutable = false;
	m_aVar.clear();
	m_mVar.clear();
}

void BbDevBufConst::AttachConst( BBUC pipe, BBUC slot )
{
	if ( m_SlotConst[pipe]!=slot )
	{
		if ( 0xFF!=m_SlotConst[pipe] )
		{
			s_BbDd->DeActiveConst( pipe, m_SlotConst[pipe] );
		}		
	}
	m_SlotConst[pipe] = slot;
	++m_AttachConst;
}

void BbDevBufConst::DetachConst( BBUC pipe, BBUC slot )
{
	if ( slot==m_SlotConst[pipe] )
	{
		m_SlotConst[pipe] = 0xFF;
		--m_AttachConst;
	}	
}

bool BbDevBufConst::IstachConst()
{
	return 0<m_AttachConst && s_Run;
}

ID3D11Buffer* BbDevBufConst::Buf()
{
	return m_Buffer;
}

void BbDevBufConst::Commit()
{
	if ( m_Update )
	{
		ID3D11DeviceContext* context = s_BbDd->Context();
		D3D11_MAPPED_SUBRESOURCE mss;
		//::RtlZeroMemory( &mss, sizeof( D3D11_MAPPED_SUBRESOURCE ) );
		memset( &mss, 0, sizeof( D3D11_MAPPED_SUBRESOURCE ) );
		if ( S_OK == context->Map( m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mss ) )
		{
			memcpy_s( mss.pData, m_Size, m_Data, m_Size );
			context->Unmap( m_Buffer, 0 );
		}
		m_Update = false;
	}
}

bool BbDevBufConst::SetValue( const std::string& str, const void* src, UINT size )
{
	if ( !m_Immutable )
	{
		VarMap::iterator iter = m_mVar.find(str);
		if ( m_mVar.end() != iter )
		{
			var* pv = &m_aVar[iter->second];
			byte* dest = (byte*)m_Data+pv->off;
			if( (NULL != src) && (pv->size == size) && (0 != memcmp( dest, src, size )) )
			{
				memcpy_s( dest, size, src, size );
				m_Update = true;
			}
			return true;
		}
	}
	return false;
}

void BbDevBufConst::SetImmutable( const void* src )
{
	if ( NULL != src )
	{
		Destroy();
		ID3D11Device* device = s_BbDd->Device();
		D3D11_BUFFER_DESC bd;
		bd.Usage				= D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth			= m_Size;
		bd.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags		= 0;
		bd.MiscFlags			= 0;
		bd.StructureByteStride	= 0;
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem			= src;
		data.SysMemPitch		= 0;
		data.SysMemSlicePitch	= 0;
		device->CreateBuffer( &bd, &data, &m_Buffer );
		m_Immutable = true;
	}
	else
	{
		SetImmutable();
	}
}

void BbDevBufConst::SetImmutable()
{
	if ( NULL != m_Data && 0 != m_Size )
	{
		BB_SAFE_RELEASE(m_Buffer);
		ID3D11Device* device = s_BbDd->Device();

		D3D11_BUFFER_DESC bd;
		bd.Usage				= D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth			= m_Size;
		bd.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags		= 0;
		bd.MiscFlags			= 0;
		bd.StructureByteStride	= 0;
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem			= m_Data;
		data.SysMemPitch		= 0;
		data.SysMemSlicePitch	= 0;
		device->CreateBuffer( &bd, &data, &m_Buffer );

		::free( m_Data );
		m_Data = nullptr;
		m_Size = 0;
		m_Immutable = true;		
		m_Update = false;		
		m_aVar.clear();
		m_mVar.clear();
	}
}