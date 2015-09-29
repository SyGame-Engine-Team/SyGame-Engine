/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/ContactManifold.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Contact points and manifolds construction/updating
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
#include "ContactManifold.h"

/////////////////////////////////////////////////////////////////////////////////
// ContactPointID implementation
const ContactPointID ContactPointID::CPID_NULL;

ContactPointID::ContactPointID()
{
    dwFeatureA = 0xffffffff;
    dwSpecifierA = 0xffffffff;
    dwFeatureB = 0xffffffff;
    dwSpecifierB = 0xffffffff;
}
ContactPointID::ContactPointID( const ContactPointID & rhs )
{
    dwFeatureA = rhs.dwFeatureA;
    dwSpecifierA = rhs.dwSpecifierA;
    dwFeatureB = rhs.dwFeatureB;
    dwSpecifierB = rhs.dwSpecifierB;
}
ContactPointID::~ContactPointID()
{
    // nothing to do
}

Void ContactPointID::ReverseObjects()
{
    Swap<DWord>( &dwFeatureA, &dwFeatureB );
    Swap<DWord>( &dwSpecifierA, &dwSpecifierB );
}

/////////////////////////////////////////////////////////////////////////////////
// ContactPoint implementation
ContactPoint::ContactPoint():
    idCPID()
{
    vWorldPointA = Vertex3::Null;
    vWorldPointB = Vertex3::Null;

    vWorldNormalBA = Vector3::Null;
    fDistance = 0.0f;

    //bActive = rhs.bActive;
    iLifeTime = 0;
    fNormalImpulse = 0.0f;
    fFrictionImpulse0 = 0.0f;
    fFrictionImpulse1 = 0.0f;
    bFrictionInitialized = false;
    vFrictionAxis0 = Vector3::Null;
    vFrictionAxis1 = Vector3::Null;
}
ContactPoint::ContactPoint( const ContactPoint & rhs ):
    idCPID( rhs.idCPID )
{
    vWorldPointA = rhs.vWorldPointA;
    vWorldPointB = rhs.vWorldPointB;

    vWorldNormalBA = rhs.vWorldNormalBA;
    fDistance = rhs.fDistance;

    //bActive = rhs.bActive;
    iLifeTime = rhs.iLifeTime;
    fNormalImpulse = rhs.fNormalImpulse;
    fFrictionImpulse0 = rhs.fFrictionImpulse0;
    fFrictionImpulse1 = rhs.fFrictionImpulse1;
    bFrictionInitialized = rhs.bFrictionInitialized;
    vFrictionAxis0 = rhs.vFrictionAxis0;
    vFrictionAxis1 = rhs.vFrictionAxis1;
}
ContactPoint::~ContactPoint()
{
    // nothing to do
}

ContactPoint & ContactPoint::operator=( const ContactPoint & rhs )
{
    idCPID = rhs.idCPID;

    vWorldPointA = rhs.vWorldPointA;
    vWorldPointB = rhs.vWorldPointB;

    vWorldNormalBA = rhs.vWorldNormalBA;
    fDistance = rhs.fDistance;

    iLifeTime = rhs.iLifeTime;
    fNormalImpulse = rhs.fNormalImpulse;
    fFrictionImpulse0 = rhs.fFrictionImpulse0;
    fFrictionImpulse1 = rhs.fFrictionImpulse1;
    bFrictionInitialized = rhs.bFrictionInitialized;
    vFrictionAxis0 = rhs.vFrictionAxis0;
    vFrictionAxis1 = rhs.vFrictionAxis1;

    return (*this);
}

/////////////////////////////////////////////////////////////////////////////////
// ContactManifold implementation
ContactManifold::ContactManifold()
{
    // nothing to do
}
ContactManifold::~ContactManifold()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SinglePlaneManifold implementation
SinglePlaneManifold::SinglePlaneManifold()
{
    m_iContactPointCount = 0;
}
SinglePlaneManifold::~SinglePlaneManifold()
{
    // nothing to do
}

SinglePlaneManifold & SinglePlaneManifold::operator=( const SinglePlaneManifold & rhs )
{
    m_iContactPointCount = rhs.m_iContactPointCount;
    for( UInt i = 0; i < m_iContactPointCount; ++i )
        m_arrContactPoints[i] = rhs.m_arrContactPoints[i];
    return (*this);
}

