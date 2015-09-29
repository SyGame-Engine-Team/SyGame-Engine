/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DGeometry.h
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
// Header prelude
#ifndef SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DGEOMETRY_H
#define SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DGEOMETRY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GPU2DStrokeStyle.h"
#include "GPU2DMesh.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Geometry types
enum GPU2DGeometryType {
    GPU2DGEOMETRY_RECTANGLE         = DEVICE2DGEOMETRY_RECTANGLE,
    GPU2DGEOMETRY_ROUNDED_RECTANGLE = DEVICE2DGEOMETRY_ROUNDED_RECTANGLE,
    GPU2DGEOMETRY_ELLIPSE           = DEVICE2DGEOMETRY_ELLIPSE,
    GPU2DGEOMETRY_PATH              = DEVICE2DGEOMETRY_PATH,
    GPU2DGEOMETRY_TRANSFORMED       = DEVICE2DGEOMETRY_TRANSFORMED,
    GPU2DGEOMETRY_GROUP             = DEVICE2DGEOMETRY_GROUP
};

// GPU2DGeometry definitions
enum GPU2DGeometryRelation {
    GPU2DGEOMETRY_RELATION_UNKNOWN     = DEVICE2DGEOMETRY_RELATION_UNKNOWN,
    GPU2DGEOMETRY_RELATION_DISJOINT    = DEVICE2DGEOMETRY_RELATION_DISJOINT,
    GPU2DGEOMETRY_RELATION_ISCONTAINED = DEVICE2DGEOMETRY_RELATION_ISCONTAINED,
    GPU2DGEOMETRY_RELATION_CONTAINS    = DEVICE2DGEOMETRY_RELATION_CONTAINS,
    GPU2DGEOMETRY_RELATION_OVERLAP     = DEVICE2DGEOMETRY_RELATION_OVERLAP
};
enum GPU2DGeometryCombineMode {
    GPU2DGEOMETRY_COMBINE_UNION     = DEVICE2DGEOMETRY_COMBINE_UNION,
    GPU2DGEOMETRY_COMBINE_INTERSECT = DEVICE2DGEOMETRY_COMBINE_INTERSECT,
    GPU2DGEOMETRY_COMBINE_EXCLUDE   = DEVICE2DGEOMETRY_COMBINE_EXCLUDE,
    GPU2DGEOMETRY_COMBINE_XOR       = DEVICE2DGEOMETRY_COMBINE_XOR
};
enum GPU2DGeometrySimplifyOption {
    GPU2DGEOMETRY_SIMPLIFY_CUBICS_AND_LINES = DEVICE2DGEOMETRY_SIMPLIFY_CUBICS_AND_LINES,
    GPU2DGEOMETRY_SIMPLIFY_LINES_ONLY       = DEVICE2DGEOMETRY_SIMPLIFY_LINES_ONLY
};

// GPU2DRectangleGeometry definitions

// GPU2DRoundedRectangleGeometry definitions
typedef struct _gpu2d_roundedrectangle {
    GPU2DRectangle hRect;
    Float fRadiusX;
    Float fRadiusY;
} GPU2DRoundedRectangle;

// GPU2DEllipseGeometry definitions
typedef struct _gpu2d_ellipse {
    GPU2DPoint hCenter;
    Float fRadiusX;
    Float fRadiusY;
} GPU2DEllipse;

// GPU2DPathGeometry definitions
enum GPU2DGeometryFillMode {
    GPU2DGEOMETRY_FILL_ALTERNATE = DEVICE2DGEOMETRY_FILL_ALTERNATE,
    GPU2DGEOMETRY_FILL_WINDING   = DEVICE2DGEOMETRY_FILL_WINDING
};
enum GPU2DGeometrySegmentFlag {
    GPU2DGEOMETRY_SEGMENT_NONE                  = DEVICE2DGEOMETRY_SEGMENT_NONE,
    GPU2DGEOMETRY_SEGMENT_FORCE_UNSTROKED       = DEVICE2DGEOMETRY_SEGMENT_FORCE_UNSTROKED,
    GPU2DGEOMETRY_SEGMENT_FORCE_ROUND_LINE_JOIN = DEVICE2DGEOMETRY_SEGMENT_FORCE_ROUND_LINE_JOIN
};

typedef struct _gpu2d_bezier_segment {
    GPU2DPoint ptControlPointA;
    GPU2DPoint ptControlPointB;
    GPU2DPoint ptEndPoint;
} GPU2DBezierSegment;

