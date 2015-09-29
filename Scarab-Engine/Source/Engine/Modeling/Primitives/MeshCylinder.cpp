/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/MeshCylinder.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Cylinder
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
#include "MeshCylinder.h"

#include "../ModelingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshCylinder implementation
MeshCylinder::MeshCylinder( const Cylinder & vCylinder, UInt iSliceSamples, UInt iRadialSamples, GPUInputLayout * pIL, Bool bInterior ):
    TriangleListMesh(), m_vCylinder( vCylinder )
{
    Assert( iSliceSamples >= 2 && iRadialSamples >= 3 );
    Assert( pIL != NULL );

    m_iSliceSamples = iSliceSamples;
    m_iRadialSamples = iRadialSamples;

    m_bInterior = bInterior;

    SetIL( pIL );
    
    UInt iSliceSamplesM1 = ( m_iSliceSamples - 1 );
    UInt iRadialSamplesP1 = ( m_iRadialSamples + 1 );

    UInt iVertexSize = m_pIL->GetSize();
    UInt iVertexCount = ( m_iSliceSamples * iRadialSamplesP1 ) + 2;
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
    Scalar fInvSliceSamplesM1 = MathFn->Invert( (Scalar)iSliceSamplesM1 );
    Scalar fInvRadialSamples = MathFn->Invert( (Scalar)m_iRadialSamples );

    Scalar fHalfHeight = ( m_vCylinder.Height * 0.5f );
    Scalar fSliceStep = ( m_vCylinder.Height * fInvSliceSamplesM1 );
    Scalar fRadialStep = ( SCALAR_2PI * fInvRadialSamples );

    Vector4 vAxisX, vAxisY;
    Vector4::MakeComplementBasis( vAxisX, vAxisY, m_vCylinder.Axis );

    Scalar fSliceHeight, fSliceU;
    Vertex4 vSliceCenter;
    Scalar fRadialAngle, fRadialT;
    Scalar fSin, fCos;
    UInt iSlice, iRay;

    Vertex4 * pSavedPosition = NULL;
    Vector4 * pSavedNormal = NULL;

    fSliceHeight = -fHalfHeight;
    fSliceU = 0.0f;
    for( iSlice = 0; iSlice < m_iSliceSamples; ++iSlice ) {
        vSliceCenter = ( m_vCylinder.Center + (m_vCylinder.Axis * fSliceHeight) );

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
            *pPosition += ( vAxisX * (fCos * m_vCylinder.Radius) );
            *pPosition += ( vAxisY * (fSin * m_vCylinder.Radius) );
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
        fSliceHeight += fSliceStep;
        fSliceU += fInvSliceSamplesM1;
    }

        // Build south cap
    pPosition = (Vertex4*)( arrPositions );
    *pPosition = ( m_vCylinder.Center - (m_vCylinder.Axis * fHalfHeight) );
    arrPositions += iVertexSize;

    if ( arrNormals != NULL ) {
        pNormal = (Vector4*)( arrNormals );
        *pNormal = -(m_vCylinder.Axis);
        if ( m_bInterior )
            *pNormal = -(*pNormal);
        arrNormals += iVertexSize;
    }
    if ( arrTexCoords != NULL ) {
        pTexCoord = (TextureCoord2*)( arrTexCoords );
        pTexCoord->T = 0.5f;
        pTexCoord->U = 0.0f;
        arrTexCoords += iVertexSize;
    }

        // Build north cap
    pPosition = (Vertex4*)( arrPositions );
    *pPosition = ( m_vCylinder.Center + (m_vCylinder.Axis * fHalfHeight) );
    arrPositions += iVertexSize;

    if ( arrNormals != NULL ) {
        pNormal = (Vector4*)( arrNormals );
        *pNormal = m_vCylinder.Axis;
        pNormal->Normalize();
        if ( m_bInterior )
            *pNormal = -(*pNormal);
        arrNormals += iVertexSize;
    }
    if ( arrTexCoords != NULL ) {
        pTexCoord = (TextureCoord2*)( arrTexCoords );
        pTexCoord->T = 0.5f;
        pTexCoord->U = 1.0f;
        arrTexCoords += iVertexSize;
    }

    // Generate indices
    UInt * arrIndices = (UInt*)( m_pIB->GetData() );

    UInt iQuad0, iQuad1, iQuad2, iQuad3;

        // Build slices
    UInt iSliceStart = 0;
    for( iSlice = 0; iSlice < iSliceSamplesM1; ++iSlice ) {
        iQuad0 = iSliceStart;
        iQuad1 = iSliceStart + 1;
        iSliceStart += iRadialSamplesP1;
        iQuad2 = iSliceStart + 1;
        iQuad3 = iSliceStart;
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

        // Build south cap
    UInt iSouthPole = iVertexCount - 2;
    for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
        if ( m_bInterior ) {
            *arrIndices++ = iSouthPole;
            *arrIndices++ = iRay;
            *arrIndices++ = iRay + 1;
        } else {
            *arrIndices++ = iSouthPole;
            *arrIndices++ = iRay + 1;
            *arrIndices++ = iRay;
        }
    }

        // Build north cap
    UInt iNorthPole = iVertexCount - 1;
    iSliceStart = ( iSliceSamplesM1 * iRadialSamplesP1 );
    for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
        if ( m_bInterior ) {
            *arrIndices++ = iNorthPole;
            *arrIndices++ = iSliceStart + iRay + 1;
            *arrIndices++ = iSliceStart + iRay;
        } else {
            *arrIndices++ = iNorthPole;
            *arrIndices++ = iSliceStart + iRay;
            *arrIndices++ = iSliceStart + iRay + 1;
        }
    }

    // Bind buffers
    m_pVB->Bind();
    m_pIB->Bind();
}
MeshCylinder::~MeshCylinder()
{
    // nothing to do
}

