#include "DeviceDataMgr.h"
#include "DeviceDirect.h"

void BbDeviceDataMgr::Init( BbDeviceDirect* dd )
{
	m_Dd = dd;
}

void BbDeviceDataMgr::Destroy()
{
	m_Dd = nullptr;

	for (StateBlendList::iterator iter(m_lStateBlend.begin), end(m_lStateBlend.end()); iter != end; ++iter )
	{
		iter->State()->Release();
	}
	for (StateDepthStencil::iterator iter(m_lStateDepthStencil.begin), end(m_lStateDepthStencil.end()); iter != end; ++iter)
	{
		iter->State()->Release();
	}
	for (StateRasterizer::iterator iter(m_lStateRasterizer.begin), end(m_lStateRasterizer.end()); iter != end; ++iter)
	{
		iter->State()->Release();
	}
	for (StateSampler::iterator iter(m_lStateSampler.begin), end(m_lStateSampler.end()); iter != end; ++iter)
	{
		iter->State()->Release();
	}

	m_lStateBlend.clear();
	m_lStateDepthStencil.clear();
	m_lStateRasterizer.clear();
	m_lStateSampler.clear();
	m_aPtrData.clear();
}

void BbDeviceDataMgr::Update()
{
	ClearRefNull();
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceDataMgr::SetRefNull( BbDevData* data )
{
	BBUS pos = FindPosFromArray( &m_aPtrData, data );
	if ( BBUS_MAX==pos )
	{
		m_aPtrData.push_back( data );
	}	
}

void BbDeviceDataMgr::ClearRefNull()
{
	if ( !m_aPtrData.empty() )
	{
		for ( DataPtrArr::iterator iter(m_aPtrData.begin()), end(m_aPtrData.end()); iter!=end; ++iter )
		{
			BbDevData* data = *iter;
			if ( 0==data->RefNum() )
			{
				switch(data->Type())
				{
				case BB_DEV_DAT_STATE_BLEND:				DestroyStateBlend(static_cast<BbDevStateBlend*>(data));							break;
				case BB_DEV_DAT_STATE_DEPTH_STENCIL:	DestroyStateDepthStencil(static_cast<BbDevStateDepthStencil*>(data));	break;
				case BB_DEV_DAT_STATE_RASTERIZER:		DestroyStateRasterizer(static_cast<BbDevStateRasterizer*>(data));			break;
				case BB_DEV_DAT_STATE_SAMPLER:			DestroyStateSampler(static_cast<BbDevStateSampler*>(data));				break;
				case BB_DEV_DAT_TEXTURE_1D:		break;
				case BB_DEV_DAT_TEXTURE_2D:		break;
				case BB_DEV_DAT_TEXTURE_3D:		break;
				case BB_DEV_DAT_RENDER_TARGET:	break;
				case BB_DEV_DAT_DEPTH_STENCIL:	break;
				case BB_DEV_DAT_BUFFER_CONST:	break;
				case BB_DEV_DAT_BUFFER_VERTEX:	break;
				case BB_DEV_DAT_BUFFER_INDEX:	break;
				default:	break;
				}
			}
		}
		m_aPtrData.clear();
	}
}

void BbDeviceDataMgr::DestroyStateBlend( BbDevStateBlend* state )
{
	StateBlendList::iterator iter = FindIterFromList( &m_lStateBlend, state );
	if ( iter!=m_lStateBlend.end() )
	{
		iter->State()->Release();
		m_lStateBlend.erase( iter );
	}
}

void BbDeviceDataMgr::DestroyStateDepthStencil( BbDevStateDepthStencil* state )
{
	StateDepthStencil::iterator iter = FindIterFromList( &m_lStateDepthStencil, state );
	if ( iter!=m_lStateDepthStencil.end() )
	{
		iter->State()->Release();
		m_lStateDepthStencil.erase( iter );
	}
}

void BbDeviceDataMgr::DestroyStateRasterizer( BbDevStateRasterizer* state )
{
	StateRasterizer::iterator iter = FindIterFromList( &m_lStateRasterizer, state );
	if ( iter!=m_lStateRasterizer.end() )
	{
		iter->State()->Release();
		m_lStateRasterizer.erase( iter );
	}
}

void BbDeviceDataMgr::DestroyStateSampler(BbDevStateSampler* state)
{
	StateSampler::iterator iter = FindIterFromList(&m_lStateSampler, state);
	if ( iter!=m_lStateSampler.end() )
	{
		iter->State()->Release();
		m_lStateSampler.erase(iter);
	}
}

//////////////////////////////////////////////////////////////////////////

BbDevStateBlend* BbDeviceDataMgr::CreateStateBlend( D3D11_BLEND_DESC* desc )
{
	BbDevStateBlend* ret = nullptr;
	ID3D11BlendState* blend = nullptr;
	if( S_OK==m_Dd->Device()->CreateBlendState( desc, &blend ) )
	{
		m_lStateBlend.push_back( BbDevStateBlend(blend) );
		ret = &m_lStateBlend.back();
		ret->RefAdd();
	}
	
	return ret;
}

BbDevStateDepthStencil*	BbDeviceDataMgr::CreateStateDepthStencil( D3D11_DEPTH_STENCIL_DESC* desc )
{
	BbDevStateDepthStencil* ret = nullptr;
	ID3D11DepthStencilState*	dss = nullptr;
	if ( S_OK==m_Dd->Device()->CreateDepthStencilState( desc, &dss ) )
	{
		m_lStateDepthStencil.push_back( BbDevStateDepthStencil(dss) );
		ret = &m_lStateDepthStencil.back();
		ret->RefAdd();
	}
	return ret;
}

BbDevStateRasterizer* BbDeviceDataMgr::CreateStateRasterizer( D3D11_RASTERIZER_DESC* desc )
{
	BbDevStateRasterizer* ret = nullptr;
	ID3D11RasterizerState* rs = nullptr;
	if ( S_OK==m_Dd->Device()->CreateRasterizerState( desc, &rs ) )
	{
		m_lStateRasterizer.push_back( BbDevStateRasterizer(rs) );
		ret = &m_lStateRasterizer.back();
		ret->RefAdd();
	}
	return ret;
}

BbDevStateSampler* BbDeviceDataMgr::CreateStateSampler(D3D11_SAMPLER_DESC* desc)
{
	BbDevStateSampler* ret = nullptr;
	ID3D11SamplerState* ss = nullptr;
	if ( S_OK==m_Dd->Device()->CreateSamplerState( &desc, &ss) )
	{
		m_lStateSampler.push_back(BbDevStateSampler(ss));
		ret = &m_lStateSampler.back();
		ret->RefAdd();
	}
	return ret;
}