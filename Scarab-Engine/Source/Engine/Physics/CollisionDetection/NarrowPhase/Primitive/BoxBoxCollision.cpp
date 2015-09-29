/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Primitive/BoxBoxCollision.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Primitive, Box-Box
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
#include "BoxBoxCollision.h"

/////////////////////////////////////////////////////////////////////////////////
// BoxBoxCollision implementation
BoxBoxCollision::BoxBoxCollision( TransformedShapeConvex * pBoxA, TransformedShapeConvex * pBoxB ):
    VoronoiClip( pBoxA, pBoxB )
{
    // nothing to do
}
BoxBoxCollision::~BoxBoxCollision()
{
    // nothing to do
}

//BoxBoxCollision::BoxBoxCollision( TransformedShapeConvex * pBoxA, TransformedShapeConvex * pBoxB ):
//    NarrowPhase( pBoxA, pBoxB )
//{
//    Assert( pBoxA->GetType() == SHAPE_PRIMITIVE_BOX );
//    Assert( pBoxB->GetType() == SHAPE_PRIMITIVE_BOX );
//}
//BoxBoxCollision::~BoxBoxCollision()
//{
//    // nothing to do
//}
//
//Void BoxBoxCollision::Initialize()
//{
//    // nothing to do
//}
//Bool BoxBoxCollision::DetectCollision( Bool bGenerateContactPoints )
//{
//    /////////////////////////////////////////////////
//    Assert( false );
//    return false;
//}

