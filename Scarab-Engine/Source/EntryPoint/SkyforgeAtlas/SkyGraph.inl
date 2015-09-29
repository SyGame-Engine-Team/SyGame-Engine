/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SkyforgeAtlas/SkyGraph.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SkyGraph, Main abstraction structure
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// SkyNode implementation
inline SkyColor SkyNode::GetColor() const {
    return m_iColor;
}

inline Bool SkyNode::HasProficiency() const {
    return ( m_iColor == SKYCOLOR_RED || m_iColor == SKYCOLOR_GREEN );
}
inline Bool SkyNode::IsLocked() const {
    return ( m_iColor == SKYCOLOR_LOCK );
}

inline SkyArea * SkyNode::GetParentArea( UInt * outNodeIndex ) const {
    if ( outNodeIndex != NULL )
        *outNodeIndex = m_iNodeIndex;
    return m_pParentArea;
}

inline SkyNode * SkyNode::GetLink( SkyGraphPort iPort, SkyGraphPort * outTargetPort ) const {
    Assert( iPort < SKYGRAPH_PORT_COUNT );
    if ( outTargetPort != NULL )
        *outTargetPort = m_arrLinks[iPort].iTargetPort;
    return m_arrLinks[iPort].pTargetNode;
}
inline UInt SkyNode::GetLinkCount() const {
    return m_iLinkCount;
}

inline Void * SkyNode::GetHolder() const {
    return m_pHolder;
}
inline Void SkyNode::SetHolder( Void * pHolder ) {
    m_pHolder = pHolder;
}

inline const Vertex2 & SkyNode::GetPosition() const {
    return m_vPosition;
}

inline UInt SkyNode::GetCost() const {
    return m_iCost;
}

inline UInt SkyNode::GetPrestige() const {
    return m_iPrestige;
}
inline Scalar SkyNode::GetPrestigeRatio() const {
    return m_fPrestigeRatio;
}

inline Void SkyNode::SetParentArea( SkyArea * pParentArea, UInt iNodeIndex ) {
    m_pParentArea = pParentArea;
    m_iNodeIndex = iNodeIndex;
}
inline Void SkyNode::SetPosition( const Vertex2 & vPosition ) {
    m_vPosition = vPosition;
}

/////////////////////////////////////////////////////////////////////////////////
// SkyProficiencyNode implementation
inline UInt SkyProficiencyNode::GetProficiency() const {
    return m_iProficiency;
}
inline Scalar SkyProficiencyNode::GetProficiencyRatio() const {
    return m_fProficiencyRatio;
}

/////////////////////////////////////////////////////////////////////////////////
// SkyMightNode implementation
inline SkyNodeType SkyMightNode::GetType() const {
    return SKYNODE_MIGHT;
}

inline UInt SkyMightNode::GetStatValue() const {
    return m_iMight;
}

inline UInt SkyMightNode::GetMight() const {
    return m_iMight;
}
inline Scalar SkyMightNode::GetMightRatio() const {
    return m_fMightRatio;
}

/////////////////////////////////////////////////////////////////////////////////
// SkyStaminaNode implementation
inline SkyNodeType SkyStaminaNode::GetType() const {
    return SKYNODE_STAMINA;
}

inline UInt SkyStaminaNode::GetStatValue() const {
    return m_iStamina;
}

inline UInt SkyStaminaNode::GetStamina() const {
    return m_iStamina;
}
inline Scalar SkyStaminaNode::GetStaminaRatio() const {
    return m_fStaminaRatio;
}

/////////////////////////////////////////////////////////////////////////////////
// SkyStrengthNode implementation
inline SkyNodeType SkyStrengthNode::GetType() const {
    return SKYNODE_STRENGTH;
}

inline UInt SkyStrengthNode::GetStatValue() const {
    return m_iStrength;
}

inline UInt SkyStrengthNode::GetStrength() const {
    return m_iStrength;
}
inline Scalar SkyStrengthNode::GetStrengthRatio() const {
    return m_fStrengthRatio;
}

/////////////////////////////////////////////////////////////////////////////////
// SkyLuckNode implementation
inline SkyNodeType SkyLuckNode::GetType() const {
    return SKYNODE_LUCK;
}

inline UInt SkyLuckNode::GetStatValue() const {
    return m_iLuck;
}

inline UInt SkyLuckNode::GetLuck() const {
    return m_iLuck;
}
inline Scalar SkyLuckNode::GetLuckRatio() const {
    return m_fLuckRatio;
}

/////////////////////////////////////////////////////////////////////////////////
// SkySpiritNode implementation
inline SkyNodeType SkySpiritNode::GetType() const {
    return SKYNODE_SPIRIT;
}

inline UInt SkySpiritNode::GetStatValue() const {
    return m_iSpirit;
}

inline UInt SkySpiritNode::GetSpirit() const {
    return m_iSpirit;
}
inline Scalar SkySpiritNode::GetSpiritRatio() const {
    return m_fSpiritRatio;
}

/////////////////////////////////////////////////////////////////////////////////
// SkyValorNode implementation
inline SkyNodeType SkyValorNode::GetType() const {
    return SKYNODE_VALOR;
}

inline UInt SkyValorNode::GetStatValue() const {
    return m_iValor;
}

