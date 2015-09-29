/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/2D/IntersectConvexPolygon2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 2D : ConvexPolygon2 and ConvexPolygon2
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TIntersectConvexPolygon2 implementation
template<typename Real>
TIntersectConvexPolygon2<Real>::TIntersectConvexPolygon2( const TConvexPolygon2<Real> & rPolygonA, const TConvexPolygon2<Real> & rPolygonB,
                                                          Real fZeroTolerance ):
    TIntersector2<Real>( fZeroTolerance )
{
    m_pPolygonA = &rPolygonA;
    m_pPolygonB = &rPolygonB;

    m_iContactCount = 0;
}
template<typename Real>
TIntersectConvexPolygon2<Real>::~TIntersectConvexPolygon2()
{
    // nothing to do
}

template<typename Real>
inline const TConvexPolygon2<Real> & TIntersectConvexPolygon2<Real>::GetPolygonA() const {
    return *m_pPolygonA;
}
template<typename Real>
inline const TConvexPolygon2<Real> & TIntersectConvexPolygon2<Real>::GetPolygonB() const {
    return *m_pPolygonB;
}

template<typename Real>
inline UInt TIntersectConvexPolygon2<Real>::GetContactCount() const {
    return m_iContactCount;
}
template<typename Real>
inline const TVertex2<Real> & TIntersectConvexPolygon2<Real>::GetContact( UInt iContact ) const {
    Assert( iContact < m_iContactCount );
    return m_arrContacts[iContact];
}
template<typename Real>
inline const TVertex2<Real> * TIntersectConvexPolygon2<Real>::GetContactArray() const {
    return m_arrContacts;
}

template<typename Real>
inline Intersect2Specifier TIntersectConvexPolygon2<Real>::GetContactSpecifier( UInt iContact ) const {
    Assert( iContact < m_iContactCount );
    return m_arrContactsData[iContact].iSpecifier;
}
template<typename Real>
inline UInt TIntersectConvexPolygon2<Real>::GetContactIndexA( UInt iContact ) const {
    Assert( iContact < m_iContactCount );
    return m_arrContactsData[iContact].iIndexA;
}
template<typename Real>
inline UInt TIntersectConvexPolygon2<Real>::GetContactIndexB( UInt iContact ) const {
    Assert( iContact < m_iContactCount );
    return m_arrContactsData[iContact].iIndexB;
}
template<typename Real>
inline Real TIntersectConvexPolygon2<Real>::GetContactParameterA( UInt iContact ) const {
    Assert( iContact < m_iContactCount );
    return m_arrContactsData[iContact].fParameterA;
}
template<typename Real>
inline Real TIntersectConvexPolygon2<Real>::GetContactParameterB( UInt iContact ) const {
    Assert( iContact < m_iContactCount );
    return m_arrContactsData[iContact].fParameterB;
}

