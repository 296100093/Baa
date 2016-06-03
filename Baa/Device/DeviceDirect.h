#pragma once

#include "DeviceDef.h"
#include "DeviceInputAssembler.h"
#include "DeviceOutputMerger.h"
#include "DeviceRasterizer.h"
#include "DevicePipe.h"
#include "DeviceDataMgr.h"

class BbDeviceDirect : public BbMod
{
public:
	static unsigned long GetModId();

	void Init();
	bool Init( BBS width, BBS height, HWND hwnd );
	void Destroy();

	void Present();
	void ClearRtv( const BbDevRenderTarget* rt=nullptr, const BbColor color=BbColor(0.0f,0.0f,0.0f,0.0f) );
	void ClearDsv( const BbDevDepthStencil* ds=nullptr, UINT flag=D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, float depth=1.0f, UINT8 stencil=0 );

	void ActiveStateBlend( BbDevStateBlend* state );
	void ActiveStateDepthStencil( BbDevStateDepthStencil* state );
	void ActiveStateRasterizer( BbDevStateRasterizer* state );
	void ActiveViewPort( BbDevViewPort* vp );	
	void ActiveDepthStencil( BbDevDepthStencil* ds );
	void ActiveTopology( D3D11_PRIMITIVE_TOPOLOGY topo );
	void ActiveLayout( BbDevLayout* ly );
	void ActiveBufVertex( BbDevBufVertex* bv, UINT off );
	void ActiveBufIndex( BbDevBufIndex* bi, UINT off );

	void ActiveRenderTarget( BbDevRenderTarget** rt, BBUC count );
	void ActiveStateSampler( BbDevStateSampler* ss, BBUC pipe, BBUC slot );
	void ActiveSrv( BbDevDataSrv* dds, BBUC pipe, BBUC slot );
	void ActiveConst( BbDevBufConst* dbc, BBUC pipe, BBUC slot );
	void DeActiveConst( BBUC pipe, BBUC slot );
	void DeActiveSrv( BBUC pipe, BBUC slot );
	void DeActiveStateSampler( BBUC pipe, BBUC slot );	
	void DeActiveRenderTarget();

	ID3D11Device*			Device() const;
	ID3D11DeviceContext*	Context() const;
	IDXGISwapChain*			SwapChain() const;
	BbDeviceDataMgr*		DataMgr();

private:
	void InitDefault();
	void InitDefaultData();
	void InitDefaultValue();
	void DestroyDefault();
	void DestroyDefaultValue();
	void DestroyDefaultData();

private:
	BbDeviceDataMgr			m_DataMgr;

private:
	BbDevStateBlend			m_DefaultStateBlend;
	BbDevStateDepthStencil	m_DefaultStateDepthStencil;
	BbDevStateRasterizer	m_DefaultStateRasterizer;
	BbDevViewPort			m_DefaultViewPort;
	BbDevRenderTarget		m_DefaultRenderTarget;
	BbDevDepthStencil		m_DefaultDepthStencil;

private:
	BbDevStateSampler*		m_PipeSampler[BB_PIPE_TYPE_COUNT][D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	BbDevDataSrv*			m_PipeSrv[BB_PIPE_TYPE_COUNT][D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	BbDevBufConst*			m_PipeBufConst[BB_PIPE_TYPE_COUNT][D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	BbDevRenderTarget*		m_CurRenderTarget[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];

private:
	BbDevStateBlend*			m_CurStateBlend;
	BbDevStateDepthStencil*		m_CurStateDepthStencil;
	BbDevStateRasterizer*		m_CurStateRasterizer;
	BbDevViewPort*				m_CurViewPort;	
	BbDevDepthStencil*			m_CurDepthStencil;
	BbDevLayout*				m_CurLayout;
	BbDevBufVertex*				m_CurBufVertex;
	BbDevBufIndex*				m_CurBufIndex;
	D3D11_PRIMITIVE_TOPOLOGY	m_CurTopology;

private:
	ID3D11Device*			m_Device;
	ID3D11DeviceContext*	m_Context;
	IDXGISwapChain*			m_SwapChain;
	BBS		m_Width;
	BBS		m_Height;
	bool	m_Init;
};