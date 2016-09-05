#include "DeviceDataMgr.h"
#include "DeviceDirect.h"

//////////////////////////////////////////////////////////////////////////

BbDeviceDataMgr* BbDeviceDataMgr::s_BbdDataMgr = nullptr;
BbDeviceDataMgr* BbDeviceDataMgr::Get()
{
	return s_BbdDataMgr;
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceDataMgr::Init( BbDeviceDirect* direct )
{
	s_BbdDataMgr = this;
	m_BbdDirect = direct;
}

void BbDeviceDataMgr::Destroy()
{
	s_BbdDataMgr = nullptr;
	m_BbdDirect = nullptr;

	m_ItlStateBlend.Destroy();
	m_ItlStateDepthStencil.Destroy();
	m_ItlStateRasterizer.Destroy();
	m_ItlStateSampler.Destroy();
	m_ItlBufVertex.Destroy();
	m_ItlBufIndex.Destroy();
	m_ItlTex1D.Destroy();
	m_ItlTex2D.Destroy();
	m_ItlTex3D.Destroy();
	m_ItlRenderTarget.Destroy();
	m_ItlDepthStencil.Destroy();
	m_ItlDataNull.clear();
}

void BbDeviceDataMgr::Update()
{
	ClearRefNull();
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceDataMgr::SetRefNull( BbDevDataItl* data )
{
	if ( BBUL_MAX!=data->IdxGet() )
	{
		m_ItlDataNull.push_back( data );
	}	
}

void BbDeviceDataMgr::ClearRefNull()
{
	if ( !m_ItlDataNull.empty() )
	{
		for ( ItlDataPtr::iterator iter(m_ItlDataNull.begin()), end(m_ItlDataNull.end()); iter!=end; ++iter )
		{
			BbDevDataItl* data = *iter;
			if ( 0==data->RefNum() )
			{
				switch(data->Type())
				{
// 				case BB_DEV_DAT_STATE_BLEND:			static_cast<BbDevStateBlendItl*>(data)->DestroyNull();			break;
// 				case BB_DEV_DAT_STATE_DEPTH_STENCIL:	static_cast<BbDevStateDepthStencilItl*>(data)->DestroyNull();	break;
// 				case BB_DEV_DAT_STATE_RASTERIZER:		static_cast<BbDevStateRasterizerItl*>(data)->DestroyNull();		break;
// 				case BB_DEV_DAT_STATE_SAMPLER:			static_cast<BbDevStateSamplerItl*>(data)->DestroyNull();		break;
				case BB_DEV_DATA_BUFFER_VERTEX:	m_ItlBufVertex.DestroyOne( data->IdxGet() );	break;
				case BB_DEV_DATA_BUFFER_INDEX:	m_ItlBufIndex.DestroyOne( data->IdxGet() );		break;
				case BB_DEV_DATA_TEXTURE_1D:		break;
				case BB_DEV_DATA_TEXTURE_2D:		break;
				case BB_DEV_DATA_TEXTURE_3D:		break;
				case BB_DEV_DATA_RENDER_TARGET:	break;
				case BB_DEV_DATA_DEPTH_STENCIL:	break;
				default:	break;
				}
			}
		}
		m_ItlDataNull.clear();
	}
}

//////////////////////////////////////////////////////////////////////////

BBUL BbDeviceDataMgr::CreateStateBlend( D3D11_BLEND_DESC* desc )
{
	BBUL ret = m_ItlStateBlend.FindByDesc( desc );
	BbDevStateBlendItl* itl = nullptr;
	if ( BBUL_MAX==ret )
	{
		ret = m_ItlStateBlend.AddOne();
		itl = m_ItlStateBlend.At(ret);
		itl->Create( desc );
	}
	else
	{
		itl = m_ItlStateBlend.At(ret);
	}
	itl->RefAdd();
	return ret;
}

BBUL BbDeviceDataMgr::CreateStateDepthStencil( D3D11_DEPTH_STENCIL_DESC* desc )
{
	BBUL ret = m_ItlStateDepthStencil.FindByDesc( desc );
	BbDevStateDepthStencilItl* itl = nullptr;
	if ( BBUL_MAX==ret )
	{
		ret = m_ItlStateDepthStencil.AddOne();
		itl = m_ItlStateDepthStencil.At(ret);
		itl->Create( desc );
	}
	else
	{
		itl = m_ItlStateDepthStencil.At(ret);
	}
	itl->RefAdd();
	return ret;
}

BBUL BbDeviceDataMgr::CreateStateRasterizer( D3D11_RASTERIZER_DESC* desc )
{
	BBUL ret = m_ItlStateRasterizer.FindByDesc( desc );
	BbDevStateRasterizerItl* itl = nullptr;
	if ( BBUL_MAX==ret )
	{
		ret = m_ItlStateRasterizer.AddOne();
		itl = m_ItlStateRasterizer.At(ret);
		itl->Create( desc );
	}
	else
	{
		itl = m_ItlStateRasterizer.At(ret);
	}
	itl->RefAdd();
	return ret;
}

BBUL BbDeviceDataMgr::CreateStateSampler(D3D11_SAMPLER_DESC* desc)
{
	BBUL ret = m_ItlStateSampler.FindByDesc( desc );
	BbDevStateSamplerItl* itl = nullptr;
	if ( BBUL_MAX==ret )
	{
		ret = m_ItlStateSampler.AddOne();
		itl = m_ItlStateSampler.At(ret);
		itl->Create( desc );
	}
	else
	{
		itl = m_ItlStateSampler.At(ret);
	}
	itl->RefAdd();
	return ret;
}

BBUL BbDeviceDataMgr::CreateBufVertex()
{
	BBUL ret = m_ItlBufVertex.ApplyOne();
	BbDevBufVertexItl* itl = m_ItlBufVertex.At(ret);
	itl->RefAdd();
	return ret;
}

BBUL BbDeviceDataMgr::CreateBufIndex()
{
	BBUL ret = m_ItlBufIndex.ApplyOne();
	BbDevBufIndexItl* itl = m_ItlBufIndex.At(ret);
	itl->RefAdd();
	return ret;
}

BBUL BbDeviceDataMgr::CreateTex1D(const wchar_t* str)
{
	BbWstring bws(str);
	BBUL ret = m_ItlTex1D.FindByDesc( &bws );
	BbDevTex1DItl* itl = nullptr;
	if ( BBUL_MAX==ret )
	{
		ret = m_ItlTex1D.AddOne();
		itl = m_ItlTex1D.At(ret);
		itl->Create( str );
	}
	else
	{
		itl = m_ItlTex1D.At(ret);
	}
	itl->RefAdd();
	return ret;
}

BBUL BbDeviceDataMgr::CreateTex1D(const BbDevTextureDesc* desc)
{
	BBUL ret = m_ItlTex1D.ApplyOne();
	BbDevTex1DItl* itl = m_ItlTex1D.At(ret);
	itl->Create( desc );
	itl->RefAdd();
	return ret;
}

BBUL BbDeviceDataMgr::CreateTex2D(const wchar_t* str)
{
	BbWstring bws(str);
	BBUL ret = m_ItlTex2D.FindByDesc( &bws );
	BbDevTex2DItl* itl = nullptr;
	if ( BBUL_MAX==ret )
	{
		ret = m_ItlTex2D.AddOne();
		itl = m_ItlTex2D.At(ret);
		itl->Create( str );
	}
	else
	{
		itl = m_ItlTex2D.At(ret);
	}
	itl->RefAdd();
	return ret;
}

BBUL BbDeviceDataMgr::CreateTex2D(const BbDevTextureDesc* desc)
{
	BBUL ret = m_ItlTex2D.ApplyOne();
	BbDevTex2DItl* itl = m_ItlTex2D.At(ret);
	itl->Create( desc );
	itl->RefAdd();
	return ret;
}

BBUL BbDeviceDataMgr::CreateTex3D()
{
	return BBUL_MAX;
}

BBUL BbDeviceDataMgr::CreateRenderTarget()
{
	BBUL ret = m_ItlRenderTarget.ApplyOne();
	BbDevRenderTargetItl* itl = m_ItlRenderTarget.At(ret);
	itl->RefAdd();
	return ret;
}

BBUL BbDeviceDataMgr::CreateDepthStencil()
{
	BBUL ret = m_ItlDepthStencil.ApplyOne();
	BbDevDepthStencilItl* itl = m_ItlDepthStencil.At(ret);
	itl->RefAdd();
	return ret;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BbDevStateBlendItl* BbDeviceDataMgr::GetStateBlendItl(BBUL idx)
{
	return m_ItlStateBlend.At(idx);
}

BbDevStateDepthStencilItl* BbDeviceDataMgr::GetStateDepthStencilItl(BBUL idx)
{
	return m_ItlStateDepthStencil.At(idx);
}

BbDevStateRasterizerItl* BbDeviceDataMgr::GetStateRasterizerItl(BBUL idx)
{
	return m_ItlStateRasterizer.At(idx);
}

BbDevStateSamplerItl* BbDeviceDataMgr::GetStateSamplerItl(BBUL idx)
{
	return m_ItlStateSampler.At(idx);
}

BbDevBufVertexItl* BbDeviceDataMgr::GetBufVertexItl(BBUL idx)
{
	return m_ItlBufVertex.At(idx);
}

BbDevBufIndexItl* BbDeviceDataMgr::GetBufIndexItl(BBUL idx)
{
	return m_ItlBufIndex.At(idx);
}

BbDevTex1DItl* BbDeviceDataMgr::GetTex1DItl(BBUL idx)
{
	return m_ItlTex1D.At(idx);
}

BbDevTex2DItl* BbDeviceDataMgr::GetTex2DItl(BBUL idx)
{
	return m_ItlTex2D.At(idx);
}

BbDevTex3DItl* BbDeviceDataMgr::GetTex3DItl(BBUL idx)
{
	return m_ItlTex3D.At(idx);
}

BbDevRenderTargetItl* BbDeviceDataMgr::GetRenderTargetItl(BBUL idx)
{
	return m_ItlRenderTarget.At(idx);
}

BbDevDepthStencilItl* BbDeviceDataMgr::GetDepthStencilItl(BBUL idx)
{
	return m_ItlDepthStencil.At(idx);
}

void BbDeviceDataMgr::GetRenderPadItl( BbDevRenderPadItl* itl, BbDevRenderPad* pad )
{
	BBUL* data = pad->Data();
	itl->BeNull();
	itl->m_Data.Count = pad->Count();
	itl->m_Data.DepthStencil = GetDepthStencilItl( data[8] );
	for ( BBUS i(0); i<pad->Count(); ++i )
	{
		itl->m_Data.RenderTargetItl[i] = GetRenderTargetItl( data[i] );
		itl->m_Data.Rtv[i] = itl->m_Data.RenderTargetItl[i]->Rtv();
	}	
}

//////////////////////////////////////////////////////////////////////////

BbDeviceDirect* BbDeviceDataMgr::BbdDirect()
{
	return m_BbdDirect;
}

ID3D11Device* BbDeviceDataMgr::Device()
{
	return m_BbdDirect->Device();
}

ID3D11DeviceContext* BbDeviceDataMgr::Context()
{
	return m_BbdDirect->Context();
}

IDXGISwapChain* BbDeviceDataMgr::SwapChain()
{
	return m_BbdDirect->SwapChain();
}