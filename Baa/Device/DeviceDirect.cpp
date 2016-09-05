#include "DeviceDirect.h"
#include "DeviceDataMgr.h"

unsigned long BbDeviceDirect::GetModId()
{
	static const BbStringHash sh("BbDeviceDirect");
	return sh.Value();
}

void BbDeviceDirect::Init()
{
	BeNull();
	m_DataMgr = new BbDeviceDataMgr();
	m_RenderPadItl = new BbDevRenderPadItl();
}

bool BbDeviceDirect::InitDevice( BBUL width, BBUL height, HWND hwnd )
{
	if ( 0<width && 0<height )
	{
		m_Width = width;
		m_Height = height;
		DXGI_SWAP_CHAIN_DESC dscd;
		D3D_FEATURE_LEVEL dflIn = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL dflOut;
		DXGI_ADAPTER_DESC dad;
		IDXGIFactory*	factory = nullptr;
		IDXGIAdapter*	adapter = nullptr;
		IDXGIAdapter*	adapter1 = nullptr;
		SIZE_T dvm = 0;

		if ( S_OK==::CreateDXGIFactory( __uuidof(IDXGIFactory), (void**)(&factory) ) )
		{

			for ( UINT i(0); factory->EnumAdapters( i, &adapter1 )!=DXGI_ERROR_NOT_FOUND; ++i )
			{
				if( S_OK==adapter1->GetDesc( &dad ) )
				{
					if ( dvm<dad.DedicatedVideoMemory )
					{
						dvm = dad.DedicatedVideoMemory;
						BB_SAFE_RELEASE(adapter);
						adapter = adapter1;
					}
					else
					{
						BB_SAFE_RELEASE(adapter1);
					}
				}
				else
				{
					BB_SAFE_RELEASE(adapter1);
				}
			}
			if ( S_OK==::D3D11CreateDevice( adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, 0, &dflIn, 1, D3D11_SDK_VERSION, &m_Device, &dflOut, &m_Context ) )
			{			
				memset( &dscd, 0, sizeof(DXGI_SWAP_CHAIN_DESC) );
				dscd.BufferCount	= 1;
				dscd.BufferDesc.Width	= m_Width;
				dscd.BufferDesc.Height	= m_Height;
				dscd.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;
				dscd.BufferDesc.RefreshRate.Numerator = 0;
				dscd.BufferDesc.RefreshRate.Denominator = 1;
				dscd.BufferDesc.ScanlineOrdering	= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
				dscd.BufferDesc.Scaling				= DXGI_MODE_SCALING_UNSPECIFIED;
				dscd.BufferUsage					= DXGI_USAGE_RENDER_TARGET_OUTPUT;
				dscd.OutputWindow					= hwnd;
				dscd.Windowed						= TRUE;
				dscd.SwapEffect						= DXGI_SWAP_EFFECT_DISCARD;
				dscd.Flags							= 0;
				dscd.SampleDesc.Count				= 1;
				dscd.SampleDesc.Quality				= 0;

				if ( S_OK==factory->CreateSwapChain( m_Device, &dscd, &m_SwapChain ) )
				{
					if ( S_OK==factory->MakeWindowAssociation( hwnd, DXGI_MWA_NO_ALT_ENTER|DXGI_MWA_NO_WINDOW_CHANGES ) )
					{
						m_Init = true;
						InitDefault();
					}
				}
			}
			BB_SAFE_RELEASE(adapter);
			BB_SAFE_RELEASE(factory);
		}
	}
	return m_Init;
}

void BbDeviceDirect::ResetDevice( BBUL width, BBUL height )
{
	width;
	height;
}

