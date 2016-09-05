#pragma once

#include "DeviceStateInternal.h"
#include "DeviceBufferInternal.h"
#include "DeviceTextureInternal.h"

class BbDeviceDirect;
class BbDevRenderPad;

class BbDeviceDataMgr
{
	template<class T, class T1>
	class DevItlData
	{
	public:
		typedef std::list<T>		List;
		typedef std::vector<T*>		Array;
		typedef std::vector<BBUL>	ArrayIdle;

		T*	At( BBUL idx )
		{
			return BBUL_MAX==idx?&DataDefault:DataArr[idx];			
		}

		BBUL FindByDesc( T1* desc )
		{
			BBUL i(0), size(static_cast<BBUL>(DataArr.size()));
			while( i<size )
			{
				if ( DataArr.at(i)->Equal(desc) )
				{
					return i;
				}
				++i;
			}
			return BBUL_MAX;
		}

		BBUL FindByRhs( T* rhs )
		{
			BBUL i(0), size(static_cast<BBUL>(DataArr.size()));
			while( i<size )
			{
				if ( DataArr.at(i)->Equal(rhs) )
				{
					return i;
				}
				++i;
			}
			return BBUL_MAX;
		}

		void Destroy()
		{
			DataDefault.Destroy();
			DataDefault.RefZero();
			for (List::iterator iter(DataList.begin()), end(DataList.end()); iter != end; ++iter )
			{
				iter->Destroy();
			}
			DataList.clear();
			DataArr.clear();
			DataIdle.clear();
		}

		void DestroyOne( BBUL idx )
		{
			if ( idx!=BBUL_MAX )
			{
				T* a = DataArr[idx];
				if ( BBUS_MAX!=a->RefNum())
				{
					a->Destroy();
					a->RefMax();
					DataIdle.push_back( idx );
				}
			}
		}

		BBUL AddOne()
		{			
			DataList.push_back( T() );
			T* t = &DataList.back();
			t->IdxSet( static_cast<BBUL>( DataArr.size() ) );
			DataArr.push_back( t );			
			return t->IdxGet();
		}

		BBUL ApplyOne()
		{
			BBUL idx = BBUL_MAX;
			if ( DataIdle.empty() )
			{
				idx = AddOne();
			}
			else
			{
				idx = DataIdle.back();
				DataArr[idx]->RefZero();
				DataIdle.pop_back();
			}
			return idx;
		}

		List		DataList;
		Array		DataArr;
		ArrayIdle	DataIdle;
		T			DataDefault;
	};

	typedef DevItlData<BbDevStateBlendItl,D3D11_BLEND_DESC>						ItlStateBlend;
	typedef DevItlData<BbDevStateDepthStencilItl,D3D11_DEPTH_STENCIL_DESC>		ItlStateDepthStencil;
	typedef DevItlData<BbDevStateRasterizerItl,D3D11_RASTERIZER_DESC>			ItlStateRasterizer;
	typedef DevItlData<BbDevStateSamplerItl,D3D11_SAMPLER_DESC>					ItlStateSampler;
	typedef DevItlData<BbDevBufVertexItl,int>			ItlBufVertex;
	typedef DevItlData<BbDevBufIndexItl,int>			ItlBufIndex;
	typedef DevItlData<BbDevTex1DItl,BbWstring>			ItlTex1D;
	typedef DevItlData<BbDevTex2DItl,BbWstring>			ItlTex2D;
	typedef DevItlData<BbDevTex3DItl,BbWstring>			ItlTex3D;
	typedef DevItlData<BbDevRenderTargetItl,int>		ItlRenderTarget;
	typedef DevItlData<BbDevDepthStencilItl,int>		ItlDepthStencil;	
	typedef std::vector<BbDevDataItl*>					ItlDataPtr;

public:
	static BbDeviceDataMgr* Get();
private:
	static BbDeviceDataMgr* s_BbdDataMgr;

public:
	void Init( BbDeviceDirect* direct );
	void Destroy();
	void Update();

	void SetRefNull( BbDevDataItl* data );

	BBUL	CreateStateBlend( D3D11_BLEND_DESC* desc );
	BBUL	CreateStateDepthStencil( D3D11_DEPTH_STENCIL_DESC* desc );
	BBUL	CreateStateRasterizer( D3D11_RASTERIZER_DESC* desc );
	BBUL	CreateStateSampler(D3D11_SAMPLER_DESC* desc);
	BBUL	CreateBufVertex();
	BBUL	CreateBufIndex();
	BBUL	CreateTex1D(const wchar_t* str);
	BBUL	CreateTex1D(const BbDevTextureDesc* desc);
	BBUL	CreateTex2D(const wchar_t* str);	
	BBUL	CreateTex2D(const BbDevTextureDesc* desc);
	BBUL	CreateTex3D();
	BBUL	CreateRenderTarget();
	BBUL	CreateDepthStencil();

	BbDevStateBlendItl*			GetStateBlendItl(BBUL idx);
	BbDevStateDepthStencilItl*	GetStateDepthStencilItl(BBUL idx);
	BbDevStateRasterizerItl*	GetStateRasterizerItl(BBUL idx);
	BbDevStateSamplerItl*		GetStateSamplerItl(BBUL idx);
	BbDevBufVertexItl*			GetBufVertexItl(BBUL idx);
	BbDevBufIndexItl*			GetBufIndexItl(BBUL idx);
	BbDevTex1DItl*				GetTex1DItl(BBUL idx);
	BbDevTex2DItl*				GetTex2DItl(BBUL idx);
	BbDevTex3DItl*				GetTex3DItl(BBUL idx);
	BbDevRenderTargetItl*		GetRenderTargetItl(BBUL idx);
	BbDevDepthStencilItl*		GetDepthStencilItl(BBUL idx);
	void GetRenderPadItl( BbDevRenderPadItl* itl, BbDevRenderPad* pad );

	BbDeviceDirect*			BbdDirect();
	ID3D11Device*			Device();
	ID3D11DeviceContext*	Context();
	IDXGISwapChain*			SwapChain();

private:
	void ClearRefNull();

private:
	ItlStateBlend			m_ItlStateBlend;
	ItlStateDepthStencil	m_ItlStateDepthStencil;
	ItlStateRasterizer		m_ItlStateRasterizer;
	ItlStateSampler			m_ItlStateSampler;
	ItlBufVertex			m_ItlBufVertex;
	ItlBufIndex				m_ItlBufIndex;
	ItlTex1D				m_ItlTex1D;
	ItlTex2D				m_ItlTex2D;
	ItlTex3D				m_ItlTex3D;
	ItlRenderTarget			m_ItlRenderTarget;
	ItlDepthStencil			m_ItlDepthStencil;
	ItlDataPtr				m_ItlDataNull;

private:
	BbDeviceDirect*				m_BbdDirect;
};