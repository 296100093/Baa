#pragma once

#include "DeviceState.h"
#include "DeviceTexture.h"
#include "DeviceBuffer.h"

class BbDevSrvItl;
class BbDeviceEffect;

//////////////////////////////////////////////////////////////////////////

class BbDeviceEffectTech
{
public:
	BbDeviceEffectTech();
	BbDeviceEffectTech( BbDeviceEffect* eft, BBUC idx );
	void Begin();
	void End();
	void Flush();

private:
	BbDeviceEffect*	m_Effect;
	BBUC	m_Idx;
};

class BbDeviceEffectSampler
{
public:
	BbDeviceEffectSampler();
	BbDeviceEffectSampler( BbDeviceEffect* eft, BBUC idx );
	void Set( BbDevStateSampler* state );

private:
	BbDeviceEffect*	m_Effect;
	BBUC	m_Idx;
};

class BbDeviceEffectTex
{
public:
	BbDeviceEffectTex();
	BbDeviceEffectTex( BbDeviceEffect* eft, BBUC idx );
	void Set( BbDevTex1D* tex );
	void Set( BbDevTex2D* tex );
	void Set( BbDevRenderTarget* rt );
	void Set( BbDevDepthStencil* ds );

private:
	BbDeviceEffect*	m_Effect;
	BBUC	m_Idx;
};

class BbDeviceEffectVar
{
public:
	BbDeviceEffectVar();
	BbDeviceEffectVar( BbDeviceEffect* eft, BBUC idx, BBUL size, BBUL off );
	void SetFloat( float f );
	void SetFloatArray( float* pf, UINT count );
	void SetFloat2( const D3DXVECTOR2& f2 );
	void SetFloat3( const D3DXVECTOR3& f3 );
	void SetFloat4( const D3DXVECTOR4& f4 );
	void SetMatrix( const D3DXMATRIX* pMatrix );
	void SetMatrixSet( const D3DXMATRIX* pMatrix );

private:
	BbDeviceEffect*	m_Effect;
	BBUL	m_Size;
	BBUL	m_Off;
	BBUC	m_Idx;
};

//////////////////////////////////////////////////////////////////////////

class BbDeviceEffect
{
	struct PackSlot
	{
		D3D_SHADER_INPUT_TYPE type;
		BBUC count;
		BBUC point;
		BBUC idx;
		PackSlot() : type(D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER), count(BBUC_MAX), point(BBUC_MAX), idx(BBUC_MAX) {;}
	};

	typedef std::vector<PackSlot>	PackSlotArray;

	struct PackBase
	{
		BbString name;
		BbStringHash namehash;
		PackSlotArray	slot;
	};

	struct PackVertex : public PackBase
	{
		ID3D11VertexShader* vs;
		ID3D11InputLayout* il;
		PackVertex() : vs(nullptr), il(nullptr) {;}
	};

	struct PackHull : public PackBase
	{
		ID3D11HullShader* hs;
		PackHull() : hs(nullptr) {;}
	};

	struct PackGeometry : public PackBase
	{
		ID3D11GeometryShader* gs;
		PackGeometry() : gs(nullptr) {;}
	};

	struct PackDomain : public PackBase
	{
		ID3D11DomainShader* ds;
		PackDomain() : ds(nullptr) {;}
	};

	struct PackPixel : public PackBase
	{
		ID3D11PixelShader* ps;
		PackPixel() : ps(nullptr) {;}
	};

	struct PackTech
	{
		BbString name;
		BbStringHash namehash;
		BbDevStateBlend stateBlend;
		BbDevStateDepthStencil stateDepthStl;
		BbDevStateRasterizer stateRstrz;
		BBUC	vs, hs, gs, ds, ps;
		PackTech() : vs(BBUC_MAX), hs(BBUC_MAX), gs(BBUC_MAX), ds(BBUC_MAX), ps(BBUC_MAX) {;}
	};

	struct SlotBufConst
	{
		BbString name;
		BbStringHash namehash;
		BbDevBufConst	bc;
		bool update;
	};

