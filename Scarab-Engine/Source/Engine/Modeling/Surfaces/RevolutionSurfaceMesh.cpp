/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Curves/RevolutionSurfaceMesh.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curved Surface Mesh, Revolution surfaces
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
#include "RevolutionSurfaceMesh.h"

#include "../ModelingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// RevolutionSurfaceMesh implementation
//RevolutionSurfaceMesh::RevolutionSurfaceMesh( RevolutionSurfaceTopology iTopology, Curve2 * pCurve,
//                                              UInt iRadialSamples, UInt iCurveSamples, Scalar fCenterX,
//                                              Bool bUseArcLengthSampling, Bool bInterior,
//                                              const VertexFormat * pVF, Bool bDynamic ):
//    WorldTriangleMesh()
//{
//    Assert( pCurve != NULL );
//    Assert( pVF != NULL );
//
//    m_pVF = pVF;
//
//    m_bDynamic = bDynamic;
//
//    m_iTopology = iTopology;
//    m_pCurve = pCurve;
//    m_iRadialSamples = iRadialSamples;
//    m_iCurveSamples = iCurveSamples;
//    m_fCenterX = fCenterX;
//    m_bArcLengthSampling = bUseArcLengthSampling;
//
//    // Build Topology
//    MeshLibFn->Reset();
//    MeshLibFn->SetVF( m_pVF );
//    MeshLibFn->SetInteriorMesh( bInterior );
//    MeshLibFn->SetDynamic( m_bDynamic );
//    switch( iTopology ) {
//        case REVOLUTION_DISK:     MeshLibFn->Disk( &m_pVB, &m_pIB, m_iRadialSamples, m_iCurveSamples, 1.0f ); break;
//        case REVOLUTION_CYLINDER: MeshLibFn->Cylinder( &m_pVB, &m_pIB, m_iRadialSamples, m_iCurveSamples, 1.0f, 1.0f, true ); break;
//        case REVOLUTION_SPHERE:   MeshLibFn->Sphere( &m_pVB, &m_pIB, m_iRadialSamples, m_iCurveSamples, 1.0f ); break;
//        case REVOLUTION_TORUS:    MeshLibFn->Torus( &m_pVB, &m_pIB, m_iRadialSamples, m_iCurveSamples, 1.0f, 0.25f ); break;
//        default: Assert(false); break;
//    }
//
//    ModelingFn->SelectMemory( TEXT("Curves") );
//
//    // Compute slice coefficients on a unit circle
//    m_arrSin = KNew() Scalar[m_iRadialSamples + 1];
//    m_arrCos = KNew() Scalar[m_iRadialSamples + 1];
//    Scalar fRadialFactor = SCALAR_2PI * MathFn->Invert( (Scalar)m_iRadialSamples );
//    Scalar fAngle = 0.0f;
//    for( UInt i = 0; i < m_iRadialSamples; ++i ) {
//        MathFn->SinCos( fAngle, m_arrSin + i, m_arrCos + i );
//        fAngle += fRadialFactor;
//    }
//    m_arrSin[m_iRadialSamples] = m_arrSin[0];
//    m_arrCos[m_iRadialSamples] = m_arrCos[0];
//
//    // Storage for curve samples
//    m_arrSamples = KNew() Vertex3[m_iCurveSamples];
//
//    // Vertices update
//    if ( m_bDynamic )
//        Update();
//    else {
//        m_bDynamic = true;
//        Update();
//        m_bDynamic = false;
//        KDeleteA( m_arrSin );
//        m_arrSin = NULL;
//        KDeleteA( m_arrCos );
//        m_arrCos = NULL;
//        KDeleteA( m_arrSamples );
//        m_arrSamples = NULL;
//    }
//
//    ModelingFn->UnSelectMemory();
//}
//RevolutionSurfaceMesh::~RevolutionSurfaceMesh()
//{
//    ModelingFn->SelectMemory( TEXT("Curves") );
//    if ( m_arrSin != NULL )
//        KDeleteA( m_arrSin );
//    if ( m_arrCos != NULL )
//        KDeleteA( m_arrCos );
//    if ( m_arrSamples != NULL )
//        KDeleteA( m_arrSamples );
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
//Void RevolutionSurfaceMesh::Update()
//{
//    Assert( m_bDynamic );
//
//    Scalar fMinT = m_pCurve->GetMinT();
//    Scalar fRangeT = m_pCurve->GetMaxT() - fMinT;
//    Scalar fCurveLength = 0.0f;
//    if ( m_bArcLengthSampling )
//        fCurveLength = m_pCurve->Length();
//
//    // Curve sampling
//    Scalar fCurveFactor = MathFn->Invert( (Scalar)(m_iCurveSamples - 1) );
//    Scalar fT;
//    Vertex2 vPosition;
//    for( UInt i = 0; i < m_iCurveSamples; ++i ) {
//        if ( m_bArcLengthSampling )
//            fT = m_pCurve->ParamT( i * fCurveLength * fCurveFactor );
//        else
//            fT = fMinT + ( i * fRangeT * fCurveFactor );
//        vPosition = m_pCurve->P( fT );
//        m_arrSamples[i].X = vPosition.X;
//        m_arrSamples[i].Y = 0.0f;
//        m_arrSamples[i].Z = vPosition.Y;
//    }
//
//    // Topology-dependant update
//    switch( m_iTopology ) {
//        case REVOLUTION_DISK:     _Update_Disk(); break;
//        case REVOLUTION_CYLINDER: _Update_Cylinder(); break;
//        case REVOLUTION_SPHERE:   _Update_Sphere(); break;
//        case REVOLUTION_TORUS:    _Update_Torus(); break;
//        default: Assert(false); break;
//    }
//
//    UpdateModel( MODEL_UPDATE_NORMALS );
//}

