/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/Planar/MeshDisk.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Planar, Disk
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
#include "MeshDisk.h"

#include "../../ModelingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshDisk implementation
MeshDisk::MeshDisk( const Circle3 & vDisk, UInt iSliceSamples, UInt iRadialSamples, GPUInputLayout * pIL ):
    TriangleListMesh(), m_vDisk( vDisk )
{
    Assert( iSliceSamples >= 1 && iRadialSamples >= 3 );
    Assert( pIL != NULL );

    m_iSliceSamples = iSliceSamples;
    m_iRadialSamples = iRadialSamples;

    SetIL( pIL );

    UInt iSliceSamplesM1 = ( m_iSliceSamples - 1 );
    UInt iRadialSamplesP1 = ( m_iRadialSamples + 1 );

    UInt iVertexSize = m_pIL->GetSize();
    UInt iVertexCount = ( m_iSliceSamples * iRadialSamplesP1 ) + 1;
    UInt iTriangleCount = ( (iSliceSamplesM1 * m_iRadialSamples) << 1 ) + m_iRadialSamples; // *2
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

    Scalar fSliceStep = ( m_vDisk.Radius * fInvSliceSamples );
    Scalar fRadialStep = ( SCALAR_2PI * fInvRadialSamples );

    Vector4 vAxisX, vAxisY;
    Vector4::MakeComplementBasis( vAxisX, vAxisY, m_vDisk.Axis );

    Scalar fSliceRadius, fSliceU;
    Scalar fRadialAngle, fRadialT;
    Scalar fSin, fCos;
    UInt iSlice, iRay;

    Vertex4 * pSavedPosition = NULL;
    Vector4 * pSavedNormal = NULL;

    fSliceRadius = fSliceStep;
    fSliceU = fInvSliceSamples;
    for( iSlice = 0; iSlice < m_iSliceSamples; ++iSlice ) {
        // Save first ray
        pSavedPosition = (Vertex4*)( arrPositions );
        if ( arrNormals != NULL )
            pSavedNormal = (Vector4*)( arrNormals );

        fRadialAngle = 0.0f;
        fRadialT = 0.0f;
        for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
            MathFn->SinCos( &fSin, &fCos, fRadialAngle );

            pPosition = (Vertex4*)( arrPositions );
            *pPosition = m_vDisk.Center;
            *pPosition += ( vAxisX * (fCos * fSliceRadius) );
            *pPosition += ( vAxisY * (fSin * fSliceRadius) );
            arrPositions += iVertexSize;

            if ( arrNormals != NULL ) {
                pNormal = (Vector4*)( arrNormals );
                *pNormal = m_vDisk.Axis;
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
        fSliceRadius += fSliceStep;
        fSliceU += fInvSliceSamples;
    }

        // Build center
    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vDisk.Center;
    arrPositions += iVertexSize;

    if ( arrNormals != NULL ) {
        pNormal = (Vector4*)( arrNormals );
        *pNormal = m_vDisk.Axis;
        arrNormals += iVertexSize;
    }
    if ( arrTexCoords != NULL ) {
        pTexCoord = (TextureCoord2*)( arrTexCoords );
        pTexCoord->T = 0.5f;
        pTexCoord->U = 0.0f;
        arrTexCoords += iVertexSize;
    }

    // Generate indices
    UInt * arrIndices = (UInt*)( m_pIB->GetData() );

    UInt iQuad0, iQuad1, iQuad2, iQuad3;

        // Build slices
    UInt iSliceStart = 0;
    for( iSlice = 0; iSlice < iSliceSamplesM1; ++iSlice ) {
        iQuad0 = iSliceStart + 1;
        iQuad1 = iSliceStart;
        iSliceStart += iRadialSamplesP1;
        iQuad2 = iSliceStart;
        iQuad3 = iSliceStart + 1;
        for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
            *arrIndices++ = iQuad0;
            *arrIndices++ = iQuad1;
            *arrIndices++ = iQuad2;
            *arrIndices++ = iQuad0;
            *arrIndices++ = iQuad2;
            *arrIndices++ = iQuad3;
            ++iQuad0;
            ++iQuad1;
            ++iQuad2;
            ++iQuad3;
        }
    }

        // Build center
    UInt iCenter = ( iVertexCount - 1 );
    for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
        *arrIndices++ = iCenter;
        *arrIndices++ = iRay;
        *arrIndices++ = iRay + 1;
    }

    // Bind buffers
    m_pVB->Bind();
    m_pIB->Bind();
}
MeshDisk::~MeshDisk()
{
    // nothing to do
}

