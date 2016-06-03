#include "DeviceOutputMerger.h"
#include "DeviceDirect.h"


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

BbDevRenderTarget::BbDevRenderTarget()
	:	m_Texture2D(nullptr)
	,	m_ShaderResourceView(nullptr)
	,	m_RenderTarget(nullptr)
{

}

void BbDevRenderTarget::Init( BbDevRenderTargetDesc* rtd )
{
	Destroy();
	ID3D11Device* device = s_BbDd->Device();
	D3D11_TEXTURE2D_DESC td;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	td.Width						= static_cast<UINT>( rtd->width );
	td.Height						= static_cast<UINT>( rtd->height );
	td.MipLevels					= rtd->miplevel;
	td.ArraySize					= 1;
	td.Format						= rtd->t2dfmt;
	td.SampleDesc.Count				= 1;
	td.SampleDesc.Quality			= 0;
	td.Usage						= D3D11_USAGE_DEFAULT;
	td.BindFlags					= D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	td.CPUAccessFlags				= 0;
	td.MiscFlags					= 0;

	srvd.Format						= rtd->srvfmt;
	srvd.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels		= rtd->miplevel;
	srvd.Texture2D.MostDetailedMip	= 0;

	rtvd.Format					= rtd->srvfmt;
	rtvd.ViewDimension			= D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvd.Texture2D.MipSlice		= 0;

	device->CreateTexture2D( &td, NULL, &m_Texture2D );
	device->CreateShaderResourceView( m_Texture2D, &srvd, &m_ShaderResourceView );
	device->CreateRenderTargetView( m_Texture2D, &rtvd, &m_RenderTarget );
}

void BbDevRenderTarget::Init( ID3D11Texture2D* t2d )
{
	if ( nullptr==t2d )	return;
	Destroy();
	ID3D11Device* device = s_BbDd->Device();
	D3D11_TEXTURE2D_DESC td;
	t2d->GetDesc(&td);

	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	rtvd.Format = td.Format;
	rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvd.Texture2D.MipSlice = 0;

	m_Texture2D = t2d;
	device->CreateRenderTargetView( m_Texture2D, &rtvd, &m_RenderTarget );
}

void BbDevRenderTarget::Destroy()
{
	BbDevDataSrv::Destroy();
	BB_SAFE_RELEASE(m_RenderTarget);
	BB_SAFE_RELEASE(m_ShaderResourceView);
	BB_SAFE_RELEASE(m_Texture2D);
}

void BbDevRenderTarget::Active()
{
}

ID3D11ShaderResourceView* BbDevRenderTarget::Srv() const
{
	return m_ShaderResourceView;
}

ID3D11RenderTargetView* BbDevRenderTarget::Rtv() const
{
	return m_RenderTarget;
}

//////////////////////////////////////////////////////////////////////////

BbDevDepthStencil::BbDevDepthStencil()
	:	m_Texture2D(nullptr)
	,	m_ShaderResourceView(nullptr)
	,	m_DepthStencilView(nullptr)
{

}

void BbDevDepthStencil::Init( BbDevDepthStencilDesc* dsd )
{
	Destroy();
	ID3D11Device* device = s_BbDd->Device();
	D3D11_TEXTURE2D_DESC td;
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	td.Width				= static_cast<UINT>( dsd->width );
	td.Height				= static_cast<UINT>( dsd->height );
	td.MipLevels			= 1;
	td.ArraySize			= 1;
	td.Format				= dsd->t2dfmt;
	td.SampleDesc.Count		= 1;
	td.SampleDesc.Quality	= 0;
	td.Usage				= D3D11_USAGE_DEFAULT;
	td.BindFlags			= DXGI_FORMAT_UNKNOWN==dsd->srvfmt?D3D11_BIND_DEPTH_STENCIL:D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags		= 0;
	td.MiscFlags			= 0;

	dsvd.Format				= dsd->dsvfmt;
	dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags				= 0;
	dsvd.Texture2D.MipSlice	= 0;

	device->CreateTexture2D( &td, NULL, &m_Texture2D );	
	//device->CreateDepthStencilView( m_Texture2D, &dsvd, &m_DepthStencilView );
	device->CreateDepthStencilView( m_Texture2D, nullptr, &m_DepthStencilView );

	if ( DXGI_FORMAT_UNKNOWN!=dsd->srvfmt )
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		srvd.Format						= dsd->srvfmt;
		srvd.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels		= 1;
		srvd.Texture2D.MostDetailedMip	= 0;
		device->CreateShaderResourceView( m_Texture2D, &srvd, &m_ShaderResourceView );
	}
}

void BbDevDepthStencil::Destroy()
{
	if ( Istach() )
	{
		s_BbDd->ActiveDepthStencil( nullptr );
	}
	BB_SAFE_RELEASE(m_DepthStencilView);
	BB_SAFE_RELEASE(m_ShaderResourceView);
	BB_SAFE_RELEASE(m_Texture2D);
}

void BbDevDepthStencil::Active()
{
	s_BbDd->ActiveDepthStencil( nullptr==m_DepthStencilView?nullptr:this );
}

ID3D11ShaderResourceView* BbDevDepthStencil::Srv() const
{
	return m_ShaderResourceView;
}

ID3D11DepthStencilView* BbDevDepthStencil::Dsv() const
{
	return m_DepthStencilView;
}
