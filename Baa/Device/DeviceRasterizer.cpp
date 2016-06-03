#include "DeviceRasterizer.h"
#include "DeviceDirect.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

void BbDevViewPort::Init( BBS width, BBS height )
{
	m_ViewPort.Width = static_cast< float >( width );
	m_ViewPort.Height = static_cast< float >( height );
	m_ViewPort.MaxDepth = 1;
	m_ViewPort.MinDepth = 0;	
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
}

void BbDevViewPort::Destroy()
{
	if ( Istach() )
	{
		s_BbDd->ActiveViewPort( nullptr );
	}
}

void BbDevViewPort::Active()
{
	s_BbDd->ActiveViewPort( this );
}

D3D11_VIEWPORT* BbDevViewPort::Vp()
{
	return &m_ViewPort;
}