/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/Planar/MeshGridPlane.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Planar, Grid-Plane
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
#include "MeshGridPlane.h"

#include "../../ModelingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshGridPlane implementation
MeshGridPlane::MeshGridPlane( const Plane & vPlane, Scalar fWidth, Scalar fHeight,
                              UInt iWidthSamples, UInt iHeightSamples, GPUInputLayout * pIL ):
    TriangleListMesh(), m_vPlane( vPlane )
{
    Assert( iWidthSamples >= 2 && iHeightSamples >= 2 );
    Assert( pIL != NULL );

    m_fWidth = fWidth;
    m_fHeight = fHeight;
    m_iWidthSamples = iWidthSamples;
    m_iHeightSamples = iHeightSamples;

    SetIL( pIL );

    UInt iWidthSamplesM1 = m_iWidthSamples - 1;
    UInt iHeightSamplesM1 = m_iHeightSamples - 1;

    UInt iVertexSize = m_pIL->GetSize();
    UInt iVertexCount = ( m_iWidthSamples * m_iHeightSamples );
    UInt iTriangleCount = ( iWidthSamplesM1 * iHeightSamplesM1 ) << 1; // *2
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

    Byte * arrTexCoords = NULL;
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0 );
        arrTexCoords = m_pVB->GetData() + iOffset;
    }

    Vertex4 * pPosition;
    Vector4 * pNormal;
    TextureCoord2 * pTexCoord;

    Scalar fInvW = MathFn->Invert( (Scalar)iWidthSamplesM1 );
    Scalar fInvH = MathFn->Invert( (Scalar)iHeightSamplesM1 );

    Vector4 vAxisX, vAxisY;
    Vector4::MakeComplementBasis( vAxisX, vAxisY, m_vPlane.Normal );

    Scalar fX, fY, fT, fU;
    UInt iX, iY;

    fU = 0.0f;
    for( iY = 0; iY < m_iHeightSamples; ++iY ) {
        fY = ( fU - 0.5f ) * m_fHeight;

        fT = 0.0f;
        for( iX = 0; iX < m_iWidthSamples; ++iX ) {
            fX = ( fT - 0.5f ) * m_fWidth;

            pPosition = (Vertex4*)( arrPositions );
            *pPosition = m_vPlane.Position;
            *pPosition += vAxisX * fX;
            *pPosition += vAxisY * fY;
            arrPositions += iVertexSize;

            if ( arrNormals != NULL ) {
                pNormal = (Vector4*)( arrNormals );
                *pNormal = m_vPlane.Normal;
                arrNormals += iVertexSize;
            }
            if ( arrTexCoords != NULL ) {
                pTexCoord = (TextureCoord2*)( arrTexCoords );
                pTexCoord->T = fT;
                pTexCoord->U = fU;
                arrTexCoords += iVertexSize;
            }

            fT += fInvW;
        }

        fU += fInvH;
    }

    // Generate indices
    UInt * arrIndices = (UInt*)( m_pIB->GetData() );

    UInt iQuad0, iQuad1, iQuad2, iQuad3;

    for( iY = 0; iY < iHeightSamplesM1; ++iY ) {
        for( iX = 0; iX < iWidthSamplesM1; ++iX ) {
            iQuad0 = (iY * m_iWidthSamples) + iX;
            iQuad1 = iQuad0 + 1;
            iQuad2 = iQuad1 + m_iWidthSamples;
            iQuad3 = iQuad0 + m_iWidthSamples;
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
MeshGridPlane::~MeshGridPlane()
{
    // nothing to do
}

Void MeshGridPlane::GenerateGraph( MeshVoronoiGraph * outGraph )
{
    // Generate facets
    Assert( false );

    // Sort links
    outGraph->SortVertexLinkage();
    outGraph->SortEdgeLinkage();
}

/////////////////////////////////////////////////////////////////////////////////

Void MeshGridPlane::_Update( GPUDeferredContext * pContext )
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

    Byte * arrTexCoords = NULL;
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0 );
        arrTexCoords = m_pVB->GetData() + iOffset;
    }

    Vertex4 * pPosition;
    Vector4 * pNormal;
    TextureCoord2 * pTexCoord;

    Scalar fInvW = MathFn->Invert( (Scalar)(m_iWidthSamples - 1) );
    Scalar fInvH = MathFn->Invert( (Scalar)(m_iHeightSamples - 1) );

    Vector4 vAxisX, vAxisY;
    Vector4::MakeComplementBasis( vAxisX, vAxisY, m_vPlane.Normal );

    Scalar fX, fY, fT, fU;
    UInt iX, iY;

    fU = 0.0f;
    for( iY = 0; iY < m_iHeightSamples; ++iY ) {
        fY = ( fU - 0.5f ) * m_fHeight;
        fT = 0.0f;
        for( iX = 0; iX < m_iWidthSamples; ++iX ) {
            fX = ( fT - 0.5f ) * m_fWidth;

            pPosition = (Vertex4*)( arrPositions );
            *pPosition = m_vPlane.Position;
            *pPosition += vAxisX * fX;
            *pPosition += vAxisY * fY;
            arrPositions += iVertexSize;

            if ( arrNormals != NULL ) {
                pNormal = (Vector4*)( arrNormals );
                *pNormal = m_vPlane.Normal;
                arrNormals += iVertexSize;
            }
            if ( arrTexCoords != NULL ) {
                pTexCoord = (TextureCoord2*)( arrTexCoords );
                pTexCoord->T = fT;
                pTexCoord->U = fU;
                arrTexCoords += iVertexSize;
            }

            fT += fInvW;
        }
        fU += fInvH;
    }

    // Done
    m_pVB->Update( 0, INVALID_OFFSET, pContext );
}


