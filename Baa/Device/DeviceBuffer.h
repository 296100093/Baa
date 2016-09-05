#pragma once

#include "DeviceDef.h"


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class BbDevBufVertex
{
public:
	BB_DEV_SMART_REF_DECL(BbDevBufVertex);

	void	Create( void* data, BBUL stride, BBUL count );
	void	Create( BBUL stride, BBUL count );
	void	Update( void* data, BBUL left, BBUL width );	// by num
private:
	BBUL	m_Idx;
};

//////////////////////////////////////////////////////////////////////////

class BbDevBufIndex
{
public:
	BB_DEV_SMART_REF_DECL(BbDevBufIndex);

	void Create16( void* data, BBUL count );
	void Create32( void* data, BBUL count );
	void Create16( BBUL count );
	void Create32( BBUL count );
	void Update( void* data, BBUL left, BBUL width );	// by num
private:
	BBUL	m_Idx;
};

//////////////////////////////////////////////////////////////////////////

class BbDevBufConst
{
public:
	struct BcstVar
	{
		BbString name;
		BbStringHash namehash;
		BBUL off;
		BBUL size;
		BcstVar() : off(0), size(0)	{;}
	};	
	typedef std::vector<BcstVar>	BcstVarArray;

public:
	BbDevBufConst();

	void Create( BBUL size );
	void Destroy();

	void Commit();
	void SetValue( const void* src, BBUL size, BBUL off );

	ID3D11Buffer*		Buf();

public:
	BcstVarArray	m_aBcstVar;

private:	
	ID3D11Buffer*	m_Buffer;
	void*			m_Data;
	BBUL			m_Size;
	bool			m_Update;
};

//////////////////////////////////////////////////////////////////////////