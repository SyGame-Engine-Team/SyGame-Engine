/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/2D/Linear/IntersectSegment2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 2D : Segment2 and Segment2
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TIntersectSegment2 implementation
template<typename Real>
TIntersectSegment2<Real>::TIntersectSegment2( const TSegment2<Real> & rSegmentA, const TSegment2<Real> & rSegmentB,
                                              Real fZeroTolerance ):
    TIntersector2<Real>( fZeroTolerance )
{
    m_pSegmentA = &rSegmentA;
    m_pSegmentB = &rSegmentB;

    m_iContactCount = 0;
    m_arrContacts[0] = TVertex2<Real>::Null;
    m_arrContacts[1] = TVertex2<Real>::Null;

    m_fSegmentAParameter[0] = (Real)0;
    m_fSegmentAParameter[1] = (Real)0;
    m_fSegmentBParameter[0] = (Real)0;
    m_fSegmentBParameter[1] = (Real)0;
}
template<typename Real>
TIntersectSegment2<Real>::~TIntersectSegment2()
{
    // nothing to do
}

template<typename Real>
inline const TSegment2<Real> & TIntersectSegment2<Real>::GetSegmentA() const {
    return *m_pSegmentA;
}
template<typename Real>
inline const TSegment2<Real> & TIntersectSegment2<Real>::GetSegmentB() const {
    return *m_pSegmentB;
}

template<typename Real>
inline UInt TIntersectSegment2<Real>::GetContactCount() const {
    return m_iContactCount;
}
template<typename Real>
inline const TVertex2<Real> & TIntersectSegment2<Real>::GetContact( UInt iContact ) const {
    Assert( iContact < m_iContactCount );
    return m_arrContacts[iContact];
}
template<typename Real>
inline const TVertex2<Real> * TIntersectSegment2<Real>::GetContactArray() const {
    return m_arrContacts;
}

template<typename Real>
inline Real TIntersectSegment2<Real>::GetSegmentAParameter( UInt iContact ) const {
    Assert( iContact < m_iContactCount );
    return m_fSegmentAParameter[iContact];
}
template<typename Real>
inline Real TIntersectSegment2<Real>::GetSegmentBParameter( UInt iContact ) const {
    Assert( iContact < m_iContactCount );
    return m_fSegmentBParameter[iContact];
}

