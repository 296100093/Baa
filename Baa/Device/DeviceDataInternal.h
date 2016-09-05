#pragma once

#include "DeviceDef.h"
#include "DeviceDefInternal.h"


//////////////////////////////////////////////////////////////////////////

class BbDevDataItl
{
public:
	BbDevDataItl();
	void RefAdd();
	void RefSub();
	BBUS RefNum();	
	BbDevDataTypeE Type() const;
	void RefZero();
	void RefMax();

	void IdxSet( BBUL idx );
	BBUL IdxGet();

protected:
	void SetType(BbDevDataTypeE type);

private:
	BbDevDataTypeE	m_Type;
	BBUL			m_Idx;
	BBUS			m_Ref;
};

class BbDevSrvItl : public BbDevDataItl
{
public:
	BbDevSrvItl();
	ID3D11ShaderResourceView*	Srv();

protected:
	ID3D11ShaderResourceView*	m_ShaderResourceView;
};