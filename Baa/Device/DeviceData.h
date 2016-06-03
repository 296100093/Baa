#pragma once

#include "DeviceDef.h"

class BbDeviceDataMgr;

//////////////////////////////////////////////////////////////////////////

class BbDevData
{
public:
	static void InitMgr( BbDeviceDataMgr* mgr );
	static void DestroyMgr();

protected:	
	static BbDeviceDataMgr*	s_BbDdMgr;
	static bool s_Run;

public:
	BbDevData();
	void RefAdd();
	void Release();	//RefSub
	BBUC RefNum();
	BbDevDataTypeE Type() const;

protected:
	BbDevDataTypeE	m_Type;

private:	
	BBUC			m_Ref;
};

class BbDevSrv : public BbDevData
{
public:
	BbDevSrv();

	void RefSlotAdd( BBUC pipe, BBUC slot );
	void RefSlotSub( BBUC pipe, BBUC slot );
	void RefSlotClear();

	virtual ID3D11ShaderResourceView*	Srv() const = 0;

private:
	BBUC m_SlotSrv[BB_PIPE_TYPE_COUNT];
};