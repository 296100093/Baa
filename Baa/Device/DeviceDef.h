#pragma once

#include "../Core/CoreSpec.h"

#include <d3d11.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")

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
	BB_DEV_DAT_UNKNOWN,
	BB_DEV_DAT_SRV,
	BB_DEV_DAT_STATE_BLEND,
	BB_DEV_DAT_STATE_DEPTH_STENCIL,
	BB_DEV_DAT_STATE_RASTERIZER,
	BB_DEV_DAT_STATE_SAMPLER,
	BB_DEV_DAT_TEXTURE_1D,
	BB_DEV_DAT_TEXTURE_2D,
	BB_DEV_DAT_TEXTURE_3D,
	BB_DEV_DAT_RENDER_TARGET,
	BB_DEV_DAT_DEPTH_STENCIL,
	BB_DEV_DAT_BUFFER_CONST,
	BB_DEV_DAT_BUFFER_VERTEX,
	BB_DEV_DAT_BUFFER_INDEX,
};

enum BbLoadFromTypeE
{
	//BB_LOAD_FROM_MEM,
	BB_LOAD_FROM_RES,
	BB_LOAD_FROM_ZIP,
	BB_LOAD_FROM_FILE,
};

enum BbLayoutFmtTypeE
{
	BB_LF_XYZW_F,
	BB_LF_XYZ_F,
	BB_LF_XY_F,
	BB_LF_X_F,
};

struct BbDevRenderTargetDesc
{
	DXGI_FORMAT t2dfmt;
	DXGI_FORMAT srvfmt;
	BBS	miplevel;
	BBS width;
	BBS height;
};

struct BbDevDepthStencilDesc
{
	DXGI_FORMAT t2dfmt;
	DXGI_FORMAT dsvfmt;
	DXGI_FORMAT srvfmt;	//DXGI_FORMAT_UNKNOWN ²»´´½¨srv
	BBS width;
	BBS height;
};

struct BbDevBufVertexDesc
{
	const void* src;
	BBL stride;
	BBL count;
	bool immutable;
};

struct BbDevBufIndexDesc
{
	const void* src;
	UINT count;
	bool immutable;
	bool i16;
};

#define BB_DEV_SLOT_NULL	0xFF