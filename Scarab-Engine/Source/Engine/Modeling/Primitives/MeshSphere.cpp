/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/MeshSphere.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Sphere
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
#include "MeshSphere.h"

#include "../ModelingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshSphere implementation
MeshSphere::MeshSphere( const Sphere & vSphere, UInt iSliceSamples, UInt iRadialSamples, GPUInputLayout * pIL, Bool bInterior ):
    TriangleListMesh(), m_vSphere( vSphere )
{
    Assert( iSliceSamples >= 1 && iRadialSamples >= 3 );
    Assert( pIL != NULL );

    m_iSliceSamples = iSliceSamples;
    m_iRadialSamples = iRadialSamples;

    m_bInterior = bInterior;

    SetIL( pIL );
    
    UInt iSliceSamplesP1 = ( m_iSliceSamples + 1 );
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
    Scalar fInvSliceSamplesP1 = MathFn->Invert( (Scalar)iSliceSamplesP1 );
    Scalar fInvRadialSamples = MathFn->Invert( (Scalar)m_iRadialSamples );

    Scalar fRadiusSqr = ( m_vSphere.Radius * m_vSphere.Radius );
    Scalar fSliceStep = ( 2.0f * m_vSphere.Radius * fInvSliceSamplesP1 );
    Scalar fRadialStep = ( SCALAR_2PI * fInvRadialSamples );

    Scalar fSliceHeight, fSliceU;
    Scalar fSliceRadius;
    Scalar fRadialAngle, fRadialT;
    Scalar fSin, fCos;
    UInt iSlice, iRay;

    Vertex4 * pSavedPosition = NULL;
    Vector4 * pSavedNormal = NULL;

    fSliceHeight = ( -m_vSphere.Radius + fSliceStep );
    fSliceU = ( fInvSliceSamplesP1 );
    fSliceRadius = MathFn->Sqrt( fRadiusSqr - (fSliceHeight * fSliceHeight) );
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
            *pPosition = m_vSphere.Center;
            pPosition->X += ( fCos * fSliceRadius );
            pPosition->Y += ( fSin * fSliceRadius );
            pPosition->Z += fSliceHeight;
            arrPositions += iVertexSize;

            if ( arrNormals != NULL ) {
                pNormal = (Vector4*)( arrNormals );
                *pNormal = ( *pPosition - m_vSphere.Center );
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
        fSliceU += fInvSliceSamplesP1;
        fSliceRadius = MathFn->Sqrt( fRadiusSqr - (fSliceHeight * fSliceHeight) );
    }

        // Build south pole
    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vSphere.Center;
    pPosition->Z -= m_vSphere.Radius;
    arrPositions += iVertexSize;

    if ( arrNormals != NULL ) {
        pNormal = (Vector4*)( arrNormals );
        *pNormal = -(Vector4::eK);
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

        // Build north pole
    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vSphere.Center;
    pPosition->Z += m_vSphere.Radius;
    arrPositions += iVertexSize;

    if ( arrNormals != NULL ) {
        pNormal = (Vector4*)( arrNormals );
        *pNormal = Vector4::eK;
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

        // Build south pole
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

        // Build north pole
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
MeshSphere::~MeshSphere()
{
    // nothing to do
}

Void MeshSphere::GenerateGraph( MeshVoronoiGraph * outGraph, UInt iSliceSamples, UInt iRadialSamples, Bool bInterior )
{
    // Generate facets
    UInt iSliceSamplesM1 = ( iSliceSamples - 1 );
    UInt iRadialSamplesP1 = ( iRadialSamples + 1 );
    UInt iSlice, iRay;

        // Build slices
    UInt iQuad0, iQuad1, iQuad2, iQuad3;
    UInt iLowSliceStart = 0;
    UInt iHighSliceStart = iRadialSamplesP1;
    UInt iNextRay;

    for( iSlice = 0; iSlice < iSliceSamplesM1; ++iSlice ) {
        iQuad0 = iLowSliceStart;
        iQuad1 = iLowSliceStart + 1;
        iQuad2 = iHighSliceStart + 1;
        iQuad3 = iHighSliceStart;
        for( iRay = 0; iRay < iRadialSamples; ++iRay ) {
            if ( bInterior )
                outGraph->CreateFacet( MeshQuadKey(iQuad3, iQuad2, iQuad1, iQuad0) );
            else
                outGraph->CreateFacet( MeshQuadKey(iQuad0, iQuad1, iQuad2, iQuad3) );
            ++iQuad0;
            ++iQuad1;
            if ( iQuad1 >= iLowSliceStart + iRadialSamples )
                iQuad1 = iLowSliceStart;
            ++iQuad2;
            if ( iQuad2 >= iHighSliceStart + iRadialSamples )
                iQuad2 = iHighSliceStart;
            ++iQuad3;
        }
        iLowSliceStart = iHighSliceStart;
        iHighSliceStart += iRadialSamplesP1;
    }

        // Build south pole
    UInt iSouthPole = ( iSliceSamples * iRadialSamplesP1 );
    for( iRay = 0; iRay < iRadialSamples; ++iRay ) {
        iNextRay = (iRay + 1) % iRadialSamples;
        if ( bInterior )
            outGraph->CreateFacet( MeshTriangleKey(iSouthPole, iRay, iNextRay) );
        else
            outGraph->CreateFacet( MeshTriangleKey(iSouthPole, iNextRay, iRay) );
    }

        // Build north pole
    UInt iNorthPole = iSouthPole + 1;
    iHighSliceStart = ( iSliceSamplesM1 * iRadialSamplesP1 );
    for( iRay = 0; iRay < iRadialSamples; ++iRay ) {
        iNextRay = (iRay + 1) % iRadialSamples;
        if ( bInterior )
            outGraph->CreateFacet( MeshTriangleKey(iNorthPole, iHighSliceStart + iNextRay, iHighSliceStart + iRay) );
        else
            outGraph->CreateFacet( MeshTriangleKey(iNorthPole, iHighSliceStart + iRay, iHighSliceStart + iNextRay) );
    }

    // Sort links
    outGraph->SortVertexLinkage();
    outGraph->SortEdgeLinkage();
}

/////////////////////////////////////////////////////////////////////////////////

Void MeshSphere::_Update( GPUDeferredContext * pContext )
{
    // Update vertices
    UInt iVertexSize = m_pIL->GetSize();

    UInt iOffset, iSize;

    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    Byte * arrPositions = m_pVB->GetData() + iOffset;

    Vertex4 * pPosition;

        // Update slices
    Scalar fInvSliceSamplesP1 = MathFn->Invert( (Scalar)(m_iSliceSamples + 1) );
    Scalar fInvRadialSamples = MathFn->Invert( (Scalar)m_iRadialSamples );

    Scalar fRadiusSqr = ( m_vSphere.Radius * m_vSphere.Radius );
    Scalar fSliceStep = ( 2.0f * m_vSphere.Radius * fInvSliceSamplesP1 );
    Scalar fRadialStep = ( SCALAR_2PI * fInvRadialSamples );

    Scalar fSliceHeight;
    Scalar fSliceRadius;
    Scalar fRadialAngle;
    Scalar fSin, fCos;
    UInt iSlice, iRay;

    Vertex4 * pSavedPosition;

    fSliceHeight = ( -m_vSphere.Radius + fSliceStep );
    fSliceRadius = MathFn->Sqrt( fRadiusSqr - (fSliceHeight * fSliceHeight) );
    for( iSlice = 0; iSlice < m_iSliceSamples; ++iSlice ) {
        // Save first ray
        pSavedPosition = (Vertex4*)( arrPositions );

        fRadialAngle = 0.0f;
        for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
            MathFn->SinCos( &fSin, &fCos, fRadialAngle );

            pPosition = (Vertex4*)( arrPositions );
            *pPosition = m_vSphere.Center;
            pPosition->X += ( fCos * fSliceRadius );
            pPosition->Y += ( fSin * fSliceRadius );
            pPosition->Z += fSliceHeight;
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
        fSliceRadius = MathFn->Sqrt( fRadiusSqr - (fSliceHeight * fSliceHeight) );
    }

        // Update south pole
    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vSphere.Center;
    pPosition->Z -= m_vSphere.Radius;
    arrPositions += iVertexSize;

        // Update north pole
    pPosition = (Vertex4*)( arrPositions );
    *pPosition = m_vSphere.Center;
    pPosition->Z += m_vSphere.Radius;
    arrPositions += iVertexSize;

    // Done
    m_pVB->Update( 0, INVALID_OFFSET, pContext );
}

