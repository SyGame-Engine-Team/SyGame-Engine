/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Concave/ShapeConcaveMesh.cpp
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
// Includes
#include "ShapeConcaveMesh.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ShapeConcaveMesh implementation
ShapeConcaveMesh::ShapeConcaveMesh( const Vertex3 * arrNodes, UInt iNodeCount, const Triangle3 * arrTriangles, UInt iTriangleCount,
                                    TriangleMesh * pMesh ):
    ShapeConcave( pMesh ), m_hNodeTree(), m_hTriangleTree()
{
    // Node & Triangle data
    m_iNodeCount = iNodeCount;

    PhysicsFn->SelectMemory( TEXT("Shapes") );
    m_arrNodes = New NodeLeaf[m_iNodeCount];
    PhysicsFn->UnSelectMemory();

    for( UInt i = 0; i < m_iNodeCount; ++i ) {
        m_arrNodes[i].vNode = Sphere( arrNodes[i], SHAPECONCAVE_NODE_RADIUS );
        m_arrNodes[i].dwNodeID = i;
        m_arrNodes[i].pLeaf = NULL;
    }

    m_hNodeTree.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Shapes") );
    m_hNodeTree.Create();

    _BuildNodeTree();

    m_iTriangleCount = iTriangleCount;

    PhysicsFn->SelectMemory( TEXT("Shapes") );
    m_arrTriangles = New TriangleLeaf[m_iTriangleCount];
    PhysicsFn->UnSelectMemory();

    for( UInt i = 0; i < m_iTriangleCount; ++i ) {
        m_arrTriangles[i].vTriangle = arrTriangles[i];
        m_arrTriangles[i].dwTriangleID = i;
        m_arrTriangles[i].pLeaf = NULL;
    }

    m_hTriangleTree.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Shapes") );
    m_hTriangleTree.Create();

    _BuildTriangleTree();

    // Local AAB
    m_bvLocalAAB = m_hNodeTree.GetRoot()->bvAABox;
}
ShapeConcaveMesh::ShapeConcaveMesh( TriangleMesh * pMesh ):
    ShapeConcave( pMesh ), m_hNodeTree(), m_hTriangleTree()
{
    UInt iVertexSize = pMesh->GetVertexSize();

    UInt iOffset, iSize;

    pMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    Byte * arrPositions = pMesh->GetVB()->GetData() + iOffset;

    UInt iA, iB, iC;
    const Vertex3 *pV, *pA, *pB, *pC;

    // Node & Triangle data
    m_iNodeCount = pMesh->GetVertexCount();

    PhysicsFn->SelectMemory( TEXT("Shapes") );
    m_arrNodes = New NodeLeaf[m_iNodeCount];
    PhysicsFn->UnSelectMemory();

    for( UInt i = 0; i < m_iNodeCount; ++i ) {
        pV = (const Vertex3 *)( arrPositions + (i * iVertexSize) );

        m_arrNodes[i].vNode = Sphere( *pV, SHAPECONCAVE_NODE_RADIUS );
        m_arrNodes[i].dwNodeID = i;
        m_arrNodes[i].pLeaf = NULL;
    }

    m_hNodeTree.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Shapes") );
    m_hNodeTree.Create();

    _BuildNodeTree();

    m_iTriangleCount = pMesh->GetTriangleCount();

    PhysicsFn->SelectMemory( TEXT("Shapes") );
    m_arrTriangles = New TriangleLeaf[m_iTriangleCount];
    PhysicsFn->UnSelectMemory();

    for( UInt i = 0; i < m_iTriangleCount; ++i ) {
        pMesh->GetTriangle( i, iA, iB, iC );
        pA = (const Vertex3 *)( arrPositions + (iA * iVertexSize) );
        pB = (const Vertex3 *)( arrPositions + (iB * iVertexSize) );
        pC = (const Vertex3 *)( arrPositions + (iC * iVertexSize) );

        m_arrTriangles[i].vTriangle.A = *pA;
        m_arrTriangles[i].vTriangle.B = *pB;
        m_arrTriangles[i].vTriangle.C = *pC;
        m_arrTriangles[i].dwTriangleID = i;
        m_arrTriangles[i].pLeaf = NULL;
    }

    m_hTriangleTree.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Shapes") );
    m_hTriangleTree.Create();

    _BuildTriangleTree();

    // Local AAB
    m_bvLocalAAB = m_hNodeTree.GetRoot()->bvAABox;
}
ShapeConcaveMesh::~ShapeConcaveMesh()
{
    m_hTriangleTree.Destroy();
    m_hNodeTree.Destroy();

    PhysicsFn->SelectMemory( TEXT("Shapes") );
    DeleteA( m_arrTriangles );
    DeleteA( m_arrNodes );
    PhysicsFn->UnSelectMemory();
}

Void ShapeConcaveMesh::ComputeMassProperties( Scalar /*fDensity*/, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const
{
    //Assert( false );
    *outMass = 1.0f;
    *outCenterOfMass = Vertex3::Null;
    *outInertiaTensor = Matrix3::Identity;
    /////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////

Void ShapeConcaveMesh::_BuildNodeTree()
{
    NodeLeaf * pNode;
    BVAABox bvNodeBox;

    for( UInt i = 0; i < m_iNodeCount; ++i ) {
        pNode = ( m_arrNodes + i );

        bvNodeBox.Reset( pNode->vNode.Center );
        bvNodeBox.Expand( Vector3(pNode->vNode.Radius, pNode->vNode.Radius, pNode->vNode.Radius) );

        pNode->pLeaf = m_hNodeTree.Insert( bvNodeBox, pNode );
    }
    //m_hNodeTree.OptimizeBottomUp();
}
Void ShapeConcaveMesh::_BuildTriangleTree()
{
    TriangleLeaf * pTriangle;
    BVAABox bvTriangleBox;

    for( UInt i = 0; i < m_iTriangleCount; ++i ) {
        pTriangle = ( m_arrTriangles + i );

        bvTriangleBox.Reset( pTriangle->vTriangle.A );
        bvTriangleBox.Grow( pTriangle->vTriangle.B );
        bvTriangleBox.Grow( pTriangle->vTriangle.C );

        pTriangle->pLeaf = m_hTriangleTree.Insert( bvTriangleBox, pTriangle );
    }
    //m_hTriangleTree.OptimizeBottomUp();
}
