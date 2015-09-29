/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/ContactManifold.inl
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
// ContactPointID implementation
inline ContactPointID & ContactPointID::operator=( const ContactPointID & rhs ) {
    dwFeatureA = rhs.dwFeatureA;
    dwSpecifierA = rhs.dwSpecifierA;
    dwFeatureB = rhs.dwFeatureB;
    dwSpecifierB = rhs.dwSpecifierB;
    return (*this);
}

inline Bool ContactPointID::operator==( const ContactPointID & rhs ) const {
    return ( dwFeatureA == rhs.dwFeatureA && dwSpecifierA == rhs.dwSpecifierA &&
             dwFeatureB == rhs.dwFeatureB && dwSpecifierB == rhs.dwSpecifierB );
}
inline Bool ContactPointID::operator!=( const ContactPointID & rhs ) const {
    return ( dwFeatureA != rhs.dwFeatureA || dwSpecifierA != rhs.dwSpecifierA ||
             dwFeatureB != rhs.dwFeatureB || dwSpecifierB != rhs.dwSpecifierB );
}

/////////////////////////////////////////////////////////////////////////////////
// ContactPoint implementation

/////////////////////////////////////////////////////////////////////////////////
// ContactManifold implementation

/////////////////////////////////////////////////////////////////////////////////
// SinglePlaneManifold implementation
inline Bool SinglePlaneManifold::IsEmpty() const {
    return ( m_iContactPointCount == 0 );
}

inline UInt SinglePlaneManifold::GetContactPointCount() const {
    return m_iContactPointCount;
}

inline const ContactPoint * SinglePlaneManifold::GetContactPoint( UInt iIndex ) const {
    Assert( iIndex < m_iContactPointCount );
    return ( m_arrContactPoints + iIndex );
}
inline ContactPoint * SinglePlaneManifold::GetContactPoint( UInt iIndex ) {
    Assert( iIndex < m_iContactPointCount );
    return ( m_arrContactPoints + iIndex );
}

inline Void SinglePlaneManifold::Clear() {
    m_iContactPointCount = 0;
}

/////////////////////////////////////////////////////////////////////////////////
// MultiPlaneManifold implementation
inline Bool MultiPlaneManifold::IsEmpty() const {
    return ( m_iContactPointCount == 0 );
}

inline UInt MultiPlaneManifold::GetContactPointCount() const {
    return m_iContactPointCount;
}

inline const ContactPoint * MultiPlaneManifold::GetContactPoint( UInt iIndex ) const {
    Assert( iIndex < m_iContactPointCount );
    UInt iPlane = 0;
    while( iIndex >= m_arrPlanes[iPlane].GetContactPointCount() ) {
        iIndex -= m_arrPlanes[iPlane].GetContactPointCount();
        ++iPlane;
    }
    return m_arrPlanes[iPlane].GetContactPoint( iIndex );
}
inline ContactPoint * MultiPlaneManifold::GetContactPoint( UInt iIndex ) {
    Assert( iIndex < m_iContactPointCount );
    UInt iPlane = 0;
    while( iIndex >= m_arrPlanes[iPlane].GetContactPointCount() ) {
        iIndex -= m_arrPlanes[iPlane].GetContactPointCount();
        ++iPlane;
    }
    return m_arrPlanes[iPlane].GetContactPoint( iIndex );
}

inline Void MultiPlaneManifold::Clear() {
    for( UInt i = 0; i < m_iPlaneCount; ++i )
        m_arrPlanes[i].Clear();
    m_iPlaneCount = 0;
    m_iContactPointCount = 0;
}


