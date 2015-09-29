/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SkyforgeAtlas/Entities.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Entities, main scene objects
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// EntityCharacter implementation
inline CharacterController * EntityCharacter::GetController() const {
    return m_pController;
}

/////////////////////////////////////////////////////////////////////////////////
// EntityArea implementation
inline SkyArea * EntityArea::GetArea() const {
    return m_pArea;
}

/////////////////////////////////////////////////////////////////////////////////
// EntityNode implementation
inline SkyNode * EntityNode::GetNode() const {
    return m_pNode;
}

/////////////////////////////////////////////////////////////////////////////////
// EntityNodeLinkKey implementation
inline Int EntityNodeLinkKey::Compare( const EntityNodeLinkKey & rLeft, const EntityNodeLinkKey & rRight, Void * /*pUserData*/ ) {
    if ( rLeft.pNodeA < rRight.pNodeA )
        return +1;
    if ( rLeft.pNodeA > rRight.pNodeA )
        return -1;
    if ( rLeft.pNodeB < rRight.pNodeB )
        return +1;
    if ( rLeft.pNodeB > rRight.pNodeB )
        return -1;
    return 0;
}

inline EntityNodeLinkKey & EntityNodeLinkKey::operator=( const EntityNodeLinkKey & rhs ) {
    pNodeA = rhs.pNodeA;
    pNodeB = rhs.pNodeB;
    return (*this);
}

inline Bool EntityNodeLinkKey::operator==( const EntityNodeLinkKey & rhs ) const {
    return ( pNodeA == rhs.pNodeA && pNodeB == rhs.pNodeB );
}
inline Bool EntityNodeLinkKey::operator!=( const EntityNodeLinkKey & rhs ) const {
    return ( pNodeA != rhs.pNodeA || pNodeB != rhs.pNodeB );
}

/////////////////////////////////////////////////////////////////////////////////
// EntityNodeLink implementation
inline EntityNode * EntityNodeLink::GetNodeA() const {
    return m_pNodeA;
}
inline EntityNode * EntityNodeLink::GetNodeB() const {
    return m_pNodeB;
}


