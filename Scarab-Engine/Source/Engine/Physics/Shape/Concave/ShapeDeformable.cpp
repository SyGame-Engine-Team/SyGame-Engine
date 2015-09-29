/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Concave/ShapeDeformable.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shapes : Concave, Deformable
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
#include "ShapeDeformable.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ShapeDeformable::Node implementation
ShapeDeformable::Node::Node( const MeshVertexKey & vertexKey ):
    MeshDeformableGraph::Vertex( vertexKey ), Normal()
{
    Normal = Vector3::Null;
    Area = 0.0f;
}
ShapeDeformable::Node::~Node()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ShapeDeformable::Link implementation
ShapeDeformable::Link::Link( const MeshEdgeKey & edgeKey ):
    MeshDeformableGraph::Edge( edgeKey )
{
    // nothing to do
}
ShapeDeformable::Link::~Link()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ShapeDeformable::Facet implementation
ShapeDeformable::Facet::Facet( const MeshTriangleKey & triangleKey ):
    MeshDeformableGraph::Triangle( triangleKey )
{
    // nothing to do
}
ShapeDeformable::Facet::~Facet()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ShapeDeformable implementation
ShapeDeformable::ShapeDeformable( MeshDeformableGraph * pGraph, TriangleMesh * pMesh ):
    ShapeConcave( pMesh ), m_hSelfCollider()
{
    Assert( pGraph != NULL );

    // Graph data
    m_bOwningGraph = false;

    m_pGraph = pGraph;

    m_pNodeTree = m_pGraph->GetVertexTree();
    m_pFacetTree = m_pGraph->GetTriangleTree();

    _UpdateNormalsAreas();

    // Local AAB
    m_bvLocalAAB = m_pNodeTree->GetRoot()->bvAABox;

    // Self collision data
    m_bUseSelfCollisions = true;
}
ShapeDeformable::ShapeDeformable( TriangleMesh * pMesh, MeshManifold3::VertexBuilder pfNodeBuilder,
                                                        MeshManifold3::EdgeBuilder pfLinkBuilder,
                                                        MeshManifold3::FacetBuilder pfFacetBuilder ):
    ShapeConcave( pMesh ), m_hSelfCollider()
{
    Assert( pMesh != NULL );

    // Graph data
    m_bOwningGraph = true;

    PhysicsFn->SelectMemory( TEXT("Shapes") );
    m_pGraph = New MeshDeformableGraph( pfNodeBuilder, pfLinkBuilder, pfFacetBuilder );
    PhysicsFn->UnSelectMemory();

    m_pGraph->UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Shapes") );
    m_pGraph->Create();

    UInt iTriangleCount = pMesh->GetTriangleCount();
    UInt iA, iB, iC;
    for( UInt i = 0; i < iTriangleCount; ++i ) {
        pMesh->GetTriangle( i, iA, iB, iC );
        m_pGraph->CreateFacet( MeshTriangleKey(iA, iB, iC) );
    }

    m_pGraph->SortVertexLinkage();
    m_pGraph->SortEdgeLinkage();

    UInt iVertexSize = pMesh->GetVertexSize();

    UInt iOffset, iSize;

    pMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    Byte * arrPositions = pMesh->GetVB()->GetData() + iOffset;

    m_pGraph->ComputeFeatureData( arrPositions, iVertexSize, SHAPECONCAVE_NODE_RADIUS, SHAPECONCAVE_FACET_THICKNESS );

    m_pNodeTree = m_pGraph->GetVertexTree();
    m_pFacetTree = m_pGraph->GetTriangleTree();

    _UpdateNormalsAreas();

    // Local AAB
    m_bvLocalAAB = m_pFacetTree->GetRoot()->bvAABox;

    // Self collision data
    m_bUseSelfCollisions = true;
}
ShapeDeformable::~ShapeDeformable()
{
    if ( m_bOwningGraph ) {
        m_pGraph->Destroy();

        PhysicsFn->SelectMemory( TEXT("Shapes") );
        Delete( m_pGraph );
        PhysicsFn->UnSelectMemory();
    }
}

Void ShapeDeformable::ComputeMassProperties( Scalar /*fDensity*/, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const
{
    //Assert( false );
    *outMass = 1.0f;
    *outCenterOfMass = Vertex3::Null;
    *outInertiaTensor = Matrix3::Identity;
    ///////////////////////////////////////////////////////////////////////////////
}

Void ShapeDeformable::SelfCollisionDetection()
{
    // Clear contact points even when disabled as they most probably
    // won't be a good init when enabled again ... (discontinuity)
    m_hSelfCollider.arrSelfContacts.Clear();

    if ( m_bUseSelfCollisions )
        m_pFacetTree->CollideTree( m_pNodeTree, &m_hSelfCollider );
}

Void ShapeDeformable::UpdateCollisionData()
{
    // Graph data
    m_pGraph->UpdateTrees( SHAPECONCAVE_NODE_RADIUS, SHAPECONCAVE_FACET_THICKNESS );

    _UpdateNormalsAreas();

    // LocalAAB
    m_bvLocalAAB = m_pNodeTree->GetRoot()->bvAABox;
}
Void ShapeDeformable::UpdateMesh()
{
    if ( m_pMesh == NULL )
        return;

    TriangleMesh * pMesh = (TriangleMesh*)m_pMesh;

    UInt iVertexSize = pMesh->GetVertexSize();

    UInt iOffset, iSize;

    pMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    Byte * arrPositions = pMesh->GetVB()->GetData() + iOffset;

    Byte * arrNormals = NULL;
    if ( pMesh->GetIL()->HasField(GPUINPUTFIELD_SEMANTIC_NORMAL, 0) ) {
        pMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_NORMAL, 0 );
        arrNormals = pMesh->GetVB()->GetData() + iOffset;
    }

    Vertex3 * pPosition;
    Vector3 * pNormal;

    // Nodes are stored in a treemap (MeshManifold3), so they are sorted
    // by growing indices and the code below works ...
    // Assumes node indices make a consistent array index, without hole
    // and ranging from 0 to N-1 ... This will allways be OK.
    UInt i = 0;
    EnumNodes();
    const Node * pNode = EnumNextNode();
    while( pNode != NULL ) {
        Assert( pNode->GetIndex() == i );

        pPosition = (Vertex3*)arrPositions;
        *pPosition = pNode->Position;
        arrPositions += iVertexSize;

        if ( arrNormals != NULL ) {
            pNormal = (Vector3*)arrNormals;
            *pNormal = pNode->Normal;
            arrNormals += iVertexSize;
        }

        ++i;
        pNode = EnumNextNode();
    }
    Assert( i == pMesh->GetVertexCount() );

    // Done
    pMesh->GetVB()->Update( 0, INVALID_OFFSET, NULL );

    // Update tangents & binormals
    pMesh->UpdateTangentsFromGeometry( NULL );
}

