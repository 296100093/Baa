#include "DeviceEffect.h"
#include "DeviceDataMgr.h"
#include "DeviceDirect.h"
#include "DeviceTextureInternal.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////

#if defined( DEBUG ) || defined( _DEBUG )
#define BB_COMPILE_SHADER_FLAG1	D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION | D3D10_SHADER_WARNINGS_ARE_ERRORS
#else
#define BB_COMPILE_SHADER_FLAG1	D3D10_SHADER_ENABLE_STRICTNESS
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BbDeviceEffectTech::BbDeviceEffectTech()
	:	m_Effect(nullptr)
	,	m_Idx(BBUC_MAX)
{

}

BbDeviceEffectTech::BbDeviceEffectTech( BbDeviceEffect* eft, BBUC idx )
	:	m_Effect(eft)
	,	m_Idx(idx)
{

}

void BbDeviceEffectTech::Begin()
{
	if ( BBUC_MAX!=m_Idx )
	{
		m_Effect->BeginTech( m_Idx );
	}	
}

void BbDeviceEffectTech::End()
{
	if ( BBUC_MAX!=m_Idx )
	{
		m_Effect->EndTech();
	}	
}

void BbDeviceEffectTech::Flush()
{
	if ( BBUC_MAX!=m_Idx )
	{
		m_Effect->FlushTech();
	}	
}

//////////////////////////////////////////////////////////////////////////

BbDeviceEffectSampler::BbDeviceEffectSampler()
	:	m_Effect(nullptr)
	,	m_Idx(BBUC_MAX)
{

}

BbDeviceEffectSampler::BbDeviceEffectSampler( BbDeviceEffect* eft, BBUC idx )
	:	m_Effect(eft)
	,	m_Idx(idx)
{

}

void BbDeviceEffectSampler::Set( BbDevStateSampler* state )
{
	if ( BBUC_MAX!=m_Idx )
	{
		m_Effect->SetStateSampler( m_Idx, state );
	}	
}

//////////////////////////////////////////////////////////////////////////

BbDeviceEffectTex::BbDeviceEffectTex()
	:	m_Effect(nullptr)
	,	m_Idx(BBUC_MAX)
{

}

BbDeviceEffectTex::BbDeviceEffectTex( BbDeviceEffect* eft, BBUC idx )
	:	m_Effect(eft)
	,	m_Idx(idx)
{

}

void BbDeviceEffectTex::Set( BbDevTex1D* tex )
{
	if ( BBUC_MAX!=m_Idx )
	{
		m_Effect->SetTexSrv( m_Idx, static_cast<BbDevSrvItl*>( BbDeviceDataMgr::Get()->GetTex1DItl( tex->Data() ) ) );
	}	
}

void BbDeviceEffectTex::Set( BbDevTex2D* tex )
{
	if ( BBUC_MAX!=m_Idx )
	{
		m_Effect->SetTexSrv( m_Idx, static_cast<BbDevSrvItl*>(BbDeviceDataMgr::Get()->GetTex2DItl( tex->Data() )) );
	}	
}

void BbDeviceEffectTex::Set( BbDevRenderTarget* rt )
{
	if ( BBUC_MAX!=m_Idx )
	{
		m_Effect->SetTexSrv( m_Idx, static_cast<BbDevSrvItl*>(BbDeviceDataMgr::Get()->GetRenderTargetItl( rt->Data() )) );
	}	
}

void BbDeviceEffectTex::Set( BbDevDepthStencil* ds )
{
	if ( BBUC_MAX!=m_Idx )
	{
		m_Effect->SetTexSrv( m_Idx, static_cast<BbDevSrvItl*>(BbDeviceDataMgr::Get()->GetDepthStencilItl( ds->Data() )) );
	}	
}

//////////////////////////////////////////////////////////////////////////

BbDeviceEffectVar::BbDeviceEffectVar()
	:	m_Effect(nullptr)	
	,	m_Size(0)
	,	m_Off(0)
	,	m_Idx(BBUC_MAX)
{

}

BbDeviceEffectVar::BbDeviceEffectVar( BbDeviceEffect* eft, BBUC idx, BBUL size, BBUL off )
	:	m_Effect(eft)	
	,	m_Size(size)
	,	m_Off(off)
	,	m_Idx(idx)
{

}

