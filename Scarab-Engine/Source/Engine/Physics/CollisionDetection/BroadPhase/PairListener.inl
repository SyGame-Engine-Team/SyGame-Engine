/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/BroadPhase/PairListener.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Pair events entry point for NarrowPhase.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// NodePair implementation
inline Int NodePair::_Compare( const NodePair & leftPair, const NodePair & rightPair, Void * /*pUserData*/ ) {
    if ( leftPair.pNodeA < rightPair.pNodeA )
        return +1;
    if ( rightPair.pNodeA < leftPair.pNodeA )
        return -1;
    if ( leftPair.pNodeB < rightPair.pNodeB )
        return +1;
    if ( rightPair.pNodeB < leftPair.pNodeB )
        return -1;
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// PairListener implementation
