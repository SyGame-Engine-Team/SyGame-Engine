/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Concave/ShapeHeightField.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shapes : Concave, Height-Field
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_SHAPE_CONCAVE_SHAPEHEIGHTFIELD_H
#define SCARAB_ENGINE_PHYSICS_SHAPE_CONCAVE_SHAPEHEIGHTFIELD_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Modeling/Surfaces/MeshHeightField.h"

#include "../Shape.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ShapeHeightField class
class ShapeHeightField : public ShapeConcave
{
public:
    ShapeHeightField( MeshHeightField * pMesh );
    virtual ~ShapeHeightField();

    // Shape type
    inline virtual ShapeType GetType() const;

    // Mass properties (in local coordinates)
    virtual Void ComputeMassProperties( Scalar fDensity, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const;

    // Node queries support
    virtual Void QueryNodes( const BVAABox & bvAABox, HNodeQuery * pQuery ) const;

    // Triangle queries support
    virtual Void QueryTriangles( const BVAABox & bvAABox, HTriangleQuery * pQuery ) const;

    // Fast AAB update access
    Void UpdateAAB();

private:
    UInt _RasterizeAABox( const BVAABox & bvAABox, UInt * outMinX, UInt * outMaxX, UInt * outMinY, UInt * outMaxY ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ShapeHeightField.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_SHAPE_CONCAVE_SHAPEHEIGHTFIELD_H


