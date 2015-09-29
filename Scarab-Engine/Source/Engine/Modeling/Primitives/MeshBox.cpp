/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/MeshBox.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Box
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
#include "MeshBox.h"

#include "../ModelingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshBox implementation
MeshBox::MeshBox( const Box & vBox, GPUInputLayout * pIL, Bool bInterior ):
    TriangleListMesh(), m_vBox( vBox )
{
    Assert( pIL != NULL );

    m_bInterior = bInterior;

    SetIL( pIL );
    
    UInt iVertexSize = m_pIL->GetSize();
    UInt iVertexCount = 24;
    //UInt iTriangleCount = 12;
    UInt iIndexCount = 36;

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

    UInt iAxis, iAxis2, iAxis3, iFace, iFaceSelect, iVertex;
    static const Scalar fAxisSign[2]  = { -1.0f, +1.0f };
    static const Scalar fAxis2Sign[8] = { +1.0f, +1.0f, -1.0f, -1.0f, +1.0f, -1.0f, -1.0f, +1.0f };
    static const Scalar fAxis3Sign[8] = { +1.0f, -1.0f, -1.0f, +1.0f, +1.0f, +1.0f, -1.0f, -1.0f };
    static const Scalar fTexCoordT[8] = {  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f };
    static const Scalar fTexCoordU[8] = {  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f };

    for( iAxis = 0; iAxis < 3; ++iAxis ) {
        iAxis2 = (iAxis+1) % 3;
        iAxis3 = (iAxis+2) % 3;
        for( iFace = 0; iFace < 2; ++iFace ) {
            iFaceSelect = (iFace << 2);
            for( iVertex = 0; iVertex < 4; ++iVertex ) {
                pPosition = (Vertex4*)( arrPositions );
                (*pPosition)[iAxis]  = m_vBox.Center[iAxis]  + ( fAxisSign[iFace]                  * (m_vBox.Extents[iAxis]) );
                (*pPosition)[iAxis2] = m_vBox.Center[iAxis2] + ( fAxis2Sign[iFaceSelect + iVertex] * (m_vBox.Extents[iAxis2]) );
                (*pPosition)[iAxis3] = m_vBox.Center[iAxis3] + ( fAxis3Sign[iFaceSelect + iVertex] * (m_vBox.Extents[iAxis3]) );
                pPosition->W = 1.0f;
                arrPositions += iVertexSize;

                if ( arrNormals != NULL ) {
                    pNormal = (Vector4*)( arrNormals );
                    (*pNormal)[iAxis]  = fAxisSign[iFace];
                    (*pNormal)[iAxis2] = 0.0f;
                    (*pNormal)[iAxis3] = 0.0f;
                    if ( m_bInterior )
                        (*pNormal)[iAxis] *= -1.0f;
                    arrNormals += iVertexSize;
                }
                if ( arrTexCoords != NULL ) {
                    pTexCoord = (TextureCoord2*)( arrTexCoords );
                    pTexCoord->T = fTexCoordT[iFaceSelect + iVertex];
                    pTexCoord->U = fTexCoordU[iFaceSelect + iVertex];
                    if ( m_bInterior )
                        pTexCoord->T = (1.0f - pTexCoord->T);
                    arrTexCoords += iVertexSize;
                }
            }
        }
    }

    // Generate indices
    UInt * arrIndices = (UInt*)( m_pIB->GetData() );

    UInt iQuad0, iQuad1, iQuad2, iQuad3;

    for( iAxis = 0; iAxis < 3; ++iAxis ) {
        for( iFace = 0; iFace < 2; ++iFace ) {
            iOffset = ( (iAxis << 3) + (iFace << 2) );
            iQuad0 = iOffset;
            iQuad1 = iOffset + 1;
            iQuad2 = iOffset + 2;
            iQuad3 = iOffset + 3;
            if ( m_bInterior ) {
                *arrIndices++ = iQuad0;
                *arrIndices++ = iQuad3;
                *arrIndices++ = iQuad2;
                *arrIndices++ = iQuad0;
                *arrIndices++ = iQuad2;
                *arrIndices++ = iQuad1;
            } else {
                *arrIndices++ = iQuad0;
                *arrIndices++ = iQuad1;
                *arrIndices++ = iQuad2;
                *arrIndices++ = iQuad0;
                *arrIndices++ = iQuad2;
                *arrIndices++ = iQuad3;
            }
        }
    }

    // Bind buffers
    m_pVB->Bind();
    m_pIB->Bind();
}
MeshBox::~MeshBox()
{
    // nothing to do
}