void BbDeviceDirect::Destroy()
{
	m_DataMgr->Destroy();
	delete m_DataMgr;
	delete m_RenderPadItl;
	BB_SAFE_RELEASE( m_SwapChain );
	BB_SAFE_RELEASE( m_Context );
	BB_SAFE_RELEASE( m_Device );
	BeNull();
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceDirect::Present()
{
	m_SwapChain->Present( 0, 0 );
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceDirect::ActiveStateBlend( BbDevStateBlendItl* state )
{
	if ( state!=m_CurStateBlend )
	{
		m_CurStateBlend->RefSub();
		state->RefAdd();
		m_CurStateBlend = state;
		m_Context->OMSetBlendState( state->State(), state->BlendFactor(), state->SampleMask() );		
	}
}

void BbDeviceDirect::ActiveStateDepthStencil( BbDevStateDepthStencilItl* state )
{
	if ( state!=m_CurStateDepthStencil )
	{
		m_CurStateDepthStencil->RefSub();
		state->RefAdd();
		m_Context->OMSetDepthStencilState( state->State(), state->StencilRef() );
		m_CurStateDepthStencil = state;
	}
}

void BbDeviceDirect::ActiveStateRasterizer( BbDevStateRasterizerItl* state )
{
	if ( state!=m_CurStateRasterizer )
	{
		m_CurStateRasterizer->RefSub();
		state->RefAdd();
		m_Context->RSSetState( state->State() );
		m_CurStateRasterizer = state;
	}
}

void BbDeviceDirect::ActiveStateSampler( BbDevStateSamplerItl* state, BBUC pipe, BBUC slot )
{
	if ( state!=m_PipeSampler[pipe][slot] )
	{
		ID3D11SamplerState** pdss = nullptr;
		ID3D11SamplerState* dss = nullptr;
		if ( nullptr!=m_PipeSampler[pipe][slot] )
		{
			m_PipeSampler[pipe][slot]->RefSub();
		}
		m_PipeSampler[pipe][slot] = state;
		if ( nullptr!=state )
		{
			state->RefAdd();
			dss = state->State();
			pdss = &dss;
		}
		switch( pipe )
		{
		case BB_PIPE_TYPE_VS:	m_Context->VSSetSamplers( slot, 1, pdss );	break;
		case BB_PIPE_TYPE_HS:	m_Context->HSSetSamplers( slot, 1, pdss );	break;
		case BB_PIPE_TYPE_GS:	m_Context->GSSetSamplers( slot, 1, pdss );	break;
		case BB_PIPE_TYPE_DS:	m_Context->DSSetSamplers( slot, 1, pdss );	break;
		case BB_PIPE_TYPE_PS:	m_Context->PSSetSamplers( slot, 1, pdss );	break;
		default:	break;
		}	
	}
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceDirect::ActiveLayout( ID3D11InputLayout* il )
{
	if ( il!=m_CurLayout )
	{
		m_Context->IASetInputLayout( il );
		m_CurLayout = il;
	}
}

void BbDeviceDirect::ActiveViewPort( D3D11_VIEWPORT* vp )
{
	if ( 0!=memcpy_s( vp, sizeof(D3D11_VIEWPORT), &m_CurViewPort, sizeof(D3D11_VIEWPORT) ) )
	{
		m_CurViewPort = *vp;
		m_Context->RSSetViewports( 1, vp );
	}
}

void BbDeviceDirect::ActiveTopology( D3D11_PRIMITIVE_TOPOLOGY topo )
{
	if ( topo!=m_CurTopology )
	{
		m_CurTopology = topo;
		m_Context->IASetPrimitiveTopology( topo );
	}
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceDirect::ActiveBufVertex( BbDevBufVertexItl* bv, BBUL off )
{
	if ( bv!=m_CurBufVertex )
	{
		if ( nullptr!=m_CurBufVertex )
		{
			m_CurBufVertex->RefSub();
		}
		if ( nullptr==bv )
		{
			m_Context->IASetVertexBuffers( 0, 1, nullptr, nullptr, nullptr );
		}
		else
		{
			bv->RefAdd();
			ID3D11Buffer* b = bv->Buf();
			UINT stride = bv->Stride();
			UINT offff = off;
			m_Context->IASetVertexBuffers( 0, 1, &b, &stride, &offff );
		}
		m_CurBufVertex = bv;
	}
}

void BbDeviceDirect::ActiveBufIndex( BbDevBufIndexItl* bi, BBUL off )
{
	if ( bi!=m_CurBufIndex )
	{
		if ( nullptr!=m_CurBufIndex )
		{
			m_CurBufIndex->RefSub();
		}
		if ( nullptr==bi )
		{
			m_Context->IASetIndexBuffer( nullptr, DXGI_FORMAT_R16_UINT, 0 );
		}
		else
		{
			bi->RefAdd();
			m_Context->IASetIndexBuffer( bi->Buf(), bi->Format(), off );
		}
		m_CurBufIndex = bi;
	}
}

void BbDeviceDirect::ActiveBufConst( ID3D11Buffer* dbc, BBUC pipe, BBUC slot )
{
	if ( dbc!=m_PipeBufConst[pipe][slot] )
	{
		ID3D11Buffer** pbuf = &dbc;
		m_PipeBufConst[pipe][slot] = dbc;
		switch( pipe )
		{
		case BB_PIPE_TYPE_VS:	m_Context->VSSetConstantBuffers( slot, 1, pbuf );	break;
		case BB_PIPE_TYPE_HS:	m_Context->HSSetConstantBuffers( slot, 1, pbuf );	break;
		case BB_PIPE_TYPE_GS:	m_Context->GSSetConstantBuffers( slot, 1, pbuf );	break;
		case BB_PIPE_TYPE_DS:	m_Context->DSSetConstantBuffers( slot, 1, pbuf );	break;
		case BB_PIPE_TYPE_PS:	m_Context->PSSetConstantBuffers( slot, 1, pbuf );	break;
		default:	break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceDirect::ActiveRenderPad( BbDevRenderPad* pad )
{
	if ( 0!=memcpy_s( pad, sizeof(BbDevRenderPad), &m_RenderPad, sizeof(BbDevRenderPad) ) )
	{
		m_RenderPadItl->RefSub();
		m_DataMgr->GetRenderPadItl( m_RenderPadItl, pad );
		m_RenderPadItl->RefAdd();
		BbDevRenderPadItl::PadItlData* data = &m_RenderPadItl->m_Data;
		m_Context->OMSetRenderTargets( data->Count, data->Rtv, data->DepthStencil->Dsv() );
	}
}

void BbDeviceDirect::ActiveSrv( BbDevSrvItl* srv, BBUC pipe, BBUC slot )
{
	if ( srv!=m_PipeSrv[pipe][slot] )
	{
		ID3D11ShaderResourceView* sss = nullptr;
		ID3D11ShaderResourceView** psss = nullptr;
		if ( nullptr!=m_PipeSrv[pipe][slot] )
		{
			m_PipeSrv[pipe][slot]->RefSub();
		}
		m_PipeSrv[pipe][slot] = srv;
		if ( nullptr!=srv )
		{
			srv->RefAdd();
			sss = srv->Srv();
			psss = &sss;
		}
		switch( pipe )
		{
		case BB_PIPE_TYPE_VS:	m_Context->VSSetShaderResources( slot, 1, psss );	break;
		case BB_PIPE_TYPE_HS:	m_Context->HSSetShaderResources( slot, 1, psss );	break;
		case BB_PIPE_TYPE_GS:	m_Context->GSSetShaderResources( slot, 1, psss );	break;
		case BB_PIPE_TYPE_DS:	m_Context->DSSetShaderResources( slot, 1, psss );	break;
		case BB_PIPE_TYPE_PS:	m_Context->PSSetShaderResources( slot, 1, psss );	break;
		default:	break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceDirect::ActiveVertexShader( ID3D11VertexShader* vs )
{
	if ( vs!=m_CurVertexShader )
	{
		m_CurVertexShader = vs;
		m_Context->VSSetShader( vs, nullptr, 0 );
	}
}

void BbDeviceDirect::ActivePixelShader( ID3D11PixelShader* ps )
{
	if ( ps!=m_CurPixelShader )
	{
		m_CurPixelShader = ps;
		m_Context->PSSetShader( ps, nullptr, 0 );
	}
}

void BbDeviceDirect::ActiveHullShader( ID3D11HullShader* hs )
{
	if ( hs!=m_CurHullShader )
	{
		m_CurHullShader = hs;
		m_Context->HSSetShader( hs, nullptr, 0 );
	}
}

void BbDeviceDirect::ActiveDomainShader( ID3D11DomainShader* ds )
{
	if ( ds!=m_CurDomainShader )
	{
		m_CurDomainShader = ds;
		m_Context->DSSetShader( ds, nullptr, 0 );
	}
}

void BbDeviceDirect::ActiveGeometryShader( ID3D11GeometryShader* gs )
{
	if ( gs!=m_CurGeometryShader )
	{
		m_CurGeometryShader = gs;
		m_Context->GSSetShader( gs, nullptr, 0 );
	}
}

//////////////////////////////////////////////////////////////////////////

ID3D11Device* BbDeviceDirect::Device()
{
	return m_Device;
}

ID3D11DeviceContext* BbDeviceDirect::Context()
{
	return m_Context;
}

IDXGISwapChain* BbDeviceDirect::SwapChain()
{
	return m_SwapChain;
}

BbDeviceDataMgr* BbDeviceDirect::DataMgr()
{
	return m_DataMgr;
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceDirect::InitDefault()
{
	m_DataMgr->Init( this );

	//BlendNone
	D3D11_BLEND_DESC bd;
	memset( &bd, 0, sizeof(D3D11_BLEND_DESC) );
	bd.AlphaToCoverageEnable					= FALSE;
	bd.IndependentBlendEnable					= FALSE;
	bd.RenderTarget[0].BlendEnable				= FALSE;
	bd.RenderTarget[0].SrcBlend					= D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlend				= D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha				= D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	BbDevStateBlendItl* blenditl = m_DataMgr->GetStateBlendItl(BBUL_MAX);
	blenditl->Create( &bd );
	ActiveStateBlend( blenditl );


	//DepthLess
	D3D11_DEPTH_STENCIL_DESC dsd;
	dsd.DepthEnable						= TRUE;
	dsd.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc						= D3D11_COMPARISON_LESS;
	dsd.StencilEnable					= FALSE;
	dsd.StencilReadMask					= D3D11_DEFAULT_STENCIL_READ_MASK;
	dsd.StencilWriteMask				= D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsd.FrontFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	dsd.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilDepthFailOp		= D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	BbDevStateDepthStencilItl* sdsitl = m_DataMgr->GetStateDepthStencilItl(BBUL_MAX);
	sdsitl->Create( &dsd );
	ActiveStateDepthStencil( sdsitl );

	//CullBack
	D3D11_RASTERIZER_DESC rsd;
	rsd.FillMode				= D3D11_FILL_SOLID;
	rsd.CullMode				= D3D11_CULL_BACK;
	rsd.FrontCounterClockwise	= FALSE;	// FALSE
	rsd.DepthBias				= 0;
	rsd.SlopeScaledDepthBias	= 0.0;
	rsd.DepthBiasClamp			= 0.0f;
	rsd.DepthClipEnable			= TRUE;
	rsd.ScissorEnable			= FALSE;
	rsd.MultisampleEnable		= FALSE;
	rsd.AntialiasedLineEnable	= FALSE;
	BbDevStateRasterizerItl* rtrzitl = m_DataMgr->GetStateRasterizerItl(BBUL_MAX);
	rtrzitl->Create( &rsd );
	ActiveStateRasterizer( rtrzitl );

	//View Port
	D3D11_VIEWPORT vp;
	vp.Width = static_cast< float >( m_Width );
	vp.Height = static_cast< float >( m_Height );
	vp.MaxDepth = 1;
	vp.MinDepth = 0;	
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	ActiveViewPort( &vp );

	//Render Target
	ID3D11Texture2D* pBackBuffer = NULL;
	m_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	BbDevRenderTargetItl* rtitl = m_DataMgr->GetRenderTargetItl(BBUL_MAX);
	rtitl->Create( pBackBuffer );

	//Depth Stencil
	BbDevTextureDesc dstd;
	dstd.width = m_Width;
	dstd.height = m_Height;
	dstd.fmttex = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dstd.fmtdsv = DXGI_FORMAT_D24_UNORM_S8_UINT;
	BbDevDepthStencilItl* dsitl = m_DataMgr->GetDepthStencilItl(BBUL_MAX);
	dsitl->Create( &dstd );

	BbDevRenderPad pad;
	ActiveRenderPad( &pad );

	ActiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void BbDeviceDirect::BeNull()
{
	BBUL len1 = sizeof(void*) * BB_PIPE_TYPE_COUNT * D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT;	
	memset( m_PipeSampler, 0, len1 );
	memset( m_PipeSrv, 0, len1 );
	memset( m_PipeBufConst, 0, len1 );
	memset( &m_CurViewPort, 0, sizeof(D3D11_VIEWPORT) );
	m_CurTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;

	m_CurStateBlend = nullptr;
	m_CurStateDepthStencil = nullptr;
	m_CurStateRasterizer = nullptr;	
	m_CurDepthStencil = nullptr;	
	m_CurBufVertex = nullptr;
	m_CurBufIndex = nullptr;
	m_CurLayout = nullptr;
	m_CurVertexShader = nullptr;
	m_CurPixelShader = nullptr;
	m_CurHullShader = nullptr;
	m_CurDomainShader = nullptr;
	m_CurGeometryShader = nullptr;

	m_Device = nullptr;
	m_Context = nullptr;
	m_SwapChain = nullptr;

	m_Width = 0;
	m_Height = 0;
	m_Init = false;

	m_DataMgr = nullptr;
}