/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1Geometry.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : D2D1 Dev-Ind Resource : Geometries.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Third-Party Includes
#pragma warning(disable:4005)

#define WIN32_LEAN_AND_MEAN
#include <d2d1.h>

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D2D1Geometry.h"

#include "../D2D1RenderingContext.h"

/////////////////////////////////////////////////////////////////////////////////
// D2D1Geometry implementation
D2D1Geometry::D2D1Geometry()
{
    m_pGeometry = NULL;
}
D2D1Geometry::~D2D1Geometry()
{
    // nothing to do
}

Void D2D1Geometry::GetAABRect( D2D1Rectangle * outBounds, const D2D1Matrix32 * pWorldTransform ) const
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pGeometry->GetBounds( (const D2D1_MATRIX_3X2_F *)pWorldTransform, (D2D1_RECT_F*)outBounds );
    DebugAssert( hRes == S_OK );
}
Void D2D1Geometry::GetWidenedAABRect( D2D1Rectangle * outBounds, const D2D1StrokeStyle * pStrokeStyle, Float fStrokeWidth, const D2D1Matrix32 * pWorldTransform, Float fFlatteningTolerance ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( pStrokeStyle->IsCreated() );

    if ( fFlatteningTolerance != 0.0f ) {
        HRESULT hRes = m_pGeometry->GetWidenedBounds( fStrokeWidth, pStrokeStyle->m_pStrokeStyle, (const D2D1_MATRIX_3X2_F *)pWorldTransform, fFlatteningTolerance, (D2D1_RECT_F*)outBounds );
        DebugAssert( hRes == S_OK );
    } else {
        HRESULT hRes = m_pGeometry->GetWidenedBounds( fStrokeWidth, pStrokeStyle->m_pStrokeStyle, (const D2D1_MATRIX_3X2_F *)pWorldTransform, (D2D1_RECT_F*)outBounds );
        DebugAssert( hRes == S_OK );
    }
}

Bool D2D1Geometry::FillContainsPoint( const D2D1Point * pPoint, const D2D1Matrix32 * pWorldTransform, Float fFlatteningTolerance ) const
{
    DebugAssert( IsCreated() );

    BOOL bContained = FALSE;
    if ( fFlatteningTolerance != 0.0f ) {
        HRESULT hRes = m_pGeometry->FillContainsPoint( *( (const D2D1_POINT_2F *)pPoint ), (const D2D1_MATRIX_3X2_F *)pWorldTransform, fFlatteningTolerance, &bContained );
        DebugAssert( hRes == S_OK );
    } else {
        HRESULT hRes = m_pGeometry->FillContainsPoint( *( (const D2D1_POINT_2F *)pPoint ), (const D2D1_MATRIX_3X2_F *)pWorldTransform, &bContained );
        DebugAssert( hRes == S_OK );
    }

    return ( bContained != FALSE );
}
Bool D2D1Geometry::StrokeContainsPoint( const D2D1Point * pPoint, const D2D1StrokeStyle * pStrokeStyle, Float fStrokeWidth, const D2D1Matrix32 * pWorldTransform, Float fFlatteningTolerance ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( pStrokeStyle->IsCreated() );

    BOOL bContained = FALSE;
    if ( fFlatteningTolerance != 0.0f ) {
        HRESULT hRes = m_pGeometry->StrokeContainsPoint( *( (const D2D1_POINT_2F *)pPoint ), fStrokeWidth, pStrokeStyle->m_pStrokeStyle, (const D2D1_MATRIX_3X2_F *)pWorldTransform, fFlatteningTolerance, &bContained );
        DebugAssert( hRes == S_OK );
    } else {
        HRESULT hRes = m_pGeometry->StrokeContainsPoint( *( (const D2D1_POINT_2F *)pPoint ), fStrokeWidth, pStrokeStyle->m_pStrokeStyle, (const D2D1_MATRIX_3X2_F *)pWorldTransform, &bContained );
        DebugAssert( hRes == S_OK );
    }

    return ( bContained != FALSE );
}

