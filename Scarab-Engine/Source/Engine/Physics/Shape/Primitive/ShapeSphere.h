/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Primitive/ShapeSphere.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shapes : Primitive, Sphere
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
#ifndef SCARAB_ENGINE_PHYSICS_SHAPE_PRIMITIVE_SHAPESPHERE_H
#define SCARAB_ENGINE_PHYSICS_SHAPE_PRIMITIVE_SHAPESPHERE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Modeling/Primitives/MeshSphere.h"

#include "../Shape.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ShapeSphere class
class ShapeSphere : public ShapeConvex
{
public:
    ShapeSphere( const Sphere & vSphere, UInt iSliceSamples, UInt iRadialSamples );
    ShapeSphere( MeshSphere * pSphereMesh );
    virtual ~ShapeSphere();

    // Shape type
    inline virtual ShapeType GetType() const;

    // Sphere access
    inline const Sphere & GetSphere() const;

    // Mass properties (in local coordinates)
    virtual Void ComputeMassProperties( Scalar fDensity, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const;

    // V-Clip support
    inline virtual const MeshVoronoiGraph * GetGraph() const;

    // GJK/MPR support
    virtual Void QueryExtremalVertices( const Vector3 & vDirection, UInt * outMax = NULL, UInt * outMin = NULL ) const;
    virtual Void QueryExtremalVertices( const Vector3 & vDirection, Vertex3 * outMax = NULL, Vertex3 * outMin = NULL ) const;

private:
    Sphere m_vSphere;

    UInt m_iSliceSamples;
    UInt m_iRadialSamples;
    Scalar m_fSliceHeight;
    Scalar m_fRadialAngle;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ShapeSphere.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_SHAPE_PRIMITIVE_SHAPESPHERE_H

