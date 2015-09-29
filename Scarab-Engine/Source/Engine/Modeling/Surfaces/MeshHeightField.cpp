/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Surfaces/MeshHeightField.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh Height-Field surface (landscapes)
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
#include "MeshHeightField.h"

#include "../ModelingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshHeightField implementation
MeshHeightField::MeshHeightField( Scalar * arrHeightField, const Vertex2 & vOrigin, const Vector2 & vSize, Scalar fMinHeight, Scalar fMaxHeight,
                                  UInt iSampleCountU, UInt iSampleCountV, GPUInputLayout * pIL ):
    TriangleListMesh(), m_vOrigin( vOrigin ), m_vSize( vSize ), m_vSampleSize(), m_vInvSampleSize()
{
    Assert( arrHeightField != NULL );
	Assert( iSampleCountU >= 2 && iSampleCountV >= 2 );
    Assert( pIL != NULL );

    m_arrHeightField = arrHeightField;
    m_fMinHeight = fMinHeight;
    m_fMaxHeight = fMaxHeight;

    m_iSampleCountU = iSampleCountU;
    m_iSampleCountV = iSampleCountV;

    m_vSampleSize.X = ( vSize.X / (Scalar)(iSampleCountU - 1) );
    m_vSampleSize.Y = ( vSize.Y / (Scalar)(iSampleCountV - 1) );
    m_vInvSampleSize.X = MathFn->Invert( m_vSampleSize.X );
    m_vInvSampleSize.Y = MathFn->Invert( m_vSampleSize.Y );
    m_fSampleDiagSlope = ( m_vSampleSize.Y * m_vInvSampleSize.X );

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

    Byte * arrTexCoords = NULL;
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0 );
        arrTexCoords = m_pVB->GetData() + iOffset;
    }

    Vertex4 * pPosition;
    TextureCoord2 * pTexCoord;

    Scalar fInvUM1 = MathFn->Invert( (Scalar)iSampleCountUM1 );
    Scalar fInvVM1 = MathFn->Invert( (Scalar)iSampleCountVM1 );

    Scalar fX, fY, fT, fU;
    UInt iX, iY, iIndex = 0;

    fU = 0.0f;
    fY = 0.0f;
    for( iY = 0; iY < m_iSampleCountV; ++iY ) {
        fT = 0.0f;
        fX = 0.0f;
        for( iX = 0; iX < m_iSampleCountU; ++iX ) {
            pPosition = (Vertex4*)( arrPositions );
            pPosition->X = m_vOrigin.X + fX;
            pPosition->Y = m_vOrigin.Y + fY;
            pPosition->Z = m_arrHeightField[iIndex++];
            pPosition->W = 1.0f;
            arrPositions += iVertexSize;

            if ( arrTexCoords != NULL ) {
                pTexCoord = (TextureCoord2*)( arrTexCoords );
                pTexCoord->T = fT;
                pTexCoord->U = fU;
                arrTexCoords += iVertexSize;
            }

            fT += fInvUM1;
            fX += m_vSampleSize.X;
        }
        fU += fInvVM1;
        fY += m_vSampleSize.Y;
    }

    // Generate indices
    UInt * arrIndices = (UInt*)( m_pIB->GetData() );

    UInt iQuad0, iQuad1, iQuad2, iQuad3;

    for( iY = 0; iY < iSampleCountVM1; ++iY ) {
        for( iX = 0; iX < iSampleCountUM1; ++iX ) {
            iQuad0 = (iY * m_iSampleCountU) + iX;
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

    // Generate normals
    //UpdateNormals();
}
MeshHeightField::~MeshHeightField()
{
    // nothing to do
}

Scalar MeshHeightField::GetHeight( const Vertex2 & vPosition ) const
{
    // Clip to field's size
    Vector2 vLocalPos = ( vPosition - m_vOrigin );
    if ( vLocalPos.X < 0.0f || vLocalPos.X >= m_vSize.X )
        return 0.0f;
    if ( vLocalPos.Y < 0.0f || vLocalPos.Y >= m_vSize.Y )
        return 0.0f;

    // Rasterize, top-left convention
    Scalar fGridX = ( vLocalPos.X * m_vInvSampleSize.X );
    Scalar fGridY = ( vLocalPos.Y * m_vInvSampleSize.Y );

    UInt iGridX = (UInt)( MathFn->Floor(fGridX) );
    UInt iGridY = (UInt)( MathFn->Floor(fGridY) );
    Scalar fOffsetX = ( fGridX - (Scalar)iGridX );
    Scalar fOffsetY = ( fGridY - (Scalar)iGridY );
    Scalar fDiagY = ( m_fSampleDiagSlope * fOffsetX );
    Bool bSecondTriangle = ( fOffsetY > fDiagY );

    UInt iSampleIndex = ( iGridY * m_iSampleCountU ) + iGridX;
    Scalar fNormalizedX = ( fOffsetX * m_vInvSampleSize.X );
    Scalar fNormalizedY = ( fOffsetY * m_vInvSampleSize.Y );

    // Bilinear interpolation
    Scalar fA, fB, fC;
    if ( bSecondTriangle ) {
        fA = m_arrHeightField[iSampleIndex];
        fB = m_arrHeightField[iSampleIndex + m_iSampleCountU + 1];
        fC = m_arrHeightField[iSampleIndex + m_iSampleCountU];
    } else {
        fA = m_arrHeightField[iSampleIndex];
        fB = m_arrHeightField[iSampleIndex + 1];
        fC = m_arrHeightField[iSampleIndex + m_iSampleCountU + 1];
    }
    Scalar fTmp = (1.0f - fNormalizedY) * fB + fNormalizedY * fC;
    Scalar fHeight = (1.0f - fNormalizedX) * fA + fNormalizedX * fTmp;

    return fHeight;
}
Vector3 MeshHeightField::GetNormal( const Vertex2 & vPosition ) const
{
    Vertex2 vTmp;
    vTmp.Y = vPosition.Y;

    vTmp.X = vPosition.X + m_vSampleSize.X;
    Scalar fHp0 = GetHeight( vTmp );

    vTmp.X = vPosition.X - m_vSampleSize.X;
    Scalar fHm0 = GetHeight( vTmp );

    vTmp.X = vPosition.X;

    vTmp.Y = vPosition.Y + m_vSampleSize.Y;
    Scalar fH0p = GetHeight( vTmp );

    vTmp.Y = vPosition.Y - m_vSampleSize.Y;
    Scalar fH0m = GetHeight( vTmp );

    Vector3 vNormal( fHp0 - fHm0, fH0p - fH0m, 1.0f );
    vNormal.Normalize();

    return vNormal;
}

UInt MeshHeightField::GetTriangleIndex( const Vertex2 & vPosition, UInt * outGridX, UInt * outGridY, Bool * outSecondTriangle ) const
{
    // Clip to field's size
    Vector2 vLocalPos = ( vPosition - m_vOrigin );
    if ( vLocalPos.X < 0.0f || vLocalPos.X >= m_vSize.X )
        return INVALID_OFFSET;
    if ( vLocalPos.Y < 0.0f || vLocalPos.Y >= m_vSize.Y )
        return INVALID_OFFSET;

    // Rasterize, top-left convention
    Scalar fGridX = ( vLocalPos.X * m_vInvSampleSize.X );
    Scalar fGridY = ( vLocalPos.Y * m_vInvSampleSize.Y );

    UInt iGridX = (UInt)( MathFn->Floor(fGridX) );
    UInt iGridY = (UInt)( MathFn->Floor(fGridY) );
    Scalar fOffsetX = ( fGridX - (Scalar)iGridX );
    Scalar fOffsetY = ( fGridY - (Scalar)iGridY );
    Scalar fDiagY = ( m_fSampleDiagSlope * fOffsetX );
    Bool bSecondTriangle = ( fOffsetY > fDiagY );

    // Grid coordinates
    if ( outGridX != NULL )
        *outGridX = iGridX;
    if ( outGridY != NULL )
        *outGridY = iGridY;

    if ( outSecondTriangle != NULL )
        *outSecondTriangle = bSecondTriangle;
    return ( (iGridY * ((m_iSampleCountU - 1) << 1)) + (iGridX << 1) + (bSecondTriangle ? 1 : 0) );
}

Void MeshHeightField::GenerateGraph( MeshManifold3 * outGraph, UInt iSampleCountX, UInt iSampleCountY )
{
    // Generate facets
    UInt iSampleCountXM1 = iSampleCountX - 1;
    UInt iSampleCountYM1 = iSampleCountY - 1;

    UInt iQuad0, iQuad1, iQuad2, iQuad3;

    for( UInt iY = 0; iY < iSampleCountYM1; ++iY ) {
        for( UInt iX = 0; iX < iSampleCountXM1; ++iX ) {
            iQuad0 = (iY * iSampleCountX) + iX;
            iQuad1 = iQuad0 + 1;
            iQuad2 = iQuad1 + iSampleCountX;
            iQuad3 = iQuad0 + iSampleCountX;
            outGraph->CreateFacet( MeshTriangleKey(iQuad0, iQuad1, iQuad2) );
            outGraph->CreateFacet( MeshTriangleKey(iQuad0, iQuad2, iQuad3) );
        }
    }

    // Sort links
    outGraph->SortVertexLinkage();
    outGraph->SortEdgeLinkage();
}

/////////////////////////////////////////////////////////////////////////////////

Void MeshHeightField::_Update( GPUDeferredContext * pContext )
{
    // Update vertices
    UInt iVertexSize = m_pIL->GetSize();

    UInt iOffset, iSize;

    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    Byte * arrPositions = m_pVB->GetData() + iOffset;

    Vertex4 * pPosition;

    Scalar fX, fY;
    UInt iX, iY, iIndex = 0;

    fY = 0.0f;
    for( iY = 0; iY < m_iSampleCountV; ++iY ) {
        fX = 0.0f;
        for( iX = 0; iX < m_iSampleCountU; ++iX ) {
            pPosition = (Vertex4*)( arrPositions );
            pPosition->X = m_vOrigin.X + fX;
            pPosition->Y = m_vOrigin.Y + fY;
            pPosition->Z = m_arrHeightField[iIndex++];
            pPosition->W = 1.0f;
            arrPositions += iVertexSize;

            fX += m_vSampleSize.X;
        }
        fY += m_vSampleSize.Y;
    }

    // Done
    m_pVB->Update( 0, INVALID_OFFSET, pContext );

    // Update normals
    //UpdateNormals();
}