Float D2D1Geometry::ComputeLength( const D2D1Matrix32 * pWorldTransform, Float fFlatteningTolerance ) const
{
    DebugAssert( IsCreated() );

    Float fLength = 0.0f;
    if ( fFlatteningTolerance != 0.0f ) {
        HRESULT hRes = m_pGeometry->ComputeLength( (const D2D1_MATRIX_3X2_F *)pWorldTransform, fFlatteningTolerance, &fLength );
        DebugAssert( hRes == S_OK );
    } else {
        HRESULT hRes = m_pGeometry->ComputeLength( (const D2D1_MATRIX_3X2_F *)pWorldTransform, &fLength );
        DebugAssert( hRes == S_OK );
    }

    return fLength;
}
Float D2D1Geometry::ComputeArea( const D2D1Matrix32 * pWorldTransform, Float fFlatteningTolerance ) const
{
    DebugAssert( IsCreated() );

    Float fArea = 0.0f;
    if ( fFlatteningTolerance != 0.0f ) {
        HRESULT hRes = m_pGeometry->ComputeArea( (const D2D1_MATRIX_3X2_F *)pWorldTransform, fFlatteningTolerance, &fArea );
        DebugAssert( hRes == S_OK );
    } else {
        HRESULT hRes = m_pGeometry->ComputeArea( (const D2D1_MATRIX_3X2_F *)pWorldTransform, &fArea );
        DebugAssert( hRes == S_OK );
    }

    return fArea;
}
Void D2D1Geometry::ComputePointAtLength( D2D1Point * outPoint, D2D1Point * outUnitTangent, Float fLength, const D2D1Matrix32 * pWorldTransform, Float fFlatteningTolerance ) const
{
    DebugAssert( IsCreated() );

    if ( fFlatteningTolerance != 0.0f ) {
        HRESULT hRes = m_pGeometry->ComputePointAtLength( fLength, (const D2D1_MATRIX_3X2_F *)pWorldTransform, fFlatteningTolerance, (D2D1_POINT_2F*)outPoint, (D2D1_POINT_2F*)outUnitTangent );
        DebugAssert( hRes == S_OK );
    } else {
        HRESULT hRes = m_pGeometry->ComputePointAtLength( fLength, (const D2D1_MATRIX_3X2_F *)pWorldTransform, (D2D1_POINT_2F*)outPoint, (D2D1_POINT_2F*)outUnitTangent );
        DebugAssert( hRes == S_OK );
    }
}

Void D2D1Geometry::CompareWithGeometry( D2D1GeometryRelation * outRelation, const D2D1Geometry * pInputGeometry, const D2D1Matrix32 * pInputWorldTransform, Float fFlatteningTolerance ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( pInputGeometry->IsCreated() );

    D2D1_GEOMETRY_RELATION iTmpRelation = D2D1_GEOMETRY_RELATION_UNKNOWN;
    if ( fFlatteningTolerance != 0.0f ) {
        HRESULT hRes = m_pGeometry->CompareWithGeometry( pInputGeometry->m_pGeometry, (const D2D1_MATRIX_3X2_F *)pInputWorldTransform, fFlatteningTolerance, &iTmpRelation );
        DebugAssert( hRes == S_OK );
    } else {
        HRESULT hRes = m_pGeometry->CompareWithGeometry( pInputGeometry->m_pGeometry, (const D2D1_MATRIX_3X2_F *)pInputWorldTransform, &iTmpRelation );
        DebugAssert( hRes == S_OK );
    }
    *outRelation = D2D1GeometryRelationFromD2D1[iTmpRelation];
}
Void D2D1Geometry::CombineWithGeometry( D2D1PathGeometry * outGeometry, D2D1GeometryCombineMode iMode, const D2D1Geometry * pInputGeometry, const D2D1Matrix32 * pInputWorldTransform, Float fFlatteningTolerance ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( pInputGeometry->IsCreated() );
    DebugAssert( outGeometry->IsCreated() );
    DebugAssert( outGeometry->IsOpened() );

    if ( fFlatteningTolerance != 0.0f ) {
        HRESULT hRes = m_pGeometry->CombineWithGeometry( pInputGeometry->m_pGeometry, D2D1GeometryCombineModeToD2D1[iMode], (const D2D1_MATRIX_3X2_F *)pInputWorldTransform, fFlatteningTolerance, outGeometry->m_pGeometrySink );
        DebugAssert( hRes == S_OK );
    } else {
        HRESULT hRes = m_pGeometry->CombineWithGeometry( pInputGeometry->m_pGeometry, D2D1GeometryCombineModeToD2D1[iMode], (const D2D1_MATRIX_3X2_F *)pInputWorldTransform, outGeometry->m_pGeometrySink );
        DebugAssert( hRes == S_OK );
    }
}