void BbDeviceEffectVar::SetFloat( float f )
{
	if ( BBUC_MAX!=m_Idx )
	{
		assert( sizeof(float)==m_Size );
		m_Effect->SetCbufVar( m_Idx, &f, sizeof(float), m_Off );
	}
}

void BbDeviceEffectVar::SetFloatArray( float* pf, UINT count )
{
	if ( BBUC_MAX!=m_Idx )
	{
		assert( sizeof(float)*count==m_Size );
		m_Effect->SetCbufVar( m_Idx, pf, m_Size, m_Off );
	}
}

void BbDeviceEffectVar::SetFloat2( const D3DXVECTOR2& f2 )
{
	if ( BBUC_MAX!=m_Idx )
	{
		assert( sizeof(D3DXVECTOR2)==m_Size );
		m_Effect->SetCbufVar( m_Idx, (void*)&f2, sizeof(D3DXVECTOR2), m_Off );
	}
}

void BbDeviceEffectVar::SetFloat3( const D3DXVECTOR3& f3 )
{
	if ( BBUC_MAX!=m_Idx )
	{
		assert( sizeof(D3DXVECTOR3)==m_Size );
		m_Effect->SetCbufVar( m_Idx, (void*)&f3, sizeof(D3DXVECTOR3), m_Off );
	}
}

void BbDeviceEffectVar::SetFloat4( const D3DXVECTOR4& f4 )
{
	if ( BBUC_MAX!=m_Idx )
	{
		assert( sizeof(D3DXVECTOR4)==m_Size );
		m_Effect->SetCbufVar( m_Idx, (void*)&f4, sizeof(D3DXVECTOR4), m_Off );
	}
}

void BbDeviceEffectVar::SetMatrix( const D3DXMATRIX* pMatrix )
{
	if ( BBUC_MAX!=m_Idx )
	{
		assert( sizeof(D3DXMATRIX)==m_Size );
		D3DXMATRIX mat;
		D3DXMatrixTranspose( &mat, pMatrix );
		m_Effect->SetCbufVar( m_Idx, (void*)&mat, sizeof(D3DXMATRIX), m_Off );
	}
}