	struct SlotSampler
	{
		BbString name;
		BbStringHash namehash;
		BbDevStateSampler ss;
		bool update;
	};

	struct SlotSrv
	{
		BbString name;
		BbStringHash namehash;
		BbDevSrvItl* srv;
		bool update;
	};

	struct SlotBufConstVar
	{
		BbString name;
		BbStringHash namehash;
		BBUL off;
		BBUL size;
		BBUC idx;
	};

	typedef std::vector<PackTech>		PackTechArray;
	typedef std::vector<PackVertex>		PackVertexArray;
	typedef std::vector<PackHull>		PackHullArray;
	typedef std::vector<PackGeometry>	PackGeometryArray;	
	typedef std::vector<PackDomain>		PackDomainArray;
	typedef std::vector<PackPixel>		PackPixelArray;
	
	typedef std::vector<SlotBufConst>	SlotBufConstArray;
	typedef std::vector<SlotSampler>	SlotSamplerArray;
	typedef std::vector<SlotSrv>		SlotSrvArray;

	typedef std::map<BbStringHash,SlotBufConstVar>	SlotBufConstVarMap;

public:
	BbDeviceEffect();
	~BbDeviceEffect();

	void Destroy();
	void InitVertex( const BbFileData* data, const char* func, BbDevLayoutDesc* desc );
 	void InitPixel( const BbFileData* data, const char* func );
	void InitHull( const BbFileData* data, const char* func );
	void InitDomain( const BbFileData* data, const char* func );
	void InitGeometry( const BbFileData* data, const char* func );
 	void InitTech( const char* name, BbDevStateBlend* sb, BbDevStateDepthStencil* sds, BbDevStateRasterizer* sr, const char* vs, const char* ps, const char* hs=nullptr, const char* ds=nullptr, const char*gs=nullptr );

	void BeginTech( BBUC idx );
	void EndTech();
	void FlushTech();
	void SetStateSampler( BBUC idx, BbDevStateSampler* state );
	void SetTexSrv( BBUC idx, BbDevSrvItl* srv );
	void SetCbufVar( BBUC idx, void* src, BBUL size, BBUL off );

	BbDeviceEffectTech GetTech( const char* name );
	BbDeviceEffectSampler GetSampler( const char* name );
	BbDeviceEffectTex GetTexture( const char* name );
	BbDeviceEffectVar GetVariable( const char* name );

private:
	bool InitCompiler( const BbFileData* data, const char* func, const char* profile, ID3DBlob** bs );
 	void OnReflect( PackBase* pb, ID3DBlob* blob  );
	void OnReflectCbuffer( PackBase* pb, ID3D11ShaderReflection* reflection, D3D11_SHADER_INPUT_BIND_DESC* sibd );
	void OnReflectTexture( PackBase* pb, D3D11_SHADER_INPUT_BIND_DESC* sibd );
	void OnReflectSampler( PackBase* pb, D3D11_SHADER_INPUT_BIND_DESC* sibd );
	void SetSlotAll( PackSlotArray* psa, BBUC pipe );
	void SetSlotUpdate( PackSlotArray* psa, BBUC pipe );

private:
	PackTechArray			m_aTech;
	PackVertexArray			m_aVertex;
	PackHullArray			m_aHull;
	PackGeometryArray		m_aGeometry;
	PackDomainArray			m_aDomain;
	PackPixelArray			m_aPixel;

	SlotBufConstArray		m_aSlotBc;
	SlotSamplerArray		m_aSlotSam;
	SlotSrvArray			m_aSlotSrv;

	SlotBufConstVarMap		m_mSlotBcVar;

	BBUC					m_ActiveTech;

private:
	template<class T>
	BBUC GetIdxFromArray( const std::vector<T>* arr, const char* name )
	{
		if ( nullptr!=name )
		{
			BbStringHash sh( name );
			for ( BBUC i(0), size(static_cast<BBUC>(arr->size())); i<size; ++i )
			{
				if ( sh==arr->at(i).namehash )
				{
					return i;
				}
			}
		}
		return BBUC_MAX;
	}
};