Void D2D1Geometry::Simplify( D2D1PathGeometry * outGeometry, D2D1GeometrySimplifyOption iOption, const D2D1Matrix32 * pWorldTransform, Float fFlatteningTolerance ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( outGeometry->IsCreated() );
    DebugAssert( outGeometry->IsOpened() );

    if ( fFlatteningTolerance != 0.0f ) {
        HRESULT hRes = m_pGeometry->Simplify( D2D1GeometrySimplifyOptionToD2D1[iOption], (const D2D1_MATRIX_3X2_F *)pWorldTransform, fFlatteningTolerance, outGeometry->m_pGeometrySink );
        DebugAssert( hRes == S_OK );
    } else {
        HRESULT hRes = m_pGeometry->Simplify( D2D1GeometrySimplifyOptionToD2D1[iOption], (const D2D1_MATRIX_3X2_F *)pWorldTransform, outGeometry->m_pGeometrySink );
        DebugAssert( hRes == S_OK );
    }
}
Void D2D1Geometry::Outline( D2D1PathGeometry * outGeometry, const D2D1Matrix32 * pWorldTransform, Float fFlatteningTolerance ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( outGeometry->IsCreated() );
    DebugAssert( outGeometry->IsOpened() );

    if ( fFlatteningTolerance != 0.0f ) {
        HRESULT hRes = m_pGeometry->Outline(  (const D2D1_MATRIX_3X2_F *)pWorldTransform, fFlatteningTolerance, outGeometry->m_pGeometrySink );
        DebugAssert( hRes == S_OK );
    } else {
        HRESULT hRes = m_pGeometry->Outline( (const D2D1_MATRIX_3X2_F *)pWorldTransform, outGeometry->m_pGeometrySink );
        DebugAssert( hRes == S_OK );
    }
}
Void D2D1Geometry::Widen( D2D1PathGeometry * outGeometry, const D2D1StrokeStyle * pStrokeStyle, Float fStrokeWidth, const D2D1Matrix32 * pWorldTransform, Float fFlatteningTolerance ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( pStrokeStyle->IsCreated() );
    DebugAssert( outGeometry->IsCreated() );
    DebugAssert( outGeometry->IsOpened() );

    if ( fFlatteningTolerance != 0.0f ) {
        HRESULT hRes = m_pGeometry->Widen( fStrokeWidth, pStrokeStyle->m_pStrokeStyle, (const D2D1_MATRIX_3X2_F *)pWorldTransform, fFlatteningTolerance, outGeometry->m_pGeometrySink );
        DebugAssert( hRes == S_OK );
    } else {
        HRESULT hRes = m_pGeometry->Widen( fStrokeWidth, pStrokeStyle->m_pStrokeStyle, (const D2D1_MATRIX_3X2_F *)pWorldTransform, outGeometry->m_pGeometrySink );
        DebugAssert( hRes == S_OK );
    }
}