template<typename Real>
Bool TIntersectConvexPolygon2<Real>::Test()
{
    //////////////////////////
    return false;
}
template<typename Real>
Bool TIntersectConvexPolygon2<Real>::Compute()
{
    // Create helper macros
#define _EDGESTEP_A { \
    ++iCount; \
    if ( iCount >= iMaxCount ) break; \
    iA = iB; iB = ( iB + 1 ) % iSizeA; \
    m_pPolygonA->GetEdge( &vSegA, iA ); \
    vAB = ( vB - vA ); \
}
#define _EDGESTEP_B { \
    ++iCount; \
    if ( iCount >= iMaxCount ) break; \
    iC = iD; iD = ( iD + 1 ) % iSizeB; \
    m_pPolygonB->GetEdge( &vSegB, iC ); \
    vCD = ( vD - vC ); \
}
#define _EDGESTEP_ADD_A( _test, _specB, _indexB, _paramB ) { \
    if ( _test ) \
        _AddContact( INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A | (_specB), iB, _indexB, vB, (Real)0, _paramB ); \
    _EDGESTEP_A; \
}
#define _EDGESTEP_ADD_B( _test, _specA, _indexA, _paramA ) { \
    if ( _test ) \
        _AddContact( INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B | (_specA), _indexA, iD, vD, _paramA, (Real)0 ); \
    _EDGESTEP_B; \
}
#define _TUNNEL_LOOP( _spec ) { \
    iSegType = INTERSECT2D_POINT; \
    iSegSpec = (_spec); \
    bTunneling = true; \
}

    // The O'Rourke-Preparata-Laszlo algorithm,
    // will full support for special cases and ...
    // -> Loop-bypass by tunneling junction cases,
    //    avoiding useless segment intersections
    // -> Early-out for disjoint case
    const UInt iSizeA = m_pPolygonA->GetVertexCount();
    const UInt iSizeB = m_pPolygonB->GetVertexCount();
    const UInt iSizeSum = ( iSizeA + iSizeB );

    _SickleState iSickleState = _SICKLE_UNKNOWN;
    Bool bLookupDone = false;
    Bool bTunneling = false;
    Bool bExclusionFlagA = false;
    Bool bExclusionFlagB = false;
    UInt iCount = 0;
    UInt iMaxCount = ( iSizeSum << 1 );

    UInt iA = (iSizeA - 1), iB = 0;
    UInt iC = (iSizeB - 1), iD = 0;

    TSegment2<Real> vSegA, vSegB;
    const TVertex2<Real> & vA = vSegA.EndA;
    const TVertex2<Real> & vB = vSegA.EndB;
    const TVertex2<Real> & vC = vSegB.EndA;
    const TVertex2<Real> & vD = vSegB.EndB;
    TVector2<Real> vAB, vCD;

    Int iDot, iCross;
    Int iAHB, iBHA;

    TIntersectSegment2<Real> intersectSegSeg( vSegA, vSegB, ZeroTolerance );
    Bool bSegIntersection = false;
    Intersect2Type iSegType = INTERSECT2D_EMPTY;
    Intersect2Specifier iSegSpec = INTERSECT2D_SPECIFIER_NONE;

    // Main loop
    m_pPolygonA->GetEdge( &vSegA, iA );
    m_pPolygonB->GetEdge( &vSegB, iC );
    vAB = ( vB - vA );
    vCD = ( vD - vC );

    m_iSpecifier = INTERSECTCONVEXPOLYGON2_SPEC_EQUALITY;

    while( true ) {
        // Compute segment intersection
        if ( !bTunneling ) {
            bSegIntersection = intersectSegSeg.Compute();
            iSegType = intersectSegSeg.GetType();
            iSegSpec = intersectSegSeg.GetSpecifier();
        } else
            bTunneling = false;

        // Lookup / Sickle phase
        if ( !bSegIntersection ) {
            // Compute classification values
            iDot = MathRealFn->SignI( (vAB * vCD), ZeroTolerance );
            iCross = MathRealFn->SignI( (vAB ^ vCD), ZeroTolerance );
            iAHB = vSegB.ClassifyPoint( vB, ZeroTolerance );
            iBHA = vSegA.ClassifyPoint( vD, ZeroTolerance );

            // Inclusion state update
            if ( m_iSpecifier != INTERSECT2D_SPECIFIER_NONE ) {
                if ( iAHB <= 0 ) _Exclude_A();
                if ( iBHA <= 0 ) _Exclude_B();
            }

            // Disjoint case, early out
            if ( !bLookupDone ) {
                if ( iAHB <= 0 ) {
                    if ( bExclusionFlagA ) {
                        if ( iCross > 0 ) {
                            m_iType = INTERSECT2D_EMPTY;
                            m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;
                            m_iContactCount = 0;
                            return false;
                        }
                    } else if ( iCross < 0 )
                        bExclusionFlagA = true;
                } else
                    bExclusionFlagA = false;
                if ( iBHA <= 0 ) {
                    if ( bExclusionFlagB ) {
                        if ( iCross < 0 ) {
                            m_iType = INTERSECT2D_EMPTY;
                            m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;
                            m_iContactCount = 0;
                            return false;
                        }
                    } else if ( iCross > 0 )
                        bExclusionFlagB = true;
                } else
                    bExclusionFlagB = false;
            }

            // Parallel cases
            if ( INTERSECTSEGMENT2_SPEC_ISPARALLEL(iSegSpec) ) {
                Assert( iCross == 0 && iAHB != 0 && iBHA != 0 );
                if ( iBHA < 0 ) {
                    if ( iAHB < 0 ) {
                        // Disjoint case
                        m_iType = INTERSECT2D_EMPTY;
                        m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;
                        m_iContactCount = 0;
                        return false;
                    } else {
                        _EDGESTEP_ADD_B( iSickleState == _SICKLE_INNER_B, 0, INVALID_OFFSET, (Real)0 );
                    }
                } else {
                    if ( iAHB < 0 ) {
                        _EDGESTEP_ADD_A( iSickleState == _SICKLE_INNER_A, 0, INVALID_OFFSET, (Real)0 );
                    } else {
                        if ( iSickleState == _SICKLE_INNER_A ) {
                            _EDGESTEP_B;
                        } else if ( iSickleState == _SICKLE_INNER_B ) {
                            _EDGESTEP_A;
                        } else { // Doesn't matter
                            _EDGESTEP_A;
                            // _EDGESTEP_B;
                        }
                    }
                }
                continue;
            }

            // Colinear cases
            if ( INTERSECTSEGMENT2_SPEC_ISCOLINEAR(iSegSpec) ) {
                Assert( iCross == 0 && iAHB == 0 && iBHA == 0 );
                Assert( iDot != 0 && INTERSECTSEGMENT2_SPEC_ISDISJOINT(iSegSpec) );
                if ( iDot < 0 ) {
                    // Disjoint case
                    m_iType = INTERSECT2D_EMPTY;
                    m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;
                    m_iContactCount = 0;
                    return false;
                }
                if ( iSickleState == _SICKLE_INNER_A ) {
                    _EDGESTEP_B;
                } else if ( iSickleState == _SICKLE_INNER_B ) {
                    _EDGESTEP_A;
                } else {
                    if ( iSegSpec & INTERSECTSEGMENT2_SPEC_DISJOINT_ABCD ) {
                        _EDGESTEP_A;
                    } else {
                        Assert( iSegSpec & INTERSECTSEGMENT2_SPEC_DISJOINT_CDAB );
                        _EDGESTEP_B;
                    }
                }
                continue;
            }

            // Generic cases (no intersection)
            Assert( iCross != 0 );
            if ( iCross < 0 ) {
                if ( iAHB <= 0 ) {
                    _EDGESTEP_ADD_A( iSickleState == _SICKLE_INNER_A, 0, INVALID_OFFSET, (Real)0 );
                } else {
                    _EDGESTEP_ADD_B( iSickleState == _SICKLE_INNER_B, 0, INVALID_OFFSET, (Real)0 );
                }
            } else {
                if ( iBHA <= 0 ) {
                    _EDGESTEP_ADD_B( iSickleState == _SICKLE_INNER_B, 0, INVALID_OFFSET, (Real)0 );
                } else {
                    _EDGESTEP_ADD_A( iSickleState == _SICKLE_INNER_A, 0, INVALID_OFFSET, (Real)0 );
                }
            }
            continue;
        }

        // Intersecting phase
        Assert( bSegIntersection );

        // Compute classification values
        iDot = MathRealFn->SignI( (vAB * vCD), ZeroTolerance );
        iCross = MathRealFn->SignI( (vAB ^ vCD), ZeroTolerance );

        // Switch to sickle phase
        if ( !bLookupDone ) {
            iCount = 0;
            iMaxCount = iSizeSum;

            m_iType = INTERSECT2D_POLYGON;
            m_iContactCount = 0;

            bLookupDone = true;
        }

        // Colinear cases
        if ( INTERSECTSEGMENT2_SPEC_ISCOLINEAR(iSegSpec) ) {
            Assert( iCross == 0 && iDot != 0 );

            // Boundary cases
            if ( iDot < 0 ) {
                m_iType = iSegType;
                m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;

                m_iContactCount = 0;
                if ( m_iType == INTERSECT2D_SEGMENT ) {
                    Intersect2Specifier iContactSpec0, iContactSpec1;
                    UInt iIndexA, iIndexB;
                    Real fParamA, fParamB;

                    _ConvertSpecifier( &iContactSpec0, &iContactSpec1, iSegSpec );
                    
                    iIndexA = iA;
                    iIndexB = (iContactSpec0 & INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B) ? iC : iD;
                    fParamA = (iContactSpec0 & INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A) ? intersectSegSeg.GetSegmentAParameter(0) : (Real)0;
                    fParamB = (iContactSpec0 & INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B) ? intersectSegSeg.GetSegmentBParameter(0) : (Real)0;
                    _AddContact( iContactSpec0, iIndexA, iIndexB,
                                 intersectSegSeg.GetContact(0), fParamA, fParamB );

                    iIndexA = (iContactSpec1 & INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A) ? iA : iB;
                    iIndexB = iC;
                    fParamA = (iContactSpec1 & INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A) ? intersectSegSeg.GetSegmentAParameter(1) : (Real)0;
                    fParamB = (iContactSpec1 & INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B) ? intersectSegSeg.GetSegmentBParameter(1) : (Real)0;
                    _AddContact( iContactSpec1, iIndexA, iIndexB,
                                 intersectSegSeg.GetContact(1), fParamA, fParamB );
                } else {
                    Assert( m_iType == INTERSECT2D_POINT );
                    if ( iSegSpec & INTERSECTSEGMENT2_SPEC_LJUNCTION_AC ) {
                        _AddContact( INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A | INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B,
                                     iA, iC, vA, (Real)0, (Real)0 );
                    } else {
                        Assert( iSegSpec & INTERSECTSEGMENT2_SPEC_LJUNCTION_BD );
                        _AddContact( INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A | INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B,
                                     iB, iD, vB, (Real)0, (Real)0 );
                    }
                }
                return true;
            }

            // Equality cases
            if ( INTERSECTSEGMENT2_SPEC_ISEQUALITY(iSegSpec) ) {
                Assert( iSegSpec & INTERSECTSEGMENT2_SPEC_EQUALITY );
                if ( iSickleState == _SICKLE_INNER_A ) {
                    _Exclude_B();
                    _EDGESTEP_B;
                    _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_LJUNCTION_BC );
                } else if ( iSickleState == _SICKLE_INNER_B ) {
                    _Exclude_A();
                    _EDGESTEP_A;
                    _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_LJUNCTION_AD );
                } else { // Doesn't matter
                    _EDGESTEP_A;
                    _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_LJUNCTION_AD );
                    //_EDGESTEP_B;
                    //_TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_LJUNCTION_BC );
                }
                continue;
            }

            // Overlap cases
            if ( INTERSECTSEGMENT2_SPEC_ISOVERLAP(iSegSpec) ) {
                _Exclude_Both();
                if ( iSegSpec & INTERSECTSEGMENT2_SPEC_OVERLAP_CADB ) {
                    // Overlap around A case
                    Assert( iSickleState != _SICKLE_INNER_B );
                    iSickleState = _SICKLE_INNER_B;
                    _EDGESTEP_ADD_B( true, INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A, iA, intersectSegSeg.GetSegmentAParameter(1) );
                    _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_TJUNCTION_C );
                } else {
                    Assert( iSegSpec & INTERSECTSEGMENT2_SPEC_OVERLAP_ACBD );
                    // Overlap around B case
                    Assert( iSickleState != _SICKLE_INNER_A );
                    iSickleState = _SICKLE_INNER_A;
                    _EDGESTEP_ADD_A( true, INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B, iC, intersectSegSeg.GetSegmentBParameter(1) );
                    _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_TJUNCTION_A );
                }
                continue;
            }

            // Inclusion cases
            if ( INTERSECTSEGMENT2_SPEC_ISINCLUSION(iSegSpec) ) {
                // L-junction BD cases
                if ( iSegSpec & INTERSECTSEGMENT2_SPEC_LJUNCTION_BD ) {
                    if ( iSegSpec & INTERSECTSEGMENT2_SPEC_INCLUSION_AB ) {
                        // Inclusion of A
                        _Exclude_B();
                        Assert( iSickleState != _SICKLE_INNER_B );
                        iSickleState = _SICKLE_INNER_A;
                        _EDGESTEP_B;
                        _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_LJUNCTION_BC );
                    } else {
                        Assert( iSegSpec & INTERSECTSEGMENT2_SPEC_INCLUSION_CD );
                        // Inclusion of B
                        _Exclude_A();
                        Assert( iSickleState != _SICKLE_INNER_A );
                        iSickleState = _SICKLE_INNER_B;
                        _EDGESTEP_A;
                        _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_LJUNCTION_AD );
                    }
                    continue;
                }

                // L-junction AC cases
                if ( iSegSpec & INTERSECTSEGMENT2_SPEC_LJUNCTION_AC ) {
                    if ( iSegSpec & INTERSECTSEGMENT2_SPEC_INCLUSION_AB ) {
                        // Inclusion of A
                        if ( iSickleState == _SICKLE_INNER_B )
                            _Exclude_Both();
                        else
                            _Exclude_B();
                        _EDGESTEP_ADD_A( true, INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B, iC, intersectSegSeg.GetSegmentBParameter(1) );
                        _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_TJUNCTION_A );
                    } else {
                        Assert( iSegSpec & INTERSECTSEGMENT2_SPEC_INCLUSION_CD );
                        // Inclusion of B
                        if ( iSickleState == _SICKLE_INNER_A )
                            _Exclude_Both();
                        else
                            _Exclude_A();
                        _EDGESTEP_ADD_B( true, INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A, iA, intersectSegSeg.GetSegmentAParameter(1) );
                        _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_TJUNCTION_C );
                    }
                    continue;
                } 

                // Strict inclusion cases
                if ( iSegSpec & INTERSECTSEGMENT2_SPEC_INCLUSION_AB ) {
                    // Inclusion of A
                    _Exclude_B();
                    Assert( iSickleState != _SICKLE_INNER_B );
                    iSickleState = _SICKLE_INNER_A;
                    _EDGESTEP_ADD_A( true, INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B, iC, intersectSegSeg.GetSegmentBParameter(1) );
                    _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_TJUNCTION_A );
                } else {
                    Assert( iSegSpec & INTERSECTSEGMENT2_SPEC_INCLUSION_CD );
                    // Inclusion of B
                    _Exclude_A();
                    Assert( iSickleState != _SICKLE_INNER_A );
                    iSickleState = _SICKLE_INNER_B;
                    _EDGESTEP_ADD_B( true, INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A, iA, intersectSegSeg.GetSegmentAParameter(1) );
                    _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_TJUNCTION_C );
                }
                continue;
            }

            // Adjacent cases
            _Exclude_Both();
            if ( iSegSpec & INTERSECTSEGMENT2_SPEC_LJUNCTION_BC ) {
                // L-junction BC
                Assert( iSickleState != _SICKLE_INNER_A );
                iSickleState = _SICKLE_INNER_B;
                _EDGESTEP_ADD_A( true, INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B, iC, (Real)0 );
                _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_LJUNCTION_AC );
            } else {
                Assert( iSegSpec & INTERSECTSEGMENT2_SPEC_LJUNCTION_AD );
                // L-junction AD
                Assert( iSickleState != _SICKLE_INNER_B );
                iSickleState = _SICKLE_INNER_A;
                _EDGESTEP_ADD_B( true, INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A, iA, (Real)0 );
                _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_LJUNCTION_AC );
            }
            continue;
        }

        // Non-colinear cases
        Assert( iCross != 0 );
        Assert( iSegType == INTERSECT2D_POINT );

        // L/T-junction cases
        if ( INTERSECTSEGMENT2_SPEC_ISJUNCTION(iSegSpec) ) {
            // L-junction BD cases
            if ( iSegSpec == INTERSECTSEGMENT2_SPEC_LJUNCTION_BD ) {
                if ( iCross < 0 ) {
                    Assert( iSickleState != _SICKLE_INNER_A );
                    iSickleState = _SICKLE_INNER_B;
                    _EDGESTEP_A;
                    iCross = MathRealFn->SignI( (vAB ^ vCD), ZeroTolerance );
                    if ( iCross > 0 ) {
                        // Boundary case
                        m_iType = INTERSECT2D_POINT;
                        m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;
                        m_iContactCount = 0;
                        _AddContact( INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A | INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B,
                                     iA, iD, vD, (Real)0, (Real)0 );
                        return true;
                    }
                    if ( iCross < 0 ) {
                        _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_LJUNCTION_AD );
                    }
                } else {
                    Assert( iSickleState != _SICKLE_INNER_B );
                    iSickleState = _SICKLE_INNER_A;
                    _EDGESTEP_B;
                    iCross = MathRealFn->SignI( (vAB ^ vCD), ZeroTolerance );
                    if ( iCross < 0 ) {
                        // Boundary case
                        m_iType = INTERSECT2D_POINT;
                        m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;
                        m_iContactCount = 0;
                        _AddContact( INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A | INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B,
                                     iB, iC, vB, (Real)0, (Real)0 );
                        return true;
                    }
                    if ( iCross > 0 ) {
                        _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_LJUNCTION_BC );
                    }
                }
                continue;
            }

            // L-junction BC cases
            if ( iSegSpec == INTERSECTSEGMENT2_SPEC_LJUNCTION_BC ) {
                if ( iCross < 0 ) {
                    Assert( iSickleState != _SICKLE_INNER_A );
                    iSickleState = _SICKLE_INNER_B;
                    _EDGESTEP_ADD_A( true, INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B, iC, (Real)0 );
                    iCross = MathRealFn->SignI( (vAB ^ vCD), ZeroTolerance );
                    if ( iCross >= 0 ) {
                        // Boundary case
                        m_iType = INTERSECT2D_POINT;
                        m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;
                        m_iContactCount = 0;
                        _AddContact( INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A | INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B,
                                     iA, iC, vC, (Real)0, (Real)0 );
                        return true;
                    }
                    _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_LJUNCTION_AC );
                } else {
                    _EDGESTEP_ADD_A( true, INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B, iC, (Real)0 );
                    iCross = MathRealFn->SignI( (vAB ^ vCD), ZeroTolerance );
                    if ( iCross != 0 ) {
                        _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_LJUNCTION_AC );
                    }
                }
                continue;
            }

            // L-junction AD cases
            if ( iSegSpec == INTERSECTSEGMENT2_SPEC_LJUNCTION_AD ) {
                if ( iCross < 0 ) {
                    _EDGESTEP_ADD_B( true, INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A, iA, (Real)0 );
                    iCross = MathRealFn->SignI( (vAB ^ vCD), ZeroTolerance );
                    if ( iCross != 0 ) {
                        _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_LJUNCTION_AC );
                    }
                } else {
                    Assert( iSickleState != _SICKLE_INNER_B );
                    iSickleState = _SICKLE_INNER_A;
                    _EDGESTEP_ADD_B( true, INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A, iA, (Real)0 );
                    iCross = MathRealFn->SignI( (vAB ^ vCD), ZeroTolerance );
                    if ( iCross <= 0 ) {
                        // Boundary case
                        m_iType = INTERSECT2D_POINT;
                        m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;
                        m_iContactCount = 0;
                        _AddContact( INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A | INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B,
                                     iA, iC, vA, (Real)0, (Real)0 );
                        return true;
                    }
                    _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_LJUNCTION_AC );
                }
                continue;
            }

            // T-junction B cases
            if ( iSegSpec == INTERSECTSEGMENT2_SPEC_TJUNCTION_B ) {
                if ( iCross < 0 ) {
                    Assert( iSickleState != _SICKLE_INNER_A );
                    iSickleState = _SICKLE_INNER_B;
                } else {
                    Assert( iSickleState != _SICKLE_INNER_B );
                    iSickleState = _SICKLE_INNER_A;
                }
                _EDGESTEP_ADD_A( true, INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B, iC, intersectSegSeg.GetSegmentBParameter(0) );
                iCross = MathRealFn->SignI( (vAB ^ vCD), ZeroTolerance );
                if ( iCross != 0 ) {
                    _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_TJUNCTION_A );
                }
                continue;
            }

            // T-junction D cases
            if ( iSegSpec == INTERSECTSEGMENT2_SPEC_TJUNCTION_D ) {
                if ( iCross < 0 ) {
                    Assert( iSickleState != _SICKLE_INNER_A );
                    iSickleState = _SICKLE_INNER_B;
                } else {
                    Assert( iSickleState != _SICKLE_INNER_B );
                    iSickleState = _SICKLE_INNER_A;
                }
                _EDGESTEP_ADD_B( true, INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A, iA, intersectSegSeg.GetSegmentAParameter(0) );
                iCross = MathRealFn->SignI( (vAB ^ vCD), ZeroTolerance );
                if ( iCross != 0 ) {
                    _TUNNEL_LOOP( INTERSECTSEGMENT2_SPEC_TJUNCTION_C );
                }
                continue;
            }

            // Terminal cases
            if ( iCross < 0 ) {
                if ( iSegSpec & INTERSECTSEGMENT2_SPEC_TJUNCTION_A ) {
                    // L-junction at AC / T-junction at A
                    if ( iSickleState == _SICKLE_INNER_B )
                        _Exclude_Both();
                    else
                        _Exclude_B();
                } else {
                    // T-junction at C
                    if ( iSickleState == _SICKLE_INNER_A ) {
                        // Boundary case
                        m_iType = INTERSECT2D_POINT;
                        m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;
                        return true;
                    }
                    _Exclude_Both();
                }
                iSickleState = _SICKLE_INNER_A;
                _EDGESTEP_B;
            } else {
                if ( iSegSpec & INTERSECTSEGMENT2_SPEC_TJUNCTION_C ) {
                    // L-junction at AC / T-junction at C
                    if ( iSickleState == _SICKLE_INNER_A )
                        _Exclude_Both();
                    else
                        _Exclude_A();
                } else {
                    // T-junction at A
                    if ( iSickleState == _SICKLE_INNER_B ) {
                        // Boundary case
                        m_iType = INTERSECT2D_POINT;
                        m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;
                        return true;
                    }
                    _Exclude_Both();
                }
                iSickleState = _SICKLE_INNER_B;
                _EDGESTEP_A;
            }
            continue;
        }

        // Generic cases (X-intersection)
        _AddContact( INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A | INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B, iA, iC,
                     intersectSegSeg.GetContact(0),
                     intersectSegSeg.GetSegmentAParameter(0),
                     intersectSegSeg.GetSegmentBParameter(0) );
        _Exclude_Both();
        if ( iCross < 0 ) {
            Assert( iSickleState != _SICKLE_INNER_A );
            iSickleState = _SICKLE_INNER_A;
            _EDGESTEP_B;
        } else {
            Assert( iSickleState != _SICKLE_INNER_B );
            iSickleState = _SICKLE_INNER_B;
            _EDGESTEP_A;
        }
    }

    // Polygon boundaries are intersecting
    if ( bLookupDone ) {
        Assert( m_iType == INTERSECT2D_POLYGON && m_iContactCount > 2 );
        return true;
    }

    // Polygon boundaries don't intersect
    if ( m_iSpecifier == INTERSECTCONVEXPOLYGON2_SPEC_INCLUSION_A ) {
        // A inside B case
        m_iType = INTERSECT2D_POLYGON;
        m_iSpecifier |= INTERSECTCONVEXPOLYGON2_SPEC_INCLUSION_STRICT;
        m_iContactCount = 0;
        for( UInt i = 0; i < iSizeA; ++i ) {
            _AddContact( INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A, i, INVALID_OFFSET,
                         m_pPolygonA->GetVertex(i), (Real)0, (Real)0 );
        }
        return true;
    }
    if ( m_iSpecifier == INTERSECTCONVEXPOLYGON2_SPEC_INCLUSION_B ) {
        // B inside A case
        m_iType = INTERSECT2D_POLYGON;
        m_iSpecifier |= INTERSECTCONVEXPOLYGON2_SPEC_INCLUSION_STRICT;
        m_iContactCount = 0;
        for( UInt i = 0; i < iSizeB; ++i ) {
            _AddContact( INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B, INVALID_OFFSET, i,
                         m_pPolygonB->GetVertex(i), (Real)0, (Real)0 );
        }
        return true;
    }
    Assert( m_iSpecifier == INTERSECT2D_SPECIFIER_NONE );
        // Disjoint case
    m_iType = INTERSECT2D_EMPTY;
    m_iContactCount = 0;
    return false;

    // Destroy helper macros
