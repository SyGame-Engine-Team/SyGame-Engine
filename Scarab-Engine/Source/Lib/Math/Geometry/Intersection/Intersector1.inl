/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/Intersector1.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection of intervals in 1D
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TIntersector1 implementation
template<typename Real>
TIntersector1<Real>::TIntersector1( Real fU0, Real fU1, Real fV0, Real fV1, Real fZeroTolerance ):
    m_intU( fU0, fU1 ), m_intV( fV0, fV1 )
{
    Assert( (fU0 <= fU1) && (fV0 <= fV1) );

    m_fFirstTOI = (Real)0;
    m_fLastTOI = (Real)0;

    m_iContactCount = 0;

    ZeroTolerance = fZeroTolerance;
}
template<typename Real>
TIntersector1<Real>::TIntersector1( const Real vU[2], const Real vV[2], Real fZeroTolerance ):
    m_intU( vU ), m_intV( vV )
{
    Assert( (vU[0] <= vU[1]) && (vV[0] <= vV[1]) );

    m_fFirstTOI = (Real)0;
    m_fLastTOI = (Real)0;

    m_iContactCount = 0;

    ZeroTolerance = fZeroTolerance;
}
template<typename Real>
TIntersector1<Real>::TIntersector1( const TVector2<Real> & vU, const TVector2<Real> & vV, Real fZeroTolerance ):
    m_intU( vU ), m_intV( vV )
{
    Assert( (vU[0] <= vU[1]) && (vV[0] <= vV[1]) );

    m_fFirstTOI = (Real)0;
    m_fLastTOI = (Real)0;

    m_iContactCount = 0;

    ZeroTolerance = fZeroTolerance;
}
template<typename Real>
TIntersector1<Real>::~TIntersector1()
{
    // nothing to do
}

template<typename Real>
inline const TVector2<Real> & TIntersector1<Real>::GetU() const {
    return m_intU;
}
template<typename Real>
inline const TVector2<Real> & TIntersector1<Real>::GetV() const {
    return m_intV;
}

template<typename Real>
inline UInt TIntersector1<Real>::GetContactCount() const {
    return m_iContactCount;
}
template<typename Real>
inline Real TIntersector1<Real>::GetContact( UInt iContact ) const {
    Assert( iContact < m_iContactCount );
    return m_arrContacts[iContact];
}

template<typename Real>
inline Real TIntersector1<Real>::GetFirstTOI() const {
    return m_fFirstTOI;
}
template<typename Real>
inline Real TIntersector1<Real>::GetLastTOI() const {
    return m_fLastTOI;
}

template<typename Real>
Bool TIntersector1<Real>::Test()
{
    return ( (m_intU[1] >= m_intV[0]) && (m_intU[0] <= m_intV[1]) );
}
template<typename Real>
Bool TIntersector1<Real>::Compute()
{
    if ( (m_intU[1] < m_intV[0]) || (m_intU[0] > m_intV[1]) )
        m_iContactCount = 0;
    else if ( m_intU[1] > m_intV[0] ) {
        if ( m_intU[0] < m_intV[1] ) {
            m_iContactCount = 2;
            m_arrContacts[0] = Max<Real>( m_intU[0], m_intV[0] );
            m_arrContacts[1] = Min<Real>( m_intU[1], m_intV[1] );
            if ( MathRealFn->Abs(m_arrContacts[0] - m_arrContacts[1]) < ZeroTolerance )
                m_iContactCount = 1;
        } else { // m_intU[0] == m_intV[1]
            m_iContactCount = 1;
            m_arrContacts[0] = m_intU[0];
        }
    } else { // m_intU[1] == m_intV[0]
        m_iContactCount = 1;
        m_arrContacts[0] = m_intU[1];
    }
    return (m_iContactCount > 0);    
}