Void D2D1Geometry::Tessellate( D2D1Mesh * outMesh, const D2D1Matrix32 * pWorldTransform, Float fFlatteningTolerance ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( outMesh->IsCreated() );
    DebugAssert( outMesh->IsOpened() );

    if ( fFlatteningTolerance != 0.0f ) {
        HRESULT hRes = m_pGeometry->Tessellate( (const D2D1_MATRIX_3X2_F *)pWorldTransform, fFlatteningTolerance, outMesh->m_pTessellationSink );
        DebugAssert( hRes == S_OK );
    } else {
        HRESULT hRes = m_pGeometry->Tessellate( (const D2D1_MATRIX_3X2_F *)pWorldTransform, outMesh->m_pTessellationSink );
        DebugAssert( hRes == S_OK );
    }
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1RectangleGeometry implementation
D2D1RectangleGeometry::D2D1RectangleGeometry():
    D2D1Geometry()
{
    m_pRectangleGeometry = NULL;
}
D2D1RectangleGeometry::~D2D1RectangleGeometry()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1RectangleGeometry::Create( const D2D1Rectangle * pRectangle )
{
    DebugAssert( !(IsCreated()) );

    m_pRectangleGeometry = NULL;
    HRESULT hRes = D2D1RenderingContext::sm_pD2D1Factory->CreateRectangleGeometry( (const D2D1_RECT_F *)pRectangle, &m_pRectangleGeometry );
    DebugAssert( hRes == S_OK && m_pRectangleGeometry != NULL );

    m_pGeometry = NULL;
    hRes = m_pRectangleGeometry->QueryInterface( __uuidof(ID2D1Geometry), (Void**)&m_pGeometry );
    DebugAssert( hRes == S_OK && m_pGeometry != NULL );
}
Void D2D1RectangleGeometry::Destroy()
{
    DebugAssert( IsCreated() );

    m_pGeometry->Release();
    m_pGeometry = NULL;

    m_pRectangleGeometry->Release();
    m_pRectangleGeometry = NULL;
}

Void D2D1RectangleGeometry::GetRect( D2D1Rectangle * outRectangle ) const
{
    DebugAssert( IsCreated() );

    m_pRectangleGeometry->GetRect( (D2D1_RECT_F*)outRectangle );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1RoundedRectangleGeometry implementation
D2D1RoundedRectangleGeometry::D2D1RoundedRectangleGeometry():
    D2D1Geometry()
{
    m_pRoundedRectangleGeometry = NULL;
}
D2D1RoundedRectangleGeometry::~D2D1RoundedRectangleGeometry()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1RoundedRectangleGeometry::Create( const D2D1RoundedRectangle * pRoundedRectangle )
{
    DebugAssert( !(IsCreated()) );

    m_pRoundedRectangleGeometry = NULL;
    HRESULT hRes = D2D1RenderingContext::sm_pD2D1Factory->CreateRoundedRectangleGeometry( (const D2D1_ROUNDED_RECT *)pRoundedRectangle, &m_pRoundedRectangleGeometry );
    DebugAssert( hRes == S_OK && m_pRoundedRectangleGeometry != NULL );

    m_pGeometry = NULL;
    hRes = m_pRoundedRectangleGeometry->QueryInterface( __uuidof(ID2D1Geometry), (Void**)&m_pGeometry );
    DebugAssert( hRes == S_OK && m_pGeometry != NULL );
}
Void D2D1RoundedRectangleGeometry::Destroy()
{
    DebugAssert( IsCreated() );

    m_pGeometry->Release();
    m_pGeometry = NULL;

    m_pRoundedRectangleGeometry->Release();
    m_pRoundedRectangleGeometry = NULL;
}

Void D2D1RoundedRectangleGeometry::GetRoundedRect( D2D1RoundedRectangle * outRoundedRectangle ) const
{
    DebugAssert( IsCreated() );

    m_pRoundedRectangleGeometry->GetRoundedRect( (D2D1_ROUNDED_RECT*)outRoundedRectangle );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1EllipseGeometry implementation
D2D1EllipseGeometry::D2D1EllipseGeometry():
    D2D1Geometry()
{
    m_pEllipseGeometry = NULL;
}
D2D1EllipseGeometry::~D2D1EllipseGeometry()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1EllipseGeometry::Create( const D2D1Ellipse * pEllipse )
{
    DebugAssert( !(IsCreated()) );

    m_pEllipseGeometry = NULL;
    HRESULT hRes = D2D1RenderingContext::sm_pD2D1Factory->CreateEllipseGeometry( (const D2D1_ELLIPSE *)pEllipse, &m_pEllipseGeometry );
    DebugAssert( hRes == S_OK && m_pEllipseGeometry != NULL );

    m_pGeometry = NULL;
    hRes = m_pEllipseGeometry->QueryInterface( __uuidof(ID2D1Geometry), (Void**)&m_pGeometry );
    DebugAssert( hRes == S_OK && m_pGeometry != NULL );
}
Void D2D1EllipseGeometry::Destroy()
{
    DebugAssert( IsCreated() );

    m_pGeometry->Release();
    m_pGeometry = NULL;

    m_pEllipseGeometry->Release();
    m_pEllipseGeometry = NULL;
}

Void D2D1EllipseGeometry::GetEllipse( D2D1Ellipse * outEllipse ) const
{
    DebugAssert( IsCreated() );

    m_pEllipseGeometry->GetEllipse( (D2D1_ELLIPSE*)outEllipse );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1PathGeometry implementation
D2D1PathGeometry::D2D1PathGeometry():
    D2D1Geometry()
{
    m_pPathGeometry = NULL;

    m_bOpened = false;
    m_bFigureInProgress = false;
    m_pGeometrySink = NULL;
}
D2D1PathGeometry::~D2D1PathGeometry()
{
    if ( IsCreated() ) {
        if ( IsOpened() ) {
            if ( IsFigureInProgress() )
                EndFigure( false );
            Close();
        }
        Destroy();
    }
}

Void D2D1PathGeometry::Create()
{
    DebugAssert( !(IsCreated()) );

    m_pPathGeometry = NULL;
    HRESULT hRes = D2D1RenderingContext::sm_pD2D1Factory->CreatePathGeometry( &m_pPathGeometry );
    DebugAssert( hRes == S_OK && m_pPathGeometry != NULL );

    m_pGeometry = NULL;
    hRes = m_pPathGeometry->QueryInterface( __uuidof(ID2D1Geometry), (Void**)&m_pGeometry );
    DebugAssert( hRes == S_OK && m_pGeometry != NULL );
}
Void D2D1PathGeometry::Destroy()
{
    DebugAssert( IsCreated() );
    DebugAssert( !(IsOpened()) );

    m_pGeometry->Release();
    m_pGeometry = NULL;

    m_pPathGeometry->Release();
    m_pPathGeometry = NULL;
}

UInt D2D1PathGeometry::GetFigureCount() const
{
    DebugAssert( IsCreated() );

    UInt iFigureCount = 0;
    HRESULT hRes = m_pPathGeometry->GetFigureCount( &iFigureCount );
    DebugAssert( hRes == S_OK );

    return iFigureCount;
}
UInt D2D1PathGeometry::GetSegmentCount() const
{
    DebugAssert( IsCreated() );

    UInt iSegmentCount = 0;
    HRESULT hRes = m_pPathGeometry->GetSegmentCount( &iSegmentCount );
    DebugAssert( hRes == S_OK );

    return iSegmentCount;
}

Void D2D1PathGeometry::Open( D2D1GeometryFillMode iFillMode, D2D1GeometrySegmentFlag iSegmentFlags )
{
    DebugAssert( IsCreated() );
    DebugAssert( !(IsOpened()) );

    m_pGeometrySink = NULL;
    HRESULT hRes = m_pPathGeometry->Open( &m_pGeometrySink );
    DebugAssert( hRes == S_OK && m_pGeometrySink != NULL );

    m_pGeometrySink->SetFillMode( D2D1GeometryFillModeToD2D1[iFillMode] );
    m_pGeometrySink->SetSegmentFlags( D2D1GeometrySegmentFlagToD2D1[iSegmentFlags] );

    m_bOpened = true;
}
Void D2D1PathGeometry::Close()
{
    DebugAssert( IsCreated() );
    DebugAssert( IsOpened() );
    DebugAssert( !(IsFigureInProgress()) );
    
    HRESULT hRes = m_pGeometrySink->Close();
    DebugAssert( hRes == S_OK );

    m_pGeometrySink->Release();
    m_pGeometrySink = NULL;

    m_bOpened = false;
}
Void D2D1PathGeometry::SetFillMode( D2D1GeometryFillMode iFillMode )
{
    DebugAssert( IsCreated() );
    DebugAssert( IsOpened() );

    m_pGeometrySink->SetFillMode( D2D1GeometryFillModeToD2D1[iFillMode] );
}
Void D2D1PathGeometry::SetSegmentFlags( D2D1GeometrySegmentFlag iSegmentFlags )
{
    DebugAssert( IsCreated() );
    DebugAssert( IsOpened() );

    m_pGeometrySink->SetSegmentFlags( D2D1GeometrySegmentFlagToD2D1[iSegmentFlags] );
}

Void D2D1PathGeometry::BeginFigure( const D2D1Point * pStart, Bool bIsFilled )
{
    DebugAssert( IsCreated() );
    DebugAssert( IsOpened() );
    DebugAssert( !(IsFigureInProgress()) );

    m_pGeometrySink->BeginFigure( *( (const D2D1_POINT_2F *)pStart ), (bIsFilled) ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW );

    m_bFigureInProgress = true;
}
Void D2D1PathGeometry::EndFigure( Bool bClose )
{
    DebugAssert( IsCreated() );
    DebugAssert( IsOpened() );
    DebugAssert( IsFigureInProgress() );

    m_pGeometrySink->EndFigure( (bClose) ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN );

    m_bFigureInProgress = false;
}

Void D2D1PathGeometry::AddLine( const D2D1Point * pPoint )
{
    DebugAssert( IsCreated() );
    DebugAssert( IsOpened() );
    DebugAssert( IsFigureInProgress() );

    m_pGeometrySink->AddLine( *( (const D2D1_POINT_2F *)pPoint ) );
}
Void D2D1PathGeometry::AddLines( const D2D1Point * arrPoints, UInt iPointCount )
{
    DebugAssert( IsCreated() );
    DebugAssert( IsOpened() );
    DebugAssert( IsFigureInProgress() );

    m_pGeometrySink->AddLines( (const D2D1_POINT_2F *)arrPoints, iPointCount );
}
Void D2D1PathGeometry::AddBezier( const D2D1BezierSegment * pBezierSegment )
{
    DebugAssert( IsCreated() );
    DebugAssert( IsOpened() );
    DebugAssert( IsFigureInProgress() );

    m_pGeometrySink->AddBezier( (const D2D1_BEZIER_SEGMENT *)pBezierSegment );
}
Void D2D1PathGeometry::AddBeziers( const D2D1BezierSegment * arrBezierSegments, UInt iSegmentCount )
{
    DebugAssert( IsCreated() );
    DebugAssert( IsOpened() );
    DebugAssert( IsFigureInProgress() );

    m_pGeometrySink->AddBeziers( (const D2D1_BEZIER_SEGMENT *)arrBezierSegments, iSegmentCount );
}
Void D2D1PathGeometry::AddQuadraticBezier( const D2D1QuadraticBezierSegment * pQuadraticBezierSegment )
{
    DebugAssert( IsCreated() );
    DebugAssert( IsOpened() );
    DebugAssert( IsFigureInProgress() );

    m_pGeometrySink->AddQuadraticBezier( (const D2D1_QUADRATIC_BEZIER_SEGMENT *)pQuadraticBezierSegment );
}
Void D2D1PathGeometry::AddQuadraticBeziers( const D2D1QuadraticBezierSegment * arrQuadraticBezierSegments, UInt iSegmentCount )
{
    DebugAssert( IsCreated() );
    DebugAssert( IsOpened() );
    DebugAssert( IsFigureInProgress() );

    m_pGeometrySink->AddQuadraticBeziers( (const D2D1_QUADRATIC_BEZIER_SEGMENT *)arrQuadraticBezierSegments, iSegmentCount );
}
Void D2D1PathGeometry::AddArc( const D2D1ArcSegment * pArcSegment )
{
    DebugAssert( IsCreated() );
    DebugAssert( IsOpened() );
    DebugAssert( IsFigureInProgress() );

    m_pGeometrySink->AddArc( (const D2D1_ARC_SEGMENT *)pArcSegment );
}

Void D2D1PathGeometry::Copy( D2D1PathGeometry * outGeometry )
{
    DebugAssert( IsCreated() );
    DebugAssert( !(IsOpened()) );
    DebugAssert( outGeometry->IsCreated() );
    DebugAssert( outGeometry->IsOpened() );

    HRESULT hRes = m_pPathGeometry->Stream( outGeometry->m_pGeometrySink );
    DebugAssert( hRes == S_OK );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1TransformedGeometry implementation
D2D1TransformedGeometry::D2D1TransformedGeometry():
    D2D1Geometry()
{
    m_pTransformedGeometry = NULL;
}
D2D1TransformedGeometry::~D2D1TransformedGeometry()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1TransformedGeometry::Create( D2D1Geometry * pSourceGeometry, const D2D1Matrix32 * pTransform )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( pSourceGeometry->IsCreated() );
    
    m_pTransformedGeometry = NULL;
    HRESULT hRes = D2D1RenderingContext::sm_pD2D1Factory->CreateTransformedGeometry( pSourceGeometry->m_pGeometry, (const D2D1_MATRIX_3X2_F *)pTransform, &m_pTransformedGeometry );
    DebugAssert( hRes == S_OK && m_pTransformedGeometry != NULL );

    m_pGeometry = NULL;
    hRes = m_pTransformedGeometry->QueryInterface( __uuidof(ID2D1Geometry), (Void**)&m_pGeometry );
    DebugAssert( hRes == S_OK && m_pGeometry != NULL );
}
Void D2D1TransformedGeometry::Destroy()
{
    DebugAssert( IsCreated() );

    m_pGeometry->Release();
    m_pGeometry = NULL;

    m_pTransformedGeometry->Release();
    m_pTransformedGeometry = NULL;
}

Void D2D1TransformedGeometry::GetTransform( D2D1Matrix32 * outTransform ) const
{
    DebugAssert( IsCreated() );

    m_pTransformedGeometry->GetTransform( (D2D1_MATRIX_3X2_F*)outTransform );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1GeometryGroup implementation
D2D1GeometryGroup::D2D1GeometryGroup():
    D2D1Geometry()
{
    m_pGeometryGroup = NULL;
}
D2D1GeometryGroup::~D2D1GeometryGroup()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1GeometryGroup::Create( D2D1Geometry ** arrSourceGeometries, UInt iSourceGeometryCount, D2D1GeometryFillMode iFillMode )
{
    DebugAssert( !(IsCreated()) );

    ID2D1Geometry ** arrTemp = (ID2D1Geometry**)( SystemFn->MemAlloc( iSourceGeometryCount * sizeof(ID2D1Geometry*) ) );
    for( UInt i = 0; i < iSourceGeometryCount; ++i )
        arrTemp[i] = arrSourceGeometries[i]->m_pGeometry;

    m_pGeometryGroup = NULL;
    HRESULT hRes = D2D1RenderingContext::sm_pD2D1Factory->CreateGeometryGroup( D2D1GeometryFillModeToD2D1[iFillMode], arrTemp, iSourceGeometryCount, &m_pGeometryGroup );
    DebugAssert( hRes == S_OK && m_pGeometryGroup != NULL );

    m_pGeometry = NULL;
    hRes = m_pGeometryGroup->QueryInterface( __uuidof(ID2D1Geometry), (Void**)&m_pGeometry );
    DebugAssert( hRes == S_OK && m_pGeometry != NULL );

    SystemFn->MemFree( arrTemp );
    arrTemp = NULL;
}
Void D2D1GeometryGroup::Destroy()
{
    DebugAssert( IsCreated() );

    m_pGeometry->Release();
    m_pGeometry = NULL;

    m_pGeometryGroup->Release();
    m_pGeometryGroup = NULL;
}

D2D1GeometryFillMode D2D1GeometryGroup::GetFillMode() const
{
    DebugAssert( IsCreated() );

    return D2D1GeometryFillModeFromD2D1[m_pGeometryGroup->GetFillMode()];
}