Void MeshDisk::GenerateGraph( MeshVoronoiGraph * outGraph, UInt iRadialSamples )
{
    // Generate facets
    UInt i;

    MeshPolygonKey polyKey( iRadialSamples );
    for( i = 0; i < iRadialSamples; ++i )
        polyKey.V[i] = i;
    outGraph->CreateFacet( polyKey );

    for( i = 0; i < iRadialSamples; ++i )
        polyKey.V[i] = iRadialSamples + i;
    outGraph->CreateFacet( polyKey );

    UInt iTopLeft = iRadialSamples - 1;
    UInt iBottomLeft = iRadialSamples + 1;
    UInt iTopRight = 0;
    UInt iBottomRight = iRadialSamples;
    outGraph->CreateFacet( MeshQuadKey(iTopLeft, iBottomLeft, iBottomRight, iTopRight) );

    iTopLeft = iTopRight;
    iBottomLeft = iBottomRight;
    iTopRight = 1;
    iBottomRight = (iRadialSamples << 1) - 1;
    outGraph->CreateFacet( MeshQuadKey(iTopLeft, iBottomLeft, iBottomRight, iTopRight) );

    for( i = 2; i < iRadialSamples; ++i ) {
        iTopLeft = iTopRight;
        iBottomLeft = iBottomRight;
        ++iTopRight;
        --iBottomRight;
        outGraph->CreateFacet( MeshQuadKey(iTopLeft, iBottomLeft, iBottomRight, iTopRight) );
    }

    // Sort links
    outGraph->SortVertexLinkage();
    outGraph->SortEdgeLinkage();
}

/////////////////////////////////////////////////////////////////////////////////

Void MeshDisk::_Update( GPUDeferredContext * pContext )
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

    Scalar fSliceStep = ( m_vDisk.Radius * fInvSliceSamples );
    Scalar fRadialStep = ( SCALAR_2PI * fInvRadialSamples );

    Vector4 vAxisX, vAxisY;
    Vector4::MakeComplementBasis( vAxisX, vAxisY, m_vDisk.Axis );

    Scalar fSliceRadius;
    Scalar fRadialAngle;
    Scalar fSin, fCos;
    UInt iSlice, iRay;

    Vertex4 * pSavedPosition;

    fSliceRadius = fSliceStep;
    for( iSlice = 0; iSlice < m_iSliceSamples; ++iSlice ) {

        // Save first ray
        pSavedPosition = (Vertex4*)( arrPositions );

        fRadialAngle = 0.0f;
        for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
            MathFn->SinCos( &fSin, &fCos, fRadialAngle );

            pPosition = (Vertex4*)( arrPositions );
            *pPosition = m_vDisk.Center;
            *pPosition += ( vAxisX * (fCos * fSliceRadius) );
            *pPosition += ( vAxisY * (fSin * fSliceRadius) );
            arrPositions += iVertexSize;

            // Next ray
            fRadialAngle += fRadialStep;
        }

        // Duplicate first ray
        pPosition = (Vertex4*)( arrPositions );
        *pPosition = *pSavedPosition;
        arrPositions += iVertexSize;

        // Next slice
        fSliceRadius += fSliceStep;
    }

        // Update center
    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vDisk.Center;

    // Done
    m_pVB->Update( 0, INVALID_OFFSET, pContext );
}