/////////////////////////////////////////////////////////////////////////////////

//Void RevolutionSurfaceMesh::_Update_Disk()
//{
//    UInt iVertexSize = GetVertexSize();
//    Byte * pVertexPosition = GetVertexField( 0, VFUSAGE_POSITION );
//    Byte * pVertex;
//    Vertex3 * pPosition;
//    UInt iVertex;
//
//    // First ray    
//    UInt iCurve;
//    pVertex = pVertexPosition;
//    for( iCurve = 0; iCurve < m_iCurveSamples; ++iCurve ) {
//        pPosition = (Vertex3*)pVertex;
//        *pPosition = m_arrSamples[iCurve];
//        pVertex += iVertexSize;
//    }
//
//    // Revolution
//    UInt iCurveSamplesM1 = m_iCurveSamples - 1;
//    Scalar fRadius;
//    for( UInt iRay = 1; iRay < m_iRadialSamples; ++iRay ) {
//        for( iCurve = 1; iCurve < m_iCurveSamples; ++iCurve ) {
//            fRadius = ( m_arrSamples[iCurve].X - m_fCenterX );
//            if ( fRadius < 0.0f )
//                fRadius = 0.0f;
//            iVertex = ( iRay * iCurveSamplesM1 ) + iCurve;
//            pPosition = (Vertex3*)( pVertexPosition + (iVertex * iVertexSize) );
//            pPosition->X = ( m_fCenterX + (m_arrCos[iRay] * fRadius) );
//            pPosition->Y = ( m_arrSin[iRay] * fRadius );
//            pPosition->Z = ( m_arrSamples[iCurve].Z );
//        }
//    }
//}
//Void RevolutionSurfaceMesh::_Update_Cylinder()
//{
//    UInt iVertexSize = GetVertexSize();
//    Byte * pVertexPosition = GetVertexField( 0, VFUSAGE_POSITION );
//    Vertex3 * pPosition;
//    UInt iVertex;
//
//    // First / Last rays
//    UInt iRadialSamplesP1 = m_iRadialSamples + 1;
//    UInt iCurve;
//    for( iCurve = 0; iCurve < m_iCurveSamples; ++iCurve ) {
//        iVertex = ( iCurve * iRadialSamplesP1 );
//        pPosition = (Vertex3*)( pVertexPosition + (iVertex * iVertexSize) );
//        *pPosition = m_arrSamples[iCurve];
//        iVertex += m_iRadialSamples;
//        pPosition = (Vertex3*)( pVertexPosition + (iVertex * iVertexSize) );
//        *pPosition = m_arrSamples[iCurve];
//    }
//
//    // Revolution
//    Scalar fRadius;
//    for( UInt iRay = 1; iRay < m_iRadialSamples; ++iRay ) {
//        for( iCurve = 0; iCurve < m_iCurveSamples; ++iCurve ) {
//            fRadius = ( m_arrSamples[iCurve].X - m_fCenterX );
//            if ( fRadius < 0.0f )
//                fRadius = 0.0f;
//            iVertex = ( iCurve * iRadialSamplesP1 ) + iRay;
//            pPosition = (Vertex3*)( pVertexPosition + (iVertex * iVertexSize) );
//            pPosition->X = ( m_fCenterX + (m_arrCos[iRay] * fRadius) );
//            pPosition->Y = ( m_arrSin[iRay] * fRadius );
//            pPosition->Z = ( m_arrSamples[iCurve].Z );
//        }
//    }
//}
//Void RevolutionSurfaceMesh::_Update_Sphere()
//{
//    UInt iVertexSize = GetVertexSize();
//    Byte * pVertexPosition = GetVertexField( 0, VFUSAGE_POSITION );
//    Vertex3 * pPosition;
//    UInt iVertex;
//
//    // South/North poles
//    UInt iVertexCount = GetVertexCount();
//    pPosition = (Vertex3*)( pVertexPosition + ((iVertexCount - 2) * iVertexSize) );
//    *pPosition = m_arrSamples[0];
//    pPosition = (Vertex3*)( pVertexPosition + ((iVertexCount - 1) * iVertexSize) );
//    *pPosition = m_arrSamples[m_iCurveSamples - 1];
//
//    // First / Last rays
//    UInt iRadialSamplesP1 = m_iRadialSamples + 1;
//    UInt iCurveSamplesM2 = m_iCurveSamples - 2;
//    UInt iCurve;
//    for( iCurve = 0; iCurve < iCurveSamplesM2; ++iCurve ) {
//        iVertex = ( iCurve * iRadialSamplesP1 );
//        pPosition = (Vertex3*)( pVertexPosition + (iVertex * iVertexSize) );
//        *pPosition = m_arrSamples[iCurve + 1];
//        iVertex += m_iRadialSamples;
//        pPosition = (Vertex3*)( pVertexPosition + (iVertex * iVertexSize) );
//        *pPosition = m_arrSamples[iCurve + 1];
//    }
//
//    // Revolution
//    Scalar fRadius;
//    for( UInt iRay = 1; iRay < m_iRadialSamples; ++iRay ) {
//        for( iCurve = 0; iCurve < iCurveSamplesM2; ++iCurve ) {
//            fRadius = ( m_arrSamples[iCurve + 1].X - m_fCenterX );
//            if ( fRadius < 0.0f )
//                fRadius = 0.0f;
//            iVertex = ( iCurve * iRadialSamplesP1 ) + iRay;
//            pPosition = (Vertex3*)( pVertexPosition + (iVertex * iVertexSize) );
//            pPosition->X = ( m_fCenterX + (m_arrCos[iRay] * fRadius) );
//            pPosition->Y = ( m_arrSin[iRay] * fRadius );
//            pPosition->Z = ( m_arrSamples[iCurve + 1].Z );
//        }
//    }
//}
//Void RevolutionSurfaceMesh::_Update_Torus()
//{
//    UInt iVertexSize = GetVertexSize();
//    Byte * pVertexPosition = GetVertexField( 0, VFUSAGE_POSITION );
//    Vertex3 *pPosition, *pFirstPosition;
//    UInt iVertex;
//
//    // First / Last rays
//    UInt iRadialSamplesP1 = m_iRadialSamples + 1;
//    UInt iCurve;
//    for( iCurve = 0; iCurve < m_iCurveSamples; ++iCurve ) {
//        iVertex = ( iCurve * iRadialSamplesP1 );
//        pPosition = (Vertex3*)( pVertexPosition + (iVertex * iVertexSize) );
//        *pPosition = m_arrSamples[iCurve];
//        iVertex += m_iRadialSamples;
//        pPosition = (Vertex3*)( pVertexPosition + (iVertex * iVertexSize) );
//        *pPosition = m_arrSamples[iCurve];
//    }
//
//    // Revolution
//    Scalar fRadius;
//    UInt iRay;
//    for( iRay = 1; iRay < m_iRadialSamples; ++iRay ) {
//        for( iCurve = 0; iCurve < m_iCurveSamples; ++iCurve ) {
//            fRadius = ( m_arrSamples[iCurve].X - m_fCenterX );
//            if ( fRadius < 0.0f )
//                fRadius = 0.0f;
//            iVertex = ( iCurve * iRadialSamplesP1 ) + iRay;
//            pPosition = (Vertex3*)( pVertexPosition + (iVertex * iVertexSize) );
//            pPosition->X = ( m_fCenterX + (m_arrCos[iRay] * fRadius) );
//            pPosition->Y = ( m_arrSin[iRay] * fRadius );
//            pPosition->Z = ( m_arrSamples[iCurve].Z );
//        }
//    }
//
//    // Duplicated last circle
//    iVertex = GetVertexCount() - iRadialSamplesP1;
//    for( iRay = 0; iRay < iRadialSamplesP1; ++iRay ) {
//        pPosition = (Vertex3*)( pVertexPosition + (iVertex * iVertexSize) );
//        pFirstPosition = (Vertex3*)( pVertexPosition + (iRay * iVertexSize) );
//        *pPosition = *pFirstPosition;
//        ++iVertex;
//    }
//}
