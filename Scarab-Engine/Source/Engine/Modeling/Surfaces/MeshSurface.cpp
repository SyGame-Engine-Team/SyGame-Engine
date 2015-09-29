/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Surfaces/MeshSurface.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh Surface
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
#include "MeshSurface.h"

#include "../ModelingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshSurface implementation
MeshSurface::MeshSurface( ParametricSurface * pSurface, UInt iSampleCountU, UInt iSampleCountV, GPUInputLayout * pIL ):
    TriangleListMesh()
{
    Assert( pSurface != NULL && iSampleCountU >= 2 && iSampleCountV >= 2 );
    Assert( pIL != NULL );

    m_pSurface = pSurface;

    m_iSampleCountU = iSampleCountU;
    m_iSampleCountV = iSampleCountV;

    ModelingFn->SelectMemory( TEXT("Surfaces") );
    m_arrSamplesU = New Scalar[m_iSampleCountU];
    m_arrSamplesV = New Scalar[m_iSampleCountV];
    ModelingFn->UnSelectMemory();

    m_pSurface->SubDivideByParameter( m_arrSamplesU, m_iSampleCountU, m_arrSamplesV, m_iSampleCountV );

    SetIL( pIL );

    UInt iSampleCountUM1 = m_iSampleCountU - 1;
    UInt iSampleCountVM1 = m_iSampleCountV - 1;

    UInt iVertexSize = m_pIL->GetSize();
    UInt iVertexCount = ( m_iSampleCountU * m_iSampleCountV );
    UInt iTriangleCount = ( iSampleCountUM1 * iSampleCountVM1 ) << 1; // *2
    UInt iIndexCount = ( iTriangleCount << 1 ) + iTriangleCount; // *3

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
    TextureCoord2 * pTexCoord;

    UInt iU, iV;
    Scalar fU, fV;

    for( iV = 0; iV < m_iSampleCountV; ++iV ) {
        fV = m_arrSamplesV[iV];
        for( iU = 0; iU < m_iSampleCountU; ++iU ) {
            fU = m_arrSamplesU[iU];

            pPosition = (Vertex4*)arrPositions;
            *pPosition = m_pSurface->Position( fU, fV );
            arrPositions += iVertexSize;

            if ( arrNormals != NULL ) {
                pNormal = (Vector4*)arrNormals;
                *pNormal = m_pSurface->Normal( fU, fV );
                arrNormals += iVertexSize;
            }
            if ( arrTangents != NULL ) {
                pTangent = (Vector4*)arrTangents;
                *pTangent = m_pSurface->TangentU( fU, fV );
                arrTangents += iVertexSize;
            }
            if ( arrBiNormals != NULL ) {
                pBiNormal = (Vector4*)arrBiNormals;
                *pBiNormal = m_pSurface->TangentV( fU, fV );
                arrBiNormals += iVertexSize;
            }
            if ( arrTexCoords != NULL ) {
                pTexCoord = (TextureCoord2*)arrTexCoords;
                pTexCoord->T = fU;
                pTexCoord->U = fV;
                arrTexCoords += iVertexSize;
            }
        }
    }

    // Generate indices
    UInt * arrIndices = (UInt*)( m_pIB->GetData() );

    UInt iQuad0, iQuad1, iQuad2, iQuad3;

    for( iV = 0; iV < iSampleCountVM1; ++iV ) {
        for( iU = 0; iU < iSampleCountUM1; ++iU ) {
            iQuad0 = (iV * m_iSampleCountU) + iU;
            iQuad1 = iQuad0 + 1;
            iQuad2 = iQuad1 + m_iSampleCountU;
            iQuad3 = iQuad0 + m_iSampleCountU;
            *arrIndices++ = iQuad0;
            *arrIndices++ = iQuad1;
            *arrIndices++ = iQuad2;
            *arrIndices++ = iQuad0;
            *arrIndices++ = iQuad2;
            *arrIndices++ = iQuad3;
        }
    }

    // Bind buffers
    m_pVB->Bind();
    m_pIB->Bind();
}
MeshSurface::~MeshSurface()
{
    ModelingFn->SelectMemory( TEXT("Surfaces") );
    DeleteA( m_arrSamplesU );
    DeleteA( m_arrSamplesV );
    ModelingFn->UnSelectMemory();
}

Void MeshSurface::GenerateGraph( MeshManifold3 * outGraph, UInt iSampleCountU, UInt iSampleCountV )
{
    // Generate facets
    UInt iSampleCountUM1 = iSampleCountU - 1;
    UInt iSampleCountVM1 = iSampleCountV - 1;

    UInt iQuad0, iQuad1, iQuad2, iQuad3;

    for( UInt iV = 0; iV < iSampleCountVM1; ++iV ) {
        for( UInt iU = 0; iU < iSampleCountUM1; ++iU ) {
            iQuad0 = (iV * iSampleCountU) + iU;
            iQuad1 = iQuad0 + 1;
            iQuad2 = iQuad1 + iSampleCountU;
            iQuad3 = iQuad0 + iSampleCountU;
            outGraph->CreateFacet( MeshTriangleKey( iQuad0, iQuad1, iQuad2 ) );
            outGraph->CreateFacet( MeshTriangleKey( iQuad0, iQuad2, iQuad3 ) );
        }
    }

    // Sort links
    outGraph->SortVertexLinkage();
    outGraph->SortEdgeLinkage();
}

/////////////////////////////////////////////////////////////////////////////////

Void MeshSurface::_Update( GPUDeferredContext * pContext )
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
    TextureCoord2 * pTexCoord;

    UInt iU, iV;
    Scalar fU, fV;

    for( iV = 0; iV < m_iSampleCountV; ++iV ) {
        fV = m_arrSamplesV[iV];
        for( iU = 0; iU < m_iSampleCountU; ++iU ) {
            fU = m_arrSamplesU[iU];

            pPosition = (Vertex4*)arrPositions;
            *pPosition = m_pSurface->Position( fU, fV );
            arrPositions += iVertexSize;

            if ( arrNormals != NULL ) {
                pNormal = (Vector4*)arrNormals;
                *pNormal = m_pSurface->Normal( fU, fV );
                arrNormals += iVertexSize;
            }
            if ( arrTangents != NULL ) {
                pTangent = (Vector4*)arrTangents;
                *pTangent = m_pSurface->TangentU( fU, fV );
                arrTangents += iVertexSize;
            }
            if ( arrBiNormals != NULL ) {
                pBiNormal = (Vector4*)arrBiNormals;
                *pBiNormal = m_pSurface->TangentV( fU, fV );
                arrBiNormals += iVertexSize;
            }
            if ( arrTexCoords != NULL ) {
                pTexCoord = (TextureCoord2*)arrTexCoords;
                pTexCoord->T = fU;
                pTexCoord->U = fV;
                arrTexCoords += iVertexSize;
            }
        }
    }

    // Done
    m_pVB->Update( 0, INVALID_OFFSET, pContext );
}




