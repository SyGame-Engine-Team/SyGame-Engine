/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/MeshTorus.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Torus
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
#include "MeshTorus.h"

#include "../ModelingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshTorus implementation
MeshTorus::MeshTorus( const Torus & vTorus, UInt iSliceSamples, UInt iRadialSamples, GPUInputLayout * pIL, Bool bInterior ):
    TriangleListMesh(), m_vTorus( vTorus )
{
    Assert( iSliceSamples >= 3 && iRadialSamples >= 3 );
    Assert( pIL != NULL );

    m_iSliceSamples = iSliceSamples;
    m_iRadialSamples = iRadialSamples;

    m_bInterior = bInterior;

    SetIL( pIL );
    
    UInt iSliceSamplesP1 = ( m_iSliceSamples + 1 );
    UInt iRadialSamplesP1 = ( m_iRadialSamples + 1 );

    UInt iVertexSize = m_pIL->GetSize();
    UInt iVertexCount = ( iSliceSamplesP1 * iRadialSamplesP1 );
    UInt iTriangleCount = ( (m_iSliceSamples * m_iRadialSamples) << 1 ); // *2
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

        // Build slices
    Scalar fInvSliceSamples = MathFn->Invert( (Scalar)m_iSliceSamples );
    Scalar fInvRadialSamples = MathFn->Invert( (Scalar)m_iRadialSamples );

    Scalar fSliceStep = ( SCALAR_2PI * fInvSliceSamples );
    Scalar fRadialStep = ( SCALAR_2PI * fInvRadialSamples );

    Vector4 vAxisX, vAxisY;
    Vector4::MakeComplementBasis( vAxisX, vAxisY, m_vTorus.Axis );

    Scalar fSliceAngle, fSliceU;
    Vector4 vSliceRay;
    Vertex4 vSliceCenter;
    Scalar fRadialAngle, fRadialT;
    Scalar fSin, fCos;
    UInt iSlice, iRay;

    Byte * arrFirstSlicePositions = arrPositions;
    Byte * arrFirstSliceNormals = arrNormals;
    Byte * arrFirstSliceTexCoords = arrTexCoords;

    Vertex4 * pSavedPosition = NULL;
    Vector4 * pSavedNormal = NULL;
    TextureCoord2 * pSavedTexCoord = NULL;

    fSliceAngle = 0.0f;
    fSliceU = 0.0f;
    for( iSlice = 0; iSlice < m_iSliceSamples; ++iSlice ) {
        MathFn->SinCos( &fSin, &fCos, fSliceAngle );
        vSliceRay = ( (vAxisX * fCos) + (vAxisY * fSin) );
        vSliceCenter = ( m_vTorus.Center + (vSliceRay * m_vTorus.OuterRadius) );

        // Save first ray
        pSavedPosition = (Vertex4*)( arrPositions );
        if ( arrNormals != NULL )
            pSavedNormal = (Vector4*)( arrNormals );

        fRadialAngle = 0.0f;
        fRadialT = 0.0f;
        for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
            MathFn->SinCos( &fSin, &fCos, fRadialAngle );

            pPosition = (Vertex4*)( arrPositions );
            *pPosition = vSliceCenter;
            *pPosition += ( vSliceRay * (fCos * m_vTorus.InnerRadius) );
            *pPosition += ( m_vTorus.Axis * (fSin * m_vTorus.InnerRadius) );
            arrPositions += iVertexSize;

            if ( arrNormals != NULL ) {
                pNormal = (Vector4*)( arrNormals );
                *pNormal = ( *pPosition - vSliceCenter );
                pNormal->Normalize();
                if ( m_bInterior )
                    *pNormal = -(*pNormal);
                arrNormals += iVertexSize;
            }
            if ( arrTexCoords != NULL ) {
                pTexCoord = (TextureCoord2*)( arrTexCoords );
                pTexCoord->T = fRadialT;
                pTexCoord->U = fSliceU;
                arrTexCoords += iVertexSize;
            }

            // Next ray
            fRadialAngle += fRadialStep;
            fRadialT += fInvRadialSamples;
        }

        // Duplicate first ray
        pPosition = (Vertex4*)( arrPositions );
        *pPosition = *pSavedPosition;
        arrPositions += iVertexSize;

        if ( arrNormals != NULL ) {
            pNormal = (Vector4*)( arrNormals );
            *pNormal = *pSavedNormal;
            arrNormals += iVertexSize;
        }
        if ( arrTexCoords != NULL ) {
            pTexCoord = (TextureCoord2*)( arrTexCoords );
            pTexCoord->T = 1.0f;
            pTexCoord->U = fSliceU;
            arrTexCoords += iVertexSize;
        }

        // Next slice
        fSliceAngle += fSliceStep;
        fSliceU += fInvSliceSamples;
    }

        // Duplicate first slice
    for( iRay = 0; iRay < iRadialSamplesP1; ++iRay ) {
        pPosition = (Vertex4*)( arrPositions );
        pSavedPosition = (Vertex4*)( arrFirstSlicePositions );
        *pPosition = *pSavedPosition;
        arrPositions += iVertexSize;
        arrFirstSlicePositions += iVertexSize;

        if ( arrNormals != NULL ) {
            pNormal = (Vector4*)( arrNormals );
            pSavedNormal = (Vector4*)( arrFirstSliceNormals );
            *pNormal = *pSavedNormal;
            arrNormals += iVertexSize;
            arrFirstSliceNormals += iVertexSize;
        }
        if ( arrTexCoords != NULL ) {
            pTexCoord = (TextureCoord2*)( arrTexCoords );
            pSavedTexCoord = (TextureCoord2*)( arrFirstSliceTexCoords );
            pTexCoord->T = pSavedTexCoord->T;
            pTexCoord->U = 1.0f;
            arrTexCoords += iVertexSize;
            arrFirstSliceTexCoords += iVertexSize;
        }
    }

    // Generate indices
    UInt * arrIndices = (UInt*)( m_pIB->GetData() );

    UInt iQuad0, iQuad1, iQuad2, iQuad3;

    UInt iSliceStart = 0;
    for( iSlice = 0; iSlice < m_iSliceSamples; ++iSlice ) {
        iQuad0 = iSliceStart + 1;
        iQuad1 = iSliceStart;
        iSliceStart += iRadialSamplesP1;
        iQuad2 = iSliceStart;
        iQuad3 = iSliceStart + 1;
        for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
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
            ++iQuad0;
            ++iQuad1;
            ++iQuad2;
            ++iQuad3;
        }
    }

    // Bind buffers
    m_pVB->Bind();
    m_pIB->Bind();
}
MeshTorus::~MeshTorus()
{
    // nothing to do
}

