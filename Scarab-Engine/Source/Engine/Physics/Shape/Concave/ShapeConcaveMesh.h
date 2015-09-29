/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Concave/ShapeConcaveMesh.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shapes : Concave, Mesh
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
#ifndef SCARAB_ENGINE_PHYSICS_SHAPE_CONCAVE_SHAPECONCAVEMESH_H
#define SCARAB_ENGINE_PHYSICS_SHAPE_CONCAVE_SHAPECONCAVEMESH_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Math/Geometry/SpacePartitions/AABoxTree.h"

#include "../Shape.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ShapeConcaveMesh class
class ShapeConcaveMesh : public ShapeConcave
{
public:
    ShapeConcaveMesh( const Vertex3 * arrNodes, UInt iNodeCount, const Triangle3 * arrTriangles, UInt iTriangleCount,
                      TriangleMesh * pMesh = NULL );
    ShapeConcaveMesh( TriangleMesh * pMesh );
    virtual ~ShapeConcaveMesh();

    // Shape type
    inline virtual ShapeType GetType() const;

    // Mass properties (in local coordinates)
    virtual Void ComputeMassProperties( Scalar fDensity, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const;

    // Node queries support
    inline virtual Void QueryNodes( const BVAABox & bvAABox, HNodeQuery * pQuery ) const;

    // Triangle queries support
    inline virtual Void QueryTriangles( const BVAABox & bvAABox, HTriangleQuery * pQuery ) const;

private:
    // Node & Triangle data
    Void _BuildNodeTree();
    Void _BuildTriangleTree();

    UInt m_iNodeCount;
    NodeLeaf * m_arrNodes;
    AABoxTree m_hNodeTree;

    UInt m_iTriangleCount;
    TriangleLeaf * m_arrTriangles;
    AABoxTree m_hTriangleTree;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ShapeConcaveMesh.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_SHAPE_CONCAVE_SHAPECONCAVEMESH_H


