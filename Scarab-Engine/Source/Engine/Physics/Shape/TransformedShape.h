/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/TransformedShape.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Transformed collision shape
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
#ifndef SCARAB_ENGINE_PHYSICS_SHAPE_TRANSFORMEDSHAPE_H
#define SCARAB_ENGINE_PHYSICS_SHAPE_TRANSFORMEDSHAPE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Shape.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TransformedShape class
class TransformedShape
{
public:
    TransformedShape( const Shape * pShape, const Transform3 & vTransform );
    virtual ~TransformedShape();

    // Shape & Transform access
    inline ShapeType GetType() const;

    inline Bool IsConvex() const;
    inline Bool IsConcave() const;

    inline const Shape * GetShape() const;
    inline const Transform3 & GetTransform() const;

    inline Void OnShapeUpdate();
    inline Void OnTransformUpdate( const Transform3 & vTransform );

    // AABox access
    inline const BVAABox & GetAAB() const;

protected:
    // AAB update support
    inline Void _ComputeWorldAAB();

    const Shape * m_pShape;
    Transform3 m_vTransform;

    BVAABox m_bvWorldAAB;
};

/////////////////////////////////////////////////////////////////////////////////
// The TransformedShapeConvex class
class TransformedShapeConvex : public TransformedShape
{
public:
    TransformedShapeConvex( const ShapeConvex * pShape, const Transform3 & vTransform );
    virtual ~TransformedShapeConvex();

    // V-Clip/SAT support
    inline const MeshVoronoiGraph * GetGraph() const;

    inline Void GetVertexPosition( Vertex3 * outPosition, const MeshVertex * pVertex ) const;

    inline Void GetFacetNormal( Vector3 * outNormal, const MeshFacet * pFacet ) const;
    inline Void GetFacetPlane( Plane * outPlane, const MeshFacet * pFacet ) const;

    inline UInt GetVoronoiPlaneCount( const MeshFeature * pFeature ) const;
    inline Void GetVoronoiPlane( Plane * outPlane, const MeshFeature * pFeature, UInt iPlane ) const;

    // GJK/MPR support
    Void QueryExtremalVertices( const Vector3 & vDirection, UInt * outMax = NULL, UInt * outMin = NULL ) const;
    Void QueryExtremalVertices( const Vector3 & vDirection, Vertex3 * outMax = NULL, Vertex3 * outMin = NULL ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// The TransformedShapeConcave class
class TransformedShapeConcave : public TransformedShape
{
public:
    TransformedShapeConcave( const ShapeConcave * pShape, const Transform3 & vTransform );
    virtual ~TransformedShapeConcave();

    // Node queries support
    class HTransformedNodeQuery : public ShapeConcave::HNodeQuery { public:
        HTransformedNodeQuery();
        virtual ~HTransformedNodeQuery();

        virtual Void Process( const ShapeConcave::NodeLeaf & nodeLeaf );
        virtual Void Process( const ShapeConcave::NodeLeaf & nodeLeafA, const ShapeConcave::NodeLeaf & nodeLeafB );

        virtual Void ProcessTransformed( const ShapeConcave::NodeLeaf & nodeLeaf );
        virtual Void ProcessTransformed( const ShapeConcave::NodeLeaf & nodeLeafA, const ShapeConcave::NodeLeaf & nodeLeafB );

        const Transform3 * pWorldTransformA;
        const Transform3 * pWorldTransformB;
    };

    Void QueryNodes( const BVAABox & vAABox, HTransformedNodeQuery * pQuery ) const;

    // Triangle queries support
    class HTransformedTriangleQuery : public ShapeConcave::HTriangleQuery { public:
        HTransformedTriangleQuery();
        virtual ~HTransformedTriangleQuery();

        virtual Void Process( const ShapeConcave::TriangleLeaf & triLeaf );
        virtual Void Process( const ShapeConcave::TriangleLeaf & triLeafA, const ShapeConcave::TriangleLeaf & triLeafB );

        virtual Void ProcessTransformed( const ShapeConcave::TriangleLeaf & triLeaf );
        virtual Void ProcessTransformed( const ShapeConcave::TriangleLeaf & triLeafA, const ShapeConcave::TriangleLeaf & triLeafB );

        const Transform3 * pWorldTransformA;
        const Transform3 * pWorldTransformB;
    };

    Void QueryTriangles( const BVAABox & vAABox, HTransformedTriangleQuery * pQuery ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TransformedShape.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_SHAPE_TRANSFORMEDSHAPE_H