//#define _SAT_FACETS( _dist, _radius, _normal, _axis ) \
//    fDistance = MathFn->Abs(_dist) - (_radius); \
//    if ( fDistance > 0 && !bGenerateContactPoints ) { \
//        m_iContactPointCount = 0; \
//        return false; \
//    } \
//    if ( fDistance > fMinPenetration ) { \
//        fMinPenetration = fDistance; \
//        iMinPenetrationAxis = (_axis); \
//        vMinPenetrationNormal = (_normal); \
//        bInvertNormal = ( (_dist) < 0 ); \
//    }
//
//#define _SAT_EDGES( _dist, _radius, _normal, _axis ) \
//    fDistance = MathFn->Abs(_dist) - (_radius); \
//    if ( fDistance > SCALAR_EPSILON && !bGenerateContactPoints ) { \
//        m_iContactPointCount = 0; \
//        return false; \
//    } \
//    fTestLength = (_normal).Normalize(); \
//    if ( fTestLength > SCALAR_EPSILON ) { \
//        fDistance /= fTestLength; \
//        if ( (fDistance * 1.05f) > fMinPenetration ) { \
//            fMinPenetration = fDistance; \
//            iMinPenetrationAxis = (_axis); \
//            vMinPenetrationNormal = (_normal); \
//            bInvertNormal = ( (_dist) < 0 ); \
//        } \
//    }
//
//Bool BoxBoxCollision::DetectCollision( Bool bGenerateContactPoints )
//{
//    TransformedBox * pBoxA = (TransformedBox*)m_pShapeA;
//    TransformedBox * pBoxB = (TransformedBox*)m_pShapeB;
//
//    // Get extents
//    Vector3 vExtentsA = pBoxA->GetExtents();
//    Vector3 vExtentsB = pBoxB->GetExtents();
//
//    // Get rotation matrices
//    Matrix3 matRotationA = pBoxA->GetTransform().GetRotate();
//    Matrix3 matRotationB = pBoxB->GetTransform().GetRotate();
//
//    // Invert A's rotation
//    Matrix3 matInvRotationA;
//    matRotationA.Transpose( matInvRotationA );
//
//    // Compute relative translation in A's frame
//    Vector3 vTranslation = ( pBoxB->GetCenter() - pBoxA->GetCenter() );
//    Vector3 vRelativeTranslation = ( matInvRotationA * vTranslation );
//
//    // Compute relative rotation in A's frame
//    Matrix3 matRelativeRotation;
//    matRelativeRotation = ( matInvRotationA * matRotationB );
//
//    // Common sub-expressions
//    Matrix3 matAbsRelativeRotation;
//    matAbsRelativeRotation.m00 = MathFn->Abs( matRelativeRotation.m00 );
//    matAbsRelativeRotation.m01 = MathFn->Abs( matRelativeRotation.m01 );
//    matAbsRelativeRotation.m02 = MathFn->Abs( matRelativeRotation.m02 );
//    matAbsRelativeRotation.m10 = MathFn->Abs( matRelativeRotation.m10 );
//    matAbsRelativeRotation.m11 = MathFn->Abs( matRelativeRotation.m11 );
//    matAbsRelativeRotation.m12 = MathFn->Abs( matRelativeRotation.m12 );
//    matAbsRelativeRotation.m20 = MathFn->Abs( matRelativeRotation.m20 );
//    matAbsRelativeRotation.m21 = MathFn->Abs( matRelativeRotation.m21 );
//    matAbsRelativeRotation.m22 = MathFn->Abs( matRelativeRotation.m22 );
//
//    // Perform SAT tests, track minimal penetration
//    Vector3 vTestAxis, vTestNormal;
//    Scalar fTestDistance, fTestRadius;
//    Scalar fTestLength;
//
//    Scalar fDistance, fMinPenetration = -SCALAR_INFINITE;
//    UInt iMinPenetrationAxis = 0;
//    Vector3 vMinPenetrationNormal;
//    Bool bInvertNormal = false;
//
//    // SAT tests for facet axes
//        // Axis from A
//    matAbsRelativeRotation.GetRow( vTestAxis, 0 );
//    matRotationA.GetColumn( vTestNormal, 0 );
//    fTestDistance = vRelativeTranslation[0];
//    fTestRadius = ( vExtentsA[0] + (vExtentsB * vTestAxis) );
//    _SAT_FACETS( fTestDistance, fTestRadius, vTestNormal, 1 );
//
//    matAbsRelativeRotation.GetRow( vTestAxis, 1 );
//    matRotationA.GetColumn( vTestNormal, 1 );
//    fTestDistance = vRelativeTranslation[1];
//    fTestRadius = ( vExtentsA[1] + (vExtentsB * vTestAxis) );
//    _SAT_FACETS( fTestDistance, fTestRadius, vTestNormal, 2 );
//
//    matAbsRelativeRotation.GetRow( vTestAxis, 2 );
//    matRotationA.GetColumn( vTestNormal, 2 );
//    fTestDistance = vRelativeTranslation[2];
//    fTestRadius = ( vExtentsA[2] + (vExtentsB * vTestAxis) );
//    _SAT_FACETS( fTestDistance, fTestRadius, vTestNormal, 3 );
//
//        // Axis from B
//    matAbsRelativeRotation.GetColumn( vTestAxis, 0 );
//    matRotationB.GetColumn( vTestNormal, 0 );
//    fTestDistance = ( vTranslation * vTestNormal );
//    fTestRadius = ( vExtentsB[0] + (vExtentsA * vTestAxis) );
//    _SAT_FACETS( fTestDistance, fTestRadius, vTestNormal, 4 );
//
//    matAbsRelativeRotation.GetColumn( vTestAxis, 1 );
//    matRotationB.GetColumn( vTestNormal, 1 );
//    fTestDistance = ( vTranslation * vTestNormal );
//    fTestRadius = ( vExtentsB[1] + (vExtentsA * vTestAxis) );
//    _SAT_FACETS( fTestDistance, fTestRadius, vTestNormal, 5 );
//
//    matAbsRelativeRotation.GetColumn( vTestAxis, 2 );
//    matRotationB.GetColumn( vTestNormal, 2 );
//    fTestDistance = ( vTranslation * vTestNormal );
//    fTestRadius = ( vExtentsB[2] + (vExtentsA * vTestAxis) );
//    _SAT_FACETS( fTestDistance, fTestRadius, vTestNormal, 6 );
//
//    // SAT tests for edge axes
//        // Axis 0 from A
//    vTestNormal = Vector3( 0.0f, -matRelativeRotation.m20, matRelativeRotation.m10 );
//    fTestDistance = ( (vRelativeTranslation[2] * matRelativeRotation.m10) - (vRelativeTranslation[1] * matRelativeRotation.m20) );
//    fTestRadius = ( (vExtentsA[1] * matAbsRelativeRotation.m20) + (vExtentsA[2] * matAbsRelativeRotation.m10) );
//    fTestRadius += ( (vExtentsB[1] * matAbsRelativeRotation.m02) + (vExtentsB[2] * matAbsRelativeRotation.m01) );
//    _SAT_EDGES( fTestDistance, fTestRadius, vTestNormal, 7 );
//
//    vTestNormal = Vector3( 0.0f, -matRelativeRotation.m21, matRelativeRotation.m11 );
//    fTestDistance = ( (vRelativeTranslation[2] * matRelativeRotation.m11) - (vRelativeTranslation[1] * matRelativeRotation.m21) );
//    fTestRadius = ( (vExtentsA[1] * matAbsRelativeRotation.m21) + (vExtentsA[2] * matAbsRelativeRotation.m11) );
//    fTestRadius += ( (vExtentsB[0] * matAbsRelativeRotation.m02) + (vExtentsB[2] * matAbsRelativeRotation.m00) );
//    _SAT_EDGES( fTestDistance, fTestRadius, vTestNormal, 8 );
//
//    vTestNormal = Vector3( 0.0f, -matRelativeRotation.m22, matRelativeRotation.m12 );
//    fTestDistance = ( (vRelativeTranslation[2] * matRelativeRotation.m12) - (vRelativeTranslation[1] * matRelativeRotation.m22) );
//    fTestRadius = ( (vExtentsA[1] * matAbsRelativeRotation.m22) + (vExtentsA[2] * matAbsRelativeRotation.m12) );
//    fTestRadius += ( (vExtentsB[0] * matAbsRelativeRotation.m01) + (vExtentsB[1] * matAbsRelativeRotation.m00) );
//    _SAT_EDGES( fTestDistance, fTestRadius, vTestNormal, 9 );
//
//        // Axis 1 from A
//    vTestNormal = Vector3( matRelativeRotation.m20, 0.0f, -matRelativeRotation.m00 );
//    fTestDistance = ( (vRelativeTranslation[0] * matRelativeRotation.m20) - (vRelativeTranslation[2] * matRelativeRotation.m00) );
//    fTestRadius = ( (vExtentsA[0] * matAbsRelativeRotation.m20) + (vExtentsA[2] * matAbsRelativeRotation.m00) );
//    fTestRadius += ( (vExtentsB[1] * matAbsRelativeRotation.m12) + (vExtentsB[2] * matAbsRelativeRotation.m11) );
//    _SAT_EDGES( fTestDistance, fTestRadius, vTestNormal, 10 );
//
//    vTestNormal = Vector3( matRelativeRotation.m21, 0.0f, -matRelativeRotation.m01 );
//    fTestDistance = ( (vRelativeTranslation[0] * matRelativeRotation.m21) - (vRelativeTranslation[2] * matRelativeRotation.m01) );
//    fTestRadius = ( (vExtentsA[0] * matAbsRelativeRotation.m21) + (vExtentsA[2] * matAbsRelativeRotation.m01) );
//    fTestRadius += ( (vExtentsB[0] * matAbsRelativeRotation.m12) + (vExtentsB[2] * matAbsRelativeRotation.m10) );
//    _SAT_EDGES( fTestDistance, fTestRadius, vTestNormal, 11 );
//
//    vTestNormal = Vector3( matRelativeRotation.m22, 0.0f, -matRelativeRotation.m02 );
//    fTestDistance = ( (vRelativeTranslation[0] * matRelativeRotation.m22) - (vRelativeTranslation[2] * matRelativeRotation.m02) );
//    fTestRadius = ( (vExtentsA[0] * matAbsRelativeRotation.m22) + (vExtentsA[2] * matAbsRelativeRotation.m02) );
//    fTestRadius += ( (vExtentsB[0] * matAbsRelativeRotation.m11) + (vExtentsB[1] * matAbsRelativeRotation.m10) );
//    _SAT_EDGES( fTestDistance, fTestRadius, vTestNormal, 12 );
//
//        // Axis 2 from A
//    vTestNormal = Vector3( -matRelativeRotation.m10, matRelativeRotation.m00, 0.0f );
//    fTestDistance = ( (vRelativeTranslation[1] * matRelativeRotation.m00) - (vRelativeTranslation[0] * matRelativeRotation.m10) );
//    fTestRadius = ( (vExtentsA[0] * matAbsRelativeRotation.m10) + (vExtentsA[1] * matAbsRelativeRotation.m00) );
//    fTestRadius += ( (vExtentsB[1] * matAbsRelativeRotation.m22) + (vExtentsB[2] * matAbsRelativeRotation.m21) );
//    _SAT_EDGES( fTestDistance, fTestRadius, vTestNormal, 13 );
//
//    vTestNormal = Vector3( -matRelativeRotation.m11, matRelativeRotation.m01, 0.0f );
//    fTestDistance = ( (vRelativeTranslation[1] * matRelativeRotation.m01) - (vRelativeTranslation[0] * matRelativeRotation.m11) );
//    fTestRadius = ( (vExtentsA[0] * matAbsRelativeRotation.m11) + (vExtentsA[1] * matAbsRelativeRotation.m01) );
//    fTestRadius += ( (vExtentsB[0] * matAbsRelativeRotation.m22) + (vExtentsB[2] * matAbsRelativeRotation.m20) );
//    _SAT_EDGES( fTestDistance, fTestRadius, vTestNormal, 14 );
//
//    vTestNormal = Vector3( -matRelativeRotation.m12, matRelativeRotation.m02, 0.0f );
//    fTestDistance = ( (vRelativeTranslation[1] * matRelativeRotation.m02) - (vRelativeTranslation[0] * matRelativeRotation.m12) );
//    fTestRadius = ( (vExtentsA[0] * matAbsRelativeRotation.m12) + (vExtentsA[1] * matAbsRelativeRotation.m02) );
//    fTestRadius += ( (vExtentsB[0] * matAbsRelativeRotation.m21) + (vExtentsB[1] * matAbsRelativeRotation.m20) );
//    _SAT_EDGES( fTestDistance, fTestRadius, vTestNormal, 15 );
//
//    // Safety check
//    if ( iMinPenetrationAxis == 0 ) {
//        m_iContactPointCount = 0;
//        return false;
//    }
//
//    // Contact points not needed
//    if ( !bGenerateContactPoints ) {
//        m_iContactPointCount = 0;
//        return true;
//    }
//
//    // We have penetration, compute world normal
//    Vector3 vNormal;
//    if ( iMinPenetrationAxis < 7 )
//        vNormal = vMinPenetrationNormal;
//    else
//        vNormal = ( matRotationA * vMinPenetrationNormal );
//    if ( bInvertNormal )
//        vNormal = -vNormal;
//
//    // Generate contact points
//    UInt i, j;
//    Scalar fSign;
//
//    Vertex3 vA = pBoxA->GetCenter();
//    Vertex3 vB = pBoxB->GetCenter();
//
//        // Edge-Edge case
//    if ( iMinPenetrationAxis > 6 ) {
//        // Generate contact ID
//        QWord qwContactPointID = 0;
//
//        // Find a point on A's edge
//        for( i = 0; i < 3; ++i ) {
//            matRotationA.GetColumn( vTestAxis, i );
//            if ( (vNormal * vTestAxis) > 0 ) {
//                //pContactPoint->qwContactPointID |= ( 1 << i );
//                fSign = 1.0f;
//            } else
//                fSign = -1.0f;
//            for( j = 0; j < 3; ++j )
//                vA[j] += ( vExtentsA[i] * matRotationA(j,i) ) * fSign;
//        }
//
//        // Find a point on B's edge
//        for( i = 0; i < 3; ++i ) {
//            matRotationB.GetColumn( vTestAxis, i );
//            if ( (vNormal * vTestAxis) > 0 )    
//                fSign = -1.0f;
//            else {
//                //pContactPoint->qwContactPointID |= ( 1 << (16 + i) );
//                fSign = 1.0f;
//            }
//            for( j = 0; j < 3; ++j )
//                vB[j] += ( vExtentsB[i] * matRotationB(j,i) ) * fSign;
//        }
//
//        // Compute closest points on both edges
//        Scalar fParamA, fParamB;
//        Vector3 vAxisA, vAxisB;
//        for( i = 0; i < 3; ++i ) {
//            vAxisA[i] = matRotationA( i, (iMinPenetrationAxis - 7) / 3 );
//            vAxisB[i] = matRotationB( i, (iMinPenetrationAxis - 7) % 3 );
//        }
//
//        _EdgesClosestPoints( &fParamA, &fParamB, vA, vAxisA, vB, vAxisB );
//        for( i = 0; i < 3; ++i ) {
//            vA[i] += ( vAxisA[i] * fParamA );
//            vB[i] += ( vAxisB[i] * fParamB );
//        }
//
//        m_iContactPointCount = 0;
//        _AddContactPoint( qwContactPointID, vA, vB, -vNormal, fMinPenetration );
//
//        return true;
//    }
//
//        // Facet-Face case
//    // Select reference & incident faces
//    Vertex3 vCenterReference, vCenterIncident;
//    Vector3 vExtentsReference, vExtentsIncident;
//    Matrix3 matRotReference, matRotIncident;
//    Matrix3 matInvRotIncident;
//    if ( iMinPenetrationAxis < 4 ) {
//        vCenterReference = pBoxA->GetCenter();
//        vCenterIncident = pBoxB->GetCenter();
//        vExtentsReference = vExtentsA;
//        vExtentsIncident = vExtentsB;
//        matRotReference = matRotationA;
//        matRotIncident = matRotationB;
//    } else {
//        vCenterReference = pBoxB->GetCenter();
//        vCenterIncident = pBoxA->GetCenter();
//        vExtentsReference = vExtentsB;
//        vExtentsIncident = vExtentsA;
//        matRotReference = matRotationB;
//        matRotIncident = matRotationA;
//    }
//    matRotIncident.Transpose( matInvRotIncident );
//
//    // Compute reference normal (dotted with incident box axes)
//    Vector3 vSavedNormalReference;
//    Vector3 vNormalReference, vAbsNormalReference;
//    if ( iMinPenetrationAxis < 4 )
//        vSavedNormalReference = vNormal;
//    else
//        vSavedNormalReference = -vNormal;
//    vNormalReference = ( matInvRotIncident * vSavedNormalReference );
//    vAbsNormalReference.X = MathFn->Abs( vNormalReference.X );
//    vAbsNormalReference.Y = MathFn->Abs( vNormalReference.Y );
//    vAbsNormalReference.Z = MathFn->Abs( vNormalReference.Z );
//
//    // Compute incident face axis indices (offset 0 => normal)
//    UInt iIncidentFaceAxis[3];
//    if ( vAbsNormalReference[0] > vAbsNormalReference[1] ) {
//        if ( vAbsNormalReference[0] > vAbsNormalReference[2] ) {
//            iIncidentFaceAxis[0] = 0;
//            iIncidentFaceAxis[1] = 1;
//            iIncidentFaceAxis[2] = 2;
//        } else {
//            iIncidentFaceAxis[0] = 2;
//            iIncidentFaceAxis[1] = 0;
//            iIncidentFaceAxis[2] = 1;
//        }
//    } else {
//        if ( vAbsNormalReference[1] > vAbsNormalReference[2] ) {
//            iIncidentFaceAxis[0] = 1;
//            iIncidentFaceAxis[1] = 0;
//            iIncidentFaceAxis[2] = 2;
//        } else {
//            iIncidentFaceAxis[0] = 2;
//            iIncidentFaceAxis[1] = 0;
//            iIncidentFaceAxis[2] = 1;
//        }
//    }
//
//    // Compute center point for incident face, in reference face's frame
//    Vector3 vFaceCenterIncident = ( vCenterIncident - vCenterReference );
//    if ( vNormalReference[iIncidentFaceAxis[0]] < 0 ) {
//        for( i = 0; i < 3; ++i )
//            vFaceCenterIncident[i] += ( vExtentsIncident[iIncidentFaceAxis[0]] * matRotIncident(i,iIncidentFaceAxis[0]) );
//    } else {
//        for( i = 0; i < 3; ++i )
//            vFaceCenterIncident[i] -= ( vExtentsIncident[iIncidentFaceAxis[0]] * matRotIncident(i,iIncidentFaceAxis[0]) );
//    }
//
//    // Compute reference face axis indices (offset 0 => normal)
//    UInt iReferenceFaceAxis[3];
//    if ( iMinPenetrationAxis < 4 )
//        iReferenceFaceAxis[0] = iMinPenetrationAxis - 1;
//    else
//        iReferenceFaceAxis[0] = iMinPenetrationAxis - 4;
//    if ( iReferenceFaceAxis[0] == 0 ) {
//        iReferenceFaceAxis[1] = 1;
//        iReferenceFaceAxis[2] = 2;
//    } else if ( iReferenceFaceAxis[0] == 1 ) {
//        iReferenceFaceAxis[1] = 0;
//        iReferenceFaceAxis[2] = 2;
//    } else {
//        iReferenceFaceAxis[1] = 0;
//        iReferenceFaceAxis[2] = 1;
//    }
//
//    // Compute the 4 vertices of incident face, in reference face's frame
//    Vector3 vReferenceAxis1, vReferenceAxis2;
//    matRotReference.GetColumn( vReferenceAxis1, iReferenceFaceAxis[1] );
//    matRotReference.GetColumn( vReferenceAxis2, iReferenceFaceAxis[2] );
//
//    Vector3 vIncidentAxis1, vIncidentAxis2;
//    matRotIncident.GetColumn( vIncidentAxis1, iIncidentFaceAxis[1] );
//    matRotIncident.GetColumn( vIncidentAxis2, iIncidentFaceAxis[2] );
//
//    Scalar fDot1 = ( vFaceCenterIncident * vReferenceAxis1 );    
//    Scalar fDot2 = ( vFaceCenterIncident * vReferenceAxis2 );
//
//    Scalar fDot11 = ( vReferenceAxis1 * vIncidentAxis1 ) * vExtentsIncident[iIncidentFaceAxis[1]];
//    Scalar fDot21 = ( vReferenceAxis2 * vIncidentAxis1 ) * vExtentsIncident[iIncidentFaceAxis[1]];
//    Scalar fDot12 = ( vReferenceAxis1 * vIncidentAxis2 ) * vExtentsIncident[iIncidentFaceAxis[2]];
//    Scalar fDot22 = ( vReferenceAxis2 * vIncidentAxis2 ) * vExtentsIncident[iIncidentFaceAxis[2]];
//
//    Vertex2 vQuadIncident[4];
//    vQuadIncident[0].X = fDot1 - fDot11 - fDot12;
//    vQuadIncident[0].Y = fDot2 - fDot21 - fDot22;
//    vQuadIncident[1].X = fDot1 - fDot11 + fDot12;
//    vQuadIncident[1].Y = fDot2 - fDot21 + fDot22;
//    vQuadIncident[2].X = fDot1 + fDot11 + fDot12;
//    vQuadIncident[2].Y = fDot2 + fDot21 + fDot22;
//    vQuadIncident[3].X = fDot1 + fDot11 - fDot12;
//    vQuadIncident[3].Y = fDot2 + fDot21 - fDot22;
//
//    // Compute the 4 vertices of reference face
//    Vertex2 vRectReferenceSize;
//    vRectReferenceSize.X = vExtentsReference[iReferenceFaceAxis[1]];
//    vRectReferenceSize.Y = vExtentsReference[iReferenceFaceAxis[2]];
//
//    // Clip reference & incident faces together
//    Vertex2 vClippedPolygon[8];
//    UInt iClippedPolygonSize = _ClipFacePolygons( vClippedPolygon, vQuadIncident, vRectReferenceSize );
//    Assert( iClippedPolygonSize > 0 );
//
//    // Convert back to reference face's frame, compute distances
//    // Select penetrating points only
//    Vector3 vContactPoint[8];
//    Scalar fPenetration[8];
//
//    Scalar fInvDet = MathFn->Invert( (fDot11 * fDot22) - (fDot12 * fDot21) );
//    fDot11 *= fInvDet;
//    fDot12 *= fInvDet;
//    fDot21 *= fInvDet;
//    fDot22 *= fInvDet;
//
//    Scalar fAxisWeight1, fAxisWeight2;
//    UInt iPenetratingPointCount = 0;
//    for( i = 0; i < iClippedPolygonSize; ++i ) {
//        fAxisWeight1 = +fDot22 * ( vClippedPolygon[i].X - fDot1) - fDot12 * ( vClippedPolygon[i].Y - fDot2);
//        fAxisWeight2 = -fDot21 * ( vClippedPolygon[i].X - fDot1) + fDot11 * ( vClippedPolygon[i].Y - fDot2);
//        vContactPoint[iPenetratingPointCount] = vFaceCenterIncident;
//        vContactPoint[iPenetratingPointCount] += vIncidentAxis1 * fAxisWeight1;
//        vContactPoint[iPenetratingPointCount] += vIncidentAxis2 * fAxisWeight2;
//        fPenetration[iPenetratingPointCount] = vExtentsReference[iReferenceFaceAxis[0]] -
//                                               (vSavedNormalReference * vContactPoint[iPenetratingPointCount]);
//        if ( fPenetration[iPenetratingPointCount] >= 0 ) {
//            vClippedPolygon[iPenetratingPointCount] = vClippedPolygon[i];
//            ++iPenetratingPointCount;
//        }
//    }
//    Assert( iPenetratingPointCount > 0 );
//
//    // Cull extra points if needed
//    Vertex3 vCurPoint;
//
//    if ( iPenetratingPointCount <= NARROWPHASE_MAX_CONTACTPOINTS ) {
//        // Use them all
//        m_iContactPointCount = 0;
//        for( i = 0; i < iPenetratingPointCount; ++i ) {
//            vCurPoint = ( vA + vContactPoint[i] );
//            if ( iMinPenetrationAxis < 4 ) {
//                _AddContactPoint( 0, vCurPoint,
//                                     vCurPoint - ( vNormal * fPenetration[i] ),
//                                     -vNormal, -fPenetration[i] );
//            } else {
//                _AddContactPoint( 0, vCurPoint - ( vNormal * fPenetration[i] ),
//                                     vCurPoint,
//                                     -vNormal, -fPenetration[i] );
//            }
//        }
//    } else {
//        // Find deepest penetration point
//        UInt iDeepestContactPoint = 0;
//        Scalar fDeepestPenetration = fPenetration[0];
//        for( i = 1; i < iPenetratingPointCount; ++i ) {
//            if ( fPenetration[i] > fDeepestPenetration ) {
//                iDeepestContactPoint = i;
//                fDeepestPenetration = fPenetration[i];
//            }
//        }
//
//        // Perform culling
//        UInt iCulledPolygon[NARROWPHASE_MAX_CONTACTPOINTS];
//        _CullContactPolygon( iCulledPolygon, vClippedPolygon, iPenetratingPointCount, iDeepestContactPoint );
//
//        // Use remaining points
//        m_iContactPointCount = 0;
//        for( i = 0; i < NARROWPHASE_MAX_CONTACTPOINTS; ++i ) {
//            vCurPoint = ( vA + vContactPoint[ iCulledPolygon[i] ] );
//            if ( iMinPenetrationAxis < 4 ) {
//                _AddContactPoint( 0, vCurPoint,
//                                     vCurPoint - ( vNormal * fPenetration[ iCulledPolygon[i] ] ),
//                                     -vNormal, -fPenetration[ iCulledPolygon[i] ] );
//            } else {
//                _AddContactPoint( 0, vCurPoint - ( vNormal * fPenetration[ iCulledPolygon[i] ] ),
//                                     vCurPoint,
//                                     -vNormal, -fPenetration[ iCulledPolygon[i] ] );
//            }
//        }
//    }
//
//    // Done
//    return true;
//}
//
//#undef _SAT_FACETS
//#undef _SAT_EDGES
//
///////////////////////////////////////////////////////////////////////////////////
//
//Void BoxBoxCollision::_EdgesClosestPoints( Scalar * outParamA, Scalar * outParamB,
//                                           const Vertex3 & vA, const Vector3 & vAxisA,
//                                           const Vertex3 & vB, const Vector3 & vAxisB )
//{
//    Vector3 vU = ( vB - vA );
//    Scalar fDotU = ( vAxisA * vAxisB );
//    Scalar fDotA = +( vAxisA * vU );
//    Scalar fDotB = -( vAxisB * vU );
//    Scalar fT = 1.0f - (fDotU * fDotU);
//
//    *outParamA = 0.0f;
//    *outParamB = 0.0f;
//    if ( fT > SCALAR_ERROR ) {
//        fT = MathFn->Invert( fT );
//        *outParamA = fT * ( fDotA + (fDotU * fDotB) );
//        *outParamB = fT * ( (fDotU * fDotA) + fDotB );
//    }
//}
//UInt BoxBoxCollision::_ClipFacePolygons( Vertex2 outClippedPolygon[8],
//                                         Vertex2 vQuadIncident[4], const Vertex2 & vRectReferenceSize )
//{
//    Vertex2 vTempPolygon[8];
//
//    UInt iClippedCount = 0;
//    Vertex2 * pClippedPolygon = outClippedPolygon;
//
//    UInt iCurrentCount = 4;
//    Vertex2 * pCurrentPolygon = vQuadIncident;
//
//    for( UInt iAxis = 0; iAxis < 2; ++iAxis ) {
//        for( Int iSign = -1; iSign <= +1; iSign += 2 ) {
//            Vertex2 * pCurr = pCurrentPolygon;
//            Vertex2 * pClip = pClippedPolygon;
//            iClippedCount = 0;
//
//            // Walk through current polygon
//            for( Int i = iCurrentCount; i > 0; --i ) {
//                // Check if point is inside clipped polygon
//                if ( iSign * (*pCurr)[iAxis] < vRectReferenceSize[iAxis] ) {
//                    *pClip = *pCurr;
//                    ++pClip;
//                    ++iClippedCount;
//                    if ( iClippedCount >= 8 ) {
//                        pCurrentPolygon = pClippedPolygon;
//                        goto _ClipFacePolygons_Done;
//                    }
//                }
//
//                // Check if edge crosses clipped polygon
//                Vertex2 * pNextCurr = (i > 1) ? (pCurr + 1) : pCurrentPolygon;
//                if ( (iSign * (*pCurr)[iAxis] < vRectReferenceSize[iAxis]) ^
//                     (iSign * (*pNextCurr)[iAxis] < vRectReferenceSize[iAxis]) ) {
//                    (*pClip)[1 - iAxis] = (*pCurr)[1 - iAxis]
//                                       + ((*pNextCurr)[1 - iAxis] - (*pCurr)[1 - iAxis]) / ((*pNextCurr)[iAxis] - (*pCurr)[iAxis])
//                                       * (iSign * vRectReferenceSize[iAxis] - (*pCurr)[iAxis]);
//                    (*pClip)[iAxis] = iSign * vRectReferenceSize[iAxis];
//                    ++pClip;
//                    ++iClippedCount;
//                    if ( iClippedCount >= 8 ) {
//                        pCurrentPolygon = pClippedPolygon;
//                        goto _ClipFacePolygons_Done;
//                    }
//                }
//
//                ++pCurr;
//            }
//
//            pCurrentPolygon = pClippedPolygon;
//            pClippedPolygon = ( pCurrentPolygon == outClippedPolygon ) ? vTempPolygon : outClippedPolygon;
//            iCurrentCount = iClippedCount;
//        }
//    }
//
//_ClipFacePolygons_Done:
//    if ( pCurrentPolygon != outClippedPolygon )
//        MemCopy( outClippedPolygon, pCurrentPolygon, iClippedCount * sizeof(Vertex2) );
//    return iClippedCount;
//}
//Void BoxBoxCollision::_CullContactPolygon( UInt outCulledPolygon[NARROWPHASE_MAX_CONTACTPOINTS],
//                                           Vertex2 vClippedPolygon[8], UInt iClippedPolygonSize, UInt iMustKeep )
//{
//    UInt i, j;
//
//    // Compute centroid of polygon
//    Vertex2 vCenter;
//    if ( iClippedPolygonSize == 1 )
//        vCenter = vClippedPolygon[0];
//    else if ( iClippedPolygonSize == 2 ) {
//        vCenter.X = ( vClippedPolygon[0].X + vClippedPolygon[1].X ) * 0.5f;
//        vCenter.Y = ( vClippedPolygon[0].Y + vClippedPolygon[1].Y ) * 0.5f;
//    } else {
//        Scalar fWeight, fWeightSum = 0.0f;
//        vCenter = Vertex2::Null;
//        for( i = 0; i < iClippedPolygonSize - 1; ++i ) {
//            fWeight = ( vClippedPolygon[i].X * vClippedPolygon[i+1].Y - vClippedPolygon[i+1].X * vClippedPolygon[i].Y );
//            fWeightSum += fWeight;
//            vCenter.X += ( vClippedPolygon[i].X + vClippedPolygon[i+1].X ) * fWeight;
//            vCenter.Y += ( vClippedPolygon[i].Y + vClippedPolygon[i+1].Y ) * fWeight;
//        }
//        fWeight = ( vClippedPolygon[iClippedPolygonSize - 1].X * vClippedPolygon[0].Y -
//                    vClippedPolygon[0].X * vClippedPolygon[iClippedPolygonSize - 1].Y );
//        fWeightSum += fWeight;
//        vCenter.X += ( vClippedPolygon[iClippedPolygonSize - 1].X + vClippedPolygon[0].X ) * fWeight;
//        vCenter.Y += ( vClippedPolygon[iClippedPolygonSize - 1].Y + vClippedPolygon[0].Y ) * fWeight;
//
//        if ( MathFn->Abs(fWeightSum) > SCALAR_EPSILON )
//            fWeightSum = MathFn->Invert( fWeightSum * 3.0f );
//        else
//            fWeightSum = SCALAR_INFINITE;
//        vCenter *= fWeightSum;
//    }
//
//    // Compute angles from the centroid
//    Scalar fAngle[8];
//    for( i = 0; i < iClippedPolygonSize; ++i )
//        fAngle[i] = MathFn->ArcTan2( vClippedPolygon[i].Y - vCenter.Y, vClippedPolygon[i].X - vCenter.X );
//
//    // We want points with angle closest to fAngle[iMustKeep] + i*(2pi / NARROWPHASE_MAX_CONTACTPOINTS)
//    Bool bCanBeCulled[8];
//    for( i = 0; i < iClippedPolygonSize; ++i )
//        bCanBeCulled[i] = true;
//    bCanBeCulled[iMustKeep] = false;
//
//    outCulledPolygon[0] = iMustKeep;
//    UInt * pCulled = ( outCulledPolygon + 1 );
//
//    Scalar fAngleMaxDiff;
//    Scalar fCurAngle, fAngleDiff;
//    for( i = 1; i < NARROWPHASE_MAX_CONTACTPOINTS; ++i ) {
//        fCurAngle = fAngle[iMustKeep] + ( (SCALAR_2PI / NARROWPHASE_MAX_CONTACTPOINTS) * (Scalar)i );
//        if ( fCurAngle > SCALAR_PI )
//            fCurAngle -= SCALAR_2PI;
//
//        fAngleMaxDiff = SCALAR_INFINITE;
//        *pCulled = INVALID_OFFSET;
//        for( j = 0; j < iClippedPolygonSize; ++j ) {
//            if ( bCanBeCulled[j] ) {
//                fAngleDiff = MathFn->Abs( fAngle[j] - fCurAngle );
//                if ( fAngleDiff > SCALAR_PI )
//                    fAngleDiff = SCALAR_2PI - fAngleDiff;
//                if ( fAngleDiff < fAngleMaxDiff ) {
//                    fAngleMaxDiff = fAngleDiff;
//                    *pCulled = j;
//                }
//            }
//        }
//
//        Assert( *pCulled != INVALID_OFFSET );
//        bCanBeCulled[*pCulled] = false;
//        ++pCulled;
//    }
//}