#undef _TUNNEL_LOOP
#undef _EDGESTEP_ADD_B
#undef _EDGESTEP_ADD_A
#undef _EDGESTEP_B
#undef _EDGESTEP_A
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
inline Void TIntersectConvexPolygon2<Real>::_Exclude_A() {
    m_iSpecifier &= ~INTERSECTCONVEXPOLYGON2_SPEC_INCLUSION_A;
}
template<typename Real>
inline Void TIntersectConvexPolygon2<Real>::_Exclude_B() {
    m_iSpecifier &= ~INTERSECTCONVEXPOLYGON2_SPEC_INCLUSION_B;
}
template<typename Real>
inline Void TIntersectConvexPolygon2<Real>::_Exclude_Both() {
    m_iSpecifier = INTERSECT2D_SPECIFIER_NONE;
}

template<typename Real>
inline Void TIntersectConvexPolygon2<Real>::_AddContact( Intersect2Specifier iSpecifier, UInt iIndexA, UInt iIndexB,
                                                         const TVertex2<Real> & vPosition, Real fParameterA, Real fParameterB ) {
    m_arrContacts[m_iContactCount] = vPosition;
    m_arrContactsData[m_iContactCount].iSpecifier = iSpecifier;
    m_arrContactsData[m_iContactCount].iIndexA = iIndexA;
    m_arrContactsData[m_iContactCount].iIndexB = iIndexB;
    m_arrContactsData[m_iContactCount].fParameterA = fParameterA;
    m_arrContactsData[m_iContactCount].fParameterB = fParameterB;
    ++m_iContactCount;
}

