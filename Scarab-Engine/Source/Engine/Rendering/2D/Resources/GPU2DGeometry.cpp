/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DGeometry.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Geometry
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
#include "GPU2DGeometry.h"

#include "../GPU2DContext.h"
#include "../../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GPU2DGeometry implementation
GPU2DGeometry::GPU2DGeometry()
{
    m_pDevice2DGeometry = NULL;
}
GPU2DGeometry::~GPU2DGeometry()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DRectangleGeometry implementation
GPU2DRectangleGeometry::GPU2DRectangleGeometry():
    GPU2DGeometry(), m_hDevice2DRectangleGeometry()
{
    m_pDevice2DGeometry = &m_hDevice2DRectangleGeometry;
}
GPU2DRectangleGeometry::~GPU2DRectangleGeometry()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DRoundedRectangleGeometry implementation
GPU2DRoundedRectangleGeometry::GPU2DRoundedRectangleGeometry():
    GPU2DGeometry(), m_hDevice2DRoundedRectangleGeometry()
{
    m_pDevice2DGeometry = &m_hDevice2DRoundedRectangleGeometry;
}
GPU2DRoundedRectangleGeometry::~GPU2DRoundedRectangleGeometry()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DEllipseGeometry implementation
GPU2DEllipseGeometry::GPU2DEllipseGeometry():
    GPU2DGeometry(), m_hDevice2DEllipseGeometry()
{
    m_pDevice2DGeometry = &m_hDevice2DEllipseGeometry;
}
GPU2DEllipseGeometry::~GPU2DEllipseGeometry()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DPathGeometry implementation
GPU2DPathGeometry::GPU2DPathGeometry():
    GPU2DGeometry(), m_hDevice2DPathGeometry()
{
    m_pDevice2DGeometry = &m_hDevice2DPathGeometry;
}
GPU2DPathGeometry::~GPU2DPathGeometry()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DTransformedGeometry implementation
GPU2DTransformedGeometry::GPU2DTransformedGeometry():
    GPU2DGeometry(), m_hDevice2DTransformedGeometry()
{
    m_pDevice2DGeometry = &m_hDevice2DTransformedGeometry;

    m_pSourceGeometry = NULL;
}
GPU2DTransformedGeometry::~GPU2DTransformedGeometry()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DGeometryGroup implementation
GPU2DGeometryGroup::GPU2DGeometryGroup( UInt iSourceGeometryCapacity ):
    GPU2DGeometry(), m_hDevice2DGeometryGroup()
{
    Assert( iSourceGeometryCapacity > 1 );

    m_pDevice2DGeometry = &m_hDevice2DGeometryGroup;

    m_iSourceGeometryCapacity = iSourceGeometryCapacity;
    m_iSourceGeometryCount = 0;

    RenderingFn->SelectMemory( TEXT("Scratch") );
    m_arrSourceGeometries = New GPU2DGeometry*[m_iSourceGeometryCapacity];
    RenderingFn->UnSelectMemory();

    m_arrSourceGeometries[0] = NULL;
}
GPU2DGeometryGroup::~GPU2DGeometryGroup()
{
    if ( IsBound() )
        UnBind();

    RenderingFn->SelectMemory( TEXT("Scratch") );
    DeleteA( m_arrSourceGeometries );
    RenderingFn->UnSelectMemory();

    m_arrSourceGeometries = NULL;
}


Void GPU2DGeometryGroup::Bind( GPU2DGeometryFillMode iFillMode )
{
    Assert( !(IsBound()) );
    Assert( m_iSourceGeometryCount > 1 );

    RenderingFn->SelectMemory( TEXT("Scratch") );
    Device2DGeometry ** arrTemp = New Device2DGeometry*[m_iSourceGeometryCount];
    RenderingFn->UnSelectMemory();

    for( UInt i = 0; i < m_iSourceGeometryCount; ++i )
        arrTemp[i] = m_arrSourceGeometries[i]->m_pDevice2DGeometry;

    m_hDevice2DGeometryGroup.Create( arrTemp, m_iSourceGeometryCount, (Device2DGeometryFillMode)iFillMode );

    RenderingFn->SelectMemory( TEXT("Scratch") );
    DeleteA( arrTemp );
    RenderingFn->UnSelectMemory();
}

