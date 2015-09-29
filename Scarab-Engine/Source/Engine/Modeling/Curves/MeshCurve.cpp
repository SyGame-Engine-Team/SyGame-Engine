/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Curves/MeshCurve.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh Curve
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
#include "MeshCurve.h"

#include "../ModelingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshCurve implementation
MeshCurve::MeshCurve( Curve3 * pCurve, Bool bClosed, UInt iSampleCount, GPUInputLayout * pIL ):
    LineStripMesh()
{
    Assert( pCurve != NULL && iSampleCount >= 2 );
    Assert( pIL != NULL );

    m_pCurve = pCurve;
    m_bClosed = bClosed;

    m_iSampleCount = iSampleCount;

    ModelingFn->SelectMemory( TEXT("Curves") );
    m_arrSamples = New Scalar[m_iSampleCount];
    ModelingFn->UnSelectMemory();

    m_pCurve->SubDivideByParameter( m_arrSamples, m_iSampleCount );

    SetIL( pIL );

    UInt iVertexSize = m_pIL->GetSize();
    UInt iVertexCount = m_iSampleCount;
    UInt iEdgeCount = m_iSampleCount - 1;
    UInt iIndexCount = m_iSampleCount;
    if ( m_bClosed ) {
        ++iEdgeCount;
        ++iIndexCount;
    }

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

    Byte * arrTangents = NULL;
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_TANGENT, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_TANGENT, 0 );
        arrTangents = m_pVB->GetData() + iOffset;
    }

    Byte * arrBiNormals = NULL;
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_BINORMAL, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_BINORMAL, 0 );
        arrBiNormals = m_pVB->GetData() + iOffset;
    }

    Byte * arrTexCoords = NULL;
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0 );
        arrTexCoords = m_pVB->GetData() + iOffset;
    }

    Vertex4 * pPosition;
    Vector4 * pNormal;
    Vector4 * pTangent;
    Vector4 * pBiNormal;
    TextureCoord1 * pTexCoord;

    UInt i;
    Scalar fT;

    for( i = 0; i < m_iSampleCount; ++i ) {
        fT = m_arrSamples[i];

        pPosition = (Vertex4*)arrPositions;
        *pPosition = m_pCurve->Position( fT );
        arrPositions += iVertexSize;

        if ( arrNormals != NULL ) {
            pNormal = (Vector4*)arrNormals;
            *pNormal = m_pCurve->Normal( fT );
            arrNormals += iVertexSize;
        }
        if ( arrTangents != NULL ) {
            pTangent = (Vector4*)arrTangents;
            *pTangent = m_pCurve->Tangent( fT );
            arrTangents += iVertexSize;
        }
        if ( arrBiNormals != NULL ) {
            pBiNormal = (Vector4*)arrBiNormals;
            *pBiNormal = m_pCurve->BiNormal( fT );
            arrBiNormals += iVertexSize;
        }
        if ( arrTexCoords != NULL ) {
            pTexCoord = (TextureCoord1*)arrTexCoords;
            *pTexCoord = fT;
            arrTexCoords += iVertexSize;
        }
    }

    // Generate indices
    UInt * arrIndices = (UInt*)( m_pIB->GetData() );

    for( i = 0; i < m_iSampleCount; ++i )
        *arrIndices++ = i;
    if ( m_bClosed )
        *arrIndices++ = 0;

    // Bind buffers
    m_pVB->Bind();
    m_pIB->Bind();
}
MeshCurve::~MeshCurve()
{
    ModelingFn->SelectMemory( TEXT("Curves") );
    DeleteA( m_arrSamples );
    ModelingFn->UnSelectMemory();
}

//Void MeshCurve::GenerateGraph( MeshManifold2 * outGraph, UInt iSampleCount )
//{
//    // Generate edges
//    for( UInt i = 0; i < (iSampleCount - 1); ++i )
//        outGraph->CreateEdge( MeshEdgeKey(i, i+1) );
//    if ( m_bClosed )
//        outGraph->CreateEdge( MeshEdgeKey(iSampleCount - 1, 0) );
//
//    // Sort links
//    outGraph->SortVertexLinkage();
//    outGraph->SortEdgeLinkage();
//}

/////////////////////////////////////////////////////////////////////////////////

Void MeshCurve::_Update( GPUDeferredContext * pContext )
{
    // Update vertices
    UInt iVertexSize = m_pIL->GetSize();

    UInt iOffset, iSize;

    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    Byte * arrPositions = m_pVB->GetData() + iOffset;

    Byte * arrNormals = NULL;
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_NORMAL, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_NORMAL, 0 );
        arrNormals = m_pVB->GetData() + iOffset;
    }

    Byte * arrTangents = NULL;
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_TANGENT, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_TANGENT, 0 );
        arrTangents = m_pVB->GetData() + iOffset;
    }

    Byte * arrBiNormals = NULL;
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_BINORMAL, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_BINORMAL, 0 );
        arrBiNormals = m_pVB->GetData() + iOffset;
    }

    Byte * arrTexCoords = NULL;
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0 );
        arrTexCoords = m_pVB->GetData() + iOffset;
    }

    Vertex4 * pPosition;
    Vector4 * pNormal;
    Vector4 * pTangent;
    Vector4 * pBiNormal;
    TextureCoord1 * pTexCoord;

    UInt i;
    Scalar fT;

    for( i = 0; i < m_iSampleCount; ++i ) {
        fT = m_arrSamples[i];

        pPosition = (Vertex4*)arrPositions;
        *pPosition = m_pCurve->Position( fT );
        arrPositions += iVertexSize;

        if ( arrNormals != NULL ) {
            pNormal = (Vector4*)arrNormals;
            *pNormal = m_pCurve->Normal( fT );
            arrNormals += iVertexSize;
        }
        if ( arrTangents != NULL ) {
            pTangent = (Vector4*)arrTangents;
            *pTangent = m_pCurve->Tangent( fT );
            arrTangents += iVertexSize;
        }
        if ( arrBiNormals != NULL ) {
            pBiNormal = (Vector4*)arrBiNormals;
            *pBiNormal = m_pCurve->BiNormal( fT );
            arrBiNormals += iVertexSize;
        }
        if ( arrTexCoords != NULL ) {
            pTexCoord = (TextureCoord1*)arrTexCoords;
            *pTexCoord = fT;
            arrTexCoords += iVertexSize;
        }
    }

    // Done
    m_pVB->Update( 0, INVALID_OFFSET, pContext );
}

