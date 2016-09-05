#pragma once

#include "DeviceDataInternal.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BbDevTex1DItl : public BbDevSrvItl
{
public:
	BbDevTex1DItl();

	void Create(const wchar_t* str);
	void Create(const BbDevTextureDesc* desc);
	void Destroy();
	bool Equal( BbWstring* str );

	ID3D11Texture1D*			Tex();

private:
	ID3D11Texture1D*			m_Texture1D;
	BbWstring					m_Name;
};

//////////////////////////////////////////////////////////////////////////

class BbDevTex2DItl : public BbDevSrvItl
{
public:
	BbDevTex2DItl();

	void Create(const wchar_t* str);
	void Create(const BbDevTextureDesc* desc);
	void Destroy();
	bool Equal( BbWstring* str );

	ID3D11Texture2D*			Tex();

private:
	ID3D11Texture2D*			m_Texture2D;
	BbWstring					m_Name;
};

//////////////////////////////////////////////////////////////////////////

class BbDevTex3DItl : public BbDevSrvItl
{
public:
	BbDevTex3DItl();

	void Create(const wchar_t* str);
	void Destroy();

	ID3D11Texture3D*			Tex();

private:
	ID3D11Texture3D*			m_Texture3D;
};

//////////////////////////////////////////////////////////////////////////

class BbDevRenderTargetItl : public BbDevSrvItl
{
public:
	BbDevRenderTargetItl();

	void Create( const BbDevTextureDesc* rtd );
	void Create( ID3D11Texture2D* t2d );
	void Destroy();
	void Clear( const BbColor& color );

	ID3D11Texture2D*			Tex();
	ID3D11RenderTargetView*		Rtv();
private:
	ID3D11Texture2D*			m_Texture2D;
	ID3D11RenderTargetView*		m_RenderTarget;
};

//////////////////////////////////////////////////////////////////////////

class BbDevDepthStencilItl : public BbDevSrvItl
{
public:
	BbDevDepthStencilItl();

	void Create( const BbDevTextureDesc* dsd );
	void Destroy();
	void Clear( float depth, BBUC stencil, bool dpt, bool stl );

	ID3D11Texture2D*			Tex();
	ID3D11DepthStencilView*		Dsv();
private:
	ID3D11Texture2D*			m_Texture2D;
	ID3D11DepthStencilView*		m_DepthStencilView;
};

//////////////////////////////////////////////////////////////////////////

class BbDevRenderPadItl
{
public:
	struct PadItlData
	{
		ID3D11RenderTargetView*		Rtv[8];
		BbDevRenderTargetItl*		RenderTargetItl[8];
		BbDevDepthStencilItl*		DepthStencil;	
		BBUL	Count;
	};
public:
	BbDevRenderPadItl();
	void RefAdd();
	void RefSub();
	void BeNull();

	PadItlData	m_Data;
};