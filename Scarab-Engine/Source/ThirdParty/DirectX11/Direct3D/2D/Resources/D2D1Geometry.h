/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1Geometry.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1GEOMETRY_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1GEOMETRY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D2D1StrokeStyle.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// D2D1Geometry types
enum D2D1GeometryType {
    D2D1GEOMETRY_RECTANGLE = 0,
    D2D1GEOMETRY_ROUNDED_RECTANGLE,
    D2D1GEOMETRY_ELLIPSE,
    D2D1GEOMETRY_PATH,
    D2D1GEOMETRY_TRANSFORMED,
    D2D1GEOMETRY_GROUP
};

// Prototypes
class D2D1RenderingContext;
class D2D1Mesh;

class D2D1Geometry;
class D2D1PathGeometry;
class D2D1TransformedGeometry;
class D2D1GeometryGroup;

/////////////////////////////////////////////////////////////////////////////////
// The D2D1Geometry class
class D2D1Geometry
{
protected:
    D2D1Geometry();
public:
    virtual ~D2D1Geometry();

    // Deferred construction
    inline Bool IsCreated() const;

    virtual Void Destroy() = 0;

    // Getters
    virtual D2D1GeometryType GetType() const = 0;

    // Methods
    Void GetAABRect( D2D1Rectangle * outBounds, const D2D1Matrix32 * pWorldTransform = NULL ) const;
    Void GetWidenedAABRect( D2D1Rectangle * outBounds, const D2D1StrokeStyle * pStrokeStyle, Float fStrokeWidth, const D2D1Matrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;

    Bool FillContainsPoint( const D2D1Point * pPoint, const D2D1Matrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;
    Bool StrokeContainsPoint( const D2D1Point * pPoint, const D2D1StrokeStyle * pStrokeStyle, Float fStrokeWidth, const D2D1Matrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;

    Float ComputeLength( const D2D1Matrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;
    Float ComputeArea( const D2D1Matrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;
    Void ComputePointAtLength( D2D1Point * outPoint, D2D1Point * outUnitTangent, Float fLength, const D2D1Matrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;

    Void CompareWithGeometry( D2D1GeometryRelation * outRelation, const D2D1Geometry * pInputGeometry, const D2D1Matrix32 * pInputWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;
    Void CombineWithGeometry( D2D1PathGeometry * outGeometry, D2D1GeometryCombineMode iMode, const D2D1Geometry * pInputGeometry, const D2D1Matrix32 * pInputWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;
    
    Void Simplify( D2D1PathGeometry * outGeometry, D2D1GeometrySimplifyOption iOption, const D2D1Matrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;
    Void Outline( D2D1PathGeometry * outGeometry, const D2D1Matrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;
    Void Widen( D2D1PathGeometry * outGeometry, const D2D1StrokeStyle * pStrokeStyle, Float fStrokeWidth, const D2D1Matrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;

    Void Tessellate( D2D1Mesh * outMesh, const D2D1Matrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;

protected:
    friend class D2D1TransformedGeometry;
    friend class D2D1GeometryGroup;
    friend class D2D1RenderingContext;

    ID2D1Geometry * m_pGeometry;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1RectangleGeometry class
class D2D1RectangleGeometry : public D2D1Geometry
{
public:
    D2D1RectangleGeometry();
    virtual ~D2D1RectangleGeometry();

    // Deferred construction
    Void Create( const D2D1Rectangle * pRectangle );
    virtual Void Destroy();

    // Getters
    inline virtual D2D1GeometryType GetType() const;

    Void GetRect( D2D1Rectangle * outRectangle ) const;

protected:
    ID2D1RectangleGeometry * m_pRectangleGeometry;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1RoundedRectangleGeometry class
class D2D1RoundedRectangleGeometry : public D2D1Geometry
{
public:
    D2D1RoundedRectangleGeometry();
    virtual ~D2D1RoundedRectangleGeometry();

    // Deferred construction
    Void Create( const D2D1RoundedRectangle * pRoundedRectangle );
    virtual Void Destroy();

    // Getters
    inline virtual D2D1GeometryType GetType() const;

    Void GetRoundedRect( D2D1RoundedRectangle * outRoundedRectangle ) const;

protected:
    ID2D1RoundedRectangleGeometry * m_pRoundedRectangleGeometry;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1EllipseGeometry class
class D2D1EllipseGeometry : public D2D1Geometry
{
public:
    D2D1EllipseGeometry();
    virtual ~D2D1EllipseGeometry();

    // Deferred construction
    Void Create( const D2D1Ellipse * pEllipse );
    virtual Void Destroy();

    // Getters
    inline virtual D2D1GeometryType GetType() const;

    Void GetEllipse( D2D1Ellipse * outEllipse ) const;

protected:
    ID2D1EllipseGeometry * m_pEllipseGeometry;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1PathGeometry class
class D2D1PathGeometry : public D2D1Geometry
{
public:
    D2D1PathGeometry();
    virtual ~D2D1PathGeometry();

    // Deferred construction
    Void Create();
    virtual Void Destroy();

    // Getters
    inline virtual D2D1GeometryType GetType() const;

    inline Bool IsOpened() const;
    inline Bool IsFigureInProgress() const;

    UInt GetFigureCount() const;
    UInt GetSegmentCount() const;

    // Methods
    Void Open( D2D1GeometryFillMode iFillMode = D2D1GEOMETRY_FILL_ALTERNATE, D2D1GeometrySegmentFlag iSegmentFlags = D2D1GEOMETRY_SEGMENT_NONE );
    Void Close();

    Void SetFillMode( D2D1GeometryFillMode iFillMode );
    Void SetSegmentFlags( D2D1GeometrySegmentFlag iSegmentFlags );

    Void BeginFigure( const D2D1Point * pStart, Bool bIsFilled = false );
    Void EndFigure( Bool bClose );

    Void AddLine( const D2D1Point * pPoint );
    Void AddLines( const D2D1Point * arrPoints, UInt iPointCount );
    Void AddBezier( const D2D1BezierSegment * pBezierSegment );
    Void AddBeziers( const D2D1BezierSegment * arrBezierSegments, UInt iSegmentCount );
    Void AddQuadraticBezier( const D2D1QuadraticBezierSegment * pQuadraticBezierSegment );
    Void AddQuadraticBeziers( const D2D1QuadraticBezierSegment * arrQuadraticBezierSegments, UInt iSegmentCount );
    Void AddArc( const D2D1ArcSegment * pArcSegment );

    Void Copy( D2D1PathGeometry * outGeometry );

protected:
    friend class D2D1FontFace;
    friend class D2D1Geometry;

    ID2D1PathGeometry * m_pPathGeometry;

    Bool m_bOpened;
    Bool m_bFigureInProgress;
    ID2D1GeometrySink * m_pGeometrySink;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1TransformedGeometry class
class D2D1TransformedGeometry : public D2D1Geometry
{
public:
    D2D1TransformedGeometry();
    virtual ~D2D1TransformedGeometry();

    // Deferred construction
    Void Create( D2D1Geometry * pSourceGeometry, const D2D1Matrix32 * pTransform );
    virtual Void Destroy();

    // Getters
    inline virtual D2D1GeometryType GetType() const;

    Void GetTransform( D2D1Matrix32 * outTransform ) const;

protected:
    ID2D1TransformedGeometry * m_pTransformedGeometry;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1GeometryGroup class
class D2D1GeometryGroup : public D2D1Geometry
{
public:
    D2D1GeometryGroup();
    virtual ~D2D1GeometryGroup();

    // Deferred construction
    Void Create( D2D1Geometry ** arrSourceGeometries, UInt iSourceGeometryCount, D2D1GeometryFillMode iFillMode );
    virtual Void Destroy();

    // Getters
    inline virtual D2D1GeometryType GetType() const;

    D2D1GeometryFillMode GetFillMode() const;

protected:
    ID2D1GeometryGroup * m_pGeometryGroup;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D2D1Geometry.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1GEOMETRY_H

