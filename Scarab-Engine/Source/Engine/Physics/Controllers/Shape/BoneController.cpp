/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Shape/BoneController.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shape Controller : Vertex skinning using bones
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
#include "BoneController.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// BoneController implementation
//BoneController::BoneController( const GChar * strName, UInt iBoneCount ):
//    ShapeController( strName )
//{
//    Assert( iBoneCount > 0 );
//
//    // Bones data
//    m_iBoneCount = iBoneCount;
//
//    PhysicsFn->SelectMemory( TEXT("Controllers") );
//    m_arrBones = New RigidBody*[m_iBoneCount];
//    PhysicsFn->UnSelectMemory();
//
//    for( UInt i = 0; i < m_iBoneCount; ++i )
//        m_arrBones[i] = NULL;
//
//    m_arrBoneOffsets = NULL;
//    m_arrBoneWeights = NULL;
//
//    // Target geometry
//    m_pTargetMesh = NULL;
//
//    m_iVertexCount = 0;
//}
//BoneController::~BoneController()
//{
//    PhysicsFn->SelectMemory( TEXT("Controllers") );
//
//    if ( m_arrBoneWeights != NULL )
//        DeleteA( m_arrBoneWeights );
//    if ( m_arrBoneOffsets != NULL )
//        DeleteA( m_arrBoneOffsets );
//    DeleteA( m_arrBones );
//
//    PhysicsFn->UnSelectMemory();
//}
//
//Void BoneController::SetTarget( RigidBody * pTarget )
//{
//    KinematicController::SetTarget( pTarget );
//
//    m_pTargetMesh = m_pTarget->GetLocalShape()->GetMesh();
//    Assert( m_pTargetMesh->IsDynamicVB() );
//
//    PhysicsFn->SelectMemory( TEXT("Controllers") );
//
//    if ( m_arrBoneWeights != NULL )
//        DeleteA( m_arrBoneWeights );
//    if ( m_arrBoneOffsets != NULL )
//        DeleteA( m_arrBoneOffsets );
//
//    m_iVertexCount = m_pTargetMesh->GetVertexCount();
//    m_arrBoneOffsets = New Vector3[m_iVertexCount * m_iBoneCount];
//    m_arrBoneWeights = New Scalar[m_iVertexCount * m_iBoneCount];
//
//    PhysicsFn->UnSelectMemory();
//}
//
///////////////////////////////////////////////////////////////////////////////////
//
//Bool BoneController::_UpdateControllerState( Scalar fTimeStep )
//{
//    // Skin mesh geometry
//    VertexBufferAccessor vba = m_pTargetMesh->MakeVBA();
//    UInt iVertexSize = vba.GetVertexSize();
//
//    Byte * arrPositions = vba.GetPosition( vba.GetVertex(0) );
//
//    Vertex3 * pPosition;
//
//    Scalar * arrWeights = m_arrBoneWeights;
//    Vector3 * arrOffsets = m_arrBoneOffsets;
//    Scalar fWeight;
//    Vector3 vOffset;
//    UInt i, j;
//
//    for( i = 0; i < m_iVertexCount; ++i ) {
//        pPosition = (Vertex3*)( arrPositions );
//
//        *pPosition = Vertex3::Null;
//        for( j = 0; j < m_iBoneCount; ++j ) {
//            fWeight = arrWeights[j];
//            if ( fWeight != 0.0f ) {
//                vOffset = ( m_arrBones[j]->GetTransform() * arrOffsets[j] );
//                *pPosition += ( vOffset * fWeight );
//            }
//        }
//
//        arrPositions += iVertexSize;
//        arrWeights += m_iBoneCount;
//        arrOffsets += m_iBoneCount;
//    }
//
//    // Trigger dynamic update
//    //m_pTarget->GetLocalShape()->OnDynamicUpdate();
//    //m_pTargetMesh->ForceDynamicUpdate();
//
//    return true;
//}

