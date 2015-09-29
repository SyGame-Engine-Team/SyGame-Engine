/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Primitive/ShapeTriangle.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shapes : Primitive, Triangle
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
#ifndef SCARAB_ENGINE_PHYSICS_SHAPE_PRIMITIVE_SHAPETRIANGLE_H
#define SCARAB_ENGINE_PHYSICS_SHAPE_PRIMITIVE_SHAPETRIANGLE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Modeling/Primitives/Planar/MeshTriangle.h"

#include "../Shape.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ShapeTriangle class
class ShapeTriangle : public ShapeConvex
{
public:
    ShapeTriangle( const Triangle3 & vTriangle, Scalar fThickness = SHAPE_MARGIN );
    ShapeTriangle( MeshTriangle * pTriangleMesh, Scalar fThickness = SHAPE_MARGIN );
    virtual ~ShapeTriangle();

    // Shape type
    inline virtual ShapeType GetType() const;

    // Triangle access
    inline const Triangle3 & GetTriangle() const;
    inline Scalar GetThickness() const;

    // Mass properties (in local coordinates)
    virtual Void ComputeMassProperties( Scalar fDensity, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const;

    // V-Clip support
    inline virtual const MeshVoronoiGraph * GetGraph() const;

    // GJK/MPR support
    virtual Void QueryExtremalVertices( const Vector3 & vDirection, UInt * outMax = NULL, UInt * outMin = NULL ) const;
    virtual Void QueryExtremalVertices( const Vector3 & vDirection, Vertex3 * outMax = NULL, Vertex3 * outMin = NULL ) const;

private:
    MeshVoronoiGraph m_hGraph;

    Triangle3 m_vTriangle;
    Scalar m_fThickness;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ShapeTriangle.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_SHAPE_PRIMITIVE_SHAPETRIANGLE_H

