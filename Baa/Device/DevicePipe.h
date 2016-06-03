#pragma once

#include "DeviceData.h"


//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

class BbDevTex2D : public BbDevDataSrv
{
public:
	BbDevTex2D();

	void Init( void* src, BbLoadFromTypeE lft );
	void Destroy();
	BbDevDataTypeE Type() const	{ return BB_DEV_DAT_TEXTURE_2D; }

	ID3D11ShaderResourceView*	Srv() const;
private:
	ID3D11Texture2D* m_Texture2D;
	ID3D11ShaderResourceView* m_ShaderResourceView;
};

//////////////////////////////////////////////////////////////////////////

class BbDevBufConst : public BbDevData
{
	struct var
	{
		UINT off;
		UINT size;
		var() : off(0), size(0)	{;}
		var( UINT o, UINT s ) : off(o), size(s)	{;}
	};
	typedef std::vector<var>			VarArray;
	typedef std::map<BbString, UINT>	VarMap;

public:
	BbDevBufConst();

	void Init( ID3D11ShaderReflectionConstantBuffer* buf );
	void Destroy();
	BbDevDataTypeE Type() const	{ return BB_DEV_DAT_BUFFER_CONST; }

	void AttachConst( BBUC pipe, BBUC slot );
	void DetachConst( BBUC pipe, BBUC slot );
	bool IstachConst();
	ID3D11Buffer*		Buf();

	void Commit();
	bool SetValue( const std::string& str, const void* src, UINT size );
	void SetImmutable( const void* src );

private:
	void SetImmutable();

private:
	VarArray		m_aVar;
	VarMap			m_mVar;
	ID3D11Buffer*	m_Buffer;
	void*			m_Data;
	UINT			m_Size;
	BBUC			m_SlotConst[BB_PIPE_TYPE_COUNT];
	BBUC			m_AttachConst;
	bool			m_Update;
	bool			m_Immutable;
};