inline UInt SkyValorNode::GetValor() const {
    return m_iValor;
}
inline Scalar SkyValorNode::GetValorRatio() const {
    return m_fValorRatio;
}

/////////////////////////////////////////////////////////////////////////////////
// SkyGreatnessNode implementation
inline SkyNodeType SkyGreatnessNode::GetType() const {
    return SKYNODE_GREATNESS;
}

inline UInt SkyGreatnessNode::GetStatValue() const {
    return m_iGreatness;
}

inline UInt SkyGreatnessNode::GetGreatness() const {
    return m_iGreatness;
}
inline Scalar SkyGreatnessNode::GetGreatnessRatio() const {
    return m_fGreatnessRatio;
}

/////////////////////////////////////////////////////////////////////////////////
// SkyTalentNode implementation
inline SkyNodeType SkyTalentNode::GetType() const {
    return SKYNODE_TALENT;
}

inline UInt SkyTalentNode::GetStatValue() const {
    return (UInt)m_iTalent;
}

inline SkyTalent SkyTalentNode::GetTalent() const {
    return m_iTalent;
}

/////////////////////////////////////////////////////////////////////////////////
// SkyClassNode implementation
inline SkyNodeType SkyClassNode::GetType() const {
    return SKYNODE_CLASS;
}

inline Bool SkyClassNode::IsDivineForm() const {
    return ( m_iClass == SKYCLASS_DIVINE_FORM );
}

inline UInt SkyClassNode::GetStatValue() const {
    return (UInt)m_iClass;
}

inline SkyClass SkyClassNode::GetClass() const {
    return m_iClass;
}

/////////////////////////////////////////////////////////////////////////////////
// SkyArea implementation
inline UInt SkyArea::GetAreaIndex() const {
    return m_iAreaIndex;
}

inline SkyArea * SkyArea::GetLink( SkyGraphPort iPort, SkyGraphPort * outTargetPort ) const {
    Assert( iPort < SKYGRAPH_PORT_COUNT );
    if ( outTargetPort != NULL )
        *outTargetPort = m_arrLinks[iPort].iTargetPort;
    return m_arrLinks[iPort].pTargetArea;
}
inline UInt SkyArea::GetLinkCount() const {
    return m_iLinkCount;
}

inline Void * SkyArea::GetHolder() const {
    return m_pHolder;
}
inline Void SkyArea::SetHolder( Void * pHolder ) {
    m_pHolder = pHolder;
}

inline const Vertex2 & SkyArea::GetPosition() const {
    return m_vPosition;
}

inline UInt SkyArea::GetNodeCount() const {
    return m_iNodeCount;
}
inline SkyNode * SkyArea::GetNode( UInt iIndex ) const {
    Assert( iIndex < m_iNodeCount );
    return m_arrNodes[iIndex];
}

inline SkyNode * SkyArea::GetDoorNode( SkyGraphPort iPort, UInt * outNodeIndex ) const {
    Assert( iPort < SKYGRAPH_PORT_COUNT );
    UInt iDoorNode = m_arrDoorNode[iPort];
    if ( outNodeIndex != NULL )
        *outNodeIndex = iDoorNode;
    if ( iDoorNode == INVALID_OFFSET )
        return NULL;
    Assert( iDoorNode < m_iNodeCount );
    return m_arrNodes[iDoorNode];
}
inline SkyNode * SkyArea::GetCenterNode() const {
    if ( m_iCenterNode == INVALID_OFFSET )
        return NULL;
    Assert( m_iCenterNode < m_iNodeCount );
    return m_arrNodes[m_iCenterNode];
}

inline UInt SkyArea::GetTotalCost() const {
    return m_iTotalCost;
}

inline UInt SkyArea::GetMinNodeCost() const {
    return m_iMinNodeCost;
}
inline UInt SkyArea::GetMaxNodeCost() const {
    return m_iMaxNodeCost;
}
inline Scalar SkyArea::GetAverageNodeCost() const {
    return m_fAverageNodeCost;
}

inline UInt SkyArea::GetTotalPrestige() const {
    return m_iTotalPrestige;
}
inline Scalar SkyArea::GetTotalPrestigeRatio() const {
    return m_fTotalPrestigeRatio;
}

inline UInt SkyArea::GetTotalProficiency() const {
    return m_iTotalProficiency;
}
inline Scalar SkyArea::GetTotalProficiencyRatio() const {
    return m_fTotalProficiencyRatio;
}

inline UInt SkyArea::GetTotalGreatness() const {
    return m_iTotalGreatness;
}
inline Scalar SkyArea::GetTotalGreatnessRatio() const {
    return m_fTotalGreatnessRatio;
}

inline UInt SkyArea::GetTraversalMark() const {
    return m_iTraversalMark;
}

inline Bool SkyArea::IsVisited( UInt iBaseMark ) const {
    return ( m_iTraversalMark != iBaseMark );
}
inline Void SkyArea::MarkVisited() {
    ++m_iTraversalMark;
}
inline Void SkyArea::MarkUnVisited() {
    --m_iTraversalMark;
}

/////////////////////////////////////////////////////////////////////////////////
// SkyGraph implementation
inline UInt SkyGraph::GetAreaCount() const {
    return m_iAreaCount;
}
inline SkyArea * SkyGraph::GetArea( UInt iIndex ) const {
    Assert( iIndex < m_iAreaCount );
    return m_arrAreas[iIndex];
}