Void SinglePlaneManifold::AddPoint( const ContactPointID & idCPID, const Vertex3 & vWorldPointA, const Vertex3 & vWorldPointB,
                                                                   const Vector3 & vWorldNormalBA, Scalar fDistance )
{
    // Remove furthest point if needed
    if ( m_iContactPointCount >= CONTACT_MANIFOLD_MAX_POINTS ) {
        UInt iMaxDistanceCP = INVALID_OFFSET;
        Scalar fMaxDistance = -SCALAR_INFINITE;
        for( UInt i = 0; i < CONTACT_MANIFOLD_MAX_POINTS; ++i ) {
            if ( m_arrContactPoints[i].fDistance > fMaxDistance ) {
                fMaxDistance = m_arrContactPoints[i].fDistance;
                iMaxDistanceCP = i;
            }
        }

        // Point to add is the furthest, discard
        if ( fDistance > fMaxDistance )
            return;

        // Remove max distance point
        if ( iMaxDistanceCP < CONTACT_MANIFOLD_MAX_POINTS - 1 )
            m_arrContactPoints[iMaxDistanceCP] = m_arrContactPoints[CONTACT_MANIFOLD_MAX_POINTS - 1];
        --m_iContactPointCount;
    }

    // Append at end
    ContactPoint * pContactPoint = ( m_arrContactPoints + m_iContactPointCount );
    ++m_iContactPointCount;

    // Ensure first point is the deepest penetrating one
    if ( m_iContactPointCount > 1 ) {
        if ( fDistance < m_arrContactPoints[0].fDistance ) {
            // First point goes to end
            *pContactPoint = m_arrContactPoints[0];
            pContactPoint = ( m_arrContactPoints );
        }
    }

    // Fill contact point data
    pContactPoint->idCPID = idCPID;

    pContactPoint->vWorldPointA = vWorldPointA;
    pContactPoint->vWorldPointB = vWorldPointB;

    pContactPoint->vWorldNormalBA = vWorldNormalBA;
    pContactPoint->fDistance = fDistance;

    pContactPoint->iLifeTime = 0;

    pContactPoint->fNormalImpulse = 0.0f;
    pContactPoint->fFrictionImpulse0 = 0.0f;
    pContactPoint->fFrictionImpulse1 = 0.0f;
    pContactPoint->bFrictionInitialized = false;
    pContactPoint->vFrictionAxis0 = Vector3::Null;
    pContactPoint->vFrictionAxis1 = Vector3::Null;
}

Void SinglePlaneManifold::UpdateAfterCollision( const ContactManifold * pBaseCollisionManifold )
{
    const SinglePlaneManifold * pCollisionManifold = (const SinglePlaneManifold *)pBaseCollisionManifold;

    UInt iNewContactPointCount = pCollisionManifold->GetContactPointCount();
    UInt i, j, iMatchedPoint;
    const ContactPoint * pCP;

    Bool arrOldUpdated[CONTACT_MANIFOLD_MAX_POINTS] = { false, false, false, false };
    Bool arrNewUpdated[CONTACT_MANIFOLD_MAX_POINTS] = { false, false, false, false };

    // Empty collision case
    if ( pCollisionManifold->IsEmpty() ) {
        Clear();
        return;
    }

    // Update matching points
    for( i = 0; i < iNewContactPointCount; ++i ) {
        pCP = pCollisionManifold->GetContactPoint( i );

        // Try to match contact point IDs
        iMatchedPoint = INVALID_OFFSET;
        for( j = 0; j < m_iContactPointCount; ++j ) {
            if ( m_arrContactPoints[j].idCPID == pCP->idCPID ) {
                iMatchedPoint = j;
                break;
            }
        }

        // Match found, update
        if ( iMatchedPoint != INVALID_OFFSET ) {
            m_arrContactPoints[iMatchedPoint].vWorldPointA = pCP->vWorldPointA;
            m_arrContactPoints[iMatchedPoint].vWorldPointB = pCP->vWorldPointB;
            m_arrContactPoints[iMatchedPoint].vWorldNormalBA = pCP->vWorldNormalBA;
            m_arrContactPoints[iMatchedPoint].fDistance = pCP->fDistance;
            ++(m_arrContactPoints[iMatchedPoint].iLifeTime);

            arrOldUpdated[iMatchedPoint] = true;
            arrNewUpdated[i] = true;
        }
    }

    // Remove obsolete points
    i = 0;
    while( i < m_iContactPointCount ) {
        if ( arrOldUpdated[i] ) {
            ++i;
            continue;
        }

        // Remove at i
        UInt iLastIndex = ( m_iContactPointCount - 1 );
        if ( i != iLastIndex ) {
            arrOldUpdated[i] = arrOldUpdated[iLastIndex];
            m_arrContactPoints[i] = m_arrContactPoints[iLastIndex];
        }
        --m_iContactPointCount;
    }

    // Add new points
    for( i = 0; i < iNewContactPointCount; ++i ) {
        if ( arrNewUpdated[i] )
            continue;
        pCP = pCollisionManifold->GetContactPoint( i );

        // Append at end
        Assert( m_iContactPointCount < CONTACT_MANIFOLD_MAX_POINTS );
        m_arrContactPoints[m_iContactPointCount] = *pCP;
        ++m_iContactPointCount;
    }

    // Ensure first point is the deepest penetrating one
    Scalar fMinDistance = SCALAR_INFINITE;
    UInt iMinPoint = INVALID_OFFSET;
    for( i = 0; i < m_iContactPointCount; ++i ) {
        pCP = ( m_arrContactPoints + i );
        if ( pCP->fDistance < fMinDistance ) {
            fMinDistance = pCP->fDistance;
            iMinPoint = i;
        }
    }
    Assert( iMinPoint != INVALID_OFFSET );
    if ( iMinPoint != 0 )
        Swap<ContactPoint>( m_arrContactPoints, m_arrContactPoints + iMinPoint );
}