template<typename Real>
Void TIntersectConvexPolygon2<Real>::_ConvertSpecifier( Intersect2Specifier * outSpecifier0, Intersect2Specifier * outSpecifier1,
                                                        Intersect2Specifier iSegmentSpecifier )
{
    if ( INTERSECTSEGMENT2_SPEC_ISEQUALITY(iSegmentSpecifier) ) {
        *outSpecifier0 = INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A | INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B;
        *outSpecifier1 = INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A | INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B;
        return;
    }
    if ( INTERSECTSEGMENT2_SPEC_ISOVERLAP(iSegmentSpecifier) ) {
        if ( iSegmentSpecifier & INTERSECTSEGMENT2_SPEC_OVERLAP_DACB ) {
            *outSpecifier0 = INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A | INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B;
            *outSpecifier1 = INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A | INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B;
        } else {
            Assert( iSegmentSpecifier & INTERSECTSEGMENT2_SPEC_OVERLAP_ADBC );
            *outSpecifier0 = INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A | INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B;
            *outSpecifier1 = INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A | INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B;
        }
        return;
    }
    Assert( INTERSECTSEGMENT2_SPEC_ISINCLUSION(iSegmentSpecifier) );
    if ( iSegmentSpecifier & INTERSECTSEGMENT2_SPEC_INCLUSION_AB ) {
        *outSpecifier0 = INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A;
        *outSpecifier1 = INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A;
        if ( iSegmentSpecifier & INTERSECTSEGMENT2_SPEC_LJUNCTION_BC ) {
            *outSpecifier0 |= INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B;
            *outSpecifier1 |= INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B;
        } else if ( iSegmentSpecifier & INTERSECTSEGMENT2_SPEC_LJUNCTION_AD ) {
            *outSpecifier0 |= INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B;
            *outSpecifier1 |= INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B;
        } else {
            *outSpecifier0 |= INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B;
            *outSpecifier1 |= INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B;
        }
        return;
    }
    Assert( iSegmentSpecifier & INTERSECTSEGMENT2_SPEC_INCLUSION_CD );
    *outSpecifier0 = INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B;
    *outSpecifier1 = INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B;
    if ( iSegmentSpecifier & INTERSECTSEGMENT2_SPEC_LJUNCTION_BC ) {
        *outSpecifier0 |= INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A;
        *outSpecifier1 |= INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A;
    } else if ( iSegmentSpecifier & INTERSECTSEGMENT2_SPEC_LJUNCTION_AD ) {
        *outSpecifier0 |= INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A;
        *outSpecifier1 |= INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A;
    } else {
        *outSpecifier0 |= INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A;
        *outSpecifier1 |= INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A;
    }
}


