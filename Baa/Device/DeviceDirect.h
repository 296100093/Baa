#pragma once

#include "DeviceDef.h"
#include "DeviceTexture.h"

class BbDeviceDataMgr;
class BbDevStateBlendItl;
class BbDevStateDepthStencilItl;
class BbDevStateRasterizerItl;
class BbDevStateSamplerItl;
class BbDevLayoutItl;
class BbDevBufVertexItl;
class BbDevBufIndexItl;
class BbDevBufConstItl;
class BbDevSrvItl;
class BbDevRenderPadItl;

class BbDeviceDirect : public BbMod
{
public:
	static unsigned long GetModId();

	void Init();
	bool InitDevice( BBUL width, BBUL height, HWND hwnd );
	void ResetDevice( BBUL width, BBUL height );
	void Destroy();

	void Present();

	void ActiveStateBlend( BbDevStateBlendItl* state );
	void ActiveStateDepthStencil( BbDevStateDepthStencilItl* state );
	void ActiveStateRasterizer( BbDevStateRasterizerItl* state );
	void ActiveStateSampler( BbDevStateSamplerItl* ss, BBUC pipe, BBUC slot );

	void ActiveLayout( ID3D11InputLayout* il );
	void ActiveViewPort( D3D11_VIEWPORT* vp );
	void ActiveTopology( D3D11_PRIMITIVE_TOPOLOGY topo );

	void ActiveBufVertex( BbDevBufVertexItl* bv, BBUL off );
	void ActiveBufIndex( BbDevBufIndexItl* bi, BBUL off );
	void ActiveBufConst( ID3D11Buffer* dbc, BBUC pipe, BBUC slot );

	void ActiveRenderPad( BbDevRenderPad* pad );
	void ActiveSrv( BbDevSrvItl* srv, BBUC pipe, BBUC slot );

	void ActiveVertexShader( ID3D11VertexShader* vs );
	void ActivePixelShader( ID3D11PixelShader* ps );
	void ActiveHullShader( ID3D11HullShader* hs );
	void ActiveDomainShader( ID3D11DomainShader* ds );
	void ActiveGeometryShader( ID3D11GeometryShader* gs );

	ID3D11Device*			Device();
	ID3D11DeviceContext*	Context();
	IDXGISwapChain*			SwapChain();
	BbDeviceDataMgr*		DataMgr();

private:
	void InitDefault();
	void DestroyDefault();
	void BeNull();

private:
	BbDeviceDataMgr*		m_DataMgr;

private:
	BbDevStateSamplerItl*	m_PipeSampler[5][D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	BbDevSrvItl*			m_PipeSrv[5][D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	ID3D11Buffer*			m_PipeBufConst[5][D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];	
	BbDevRenderPadItl*		m_RenderPadItl;
	BbDevRenderPad			m_RenderPad;	

private:
	BbDevStateBlendItl*			m_CurStateBlend;
	BbDevStateDepthStencilItl*	m_CurStateDepthStencil;
	BbDevStateRasterizerItl*	m_CurStateRasterizer;	
	BbDevDepthStencil*			m_CurDepthStencil;	
	BbDevBufVertexItl*			m_CurBufVertex;
	BbDevBufIndexItl*			m_CurBufIndex;
	ID3D11InputLayout*			m_CurLayout;
	ID3D11VertexShader*			m_CurVertexShader;
	ID3D11PixelShader*			m_CurPixelShader;
	ID3D11HullShader*			m_CurHullShader;
	ID3D11DomainShader*			m_CurDomainShader;
	ID3D11GeometryShader*		m_CurGeometryShader;
	D3D11_VIEWPORT				m_CurViewPort;
	D3D11_PRIMITIVE_TOPOLOGY	m_CurTopology;

private:
	ID3D11Device*			m_Device;
	ID3D11DeviceContext*	m_Context;
	IDXGISwapChain*			m_SwapChain;
	BBUL	m_Width;
	BBUL	m_Height;
	bool	m_Init;
};