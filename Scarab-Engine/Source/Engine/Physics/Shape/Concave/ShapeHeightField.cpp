/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Concave/ShapeHeightField.cpp
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
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "ShapeHeightField.h"

/////////////////////////////////////////////////////////////////////////////////
// ShapeHeightField implementation
ShapeHeightField::ShapeHeightField( MeshHeightField * pMesh ):
    ShapeConcave( pMesh )
{
    Vector3 vExtents = ( pMesh->GetSize() * 0.5f );
    Vertex3 vCenter = pMesh->GetOrigin() + vExtents;
    vCenter.Z = ( pMesh->GetMinHeight() + pMesh->GetMaxHeight() ) * 0.5f;
    vExtents.Z = ( vCenter.Z - pMesh->GetMinHeight() );

    m_bvLocalAAB = BVAABox( vCenter, vExtents );
}
ShapeHeightField::~ShapeHeightField()
{
    // nothing to do
}

Void ShapeHeightField::ComputeMassProperties( Scalar /*fDensity*/, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const
{
    //Assert( false );
    *outMass = 1.0f;
    *outCenterOfMass = Vertex3::Null;
    *outInertiaTensor = Matrix3::Identity;
    /////////////////////////////
}

Void ShapeHeightField::QueryNodes( const BVAABox & bvAABox, HNodeQuery * pQuery ) const
{
    UInt iStartX, iEndX, iStartY, iEndY;
    UInt iPitch = _RasterizeAABox( bvAABox, &iStartX, &iEndX, &iStartY, &iEndY );

    // Process vertices
    UInt iVertexSize = m_pMesh->GetVertexSize();

    UInt iOffset, iSize;

    m_pMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    Byte * arrPositions = m_pMesh->GetVB()->GetData() + iOffset;

    UInt iX, iY;
    UInt iIndex;
    const Vertex3 * pV;

    NodeLeaf leafNode;
    leafNode.pLeaf = NULL;

    iY = iStartY;
    while( iY <= iEndY ) {
        iX = iStartX;
        while( iX <= iEndX ) {
            iIndex = ( iY * iPitch ) + iX;
            pV = (const Vertex3 *)( arrPositions + (iIndex * iVertexSize) );

            leafNode.vNode = Sphere( *pV, SHAPECONCAVE_NODE_RADIUS );
            leafNode.dwNodeID = ( ((iY & 0xffff) << 16) + (iX & 0xffff) );

            pQuery->Process( leafNode );

            ++iX;
        }
        ++iY;
    }
}

Void ShapeHeightField::QueryTriangles( const BVAABox & bvAABox, HTriangleQuery * pQuery ) const
{
    UInt iStartX, iEndX, iStartY, iEndY;
    UInt iPitch = _RasterizeAABox( bvAABox, &iStartX, &iEndX, &iStartY, &iEndY );

    // Process triangles
    UInt iVertexSize = m_pMesh->GetVertexSize();

    UInt iOffset, iSize;

    m_pMesh->GetIL()->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    Byte * arrPositions = m_pMesh->GetVB()->GetData() + iOffset;

    UInt iX, iY;
    UInt iQuad0, iQuad1, iQuad2, iQuad3;
    const Vertex3 *pA, *pB, *pC, *pD;

    TriangleLeaf leafTriangle;
    leafTriangle.pLeaf = NULL;

    iY = iStartY;
    while( iY < iEndY ) {
        iX = iStartX;
        while( iX < iEndX ) {
            iQuad0 = ( iY * iPitch ) + iX;
            iQuad1 = iQuad0 + 1;
            iQuad2 = iQuad1 + iPitch;
            iQuad3 = iQuad0 + iPitch;
            pA = (const Vertex3 *)( arrPositions + (iQuad0 * iVertexSize) );
            pB = (const Vertex3 *)( arrPositions + (iQuad1 * iVertexSize) );
            pC = (const Vertex3 *)( arrPositions + (iQuad2 * iVertexSize) );
            pD = (const Vertex3 *)( arrPositions + (iQuad3 * iVertexSize) );

            // ABC
            leafTriangle.vTriangle.A = *pA;
            leafTriangle.vTriangle.B = *pB;
            leafTriangle.vTriangle.C = *pC;
            leafTriangle.dwTriangleID = ( ((iY & 0x7fff) << 16) + (iX & 0xffff) );

            pQuery->Process( leafTriangle );

            // ACD
            leafTriangle.vTriangle.B = *pC;
            leafTriangle.vTriangle.C = *pD;
            leafTriangle.dwTriangleID |= 0x80000000;

            pQuery->Process( leafTriangle );

            ++iX;
        }
        ++iY;
    }
}

Void ShapeHeightField::UpdateAAB()
{
    MeshHeightField * pMesh = (MeshHeightField*)m_pMesh;

    Vector3 vExtents = ( pMesh->GetSize() * 0.5f );
    Vertex3 vCenter = pMesh->GetOrigin() + vExtents;
    vCenter.Z = ( pMesh->GetMinHeight() + pMesh->GetMaxHeight() ) * 0.5f;
    vExtents.Z = ( vCenter.Z - pMesh->GetMinHeight() );

    m_bvLocalAAB = BVAABox( vCenter, vExtents );
}

/////////////////////////////////////////////////////////////////////////////////

UInt ShapeHeightField::_RasterizeAABox( const BVAABox & bvAABox, UInt * outMinX, UInt * outMaxX, UInt * outMinY, UInt * outMaxY ) const
{
    const MeshHeightField * pMesh = (const MeshHeightField *)m_pMesh;

    // Get AAB min/max
    Vertex3 vLocalMin = m_bvLocalAAB.GetMin();
    Vertex3 vLocalMax = m_bvLocalAAB.GetMax();

    Vertex3 vAABMin = bvAABox.GetMin();
    Vertex3 vAABMax = bvAABox.GetMax();

    // Clamp the AAB
    vAABMin.X = Clamp<Scalar>( vAABMin.X, vLocalMin.X, vLocalMax.X );
    vAABMin.Y = Clamp<Scalar>( vAABMin.Y, vLocalMin.Y, vLocalMax.Y );
    vAABMin.Z = Clamp<Scalar>( vAABMin.Z, vLocalMin.Z, vLocalMax.Z );
    vAABMax.X = Clamp<Scalar>( vAABMax.X, vLocalMin.X, vLocalMax.X );
    vAABMax.Y = Clamp<Scalar>( vAABMax.Y, vLocalMin.Y, vLocalMax.Y );
    vAABMax.Z = Clamp<Scalar>( vAABMax.Z, vLocalMin.Z, vLocalMax.Z );

    // Switch to local coords
    Vector2 vMin = ( Vertex2(vAABMin) - pMesh->GetOrigin() );
    Vector2 vMax = ( Vertex2(vAABMax) - pMesh->GetOrigin() );

    UInt iSampleCountU = pMesh->GetSampleCountU();
    UInt iSampleCountV = pMesh->GetSampleCountV();
    Scalar fInvFieldSizeX = MathFn->Invert( pMesh->GetSize().X );
    Scalar fInvFieldSizeY = MathFn->Invert( pMesh->GetSize().Y );
    vMin.X *= ( fInvFieldSizeX * (Scalar)iSampleCountU );
    vMin.Y *= ( fInvFieldSizeY * (Scalar)iSampleCountV );
    vMax.X *= ( fInvFieldSizeX * (Scalar)iSampleCountU );
    vMax.Y *= ( fInvFieldSizeY * (Scalar)iSampleCountV );

    // Rasterize (upper bound)
    Int iMin[2];
    iMin[0] = MathFn->Floor( vMin.X ) - 1;
    iMin[1] = MathFn->Floor( vMin.Y ) - 1;

    Int iMax[2];
    iMax[0] = MathFn->Ceil( vMax.X ) + 1;
    iMax[1] = MathFn->Ceil( vMax.Y ) + 1;

    *outMinX = 0;
    *outMaxX = ( iSampleCountU - 1 );
    *outMinY = 0;
    *outMaxY = ( iSampleCountV - 1 );
    if ( iMin[0] > (Int)*outMinX )
        *outMinX = iMin[0];
    if ( iMax[0] < (Int)*outMaxX )
        *outMaxX = iMax[0];
    if ( iMin[1] > (Int)*outMinY )
        *outMinY = iMin[1];
    if ( iMax[1] < (Int)*outMaxY )
        *outMaxY = iMax[1];

    return iSampleCountU;
}

