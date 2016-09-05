#pragma once

#include "DeviceDef.h"


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BbDevTex1D
{
public:
	BB_DEV_SMART_REF_DECL(BbDevTex1D);

	void Create(const wchar_t* str);
	void Create(const BbDevTextureDesc* desc);

	BBUL Data() const;
private:
	BBUL	m_Idx;
};

//////////////////////////////////////////////////////////////////////////

class BbDevTex2D
{
public:
	BB_DEV_SMART_REF_DECL(BbDevTex2D);

	void Create(const wchar_t* str);
	void Create(const BbDevTextureDesc* desc);

	BBUL Data() const;
private:
	BBUL	m_Idx;
};

//////////////////////////////////////////////////////////////////////////

class BbDevTex3D
{
public:
	BB_DEV_SMART_REF_DECL(BbDevTex3D);

	void Create(const BbDevTextureDesc* desc);

	BBUL Data() const;
private:
	BBUL	m_Idx;
};

//////////////////////////////////////////////////////////////////////////

class BbDevRenderTarget
{
public:
	BB_DEV_SMART_REF_DECL(BbDevRenderTarget);
	// Do Not Create Be The Default One
	void Create(const BbDevTextureDesc* desc);

	BBUL Data() const;
private:
	BBUL	m_Idx;
};

//////////////////////////////////////////////////////////////////////////

class BbDevDepthStencil
{
public:
	BB_DEV_SMART_REF_DECL(BbDevDepthStencil);
	// Do Not Create Be The Default One
	void Create(const BbDevTextureDesc* desc);

	BBUL Data() const;
private:
	BBUL	m_Idx;
};

//////////////////////////////////////////////////////////////////////////

class BbDevRenderPad
{
public:
	BbDevRenderPad();

	BbDevRenderPad* Default();
	BbDevRenderPad* Add( const BbDevRenderTarget& rhs );
	BbDevRenderPad* Append( const BbDevRenderTarget& rhs );
	BbDevRenderPad* Add( const BbDevDepthStencil& rhs );

	void Active();
	bool Equal( BbDevRenderPad* rhs );

	BBUL* Data();
	BBUL Count();

private:
	void SetDefault();

private:
	BBUL	m_Idx[9];
	BBUL	m_Count;
};