#include "DeviceState.h"
#include "DeviceStateInternal.h"
#include "DeviceDataMgr.h"
#include "DeviceDirect.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BB_DEV_SMART_REF_IMPL(BbDevStateBlend,GetStateBlendItl);

void BbDevStateBlend::Create(D3D11_BLEND_DESC* desc)
{
	assert( BBUL_MAX==m_Idx );
	m_Idx = BbDeviceDataMgr::Get()->CreateStateBlend(desc);
}

void BbDevStateBlend::SetBlendFactor(float f0, float f1, float f2, float f3)
{
	BbDeviceDataMgr::Get()->GetStateBlendItl(m_Idx)->SetBlendFactor( f0, f1, f2, f3 );
}

void BbDevStateBlend::SetSampleMask(BBUL mask)
{
	BbDeviceDataMgr::Get()->GetStateBlendItl(m_Idx)->SetSampleMask(mask);
}

void BbDevStateBlend::Active()
{
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	mgr->BbdDirect()->ActiveStateBlend( mgr->GetStateBlendItl( m_Idx ) );
}

//////////////////////////////////////////////////////////////////////////

BB_DEV_SMART_REF_IMPL(BbDevStateDepthStencil,GetStateDepthStencilItl);

void BbDevStateDepthStencil::Create(D3D11_DEPTH_STENCIL_DESC* desc)
{
	assert( BBUL_MAX==m_Idx );
	m_Idx = BbDeviceDataMgr::Get()->CreateStateDepthStencil(desc);
}

void BbDevStateDepthStencil::SetStencilRef(BBUL ref)
{
	BbDeviceDataMgr::Get()->GetStateDepthStencilItl(m_Idx)->SetStencilRef(ref);
}

void BbDevStateDepthStencil::Active()
{
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	mgr->BbdDirect()->ActiveStateDepthStencil( mgr->GetStateDepthStencilItl( m_Idx ) );
}

//////////////////////////////////////////////////////////////////////////

BB_DEV_SMART_REF_IMPL(BbDevStateRasterizer,GetStateRasterizerItl);

void BbDevStateRasterizer::Create(D3D11_RASTERIZER_DESC* desc)
{
	assert( BBUL_MAX==m_Idx );
	m_Idx = BbDeviceDataMgr::Get()->CreateStateRasterizer(desc);
}

void BbDevStateRasterizer::Active()
{
	BbDeviceDataMgr* mgr = BbDeviceDataMgr::Get();
	mgr->BbdDirect()->ActiveStateRasterizer( mgr->GetStateRasterizerItl( m_Idx ) );
}

//////////////////////////////////////////////////////////////////////////

BB_DEV_SMART_REF_IMPL(BbDevStateSampler,GetStateSamplerItl);

void BbDevStateSampler::Create(D3D11_SAMPLER_DESC* desc)
{
	assert( BBUL_MAX==m_Idx );
	m_Idx = BbDeviceDataMgr::Get()->CreateStateSampler(desc);
}

//////////////////////////////////////////////////////////////////////////

void BbDevLayoutDesc::Add(const char* name, BbLayoutFmtTypeE fmt)
{
	m_StrBuf.push_back(name);
	D3D11_INPUT_ELEMENT_DESC ied;
	ied.SemanticName = m_StrBuf.back().c_str();
	ied.SemanticIndex = 0;
	ied.Format = TurnFmt(fmt);
	ied.InputSlot = 0;
	ied.AlignedByteOffset = CalcOffset();
	ied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	ied.InstanceDataStepRate = 0;
	m_Desc.push_back(ied);
}

D3D11_INPUT_ELEMENT_DESC* BbDevLayoutDesc::Buf()
{
	return &m_Desc.front();
}

BBUL BbDevLayoutDesc::Count()
{
	return static_cast<BBUL>(m_Desc.size());
}

DXGI_FORMAT BbDevLayoutDesc::TurnFmt(BbLayoutFmtTypeE fmt)
{
	DXGI_FORMAT df = DXGI_FORMAT_UNKNOWN;
	switch (fmt)
	{
	case BB_LF_XYZW_F:	df = DXGI_FORMAT_R32G32B32A32_FLOAT;	break;
	case BB_LF_XYZ_F:	df = DXGI_FORMAT_R32G32B32_FLOAT;		break;
	case BB_LF_XY_F:	df = DXGI_FORMAT_R32G32_FLOAT;			break;
	case BB_LF_X_F:		df = DXGI_FORMAT_R32_FLOAT;				break;
	default:	break;
	}
	return df;
}

UINT BbDevLayoutDesc::TurnSize(DXGI_FORMAT fmt)
{
	UINT size = 0;
	switch (fmt)
	{
	case DXGI_FORMAT_R32G32B32A32_FLOAT:	size = 16;	break;
	case DXGI_FORMAT_R32G32B32_FLOAT:		size = 12;	break;
	case DXGI_FORMAT_R32G32_FLOAT:			size = 8;		break;
	case DXGI_FORMAT_R32_FLOAT:				size = 4;		break;
	default:	break;
	}
	return size;
}

UINT BbDevLayoutDesc::CalcOffset()
{
	UINT offset = 0;
	if (0 < m_Desc.size())
	{
		D3D11_INPUT_ELEMENT_DESC* ied = &m_Desc.back();
		offset = ied->AlignedByteOffset + TurnSize(ied->Format);
	}
	return offset;
}

//////////////////////////////////////////////////////////////////////////

BbDevViewPort::BbDevViewPort()
{
	memset( &m_ViewPort, 0, sizeof(D3D11_VIEWPORT) );
}

void BbDevViewPort::Init( BBUL width, BBUL height )
{
	m_ViewPort.Width = static_cast< float >( width );
	m_ViewPort.Height = static_cast< float >( height );
	m_ViewPort.MaxDepth = 1;
	m_ViewPort.MinDepth = 0;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
}

void BbDevViewPort::Active()
{

}

//////////////////////////////////////////////////////////////////////////

BbDevToplogy::BbDevToplogy()
	: m_Topo( D3D_PRIMITIVE_TOPOLOGY_UNDEFINED )
{

}

void BbDevToplogy::Init( D3D11_PRIMITIVE_TOPOLOGY topo )
{
	m_Topo = topo;
}

void BbDevToplogy::Active()
{

}