/////////////////////////////////////////////////////////////////////////////////
// MultiPlaneManifold implementation
MultiPlaneManifold::MultiPlaneManifold()
{
    m_iPlaneCount = 0;
    m_iContactPointCount = 0;
}
MultiPlaneManifold::~MultiPlaneManifold()
{
    // nothing to do
}

Void MultiPlaneManifold::AddPoint( const ContactPointID & idCPID, const Vertex3 & vWorldPointA, const Vertex3 & vWorldPointB,
                                                                  const Vector3 & vWorldNormalBA, Scalar fDistance )
{
    // Extract plane ID
    QWord qwPlaneID = idCPID.dwSpecifierA;
    qwPlaneID <<= 32;
    qwPlaneID |= idCPID.dwSpecifierB;

    // Existing plane case
    for( UInt i = 0; i < m_iPlaneCount; ++i ) {
        if ( m_arrPlaneIDs[i] == qwPlaneID ) {
            SinglePlaneManifold * pPlane = ( m_arrPlanes + i );

            // Ensure first plane is the deepest penetrating one
            if ( i > 0 ) {
                if ( fDistance < m_arrPlanes[0].GetContactPoint(0)->fDistance ) {
                    // Swap planes 0 and i
                    Swap<QWord>( m_arrPlaneIDs, m_arrPlaneIDs + i );
                    Swap<SinglePlaneManifold>( m_arrPlanes, m_arrPlanes + i );
                    pPlane = ( m_arrPlanes );
                }
            }

            pPlane->AddPoint( idCPID, vWorldPointA, vWorldPointB, vWorldNormalBA, fDistance );
            ++m_iContactPointCount;
            return;
        }
    }

    // Add new plane case
    if ( m_iPlaneCount < CONTACT_MANIFOLD_MAX_PLANES ) {
        m_arrPlaneIDs[m_iPlaneCount] = qwPlaneID;
        SinglePlaneManifold * pPlane = ( m_arrPlanes + m_iPlaneCount );
        ++m_iPlaneCount;

        // Ensure first plane is the deepest penetrating one
        if ( m_iPlaneCount > 1 ) {
            if ( fDistance < m_arrPlanes[0].GetContactPoint(0)->fDistance ) {
                // First plane goes to end
                Swap<QWord>( m_arrPlaneIDs, m_arrPlaneIDs + m_iPlaneCount - 1 );
                *pPlane = m_arrPlanes[0];
                pPlane = ( m_arrPlanes );
            }
        }

        pPlane->Clear();
        pPlane->AddPoint( idCPID, vWorldPointA, vWorldPointB, vWorldNormalBA, fDistance );
        ++m_iContactPointCount;
        return;
    }

    // Replace max distance plane case
    Scalar fMaxDistance = -SCALAR_INFINITE;
    UInt iMaxPlane = INVALID_OFFSET;
    for( UInt i = 0; i < m_iPlaneCount; ++i ) {
        if ( m_arrPlanes[i].GetContactPoint(0)->fDistance > fMaxDistance ) {
            fMaxDistance = m_arrPlanes[i].GetContactPoint(0)->fDistance;
            iMaxPlane = i;
        }
    }
    Assert( iMaxPlane != INVALID_OFFSET );
    if ( fDistance < fMaxDistance ) {
        m_arrPlaneIDs[iMaxPlane] = qwPlaneID;
        SinglePlaneManifold * pPlane = ( m_arrPlanes + iMaxPlane );
        pPlane->Clear();
        m_iContactPointCount -= pPlane->GetContactPointCount();

        // Ensure first plane is the deepest penetrating one
        if ( iMaxPlane > 0 ) {
            if ( fDistance < m_arrPlanes[0].GetContactPoint(0)->fDistance ) {
                // First plane goes to iMaxPlane
                Swap<QWord>( m_arrPlaneIDs, m_arrPlaneIDs + iMaxPlane );
                *pPlane = m_arrPlanes[0];
                pPlane = ( m_arrPlanes );
                pPlane->Clear();
            }
        }

        pPlane->AddPoint( idCPID, vWorldPointA, vWorldPointB, vWorldNormalBA, fDistance );
        ++m_iContactPointCount;
    }
}