/////////////////////////////////////////////////////////////////////////////////

Void ShapeDeformable::_UpdateNormalsAreas() const
{
    UInt iNodeCount = m_pGraph->GetVertexCount();

    Node *pNode, *pNodeA, *pNodeB, *pNodeC;
    Facet * pFacet;
    UInt iCount;

    PhysicsFn->SelectMemory( TEXT("Scratch") );
    UInt * arrCounts = New UInt[iNodeCount];
    PhysicsFn->UnSelectMemory();

    // Reset node values
    EnumNodes();
    pNode = EnumNextNode();
    while( pNode != NULL ) {
        pNode->Normal = Vector3::Null;
        pNode->Area = 0.0f;
        arrCounts[ pNode->GetIndex() ] = 0;
        pNode = EnumNextNode();
    }

    // Compute facet & node values
    EnumFacets();
    pFacet = EnumNextFacet();
    while( pFacet != NULL ) {
        pFacet->UpdateNormalArea();
        pNodeA = pFacet->GetNode(0);
        pNodeB = pFacet->GetNode(1);
        pNodeC = pFacet->GetNode(2);
        pNodeA->Normal += pFacet->Normal; pNodeA->Area += pFacet->Area; ++( arrCounts[ pNodeA->GetIndex() ] );
        pNodeB->Normal += pFacet->Normal; pNodeB->Area += pFacet->Area; ++( arrCounts[ pNodeB->GetIndex() ] );
        pNodeC->Normal += pFacet->Normal; pNodeC->Area += pFacet->Area; ++( arrCounts[ pNodeC->GetIndex() ] );
        pFacet = EnumNextFacet();
    }

    // Normalize node values
    EnumNodes();
    pNode = EnumNextNode();
    while( pNode != NULL ) {
        pNode->Normal.Normalize();
        iCount = arrCounts[ pNode->GetIndex() ];
        if ( iCount > 0 )
            pNode->Area /= (Scalar)iCount;
        else
            pNode->Area = 0.0f;
        pNode = EnumNextNode();
    }

    PhysicsFn->SelectMemory( TEXT("Scratch") );
    DeleteA( arrCounts );
    PhysicsFn->UnSelectMemory();
}

/////////////////////////////////////////////////////////////////////////////////

ShapeDeformable::SelfCollider::SelfCollider():
    AABoxTree::HCollider(), arrSelfContacts()
{
    arrSelfContacts.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Shapes") );
    arrSelfContacts.Create();
}
ShapeDeformable::SelfCollider::~SelfCollider()
{
    arrSelfContacts.Destroy();
}

Void ShapeDeformable::SelfCollider::Process( const AABoxNode * pLeafA, const AABoxNode * pLeafB )
{
    const Facet * pFacet = (Facet*)( pLeafA->pLeafData );
    const Node * pNode = (Node*)( pLeafB->pLeafData );

    Assert( pFacet->pLeaf == pLeafA );
    Assert( pNode->pLeaf == pLeafB );

    // Adjacent case : Ignore collision
    if ( pFacet->GetNode(0) == pNode || pFacet->GetNode(1) == pNode || pFacet->GetNode(2) == pNode )
        return;

    // Build triangle & sphere shapes
    ShapeTriangle facetShape( pFacet->ComputeShape(), SHAPECONCAVE_FACET_THICKNESS );
    TransformedShapeConvex facetTrShape( &facetShape, Transform3::Identity );

    ShapeSphere nodeShape( pNode->ComputeShape(), 1, 3 );
    TransformedShapeConvex nodeTrShape( &nodeShape, Transform3::Identity );

    // Compute collision
    TriangleSphereCollision hCollision( &facetTrShape, &nodeTrShape );
    hCollision.Initialize();
    Bool bCollision = hCollision.DetectCollision( true );

    // Build contact manifold
    if ( bCollision ) {
        const ContactPoint * pCP = hCollision.GetManifold()->GetContactPoint(0);

        UInt iIndex = arrSelfContacts.Count();
        arrSelfContacts.Push( *pCP );

        ContactPoint * pNewCP = &( arrSelfContacts[iIndex] );
        pNewCP->idCPID.dwSpecifierA = 0;
        pNewCP->idCPID.dwSpecifierB = 0;
        pNewCP->idCPID.dwFeatureA = (DWord)pFacet;
        pNewCP->idCPID.dwFeatureB = (DWord)pNode;
    }
}

