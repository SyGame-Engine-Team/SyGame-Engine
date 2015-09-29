/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/Planar/MeshTriangle.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Planar, Triangle
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
#include "MeshTriangle.h"

#include "../../ModelingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangle implementation
MeshTriangle::MeshTriangle( const Triangle3 & vTriangle, GPUInputLayout * pIL ):
    TriangleListMesh(), m_vTriangle( vTriangle )
{
    Assert( pIL != NULL );

    SetIL( pIL );
    
    UInt iVertexSize = m_pIL->GetSize();
    UInt iVertexCount = 3;
    //UInt iTriangleCount = 1;
    UInt iIndexCount = 3;

    // Create buffers
    GPUVertexBuffer * pVB = RenderingFn->CreateVertexBuffer( iVertexSize, iVertexCount );
    pVB->SetUsage( GPURESOURCE_USAGE_DEFAULT );

    GPUIndexBuffer * pIB = RenderingFn->CreateIndexBuffer( iIndexCount );
    pIB->SetUsage( GPURESOURCE_USAGE_DEFAULT );

    SetVB( pVB );
    SetIB( pIB );

    // Generate vertices
    UInt iOffset, iSize;

    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    Byte * arrPositions = m_pVB->GetData() + iOffset;

    Byte * arrNormals = NULL;
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_NORMAL, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_NORMAL, 0 );
        arrNormals = m_pVB->GetData() + iOffset;
    }

    Byte * arrTexCoords = NULL;
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0 );
        arrTexCoords = m_pVB->GetData() + iOffset;
    }

    Vertex4 * pPosition;
    Vector4 * pNormal;
    TextureCoord2 * pTexCoord;

    Vector4 vNormal = m_vTriangle.GetPlane().Normal;

    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vTriangle.A;
    arrPositions += iVertexSize;
    if ( arrNormals != NULL ) {
        pNormal = (Vector4*)( arrNormals );
        *pNormal = vNormal;
        arrNormals += iVertexSize;
    }
    if ( arrTexCoords != NULL ) {
        pTexCoord = (TextureCoord2*)( arrTexCoords );
        pTexCoord->T = 0.0f;
        pTexCoord->U = 0.0f;
        arrTexCoords += iVertexSize;
    }

    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vTriangle.B;
    arrPositions += iVertexSize;
    if ( arrNormals != NULL ) {
        pNormal = (Vector4*)( arrNormals );
        *pNormal = vNormal;
        arrNormals += iVertexSize;
    }
    if ( arrTexCoords != NULL ) {
        pTexCoord = (TextureCoord2*)( arrTexCoords );
        pTexCoord->T = 1.0f;
        pTexCoord->U = 0.0f;
        arrTexCoords += iVertexSize;
    }

    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vTriangle.C;
    if ( arrNormals != NULL ) {
        pNormal = (Vector4*)( arrNormals );
        *pNormal = vNormal;
    }
    if ( arrTexCoords != NULL ) {
        pTexCoord = (TextureCoord2*)( arrTexCoords );
        pTexCoord->T = 0.0f;
        pTexCoord->U = 1.0f;
    }

    // Generate indices
    UInt * arrIndices = (UInt*)( m_pIB->GetData() );

    arrIndices[0] = 0;
    arrIndices[1] = 1;
    arrIndices[2] = 2;

    // Bind buffers
    m_pVB->Bind();
    m_pIB->Bind();
}
MeshTriangle::~MeshTriangle()
{
    // nothing to do
}

Void MeshTriangle::GenerateGraph( MeshVoronoiGraph * outGraph )
{
    // Generate facets
    outGraph->CreateFacet( MeshTriangleKey(0, 1, 2) );
    outGraph->CreateFacet( MeshTriangleKey(0, 2, 1) );

    // Sort links
    outGraph->SortVertexLinkage();
    outGraph->SortEdgeLinkage();
}

/////////////////////////////////////////////////////////////////////////////////

Void MeshTriangle::_Update( GPUDeferredContext * pContext )
{
    // Update vertices
    UInt iVertexSize = m_pIL->GetSize();

    UInt iOffset, iSize;

    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    Byte * arrPositions = m_pVB->GetData() + iOffset;

    Vertex4 * pPosition;

    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vTriangle.A;
    arrPositions += iVertexSize;

    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vTriangle.B;
    arrPositions += iVertexSize;

    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vTriangle.C;

    // Done
    m_pVB->Update( 0, INVALID_OFFSET, pContext );
}


