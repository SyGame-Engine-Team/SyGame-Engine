/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/Linear/MeshSegment.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Linear, Segment
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
#include "MeshSegment.h"

#include "../../ModelingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshSegment implementation
MeshSegment::MeshSegment( const Segment3 & vSegment, GPUInputLayout * pIL ):
    LineListMesh(), m_vSegment( vSegment )
{
    Assert( pIL != NULL );

    SetIL( pIL );
    
    UInt iVertexSize = m_pIL->GetSize();
    UInt iVertexCount = 2;
    //UInt iEdgeCount = 1;
    UInt iIndexCount = 2;

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

    Byte * arrTexCoords = NULL;
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0 );
        arrTexCoords = m_pVB->GetData() + iOffset;
    }

    Vertex4 * pPosition;
    TextureCoord1 * pTexCoord;

    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vSegment.EndA;
    arrPositions += iVertexSize;
    if ( arrTexCoords != NULL ) {
        pTexCoord = (TextureCoord1*)( arrTexCoords );
        pTexCoord->T = 0.0f;
        arrTexCoords += iVertexSize;
    }

    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vSegment.EndB;
    if ( arrTexCoords != NULL ) {
        pTexCoord = (TextureCoord1*)( arrTexCoords );
        pTexCoord->T = 1.0f;
    }

    // Generate indices
    UInt * arrIndices = (UInt*)( m_pIB->GetData() );

    arrIndices[0] = 0;
    arrIndices[1] = 1;

    // Bind buffers
    m_pVB->Bind();
    m_pIB->Bind();
}
MeshSegment::~MeshSegment()
{
    // nothing to do
}

//Void MeshSegment::GenerateGraph( MeshManifold2 * outGraph )
//{
//    // Generate edges
//    outGraph->CreateEdge( MeshEdgeKey(0, 1) );
//
//    // Sort links
//    outGraph->SortVertexLinkage();
//    outGraph->SortEdgeLinkage();
//}

/////////////////////////////////////////////////////////////////////////////////

Void MeshSegment::_Update( GPUDeferredContext * pContext )
{
    // Update vertices
    UInt iVertexSize = m_pIL->GetSize();

    UInt iOffset, iSize;

    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    Byte * arrPositions = m_pVB->GetData() + iOffset;

    Vertex4 * pPosition;

    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vSegment.EndA;
    arrPositions += iVertexSize;

    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vSegment.EndB;

    // Done
    m_pVB->Update( 0, INVALID_OFFSET, pContext );
}