Void MultiPlaneManifold::UpdateAfterCollision( const ContactManifold * pBaseCollisionManifold )
{
    const MultiPlaneManifold * pCollisionManifold = (const MultiPlaneManifold *)pBaseCollisionManifold;

    UInt iNewPlaneCount = pCollisionManifold->m_iPlaneCount;
    UInt i, j, iMatchedPlane;

    Bool arrOldUpdated[CONTACT_MANIFOLD_MAX_PLANES] = { false, false, false, false, false, false };
    Bool arrNewUpdated[CONTACT_MANIFOLD_MAX_PLANES] = { false, false, false, false, false, false };

    // Empty collision case
    if ( pCollisionManifold->IsEmpty() ) {
        Clear();
        return;
    }

    // Update matching planes
    for( i = 0; i < iNewPlaneCount; ++i ) {
        // Try to match plane IDs
        iMatchedPlane = INVALID_OFFSET;
        for( j = 0; j < m_iPlaneCount; ++j ) {
            if ( m_arrPlaneIDs[j] == pCollisionManifold->m_arrPlaneIDs[i] ) {
                iMatchedPlane = j;
                break;
            }
        }

        // Match found, update
        if ( iMatchedPlane != INVALID_OFFSET ) {
            m_arrPlanes[iMatchedPlane].UpdateAfterCollision( pCollisionManifold->m_arrPlanes + i );

            arrOldUpdated[iMatchedPlane] = true;
            arrNewUpdated[i] = true;
        }
    }

    // Remove obsolete planes
    i = 0;
    while( i < m_iPlaneCount ) {
        if ( arrOldUpdated[i] ) {
            ++i;
            continue;
        }

        // Remove at i
        UInt iLastIndex = ( m_iPlaneCount - 1 );
        if ( i != iLastIndex ) {
            arrOldUpdated[i] = arrOldUpdated[iLastIndex];
            m_arrPlaneIDs[i] = m_arrPlaneIDs[iLastIndex];
            m_arrPlanes[i] = m_arrPlanes[iLastIndex];
        }
        --m_iPlaneCount;
    }

    // Add new planes
    for( i = 0; i < iNewPlaneCount; ++i ) {
        if ( arrNewUpdated[i] )
            continue;

        // Append at end
        Assert( m_iPlaneCount < CONTACT_MANIFOLD_MAX_PLANES );
        m_arrPlaneIDs[m_iPlaneCount] = pCollisionManifold->m_arrPlaneIDs[i];
        m_arrPlanes[m_iPlaneCount] = pCollisionManifold->m_arrPlanes[i];
        ++m_iPlaneCount;
    }

    // Update contact point count
    m_iContactPointCount = 0;
    for( i = 0; i < m_iPlaneCount; ++i )
        m_iContactPointCount += m_arrPlanes[i].GetContactPointCount();

    // Ensure first plane is the deepest penetrating one
    Scalar fMinDistance = SCALAR_INFINITE;
    UInt iMinPlane = INVALID_OFFSET;
    for( i = 0; i < m_iPlaneCount; ++i ) {
        SinglePlaneManifold * pPlane = ( m_arrPlanes + i );
        if ( pPlane->GetContactPoint(0)->fDistance < fMinDistance ) {
            fMinDistance = pPlane->GetContactPoint(0)->fDistance;
            iMinPlane = i;
        }
    }
    Assert( iMinPlane != INVALID_OFFSET );
    if ( iMinPlane != 0 )
        Swap<SinglePlaneManifold>( m_arrPlanes, m_arrPlanes + iMinPlane );
}

