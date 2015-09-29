/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/BroadPhase/SAP/SweepAndPrune.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Persitent-Incremental Sweep-And-Prune BroadPhase using
//               segmented interval lists ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// SAPBound implementation
inline SAPBound & SAPBound::operator=( const SAPBound & rhs ) {
    dwFlaggedProxyID = rhs.dwFlaggedProxyID;
    dwBoundValue = rhs.dwBoundValue;
    return (*this);
}

inline Bool SAPBound::IsMin() const {
    return ( (dwFlaggedProxyID & 0x00000001) == 0 );
}
inline Bool SAPBound::IsMax() const {
    return ( (dwFlaggedProxyID & 0x00000001) != 0 );
}

inline SAPProxyID SAPBound::GetProxyID() const {
    return (SAPProxyID)( dwFlaggedProxyID & 0xfffffffe );
}
inline Void SAPBound::SetProxyID( SAPProxyID idProxy, Bool bIsMax ) {
    Assert( (idProxy & 0x00000001) == 0 );
    dwFlaggedProxyID = (DWord)( idProxy & 0xfffffffe );
    if ( bIsMax )
        dwFlaggedProxyID |= 0x00000001;
}

/////////////////////////////////////////////////////////////////////////////////
// SAPInterval implementation
inline Void SAPInterval::AddCheckProxy( SAPProxyID idProxy ) {
    Bool bRemoved = setRemovedCheckProxies.Remove( idProxy );
    if ( !bRemoved )
        setCheckProxies.Insert( idProxy );
}
inline Void SAPInterval::RemoveCheckProxy( SAPProxyID idProxy ) {
    Bool bRemoved = setCheckProxies.Remove( idProxy );
    if ( !bRemoved )
        setRemovedCheckProxies.Insert( idProxy );
}

/////////////////////////////////////////////////////////////////////////////////
// SweepAndPrune implementation
inline Bool SweepAndPrune::HasProxy( IslandNode * pNode ) const {
    return m_mapProxies.Contains( _GetProxyID(pNode) );
}

/////////////////////////////////////////////////////////////////////////////////

inline DWord SweepAndPrune::_EncodeScalar( Scalar fBoundValue ) {
    DWord dwBoundValue = IntRepr( fBoundValue );
    if ( dwBoundValue & 0x80000000 )
		dwBoundValue = ~dwBoundValue; // reverse negative numbers
	else
		dwBoundValue |= 0x80000000; // switch sign
    return dwBoundValue;
}
inline Scalar SweepAndPrune::_DecodeScalar( DWord dwBoundValue ) {
    if ( dwBoundValue & 0x80000000 )
		dwBoundValue &= 0x7fffffff; // switch sign
	else
		dwBoundValue = ~dwBoundValue; // un-reverse negative numbers
	return ScalarRepr( dwBoundValue );
}

inline Int SweepAndPrune::_Compare_ProxyIDs( const SAPProxyID & idLeft, const SAPProxyID & idRight, Void * /*pUserData*/ ) {
    if ( idLeft < idRight )
        return +1;
    if ( idRight < idLeft )
        return -1;
    return 0;
}
inline SAPProxyID SweepAndPrune::_GetProxyID( IslandNode * pNode ) {
    Assert( (((DWord)pNode) & 0x00000001) == 0 );
    return (SAPProxyID)pNode;
}

inline SAPProxy * SweepAndPrune::_GetProxy( SAPProxyID idProxy ) const {
    ProxyMap::Iterator itProxy = m_mapProxies.Get( idProxy );
    if ( itProxy.IsNull() )
        return NULL;
    return itProxy.GetItemPtr();
}


