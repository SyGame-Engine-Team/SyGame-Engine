/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DBrush.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Brushes
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GPU2DBrush.h"

#include "../GPU2DContext.h"

/////////////////////////////////////////////////////////////////////////////////
// GPU2DBrush implementation
GPU2DBrush::GPU2DBrush( GPU2DContext * pContext2D )
{
    m_pContext2D = pContext2D;

    m_pDevice2DBrush = NULL;
}
GPU2DBrush::~GPU2DBrush()
{
    // nothing to do
}

Void GPU2DBrush::UnBind()
{
    Assert( IsBound() );

    m_pContext2D->_UnRegisterBrush( this );

    m_pDevice2DBrush->Destroy();
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DSolidColorBrush implementation
GPU2DSolidColorBrush::GPU2DSolidColorBrush( GPU2DContext * pContext2D ):
    GPU2DBrush( pContext2D ), m_hDevice2DSolidColorBrush( &(pContext2D->m_hDevice2DContext) )
{
    m_pDevice2DBrush = &m_hDevice2DSolidColorBrush;
}
GPU2DSolidColorBrush::~GPU2DSolidColorBrush()
{
    if ( IsBound() )
        UnBind();
}

Void GPU2DSolidColorBrush::Bind()
{
    Assert( !(IsBound()) );

    m_hDevice2DSolidColorBrush.Create();

    m_pContext2D->_RegisterBrush( this );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DGradientBrush implementation
GPU2DGradientBrush::GPU2DGradientBrush( GPU2DContext * pContext2D ):
    GPU2DBrush( pContext2D )
{
    m_pDevice2DGradientBrush = NULL;
}
GPU2DGradientBrush::~GPU2DGradientBrush()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DLinearGradientBrush implementation
GPU2DLinearGradientBrush::GPU2DLinearGradientBrush( GPU2DContext * pContext2D ):
    GPU2DGradientBrush( pContext2D ), m_hDevice2DLinearGradientBrush( &(pContext2D->m_hDevice2DContext) )
{
    m_pDevice2DGradientBrush = &m_hDevice2DLinearGradientBrush;
    m_pDevice2DBrush = &m_hDevice2DLinearGradientBrush;
}
GPU2DLinearGradientBrush::~GPU2DLinearGradientBrush()
{
    if ( IsBound() )
        UnBind();
}

Void GPU2DLinearGradientBrush::Bind( const GPU2DGradientDesc * pGradientDesc )
{
    Assert( !(IsBound()) );

    m_hDevice2DLinearGradientBrush.Create( (const Device2DGradientDesc *)pGradientDesc );

    m_pContext2D->_RegisterBrush( this );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DRadialGradientBrush implementation
GPU2DRadialGradientBrush::GPU2DRadialGradientBrush( GPU2DContext * pContext2D ):
    GPU2DGradientBrush( pContext2D ), m_hDevice2DRadialGradientBrush( &(pContext2D->m_hDevice2DContext) )
{
    m_pDevice2DGradientBrush = &m_hDevice2DRadialGradientBrush;
    m_pDevice2DBrush = &m_hDevice2DRadialGradientBrush;
}
GPU2DRadialGradientBrush::~GPU2DRadialGradientBrush()
{
    if ( IsBound() )
        UnBind();
}

Void GPU2DRadialGradientBrush::Bind( const GPU2DGradientDesc * pGradientDesc )
{
    Assert( !(IsBound()) );

    m_hDevice2DRadialGradientBrush.Create( (const Device2DGradientDesc *)pGradientDesc );

    m_pContext2D->_RegisterBrush( this );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DBitmapBrush implementation
GPU2DBitmapBrush::GPU2DBitmapBrush( GPU2DContext * pContext2D ):
    GPU2DBrush( pContext2D ), m_hDevice2DBitmapBrush( &(pContext2D->m_hDevice2DContext) )
{
    m_pDevice2DBrush = &m_hDevice2DBitmapBrush;

    m_pBitmap = NULL;
}
GPU2DBitmapBrush::~GPU2DBitmapBrush()
{
    if ( IsBound() )
        UnBind();
}

Void GPU2DBitmapBrush::Bind()
{
    Assert( !(IsBound()) );
    Assert( m_pBitmap != NULL && m_pBitmap->IsBound() );

    m_hDevice2DBitmapBrush.Create();

    m_pContext2D->_RegisterBrush( this );
}
