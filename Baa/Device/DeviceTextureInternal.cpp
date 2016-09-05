#include "DeviceTextureInternal.h"
#include "DeviceDataMgr.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BbDevTex1DItl::BbDevTex1DItl()
	: m_Texture1D(nullptr)
{
	SetType(BB_DEV_DATA_TEXTURE_1D);
}

void BbDevTex1DItl::Create(const wchar_t* str)
{
	m_Name = str;
	assert(nullptr==m_Texture1D);
	ID3D11Device* device = BbDeviceDataMgr::Get()->Device();
	D3D11_TEXTURE1D_DESC td;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	D3DX11_IMAGE_LOAD_INFO ili;	
	ili.Usage = D3D11_USAGE_IMMUTABLE;
	ili.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3DX11CreateTextureFromFileW( device, str, &ili, NULL, (ID3D11Resource**)&m_Texture1D, NULL );

	m_Texture1D->GetDesc( &td );
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	srvd.Texture2D.MipLevels = td.MipLevels;
	srvd.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView( m_Texture1D, &srvd, &m_ShaderResourceView );
}

void BbDevTex1DItl::Create(const BbDevTextureDesc* desc)
{
	assert(nullptr==m_Texture1D);
	ID3D11Device* device = BbDeviceDataMgr::Get()->Device();
	D3D11_TEXTURE1D_DESC td;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	memset(&td, 0, sizeof(D3D11_TEXTURE1D_DESC));
	td.Width = desc->width;
	td.MipLevels = desc->miplevel;
	td.ArraySize = 1;
	td.Format = desc->fmttex;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	device->CreateTexture1D(&td, NULL, &m_Texture1D);

	srvd.Format = desc->fmtsrv;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	srvd.Texture2D.MipLevels = td.MipLevels;
	srvd.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView(m_Texture1D, &srvd, &m_ShaderResourceView);
}

void BbDevTex1DItl::Destroy()
{
	BB_SAFE_RELEASE( m_ShaderResourceView );
	BB_SAFE_RELEASE( m_Texture1D );
}

bool BbDevTex1DItl::Equal( BbWstring* str )
{
	bool a = false;
	if ( 0<m_Name.length() && 0<str->length() )
	{
		a = m_Name == *str;
	}
	return a;
}

ID3D11Texture1D* BbDevTex1DItl::Tex()
{
	return m_Texture1D;
}

//////////////////////////////////////////////////////////////////////////

BbDevTex2DItl::BbDevTex2DItl()
	: m_Texture2D(nullptr)
{
	SetType(BB_DEV_DATA_TEXTURE_2D);
}

void BbDevTex2DItl::Create(const wchar_t* str)
{
	m_Name = str;
	assert(nullptr==m_Texture2D);
	ID3D11Device* device = BbDeviceDataMgr::Get()->Device();
	D3D11_TEXTURE2D_DESC td;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	D3DX11_IMAGE_LOAD_INFO ili;	
	ili.Usage = D3D11_USAGE_IMMUTABLE;
	ili.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3DX11CreateTextureFromFileW( device, str, &ili, NULL, (ID3D11Resource**)&m_Texture2D, NULL );

	m_Texture2D->GetDesc( &td );
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = td.MipLevels;
	srvd.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView( m_Texture2D, &srvd, &m_ShaderResourceView );
}

void BbDevTex2DItl::Create(const BbDevTextureDesc* desc)
{
	assert(nullptr==m_Texture2D);
	ID3D11Device* device = BbDeviceDataMgr::Get()->Device();
	D3D11_TEXTURE2D_DESC td;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;

	memset(&td, 0, sizeof(D3D11_TEXTURE2D_DESC));
	td.Width = desc->width;
	td.Height = desc->height;
	td.MipLevels = desc->miplevel;
	td.ArraySize = 1;
	td.Format = desc->fmttex;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	device->CreateTexture2D(&td, NULL, &m_Texture2D);

	srvd.Format = desc->fmtsrv;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = td.MipLevels;
	srvd.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView(m_Texture2D, &srvd, &m_ShaderResourceView);
}

void BbDevTex2DItl::Destroy()
{
	BB_SAFE_RELEASE( m_ShaderResourceView );
	BB_SAFE_RELEASE( m_Texture2D );
}