void BbDeviceEffectVar::SetMatrixSet( const D3DXMATRIX* pMatrix )
{
	if ( BBUC_MAX!=m_Idx )
	{
		assert( sizeof(D3DXMATRIX)==m_Size );
		m_Effect->SetCbufVar( m_Idx, (void*)&pMatrix, sizeof(D3DXMATRIX), m_Off );
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BbDeviceEffect::BbDeviceEffect()
	:	m_ActiveTech(BBUC_MAX)
{

}

BbDeviceEffect::~BbDeviceEffect()
{

}

void BbDeviceEffect::Destroy()
{
	for ( PackVertexArray::iterator iter(m_aVertex.begin()), end(m_aVertex.end()); iter!=end; ++iter )
	{
		BB_SAFE_RELEASE( iter->vs );
		BB_SAFE_RELEASE( iter->il );
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
	for ( SlotBufConstArray::iterator iter(m_aSlotBc.begin()), end(m_aSlotBc.end()); iter!=end; ++iter )
	{
		iter->bc.Destroy();
	}
	for ( SlotSrvArray::iterator iter(m_aSlotSrv.begin()), end(m_aSlotSrv.end()); iter!=end; ++iter )
	{
		if ( nullptr!=iter->srv )
		{
			iter->srv->RefSub();
		}
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
}

void BbDeviceEffect::InitVertex( const BbFileData* data, const char* func, BbDevLayoutDesc* desc )
{
	ID3DBlob* blobShader = nullptr;
	if ( InitCompiler( data, func, "vs_5_0", &blobShader ) )
	{
		PackVertex pv;
		ID3D11Device* device = BbDeviceDataMgr::Get()->Device();

		if( S_OK==device->CreateVertexShader( blobShader->GetBufferPointer(), blobShader->GetBufferSize(), nullptr, &pv.vs ) )
		{
			if ( S_OK==device->CreateInputLayout( desc->Buf(), desc->Count(), blobShader->GetBufferPointer(), blobShader->GetBufferSize(), &pv.il ) )
			{
				OnReflect( &pv, blobShader );		
				pv.name = func;
				pv.namehash = func;
				m_aVertex.push_back( pv );
			}
			else
			{
				BB_SAFE_RELEASE(pv.vs);
			}
		}
		BB_SAFE_RELEASE( blobShader );
	}
}

void BbDeviceEffect::InitPixel( const BbFileData* data, const char* func )
{
	ID3DBlob* blobShader = nullptr;
	if ( InitCompiler( data, func, "ps_5_0", &blobShader ) )
	{
		PackPixel pp;
		ID3D11Device* device = BbDeviceDataMgr::Get()->Device();

		if ( S_OK==device->CreatePixelShader( blobShader->GetBufferPointer(), blobShader->GetBufferSize(), nullptr, &pp.ps ) )
		{
			OnReflect( &pp, blobShader );
			pp.name = func;
			pp.namehash = func;
			m_aPixel.push_back( pp );
		}
		BB_SAFE_RELEASE( blobShader );
	}
}

void BbDeviceEffect::InitHull( const BbFileData* data, const char* func )
{
	ID3DBlob* blobShader = nullptr;
	if ( InitCompiler( data, func, "hs_5_0", &blobShader ) )
	{
		PackHull ph;
		ID3D11Device* device = BbDeviceDataMgr::Get()->Device();

		if ( S_OK==device->CreateHullShader( blobShader->GetBufferPointer(), blobShader->GetBufferSize(), nullptr, &ph.hs ) )
		{
			OnReflect( &ph, blobShader );
			ph.name = func;
			ph.namehash = func;
			m_aHull.push_back( ph );
		}
		BB_SAFE_RELEASE( blobShader );
	}
}

void BbDeviceEffect::InitDomain( const BbFileData* data, const char* func )
{
	ID3DBlob* blobShader = nullptr;
	if ( InitCompiler( data, func, "ds_5_0", &blobShader ) )
	{
		PackDomain pd;
		ID3D11Device* device = BbDeviceDataMgr::Get()->Device();

		if ( S_OK==device->CreateDomainShader( blobShader->GetBufferPointer(), blobShader->GetBufferSize(), nullptr, &pd.ds ) )
		{
			OnReflect( &pd, blobShader );
			pd.name = func;
			pd.namehash = func;
			m_aDomain.push_back( pd );
		}
		BB_SAFE_RELEASE( blobShader );
	}
}

void BbDeviceEffect::InitGeometry( const BbFileData* data, const char* func )
{
	ID3DBlob* blobShader = nullptr;
	if ( InitCompiler( data, func, "gs_5_0", &blobShader ) )
	{
		PackGeometry pg;
		ID3D11Device* device = BbDeviceDataMgr::Get()->Device();

		if ( S_OK==device->CreateGeometryShader( blobShader->GetBufferPointer(), blobShader->GetBufferSize(), nullptr, &pg.gs ) )
		{
			OnReflect( &pg, blobShader );
			pg.name = func;
			pg.namehash = func;
			m_aGeometry.push_back( pg );
		}
		BB_SAFE_RELEASE( blobShader );
	}
}

void BbDeviceEffect::InitTech( const char* name, BbDevStateBlend* sb, BbDevStateDepthStencil* sds, BbDevStateRasterizer* sr,
	const char* vs, const char* ps, const char* hs/* =nullptr */, const char* ds/* =nullptr */, const char*gs/* =nullptr */ )
{	
	PackTech pt;

	pt.name = name;
	pt.namehash = name;
	if ( nullptr!=sb )		pt.stateBlend = *sb;
	if ( nullptr!=sds )		pt.stateDepthStl = *sds;
	if ( nullptr!=sr )		pt.stateRstrz = *sr;
	pt.vs = GetIdxFromArray( &m_aVertex, vs );
	pt.ps = GetIdxFromArray( &m_aPixel, ps );
	if ( nullptr!=hs )		pt.hs = GetIdxFromArray( &m_aHull, hs );
	if ( nullptr!=ds )		pt.ds = GetIdxFromArray( &m_aDomain, ds );
	if ( nullptr!=gs )		pt.gs = GetIdxFromArray( &m_aGeometry, gs );

	if ( BBUC_MAX!=pt.vs && BBUC_MAX!=pt.ps )
	{
		m_aTech.push_back( pt );
	}	
}

//////////////////////////////////////////////////////////////////////////

void BbDeviceEffect::BeginTech( BBUC idx )
{
	BbDeviceDirect* bbdd = BbDeviceDataMgr::Get()->BbdDirect();

	m_ActiveTech = idx;
	PackTech* pt = &m_aTech[idx];
	
	pt->stateBlend.Active();
	pt->stateDepthStl.Active();
	pt->stateRstrz.Active();
	
	PackVertex* pv = &m_aVertex[pt->vs];
	bbdd->ActiveLayout( pv->il );
	bbdd->ActiveVertexShader( pv->vs );
	SetSlotAll( &pv->slot, BB_PIPE_TYPE_VS );
	
	
	PackPixel* pp = &m_aPixel[pt->ps];
	bbdd->ActivePixelShader( pp->ps );
	SetSlotAll( &pp->slot, BB_PIPE_TYPE_PS );
		
	if ( BBUC_MAX==pt->hs )
	{
		PackHull* ph = &m_aHull[pt->hs];
		bbdd->ActiveHullShader( ph->hs );
		SetSlotAll( &ph->slot, BB_PIPE_TYPE_HS );
	}

	if ( BBUC_MAX==pt->ds )
	{
		PackDomain* pd = &m_aDomain[pt->ds];
		bbdd->ActiveDomainShader( pd->ds );
		SetSlotAll( &pd->slot, BB_PIPE_TYPE_DS );
	}

	if ( BBUC_MAX==pt->gs )
	{
		PackGeometry* pg = &m_aGeometry[pt->gs];
		bbdd->ActiveGeometryShader( pg->gs );
		SetSlotAll( &pg->slot, BB_PIPE_TYPE_GS );
	}	
}

void BbDeviceEffect::EndTech()
{
	m_ActiveTech = BBUC_MAX;
}

void BbDeviceEffect::FlushTech()
{
	PackTech* pt = &m_aTech[m_ActiveTech];

	PackVertex* pv = &m_aVertex[pt->vs];
	SetSlotAll( &pv->slot, BB_PIPE_TYPE_VS );


	PackPixel* pp = &m_aPixel[pt->ps];
	SetSlotAll( &pp->slot, BB_PIPE_TYPE_PS );

	if ( BBUC_MAX==pt->hs )
	{
		PackHull* ph = &m_aHull[pt->hs];
		SetSlotAll( &ph->slot, BB_PIPE_TYPE_HS );
	}

	if ( BBUC_MAX==pt->ds )
	{
		PackDomain* pd = &m_aDomain[pt->ds];
		SetSlotAll( &pd->slot, BB_PIPE_TYPE_DS );
	}

	if ( BBUC_MAX==pt->gs )
	{
		PackGeometry* pg = &m_aGeometry[pt->gs];
		SetSlotAll( &pg->slot, BB_PIPE_TYPE_GS );
	}
}

void BbDeviceEffect::SetStateSampler( BBUC idx, BbDevStateSampler* state )
{
	if ( BBUC_MAX!=idx && nullptr!=state )
	{
		SlotSampler* ss = &m_aSlotSam[idx];
		ss->ss = *state;
		ss->update = true;
	}
}

void BbDeviceEffect::SetTexSrv( BBUC idx, BbDevSrvItl* srv )
{
	if ( BBUC_MAX!=idx )
	{
		SlotSrv* ss = &m_aSlotSrv[idx];
		ss->srv = srv;
		ss->update = true;
	}
}

void BbDeviceEffect::SetCbufVar( BBUC idx, void* src, BBUL size, BBUL off )
{
	if ( BBUC_MAX!=idx && nullptr!=src )
	{
		SlotBufConst* sbc = &m_aSlotBc[idx];
		sbc->bc.SetValue( src, size, off );
		sbc->update = true;
	}
}

//////////////////////////////////////////////////////////////////////////

BbDeviceEffectTech BbDeviceEffect::GetTech( const char* name )
{
	return BbDeviceEffectTech( this, GetIdxFromArray( &m_aTech, name ) );
}

BbDeviceEffectSampler BbDeviceEffect::GetSampler( const char* name )
{
	return BbDeviceEffectSampler( this, GetIdxFromArray( &m_aSlotSam, name ) );
}

BbDeviceEffectTex BbDeviceEffect::GetTexture( const char* name )
{
	return BbDeviceEffectTex( this, GetIdxFromArray( &m_aSlotSrv, name ) );
}

BbDeviceEffectVar BbDeviceEffect::GetVariable( const char* name )
{
	BbStringHash strhs( name );
	SlotBufConstVarMap::iterator iter = m_mSlotBcVar.find( strhs );
	return m_mSlotBcVar.end()==iter?BbDeviceEffectVar():BbDeviceEffectVar( this, iter->second.idx, iter->second.size, iter->second.off );
}

//////////////////////////////////////////////////////////////////////////

bool BbDeviceEffect::InitCompiler( const BbFileData* data, const char* func, const char* profile, ID3DBlob** bs )
{
	ID3DBlob* blobError = nullptr;
	HRESULT hr = D3DX11CompileFromMemory( data->Data(), data->Size(), func, nullptr, nullptr, func, profile, BB_COMPILE_SHADER_FLAG1, 0, nullptr, bs, &blobError, nullptr );
	if ( S_OK!=hr )
	{
		OutputDebugStringA( (char*)blobError->GetBufferPointer() );
		BB_SAFE_RELEASE( blobError );
		return false;
	}
	return true;
}

void BbDeviceEffect::OnReflect( PackBase* pb, ID3DBlob* blob )
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
		case D3D10_SIT_CBUFFER:	OnReflectCbuffer( pb, reflection, &sibd );	break;
		case D3D10_SIT_TBUFFER:	break;
		case D3D10_SIT_TEXTURE:	OnReflectTexture( pb, &sibd );				break;
		case D3D10_SIT_SAMPLER:	OnReflectSampler( pb, &sibd );				break;
		default:	break;
		}		
	}
}

void BbDeviceEffect::OnReflectCbuffer( PackBase* pb, ID3D11ShaderReflection* reflection, D3D11_SHADER_INPUT_BIND_DESC* sibd )
{
	BBUC pos = BBUC_MAX;
	SlotBufConst* sbc = nullptr;
	BbStringHash sh(sibd->Name);
	ID3D11ShaderReflectionConstantBuffer* srcb = reflection->GetConstantBufferByName( sibd->Name );
	D3D11_SHADER_BUFFER_DESC sbd;
	D3D11_SHADER_VARIABLE_DESC svd;
	
	for ( BBUC i(0), size(static_cast<BBUC>(m_aSlotBc.size())); i<size; ++i )
	{
		if ( sh==m_aSlotBc[i].namehash )
		{
			pos = i;
			break;
		}
	}

	if ( BBUC_MAX==pos )
	{
		pos = static_cast<BBUC>(m_aSlotBc.size());
		m_aSlotBc.push_back( SlotBufConst() );		
	}	

	srcb->GetDesc( &sbd );
	for ( BBUL n(0); n < sbd.Variables; ++n )
	{
		srcb->GetVariableByIndex(n)->GetDesc( &svd );
		SlotBufConstVar bcvar;
		bcvar.idx = pos;
		bcvar.name = svd.Name;
		bcvar.namehash = svd.Name;
		bcvar.off = svd.StartOffset;
		bcvar.size = svd.Size;
		m_mSlotBcVar[bcvar.namehash] = bcvar;
	}

	sbc = &m_aSlotBc[pos];
	sbc->name = sibd->Name;
	sbc->namehash = sh;
	sbc->bc.Create( sbd.Size );
	sbc->update = false;
	
	PackSlot ps;
	ps.type = D3D_SIT_CBUFFER;
	ps.idx = pos;
	ps.point = static_cast<BBUC>(sibd->BindPoint);
	ps.count = static_cast<BBUC>(sibd->BindCount);
	pb->slot.push_back( ps );
}

void BbDeviceEffect::OnReflectTexture( PackBase* pb, D3D11_SHADER_INPUT_BIND_DESC* sibd )
{
	BBUC pos = BBUC_MAX;
	SlotSrv* ss = nullptr;
	BbStringHash sh(sibd->Name);

	for ( BBUC i(0), size(static_cast<BBUC>(m_aSlotSrv.size())); i<size; ++i )
	{
		if ( sh==m_aSlotSrv[i].namehash )
		{
			pos = i;
			break;
		}
	}

	if ( BBUC_MAX==pos )
	{
		pos = static_cast<BBUC>(m_aSlotSrv.size());
		m_aSlotSrv.push_back( SlotSrv() );
	}

	ss = &m_aSlotSrv[pos];
	ss->name = sibd->Name;
	ss->namehash = sh;
	ss->srv = nullptr;
	ss->update = false;

	PackSlot ps;
	ps.type = D3D10_SIT_TEXTURE;
	ps.idx = pos;
	ps.point = static_cast<BBUC>(sibd->BindPoint);
	ps.count = static_cast<BBUC>(sibd->BindCount);
	pb->slot.push_back( ps );
}

void BbDeviceEffect::OnReflectSampler( PackBase* pb, D3D11_SHADER_INPUT_BIND_DESC* sibd )
{
	BBUC pos = BBUC_MAX;
	SlotSampler* ss = nullptr;
	BbStringHash sh(sibd->Name);

	for ( BBUC i(0), size(static_cast<BBUC>(m_aSlotSam.size())); i<size; ++i )
	{
		if ( sh==m_aSlotSam[i].namehash )
		{
			pos = i;
			break;
		}
	}

	if ( BBUC_MAX==pos )
	{
		pos = static_cast<BBUC>(m_aSlotSam.size());
		m_aSlotSam.push_back( SlotSampler() );
	}

	ss = &m_aSlotSam[pos];
	ss->name = sibd->Name;
	ss->namehash = sh;
	ss->update = false;
	
	PackSlot ps;
	ps.type = D3D_SIT_SAMPLER;
	ps.idx = pos;
	ps.point = static_cast<BBUC>(sibd->BindPoint);
	ps.count = static_cast<BBUC>(sibd->BindCount);
	pb->slot.push_back( ps );
}

void BbDeviceEffect::SetSlotAll( PackSlotArray* psa, BBUC pipe )
{
	BbDeviceDirect* bbdd = BbDeviceDataMgr::Get()->BbdDirect();

	for ( std::vector<PackSlot>::iterator iter(psa->begin()), end(psa->end()); iter!=end; ++iter )
	{
		switch( iter->type )
		{
		case D3D10_SIT_CBUFFER:
			{
				SlotBufConst* bc = &m_aSlotBc[iter->idx];
				bc->bc.Commit();
				bbdd->ActiveBufConst( bc->bc.Buf(), pipe, iter->point );
				bc->update = false;
			}
			break;
		case D3D10_SIT_TBUFFER:	break;
		case D3D10_SIT_TEXTURE:
			{
				SlotSrv* ss = &m_aSlotSrv[iter->idx];
				bbdd->ActiveSrv( ss->srv, pipe, iter->point );
				ss->update = false;
			}
			break;
		case D3D10_SIT_SAMPLER:
			{
				SlotSampler* ss = &m_aSlotSam[iter->idx];
				bbdd->ActiveStateSampler( BbDeviceDataMgr::Get()->GetStateSamplerItl( ss->ss.Data() ), pipe, iter->point );
				ss->update = false;
			}
			break;
		default:	break;
		}
	}
}

void BbDeviceEffect::SetSlotUpdate( PackSlotArray* psa, BBUC pipe )
{
	BbDeviceDirect* bbdd = BbDeviceDataMgr::Get()->BbdDirect();

	for ( std::vector<PackSlot>::iterator iter(psa->begin()), end(psa->end()); iter!=end; ++iter )
	{
		switch( iter->type )
		{
		case D3D10_SIT_CBUFFER:
			{
				SlotBufConst* bc = &m_aSlotBc[iter->idx];
				if ( bc->update )
				{
					bc->bc.Commit();
					bc->update = false;
				}
			}
			break;
		case D3D10_SIT_TBUFFER:	break;
		case D3D10_SIT_TEXTURE:
			{
				SlotSrv* ss = &m_aSlotSrv[iter->idx];
				if ( ss->update )
				{
					bbdd->ActiveSrv( ss->srv, pipe, iter->point );
					ss->update = false;
				}
			}
			break;
		case D3D10_SIT_SAMPLER:
			{
				SlotSampler* ss = &m_aSlotSam[iter->idx];
				if ( ss->update )
				{
					bbdd->ActiveStateSampler( BbDeviceDataMgr::Get()->GetStateSamplerItl( ss->ss.Data() ), pipe, iter->point );
					ss->update = false;
				}
			}
			break;
		default:	break;
		}
	}
}