/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Convex/ShapeConvexMesh.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shapes : Convex, Mesh
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "ShapeConvexMesh.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ShapeConvexMesh implementation
ShapeConvexMesh::ShapeConvexMesh( MeshVoronoiGraph * pGraph, TriangleMesh * pMesh ):
    ShapeConvex( pMesh ), m_hGaussTree(), m_hExtremalQuery()
{
    // Graph data
    m_bOwningGraph = false;

    m_pGraph = pGraph;

    // Local AAB
    m_bvLocalAAB.Reset( Vertex3::Null );

    m_pGraph->EnumVertices();
    const Vertex * pVertex = (Vertex*)( m_pGraph->EnumNextVertex() );
    while( pVertex != NULL ) {
        m_bvLocalAAB.Grow( pVertex->Position );
        pVertex = (Vertex*)( m_pGraph->EnumNextVertex() );
    }

    // Gauss-Tree
    m_hGaussTree.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Shapes") );
    m_hGaussTree.Create();

    m_hGaussTree.Build( m_pGraph );
}
ShapeConvexMesh::ShapeConvexMesh( TriangleMesh * pMesh ):
    ShapeConvex( pMesh ), m_hGaussTree(), m_hExtremalQuery()
{
    // Graph data
    m_bOwningGraph = true;

    PhysicsFn->SelectMemory( TEXT("Shapes") );
    m_pGraph = New MeshVoronoiGraph();
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

    m_pGraph->ComputeFeatureData( arrPositions, iVertexSize );

    // Local AAB
    m_bvLocalAAB.Reset( Vertex3::Null );

    m_pGraph->EnumVertices();
    const Vertex * pVertex = (Vertex*)( m_pGraph->EnumNextVertex() );
    while( pVertex != NULL ) {
        m_bvLocalAAB.Grow( pVertex->Position );
        pVertex = (Vertex*)( m_pGraph->EnumNextVertex() );
    }

    // Gauss-Tree
    m_hGaussTree.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Shapes") );
    m_hGaussTree.Create();

    m_hGaussTree.Build( m_pGraph );
}
ShapeConvexMesh::~ShapeConvexMesh()
{
    m_hGaussTree.Destroy();

    if ( m_bOwningGraph ) {
        m_pGraph->Destroy();

        PhysicsFn->SelectMemory( TEXT("Shapes") );
        Delete( m_pGraph );
        PhysicsFn->UnSelectMemory();
    }
}