template<typename Real>
Bool TIntersectSegment2<Real>::Test()
{
    ////////////////////////////////
    return false;
}
template<typename Real>
Bool TIntersectSegment2<Real>::Compute()
{
    Real fNumerator, fDenominator;
    Real fS, fT;

    // Compute denominator
    TVector2<Real> vAB = ( m_pSegmentA->EndB - m_pSegmentA->EndA );
    TVector2<Real> vCD = ( m_pSegmentB->EndB - m_pSegmentB->EndA );
    fDenominator = ( vCD ^ vAB );

    // Parallel segments case
    if ( MathRealFn->EqualsZero(fDenominator, ZeroTolerance) )
        return _ParallelCompute();

    // Compute numerators and solve for (fS,fT)
    TVector2<Real> vAC = ( m_pSegmentB->EndA - m_pSegmentA->EndA );
    fNumerator = ( vCD ^ vAC );
    fS = ( fNumerator / fDenominator );
    fNumerator = ( vAB ^ vAC );
    fT = ( fNumerator / fDenominator );

    // Disjoint case
    Real fMin = -ZeroTolerance;
    Real fMax = (Real)1 + ZeroTolerance;
    if ( fS < fMin || fT < fMin || fS > fMax || fT > fMax ) {
        m_iType = INTERSECT2D_EMPTY;
        m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;
        m_iContactCount = 0;
        return false;
    }

    // X/T/L intersection case
    m_iType = INTERSECT2D_POINT;
    m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;

    fMin = ZeroTolerance;
    fMax = (Real)1 - ZeroTolerance;
    if ( fS < fMin ) {
        fS = (Real)0;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_TJUNCTION_A;
    } else if ( fS > fMax ) {
        fS = (Real)1;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_TJUNCTION_B;
    }
    if ( fT < fMin ) {
        fT = (Real)0;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_TJUNCTION_C;
    } else if ( fT > fMax ) {
        fT = (Real)1;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_TJUNCTION_D;
    }

    m_iContactCount = 1;
    m_arrContacts[0] = m_pSegmentA->GetPointAB( fS );

    m_fSegmentAParameter[0] = fS;
    m_fSegmentBParameter[0] = fT;

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Int TIntersectSegment2<Real>::_InclusionTest( const TVertex2<Real> & vA, const TVertex2<Real> & vB,
                                              const TVertex2<Real> & vIsIncluded ) const
{
    if ( MathRealFn->Equals(vA.X, vB.X, ZeroTolerance) ) {
        if ( vA.Y < vB.Y ) {
            if ( vIsIncluded.Y < (vA.Y - ZeroTolerance) )
                return -2;
            if ( vIsIncluded.Y <= (vA.Y + ZeroTolerance) )
                return -1;
            if ( vIsIncluded.Y > (vB.Y + ZeroTolerance) )
                return +2;
            if ( vIsIncluded.Y >= (vB.Y - ZeroTolerance) )
                return +1;
            return 0;
        } else {
            Assert( vA.Y > vB.Y );
            if ( vIsIncluded.Y > (vA.Y + ZeroTolerance) )
                return -2;
            if ( vIsIncluded.Y >= (vA.Y - ZeroTolerance) )
                return -1;
            if ( vIsIncluded.Y < (vB.Y - ZeroTolerance) )
                return +2;
            if ( vIsIncluded.Y <= (vB.Y + ZeroTolerance) )
                return +1;
            return 0;
        }
    } else {
        if ( vA.X < vB.X ) {
            if ( vIsIncluded.X < (vA.X - ZeroTolerance) )
                return -2;
            if ( vIsIncluded.X <= (vA.X + ZeroTolerance) )
                return -1;
            if ( vIsIncluded.X > (vB.X + ZeroTolerance) )
                return +2;
            if ( vIsIncluded.X >= (vB.X - ZeroTolerance) )
                return +1;
            return 0;
        } else {
            Assert( vA.X > vB.X );
            if ( vIsIncluded.X > (vA.X + ZeroTolerance) )
                return -2;
            if ( vIsIncluded.X >= (vA.X - ZeroTolerance) )
                return -1;
            if ( vIsIncluded.X < (vB.X - ZeroTolerance) )
                return +2;
            if ( vIsIncluded.X <= (vB.X + ZeroTolerance) )
                return +1;
            return 0;
        }
    }
}
template<typename Real>
Bool TIntersectSegment2<Real>::_ParallelCompute()
{
    const TVertex2<Real> & vA = m_pSegmentA->EndA;
    const TVertex2<Real> & vB = m_pSegmentA->EndB;
    const TVertex2<Real> & vC = m_pSegmentB->EndA;
    const TVertex2<Real> & vD = m_pSegmentB->EndB;

    // Check colinearity
    if ( m_pSegmentA->ClassifyPoint(vC) != 0 ) {
        m_iType = INTERSECT2D_EMPTY;
        m_iSpecifier = INTERSECTSEGMENT2_SPEC_PARALLEL;
        m_iContactCount = 0;
        return false;
    }

    m_iSpecifier = INTERSECTSEGMENT2_SPEC_COLINEAR;

    // Lookup points ordering along common line
    Int iInclusionC = _InclusionTest( vA, vB, vC );
    Int iInclusionD = _InclusionTest( vA, vB, vD );
    Int iInclusionSum = ( iInclusionC + iInclusionD );

    // C & D in same section
    if ( iInclusionC == iInclusionD ) {
        if ( iInclusionC == -2 ) {
            // CD -- AB disjoint
            m_iType = INTERSECT2D_EMPTY;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_DISJOINT_CDAB;
            m_iContactCount = 0;
            return false;
        }
        if ( iInclusionC == +2 ) {
            // AB -- CD disjoint
            m_iType = INTERSECT2D_EMPTY;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_DISJOINT_ABCD;
            m_iContactCount = 0;
            return false;
        }
        if ( iInclusionC == 0 ) {
            // CD in AB, strict inclusion
            m_iType = INTERSECT2D_SEGMENT;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_INCLUSION_CD;

            m_iContactCount = 2;
            m_arrContacts[0] = vC;
            m_arrContacts[1] = vD;

            m_fSegmentAParameter[0] = m_pSegmentA->GetParameterAB( vC );
            m_fSegmentBParameter[0] = (Real)0;
            m_fSegmentAParameter[1] = m_pSegmentA->GetParameterAB( vD );
            m_fSegmentBParameter[1] = (Real)1;

            return true;
        }
        // CD degenerates to A/B
        Assert( false );
        return false;
    }

    // C & D in opposite sections
    if ( iInclusionSum == 0 ) {
        if ( iInclusionC == -2 || iInclusionC == +2 ) {
            // AB in CD, strict inclusion
            m_iType = INTERSECT2D_SEGMENT;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_INCLUSION_AB;

            m_iContactCount = 2;
            m_arrContacts[0] = vA;
            m_arrContacts[1] = vB;

            m_fSegmentAParameter[0] = (Real)0;
            m_fSegmentBParameter[0] = m_pSegmentB->GetParameterAB( vA );
            m_fSegmentAParameter[1] = (Real)1;
            m_fSegmentBParameter[1] = m_pSegmentB->GetParameterAB( vB );

            return true;
        }
        if ( iInclusionC == -1 ) {
            // CD = AB
            m_iType = INTERSECT2D_SEGMENT;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_EQUALITY;

            m_iContactCount = 2;
            m_arrContacts[0] = vA;
            m_arrContacts[1] = vB;

            m_fSegmentAParameter[0] = (Real)0;
            m_fSegmentBParameter[0] = (Real)0;
            m_fSegmentAParameter[1] = (Real)1;
            m_fSegmentBParameter[1] = (Real)1;

            return true;
        }
        Assert( iInclusionC == +1 );
            // CD = BA
        m_iType = INTERSECT2D_SEGMENT;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_EQUALITY_INV;

        m_iContactCount = 2;
        m_arrContacts[0] = vA;
        m_arrContacts[1] = vB;

        m_fSegmentAParameter[0] = (Real)0;
        m_fSegmentBParameter[0] = (Real)1;
        m_fSegmentAParameter[1] = (Real)1;
        m_fSegmentBParameter[1] = (Real)0;

        return true;
    }

    // C in AB
    if ( iInclusionC == 0 ) {
        if ( iInclusionD == -2 ) {
            // DACB, strict overlap
            m_iType = INTERSECT2D_SEGMENT;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_OVERLAP_DACB;

            m_iContactCount = 2;
            m_arrContacts[0] = vA;
            m_arrContacts[1] = vC;

            m_fSegmentAParameter[0] = (Real)0;
            m_fSegmentBParameter[0] = m_pSegmentB->GetParameterAB( vA );
            m_fSegmentAParameter[1] = m_pSegmentA->GetParameterAB( vC );
            m_fSegmentBParameter[1] = (Real)0;

            return true;
        }
        if ( iInclusionD == +2 ) {
            // ACBD, strict overlap
            m_iType = INTERSECT2D_SEGMENT;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_OVERLAP_ACBD;

            m_iContactCount = 2;
            m_arrContacts[0] = vC;
            m_arrContacts[1] = vB;

            m_fSegmentAParameter[0] = m_pSegmentA->GetParameterAB( vC );
            m_fSegmentBParameter[0] = (Real)0;
            m_fSegmentAParameter[1] = (Real)1;
            m_fSegmentBParameter[1] = m_pSegmentB->GetParameterAB( vB );

            return true;
        }
        if ( iInclusionD == -1 ) {
            // C in AB, D = A, junction-inclusion
            m_iType = INTERSECT2D_SEGMENT;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_INCLUSION_CD;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_LJUNCTION_AD;

            m_iContactCount = 2;
            m_arrContacts[0] = vD;
            m_arrContacts[1] = vC;

            m_fSegmentAParameter[0] = (Real)0;
            m_fSegmentBParameter[0] = (Real)1;
            m_fSegmentAParameter[1] = m_pSegmentA->GetParameterAB( vC );
            m_fSegmentBParameter[1] = (Real)0;

            return true;
        }
        Assert( iInclusionD == +1 );
            // C in AB, D = B, junction-inclusion
        m_iType = INTERSECT2D_SEGMENT;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_INCLUSION_CD;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_LJUNCTION_BD;

        m_iContactCount = 2;
        m_arrContacts[0] = vC;
        m_arrContacts[1] = vD;

        m_fSegmentAParameter[0] = m_pSegmentA->GetParameterAB( vC );
        m_fSegmentBParameter[0] = (Real)0;
        m_fSegmentAParameter[1] = (Real)1;
        m_fSegmentBParameter[1] = (Real)1;

        return true;
    }

    // D in AB
    if ( iInclusionD == 0 ) {
        if ( iInclusionC == -2 ) {
            // CADB, strict overlap
            m_iType = INTERSECT2D_SEGMENT;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_OVERLAP_CADB;

            m_iContactCount = 2;
            m_arrContacts[0] = vA;
            m_arrContacts[1] = vD;

            m_fSegmentAParameter[0] = (Real)0;
            m_fSegmentBParameter[0] = m_pSegmentB->GetParameterAB( vA );
            m_fSegmentAParameter[1] = m_pSegmentA->GetParameterAB( vD );
            m_fSegmentBParameter[1] = (Real)1;

            return true;
        }
        if ( iInclusionC == +2 ) {
            // ADBC, strict overlap
            m_iType = INTERSECT2D_SEGMENT;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_OVERLAP_ADBC;

            m_iContactCount = 2;
            m_arrContacts[0] = vD;
            m_arrContacts[1] = vB;

            m_fSegmentAParameter[0] = m_pSegmentA->GetParameterAB( vD );
            m_fSegmentBParameter[0] = (Real)1;
            m_fSegmentAParameter[1] = (Real)1;
            m_fSegmentBParameter[1] = m_pSegmentB->GetParameterAB( vB );

            return true;
        }
        if ( iInclusionC == -1 ) {
            // D in AB, C = A, junction-inclusion
            m_iType = INTERSECT2D_SEGMENT;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_INCLUSION_CD;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_LJUNCTION_AC;

            m_iContactCount = 2;
            m_arrContacts[0] = vA;
            m_arrContacts[1] = vD;

            m_fSegmentAParameter[0] = (Real)0;
            m_fSegmentBParameter[0] = (Real)0;
            m_fSegmentAParameter[1] = m_pSegmentA->GetParameterAB( vD );
            m_fSegmentBParameter[1] = (Real)1;

            return true;
        }
        Assert( iInclusionC == +1 );
            // D in AB, C = B, junction-inclusion
        m_iType = INTERSECT2D_SEGMENT;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_INCLUSION_CD;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_LJUNCTION_BC;

        m_iContactCount = 2;
        m_arrContacts[0] = vD;
        m_arrContacts[1] = vC;

        m_fSegmentAParameter[0] = m_pSegmentA->GetParameterAB( vD );
        m_fSegmentBParameter[0] = (Real)1;
        m_fSegmentAParameter[1] = (Real)1;
        m_fSegmentBParameter[1] = (Real)0;

        return true;
    }

    // C & D on left sections
    if ( iInclusionSum == -3 ) {
        if ( iInclusionC == -2 ) {
            // C left, D = A, junction
            m_iType = INTERSECT2D_POINT;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_LJUNCTION_AD;

            m_iContactCount = 1;
            m_arrContacts[0] = vA;

            m_fSegmentAParameter[0] = (Real)0;
            m_fSegmentBParameter[0] = (Real)1;

            return true;
        }
        Assert( iInclusionC == -1 );
            // D left, C = A, junction
        m_iType = INTERSECT2D_POINT;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_LJUNCTION_AC;

        m_iContactCount = 1;
        m_arrContacts[0] = vA;

        m_fSegmentAParameter[0] = (Real)0;
        m_fSegmentBParameter[0] = (Real)0;

        return true;
    }

    // C & D on right sections
    if ( iInclusionSum == +3 ) {
        if ( iInclusionC == +2 ) {
            // C right, D = B, junction
            m_iType = INTERSECT2D_POINT;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_LJUNCTION_BD;

            m_iContactCount = 1;
            m_arrContacts[0] = vB;

            m_fSegmentAParameter[0] = (Real)1;
            m_fSegmentBParameter[0] = (Real)1;

            return true;
        }
        Assert( iInclusionC == +1 );
            // D right, C = B, junction
        m_iType = INTERSECT2D_POINT;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_LJUNCTION_BC;

        m_iContactCount = 1;
        m_arrContacts[0] = vB;

        m_fSegmentAParameter[0] = (Real)1;
        m_fSegmentBParameter[0] = (Real)0;

        return true;
    }

    // AB included, junction cases
    if ( iInclusionSum == -1 ) {
        if ( iInclusionC == -2 ) {
            // C left, D = B, junction-inclusion
            m_iType = INTERSECT2D_SEGMENT;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_INCLUSION_AB;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_LJUNCTION_BD;

            m_iContactCount = 2;
            m_arrContacts[0] = vA;
            m_arrContacts[1] = vB;

            m_fSegmentAParameter[0] = (Real)0;
            m_fSegmentBParameter[0] = m_pSegmentB->GetParameterAB( vA );
            m_fSegmentAParameter[1] = (Real)1;
            m_fSegmentBParameter[1] = (Real)1;

            return true;
        }
        Assert( iInclusionC == +1 );
            // D left, C = B, junction-inclusion
        m_iType = INTERSECT2D_SEGMENT;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_INCLUSION_AB;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_LJUNCTION_BC;

        m_iContactCount = 2;
        m_arrContacts[0] = vA;
        m_arrContacts[1] = vB;

        m_fSegmentAParameter[0] = (Real)0;
        m_fSegmentBParameter[0] = m_pSegmentB->GetParameterAB( vA );
        m_fSegmentAParameter[1] = (Real)1;
        m_fSegmentBParameter[1] = (Real)0;

        return true;
    }

    // AB included, junction cases
    if ( iInclusionSum == +1 ) {
        if ( iInclusionC == +2 ) {
            // C right, D = A, junction-inclusion
            m_iType = INTERSECT2D_SEGMENT;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_INCLUSION_AB;
            m_iSpecifier |= INTERSECTSEGMENT2_SPEC_LJUNCTION_AD;

            m_iContactCount = 2;
            m_arrContacts[0] = vA;
            m_arrContacts[1] = vB;

            m_fSegmentAParameter[0] = (Real)0;
            m_fSegmentBParameter[0] = (Real)1;
            m_fSegmentAParameter[1] = (Real)1;
            m_fSegmentBParameter[1] = m_pSegmentB->GetParameterAB( vB );

            return true;
        }
        Assert( iInclusionC == -1 );
            // D right, C = A, junction-inclusion
        m_iType = INTERSECT2D_SEGMENT;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_INCLUSION_AB;
        m_iSpecifier |= INTERSECTSEGMENT2_SPEC_LJUNCTION_AC;

        m_iContactCount = 2;
        m_arrContacts[0] = vA;
        m_arrContacts[1] = vB;

        m_fSegmentAParameter[0] = (Real)0;
        m_fSegmentBParameter[0] = (Real)0;
        m_fSegmentAParameter[1] = (Real)1;
        m_fSegmentBParameter[1] = m_pSegmentB->GetParameterAB( vB );

        return true;
    }

    // Should never reach here
    Assert( false );
    return false;
}

