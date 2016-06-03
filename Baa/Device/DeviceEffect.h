#pragma once

#include "DeviceDef.h"
#include "DeviceInputAssembler.h"
#include "DevicePipe.h"

//////////////////////////////////////////////////////////////////////////

class BbDeviceEffect
{
	struct PackBase
	{
		BbString name;
		BbStringHash namehash;
		std::vector<BBUC>	slotbufc;
		std::vector<BBUC>	slotsam;
		std::vector<BBUC>	slotsrv;
	};

	struct PackVertex : public PackBase
	{
		BbDevLayout layout;
		ID3D11VertexShader* vs;
		PackVertex() : vs(nullptr) {;}
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

	struct PackTech : public PackBase
	{
		BbDevLayout layout;
		ID3D11VertexShader* vs;
		ID3D11HullShader* hs;
		ID3D11GeometryShader* gs;
		ID3D11DomainShader* ds;
		ID3D11PixelShader* ps;
		PackTech() : vs(nullptr), hs(nullptr), gs(nullptr), ds(nullptr), ps(nullptr) {;}
	};

	struct SlotBufConst
	{
		BbString name;		
		BbDevBufConst*	bc;
		BBUC slot[BB_PIPE_TYPE_COUNT];
		SlotBufConst()
		{
			bc = nullptr;
			slot[BB_PIPE_TYPE_VS] =
			slot[BB_PIPE_TYPE_HS] =
			slot[BB_PIPE_TYPE_GS] =
			slot[BB_PIPE_TYPE_DS] =
			slot[BB_PIPE_TYPE_PS] = 0xFF;
		}
	};

	struct SlotSampler
	{
		BbString name;
		BbDevStateSampler* ss;
		BBUC slot[BB_PIPE_TYPE_COUNT];
		SlotSampler()
		{
			ss = nullptr;
			slot[BB_PIPE_TYPE_VS] =
			slot[BB_PIPE_TYPE_HS] =
			slot[BB_PIPE_TYPE_GS] =
			slot[BB_PIPE_TYPE_DS] =
			slot[BB_PIPE_TYPE_PS] = 0xFF;
		}
	};

	struct SlotSrv
	{
		BbString name;
		BbDevDataSrv* ds;
		BBUC slot[BB_PIPE_TYPE_COUNT];
		SlotSrv()
		{
			ds = nullptr;
			slot[BB_PIPE_TYPE_VS] =
			slot[BB_PIPE_TYPE_HS] =
			slot[BB_PIPE_TYPE_GS] =
			slot[BB_PIPE_TYPE_DS] =
			slot[BB_PIPE_TYPE_PS] = 0xFF;
		}
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

	typedef std::list<BbDevBufConst>	DevBufConstList;

public:
	BbDeviceEffect();
	~BbDeviceEffect();

	void Destroy();
	void InitVertex( const char* src, BbLoadFromTypeE srctype, const char* func, BbDevLayoutDesc* ld );
 	void InitPixel( const char* src, BbLoadFromTypeE srctype, const char* func );
	void InitHull( const char* src, BbLoadFromTypeE srctype, const char* func );
	void InitDomain( const char* src, BbLoadFromTypeE srctype, const char* func );
	void InitGeometry( const char* src, BbLoadFromTypeE srctype, const char* func );
 	void InitTech( const char* name, const char* vs, const char* ps, const char* hs=NULL, const char* ds=NULL, const char*gs=NULL );
// 	
// 	void BeginEffect( const BbStringHash& name );
// 	void EndEffect();
// 
// 	void SetImmutable( const std::string& name, const void* src );
// 	void SetContentDataInterTech( const std::string& str, ContentData* data );	// 参数赋值 流程太复杂 需要重新考虑
// 
// 	void SetFloat( const std::string& str, float f );
// 	void SetFloatArray( const std::string& str, float* pf, UINT count );
// 	void SetFloat2( const std::string& str, const D3DXVECTOR2& f2 );
// 	void SetFloat3( const std::string& str, const D3DXVECTOR3& f3 );
// 	void SetFloat4( const std::string& str, const D3DXVECTOR4& f4 );
// 	void SetMatrix( const std::string& str, const D3DXMATRIX* pMatrix );
// 	void SetMatrixTranspose( const std::string& str, const D3DXMATRIX* pMatrix );
// 	void SetTexture( const std::string& str, const ContentDataSRV* tex );

private:
	HRESULT InitCompiler( const char* src, BbLoadFromTypeE srctype, const char* func, const char* profile, ID3DBlob** bs, ID3DBlob** be );
	void InitVertexFromBlob( ID3DBlob* pBlob, const char* func, BbDevLayoutDesc* ld );
	//void InitPixelFromBlob( ID3DBlob* pBlob, const std::string& func );
	void OnBlobError( ID3DBlob*& error );
 	void OnReflect( PackBase* pb, BbPipeTypeE type, ID3DBlob* blob  );
// 	void SetValue( const std::string& str, const void* src, UINT size );
// 	UINT GetRealData( const std::string& str );
// 	UINT GetSectionData( const std::string& str );

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

	DevBufConstList			m_lBufConst;

private:
	template<class T>
	BBUC FindArrayPos( const std::vector<T>* arr, const char* str )
	{
		BbStringHash sh(str);
		arr::size_type i(0), size(arr->size());
		while( i<size )
		{
			if ( arr->at(i).namehash == sh )
			{
				break;
			}
			++i;
		}
		return static_cast<BBUC>(i);
	}
	template<class T>
	T* FindArrayPtr( const std::vector<T>* arr, const char* str )
	{
		T* ptr = nullptr;
		if ( nullptr!=str )
		{
			BbStringHash sh(str);
			arr::size_type i(0), size(arr->size());
			while( i<size )
			{
				if ( arr->at(i).namehash == sh )
				{
					ptr = &arr->at(i);
					break;
				}
				++i;
			}			
		}
		return ptr;
	}
};