//00001 /*
//00002  * Box-Box collision detection re-distributed under the ZLib license with permission from Russell L. Smith
//00003  * Original version is from Open Dynamics Engine, Copyright (C) 2001,2002 Russell L. Smith.
//00004  * All rights reserved.  Email: russ@q12.org   Web: www.q12.org
//00005  Bullet Continuous Collision Detection and Physics Library
//00006  Bullet is Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/
//00007 
//00008 This software is provided 'as-is', without any express or implied warranty.
//00009 In no event will the authors be held liable for any damages arising from the use of this software.
//00010 Permission is granted to anyone to use this software for any purpose, 
//00011 including commercial applications, and to alter it and redistribute it freely, 
//00012 subject to the following restrictions:
//00013 
//00014 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
//00015 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
//00016 3. This notice may not be removed or altered from any source distribution.
//00017 */
//00018 
//00020 
//00021 #include "btBoxBoxDetector.h"
//00022 #include "BulletCollision/CollisionShapes/btBoxShape.h"
//00023 
//00024 #include <float.h>
//00025 #include <string.h>
//00026 
//00027 btBoxBoxDetector::btBoxBoxDetector(btBoxShape* box1,btBoxShape* box2)
//00028 : m_box1(box1),
//00029 m_box2(box2)
//00030 {
//00031 
//00032 }
//00033 
//00034 
//00035 // given two boxes (p1,R1,side1) and (p2,R2,side2), collide them together and
//00036 // generate contact points. this returns 0 if there is no contact otherwise
//00037 // it returns the number of contacts generated.
//00038 // `normal' returns the contact normal.
//00039 // `depth' returns the maximum penetration depth along that normal.
//00040 // `return_code' returns a number indicating the type of contact that was
//00041 // detected:
//00042 //        1,2,3 = box 2 intersects with a face of box 1
//00043 //        4,5,6 = box 1 intersects with a face of box 2
//00044 //        7..15 = edge-edge contact
//00045 // `maxc' is the maximum number of contacts allowed to be generated, i.e.
//00046 // the size of the `contact' array.
//00047 // `contact' and `skip' are the contact array information provided to the
//00048 // collision functions. this function only fills in the position and depth
//00049 // fields.
//00050 struct dContactGeom;
//00051 #define dDOTpq(a,b,p,q) ((a)[0]*(b)[0] + (a)[p]*(b)[q] + (a)[2*(p)]*(b)[2*(q)])
//00052 #define dInfinity FLT_MAX
//00053 
//00054 
//00055 /*PURE_INLINE btScalar dDOT   (const btScalar *a, const btScalar *b) { return dDOTpq(a,b,1,1); }
//00056 PURE_INLINE btScalar dDOT13 (const btScalar *a, const btScalar *b) { return dDOTpq(a,b,1,3); }
//00057 PURE_INLINE btScalar dDOT31 (const btScalar *a, const btScalar *b) { return dDOTpq(a,b,3,1); }
//00058 PURE_INLINE btScalar dDOT33 (const btScalar *a, const btScalar *b) { return dDOTpq(a,b,3,3); }
//00059 */
//00060 static btScalar dDOT   (const btScalar *a, const btScalar *b) { return dDOTpq(a,b,1,1); }
//00061 static btScalar dDOT44 (const btScalar *a, const btScalar *b) { return dDOTpq(a,b,4,4); }
//00062 static btScalar dDOT41 (const btScalar *a, const btScalar *b) { return dDOTpq(a,b,4,1); }
//00063 static btScalar dDOT14 (const btScalar *a, const btScalar *b) { return dDOTpq(a,b,1,4); }
//00064 #define dMULTIPLYOP1_331(A,op,B,C) \
//00065 {\
//00066   (A)[0] op dDOT41((B),(C)); \
//00067   (A)[1] op dDOT41((B+1),(C)); \
//00068   (A)[2] op dDOT41((B+2),(C)); \
//00069 }
//00070 
//00071 #define dMULTIPLYOP0_331(A,op,B,C) \
//00072 { \
//00073   (A)[0] op dDOT((B),(C)); \
//00074   (A)[1] op dDOT((B+4),(C)); \
//00075   (A)[2] op dDOT((B+8),(C)); \
//00076 } 
//00077 
//00078 #define dMULTIPLY1_331(A,B,C) dMULTIPLYOP1_331(A,=,B,C)
//00079 #define dMULTIPLY0_331(A,B,C) dMULTIPLYOP0_331(A,=,B,C)
//00080 
//00081 typedef btScalar dMatrix3[4*3];
//00082 
//00083 void dLineClosestApproach (const btVector3& pa, const btVector3& ua,
//00084                            const btVector3& pb, const btVector3& ub,
//00085                            btScalar *alpha, btScalar *beta);
//00086 void dLineClosestApproach (const btVector3& pa, const btVector3& ua,
//00087                            const btVector3& pb, const btVector3& ub,
//00088                            btScalar *alpha, btScalar *beta)
//00089 {
//00090   btVector3 p;
//00091   p[0] = pb[0] - pa[0];
//00092   p[1] = pb[1] - pa[1];
//00093   p[2] = pb[2] - pa[2];
//00094   btScalar uaub = dDOT(ua,ub);
//00095   btScalar q1 =  dDOT(ua,p);
//00096   btScalar q2 = -dDOT(ub,p);
//00097   btScalar d = 1-uaub*uaub;
//00098   if (d <= btScalar(0.0001f)) {
//00099     // @@@ this needs to be made more robust
//00100     *alpha = 0;
//00101     *beta  = 0;
//00102   }
//00103   else {
//00104     d = 1.f/d;
//00105     *alpha = (q1 + uaub*q2)*d;
//00106     *beta  = (uaub*q1 + q2)*d;
//00107   }
//00108 }
//00109 
//00110 
//00111 
//00112 // find all the intersection points between the 2D rectangle with vertices
//00113 // at (+/-h[0],+/-h[1]) and the 2D quadrilateral with vertices (p[0],p[1]),
//00114 // (p[2],p[3]),(p[4],p[5]),(p[6],p[7]).
//00115 //
//00116 // the intersection points are returned as x,y pairs in the 'ret' array.
//00117 // the number of intersection points is returned by the function (this will
//00118 // be in the range 0 to 8).
//00119 
//00120 static int intersectRectQuad2 (btScalar h[2], btScalar p[8], btScalar ret[16])
//00121 {
//00122   // q (and r) contain nq (and nr) coordinate points for the current (and
//00123   // chopped) polygons
//00124   int nq=4,nr=0;
//00125   btScalar buffer[16];
//00126   btScalar *q = p;
//00127   btScalar *r = ret;
//00128   for (int dir=0; dir <= 1; dir++) {
//00129     // direction notation: xy[0] = x axis, xy[1] = y axis
//00130     for (int sign=-1; sign <= 1; sign += 2) {
//00131       // chop q along the line xy[dir] = sign*h[dir]
//00132       btScalar *pq = q;
//00133       btScalar *pr = r;
//00134       nr = 0;
//00135       for (int i=nq; i > 0; i--) {
//00136         // go through all points in q and all lines between adjacent points
//00137         if (sign*pq[dir] < h[dir]) {
//00138           // this point is inside the chopping line
//00139           pr[0] = pq[0];
//00140           pr[1] = pq[1];
//00141           pr += 2;
//00142           nr++;
//00143           if (nr & 8) {
//00144             q = r;
//00145             goto done;
//00146           }
//00147         }
//00148         btScalar *nextq = (i > 1) ? pq+2 : q;
//00149         if ((sign*pq[dir] < h[dir]) ^ (sign*nextq[dir] < h[dir])) {
//00150           // this line crosses the chopping line
//00151           pr[1-dir] = pq[1-dir] + (nextq[1-dir]-pq[1-dir]) /
//00152             (nextq[dir]-pq[dir]) * (sign*h[dir]-pq[dir]);
//00153           pr[dir] = sign*h[dir];
//00154           pr += 2;
//00155           nr++;
//00156           if (nr & 8) {
//00157             q = r;
//00158             goto done;
//00159           }
//00160         }
//00161         pq += 2;
//00162       }
//00163       q = r;
//00164       r = (q==ret) ? buffer : ret;
//00165       nq = nr;
//00166     }
//00167   }
//00168  done:
//00169   if (q != ret) memcpy (ret,q,nr*2*sizeof(btScalar));
//00170   return nr;
//00171 }
//00172 
//00173 
//00174 #define M__PI 3.14159265f
//00175 
//00176 // given n points in the plane (array p, of size 2*n), generate m points that
//00177 // best represent the whole set. the definition of 'best' here is not
//00178 // predetermined - the idea is to select points that give good box-box
//00179 // collision detection behavior. the chosen point indexes are returned in the
//00180 // array iret (of size m). 'i0' is always the first entry in the array.
//00181 // n must be in the range [1..8]. m must be in the range [1..n]. i0 must be
//00182 // in the range [0..n-1].
//00183 
//00184 void cullPoints2 (int n, btScalar p[], int m, int i0, int iret[]);
//00185 void cullPoints2 (int n, btScalar p[], int m, int i0, int iret[])
//00186 {
//00187   // compute the centroid of the polygon in cx,cy
//00188   int i,j;
//00189   btScalar a,cx,cy,q;
//00190   if (n==1) {
//00191     cx = p[0];
//00192     cy = p[1];
//00193   }
//00194   else if (n==2) {
//00195     cx = btScalar(0.5)*(p[0] + p[2]);
//00196     cy = btScalar(0.5)*(p[1] + p[3]);
//00197   }
//00198   else {
//00199     a = 0;
//00200     cx = 0;
//00201     cy = 0;
//00202     for (i=0; i<(n-1); i++) {
//00203       q = p[i*2]*p[i*2+3] - p[i*2+2]*p[i*2+1];
//00204       a += q;
//00205       cx += q*(p[i*2]+p[i*2+2]);
//00206       cy += q*(p[i*2+1]+p[i*2+3]);
//00207     }
//00208     q = p[n*2-2]*p[1] - p[0]*p[n*2-1];
//00209         if (btFabs(a+q) > SIMD_EPSILON)
//00210         {
//00211                 a = 1.f/(btScalar(3.0)*(a+q));
//00212         } else
//00213         {
//00214                 a=BT_LARGE_FLOAT;
//00215         }
//00216     cx = a*(cx + q*(p[n*2-2]+p[0]));
//00217     cy = a*(cy + q*(p[n*2-1]+p[1]));
//00218   }
//00219 
//00220   // compute the angle of each point w.r.t. the centroid
//00221   btScalar A[8];
//00222   for (i=0; i<n; i++) A[i] = btAtan2(p[i*2+1]-cy,p[i*2]-cx);
//00223 
//00224   // search for points that have angles closest to A[i0] + i*(2*pi/m).
//00225   int avail[8];
//00226   for (i=0; i<n; i++) avail[i] = 1;
//00227   avail[i0] = 0;
//00228   iret[0] = i0;
//00229   iret++;
//00230   for (j=1; j<m; j++) {
//00231     a = btScalar(j)*(2*M__PI/m) + A[i0];
//00232     if (a > M__PI) a -= 2*M__PI;
//00233     btScalar maxdiff=1e9,diff;
//00234 
//00235     *iret = i0;                 // iret is not allowed to keep this value, but it sometimes does, when diff=#QNAN0
//00236 
//00237     for (i=0; i<n; i++) {
//00238       if (avail[i]) {
//00239         diff = btFabs (A[i]-a);
//00240         if (diff > M__PI) diff = 2*M__PI - diff;
//00241         if (diff < maxdiff) {
//00242           maxdiff = diff;
//00243           *iret = i;
//00244         }
//00245       }
//00246     }
//00247 #if defined(DEBUG) || defined (_DEBUG)
//00248     btAssert (*iret != i0);     // ensure iret got set
//00249 #endif
//00250     avail[*iret] = 0;
//00251     iret++;
//00252   }
//00253 }
//00254 
//00255 
//00256 
//00257 int dBoxBox2 (const btVector3& p1, const dMatrix3 R1,
//00258              const btVector3& side1, const btVector3& p2,
//00259              const dMatrix3 R2, const btVector3& side2,
//00260              btVector3& normal, btScalar *depth, int *return_code,
//00261                  int maxc, dContactGeom * /*contact*/, int /*skip*/,btDiscreteCollisionDetectorInterface::Result& output);
//00262 int dBoxBox2 (const btVector3& p1, const dMatrix3 R1,
//00263              const btVector3& side1, const btVector3& p2,
//00264              const dMatrix3 R2, const btVector3& side2,
//00265              btVector3& normal, btScalar *depth, int *return_code,
//00266                  int maxc, dContactGeom * /*contact*/, int /*skip*/,btDiscreteCollisionDetectorInterface::Result& output)
//00267 {
//00268   const btScalar fudge_factor = btScalar(1.05);
//00269   btVector3 p,pp,normalC(0.f,0.f,0.f);
//00270   const btScalar *normalR = 0;
//00271   btScalar A[3],B[3],R11,R12,R13,R21,R22,R23,R31,R32,R33,
//00272     Q11,Q12,Q13,Q21,Q22,Q23,Q31,Q32,Q33,s,s2,l;
//00273   int i,j,invert_normal,code;
//00274 
//00275   // get vector from centers of box 1 to box 2, relative to box 1
//00276   p = p2 - p1;
//00277   dMULTIPLY1_331 (pp,R1,p);             // get pp = p relative to body 1
//00278 
//00279   // get side lengths / 2
//00280   A[0] = side1[0]*btScalar(0.5);
//00281   A[1] = side1[1]*btScalar(0.5);
//00282   A[2] = side1[2]*btScalar(0.5);
//00283   B[0] = side2[0]*btScalar(0.5);
//00284   B[1] = side2[1]*btScalar(0.5);
//00285   B[2] = side2[2]*btScalar(0.5);
//00286 
//00287   // Rij is R1'*R2, i.e. the relative rotation between R1 and R2
//00288   R11 = dDOT44(R1+0,R2+0); R12 = dDOT44(R1+0,R2+1); R13 = dDOT44(R1+0,R2+2);
//00289   R21 = dDOT44(R1+1,R2+0); R22 = dDOT44(R1+1,R2+1); R23 = dDOT44(R1+1,R2+2);
//00290   R31 = dDOT44(R1+2,R2+0); R32 = dDOT44(R1+2,R2+1); R33 = dDOT44(R1+2,R2+2);
//00291 
//00292   Q11 = btFabs(R11); Q12 = btFabs(R12); Q13 = btFabs(R13);
//00293   Q21 = btFabs(R21); Q22 = btFabs(R22); Q23 = btFabs(R23);
//00294   Q31 = btFabs(R31); Q32 = btFabs(R32); Q33 = btFabs(R33);
//00295 
//00296   // for all 15 possible separating axes:
//00297   //   * see if the axis separates the boxes. if so, return 0.
//00298   //   * find the depth of the penetration along the separating axis (s2)
//00299   //   * if this is the largest depth so far, record it.
//00300   // the normal vector will be set to the separating axis with the smallest
//00301   // depth. note: normalR is set to point to a column of R1 or R2 if that is
//00302   // the smallest depth normal so far. otherwise normalR is 0 and normalC is
//00303   // set to a vector relative to body 1. invert_normal is 1 if the sign of
//00304   // the normal should be flipped.
//00305 
//00306 #define TST(expr1,expr2,norm,cc) \
//00307   s2 = btFabs(expr1) - (expr2); \
//00308   if (s2 > 0) return 0; \
//00309   if (s2 > s) { \
//00310     s = s2; \
//00311     normalR = norm; \
//00312     invert_normal = ((expr1) < 0); \
//00313     code = (cc); \
//00314   }
//00315 
//00316   s = -dInfinity;
//00317   invert_normal = 0;
//00318   code = 0;
//00319 
//00320   // separating axis = u1,u2,u3
//00321   TST (pp[0],(A[0] + B[0]*Q11 + B[1]*Q12 + B[2]*Q13),R1+0,1);
//00322   TST (pp[1],(A[1] + B[0]*Q21 + B[1]*Q22 + B[2]*Q23),R1+1,2);
//00323   TST (pp[2],(A[2] + B[0]*Q31 + B[1]*Q32 + B[2]*Q33),R1+2,3);
//00324 
//00325   // separating axis = v1,v2,v3
//00326   TST (dDOT41(R2+0,p),(A[0]*Q11 + A[1]*Q21 + A[2]*Q31 + B[0]),R2+0,4);
//00327   TST (dDOT41(R2+1,p),(A[0]*Q12 + A[1]*Q22 + A[2]*Q32 + B[1]),R2+1,5);
//00328   TST (dDOT41(R2+2,p),(A[0]*Q13 + A[1]*Q23 + A[2]*Q33 + B[2]),R2+2,6);
//00329 
//00330   // note: cross product axes need to be scaled when s is computed.
//00331   // normal (n1,n2,n3) is relative to box 1.
//00332 #undef TST
//00333 #define TST(expr1,expr2,n1,n2,n3,cc) \
//00334   s2 = btFabs(expr1) - (expr2); \
//00335   if (s2 > SIMD_EPSILON) return 0; \
//00336   l = btSqrt((n1)*(n1) + (n2)*(n2) + (n3)*(n3)); \
//00337   if (l > SIMD_EPSILON) { \
//00338     s2 /= l; \
//00339     if (s2*fudge_factor > s) { \
//00340       s = s2; \
//00341       normalR = 0; \
//00342       normalC[0] = (n1)/l; normalC[1] = (n2)/l; normalC[2] = (n3)/l; \
//00343       invert_normal = ((expr1) < 0); \
//00344       code = (cc); \
//00345     } \
//00346   }
//00347 
//00348   // separating axis = u1 x (v1,v2,v3)
//00349   TST(pp[2]*R21-pp[1]*R31,(A[1]*Q31+A[2]*Q21+B[1]*Q13+B[2]*Q12),0,-R31,R21,7);
//00350   TST(pp[2]*R22-pp[1]*R32,(A[1]*Q32+A[2]*Q22+B[0]*Q13+B[2]*Q11),0,-R32,R22,8);
//00351   TST(pp[2]*R23-pp[1]*R33,(A[1]*Q33+A[2]*Q23+B[0]*Q12+B[1]*Q11),0,-R33,R23,9);
//00352 
//00353   // separating axis = u2 x (v1,v2,v3)
//00354   TST(pp[0]*R31-pp[2]*R11,(A[0]*Q31+A[2]*Q11+B[1]*Q23+B[2]*Q22),R31,0,-R11,10);
//00355   TST(pp[0]*R32-pp[2]*R12,(A[0]*Q32+A[2]*Q12+B[0]*Q23+B[2]*Q21),R32,0,-R12,11);
//00356   TST(pp[0]*R33-pp[2]*R13,(A[0]*Q33+A[2]*Q13+B[0]*Q22+B[1]*Q21),R33,0,-R13,12);
//00357 
//00358   // separating axis = u3 x (v1,v2,v3)
//00359   TST(pp[1]*R11-pp[0]*R21,(A[0]*Q21+A[1]*Q11+B[1]*Q33+B[2]*Q32),-R21,R11,0,13);
//00360   TST(pp[1]*R12-pp[0]*R22,(A[0]*Q22+A[1]*Q12+B[0]*Q33+B[2]*Q31),-R22,R12,0,14);
//00361   TST(pp[1]*R13-pp[0]*R23,(A[0]*Q23+A[1]*Q13+B[0]*Q32+B[1]*Q31),-R23,R13,0,15);
//00362 
//00363 #undef TST
//00364 
//00365   if (!code) return 0;
//00366 
//00367   // if we get to this point, the boxes interpenetrate. compute the normal
//00368   // in global coordinates.
//00369   if (normalR) {
//00370     normal[0] = normalR[0];
//00371     normal[1] = normalR[4];
//00372     normal[2] = normalR[8];
//00373   }
//00374   else {
//00375     dMULTIPLY0_331 (normal,R1,normalC);
//00376   }
//00377   if (invert_normal) {
//00378     normal[0] = -normal[0];
//00379     normal[1] = -normal[1];
//00380     normal[2] = -normal[2];
//00381   }
//00382   *depth = -s;
//00383 
//00384   // compute contact point(s)
//00385 
//00386   if (code > 6) {
//00387     // an edge from box 1 touches an edge from box 2.
//00388     // find a point pa on the intersecting edge of box 1
//00389     btVector3 pa;
//00390     btScalar sign;
//00391     for (i=0; i<3; i++) pa[i] = p1[i];
//00392     for (j=0; j<3; j++) {
//00393       sign = (dDOT14(normal,R1+j) > 0) ? btScalar(1.0) : btScalar(-1.0);
//00394       for (i=0; i<3; i++) pa[i] += sign * A[j] * R1[i*4+j];
//00395     }
//00396 
//00397     // find a point pb on the intersecting edge of box 2
//00398     btVector3 pb;
//00399     for (i=0; i<3; i++) pb[i] = p2[i];
//00400     for (j=0; j<3; j++) {
//00401       sign = (dDOT14(normal,R2+j) > 0) ? btScalar(-1.0) : btScalar(1.0);
//00402       for (i=0; i<3; i++) pb[i] += sign * B[j] * R2[i*4+j];
//00403     }
//00404 
//00405     btScalar alpha,beta;
//00406     btVector3 ua,ub;
//00407     for (i=0; i<3; i++) ua[i] = R1[((code)-7)/3 + i*4];
//00408     for (i=0; i<3; i++) ub[i] = R2[((code)-7)%3 + i*4];
//00409 
//00410     dLineClosestApproach (pa,ua,pb,ub,&alpha,&beta);
//00411     for (i=0; i<3; i++) pa[i] += ua[i]*alpha;
//00412     for (i=0; i<3; i++) pb[i] += ub[i]*beta;
//00413 
//00414         {
//00415                 
//00416                 //contact[0].pos[i] = btScalar(0.5)*(pa[i]+pb[i]);
//00417                 //contact[0].depth = *depth;
//00418                 btVector3 pointInWorld;
//00419 
//00420 #ifdef USE_CENTER_POINT
//00421             for (i=0; i<3; i++) 
//00422                         pointInWorld[i] = (pa[i]+pb[i])*btScalar(0.5);
//00423                 output.addContactPoint(-normal,pointInWorld,-*depth);
//00424 #else
//00425                 output.addContactPoint(-normal,pb,-*depth);
//00426 
//00427 #endif //
//00428                 *return_code = code;
//00429         }
//00430     return 1;
//00431   }
//00432 
//00433   // okay, we have a face-something intersection (because the separating
//00434   // axis is perpendicular to a face). define face 'a' to be the reference
//00435   // face (i.e. the normal vector is perpendicular to this) and face 'b' to be
//00436   // the incident face (the closest face of the other box).
//00437 
//00438   const btScalar *Ra,*Rb,*pa,*pb,*Sa,*Sb;
//00439   if (code <= 3) {
//00440     Ra = R1;
//00441     Rb = R2;
//00442     pa = p1;
//00443     pb = p2;
//00444     Sa = A;
//00445     Sb = B;
//00446   }
//00447   else {
//00448     Ra = R2;
//00449     Rb = R1;
//00450     pa = p2;
//00451     pb = p1;
//00452     Sa = B;
//00453     Sb = A;
//00454   }
//00455 
//00456   // nr = normal vector of reference face dotted with axes of incident box.
//00457   // anr = absolute values of nr.
//00458   btVector3 normal2,nr,anr;
//00459   if (code <= 3) {
//00460     normal2[0] = normal[0];
//00461     normal2[1] = normal[1];
//00462     normal2[2] = normal[2];
//00463   }
//00464   else {
//00465     normal2[0] = -normal[0];
//00466     normal2[1] = -normal[1];
//00467     normal2[2] = -normal[2];
//00468   }
//00469   dMULTIPLY1_331 (nr,Rb,normal2);
//00470   anr[0] = btFabs (nr[0]);
//00471   anr[1] = btFabs (nr[1]);
//00472   anr[2] = btFabs (nr[2]);
//00473 
//00474   // find the largest compontent of anr: this corresponds to the normal
//00475   // for the indident face. the other axis numbers of the indicent face
//00476   // are stored in a1,a2.
//00477   int lanr,a1,a2;
//00478   if (anr[1] > anr[0]) {
//00479     if (anr[1] > anr[2]) {
//00480       a1 = 0;
//00481       lanr = 1;
//00482       a2 = 2;
//00483     }
//00484     else {
//00485       a1 = 0;
//00486       a2 = 1;
//00487       lanr = 2;
//00488     }
//00489   }
//00490   else {
//00491     if (anr[0] > anr[2]) {
//00492       lanr = 0;
//00493       a1 = 1;
//00494       a2 = 2;
//00495     }
//00496     else {
//00497       a1 = 0;
//00498       a2 = 1;
//00499       lanr = 2;
//00500     }
//00501   }
//00502 
//00503   // compute center point of incident face, in reference-face coordinates
//00504   btVector3 center;
//00505   if (nr[lanr] < 0) {
//00506     for (i=0; i<3; i++) center[i] = pb[i] - pa[i] + Sb[lanr] * Rb[i*4+lanr];
//00507   }
//00508   else {
//00509     for (i=0; i<3; i++) center[i] = pb[i] - pa[i] - Sb[lanr] * Rb[i*4+lanr];
//00510   }
//00511 
//00512   // find the normal and non-normal axis numbers of the reference box
//00513   int codeN,code1,code2;
//00514   if (code <= 3) codeN = code-1; else codeN = code-4;
//00515   if (codeN==0) {
//00516     code1 = 1;
//00517     code2 = 2;
//00518   }
//00519   else if (codeN==1) {
//00520     code1 = 0;
//00521     code2 = 2;
//00522   }
//00523   else {
//00524     code1 = 0;
//00525     code2 = 1;
//00526   }
//00527 
//00528   // find the four corners of the incident face, in reference-face coordinates
//00529   btScalar quad[8];     // 2D coordinate of incident face (x,y pairs)
//00530   btScalar c1,c2,m11,m12,m21,m22;
//00531   c1 = dDOT14 (center,Ra+code1);
//00532   c2 = dDOT14 (center,Ra+code2);
//00533   // optimize this? - we have already computed this data above, but it is not
//00534   // stored in an easy-to-index format. for now it's quicker just to recompute
//00535   // the four dot products.
//00536   m11 = dDOT44 (Ra+code1,Rb+a1);
//00537   m12 = dDOT44 (Ra+code1,Rb+a2);
//00538   m21 = dDOT44 (Ra+code2,Rb+a1);
//00539   m22 = dDOT44 (Ra+code2,Rb+a2);
//00540   {
//00541     btScalar k1 = m11*Sb[a1];
//00542     btScalar k2 = m21*Sb[a1];
//00543     btScalar k3 = m12*Sb[a2];
//00544     btScalar k4 = m22*Sb[a2];
//00545     quad[0] = c1 - k1 - k3;
//00546     quad[1] = c2 - k2 - k4;
//00547     quad[2] = c1 - k1 + k3;
//00548     quad[3] = c2 - k2 + k4;
//00549     quad[4] = c1 + k1 + k3;
//00550     quad[5] = c2 + k2 + k4;
//00551     quad[6] = c1 + k1 - k3;
//00552     quad[7] = c2 + k2 - k4;
//00553   }
//00554 
//00555   // find the size of the reference face
//00556   btScalar rect[2];
//00557   rect[0] = Sa[code1];
//00558   rect[1] = Sa[code2];
//00559 
//00560   // intersect the incident and reference faces
//00561   btScalar ret[16];
//00562   int n = intersectRectQuad2 (rect,quad,ret);
//00563   if (n < 1) return 0;          // this should never happen
//00564 
//00565   // convert the intersection points into reference-face coordinates,
//00566   // and compute the contact position and depth for each point. only keep
//00567   // those points that have a positive (penetrating) depth. delete points in
//00568   // the 'ret' array as necessary so that 'point' and 'ret' correspond.
//00569   btScalar point[3*8];          // penetrating contact points
//00570   btScalar dep[8];                      // depths for those points
//00571   btScalar det1 = 1.f/(m11*m22 - m12*m21);
//00572   m11 *= det1;
//00573   m12 *= det1;
//00574   m21 *= det1;
//00575   m22 *= det1;
//00576   int cnum = 0;                 // number of penetrating contact points found
//00577   for (j=0; j < n; j++) {
//00578     btScalar k1 =  m22*(ret[j*2]-c1) - m12*(ret[j*2+1]-c2);
//00579     btScalar k2 = -m21*(ret[j*2]-c1) + m11*(ret[j*2+1]-c2);
//00580     for (i=0; i<3; i++) point[cnum*3+i] =
//00581                           center[i] + k1*Rb[i*4+a1] + k2*Rb[i*4+a2];
//00582     dep[cnum] = Sa[codeN] - dDOT(normal2,point+cnum*3);
//00583     if (dep[cnum] >= 0) {
//00584       ret[cnum*2] = ret[j*2];
//00585       ret[cnum*2+1] = ret[j*2+1];
//00586       cnum++;
//00587     }
//00588   }
//00589   if (cnum < 1) return 0;       // this should never happen
//00590 
//00591   // we can't generate more contacts than we actually have
//00592   if (maxc > cnum) maxc = cnum;
//00593   if (maxc < 1) maxc = 1;
//00594 
//00595   if (cnum <= maxc) {
//00596 
//00597           if (code<4) 
//00598           {
//00599     // we have less contacts than we need, so we use them all
//00600     for (j=0; j < cnum; j++) 
//00601         {
//00602                 btVector3 pointInWorld;
//00603                 for (i=0; i<3; i++) 
//00604                         pointInWorld[i] = point[j*3+i] + pa[i];
//00605                 output.addContactPoint(-normal,pointInWorld,-dep[j]);
//00606 
//00607     }
//00608           } else
//00609           {
//00610                   // we have less contacts than we need, so we use them all
//00611                 for (j=0; j < cnum; j++) 
//00612                 {
//00613                         btVector3 pointInWorld;
//00614                         for (i=0; i<3; i++) 
//00615                                 pointInWorld[i] = point[j*3+i] + pa[i]-normal[i]*dep[j];
//00616                                 //pointInWorld[i] = point[j*3+i] + pa[i];
//00617                         output.addContactPoint(-normal,pointInWorld,-dep[j]);
//00618                 }
//00619           }
//00620   }
//00621   else {
//00622     // we have more contacts than are wanted, some of them must be culled.
//00623     // find the deepest point, it is always the first contact.
//00624     int i1 = 0;
//00625     btScalar maxdepth = dep[0];
//00626     for (i=1; i<cnum; i++) {
//00627       if (dep[i] > maxdepth) {
//00628         maxdepth = dep[i];
//00629         i1 = i;
//00630       }
//00631     }
//00632 
//00633     int iret[8];
//00634     cullPoints2 (cnum,ret,maxc,i1,iret);
//00635 
//00636     for (j=0; j < maxc; j++) {
//00637 //      dContactGeom *con = CONTACT(contact,skip*j);
//00638   //    for (i=0; i<3; i++) con->pos[i] = point[iret[j]*3+i] + pa[i];
//00639     //  con->depth = dep[iret[j]];
//00640 
//00641                 btVector3 posInWorld;
//00642                 for (i=0; i<3; i++) 
//00643                         posInWorld[i] = point[iret[j]*3+i] + pa[i];
//00644                 if (code<4) 
//00645            {
//00646                         output.addContactPoint(-normal,posInWorld,-dep[iret[j]]);
//00647                 } else
//00648                 {
//00649                         output.addContactPoint(-normal,posInWorld-normal*dep[iret[j]],-dep[iret[j]]);
//00650                 }
//00651     }
//00652     cnum = maxc;
//00653   }
//00654 
//00655   *return_code = code;
//00656   return cnum;
//00657 }
//00658 
//00659 void    btBoxBoxDetector::getClosestPoints(const ClosestPointInput& input,Result& output,class btIDebugDraw* /*debugDraw*/,bool /*swapResults*/)
//00660 {
//00661         
//00662         const btTransform& transformA = input.m_transformA;
//00663         const btTransform& transformB = input.m_transformB;
//00664         
//00665         int skip = 0;
//00666         dContactGeom *contact = 0;
//00667 
//00668         dMatrix3 R1;
//00669         dMatrix3 R2;
//00670 
//00671         for (int j=0;j<3;j++)
//00672         {
//00673                 R1[0+4*j] = transformA.getBasis()[j].x();
//00674                 R2[0+4*j] = transformB.getBasis()[j].x();
//00675 
//00676                 R1[1+4*j] = transformA.getBasis()[j].y();
//00677                 R2[1+4*j] = transformB.getBasis()[j].y();
//00678 
//00679 
//00680                 R1[2+4*j] = transformA.getBasis()[j].z();
//00681                 R2[2+4*j] = transformB.getBasis()[j].z();
//00682 
//00683         }
//00684 
//00685         
//00686 
//00687         btVector3 normal;
//00688         btScalar depth;
//00689         int return_code;
//00690         int maxc = 4;
//00691 
//00692 
//00693         dBoxBox2 (transformA.getOrigin(), 
//00694         R1,
//00695         2.f*m_box1->getHalfExtentsWithMargin(),
//00696         transformB.getOrigin(),
//00697         R2, 
//00698         2.f*m_box2->getHalfExtentsWithMargin(),
//00699         normal, &depth, &return_code,
//00700         maxc, contact, skip,
//00701         output
//00702         );
//00703 
//00704 }