//Void MeshTorus::GenerateGraph( MeshConcaveGraph * outGraph, UInt iSliceSamples, UInt iRadialSamples, Bool bInterior )
//{
//    // Generate facets
//    UInt iRadialSamplesP1 = ( iRadialSamples + 1 );
//
//    UInt iQuad0, iQuad1, iQuad2, iQuad3;
//
//    UInt iSliceStart = 0;
//    for( UInt iSlice = 0; iSlice < iSliceSamples; ++iSlice ) {
//        iQuad0 = iSliceStart + 1;
//        iQuad1 = iSliceStart;
//        iSliceStart += iRadialSamplesP1;
//        iQuad2 = iSliceStart;
//        iQuad3 = iSliceStart + 1;
//        for( UInt iRay = 0; iRay < iRadialSamples; ++iRay ) {
//            if ( bInterior )
//                outGraph->CreateFacet( MeshQuadKey( iQuad3, iQuad2, iQuad1, iQuad0 ) );
//            else
//                outGraph->CreateFacet( MeshQuadKey( iQuad0, iQuad1, iQuad2, iQuad3 ) );
//            iQuad0 = (iQuad0 + 1) % iRadialSamples;
//            ++iQuad1;
//            ++iQuad2;
//            iQuad3 = (iQuad3 + 1) % iRadialSamples;
//        }
//    }
//
//    // Sort links
//    outGraph->SortVertexLinkage();
//    outGraph->SortEdgeLinkage();
//}

/////////////////////////////////////////////////////////////////////////////////

Void MeshTorus::_Update( GPUDeferredContext * pContext )
{
    // Update vertices
    UInt iVertexSize = m_pIL->GetSize();

    UInt iOffset, iSize;

    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    Byte * arrPositions = m_pVB->GetData() + iOffset;

    Vertex4 * pPosition;

        // Update slices
    Scalar fInvSliceSamples = MathFn->Invert( (Scalar)m_iSliceSamples );
    Scalar fInvRadialSamples = MathFn->Invert( (Scalar)m_iRadialSamples );

    Scalar fSliceStep = ( SCALAR_2PI * fInvSliceSamples );
    Scalar fRadialStep = ( SCALAR_2PI * fInvRadialSamples );

    Vector4 vAxisX, vAxisY;
    Vector4::MakeComplementBasis( vAxisX, vAxisY, m_vTorus.Axis );

    Scalar fSliceAngle;
    Vector4 vSliceRay;
    Vertex4 vSliceCenter;
    Scalar fRadialAngle;
    Scalar fSin, fCos;
    UInt iSlice, iRay;

    Byte * arrFirstSlicePositions = arrPositions;

    Vertex4 * pSavedPosition;

    fSliceAngle = 0.0f;
    for( iSlice = 0; iSlice < m_iSliceSamples; ++iSlice ) {
        MathFn->SinCos( &fSin, &fCos, fSliceAngle );
        vSliceRay = ( (vAxisX * fCos) + (vAxisY * fSin) );
        vSliceCenter = ( m_vTorus.Center + (vSliceRay * m_vTorus.OuterRadius) );

        // Save first ray
        pSavedPosition = (Vertex4*)( arrPositions );

        fRadialAngle = 0.0f;
        for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
            MathFn->SinCos( &fSin, &fCos, fRadialAngle );

            pPosition = (Vertex4*)( arrPositions );
            *pPosition = vSliceCenter;
            *pPosition += ( vSliceRay * (fCos * m_vTorus.InnerRadius) );
            *pPosition += ( m_vTorus.Axis * (fSin * m_vTorus.InnerRadius) );
            arrPositions += iVertexSize;

            // Next ray
            fRadialAngle += fRadialStep;
        }

        // Duplicate first ray
        pPosition = (Vertex4*)( arrPositions );
        *pPosition = *pSavedPosition;
        arrPositions += iVertexSize;

        // Next slice
        fSliceAngle += fSliceStep;
    }

        // Duplicate first slice
    UInt iRadialSamplesP1 = ( m_iRadialSamples + 1 );
    for( iRay = 0; iRay < iRadialSamplesP1; ++iRay ) {
        pPosition = (Vertex4*)( arrPositions );
        pSavedPosition = (Vertex4*)( arrFirstSlicePositions );
        *pPosition = *pSavedPosition;
        arrPositions += iVertexSize;
        arrFirstSlicePositions += iVertexSize;
    }

    // Done
    m_pVB->Update( 0, INVALID_OFFSET, pContext );
}

