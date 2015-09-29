/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldCaster.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : Casting operations
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// WorldCastResult implementation
inline Bool RayCastResult::operator==( const RayCastResult & rhs ) const {
    return ( MathFn->Equals( fDistance, rhs.fDistance ) );
}
inline Bool RayCastResult::operator!=( const RayCastResult & rhs ) const {
    return !( MathFn->Equals( fDistance, rhs.fDistance ) );
}
inline Bool RayCastResult::operator<( const RayCastResult & rhs ) const {
    return ( fDistance < rhs.fDistance );
}
inline Bool RayCastResult::operator<=( const RayCastResult & rhs ) const {
    return ( fDistance <= rhs.fDistance );
}
inline Bool RayCastResult::operator>( const RayCastResult & rhs ) const {
    return ( fDistance > rhs.fDistance );
}
inline Bool RayCastResult::operator>=( const RayCastResult & rhs ) const {
    return ( fDistance >= rhs.fDistance );
}

/////////////////////////////////////////////////////////////////////////////////
// WorldCaster implementation
inline UInt WorldCaster::GetRayCastResultCount() const {
    return m_arrRayCastResults.Count();
}
inline const RayCastResult * WorldCaster::GetRayCastResult( UInt iResult ) const {
    Assert( iResult < m_arrRayCastResults.Count() );
    return &(m_arrRayCastResults[iResult]);
}

inline UInt WorldCaster::GetSphereCastResultCount() const {
    return m_arrSphereCastResults.Count();
}
inline const SphereCastResult * WorldCaster::GetSphereCastResult( UInt iResult ) const {
    Assert( iResult < m_arrSphereCastResults.Count() );
    return &(m_arrSphereCastResults[iResult]);
}

/////////////////////////////////////////////////////////////////////////////////

inline Int WorldCaster::_ResultOrdering_RayCast( const RayCastResult & rLeft, const RayCastResult & rRight ) {
    Scalar fDistLeft = MathFn->Abs( rLeft.fDistance );
    Scalar fDistRight = MathFn->Abs( rRight.fDistance );
    if ( fDistLeft < fDistRight )
        return +1;
    if ( fDistRight < fDistLeft )
        return -1;
    return 0;
}
inline Int WorldCaster::_ResultOrdering_SphereCast( const SphereCastResult & rLeft, const SphereCastResult & rRight ) {
    Scalar fDistLeft = rLeft.fDistance;
    Scalar fDistRight = rRight.fDistance;
    if ( fDistLeft < fDistRight )
        return +1;
    if ( fDistRight < fDistLeft )
        return -1;
    return 0;
}

