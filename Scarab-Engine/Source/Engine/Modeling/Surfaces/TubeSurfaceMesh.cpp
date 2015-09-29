/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Curves/TubeSurfaceMesh.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curved Surface Mesh, Tube surfaces
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
#include "TubeSurfaceMesh.h"

#include "../ModelingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// TubeSurfaceMesh implementation
//TubeSurfaceMesh::TubeSurfaceMesh( Curve3 * pCurve, RadiusFunction pfRadial, const Vector3 & vUpVect,
//                                  UInt iCurveSamples, UInt iRadialSamples,
//                                  Bool bUseArcLengthSampling, Bool bClosed, Bool bInterior,
//                                  const VertexFormat * pVF, Bool bDynamic,
//                                  const TextureCoord2 * pTexCoordMin, const TextureCoord2 * pTexCoordMax ):
//    WorldTriangleMesh()
//{
//    Assert( pCurve != NULL );
//    Assert( pfRadial != NULL );
//    Assert( pVF != NULL );
//
//    m_pVF = pVF;
//
//    m_bDynamic = bDynamic;
//
//    m_pCurve = pCurve;
//    m_pfRadial = pfRadial;
//    m_vUpVect = vUpVect;
//    m_iCurveSamples = iCurveSamples;
//    m_iRadialSamples = iRadialSamples;
//    m_bArcLengthSampling = bUseArcLengthSampling;
//    m_bClosed = bClosed;
//
//    UInt i;
//
//    ModelingFn->SelectMemory( TEXT("Curves") );
//
//    // Compute slice coefficients on a unit circle
//    m_arrSin = KNew() Scalar[m_iRadialSamples + 1];
//    m_arrCos = KNew() Scalar[m_iRadialSamples + 1];
//    Scalar fRadialFactor = SCALAR_2PI * MathFn->Invert( (Scalar)m_iRadialSamples );
//    Scalar fAngle = 0.0f;
//    for( i = 0; i < m_iRadialSamples; ++i ) {
//        MathFn->SinCos( fAngle, m_arrSin + i, m_arrCos + i );
//        fAngle += fRadialFactor;
//    }
//    m_arrSin[m_iRadialSamples] = m_arrSin[0];
//    m_arrCos[m_iRadialSamples] = m_arrCos[0];
//
//    ModelingFn->UnSelectMemory();
//
//    // Create VB
//    UInt iVertexSize = m_pVF->GetTotalSize();
//    UInt iVertexCount;
//    if ( m_bClosed )
//        iVertexCount = (m_iRadialSamples + 1) * (m_iCurveSamples + 1);
//    else
//        iVertexCount = (m_iRadialSamples + 1) * m_iCurveSamples;
//    ModelingFn->SelectMemory( TEXT("Geometry") );
//    m_pVB = KNew() VertexBuffer (
//        (m_bDynamic) ? VB_USAGE_DYNAMIC : VB_USAGE_STATIC,
//        iVertexSize, iVertexCount
//    );
//    ModelingFn->UnSelectMemory();
//
//    // build vertices fields
//    _Update_Vertices();
//    if ( HasVertexField(VFUSAGE_NORMAL) )
//        _Update_Normals();
//    for( i = 0; i < VF_MAX_TEXCOORDS; ++i ) {
//        if ( HasVertexField(VFUSAGE_TEXCOORD, i) ) {
//            if ( pTexCoordMin != NULL && pTexCoordMax != NULL )
//                _Build_TexCoords( i, *pTexCoordMin, *pTexCoordMax );
//            else
//                _Build_TexCoords( i, TextureCoord2(0.0f,0.0f), TextureCoord2(1.0f,1.0f) );
//        }
//    }
//
//    // build indices
//    _Build_Indices( bInterior );
//
//    if ( !m_bDynamic ) {
//        ModelingFn->SelectMemory( TEXT("Curves") );
//        KDeleteA( m_arrSin );
//        m_arrSin = NULL;
//        KDeleteA( m_arrCos );
//        m_arrCos = NULL;
//        ModelingFn->UnSelectMemory();
//    }
//}
//TubeSurfaceMesh::~TubeSurfaceMesh()
//{
//    ModelingFn->SelectMemory( TEXT("Curves") );
//    if ( m_arrSin != NULL )
//        KDeleteA( m_arrSin );
//    if ( m_arrCos != NULL )
//        KDeleteA( m_arrCos );
//    ModelingFn->UnSelectMemory();
//
//    ModelingFn->SelectMemory( TEXT("Geometry") );
//    if ( m_pVB != NULL )
//        KDelete( m_pVB );
//    if ( m_pIB != NULL )
//        KDelete( m_pIB );
//    ModelingFn->UnSelectMemory();
//}
//
//Void TubeSurfaceMesh::Update()
//{
//    Assert( m_bDynamic );
//
//    _Update_Vertices();
//    if ( HasVertexField(VFUSAGE_NORMAL) )
//        _Update_Normals();
//
//    UpdateModel( MODEL_UPDATE_BV_ONLY );
//}
//
//Void TubeSurfaceMesh::GetStartSlice( Vertex3 * outSlice )
//{
//    UInt iVertexSize = GetVertexSize();
//    const Byte * pVertex = GetVertexField( 0, VFUSAGE_POSITION );
//    for( UInt i = 0; i <= m_iRadialSamples; ++i ) {
//        outSlice[i] = *( (const Vertex3 *)pVertex );
//        pVertex += iVertexSize;
//    }
//}
//Void TubeSurfaceMesh::GetEndSlice( Vertex3 * outSlice )
//{
//    UInt iVertexSize = GetVertexSize();
//    const Byte * pVertex = GetVertexField( GetVertexCount() - m_iRadialSamples - 1, VFUSAGE_POSITION );
//    for( UInt i = 0; i <= m_iRadialSamples; ++i ) {
//        outSlice[i] = *( (const Vertex3 *)pVertex );
//        pVertex += iVertexSize;
//    }
//}

