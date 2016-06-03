#pragma once

#include "DeviceState.h"
#include "DeviceTexture.h"
#include "DeviceBuffer.h"

class BbDeviceDirect;

class BbDeviceDataMgr
{
	typedef std::list<BbDevStateBlend>			StateBlendList;
	typedef std::list<BbDevStateDepthStencil>	StateDepthStencil;
	typedef std::list<BbDevStateRasterizer>		StateRasterizer;
	typedef std::list<BbDevStateSampler>		StateSampler;
	typedef std::vector<BbDevData*>				DataPtrArr;

public:
	void Init( BbDeviceDirect* dd );
	void Destroy();
	void Update();

	void SetRefNull( BbDevData* data );
	void ClearRefNull();
	void DestroyStateBlend( BbDevStateBlend* state );
	void DestroyStateDepthStencil( BbDevStateDepthStencil* state );
	void DestroyStateRasterizer( BbDevStateRasterizer* state );
	void DestroyStateSampler(BbDevStateSampler* state);

	BbDevStateBlend*				CreateStateBlend( D3D11_BLEND_DESC* desc );
	BbDevStateDepthStencil*	CreateStateDepthStencil( D3D11_DEPTH_STENCIL_DESC* desc );
	BbDevStateRasterizer*			CreateStateRasterizer( D3D11_RASTERIZER_DESC* desc );
	BbDevStateSampler*			CreateStateSampler(D3D11_SAMPLER_DESC* desc);

	BbDeviceDirect* Device();

private:
	StateBlendList			m_lStateBlend;
	StateDepthStencil	m_lStateDepthStencil;
	StateRasterizer			m_lStateRasterizer;
	StateSampler			m_lStateSampler;
	DataPtrArr				m_aPtrData;

private:
	BbDeviceDirect*		m_Dd;
};