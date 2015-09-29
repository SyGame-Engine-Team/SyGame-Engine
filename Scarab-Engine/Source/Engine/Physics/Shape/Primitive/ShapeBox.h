/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Primitive/ShapeBox.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shapes : Primitive, Box
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
#ifndef SCARAB_ENGINE_PHYSICS_SHAPE_PRIMITIVE_SHAPEBOX_H
#define SCARAB_ENGINE_PHYSICS_SHAPE_PRIMITIVE_SHAPEBOX_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Modeling/Primitives/MeshBox.h"

#include "../Shape.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ShapeBox class
class ShapeBox : public ShapeConvex
{
public:
    ShapeBox( const Box & vBox );
    ShapeBox( MeshBox * pBoxMesh );
    virtual ~ShapeBox();

    // Shape type
    inline virtual ShapeType GetType() const;

    // Box access
    inline const Box & GetBox() const;

    // Mass properties (in local coordinates)
    virtual Void ComputeMassProperties( Scalar fDensity, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const;

    // V-Clip support
    inline virtual const MeshVoronoiGraph * GetGraph() const;

    // GJK/MPR support
    virtual Void QueryExtremalVertices( const Vector3 & vDirection, UInt * outMax = NULL, UInt * outMin = NULL ) const;
    virtual Void QueryExtremalVertices( const Vector3 & vDirection, Vertex3 * outMax = NULL, Vertex3 * outMin = NULL ) const;

private:
    MeshVoronoiGraph m_hGraph;

    Box m_vBox;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ShapeBox.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_SHAPE_PRIMITIVE_SHAPEBOX_H