bool BbDevTex2DItl::Equal( BbWstring* str )
{
	bool a = false;
	if ( 0<m_Name.length() && 0<str->length() )
	{
		a = m_Name == *str;
	}
	return a;
}

ID3D11Texture2D* BbDevTex2DItl::Tex()
{
	return m_Texture2D;
}

//////////////////////////////////////////////////////////////////////////

BbDevTex3DItl::BbDevTex3DItl()
	: m_Texture3D(nullptr)
{
	SetType(BB_DEV_DATA_TEXTURE_3D);
}

void BbDevTex3DItl::Create(const wchar_t* str)
{
	str;
// 	assert(nullptr==m_Texture3D);
// 	ID3D11Device* device = BbDeviceDataMgr::Get()->Device()->Device();
// 	D3D11_TEXTURE3D_DESC td;
// 	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
// 	D3DX11_IMAGE_LOAD_INFO ili;	
// 	ili.Usage = D3D11_USAGE_IMMUTABLE;
// 	ili.BindFlags = D3D11_BIND_SHADER_RESOURCE;
// 
// 	D3DX11CreateTextureFromFileW( device, str, &ili, NULL, (ID3D11Resource**)&m_Texture3D, NULL );
// 
// 	m_Texture3D->GetDesc( &td );
// 	srvd.Format = td.Format;
// 	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
// 	srvd.Texture2D.MipLevels = td.MipLevels;
// 	srvd.Texture2D.MostDetailedMip = 0;
// 	device->CreateShaderResourceView( m_Texture3D, &srvd, &m_ShaderResourceView );
}

void BbDevTex3DItl::Destroy()
{
	BB_SAFE_RELEASE( m_ShaderResourceView );
	BB_SAFE_RELEASE( m_Texture3D );
}

ID3D11Texture3D* BbDevTex3DItl::Tex()
{
	return m_Texture3D;
}

//////////////////////////////////////////////////////////////////////////

BbDevRenderTargetItl::BbDevRenderTargetItl()
	: m_Texture2D(nullptr)
	, m_RenderTarget(nullptr)
{
	SetType( BB_DEV_DATA_RENDER_TARGET );
}

void BbDevRenderTargetItl::Create(const BbDevTextureDesc* rtd)
{
	assert(nullptr==m_Texture2D);
	ID3D11Device* device = BbDeviceDataMgr::Get()->Device();
	D3D11_TEXTURE2D_DESC td;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	td.Width						= rtd->width;
	td.Height						= rtd->height;
	td.MipLevels					= rtd->miplevel;
	td.ArraySize					= 1;
	td.Format						= rtd->fmttex;
	td.SampleDesc.Count				= 1;
	td.SampleDesc.Quality			= 0;
	td.Usage						= D3D11_USAGE_DEFAULT;
	td.BindFlags					= D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	td.CPUAccessFlags				= 0;
	td.MiscFlags					= 0;

	srvd.Format						= rtd->fmtsrv;
	srvd.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels		= rtd->miplevel;
	srvd.Texture2D.MostDetailedMip	= 0;

	rtvd.Format					= rtd->fmtrtv;
	rtvd.ViewDimension			= D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvd.Texture2D.MipSlice		= 0;

	device->CreateTexture2D( &td, NULL, &m_Texture2D );
	device->CreateShaderResourceView( m_Texture2D, &srvd, &m_ShaderResourceView );
	device->CreateRenderTargetView( m_Texture2D, &rtvd, &m_RenderTarget );
}

void BbDevRenderTargetItl::Create( ID3D11Texture2D* t2d )
{
	assert(nullptr==m_Texture2D);
	ID3D11Device* device = BbDeviceDataMgr::Get()->Device();
	D3D11_TEXTURE2D_DESC td;
	t2d->GetDesc(&td);

	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	rtvd.Format = td.Format;
	rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvd.Texture2D.MipSlice = 0;

	m_Texture2D = t2d;
	device->CreateRenderTargetView( m_Texture2D, &rtvd, &m_RenderTarget );
}

void BbDevRenderTargetItl::Destroy()
{
	BB_SAFE_RELEASE(m_RenderTarget);
	BB_SAFE_RELEASE(m_ShaderResourceView);
	BB_SAFE_RELEASE(m_Texture2D);
}