Void MeshCylinder::GenerateGraph( MeshVoronoiGraph * outGraph, UInt iSliceSamples, UInt iRadialSamples, Bool bInterior )
{
    // Generate facets
    UInt iSliceSamplesM1 = ( iSliceSamples - 1 );
    UInt iRadialSamplesP1 = ( iRadialSamples + 1 );

    UInt iQuad0, iQuad1, iQuad2, iQuad3;
    MeshPolygonKey polyKey(iRadialSamples);

    UInt iRay;

        // Build slices
    UInt iFirstSlice = 0;
    UInt iLastSlice = ( iSliceSamplesM1 * iRadialSamplesP1 );
    iQuad0 = iFirstSlice;
    iQuad1 = iFirstSlice + 1;
    iQuad2 = iLastSlice + 1;
    iQuad3 = iLastSlice;
    for( iRay = 0; iRay < iRadialSamples; ++iRay ) {
        if ( bInterior )
            outGraph->CreateFacet( MeshQuadKey( iQuad3, iQuad2, iQuad1, iQuad0 ) );
        else
            outGraph->CreateFacet( MeshQuadKey( iQuad0, iQuad1, iQuad2, iQuad3 ) );
        ++iQuad0;
        ++iQuad1;
        if ( iQuad1 >= iFirstSlice + iRadialSamples )
            iQuad1 = iFirstSlice;
        ++iQuad2;
        if ( iQuad2 >= iLastSlice + iRadialSamples )
            iQuad2 = iLastSlice;
        ++iQuad3;
    }

        // Build south cap
    for( iRay = 0; iRay < iRadialSamples; ++iRay ) {
        if ( bInterior )
            polyKey.V[iRay] = iFirstSlice + iRay;
        else
            polyKey.V[(iRadialSamples - 1) - iRay] = iFirstSlice + iRay;
    }
    outGraph->CreateFacet( polyKey );

        // Build north cap
    for( iRay = 0; iRay < iRadialSamples; ++iRay ) {
        if ( bInterior )
            polyKey.V[(iRadialSamples - 1) - iRay] = iLastSlice + iRay;
        else
            polyKey.V[iRay] = iLastSlice + iRay;
    }
    outGraph->CreateFacet( polyKey );

    // Sort links
    outGraph->SortVertexLinkage();
    outGraph->SortEdgeLinkage();
}

/////////////////////////////////////////////////////////////////////////////////

Void MeshCylinder::_Update( GPUDeferredContext * pContext )
{
    // Update vertices
    UInt iVertexSize = m_pIL->GetSize();

    UInt iOffset, iSize;

    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    Byte * arrPositions = m_pVB->GetData() + iOffset;

    Vertex4 * pPosition;

        // Update slices
    Scalar fInvSliceSamplesM1 = MathFn->Invert( (Scalar)(m_iSliceSamples - 1) );
    Scalar fInvRadialSamples = MathFn->Invert( (Scalar)m_iRadialSamples );

    Scalar fHalfHeight = ( m_vCylinder.Height * 0.5f );
    Scalar fSliceStep = ( m_vCylinder.Height * fInvSliceSamplesM1 );
    Scalar fRadialStep = ( SCALAR_2PI * fInvRadialSamples );

    Vector4 vAxisX, vAxisY;
    Vector4::MakeComplementBasis( vAxisX, vAxisY, m_vCylinder.Axis );

    Scalar fSliceHeight;
    Vertex4 vSliceCenter;
    Scalar fRadialAngle;
    Scalar fSin, fCos;
    UInt iSlice, iRay;

    Vertex4 * pSavedPosition;

    fSliceHeight = -fHalfHeight;
    for( iSlice = 0; iSlice < m_iSliceSamples; ++iSlice ) {
        vSliceCenter = ( m_vCylinder.Center + (m_vCylinder.Axis * fSliceHeight) );

        // Save first ray
        pSavedPosition = (Vertex4*)( arrPositions );

        fRadialAngle = 0.0f;
        for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
            MathFn->SinCos( &fSin, &fCos, fRadialAngle );

            pPosition = (Vertex4*)( arrPositions );
            *pPosition = vSliceCenter;
            *pPosition += ( vAxisX * (fCos * m_vCylinder.Radius) );
            *pPosition += ( vAxisY * (fSin * m_vCylinder.Radius) );
            arrPositions += iVertexSize;

            // Next ray
            fRadialAngle += fRadialStep;
        }

        // Duplicate first ray
        pPosition = (Vertex4*)( arrPositions );
        *pPosition = *pSavedPosition;
        arrPositions += iVertexSize;

        // Next slice
        fSliceHeight += fSliceStep;
    }

        // Update south cap
    pPosition = (Vertex4*)( arrPositions );
    *pPosition = ( m_vCylinder.Center - (m_vCylinder.Axis * fHalfHeight) );
    arrPositions += iVertexSize;

        // Update north cap
    pPosition = (Vertex4*)( arrPositions );
    *pPosition = ( m_vCylinder.Center + (m_vCylinder.Axis * fHalfHeight) );
    arrPositions += iVertexSize;

    // Done
    m_pVB->Update( 0, INVALID_OFFSET, pContext );
}

