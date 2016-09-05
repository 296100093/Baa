#pragma once

enum BbPipeTypeE
{
	BB_PIPE_TYPE_VS,
	BB_PIPE_TYPE_HS,
	BB_PIPE_TYPE_GS,
	BB_PIPE_TYPE_DS,
	BB_PIPE_TYPE_PS,
	BB_PIPE_TYPE_COUNT,
	BB_PIPE_TYPE_UNKNOWN,
};

enum BbDevDataTypeE
{
	BB_DEV_DATA_UNKNOWN,
	BB_DEV_DATA_STATE_BLEND,
	BB_DEV_DATA_STATE_DEPTH_STENCIL,
	BB_DEV_DATA_STATE_RASTERIZER,
	BB_DEV_DATA_STATE_SAMPLER,
	BB_DEV_DATA_BUFFER_CONST,
	BB_DEV_DATA_BUFFER_VERTEX,
	BB_DEV_DATA_BUFFER_INDEX,
	BB_DEV_DATA_LAYOUT,
	BB_DEV_DATA_TEXTURE_1D,
	BB_DEV_DATA_TEXTURE_2D,
	BB_DEV_DATA_TEXTURE_3D,
	BB_DEV_DATA_RENDER_TARGET,
	BB_DEV_DATA_DEPTH_STENCIL,
};


#define BB_DEV_SMART_REF_IMPL(a,b)	\
	a::~a() {BbDeviceDataMgr::Get()->b(m_Idx)->RefSub();}	\
	a::a() : m_Idx(BBUL_MAX) {}	\
	a::a(const a& rhs) { m_Idx = rhs.m_Idx; BbDeviceDataMgr::Get()->b(m_Idx)->RefAdd(); }	\
	a& a::operator=(const a& rhs) { m_Idx = rhs.m_Idx; BbDeviceDataMgr::Get()->b(m_Idx)->RefAdd(); return *this; }