typedef struct _gpu2d_quadratic_bezier_segment {
    GPU2DPoint ptControlPoint;
    GPU2DPoint ptEndPoint;
} GPU2DQuadraticBezierSegment;

enum GPU2DArcSweepDirection {
    GPU2DARC_SWEEPDIR_COUNTER_CLOCKWISE = DEVICE2DARC_SWEEPDIR_COUNTER_CLOCKWISE,
    GPU2DARC_SWEEPDIR_CLOCKWISE         = DEVICE2DARC_SWEEPDIR_CLOCKWISE
};
enum GPU2DArcSize {
    GPU2DARC_SIZE_SMALL = DEVICE2DARC_SIZE_SMALL,
    GPU2DARC_SIZE_LARGE = DEVICE2DARC_SIZE_LARGE
};
typedef struct _gpu2d_arc_segment {
    GPU2DPoint ptEndPoint;
    GPU2DPoint ptRadius;
    Float fRotation;
    GPU2DArcSweepDirection iSweepDirection;
    GPU2DArcSize iArcSize;
} GPU2DArcSegment;

// Prototypes
class GPU2DContext;
class GPU2DFontFace;

class GPU2DGeometry;
class GPU2DPathGeometry;
class GPU2DTransformedGeometry;
class GPU2DGeometryGroup;

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DGeometry class
class GPU2DGeometry
{
protected:
    GPU2DGeometry();
public:
    virtual ~GPU2DGeometry();

    // Binding
    inline Bool IsBound() const;

    inline Void UnBind();

    // Getters
    inline GPU2DGeometryType GetType() const;

