/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Shape/MorphController.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shape Controller : Vertex morphing
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
#include "MorphController.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MorphController implementation
//MorphController::MorphController( const GChar * strName, UInt iMorphKeyCount, UInt iTargetCount ):
//    ShapeController( strName )
//{
//    Assert( iMorphKeyCount > 0 && iTargetCount > 0 );
//
//    PhysicsFn->SelectMemory( TEXT("Controllers") );
//
//    // Morphing time-line
//    m_iMorphKeyCount = iMorphKeyCount;
//    m_iMorphLastKey = 0;
//    m_arrMorphTimes = New Scalar[m_iMorphKeyCount];
//    m_arrMorphKeys = New Scalar[m_iMorphKeyCount * iTargetCount];
//
//    // Target geometry
//    m_pTargetMesh = NULL;
//
//    m_iVertexCount = 0;
//    m_arrBaseVertices = NULL;
//
//    m_iTargetCount = iTargetCount;
//    m_arrTargetVectors = NULL;
//
//    PhysicsFn->UnSelectMemory();
//}
//MorphController::~MorphController()
//{
//    PhysicsFn->SelectMemory( TEXT("Controllers") );
//
//    if ( m_arrTargetVectors != NULL )
//        DeleteA( m_arrTargetVectors );
//    if ( m_arrBaseVertices != NULL )
//        DeleteA( m_arrBaseVertices );
//    DeleteA( m_arrMorphKeys );
//    DeleteA( m_arrMorphTimes );
//
//    PhysicsFn->UnSelectMemory();
//}
//
//Void MorphController::SetTarget( RigidBody * pTarget )
//{
//    KinematicController::SetTarget( pTarget );
//
//    m_pTargetMesh = m_pTarget->GetLocalShape()->GetMesh();
//    Assert( m_pTargetMesh->IsDynamicVB() );
//
//    PhysicsFn->SelectMemory( TEXT("Controllers") );
//
//    if ( m_arrTargetVectors != NULL )
//        DeleteA( m_arrTargetVectors );
//    if ( m_arrBaseVertices != NULL )
//        DeleteA( m_arrBaseVertices );
//
//    m_iVertexCount = m_pTargetMesh->GetVertexCount();
//    m_arrBaseVertices = New Vertex3[m_iVertexCount];
//    m_arrTargetVectors = New Vector3[m_iVertexCount * m_iTargetCount];
//
//    PhysicsFn->UnSelectMemory();
//}
//
///////////////////////////////////////////////////////////////////////////////////
//
//Bool MorphController::_UpdateControllerState( Scalar fTimeStep )
//{
//    Scalar fControlTime = _ComputeControlTime();
//
//    // Lookup keyframes
//    Scalar fNormalTime;
//    UInt iPrevKey, iNextHey;
//    _LookupKeyFrames( fControlTime, &fNormalTime, &iPrevKey, &iNextHey );
//
//    // Morph mesh geometry
//    VertexBufferAccessor vba = m_pTargetMesh->MakeVBA();
//    UInt iVertexSize = vba.GetVertexSize();
//
//    Byte * arrPositions = vba.GetPosition( vba.GetVertex(0) );
//    Byte * pVertex;
//
//    Vertex3 * pPosition;
//
//    Scalar * arrPrevWeights = ( m_arrMorphKeys + (iPrevKey * m_iTargetCount) );
//    Scalar * arrNextWeights = ( m_arrMorphKeys + (iNextHey * m_iTargetCount) );
//    Scalar fWeight;
//    Vector3 * pTargetVector;
//    UInt i, j;
//
//    pVertex = arrPositions;
//    for( i = 0; i < m_iVertexCount; ++i ) {
//        pPosition = (Vertex3*)pVertex;
//        *pPosition = m_arrBaseVertices[i];
//        pVertex += iVertexSize;
//    }
//
//    for( i = 0; i < m_iTargetCount; ++i ) {
//        fWeight = ( (1.0f - fNormalTime) * arrPrevWeights[i] + fNormalTime * arrNextWeights[i] );
//        pTargetVector = ( m_arrTargetVectors + (i * m_iVertexCount) );
//
//        pVertex = arrPositions;
//        for( j = 0; j < m_iVertexCount; ++j ) {
//            pPosition = (Vertex3*)pVertex;
//            *pPosition += ( pTargetVector[j] * fWeight );
//            pVertex += iVertexSize;
//        }
//    }
//
//    // Trigger dynamic update
//    //m_pTarget->GetLocalShape()->OnDynamicUpdate();
//    //m_pTargetMesh->ForceDynamicUpdate();
//
//    return true;
//}
//
//Void MorphController::_LookupKeyFrames( Scalar fControlTime, Scalar * outNormalTime, UInt * outPrevKey, UInt * outNextKey )
//{
//    if ( fControlTime <= m_arrMorphTimes[0] ) {
//        m_iMorphLastKey = 0;
//        *outPrevKey = 0;
//        *outNextKey = 0;
//        *outNormalTime = 0.0f;
//        return;
//    }
//    if ( fControlTime >= m_arrMorphTimes[m_iMorphKeyCount - 1] ) {
//        m_iMorphLastKey = m_iMorphKeyCount - 1;
//        *outPrevKey = m_iMorphLastKey;
//        *outNextKey = m_iMorphLastKey;
//        *outNormalTime = 0.0f;
//        return;
//    }
//
//    UInt iNextKey;
//    if ( fControlTime > m_arrMorphTimes[m_iMorphLastKey] ) {
//        iNextKey = (m_iMorphLastKey + 1);
//        while( fControlTime >= m_arrMorphTimes[iNextKey] )
//            ++iNextKey;
//        m_iMorphLastKey = iNextKey - 1;
//
//        *outPrevKey = m_iMorphLastKey;
//        *outNextKey = iNextKey;
//        *outNormalTime = ( (fControlTime - m_arrMorphTimes[*outPrevKey]) / (m_arrMorphTimes[*outNextKey] - m_arrMorphTimes[*outPrevKey]) );
//
//        return;
//    }
//    if ( fControlTime < m_arrMorphTimes[m_iMorphLastKey] ) {
//        iNextKey = (m_iMorphLastKey - 1);
//        while( fControlTime <= m_arrMorphTimes[iNextKey] )
//            --iNextKey;
//        m_iMorphLastKey = iNextKey + 1;
//
//        *outPrevKey = iNextKey;
//        *outNextKey = m_iMorphLastKey;
//        *outNormalTime = ( (fControlTime - m_arrMorphTimes[*outPrevKey]) / (m_arrMorphTimes[*outNextKey] - m_arrMorphTimes[*outPrevKey]) );
//
//        return;
//    }
//
//    *outPrevKey = m_iMorphLastKey;
//    *outNextKey = m_iMorphLastKey;
//    *outNormalTime = 0.0f;
//}
//
