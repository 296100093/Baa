#include "DeviceEffect.h"
#include "DeviceDirect.h"

//////////////////////////////////////////////////////////////////////////

#if defined( DEBUG ) || defined( _DEBUG )
#define BB_COMPILE_SHADER_FLAG1	D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION | D3D10_SHADER_WARNINGS_ARE_ERRORS
#else
#define BB_COMPILE_SHADER_FLAG1	D3D10_SHADER_ENABLE_STRICTNESS
#endif


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BbDeviceEffect::BbDeviceEffect()
{

}

BbDeviceEffect::~BbDeviceEffect()
{

}

void BbDeviceEffect::Destroy()
{
	for ( PackVertexArray::iterator iter(m_aVertex.begin()), end(m_aVertex.end()); iter!=end; ++iter )
	{
		iter->layout.Destroy();
		BB_SAFE_RELEASE( iter->vs );
	}
	for ( PackHullArray::iterator iter(m_aHull.begin()), end(m_aHull.end()); iter!=end; ++iter )
	{
		BB_SAFE_RELEASE( iter->hs );
	}
	for ( PackGeometryArray::iterator iter(m_aGeometry.begin()), end(m_aGeometry.end()); iter!=end; ++iter )
	{
		BB_SAFE_RELEASE( iter->gs );
	}
	for ( PackDomainArray::iterator iter(m_aDomain.begin()), end(m_aDomain.end()); iter!=end; ++iter )
	{
		BB_SAFE_RELEASE( iter->ds );
	}
	for ( PackPixelArray::iterator iter(m_aPixel.begin()), end(m_aPixel.end()); iter!=end; ++iter )
	{
		BB_SAFE_RELEASE( iter->ps );
	}
	for ( DevBufConstList::iterator iter(m_lBufConst.begin()), end(m_lBufConst.end()); iter!=end; ++iter )
	{
		iter->Destroy();
	}

	m_aVertex.clear();
	m_aHull.clear();
	m_aGeometry.clear();
	m_aDomain.clear();
	m_aPixel.clear();
	m_aTech.clear();

	m_aSlotBc.clear();
	m_aSlotSam.clear();
	m_aSlotSrv.clear();

	m_lBufConst.clear();
}

void BbDeviceEffect::InitVertex( const char* src, BbLoadFromTypeE srctype, const char* func, BbDevLayoutDesc* ld )
{
	ID3DBlob* blobShader = nullptr;
	ID3DBlob* blobError = nullptr;
	if ( S_OK==InitCompiler( src, srctype, func, "vs_5_0", &blobShader, &blobError ) )
	{
		InitVertexFromBlob( blobShader, func, ld );
	}
	else
	{
		OnBlobError( blobError );
	}
}

void BbDeviceEffect::InitPixel( const char* src, BbLoadFromTypeE srctype, const char* func )
{
	ID3DBlob* blobShader = nullptr;
	ID3DBlob* blobError = nullptr;
	ID3D11PixelShader* ps = nullptr;
	if ( S_OK==InitCompiler( src, srctype, func, "ps_5_0", &blobShader, &blobError ) )
	{
		if ( S_OK==BbDevData::s_BbDd->Device()->CreatePixelShader( blobShader->GetBufferPointer(), blobShader->GetBufferSize(), nullptr, &ps ) )
		{
			DataPixel dp;
			dp.name = func;
			dp.namehash = func;
			dp.ps = ps;
			m_aPixel.push_back( dp );
		}
		BB_SAFE_RELEASE( blobShader );
	}
	else
	{
		OnBlobError( blobError );
	}
}

void BbDeviceEffect::InitHull( const char* src, BbLoadFromTypeE srctype, const char* func )
{
	ID3DBlob* blobShader = nullptr;
	ID3DBlob* blobError = nullptr;
	ID3D11HullShader* hs = nullptr;
	if ( S_OK==InitCompiler( src, srctype, func, "hs_5_0", &blobShader, &blobError ) )
	{
		if ( S_OK==BbDevData::s_BbDd->Device()->CreateHullShader( blobShader->GetBufferPointer(), blobShader->GetBufferSize(), nullptr, &hs ) )
		{
			DataHull dh;
			dh.name = func;
			dh.namehash = func;
			dh.hs = hs;
			m_aHull.push_back( dh );
		}
		BB_SAFE_RELEASE( blobShader );
	}
	else
	{
		OnBlobError( blobError );
	}
}

void BbDeviceEffect::InitDomain( const char* src, BbLoadFromTypeE srctype, const char* func )
{
	ID3DBlob* blobShader = nullptr;
	ID3DBlob* blobError = nullptr;
	ID3D11DomainShader* ds = nullptr;
	if ( S_OK==InitCompiler( src, srctype, func, "ds_5_0", &blobShader, &blobError ) )
	{
		if ( S_OK==BbDevData::s_BbDd->Device()->CreateDomainShader( blobShader->GetBufferPointer(), blobShader->GetBufferSize(), nullptr, &ds ) )
		{
			DataDomain dd;
			dd.name = func;
			dd.namehash = func;
			dd.ds = ds;
			m_aDomain.push_back( dd );
		}
		BB_SAFE_RELEASE( blobShader );
	}
	else
	{
		OnBlobError( blobError );
	}
}