Void MeshBox::GenerateGraph( MeshVoronoiGraph * outGraph, Bool bInterior )
{
    // Generate facets
    if ( bInterior ) {
        outGraph->CreateFacet( MeshQuadKey(3, 2, 1, 0) );
        outGraph->CreateFacet( MeshQuadKey(7, 6, 5, 4) );
        outGraph->CreateFacet( MeshQuadKey(3, 5, 6, 2) );
        outGraph->CreateFacet( MeshQuadKey(1, 7, 4, 0) );
        outGraph->CreateFacet( MeshQuadKey(2, 6, 7, 1) );
        outGraph->CreateFacet( MeshQuadKey(4, 5, 3, 0) );
    } else {
        outGraph->CreateFacet( MeshQuadKey(0, 1, 2, 3) );
        outGraph->CreateFacet( MeshQuadKey(4, 5, 6, 7) );
        outGraph->CreateFacet( MeshQuadKey(2, 6, 5, 3) );
        outGraph->CreateFacet( MeshQuadKey(0, 4, 7, 1) );
        outGraph->CreateFacet( MeshQuadKey(1, 7, 6, 2) );
        outGraph->CreateFacet( MeshQuadKey(0, 3, 5, 4) );
    }

    // Sort links
    outGraph->SortVertexLinkage();
    outGraph->SortEdgeLinkage();
}

/////////////////////////////////////////////////////////////////////////////////

Void MeshBox::_Update( GPUDeferredContext * pContext )
{
    // Update vertices
    UInt iVertexSize = m_pIL->GetSize();

    UInt iOffset, iSize;

    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    Byte * arrPositions = m_pVB->GetData() + iOffset;

    Vertex4 * pPosition;

    UInt iAxis, iAxis2, iAxis3, iFace, iFaceSelect, iVertex;
    static const Scalar fAxisSign[2]  = { -1.0f, +1.0f };
    static const Scalar fAxis2Sign[8] = { +1.0f, +1.0f, -1.0f, -1.0f, +1.0f, -1.0f, -1.0f, +1.0f };
    static const Scalar fAxis3Sign[8] = { +1.0f, -1.0f, -1.0f, +1.0f, +1.0f, +1.0f, -1.0f, -1.0f };

    for( iAxis = 0; iAxis < 3; ++iAxis ) {
        iAxis2 = (iAxis+1) % 3;
        iAxis3 = (iAxis+2) % 3;
        for( iFace = 0; iFace < 2; ++iFace ) {
            iFaceSelect = (iFace << 2);
            for( iVertex = 0; iVertex < 4; ++iVertex ) {
                pPosition = (Vertex4*)( arrPositions );
                (*pPosition)[iAxis]  = m_vBox.Center[iAxis]  + ( fAxisSign[iFace]                  * (m_vBox.Extents[iAxis]) );
                (*pPosition)[iAxis2] = m_vBox.Center[iAxis2] + ( fAxis2Sign[iFaceSelect + iVertex] * (m_vBox.Extents[iAxis2]) );
                (*pPosition)[iAxis3] = m_vBox.Center[iAxis3] + ( fAxis3Sign[iFaceSelect + iVertex] * (m_vBox.Extents[iAxis3]) );
                pPosition->W = 1.0f;
                arrPositions += iVertexSize;
            }
        }
    }

    // Done
    m_pVB->Update( 0, INVALID_OFFSET, pContext );
}

