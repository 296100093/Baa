#include "DeviceBuffer.h"
#include "DeviceDirect.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BbDevBufVertex* BbDevBufVertex::Create(BbDevBufVertexDesc* desc)
{
	return s_BbDdMgr->CreateBufVertex(desc);
}

BbDevBufVertex::BbDevBufVertex(ID3D11Buffer* buf, UINT stride, bool imm)
	: m_Buffer(buf)
	, m_Stride(stride)
	, m_Immutable(imm)
{
	m_Type = BB_DEV_DAT_BUFFER_VERTEX;
}

void BbDevBufVertex::Init(BbDevBufVertexDesc* bvd)
{
	ID3D11Device* device = s_BbDd->Device();
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA srd;
	HRESULT hr;

	m_Stride = static_cast<UINT>(bvd->stride);
	m_Immutable = bvd->immutable;

	bd.ByteWidth = bvd->stride * bvd->count;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = bvd->immutable ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = bvd->immutable ? 0 : D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	srd.pSysMem = bvd->src;
	srd.SysMemPitch = 0;
	srd.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&bd, &srd, &m_Buffer);
}

void BbDevBufVertex::Destroy()
{
	if (Istach())
	{
		s_BbDd->ActiveBufVertex(nullptr, 0);
	}
	BB_SAFE_RELEASE(m_Buffer);
	m_Stride = 0;
	m_Immutable = false;
}

void BbDevBufVertex::Active(BBUI off)
{
	s_BbDd->ActiveBufVertex(nullptr == m_Buffer ? nullptr : this, off);
}

ID3D11Buffer* BbDevBufVertex::Buf()
{
	return m_Buffer;
}

UINT BbDevBufVertex::Stride()
{
	return m_Stride;
}

//////////////////////////////////////////////////////////////////////////

BbDevBufIndex::BbDevBufIndex()
	: m_Buffer(nullptr)
	, m_Format(DXGI_FORMAT_UNKNOWN)
	, m_Immutable(false)
{

}

void BbDevBufIndex::Init(BbDevBufIndexDesc* bid)
{
	ID3D11Device* device = s_BbDd->Device();
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA srd;
	HRESULT hr;

	m_Format = bid->i16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_Immutable = bid->immutable;

	bd.ByteWidth = bid->count*(bid->i16 ? sizeof(unsigned short) : sizeof(unsigned int));
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = bid->immutable ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = bid->immutable ? 0 : D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	srd.pSysMem = bid->src;
	srd.SysMemPitch = 0;
	srd.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&bd, &srd, &m_Buffer);
}

void BbDevBufIndex::Destroy()
{
	if (Istach())
	{
		s_BbDd->ActiveBufIndex(nullptr, 0);
	}
	BB_SAFE_RELEASE(m_Buffer);
	m_Format = DXGI_FORMAT_UNKNOWN;
	m_Immutable = false;
}

void BbDevBufIndex::Active(BBUI off)
{
	s_BbDd->ActiveBufIndex(nullptr == m_Buffer ? nullptr : this, off);
}

ID3D11Buffer* BbDevBufIndex::Buf()
{
	return m_Buffer;
}

DXGI_FORMAT BbDevBufIndex::Format()
{
	return m_Format;
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
	return m_Desc.size();
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

BbDevLayout::BbDevLayout()
	: m_Layout(nullptr)
{

}

void BbDevLayout::Init(ID3D11InputLayout* il)
{
	Destroy();
	m_Layout = il;
}

void BbDevLayout::Destroy()
{
	if (Istach())
	{
		s_BbDd->ActiveLayout(nullptr);
	}
	BB_SAFE_RELEASE(m_Layout);
}

void BbDevLayout::Active()
{
	s_BbDd->ActiveLayout(nullptr == m_Layout ? nullptr : this);
}

ID3D11InputLayout* BbDevLayout::Layout()
{
	return m_Layout;
}