    // Methods
    inline Void GetAABRect( GPU2DRectangle * outBounds, const GPU2DMatrix32 * pWorldTransform = NULL ) const;
    inline Void GetWidenedAABRect( GPU2DRectangle * outBounds, const GPU2DStrokeStyle * pStrokeStyle, Float fStrokeWidth, const GPU2DMatrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;

    inline Bool FillContainsPoint( const GPU2DPoint * pPoint, const GPU2DMatrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;
    inline Bool StrokeContainsPoint( const GPU2DPoint * pPoint, const GPU2DStrokeStyle * pStrokeStyle, Float fStrokeWidth, const GPU2DMatrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;

    inline Float ComputeLength( const GPU2DMatrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;
    inline Float ComputeArea( const GPU2DMatrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;
    inline Void ComputePointAtLength( GPU2DPoint * outPoint, GPU2DPoint * outUnitTangent, Float fLength, const GPU2DMatrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;

    inline Void CompareWithGeometry( GPU2DGeometryRelation * outRelation, const GPU2DGeometry * pInputGeometry, const GPU2DMatrix32 * pInputWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;
    inline Void CombineWithGeometry( GPU2DPathGeometry * outGeometry, GPU2DGeometryCombineMode iMode, const GPU2DGeometry * pInputGeometry, const GPU2DMatrix32 * pInputWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;
    
    inline Void Simplify( GPU2DPathGeometry * outGeometry, GPU2DGeometrySimplifyOption iOption, const GPU2DMatrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;
    inline Void Outline( GPU2DPathGeometry * outGeometry, const GPU2DMatrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;
    inline Void Widen( GPU2DPathGeometry * outGeometry, const GPU2DStrokeStyle * pStrokeStyle, Float fStrokeWidth, const GPU2DMatrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;

    inline Void Tessellate( GPU2DMesh * outMesh, const GPU2DMatrix32 * pWorldTransform = NULL, Float fFlatteningTolerance = 0.0f ) const;

protected:
    friend class GPU2DContext;
    friend class GPU2DTransformedGeometry;
    friend class GPU2DGeometryGroup;

    // Device data
    Device2DGeometry * m_pDevice2DGeometry;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DRectangleGeometry class
class GPU2DRectangleGeometry : public GPU2DGeometry
{
public:
    GPU2DRectangleGeometry();
    virtual ~GPU2DRectangleGeometry();

    // Binding
    inline Void Bind( const GPU2DRectangle * pRectangle );

    // Getters
    inline Void GetRect( GPU2DRectangle * outRectangle ) const;

protected:
    // Device data
    Device2DRectangleGeometry m_hDevice2DRectangleGeometry;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DRoundedRectangleGeometry class
class GPU2DRoundedRectangleGeometry : public GPU2DGeometry
{
public:
    GPU2DRoundedRectangleGeometry();
    virtual ~GPU2DRoundedRectangleGeometry();

    // Binding
    inline Void Bind( const GPU2DRoundedRectangle * pRoundedRectangle );

    // Getters
    inline Void GetRoundedRect( GPU2DRoundedRectangle * outRoundedRectangle ) const;

protected:
    // Device data
    Device2DRoundedRectangleGeometry m_hDevice2DRoundedRectangleGeometry;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DEllipseGeometry class
class GPU2DEllipseGeometry : public GPU2DGeometry
{
public:
    GPU2DEllipseGeometry();
    virtual ~GPU2DEllipseGeometry();

    // Binding
    inline Void Bind( const GPU2DEllipse * pEllipse );

    // Getters
    inline Void GetEllipse( GPU2DEllipse * outEllipse ) const;

protected:
    // Device data
    Device2DEllipseGeometry m_hDevice2DEllipseGeometry;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DPathGeometry class
class GPU2DPathGeometry : public GPU2DGeometry
{
public:
    GPU2DPathGeometry();
    virtual ~GPU2DPathGeometry();

    // Binding
    inline Void Bind();

    // Getters
    inline Bool IsOpened() const;
    inline Bool IsFigureInProgress() const;

    inline UInt GetFigureCount() const;
    inline UInt GetSegmentCount() const;

    // Methods
    inline Void Open( GPU2DGeometryFillMode iFillMode = GPU2DGEOMETRY_FILL_ALTERNATE, GPU2DGeometrySegmentFlag iSegmentFlags = GPU2DGEOMETRY_SEGMENT_NONE );
    inline Void Close();

    inline Void SetFillMode( GPU2DGeometryFillMode iFillMode );
    inline Void SetSegmentFlags( GPU2DGeometrySegmentFlag iSegmentFlags );

    inline Void BeginFigure( const GPU2DPoint * pStart, Bool bIsFilled = false );
    inline Void EndFigure( Bool bClose );

    inline Void AddLine( const GPU2DPoint * pPoint );
    inline Void AddLines( const GPU2DPoint * arrPoints, UInt iPointCount );
    inline Void AddBezier( const GPU2DBezierSegment * pBezierSegment );
    inline Void AddBeziers( const GPU2DBezierSegment * arrBezierSegments, UInt iSegmentCount );
    inline Void AddQuadraticBezier( const GPU2DQuadraticBezierSegment * pQuadraticBezierSegment );
    inline Void AddQuadraticBeziers( const GPU2DQuadraticBezierSegment * arrQuadraticBezierSegments, UInt iSegmentCount );
    inline Void AddArc( const GPU2DArcSegment * pArcSegment );

    inline Void Copy( GPU2DPathGeometry * outGeometry );

protected:
    friend class GPU2DGeometry;
    friend class GPU2DFontFace;

    // Device data
    Device2DPathGeometry m_hDevice2DPathGeometry;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DTransformedGeometry class
class GPU2DTransformedGeometry : public GPU2DGeometry
{
public:
    GPU2DTransformedGeometry();
    virtual ~GPU2DTransformedGeometry();

    // Binding
    inline Void Bind( GPU2DGeometry * pSourceGeometry, const GPU2DMatrix32 * pTransform );

    // Getters
    inline GPU2DGeometry * GetSourceGeometry() const;
    inline Void GetTransform( GPU2DMatrix32 * outTransform ) const;

protected:
    GPU2DGeometry * m_pSourceGeometry;

    // Device data
    Device2DTransformedGeometry m_hDevice2DTransformedGeometry;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DGeometryGroup class
class GPU2DGeometryGroup : public GPU2DGeometry
{
public:
    GPU2DGeometryGroup( UInt iSourceGeometryCapacity );
    virtual ~GPU2DGeometryGroup();

    // Binding
    inline Void Add( GPU2DGeometry * pGeometry );
    Void Bind( GPU2DGeometryFillMode iFillMode );

    // Getters
    inline UInt GetSourceGeometryCapacity() const;
    inline UInt GetSourceGeometryCount() const;
    inline GPU2DGeometry * GetSourceGeometry( UInt iGeometry ) const;
    inline GPU2DGeometry ** GetSourceGeometries() const;

    inline GPU2DGeometryFillMode GetFillMode() const;

protected:
    UInt m_iSourceGeometryCapacity;
    UInt m_iSourceGeometryCount;
    GPU2DGeometry ** m_arrSourceGeometries;

    // Device data
    Device2DGeometryGroup m_hDevice2DGeometryGroup;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPU2DGeometry.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DGEOMETRY_H