void BbDevRenderTargetItl::Clear( const BbColor& color )
{
	if ( nullptr!=m_RenderTarget )
	{
		ID3D11DeviceContext* context = BbDeviceDataMgr::Get()->Context();
		context->ClearRenderTargetView( m_RenderTarget, color.Data() );
	}
}

ID3D11Texture2D* BbDevRenderTargetItl::Tex()
{
	return m_Texture2D;
}

ID3D11RenderTargetView* BbDevRenderTargetItl::Rtv()
{
	return m_RenderTarget;
}

//////////////////////////////////////////////////////////////////////////

BbDevDepthStencilItl::BbDevDepthStencilItl()
	:	m_Texture2D(nullptr)
	,	m_DepthStencilView(nullptr)
{
	SetType( BB_DEV_DATA_DEPTH_STENCIL );
}

void BbDevDepthStencilItl::Create( const BbDevTextureDesc* dsd )
{
	assert(nullptr==m_Texture2D);
	ID3D11Device* device = BbDeviceDataMgr::Get()->Device();
	D3D11_TEXTURE2D_DESC td;
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	td.Width				= dsd->width;
	td.Height				= dsd->height;
	td.MipLevels			= 1;
	td.ArraySize			= 1;
	td.Format				= dsd->fmttex;
	td.SampleDesc.Count		= 1;
	td.SampleDesc.Quality	= 0;
	td.Usage				= D3D11_USAGE_DEFAULT;
	td.BindFlags			= DXGI_FORMAT_UNKNOWN==dsd->fmtsrv?D3D11_BIND_DEPTH_STENCIL:D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags		= 0;
	td.MiscFlags			= 0;

	dsvd.Format				= dsd->fmtdsv;
	dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags				= 0;
	dsvd.Texture2D.MipSlice	= 0;

	device->CreateTexture2D( &td, NULL, &m_Texture2D );	
	device->CreateDepthStencilView( m_Texture2D, &dsvd, &m_DepthStencilView );
	//device->CreateDepthStencilView( m_Texture2D, nullptr, &m_DepthStencilView );

	if ( DXGI_FORMAT_UNKNOWN!=dsd->fmtsrv )
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		srvd.Format						= dsd->fmtsrv;
		srvd.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels		= 1;
		srvd.Texture2D.MostDetailedMip	= 0;
		device->CreateShaderResourceView( m_Texture2D, &srvd, &m_ShaderResourceView );
	}
}

void BbDevDepthStencilItl::Destroy()
{
	BB_SAFE_RELEASE(m_DepthStencilView);
	BB_SAFE_RELEASE(m_ShaderResourceView);
	BB_SAFE_RELEASE(m_Texture2D);
}

void BbDevDepthStencilItl::Clear( float depth, BBUC stencil, bool dpt, bool stl )
{
	if ( nullptr!=m_DepthStencilView )
	{
		ID3D11DeviceContext* context = BbDeviceDataMgr::Get()->Context();
		UINT flag = 0;
		if ( dpt )	flag |= D3D11_CLEAR_DEPTH;
		if ( stl )	flag |= D3D11_CLEAR_STENCIL;
		context->ClearDepthStencilView( m_DepthStencilView, flag, depth, stencil );
	}
}

ID3D11Texture2D* BbDevDepthStencilItl::Tex()
{
	return m_Texture2D;
}

ID3D11DepthStencilView* BbDevDepthStencilItl::Dsv()
{
	return m_DepthStencilView;
}

//////////////////////////////////////////////////////////////////////////

BbDevRenderPadItl::BbDevRenderPadItl()
{
	BeNull();
}

void BbDevRenderPadItl::RefAdd()
{
	for ( BBUS i(0); i<m_Data.Count; ++i )
	{
		m_Data.RenderTargetItl[i]->RefAdd();
	}
	m_Data.DepthStencil->RefAdd();
}

void BbDevRenderPadItl::RefSub()
{
	for ( BBUS i(0); i<m_Data.Count; ++i )
	{
		m_Data.RenderTargetItl[i]->RefSub();
	}
	m_Data.DepthStencil->RefSub();	
}

void BbDevRenderPadItl::BeNull()
{
	memset( &m_Data, 0, sizeof(PadItlData) );
}
