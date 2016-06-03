#pragma once

#include "DeviceData.h"


//////////////////////////////////////////////////////////////////////////



class BbDevRenderTarget : public BbDevDataSrv
{
public:
	BbDevRenderTarget();

	void Init( BbDevRenderTargetDesc* rtd );
	void Init( ID3D11Texture2D* t2d );
	void Destroy();
	void Active();
	BbDevDataTypeE Type() const	{ return BB_DEV_DAT_RENDER_TARGET; }

	ID3D11ShaderResourceView*	Srv() const;
	ID3D11RenderTargetView*		Rtv() const;
private:
	ID3D11Texture2D* m_Texture2D;
	ID3D11ShaderResourceView* m_ShaderResourceView;
	ID3D11RenderTargetView* m_RenderTarget;
};

//////////////////////////////////////////////////////////////////////////

class BbDevDepthStencil : public BbDevData
{
public:
	BbDevDepthStencil();

	void Init( BbDevDepthStencilDesc* dsd );
	void Destroy();
	void Active();
	BbDevDataTypeE Type() const	{ return BB_DEV_DAT_DEPTH_STENCIL; }

	ID3D11ShaderResourceView*	Srv() const;
	ID3D11DepthStencilView*		Dsv() const;
private:
	ID3D11Texture2D* m_Texture2D;
	ID3D11ShaderResourceView* m_ShaderResourceView;
	ID3D11DepthStencilView* m_DepthStencilView;
};