template<typename Real>
Bool TIntersector1<Real>::Test( Real fSpeedU, Real fSpeedV, Real fMaxTime )
{
    Real fRelSpeed, fInvRelSpeed;
    Real fDeltaPos;

    if ( m_intU[1] < m_intV[0] ) {
        // U is left of V
        fRelSpeed = ( fSpeedU - fSpeedV );
        if ( fRelSpeed > (Real)0 ) {
            // Intervals will collide in the future
            fDeltaPos = ( m_intV[0] - m_intU[1] );
            if ( fDeltaPos <= (fMaxTime * fRelSpeed) ) {
                // Intervals collide within specified time
                fInvRelSpeed = MathRealFn->Invert( fRelSpeed );
                m_fFirstTOI = ( fDeltaPos * fInvRelSpeed );
                m_fLastTOI = ( (m_intV[1] - m_intU[0]) * fInvRelSpeed );
                return true;
            }
        }
    } else if ( m_intU[0] > m_intV[1] ) {
        // U is right of V
        fRelSpeed = ( fSpeedV - fSpeedU );
        if ( fRelSpeed > (Real)0 ) {
            // Intervals will collide in the future
            fDeltaPos = ( m_intU[0] - m_intV[1] );
            if ( fDeltaPos <= (fMaxTime * fRelSpeed) ) {
                // Intervals collide within specified time
                fInvRelSpeed = MathRealFn->Invert( fRelSpeed );
                m_fFirstTOI = ( fDeltaPos * fInvRelSpeed );
                m_fLastTOI = ( (m_intU[1] - m_intV[0]) * fInvRelSpeed );
                return true;
            }
        }
    } else {
        // U and V initially intersecting
        m_fFirstTOI = (Real)0;
        if ( fSpeedU > fSpeedV )
            m_fLastTOI = ( (m_intV[1] - m_intU[0]) / (fSpeedU - fSpeedV) );
        else if ( fSpeedU < fSpeedV )
            m_fLastTOI = ( (m_intU[1] - m_intV[0]) / (fSpeedV - fSpeedU) );
        else // Intersects forever
            m_fLastTOI = MathRealFn->Infinity;
        return true;
    }

    return false;
}
template<typename Real>
Bool TIntersector1<Real>::Compute( Real fSpeedU, Real fSpeedV, Real fMaxTime )
{
    Real fRelSpeed, fInvRelSpeed;
    Real fDeltaPos;

    if ( m_intU[1] < m_intV[0] ) {
        // U is left of V
        fRelSpeed = ( fSpeedU - fSpeedV );
        if ( fRelSpeed > (Real)0 ) {
            // Intervals will collide in the future
            fDeltaPos = ( m_intV[0] - m_intU[1] );
            if ( fDeltaPos <= (fMaxTime * fRelSpeed) ) {
                // Intervals collide within specified time
                fInvRelSpeed = MathRealFn->Invert( fRelSpeed );
                m_fFirstTOI = ( fDeltaPos * fInvRelSpeed );
                m_fLastTOI = ( (m_intV[1] - m_intU[0]) * fInvRelSpeed );
                m_iContactCount = 1;
                m_arrContacts[0] = m_intU[0] + (m_fFirstTOI * fSpeedU);
                return true;
            }
        }
    } else if ( m_intU[0] > m_intV[1] ) {
        // U is right of V
        fRelSpeed = ( fSpeedV - fSpeedU );
        if ( fRelSpeed > (Real)0 ) {
            // Intervals will collide in the future
            fDeltaPos = ( m_intU[0] - m_intV[1] );
            if ( fDeltaPos <= (fMaxTime * fRelSpeed) ) {
                // Intervals collide within specified time
                fInvRelSpeed = MathRealFn->Invert( fRelSpeed );
                m_fFirstTOI = ( fDeltaPos * fInvRelSpeed );
                m_fLastTOI = ( (m_intU[1] - m_intV[0]) * fInvRelSpeed );
                m_iContactCount = 1;
                m_arrContacts[0] = m_intV[1] + (m_fFirstTOI * fSpeedV);
                return true;
            }
        }
    } else {
        // U and V initially intersecting
        m_fFirstTOI = (Real)0;
        if ( fSpeedU > fSpeedV )
            m_fLastTOI = ( (m_intV[1] - m_intU[0]) / (fSpeedU - fSpeedV) );
        else if ( fSpeedU < fSpeedV )
            m_fLastTOI = ( (m_intU[1] - m_intV[0]) / (fSpeedV - fSpeedU) );
        else // Intersects forever
            m_fLastTOI = MathRealFn->Infinity;

        if ( m_intU[1] > m_intV[0] ) {
            if ( m_intU[0] < m_intV[1] ) {
                m_iContactCount = 2;
                m_arrContacts[0] = Max<Real>( m_intU[0], m_intV[0] );
                m_arrContacts[1] = Min<Real>( m_intU[1], m_intV[1] );
            } else { // m_intU[0] == m_intV[1]
                m_iContactCount = 1;
                m_arrContacts[0] = m_intU[0];
            }
        } else { // m_intU[1] == m_intV[0]
            m_iContactCount = 1;
            m_arrContacts[0] = m_intU[1];
        }

        return true;
    }

    m_iContactCount = 0;
    return false;
}