void BbDeviceEffect::InitGeometry( const char* src, BbLoadFromTypeE srctype, const char* func )
{
	ID3DBlob* blobShader = nullptr;
	ID3DBlob* blobError = nullptr;
	ID3D11GeometryShader* gs = nullptr;
	if ( S_OK==InitCompiler( src, srctype, func, "gs_5_0", &blobShader, &blobError ) )
	{
		if ( S_OK==BbDevData::s_BbDd->Device()->CreateGeometryShader( blobShader->GetBufferPointer(), blobShader->GetBufferSize(), nullptr, &gs ) )
		{
			DataGeometry dg;
			dg.name = func;
			dg.namehash = func;
			dg.gs = gs;
			m_aGeometry.push_back( dg );
		}
		BB_SAFE_RELEASE( blobShader );
	}
	else
	{
		OnBlobError( blobError );
	}
}

void BbDeviceEffect::InitEffect( const char* name, const char* vs, const char* ps, const char* hs/* =NULL */, const char* ds/* =NULL */, const char*gs/* =NULL */ )
{
	DataVertex* dv = FindDataFromArray( &m_aVertex, vs );
	DataPixel* dp = FindDataFromArray( &m_aPixel, ps );
	DataHull* dh = FindDataFromArray( &m_aHull, hs );
	DataDomain* dd = FindDataFromArray( &m_aDomain, ds );
	DataGeometry* dg = FindDataFromArray( &m_aGeometry, gs );
	if ( nullptr!=dv )
	{
		DataEffect de;
		de.name = name;
		de.namehash = name;
		de.layout = dv->layout;
		de.vs = dv->vs;
		de.ps = nullptr==dp?nullptr:dp->ps;
		de.hs = nullptr==dh?nullptr:dh->hs;
		de.ds = nullptr==dd?nullptr:dd->ds;
		de.gs = nullptr==dg?nullptr:dg->gs;
		m_aEffect.push_back( de );
	}
}

//////////////////////////////////////////////////////////////////////////

HRESULT BbDeviceEffect::InitCompiler( const char* src, BbLoadFromTypeE srctype, const char* func, const char* profile, ID3DBlob** bs, ID3DBlob** be )
{
	HRESULT hr = E_FAIL;
	if ( BB_LOAD_FROM_RES==srctype )
	{
		hr = D3DX11CompileFromResourceA( GetModuleHandleA(NULL), src, 0, nullptr, 0, func, profile, BB_COMPILE_SHADER_FLAG1, 0, nullptr, bs, be, nullptr );
	}
	else if ( BB_LOAD_FROM_ZIP==srctype )
	{
		//D3DX11CompileFromMemory
	}
	else if ( BB_LOAD_FROM_FILE==srctype )
	{
		hr = D3DX11CompileFromFileA( src, nullptr, 0, func, profile, BB_COMPILE_SHADER_FLAG1, 0, nullptr, bs, be, nullptr );
	}
	return hr;
}

void BbDeviceEffect::InitVertexFromBlob( ID3DBlob* pBlob, const char* func, BbDevLayoutDesc* ld )
{
	ID3D11InputLayout* il = nullptr;
	ID3D11VertexShader* vs = nullptr;
	PackVertex pv;

	if( S_OK==BbDevData::s_BbDd->Device()->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &vs ) )
	{
		if ( S_OK==BbDevData::s_BbDd->Device()->CreateInputLayout( ld->Buf(), ld->Count(), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &il ) )
		{
			OnReflect( &pv, BB_PIPE_TYPE_VS, pBlob );			
			pv.name = func;
			pv.namehash = func;
			pv.vs = vs;
			pv.layout.Init( il );
			m_aVertex.push_back( pv );
		}
	}
	BB_SAFE_RELEASE( pBlob );	
}

void BbDeviceEffect::OnBlobError( ID3DBlob*& error )
{
	if ( nullptr!=error )
	{
		OutputDebugStringA( (char*)error->GetBufferPointer() );
		BB_SAFE_RELEASE( error );
	}
}

void BbDeviceEffect::OnReflect( PackBase* pb, BbPipeTypeE type, ID3DBlob* blob )
{
	D3D11_SHADER_DESC ShaderDesc;
	ID3D11ShaderReflection* reflection = nullptr;
	
	D3DReflect( blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &reflection );
	reflection->GetDesc( &ShaderDesc );
	for ( UINT n = 0; n < ShaderDesc.BoundResources; ++n )
	{
		D3D11_SHADER_INPUT_BIND_DESC sibd;
		reflection->GetResourceBindingDesc( n, &sibd );
		switch( sibd.Type )
		{
		case D3D10_SIT_CBUFFER:
			{

			}
			break;
		case D3D10_SIT_SAMPLER:
			{

			}
			break;
		case D3D10_SIT_TEXTURE:
			{

			}
			break;
		case D3D10_SIT_TBUFFER:
			{

			}
			break;
		default:	break;
		}

		SlotData slot;
		slot.slot = sibd.BindPoint;
		slot.data = GetRealData( sibd.Name );
		if ( ShaderEffect::InValidData == slot.data )
		{
			RealData real;
			real.section[pipe] = true;
			real.data = pContentSet->GetContentData( sibd.Name );
			if ( NULL == real.data )
			{
				if ( D3D10_SIT_CBUFFER == sibd.Type )
				{
					ContentCBuffer* ccb = ContentSet::Instance().InitCBuffer( reflection->GetConstantBufferByIndex(sibd.BindPoint) );
					real.data = ccb;
					maContentCBuffer.push_back( ccb );
				}
				else if ( D3D10_SIT_SAMPLER == sibd.Type )
				{
					real.data = pContentSet->GetContentDataFromDefault( sibd.Name );
				}
			}
			slot.data = maRealData.size();
			maRealData.push_back( real );
			maSlotData.push_back( slot );
			mmSlotDataLink[sibd.Name] = slot.data;
		}
		else
		{
			maRealData[slot.data].section[pipe] = true;
			maSlotData.push_back( slot );
		}
	}
}