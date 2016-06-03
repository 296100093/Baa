#include "DeviceDirect.h"

unsigned long BbDeviceDirect::GetModId()
{
	static const BbStringHash sh("BbDeviceDirect");
	return sh.Value();
}

void BbDeviceDirect::Init()
{
	m_CurStateBlend = nullptr;
	m_CurStateDepthStencil = nullptr;
	m_CurStateRasterizer = nullptr;
	m_CurViewPort = nullptr;
	m_CurDepthStencil = nullptr;
	m_CurLayout = nullptr;
	m_CurBufVertex = nullptr;
	m_CurBufIndex = nullptr;

	m_Device = nullptr;
	m_Context = nullptr;
	m_SwapChain = nullptr;

	m_Width = 0;
	m_Height = 0;
	m_Init = false;
}

bool BbDeviceDirect::Init( BBS width, BBS height, HWND hwnd )
{
	if ( 0<width && 0<height )
	{
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

void BbDeviceDirect::Destroy()
{
	DestroyDefault();
	BB_SAFE_RELEASE( m_SwapChain );
	BB_SAFE_RELEASE( m_Context );
	BB_SAFE_RELEASE( m_Device );
	m_Init = false;
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceDirect::Present()
{
	m_SwapChain->Present( 0, 0 );
}

void BbDeviceDirect::ClearRtv( const BbDevRenderTarget* rt/* =nullptr */, const BbColor color/* =BbColor(0.0f,0.0f,0.0f,0.0f) */ )
{
	if ( nullptr==rt )
	{
		rt = m_CurRenderTarget[0];
	}
	m_Context->ClearRenderTargetView( rt->Rtv(), color.Data() );
}

void BbDeviceDirect::ClearDsv( const BbDevDepthStencil* ds/* =nullptr */, UINT flag/* =D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL */, float depth/* =1.0f */, UINT8 stencil/* =0 */ )
{
	if ( nullptr==ds )
	{
		ds = m_CurDepthStencil;
	}
	m_Context->ClearDepthStencilView( ds->Dsv(), flag, depth, stencil );
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceDirect::ActiveStateBlend( BbDevStateBlend* state )
{
	if ( nullptr==state )
	{
		state = &m_DefaultStateBlend;
	}
	if ( state!=m_CurStateBlend )
	{
		m_CurStateBlend->Detach();
		state->Attach();
		m_Context->OMSetBlendState( state->State(), state->BlendFactor(), state->SampleMask() );
		m_CurStateBlend = state;
	}
}

void BbDeviceDirect::ActiveStateDepthStencil( BbDevStateDepthStencil* state )
{
	if ( nullptr==state )
	{
		state = &m_DefaultStateDepthStencil;
	}
	if ( state!=m_CurStateDepthStencil )
	{
		m_CurStateDepthStencil->Detach();
		state->Attach();
		m_Context->OMSetDepthStencilState( state->State(), state->Ref() );
		m_CurStateDepthStencil = state;
	}
}

void BbDeviceDirect::ActiveStateRasterizer( BbDevStateRasterizer* state )
{
	if ( nullptr==state )
	{
		state = &m_DefaultStateRasterizer;
	}
	if ( state!=m_CurStateRasterizer )
	{
		m_CurStateRasterizer->Detach();
		state->Attach();
		m_Context->RSSetState( state->State() );
		m_CurStateRasterizer = state;
	}
}

void BbDeviceDirect::ActiveViewPort( BbDevViewPort* vp )
{
	if ( nullptr==vp )
	{
		vp = &m_DefaultViewPort;
	}
	if ( vp!=m_CurViewPort )
	{
		m_CurViewPort->Detach();
		vp->Attach();
		m_Context->RSSetViewports( 1, vp->Vp() );
		m_CurViewPort = vp;
	}
}

void BbDeviceDirect::ActiveDepthStencil( BbDevDepthStencil* ds )
{
	if ( nullptr==ds )
	{
		ds = &m_DefaultDepthStencil;
	}
	if ( ds!=m_CurDepthStencil )
	{
		m_CurDepthStencil->Detach();
		ds->Attach();
		BBUL count = 0;
		ID3D11RenderTargetView*	rtv[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];		
		memset( rtv, 0, sizeof(ID3D11RenderTargetView*)*D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT );
		while ( count<D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT && nullptr!=m_CurRenderTarget[count] )
		{
			rtv[count] = m_CurRenderTarget[count]->Rtv();
			++count;
		}
		m_Context->OMSetRenderTargets( count, rtv, ds->Dsv() );
		m_CurDepthStencil = ds;
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

void BbDeviceDirect::ActiveLayout( BbDevLayout* ly )
{
	if ( ly!=m_CurLayout )
	{
		if ( nullptr!=m_CurLayout )
		{
			m_CurLayout->Detach();
		}
		if ( nullptr==ly )
		{
			m_Context->IASetInputLayout( nullptr );
		}
		else
		{
			ly->Attach();
			m_Context->IASetInputLayout( ly->Layout() );
		}
		m_CurLayout = ly;
	}
}

void BbDeviceDirect::ActiveBufVertex( BbDevBufVertex* bv, UINT off )
{
	if ( bv!=m_CurBufVertex )
	{
		if ( nullptr!=m_CurBufVertex )
		{
			m_CurBufVertex->Detach();
		}
		if ( nullptr==bv )
		{
			m_Context->IASetVertexBuffers( 0, 1, nullptr, nullptr, nullptr );
		}
		else
		{
			bv->Attach();
			ID3D11Buffer* b = bv->Buf();
			UINT stride = bv->Stride();
			m_Context->IASetVertexBuffers( 0, 1, &b, &stride, &off );
		}
		m_CurBufVertex = bv;
	}
}

void BbDeviceDirect::ActiveBufIndex( BbDevBufIndex* bi, UINT off )
{
	if ( bi!=m_CurBufIndex )
	{
		if ( nullptr!=m_CurBufIndex )
		{
			m_CurBufIndex->Detach();
		}
		if ( nullptr==bi )
		{
			m_Context->IASetIndexBuffer( nullptr, DXGI_FORMAT_R16_UINT, 0 );
		}
		else
		{
			bi->Attach();			
			m_Context->IASetIndexBuffer( bi->Buf(), bi->Format(), off );
		}
		m_CurBufIndex = bi;
	}
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceDirect::ActiveRenderTarget( BbDevRenderTarget** rt, BBUC count )
{
	bool rtsame = true;
	BbDevRenderTarget* art[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ID3D11RenderTargetView*	rtv[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	memset( art, 0 , sizeof(BbDevRenderTarget*)*D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT );
	memset( rtv, 0, sizeof(ID3D11RenderTargetView*)*D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT );

	if ( nullptr==rt || 0==count )
	{
		art[0] = &m_DefaultRenderTarget;
		count = 1;
	}
	else
	{
		if ( D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT<count )
		{
			count = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;
		}
		for ( BBUL i(0); i<count; ++i )
		{
			art[i] = rt[i];
		}
	}

	for ( BBUL i(0); i<D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i )
	{
		if ( art[i]!=m_CurRenderTarget[i] )		rtsame = false;
		if ( nullptr!=m_CurRenderTarget[i] )	m_CurRenderTarget[i]->DetachRtv();
		if ( nullptr!=art[i] )					art[i]->AttachRtv();
		m_CurRenderTarget[i] = art[i];
	}
	if ( !rtsame )
	{
		for ( BBUL i(0); i<count; ++i )
		{
			rtv[i] = art[i]->Rtv();
		}
		m_Context->OMSetRenderTargets( count, rtv, m_CurDepthStencil->Dsv() );
	}
}

void BbDeviceDirect::ActiveStateSampler( BbDevStateSampler* ss, BBUC pipe, BBUC slot )
{
	if ( ss!=m_PipeSampler[pipe][slot] )
	{
		DeActiveStateSampler( pipe, slot );
		if ( nullptr!=ss )
		{
			ID3D11SamplerState* dss = ss->State();
			ss->AttachS( pipe, slot );
			switch( pipe )
			{
			case BB_PIPE_TYPE_VS:	m_Context->VSSetSamplers( slot, 1, &dss );	break;
			case BB_PIPE_TYPE_HS:	m_Context->HSSetSamplers( slot, 1, &dss );	break;
			case BB_PIPE_TYPE_GS:	m_Context->GSSetSamplers( slot, 1, &dss );	break;
			case BB_PIPE_TYPE_DS:	m_Context->DSSetSamplers( slot, 1, &dss );	break;
			case BB_PIPE_TYPE_PS:	m_Context->PSSetSamplers( slot, 1, &dss );	break;
			default:	break;
			}			
		}
		m_PipeSampler[pipe][slot] = ss;
	}
}

void BbDeviceDirect::ActiveSrv( BbDevDataSrv* dds, BBUC pipe, BBUC slot )
{
	if ( dds!=m_PipeSrv[pipe][slot] )
	{
		DeActiveSrv( pipe, slot );
		if ( nullptr!=dds )
		{
			ID3D11ShaderResourceView* srv = dds->Srv();
			dds->AttachSrv( pipe, slot );
			switch( pipe )
			{
			case BB_PIPE_TYPE_VS:	m_Context->VSSetShaderResources( slot, 1, &srv );	break;
			case BB_PIPE_TYPE_HS:	m_Context->HSSetShaderResources( slot, 1, &srv );	break;
			case BB_PIPE_TYPE_GS:	m_Context->GSSetShaderResources( slot, 1, &srv );	break;
			case BB_PIPE_TYPE_DS:	m_Context->DSSetShaderResources( slot, 1, &srv );	break;
			case BB_PIPE_TYPE_PS:	m_Context->PSSetShaderResources( slot, 1, &srv );	break;
			default:	break;
			}
		}
		m_PipeSrv[pipe][slot] = dds;
	}
}

void BbDeviceDirect::ActiveConst( BbDevBufConst* dbc, BBUC pipe, BBUC slot )
{
	dbc->Commit();
	if ( dbc!=m_PipeBufConst[pipe][slot] )
	{
		DeActiveSrv( pipe, slot );
		if ( nullptr!=dbc )
		{
			ID3D11Buffer* buf = dbc->Buf();
			dbc->AttachConst( pipe, slot );
			switch( pipe )
			{
			case BB_PIPE_TYPE_VS:	m_Context->VSSetConstantBuffers( slot, 1, &buf );	break;
			case BB_PIPE_TYPE_HS:	m_Context->HSSetConstantBuffers( slot, 1, &buf );	break;
			case BB_PIPE_TYPE_GS:	m_Context->GSSetConstantBuffers( slot, 1, &buf );	break;
			case BB_PIPE_TYPE_DS:	m_Context->DSSetConstantBuffers( slot, 1, &buf );	break;
			case BB_PIPE_TYPE_PS:	m_Context->PSSetConstantBuffers( slot, 1, &buf );	break;
			default:	break;
			}
		}
		m_PipeBufConst[pipe][slot] = dbc;
	}
}

void BbDeviceDirect::DeActiveConst( BBUC pipe, BBUC slot )
{
	if ( nullptr!=m_PipeBufConst[pipe][slot] )
	{
		switch( pipe )
		{
		case BB_PIPE_TYPE_VS:	m_Context->VSSetConstantBuffers( slot, 1, nullptr );	break;
		case BB_PIPE_TYPE_HS:	m_Context->HSSetConstantBuffers( slot, 1, nullptr );	break;
		case BB_PIPE_TYPE_GS:	m_Context->GSSetConstantBuffers( slot, 1, nullptr );	break;
		case BB_PIPE_TYPE_DS:	m_Context->DSSetConstantBuffers( slot, 1, nullptr );	break;
		case BB_PIPE_TYPE_PS:	m_Context->PSSetConstantBuffers( slot, 1, nullptr );	break;
		default:	break;
		}
		m_PipeBufConst[pipe][slot]->DetachConst( pipe, slot );
		m_PipeBufConst[pipe][slot] = nullptr;
	}
}

void BbDeviceDirect::DeActiveSrv( BBUC pipe, BBUC slot )
{
	if ( nullptr!=m_PipeSrv[pipe][slot] )
	{
		switch( pipe )
		{
		case BB_PIPE_TYPE_VS:	m_Context->VSSetShaderResources( slot, 1, nullptr );	break;
		case BB_PIPE_TYPE_HS:	m_Context->HSSetShaderResources( slot, 1, nullptr );	break;
		case BB_PIPE_TYPE_GS:	m_Context->GSSetShaderResources( slot, 1, nullptr );	break;
		case BB_PIPE_TYPE_DS:	m_Context->DSSetShaderResources( slot, 1, nullptr );	break;
		case BB_PIPE_TYPE_PS:	m_Context->PSSetShaderResources( slot, 1, nullptr );	break;
		default:	break;
		}
		m_PipeSrv[pipe][slot]->DetachSrv( pipe, slot );
		m_PipeSrv[pipe][slot] = nullptr;
	}
}

void BbDeviceDirect::DeActiveStateSampler( BBUC pipe, BBUC slot )
{
	if ( nullptr!=m_PipeSampler[pipe][slot] )
	{
		switch( pipe )
		{
		case BB_PIPE_TYPE_VS:	m_Context->VSSetSamplers( slot, 1, nullptr );	break;
		case BB_PIPE_TYPE_HS:	m_Context->HSSetSamplers( slot, 1, nullptr );	break;
		case BB_PIPE_TYPE_GS:	m_Context->GSSetSamplers( slot, 1, nullptr );	break;
		case BB_PIPE_TYPE_DS:	m_Context->DSSetSamplers( slot, 1, nullptr );	break;
		case BB_PIPE_TYPE_PS:	m_Context->PSSetSamplers( slot, 1, nullptr );	break;
		default:	break;
		}
		m_PipeSampler[pipe][slot]->DetachS( pipe, slot );
		m_PipeSampler[pipe][slot] = nullptr;
	}
}

void BbDeviceDirect::DeActiveRenderTarget()
{
	BBUC i=0;
	while ( i<D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT && nullptr!=m_CurRenderTarget[i] )
	{
		m_CurRenderTarget[i]->DetachRtv();
		m_CurRenderTarget[i] = nullptr;
		++i;
	}
	m_DefaultRenderTarget.AttachRtv();
	m_CurRenderTarget[0] = &m_DefaultRenderTarget;
	ID3D11RenderTargetView* rtv = m_DefaultRenderTarget.Rtv();
	m_Context->OMSetRenderTargets( 1, &rtv, m_CurDepthStencil->Dsv() );
}

//////////////////////////////////////////////////////////////////////////

ID3D11Device* BbDeviceDirect::Device() const
{
	return m_Device;
}

ID3D11DeviceContext* BbDeviceDirect::Context() const
{
	return m_Context;
}

IDXGISwapChain* BbDeviceDirect::SwapChain() const
{
	return m_SwapChain;
}

BbDeviceDataMgr* BbDeviceDirect::DataMgr()
{
	return &m_DataMgr;
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceDirect::InitDefault()
{
	BbDevData::InitDevice( this );
	InitDefaultData();
	InitDefaultValue();
}

void BbDeviceDirect::InitDefaultData()
{
	//BlendNone
	D3D11_BLEND_DESC bd;
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
	m_DefaultStateBlend.Init( &bd );

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
	m_DefaultStateDepthStencil.Init( &dsd );

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
	m_DefaultStateRasterizer.Init( &rsd );

	//View Port
	m_DefaultViewPort.Init( m_Width, m_Height );

	//Render Target
	ID3D11Texture2D* pBackBuffer = NULL;
	m_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	m_DefaultRenderTarget.Init( pBackBuffer );

	//Depth Stencil
	BbDevDepthStencilDesc bddsd;
	bddsd.width = m_Width;
	bddsd.height = m_Height;
	bddsd.t2dfmt = DXGI_FORMAT_D24_UNORM_S8_UINT;
	bddsd.dsvfmt = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//bddsd.dsvfmt = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	bddsd.srvfmt = DXGI_FORMAT_UNKNOWN;
	m_DefaultDepthStencil.Init( &bddsd );
}

void BbDeviceDirect::InitDefaultValue()
{
	BBUL len1 = sizeof(void*) * BB_PIPE_TYPE_COUNT * D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT;	
	BBUL len2 = sizeof(void*) * D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;
	memset( m_PipeSampler, 0, len1 );
	memset( m_PipeSrv, 0, len1 );
	memset( m_PipeBufConst, 0, len1 );
	memset( m_CurRenderTarget, 0, len2 );

	m_DefaultStateBlend.Attach();
	m_DefaultStateDepthStencil.Attach();
	m_DefaultStateRasterizer.Attach();
	m_DefaultViewPort.Attach();
	m_DefaultRenderTarget.Attach();
	m_DefaultDepthStencil.Attach();
	
	m_CurStateBlend = &m_DefaultStateBlend;
	m_CurStateDepthStencil = &m_DefaultStateDepthStencil;
	m_CurStateRasterizer = &m_DefaultStateRasterizer;
	m_CurViewPort = &m_DefaultViewPort;	
	m_CurDepthStencil = &m_DefaultDepthStencil;
	m_CurBufVertex = nullptr;
	m_CurBufIndex = nullptr;
	m_CurLayout = nullptr;
	m_CurTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_CurRenderTarget[0] = &m_DefaultRenderTarget;
	m_DefaultRenderTarget.AttachRtv();

	m_Context->OMSetBlendState( m_CurStateBlend->State(), m_CurStateBlend->BlendFactor(), m_CurStateBlend->SampleMask() );
	m_Context->OMSetDepthStencilState( m_CurStateDepthStencil->State(), m_CurStateDepthStencil->Ref() );
	m_Context->RSSetState( m_CurStateRasterizer->State() );
	m_Context->RSSetViewports( 1, m_CurViewPort->Vp() );
	ID3D11RenderTargetView* rtv = m_CurRenderTarget[0]->Rtv();
	m_Context->OMSetRenderTargets( 1, &rtv, m_CurDepthStencil->Dsv() );
	m_Context->IASetPrimitiveTopology( m_CurTopology );	
}

void BbDeviceDirect::DestroyDefault()
{
	BbDevData::DestroyDevice();
	DestroyDefaultValue();
	DestroyDefaultData();	
}

void BbDeviceDirect::DestroyDefaultValue()
{
	m_DefaultStateBlend.Detach();
	m_DefaultStateDepthStencil.Detach();
	m_DefaultStateRasterizer.Detach();
	m_DefaultViewPort.Detach();
	m_DefaultRenderTarget.Detach();
	m_DefaultDepthStencil.Detach();

	m_CurStateBlend = nullptr;
	m_CurStateDepthStencil = nullptr;
	m_CurStateRasterizer = nullptr;
	m_CurViewPort = nullptr;
	m_CurDepthStencil = nullptr;
	m_CurLayout = nullptr;

	m_Context->OMSetBlendState( nullptr, nullptr, 0 );
	m_Context->OMSetDepthStencilState( nullptr, 1 );
	m_Context->RSSetState( nullptr );
	m_Context->RSSetViewports( 0, nullptr );
	m_Context->OMSetRenderTargets( 0, nullptr, nullptr );
	m_Context->IASetInputLayout( nullptr );
}

void BbDeviceDirect::DestroyDefaultData()
{
	m_DefaultDepthStencil.Destroy();
	m_DefaultRenderTarget.Destroy();
	m_DefaultViewPort.Destroy();
	m_DefaultStateRasterizer.Destroy();
	m_DefaultStateBlend.Destroy();
	m_DefaultStateDepthStencil.Destroy();
}