Void ShapeConvexMesh::ComputeMassProperties( Scalar fDensity, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const
{
    static const Scalar Inv6 = (1.0f / 6.0f);
    static const Scalar Inv24 = (1.0f / 24.0f);
    static const Scalar Inv60 = (1.0f / 60.0f);
    static const Scalar Inv120 = (1.0f / 120.0f);

    // 1, x, y, z, x2, y2, z2, xy, yz, zx
    Scalar arrIntegrals[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

    // Walk through all facets
    m_pGraph->EnumFacets();
    const MeshFacet * pFacet = m_pGraph->EnumNextFacet();
    while( pFacet != NULL ) {
        switch( pFacet->GetFacetType() ) {
            case MESH_FACET_TRIANGLE: {
                    const Triangle * pTriangle = (const Triangle *)pFacet;
                    const Vertex3 & vA = m_pGraph->GetVertexPosition( pTriangle->Vertices[0] );
                    const Vertex3 & vB = m_pGraph->GetVertexPosition( pTriangle->Vertices[1] );
                    const Vertex3 & vC = m_pGraph->GetVertexPosition( pTriangle->Vertices[2] );
                    _UpdateMassIntegral( arrIntegrals, vA, vB, vC );
                } break;
            case MESH_FACET_QUAD: {
                    const Quad * pQuad = (const Quad *)pFacet;
                    const Vertex3 & vA = m_pGraph->GetVertexPosition( pQuad->Vertices[0] );
                    const Vertex3 & vB = m_pGraph->GetVertexPosition( pQuad->Vertices[1] );
                    const Vertex3 & vC = m_pGraph->GetVertexPosition( pQuad->Vertices[2] );
                    const Vertex3 & vD = m_pGraph->GetVertexPosition( pQuad->Vertices[3] );
                    _UpdateMassIntegral( arrIntegrals, vA, vB, vC );
                    _UpdateMassIntegral( arrIntegrals, vA, vC, vD );
                } break;
            case MESH_FACET_POLYGON: {
                    // Quick on-the-fly fan triangulation
                    const Polygon * pPolygon = (const Polygon *)pFacet;
                    UInt iTriangleCount = ( pPolygon->GetVertexCount() - 2 );
                    const Vertex3 & vA = m_pGraph->GetVertexPosition( pPolygon->Vertices[0] );
                    for( UInt i = 0; i < iTriangleCount; ++i ) {
                        const Vertex3 & vB = m_pGraph->GetVertexPosition( pPolygon->Vertices[i+1] );
                        const Vertex3 & vC = m_pGraph->GetVertexPosition( pPolygon->Vertices[i+2] );
                        _UpdateMassIntegral( arrIntegrals, vA, vB, vC );
                    }
                } break;
            default: Assert(false); break;
        }
        pFacet = m_pGraph->EnumNextFacet();
    }

    // Finalize
    Scalar fTmp0 = ( Inv24 * fDensity );
    Scalar fTmp1 = ( Inv60 * fDensity );
    Scalar fTmp2 = ( Inv120 * fDensity );
    arrIntegrals[0] *= ( Inv6 * fDensity );
    arrIntegrals[1] *= fTmp0;
    arrIntegrals[2] *= fTmp0;
    arrIntegrals[3] *= fTmp0;
    arrIntegrals[4] *= fTmp1;
    arrIntegrals[5] *= fTmp1;
    arrIntegrals[6] *= fTmp1;
    arrIntegrals[7] *= fTmp2;
    arrIntegrals[8] *= fTmp2;
    arrIntegrals[9] *= fTmp2;

    // Mass / CenterOfMass
    *outMass = arrIntegrals[0];
    *outCenterOfMass = ( Vertex3(arrIntegrals[1], arrIntegrals[2], arrIntegrals[3]) / (*outMass) );

    // Inertia Tensor
    outInertiaTensor->m00 = arrIntegrals[5] + arrIntegrals[6];
    outInertiaTensor->m01 = -arrIntegrals[7];
    outInertiaTensor->m02 = -arrIntegrals[9];
    outInertiaTensor->m10 = outInertiaTensor->m01;
    outInertiaTensor->m11 = arrIntegrals[4] + arrIntegrals[6];
    outInertiaTensor->m12 = -arrIntegrals[8];
    outInertiaTensor->m20 = outInertiaTensor->m02;
    outInertiaTensor->m21 = outInertiaTensor->m12;
    outInertiaTensor->m22 = arrIntegrals[4] + arrIntegrals[5];
}

Void ShapeConvexMesh::QueryExtremalVertices( const Vector3 & vDirection, UInt * outMax, UInt * outMin ) const
{
    if ( outMax != NULL ) {
        m_hExtremalQuery.pOutResult = outMax;
        m_hGaussTree.ExtremalQuery( vDirection, &m_hExtremalQuery );
    }
    if ( outMin != NULL ) {
        m_hExtremalQuery.pOutResult = outMin;
        m_hGaussTree.ExtremalQuery( -vDirection, &m_hExtremalQuery );
    }
}
Void ShapeConvexMesh::QueryExtremalVertices( const Vector3 & vDirection, Vertex3 * outMax, Vertex3 * outMin ) const
{
    UInt iTemp;

    if ( outMax != NULL ) {
        m_hExtremalQuery.pOutResult = &iTemp;
        m_hGaussTree.ExtremalQuery( vDirection, &m_hExtremalQuery );
        *outMax = m_pGraph->GetVertexPosition( MeshVertexKey(iTemp) );
    }
    if ( outMin != NULL ) {
        m_hExtremalQuery.pOutResult = &iTemp;
        m_hGaussTree.ExtremalQuery( -vDirection, &m_hExtremalQuery );
        *outMin = m_pGraph->GetVertexPosition( MeshVertexKey(iTemp) );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void ShapeConvexMesh::_UpdateMassIntegral( Scalar arrIntegrals[10], const Vertex3 & vA, const Vertex3 & vB, const Vertex3 & vC )
{
    Vector3 vAB, vAC, vNormal;

    Scalar fTmp0, fTmp1, fTmp2;
    Scalar fF1X, fF2X, fF3X, fG0X, fG1X, fG2X;
    Scalar fF1Y, fF2Y, fF3Y, fG0Y, fG1Y, fG2Y;
    Scalar fF1Z, fF2Z, fF3Z, fG0Z, fG1Z, fG2Z;

    vAB = ( vB - vA );
    vAC = ( vC - vA );
    vNormal = ( vAB ^ vAC );

    // Integral terms for X
    fTmp0 = vA.X + vB.X;
    fF1X = fTmp0 + vC.X;
    fTmp1 = vA.X * vA.X;
    fTmp2 = fTmp1 + vB.X * fTmp0;
    fF2X = fTmp2 + vC.X * fF1X;
    fF3X = vA.X * fTmp1 + vB.X * fTmp2 + vC.X * fF2X;
    fG0X = fF2X + vA.X * (fF1X + vA.X);
    fG1X = fF2X + vB.X * (fF1X + vB.X);
    fG2X = fF2X + vC.X * (fF1X + vC.X);

    // Integral terms for Y
    fTmp0 = vA.Y + vB.Y;
    fF1Y = fTmp0 + vC.Y;
    fTmp1 = vA.Y * vA.Y;
    fTmp2 = fTmp1 + vB.Y * fTmp0;
    fF2Y = fTmp2 + vC.Y * fF1Y;
    fF3Y = vA.Y * fTmp1 + vB.Y * fTmp2 + vC.Y * fF2Y;
    fG0Y = fF2Y + vA.Y * (fF1Y + vA.Y);
    fG1Y = fF2Y + vB.Y * (fF1Y + vB.Y);
    fG2Y = fF2Y + vC.Y * (fF1Y + vC.Y);

    // Integral terms for Z
    fTmp0 = vA.Z + vB.Z;
    fF1Z = fTmp0 + vC.Z;
    fTmp1 = vA.Z * vA.Z;
    fTmp2 = fTmp1 + vB.Z * fTmp0;
    fF2Z = fTmp2 + vC.Z * fF1Z;
    fF3Z = vA.Z * fTmp1 + vB.Z * fTmp2 + vC.Z * fF2Z;
    fG0Z = fF2Z + vA.Z * (fF1Z + vA.Z);
    fG1Z = fF2Z + vB.Z * (fF1Z + vB.Z);
    fG2Z = fF2Z + vC.Z * (fF1Z + vC.Z);

    // Update
    arrIntegrals[0] += vNormal.X * fF1X;
    arrIntegrals[1] += vNormal.X * fF2X;
    arrIntegrals[2] += vNormal.Y * fF2Y;
    arrIntegrals[3] += vNormal.Z * fF2Z;
    arrIntegrals[4] += vNormal.X * fF3X;
    arrIntegrals[5] += vNormal.Y * fF3Y;
    arrIntegrals[6] += vNormal.Z * fF3Z;
    arrIntegrals[7] += vNormal.X * ( vA.Y * fG0X + vB.Y * fG1X + vC.Y * fG2X );
    arrIntegrals[8] += vNormal.Y * ( vA.Z * fG0Y + vB.Z * fG1Y + vC.Z * fG2Y );
    arrIntegrals[9] += vNormal.Z * ( vA.X * fG0Z + vB.X * fG1Z + vC.X * fG2Z );
}

ShapeConvexMesh::ExtremalQuery::ExtremalQuery():
    GaussTree::HQuery()
{
    pOutResult = NULL;
}
ShapeConvexMesh::ExtremalQuery::~ExtremalQuery()
{
    // nothing to do
}

