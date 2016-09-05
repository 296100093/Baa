#pragma once

#include "../Core/CoreSpec.h"

#include <d3d11.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include <D3DX10math.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")


// enum BbPipeTypeE
// {
// 	BB_PIPE_TYPE_VS,
// 	BB_PIPE_TYPE_HS,
// 	BB_PIPE_TYPE_GS,
// 	BB_PIPE_TYPE_DS,
// 	BB_PIPE_TYPE_PS,
// 	BB_PIPE_TYPE_COUNT,
// 	BB_PIPE_TYPE_UNKNOWN,
// };

enum BbLayoutFmtTypeE
{
	BB_LF_XYZW_F,
	BB_LF_XYZ_F,
	BB_LF_XY_F,
	BB_LF_X_F,
};

struct BbDevTextureDesc
{
	DXGI_FORMAT fmttex;
	DXGI_FORMAT fmtsrv;
	DXGI_FORMAT fmtdsv;
	DXGI_FORMAT fmtrtv;
	BBUL miplevel;
	BBUL width;
	BBUL height;
	BbDevTextureDesc() : fmttex(DXGI_FORMAT_UNKNOWN), fmtsrv(DXGI_FORMAT_UNKNOWN), fmtdsv(DXGI_FORMAT_UNKNOWN),
		fmtrtv(DXGI_FORMAT_UNKNOWN), miplevel(1), width(0), height(0) {;}
};

//////////////////////////////////////////////////////////////////////////

#define BB_DEV_SMART_REF_DECL(a)	\
	~a();	\
	a();	\
	a(const a& rhs);	\
	a& operator=(const a& rhs);