/////////////////////////////////////////////////////////////////////////////////

//Void TubeSurfaceMesh::_Update_Vertices()
//{
//    Scalar fMinT = m_pCurve->GetMinT();
//    Scalar fRangeT = m_pCurve->GetMaxT() - fMinT;
//    Scalar fCurveLength = 0.0f;
//    if ( m_bArcLengthSampling )
//        fCurveLength = m_pCurve->Length();
//    Scalar fCurveFactor;
//    if ( m_bClosed )
//        fCurveFactor = MathFn->Invert( (Scalar)m_iCurveSamples );
//    else
//        fCurveFactor = MathFn->Invert( (Scalar)(m_iCurveSamples - 1) );
//
//    UInt iVertexSize = GetVertexSize();
//    Byte * pVertexPosition = GetVertexField( 0, VFUSAGE_POSITION );
//    Byte *pVertex, *pSavedVertex;
//    Vertex3 *pPosition, *pSavedPosition;
//
//    pVertex = pVertexPosition;
//    Scalar fT, fRadius;
//    Vertex3 vPosition;
//    Vector3 vNormal, vTangent, vBiNormal;
//    UInt iCurve, i;
//    for( iCurve = 0; iCurve < m_iCurveSamples; ++iCurve ) {
//        if ( m_bArcLengthSampling )
//            fT = m_pCurve->ParamT( iCurve * fCurveLength * fCurveFactor );
//        else
//            fT = fMinT + ( iCurve * fRangeT * fCurveFactor );
//        fRadius = m_pfRadial(fT);
//        if ( m_vUpVect == Vector3::Null ) {
//            // Frenet frame
//            m_pCurve->Frame( fT, vPosition, vTangent, vNormal, vBiNormal );
//        } else {
//            // Extract from tangent
//            vPosition = m_pCurve->P( fT );
//            vTangent = m_pCurve->Tangent( fT );
//            vBiNormal = (vTangent ^ m_vUpVect);
//            vNormal = (vBiNormal ^ vTangent);
//        }
//
//        pSavedVertex = pVertex;
//        for( i = 0; i < m_iRadialSamples; ++i ) {
//            pPosition = (Vertex3*)pVertex;
//            *pPosition = vPosition + ( ( (vNormal * m_arrCos[i]) + (vBiNormal * m_arrSin[i]) ) * fRadius );
//            pVertex += iVertexSize;
//        }
//        pPosition = (Vertex3*)pVertex;
//        pSavedPosition = (Vertex3*)pSavedVertex;
//        *pPosition = *pSavedPosition;
//
//        pVertex += iVertexSize;
//    }
//
//    if ( m_bClosed ) {
//        for( i = 0; i <= m_iRadialSamples; ++i ) {
//            pPosition = (Vertex3*)pVertex;
//            pSavedPosition = (Vertex3*)pVertexPosition;
//            *pPosition = *pSavedPosition;
//            pVertex += iVertexSize;
//            pVertexPosition += iVertexSize;
//        }
//    }
//}
//Void TubeSurfaceMesh::_Update_Normals()
//{
//    UInt iVertexSize = GetVertexSize();
//    const Byte * pVertexPosition = GetVertexField( 0, VFUSAGE_POSITION );
//    Byte * pVertexNormal = GetVertexField( 0, VFUSAGE_NORMAL );
//    const Vertex3 *pPosA, *pPosB, *pPosC, *pPosD;
//    Vector3 *pNormal, *pFirstNormal;
//
//    // Interior normals
//    UInt iCurve, iCurveM1, iCurveP1;
//    UInt iRay, iRayM1, iRayP1;
//    Vector3 vDir0, vDir1;
//    for( iCurve = 1; iCurve < m_iCurveSamples - 1; ++iCurve ) {
//        for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
//            iRayM1 = (iRay > 0) ? (iRay - 1) : (m_iRadialSamples - 1);
//            iRayP1 = iRay + 1;
//            iCurveM1 = iCurve - 1;
//            iCurveP1 = iCurve + 1;
//            pPosA = (const Vertex3 *)( pVertexPosition + (_GetIndex(iCurve, iRayM1) * iVertexSize) );
//            pPosB = (const Vertex3 *)( pVertexPosition + (_GetIndex(iCurve, iRayP1) * iVertexSize) );
//            pPosC = (const Vertex3 *)( pVertexPosition + (_GetIndex(iCurveM1, iRay) * iVertexSize) );
//            pPosD = (const Vertex3 *)( pVertexPosition + (_GetIndex(iCurveP1, iRay) * iVertexSize) );
//            vDir0 = ( *pPosA - *pPosB );
//            vDir1 = ( *pPosC - *pPosD );
//            pNormal = (Vector3*)( pVertexNormal + (_GetIndex(iCurve, iRay) * iVertexSize) );
//            *pNormal = (vDir0 ^ vDir1);
//            pNormal->Normalize();
//        }
//        pNormal = (Vector3*)( pVertexNormal + (_GetIndex(iCurve, m_iRadialSamples) * iVertexSize) );
//        pFirstNormal = (Vector3*)( pVertexNormal + (_GetIndex(iCurve, 0) * iVertexSize) );
//        *pNormal = *pFirstNormal;
//    }
//
//    // Boundary normals
//    if ( m_bClosed ) {
//        for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
//            iRayM1 = (iRay > 0) ? (iRay - 1) : (m_iRadialSamples - 1);
//            iRayP1 = iRay + 1;
//            pPosA = (const Vertex3 *)( pVertexPosition + (_GetIndex(0, iRayM1) * iVertexSize) );
//            pPosB = (const Vertex3 *)( pVertexPosition + (_GetIndex(0, iRayP1) * iVertexSize) );
//            pPosC = (const Vertex3 *)( pVertexPosition + (_GetIndex(m_iCurveSamples - 1, iRay) * iVertexSize) );
//            pPosD = (const Vertex3 *)( pVertexPosition + (_GetIndex(1, iRay) * iVertexSize) );
//            vDir0 = ( *pPosA - *pPosB );
//            vDir1 = ( *pPosC - *pPosD );
//            pNormal = (Vector3*)( pVertexNormal + (_GetIndex(m_iCurveSamples, iRay) * iVertexSize) );
//            pFirstNormal = (Vector3*)( pVertexNormal + (_GetIndex(0, iRay) * iVertexSize) );
//            *pFirstNormal = (vDir0 ^ vDir1);
//            pFirstNormal->Normalize();
//            *pNormal = *pFirstNormal;
//        }
//        pNormal = (Vector3*)( pVertexNormal + (_GetIndex(0, m_iRadialSamples) * iVertexSize) );
//        pFirstNormal = (Vector3*)( pVertexNormal + (_GetIndex(0, 0) * iVertexSize) );
//        *pNormal = *pFirstNormal;
//        pNormal = (Vector3*)( pVertexNormal + (_GetIndex(m_iCurveSamples, m_iRadialSamples) * iVertexSize) );
//        pFirstNormal = (Vector3*)( pVertexNormal + (_GetIndex(m_iCurveSamples, 0) * iVertexSize) );
//        *pNormal = *pFirstNormal;
//    } else {
//        for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
//            iRayM1 = (iRay > 0) ? (iRay - 1) : (m_iRadialSamples - 1);
//            iRayP1 = iRay + 1;
//            pPosA = (const Vertex3 *)( pVertexPosition + (_GetIndex(0, iRayM1) * iVertexSize) );
//            pPosB = (const Vertex3 *)( pVertexPosition + (_GetIndex(0, iRayP1) * iVertexSize) );
//            pPosC = (const Vertex3 *)( pVertexPosition + (_GetIndex(0, iRay) * iVertexSize) );
//            pPosD = (const Vertex3 *)( pVertexPosition + (_GetIndex(1, iRay) * iVertexSize) );
//            vDir0 = ( *pPosA - *pPosB );
//            vDir1 = ( *pPosC - *pPosD );
//            pNormal = (Vector3*)( pVertexNormal + (_GetIndex(0, iRay) * iVertexSize) );
//            *pNormal = (vDir0 ^ vDir1);
//            pNormal->Normalize();
//        }
//        pNormal = (Vector3*)( pVertexNormal + (_GetIndex(0, m_iRadialSamples) * iVertexSize) );
//        pFirstNormal = (Vector3*)( pVertexNormal + (_GetIndex(0, 0) * iVertexSize) );
//        *pNormal = *pFirstNormal;
//
//        for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
//            iRayM1 = (iRay > 0) ? (iRay - 1) : (m_iRadialSamples - 1);
//            iRayP1 = iRay + 1;
//            pPosA = (const Vertex3 *)( pVertexPosition + (_GetIndex(m_iCurveSamples - 1, iRayM1) * iVertexSize) );
//            pPosB = (const Vertex3 *)( pVertexPosition + (_GetIndex(m_iCurveSamples - 1, iRayP1) * iVertexSize) );
//            pPosC = (const Vertex3 *)( pVertexPosition + (_GetIndex(m_iCurveSamples - 2, iRay) * iVertexSize) );
//            pPosD = (const Vertex3 *)( pVertexPosition + (_GetIndex(m_iCurveSamples - 1, iRay) * iVertexSize) );
//            vDir0 = ( *pPosA - *pPosB );
//            vDir1 = ( *pPosC - *pPosD );
//            pNormal = (Vector3*)( pVertexNormal + (_GetIndex(0, iRay) * iVertexSize) );
//            *pNormal = (vDir0 ^ vDir1);
//            pNormal->Normalize();
//        }
//        pNormal = (Vector3*)( pVertexNormal + (_GetIndex(m_iCurveSamples - 1, m_iRadialSamples) * iVertexSize) );
//        pFirstNormal = (Vector3*)( pVertexNormal + (_GetIndex(m_iCurveSamples - 1, 0) * iVertexSize) );
//        *pNormal = *pFirstNormal;
//    }
//}
//Void TubeSurfaceMesh::_Build_TexCoords( UInt iUnit, const TextureCoord2 & tcMin, const TextureCoord2 & tcMax )
//{
//    UInt iVertexSize = GetVertexSize();
//    Byte * pVertexTexCoord = GetVertexField( 0, VFUSAGE_TEXCOORD, iUnit );
//    TextureCoord2 * pTexCoord;
//
//    TextureCoord2 tcRange = (tcMax - tcMin);
//    Scalar fCurveRatio, fCurveValue;
//    Scalar fRayRatio, fRayValue;
//
//    UInt iMaxCurves = (m_bClosed) ? m_iCurveSamples : m_iCurveSamples - 1;
//    for( UInt iCurve = 0; iCurve <= iMaxCurves; ++iCurve ) {
//        fCurveRatio = ((Scalar)iCurve) / ((Scalar)iMaxCurves);
//        fCurveValue = tcMin.U + ( tcRange.U * fCurveRatio );
//        for( UInt iRay = 0; iRay <= m_iRadialSamples; ++iRay ) {
//            fRayRatio = ((Scalar)iRay) / ((Scalar)m_iRadialSamples);
//            fRayValue = tcMin.T + ( tcRange.T * fRayRatio );
//            pTexCoord = (TextureCoord2*)pVertexTexCoord;
//            pTexCoord->T = fRayValue;
//            pTexCoord->U = fCurveValue;
//            pVertexTexCoord += iVertexSize;
//        }
//    }
//}
//Void TubeSurfaceMesh::_Build_Indices( Bool bInterior )
//{
//    UInt iTriangleCount;
//    if ( m_bClosed )
//        iTriangleCount = ( m_iCurveSamples * m_iRadialSamples ) * 2;
//    else
//        iTriangleCount = ( (m_iCurveSamples - 1) * m_iRadialSamples ) * 2;
//    UInt iIndexCount = (iTriangleCount << 1) + iTriangleCount; // *3
//
//    ModelingFn->SelectMemory( TEXT("Geometry") );
//    m_pIB = KNew() IndexBuffer (
//        (m_bDynamic) ? IB_USAGE_DYNAMIC : IB_USAGE_STATIC,
//        sizeof(UInt), iIndexCount
//    );
//    ModelingFn->UnSelectMemory();
//
//    UInt * pIndex = (UInt*)( m_pIB->GetData() );
//    UInt iCurve, iCurveStart = 0;
//    UInt iRay;
//    UInt iA, iB, iC, iD;
//    for( iCurve = 0; iCurve < m_iCurveSamples - 1; ++iCurve ) {
//        iA = iCurveStart;
//        iB = iCurveStart + 1;
//        iCurveStart += (m_iRadialSamples + 1);
//        iC = iCurveStart;
//        iD = iCurveStart + 1;
//        for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
//            if ( bInterior ) {
//                pIndex[0] = iA++;
//                pIndex[1] = iC;
//                pIndex[2] = iB;
//                pIndex[3] = iB++;
//                pIndex[4] = iC++;
//                pIndex[5] = iD++;
//            } else {
//                pIndex[0] = iA++;
//                pIndex[1] = iB;
//                pIndex[2] = iC;
//                pIndex[3] = iB++;
//                pIndex[4] = iD++;
//                pIndex[5] = iC++;
//            }
//            pIndex += 6;
//        }
//    }
//    if ( m_bClosed ) {
//        iA = iCurveStart;
//        iB = iCurveStart + 1;
//        iC = 0;
//        iD = 1;
//        for( iRay = 0; iRay < m_iRadialSamples; ++iRay ) {
//            if ( bInterior ) {
//                pIndex[0] = iA++;
//                pIndex[1] = iC;
//                pIndex[2] = iB;
//                pIndex[3] = iB++;
//                pIndex[4] = iC++;
//                pIndex[5] = iD++;
//            } else {
//                pIndex[0] = iA++;
//                pIndex[1] = iB;
//                pIndex[2] = iC;
//                pIndex[3] = iB++;
//                pIndex[4] = iD++;
//                pIndex[5] = iC++;
//            }
//            pIndex += 6;
//        }
//    }
//}
