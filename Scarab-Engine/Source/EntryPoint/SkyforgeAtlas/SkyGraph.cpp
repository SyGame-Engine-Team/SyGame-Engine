/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SkyforgeAtlas/SkyGraph.cpp
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
// Includes
#include "SkyGraph.h"

/////////////////////////////////////////////////////////////////////////////////
// String conversions
const GChar * _SkyTalentConvert( SkyTalent iTalent )
{
    static const GChar * s_arrTalents[SKYTALENT_COUNT] = {
        TEXT("SLOT_SUN"), TEXT("SLOT_HET"), TEXT("SLOT_COPH"),

        TEXT("ULTIMATE_STRENGTH"), TEXT("MAXIMUM_RECOIL"), TEXT("IMPULSE_INTENSITY"), TEXT("MOBILISATION"),
        TEXT("LUCKY_SHOT"), TEXT("DOUBLE_DISCHARGE"), TEXT("LACERATED_WOUNDS"), TEXT("PAINFUL_SPASMS"),
        TEXT("WARRIORS_CREED"),
        
        TEXT("ELEMENT_OF_SURPRISE"), TEXT("RAPID_ATTACK"),

        TEXT("SLOWING_ATTACKS"), TEXT("IMPACT"), TEXT("GLORIOUS TRIUMPH"), TEXT("MARK_OF_DEATH"),
        TEXT("SUMMON_OCULAT"),

        TEXT("SHOCK_PAIN"), TEXT("SHOCK_PARALYZE"), TEXT("WAVE_DAMAGE"), TEXT("WAVE_SLOW"),
        TEXT("RUTHLESSNESS"),
        
        TEXT("TACTICAL_DEFENSE"), TEXT("PROTECTORS_CREED"), TEXT("BALANCE"),
        TEXT("ADRENALINE_SURGE"), TEXT("LIFE_ENERGY"),
        TEXT("IMPULSE_BARRIER"),

        TEXT("BONUS_DASH"), TEXT("CLEVER_MANOEUVER"), TEXT("INERTIA"), TEXT("BREAKOUT"),
        TEXT("DESPERATE_RESISTANCE"), TEXT("FORCED_RESPITE"),

        TEXT("DEFENSE_REFLEX"),
        
        TEXT("CONSUMABLES_EFFICIENCY"), TEXT("RECHARGING_CONSUMABLES")
    };
    return s_arrTalents[iTalent];
}
SkyTalent _SkyTalentConvert( const GChar * strTalent )
{
    for( UInt i = 0; i < SKYTALENT_COUNT; ++i ) {
        const GChar * strTmp = _SkyTalentConvert( (SkyTalent)i );
        if ( StringFn->Cmp(strTalent, strTmp) == 0 )
            return (SkyTalent)i;
    }
    Assert( false );
    return SKYTALENT_COUNT;
}

const GChar * _SkyClassConvert( SkyClass iClass )
{
    static const GChar * s_arrClasses[SKYCLASS_COUNT] = {
        TEXT("CRYOMANCER"), TEXT("PALADIN"), TEXT("LIGHTBRINGER"), TEXT("BERSERKER"),
        TEXT("ARCHER"), TEXT("SLAYER"), TEXT("NECROMANCER"), TEXT("KINETIC"),
        TEXT("GUNNER"), TEXT("KNIGHT"), TEXT("MONK"), TEXT("ALCHIMIST"),
        TEXT("WITCH"),
        
        TEXT("DIVINE_FORM")
    };
    return s_arrClasses[iClass];
}
SkyClass _SkyClassConvert( const GChar * strClass )
{
    for( UInt i = 0; i < SKYCLASS_COUNT; ++i ) {
        const GChar * strTmp = _SkyClassConvert( (SkyClass)i );
        if ( StringFn->Cmp(strClass, strTmp) == 0 )
            return (SkyClass)i;
    }
    Assert( false );
    return SKYCLASS_COUNT;
}

/////////////////////////////////////////////////////////////////////////////////
// SkyNode implementation
SkyNode::SkyNode( SkyColor iColor, UInt iCost, UInt iPrestige ):
    m_vPosition( Vertex2::Null )
{
    // Linkage data
    m_pParentArea = NULL;
    m_iNodeIndex = INVALID_OFFSET;

    m_arrLinks[SKYGRAPH_EAST].pTargetNode = NULL;
    m_arrLinks[SKYGRAPH_NORTH].pTargetNode = NULL;
    m_arrLinks[SKYGRAPH_WEST].pTargetNode = NULL;
    m_arrLinks[SKYGRAPH_SOUTH].pTargetNode = NULL;
    m_iLinkCount = 0;

    m_pHolder = NULL;

    // Node data
    m_iColor = iColor;
    m_iCost = iCost;
    m_fInvCost = 0.0f;
    if ( m_iCost > 0 )
        m_fInvCost = MathFn->Invert( (Scalar)m_iCost );

    m_iPrestige = iPrestige;
    m_fPrestigeRatio = m_fInvCost * (Scalar)m_iPrestige;
}
SkyNode::~SkyNode()
{
    // Disconnect all
    Disconnect( SKYGRAPH_EAST );
    Disconnect( SKYGRAPH_NORTH );
    Disconnect( SKYGRAPH_WEST );
    Disconnect( SKYGRAPH_SOUTH );
}

Void SkyNode::Connect( SkyGraphPort iPort, SkyNode * pTargetNode, SkyGraphPort iTargetPort )
{
    Assert( iPort < SKYGRAPH_PORT_COUNT );
    Assert( iTargetPort < SKYGRAPH_PORT_COUNT );
    Assert( m_arrLinks[iPort].pTargetNode == NULL );

    // This => Target
    m_arrLinks[iPort].pTargetNode = pTargetNode;
    m_arrLinks[iPort].iTargetPort = iTargetPort;
    ++m_iLinkCount;
}
Void SkyNode::Disconnect( SkyGraphPort iPort )
{
    Assert( iPort < SKYGRAPH_PORT_COUNT );
    if ( m_arrLinks[iPort].pTargetNode == NULL )
        return;

    // This => Target
    m_arrLinks[iPort].pTargetNode = NULL;
    --m_iLinkCount;
}

/////////////////////////////////////////////////////////////////////////////////
// SkyProficiencyNode implementation
SkyProficiencyNode::SkyProficiencyNode( SkyColor iColor, UInt iCost, UInt iPrestige, UInt iProficiency ):
    SkyNode( iColor, iCost, iPrestige )
{
    // Node data
    m_iProficiency = iProficiency;

    // Pre-computed values
    m_fProficiencyRatio = m_fInvCost * (Scalar)m_iProficiency;
}
SkyProficiencyNode::~SkyProficiencyNode()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SkyMightNode implementation
SkyMightNode::SkyMightNode( UInt iCost, UInt iPrestige, UInt iProficiency, UInt iMight, Bool bIsLocked ):
    SkyProficiencyNode( bIsLocked ? SKYCOLOR_LOCK : SKYCOLOR_RED, iCost, iPrestige, bIsLocked ? 0 : iProficiency )
{
    // Node data
    m_iMight = iMight;

    // Pre-computed values
    m_fMightRatio = m_fInvCost * (Scalar)m_iMight;
}
SkyMightNode::~SkyMightNode()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SkyStaminaNode implementation
SkyStaminaNode::SkyStaminaNode( UInt iCost, UInt iPrestige, UInt iProficiency, UInt iStamina, Bool bIsLocked ):
    SkyProficiencyNode( bIsLocked ? SKYCOLOR_LOCK : SKYCOLOR_GREEN, iCost, iPrestige, bIsLocked ? 0 : iProficiency )
{
    // Node data
    m_iStamina = iStamina;

    // Pre-computed values
    m_fStaminaRatio = m_fInvCost * (Scalar)m_iStamina;
}
SkyStaminaNode::~SkyStaminaNode()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SkyStrengthNode implementation
SkyStrengthNode::SkyStrengthNode( UInt iCost, UInt iPrestige, UInt iStrength, Bool bIsLocked ):
    SkyNode( bIsLocked ? SKYCOLOR_LOCK : SKYCOLOR_BLUE, iCost, iPrestige )
{
    // Node data
    m_iStrength = iStrength;

    // Pre-computed values
    m_fStrengthRatio = m_fInvCost * (Scalar)m_iStrength;
}
SkyStrengthNode::~SkyStrengthNode()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SkyLuckNode implementation
SkyLuckNode::SkyLuckNode( UInt iCost, UInt iPrestige, UInt iLuck, Bool bIsLocked ):
    SkyNode( bIsLocked ? SKYCOLOR_LOCK : SKYCOLOR_BLUE, iCost, iPrestige )
{
    // Node data
    m_iLuck = iLuck;

    // Pre-computed values
    m_fLuckRatio = m_fInvCost * (Scalar)m_iLuck;
}
SkyLuckNode::~SkyLuckNode()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SkySpiritNode implementation
SkySpiritNode::SkySpiritNode( UInt iCost, UInt iPrestige, UInt iSpirit, Bool bIsLocked ):
    SkyNode( bIsLocked ? SKYCOLOR_LOCK : SKYCOLOR_BLUE, iCost, iPrestige )
{
    // Node data
    m_iSpirit = iSpirit;

    // Pre-computed values
    m_fSpiritRatio = m_fInvCost * (Scalar)m_iSpirit;
}
SkySpiritNode::~SkySpiritNode()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SkyValorNode implementation
SkyValorNode::SkyValorNode( UInt iCost, UInt iPrestige, UInt iValor, Bool bIsLocked ):
    SkyNode( bIsLocked ? SKYCOLOR_LOCK : SKYCOLOR_BLUE, iCost, iPrestige )
{
    // Node data
    m_iValor = iValor;

    // Pre-computed values
    m_fValorRatio = m_fInvCost * (Scalar)m_iValor;
}
SkyValorNode::~SkyValorNode()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SkyGreatnessNode implementation
SkyGreatnessNode::SkyGreatnessNode( UInt iCost, UInt iPrestige, UInt iGreatness ):
    SkyNode( SKYCOLOR_BLUE, iCost, iPrestige )
{
    // Node data
    m_iGreatness = iGreatness;

    // Pre-computed values
    m_fGreatnessRatio = m_fInvCost * (Scalar)m_iGreatness;
}
SkyGreatnessNode::~SkyGreatnessNode()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SkyTalentNode implementation
SkyTalentNode::SkyTalentNode( UInt iCost, UInt iPrestige, SkyTalent iTalent ):
    SkyNode( SKYCOLOR_BLUE, iCost, iPrestige )
{
    // Node data
    m_iTalent = iTalent;
}
SkyTalentNode::~SkyTalentNode()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SkyClassNode implementation
SkyClassNode::SkyClassNode( UInt iCost, SkyClass iClass ):
    SkyNode( (iClass == SKYCLASS_DIVINE_FORM) ? SKYCOLOR_DIVINE_FORM : SKYCOLOR_CLASS, iCost, 0 )
{
    // Node data
    m_iClass = iClass;
}
SkyClassNode::~SkyClassNode()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SkyArea implementation
SkyArea::SkyArea( UInt iAreaIndex, const Vertex2 & vPosition ):
    m_vPosition( vPosition )
{
    m_iTraversalMark = 0;

    // Linkage data
    m_iAreaIndex = iAreaIndex;

    m_arrLinks[SKYGRAPH_EAST].pTargetArea = NULL;
    m_arrLinks[SKYGRAPH_NORTH].pTargetArea = NULL;
    m_arrLinks[SKYGRAPH_WEST].pTargetArea = NULL;
    m_arrLinks[SKYGRAPH_SOUTH].pTargetArea = NULL;
    m_iLinkCount = 0;

    m_pHolder = NULL;

    // Area data
    m_iNodeCount = 0;
    for( UInt i = 0; i < SKYGRAPH_MAX_NODES_PER_AREA; ++i )
        m_arrNodes[i] = NULL;

    m_arrDoorNode[SKYGRAPH_EAST] = INVALID_OFFSET;
    m_arrDoorNode[SKYGRAPH_NORTH] = INVALID_OFFSET;
    m_arrDoorNode[SKYGRAPH_WEST] = INVALID_OFFSET;
    m_arrDoorNode[SKYGRAPH_SOUTH] = INVALID_OFFSET;
    m_iCenterNode = INVALID_OFFSET;

    // Pre-computed values
    m_fInvNodeCount = 0.0f;

    m_iTotalCost = 0;

    m_iMinNodeCost = 0;
    m_iMaxNodeCost = 0;
    m_fAverageNodeCost = 0.0f;

    m_iPrestigeNodeCount = 0;
    m_iTotalPrestige = 0;
    m_fTotalPrestigeRatio = 0.0f;

    m_iProficiencyNodeCount = 0;
    m_iTotalProficiency = 0;
    m_fTotalProficiencyRatio = 0.0f;

    m_iGreatnessNodeCount = 0;
    m_iTotalGreatness = 0;
    m_fTotalGreatnessRatio = 0.0f;

    // Pre-computed paths
    // nothing to do
}
SkyArea::~SkyArea()
{
    // Destroy nodes
    for( UInt i = 0; i < m_iNodeCount; ++i ) {
        if (  m_arrNodes[i] != NULL ) {
            Delete( m_arrNodes[i] );
            m_arrNodes[i] = NULL;
        }
    }
}

Void SkyArea::SetDoorNode( SkyGraphPort iPort, UInt iDoorNode )
{
    Assert( iPort < SKYGRAPH_PORT_COUNT );
    Assert( iDoorNode == INVALID_OFFSET || iDoorNode < m_iNodeCount );
    Assert( m_arrNodes[iDoorNode] != NULL );

    m_arrDoorNode[iPort] = iDoorNode;
}

Void SkyArea::Connect( SkyGraphPort iPort, SkyArea * pTargetArea, SkyGraphPort iTargetPort )
{
    Assert( iPort < SKYGRAPH_PORT_COUNT );
    Assert( iTargetPort < SKYGRAPH_PORT_COUNT );
    Assert( m_arrLinks[iPort].pTargetArea == NULL );

    // This => Target
    m_arrLinks[iPort].pTargetArea = pTargetArea;
    m_arrLinks[iPort].iTargetPort = iTargetPort;
    ++m_iLinkCount;

    SkyNode * pDoorNode = GetDoorNode( iPort );
    SkyNode * pTargetDoorNode = pTargetArea->GetDoorNode( iTargetPort );
    Assert( pDoorNode != NULL && pTargetDoorNode != NULL );

    pDoorNode->Connect( iPort, pTargetDoorNode, iTargetPort );
}
Void SkyArea::Disconnect( SkyGraphPort iPort )
{
    Assert( iPort < SKYGRAPH_PORT_COUNT );
    if ( m_arrLinks[iPort].pTargetArea == NULL )
        return;

    // This => Target
    m_arrLinks[iPort].pTargetArea = NULL;
    --m_iLinkCount;

    SkyNode * pDoorNode = GetDoorNode( iPort );
    Assert( pDoorNode != NULL );

    pDoorNode->Disconnect( iPort );
}

/////////////////////////////////////////////////////////////////////////////////

Void SkyArea::_LoadFromXML( const XMLNode * pXMLArea )
{
    m_iNodeCount = pXMLArea->GetChildCount();

    // Pass 1 : Load all nodes
    for( UInt i = 0; i < m_iNodeCount; ++i ) {
        XMLNode * pXMLNode = pXMLArea->GetChild( i );
        Assert( pXMLNode != NULL );

        // Grab index attribute
        XMLAttribute * pAttrib = pXMLNode->GetAttribute( TEXT("index") );
        Assert( pAttrib != NULL );
        UInt iIndex = (UInt)( StringFn->ToUInt( pAttrib->GetValue() ) );

        // Grab position attribute
        pAttrib = pXMLNode->GetAttribute( TEXT("position") );
        Assert( pAttrib != NULL );
        const GChar * strPosition = pAttrib->GetValue();

        Scalar fX = SCALAR_INFINITE, fY = SCALAR_INFINITE;
        StringFn->Scan( strPosition, TEXT("%f,%f"), &fX, &fY );
        Assert( fX != SCALAR_INFINITE && fY != SCALAR_INFINITE );

        // Grab type attribute
        pAttrib = pXMLNode->GetAttribute( TEXT("type") );
        Assert( pAttrib != NULL );
        const GChar * strType = pAttrib->GetValue();

        // Grab color attribute
        pAttrib = pXMLNode->GetAttribute( TEXT("color") );
        Assert( pAttrib != NULL );
        const GChar * strColor = pAttrib->GetValue();

        SkyColor iColor = SKYCOLOR_COUNT;
        if ( StringFn->Cmp(strColor, TEXT("RED")) == 0 )
            iColor = SKYCOLOR_RED;
        else if ( StringFn->Cmp(strColor, TEXT("GREEN")) == 0 )
            iColor = SKYCOLOR_GREEN;
        else if ( StringFn->Cmp(strColor, TEXT("BLUE")) == 0 )
            iColor = SKYCOLOR_BLUE;
        else if ( StringFn->Cmp(strColor, TEXT("LOCK")) == 0 )
            iColor = SKYCOLOR_LOCK;
        else if ( StringFn->Cmp(strColor, TEXT("CLASS")) == 0 )
            iColor = SKYCOLOR_CLASS;
        else if ( StringFn->Cmp(strColor, TEXT("DIVINEFORM")) == 0 )
            iColor = SKYCOLOR_DIVINE_FORM;
        Assert( iColor != SKYCOLOR_COUNT );

        // Grab cost attribute
        pAttrib = pXMLNode->GetAttribute( TEXT("cost") );
        Assert( pAttrib != NULL );
        UInt iCost = (UInt)( StringFn->ToUInt( pAttrib->GetValue() ) );

        // Grab prestige attribute
        UInt iPrestige = 0;
        if ( iColor < SKYCOLOR_CLASS ) {
            pAttrib = pXMLNode->GetAttribute( TEXT("prestige") );
            Assert( pAttrib != NULL );
            iPrestige = (UInt)( StringFn->ToUInt( pAttrib->GetValue() ) );
        }

        // Grab proficiency attribute
        UInt iProficiency = 0;
        if ( iColor == SKYCOLOR_RED || iColor == SKYCOLOR_GREEN ) {
            pAttrib = pXMLNode->GetAttribute( TEXT("proficiency") );
            Assert( pAttrib != NULL );
            iProficiency = (UInt)( StringFn->ToUInt( pAttrib->GetValue() ) );
        }

        // Grab statvalue attribute
        pAttrib = pXMLNode->GetAttribute( TEXT("statvalue") );
        Assert( pAttrib != NULL );
        const GChar * strStatValue = pAttrib->GetValue();

        // Switch on types
        if ( StringFn->Cmp(strType, TEXT("MIGHT")) == 0 ) {
            UInt iStatValue = (UInt)( StringFn->ToUInt( strStatValue ) );
            m_arrNodes[iIndex] = New SkyMightNode( iCost, iPrestige, iProficiency, iStatValue, (iColor == SKYCOLOR_LOCK) );
        }
        else if ( StringFn->Cmp(strType, TEXT("STAMINA")) == 0 ) {
            UInt iStatValue = (UInt)( StringFn->ToUInt( strStatValue ) );
            m_arrNodes[iIndex] = New SkyStaminaNode( iCost, iPrestige, iProficiency, iStatValue, (iColor == SKYCOLOR_LOCK) );
        }
        else if ( StringFn->Cmp(strType, TEXT("STRENGTH")) == 0 ) {
            UInt iStatValue = (UInt)( StringFn->ToUInt( strStatValue ) );
            m_arrNodes[iIndex] = New SkyStrengthNode( iCost, iPrestige, iStatValue, (iColor == SKYCOLOR_LOCK) );
        }
        else if ( StringFn->Cmp(strType, TEXT("LUCK")) == 0 ) {
            UInt iStatValue = (UInt)( StringFn->ToUInt( strStatValue ) );
            m_arrNodes[iIndex] = New SkyLuckNode( iCost, iPrestige, iStatValue, (iColor == SKYCOLOR_LOCK) );
        }
        else if ( StringFn->Cmp(strType, TEXT("SPIRIT")) == 0 ) {
            UInt iStatValue = (UInt)( StringFn->ToUInt( strStatValue ) );
            m_arrNodes[iIndex] = New SkySpiritNode( iCost, iPrestige, iStatValue, (iColor == SKYCOLOR_LOCK) );
        }
        else if ( StringFn->Cmp(strType, TEXT("VALOR")) == 0 ) {
            UInt iStatValue = (UInt)( StringFn->ToUInt( strStatValue ) );
            m_arrNodes[iIndex] = New SkyValorNode( iCost, iPrestige, iStatValue, (iColor == SKYCOLOR_LOCK) );
        }
        else if ( StringFn->Cmp(strType, TEXT("GREATNESS")) == 0 ) {
            UInt iStatValue = (UInt)( StringFn->ToUInt( strStatValue ) );
            m_arrNodes[iIndex] = New SkyGreatnessNode( iCost, iPrestige, iStatValue );
        }
        else if ( StringFn->Cmp(strType, TEXT("TALENT")) == 0 ) {
            SkyTalent iTalent = _SkyTalentConvert( strStatValue );
            m_arrNodes[iIndex] = New SkyTalentNode( iCost, iPrestige, iTalent );
        }
        else if ( StringFn->Cmp(strType, TEXT("CLASS")) == 0 ) {
            SkyClass iClass = _SkyClassConvert( strStatValue );
            m_arrNodes[iIndex] = New SkyClassNode( iCost, iClass );
        }
        else {
            Assert( false );
        }

        // Setup node
        m_arrNodes[iIndex]->SetParentArea( this, iIndex );
        m_arrNodes[iIndex]->SetPosition( Vertex2(fX,fY) );
    }

    // Pass 2 : Node connections
    for( UInt i = 0; i < m_iNodeCount; ++i ) {
        XMLNode * pXMLNode = pXMLArea->GetChild( i );
        Assert( pXMLNode != NULL );

        // Grab index attribute
        XMLAttribute * pAttrib = pXMLNode->GetAttribute( TEXT("index") );
        Assert( pAttrib != NULL );
        UInt iIndex = (UInt)( StringFn->ToUInt( pAttrib->GetValue() ) );

        // Pick loaded node
        SkyNode * pNode = m_arrNodes[iIndex];

        const GChar * strLink;
        UInt iTargetIndex;
        GChar strTargetPort[2];
        SkyGraphPort iTargetPort;

        // East-connection
        if ( pXMLNode->HasAttribute(TEXT("ELink")) ) {
            // Grab ELink attribute
            pAttrib = pXMLNode->GetAttribute( TEXT("ELink") );
            Assert( pAttrib != NULL );
            strLink = pAttrib->GetValue();

            iTargetIndex = INVALID_OFFSET;
            StringFn->Scan( strLink, TEXT("%un,%s"), &iTargetIndex, strTargetPort, 2 );
            Assert( iTargetIndex != INVALID_OFFSET );
            Assert( m_arrNodes[iTargetIndex] != NULL );

            // Convert target port
            iTargetPort = SKYGRAPH_PORT_COUNT;
            if ( strTargetPort[0] == TEXT('E') )
                iTargetPort = SKYGRAPH_EAST;
            else if ( strTargetPort[0] == TEXT('N') )
                iTargetPort = SKYGRAPH_NORTH;
            else if ( strTargetPort[0] == TEXT('W') )
                iTargetPort = SKYGRAPH_WEST;
            else if ( strTargetPort[0] == TEXT('S') )
                iTargetPort = SKYGRAPH_SOUTH;
            Assert( iTargetPort != SKYGRAPH_PORT_COUNT );

            pNode->Connect( SKYGRAPH_EAST, m_arrNodes[iTargetIndex], iTargetPort );
        }

        // North-connection
        if ( pXMLNode->HasAttribute(TEXT("NLink")) ) {
            // Grab NLink attribute
            pAttrib = pXMLNode->GetAttribute( TEXT("NLink") );
            Assert( pAttrib != NULL );
            strLink = pAttrib->GetValue();

            iTargetIndex = INVALID_OFFSET;
            StringFn->Scan( strLink, TEXT("%un,%s"), &iTargetIndex, strTargetPort, 2 );
            Assert( iTargetIndex != INVALID_OFFSET );
            Assert( m_arrNodes[iTargetIndex] != NULL );

            // Convert target port
            iTargetPort = SKYGRAPH_PORT_COUNT;
            if ( strTargetPort[0] == TEXT('E') )
                iTargetPort = SKYGRAPH_EAST;
            else if ( strTargetPort[0] == TEXT('N') )
                iTargetPort = SKYGRAPH_NORTH;
            else if ( strTargetPort[0] == TEXT('W') )
                iTargetPort = SKYGRAPH_WEST;
            else if ( strTargetPort[0] == TEXT('S') )
                iTargetPort = SKYGRAPH_SOUTH;
            Assert( iTargetPort != SKYGRAPH_PORT_COUNT );

            pNode->Connect( SKYGRAPH_NORTH, m_arrNodes[iTargetIndex], iTargetPort );
        }

        // West-connection
        if ( pXMLNode->HasAttribute(TEXT("WLink")) ) {
            // Grab WLink attribute
            pAttrib = pXMLNode->GetAttribute( TEXT("WLink") );
            Assert( pAttrib != NULL );
            strLink = pAttrib->GetValue();

            iTargetIndex = INVALID_OFFSET;
            StringFn->Scan( strLink, TEXT("%un,%s"), &iTargetIndex, strTargetPort, 2 );
            Assert( iTargetIndex != INVALID_OFFSET );
            Assert( m_arrNodes[iTargetIndex] != NULL );

            // Convert target port
            iTargetPort = SKYGRAPH_PORT_COUNT;
            if ( strTargetPort[0] == TEXT('E') )
                iTargetPort = SKYGRAPH_EAST;
            else if ( strTargetPort[0] == TEXT('N') )
                iTargetPort = SKYGRAPH_NORTH;
            else if ( strTargetPort[0] == TEXT('W') )
                iTargetPort = SKYGRAPH_WEST;
            else if ( strTargetPort[0] == TEXT('S') )
                iTargetPort = SKYGRAPH_SOUTH;
            Assert( iTargetPort != SKYGRAPH_PORT_COUNT );

            pNode->Connect( SKYGRAPH_WEST, m_arrNodes[iTargetIndex], iTargetPort );
        }

        // South-connection
        if ( pXMLNode->HasAttribute(TEXT("SLink")) ) {
            // Grab SLink attribute
            pAttrib = pXMLNode->GetAttribute( TEXT("SLink") );
            Assert( pAttrib != NULL );
            strLink = pAttrib->GetValue();

            iTargetIndex = INVALID_OFFSET;
            StringFn->Scan( strLink, TEXT("%un,%s"), &iTargetIndex, strTargetPort, 2 );
            Assert( iTargetIndex != INVALID_OFFSET );
            Assert( m_arrNodes[iTargetIndex] != NULL );

            // Convert target port
            iTargetPort = SKYGRAPH_PORT_COUNT;
            if ( strTargetPort[0] == TEXT('E') )
                iTargetPort = SKYGRAPH_EAST;
            else if ( strTargetPort[0] == TEXT('N') )
                iTargetPort = SKYGRAPH_NORTH;
            else if ( strTargetPort[0] == TEXT('W') )
                iTargetPort = SKYGRAPH_WEST;
            else if ( strTargetPort[0] == TEXT('S') )
                iTargetPort = SKYGRAPH_SOUTH;
            Assert( iTargetPort != SKYGRAPH_PORT_COUNT );

            pNode->Connect( SKYGRAPH_SOUTH, m_arrNodes[iTargetIndex], iTargetPort );
        }
    }

    // Done !
}
Void SkyArea::_SaveToXML( XMLNode * pXMLArea ) const
{
    GChar strBuffer[64];

    // Enumerate all nodes
    for( UInt i = 0; i < m_iNodeCount; ++i ) {
        SkyNode * pNode = m_arrNodes[i];

        // Create an xml node
        XMLNode * pXMLNode = XMLDocument::CreateNode( TEXT("SkyNode"), true );

        // Add attributes
            // index
        StringFn->FromUInt( strBuffer, i );
        pXMLNode->CreateAttribute( TEXT("index"), strBuffer );

            // position
        StringFn->Format( strBuffer, TEXT("%f,%f"), pNode->GetPosition().X, pNode->GetPosition().Y );
        pXMLNode->CreateAttribute( TEXT("position"), strBuffer );

            // type
        switch( pNode->GetType() ) {
            case SKYNODE_MIGHT:     pXMLNode->CreateAttribute( TEXT("type"), TEXT("MIGHT") ); break;
            case SKYNODE_STAMINA:   pXMLNode->CreateAttribute( TEXT("type"), TEXT("STAMINA") ); break;
            case SKYNODE_STRENGTH:  pXMLNode->CreateAttribute( TEXT("type"), TEXT("STRENGTH") ); break;
            case SKYNODE_LUCK:      pXMLNode->CreateAttribute( TEXT("type"), TEXT("LUCK") ); break;
            case SKYNODE_SPIRIT:    pXMLNode->CreateAttribute( TEXT("type"), TEXT("SPIRIT") ); break;
            case SKYNODE_VALOR:     pXMLNode->CreateAttribute( TEXT("type"), TEXT("VALOR") ); break;
            case SKYNODE_GREATNESS: pXMLNode->CreateAttribute( TEXT("type"), TEXT("GREATNESS") ); break;
            case SKYNODE_TALENT:    pXMLNode->CreateAttribute( TEXT("type"), TEXT("TALENT") ); break;
            case SKYNODE_CLASS:     pXMLNode->CreateAttribute( TEXT("type"), TEXT("CLASS") ); break;
            default: Assert( false ); break;
        }

            // color
        switch( pNode->GetColor() ) {
            case SKYCOLOR_RED:         pXMLNode->CreateAttribute( TEXT("color"), TEXT("RED") ); break;
            case SKYCOLOR_GREEN:       pXMLNode->CreateAttribute( TEXT("color"), TEXT("GREEN") ); break;
            case SKYCOLOR_BLUE:        pXMLNode->CreateAttribute( TEXT("color"), TEXT("BLUE") ); break;
            case SKYCOLOR_LOCK:        pXMLNode->CreateAttribute( TEXT("color"), TEXT("LOCK") ); break;
            case SKYCOLOR_CLASS:       pXMLNode->CreateAttribute( TEXT("color"), TEXT("CLASS") ); break;
            case SKYCOLOR_DIVINE_FORM: pXMLNode->CreateAttribute( TEXT("color"), TEXT("DIVINEFORM") ); break;
            default: Assert( false ); break;
        }

            // cost
        StringFn->FromUInt( strBuffer, pNode->GetCost() );
        pXMLNode->CreateAttribute( TEXT("cost"), strBuffer );

            // prestige
        if ( pNode->GetType() != SKYNODE_CLASS ) {
            StringFn->FromUInt( strBuffer, pNode->GetPrestige() );
            pXMLNode->CreateAttribute( TEXT("prestige"), strBuffer );
        }

            // proficiency
        if ( pNode->HasProficiency() ) {
            SkyProficiencyNode * pProficiencyNode = (SkyProficiencyNode*)pNode;
            StringFn->FromUInt( strBuffer, pProficiencyNode->GetProficiency() );
            pXMLNode->CreateAttribute( TEXT("proficiency"), strBuffer );
        }

            // statvalue
        if ( pNode->GetType() == SKYNODE_TALENT ) {
            pXMLNode->CreateAttribute( TEXT("statvalue"), _SkyTalentConvert((SkyTalent)(pNode->GetStatValue())) );
        } else if ( pNode->GetType() == SKYNODE_CLASS ) {
            pXMLNode->CreateAttribute( TEXT("statvalue"), _SkyClassConvert((SkyClass)(pNode->GetStatValue())) );
        } else {
            StringFn->FromUInt( strBuffer, pNode->GetStatValue() );
            pXMLNode->CreateAttribute( TEXT("statvalue"), strBuffer );
        }

            // links
        static const GChar * s_strTagNames[4] = {
            TEXT("ELink"), TEXT("NLink"), TEXT("WLink"), TEXT("SLink")
        };
        static const GChar * s_strPortLabels[4] = {
            TEXT("E"), TEXT("N"), TEXT("W"), TEXT("S")
        };
        for( UInt j = 0; j < SKYGRAPH_PORT_COUNT; ++j ) {
            SkyGraphPort iPort = (SkyGraphPort)j;
            SkyGraphPort iTargetPort = SKYGRAPH_PORT_COUNT;

            SkyNode * pTargetNode = pNode->GetLink( iPort, &iTargetPort );
            if ( pTargetNode == NULL )
                continue;
            Assert( iTargetPort != SKYGRAPH_PORT_COUNT );

            UInt iTargetNode = INVALID_OFFSET;
            pTargetNode->GetParentArea( &iTargetNode );
            Assert( iTargetNode != INVALID_OFFSET );

            StringFn->Format( strBuffer, TEXT("%ud,%s"), iTargetNode, s_strPortLabels[iTargetPort] );
            pXMLNode->CreateAttribute( s_strTagNames[iPort], strBuffer );
        }

        // Append the new node
        pXMLArea->AppendChild( pXMLNode );
    }

    // Done !
}

Void SkyArea::_PreComputeValues()
{
    m_fInvNodeCount = MathFn->Invert( (Scalar)m_iNodeCount );

    m_iTotalCost = 0;
    m_iMinNodeCost = 0xffffffff;
    m_iMaxNodeCost = 0;

    m_iPrestigeNodeCount = 0;
    m_iTotalPrestige = 0;
    UInt iTotalPrestigeCost = 0;

    m_iProficiencyNodeCount = 0;
    m_iTotalProficiency = 0;
    UInt iTotalProficiencyCost = 0;

    m_iGreatnessNodeCount = 0;
    m_iTotalGreatness = 0;
    UInt iTotalGreatnessCost = 0;

    for( UInt i = 0; i < m_iNodeCount; ++i ) {
        SkyNode * pNode = m_arrNodes[i];
        UInt iCost = pNode->GetCost();
        UInt iPrestige = pNode->GetPrestige();

        m_iTotalCost += iCost;
        if ( iCost < m_iMinNodeCost )
            m_iMinNodeCost = iCost;
        if ( iCost > m_iMaxNodeCost )
            m_iMaxNodeCost = iCost;

        if ( iPrestige > 0 ) {
            ++m_iPrestigeNodeCount;
            m_iTotalPrestige += iPrestige;
            iTotalPrestigeCost += iCost;
        }
        if ( pNode->HasProficiency() ) {
            SkyProficiencyNode * pProficiencyNode = (SkyProficiencyNode*)pNode;
            ++m_iProficiencyNodeCount;
            m_iTotalProficiency += pProficiencyNode->GetProficiency();
            iTotalProficiencyCost += iCost;
        }
        if ( pNode->GetType() == SKYNODE_GREATNESS ) {
            SkyGreatnessNode * pGreatnessNode = (SkyGreatnessNode*)pNode;
            ++m_iGreatnessNodeCount;
            m_iTotalGreatness += pGreatnessNode->GetGreatness();
            iTotalGreatnessCost += iCost;
        }
    }

    m_fAverageNodeCost = m_fInvNodeCount * (Scalar)m_iTotalCost;

    m_fTotalPrestigeRatio = ( (Scalar)m_iTotalPrestige ) * MathFn->Invert( (Scalar)iTotalPrestigeCost );
    m_fTotalProficiencyRatio = ( (Scalar)m_iTotalProficiency ) * MathFn->Invert( (Scalar)iTotalProficiencyCost );
    m_fTotalGreatnessRatio = ( (Scalar)m_iTotalGreatness ) * MathFn->Invert( (Scalar)iTotalGreatnessCost );
}
Void SkyArea::_PreComputePaths()
{
    // Half Paths
    ///////////////////////////////////////////

    // Cross Paths
    ///////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////
// SkyGraph implementation
SkyGraph::SkyGraph()
{   
    m_iAreaCount = 0;

    for( UInt i = 0; i < SKYGRAPH_MAX_AREAS; ++i )
        m_arrAreas[i] = NULL;
}
SkyGraph::~SkyGraph()
{
    for( UInt i = 0; i < m_iAreaCount; ++i ) {
        if (  m_arrAreas[i] != NULL ) {
            Delete( m_arrAreas[i] );
            m_arrAreas[i] = NULL;
        }
    }
}

Void SkyGraph::LoadFromXML( const GChar * strFilename )
{
    // Open XML document
    XMLDocument * pDoc = XMLDocument::CreateDocument( strFilename );
    Assert( pDoc != NULL );

    // Get root node
    XMLNode * pXMLAtlas = (XMLNode*)pDoc;
    Assert( StringFn->Cmp(pXMLAtlas->GetTagName(), TEXT("SkyAtlas")) == 0 );

    // Pass 1 : Load all areas
    m_iAreaCount = pXMLAtlas->GetChildCount();
    for( UInt i = 0; i < m_iAreaCount; ++i ) {
        XMLNode * pXMLArea = pXMLAtlas->GetChild( i );
        Assert( pXMLArea != NULL );

        // Grab index attribute
        XMLAttribute * pAttrib = pXMLArea->GetAttribute( TEXT("index") );
        Assert( pAttrib != NULL );
        UInt iIndex = (UInt)( StringFn->ToUInt( pAttrib->GetValue() ) );

        // Grab position attribute
        pAttrib = pXMLArea->GetAttribute( TEXT("position") );
        Assert( pAttrib != NULL );
        const GChar * strPosition = pAttrib->GetValue();

        Scalar fX = SCALAR_INFINITE, fY = SCALAR_INFINITE;
        StringFn->Scan( strPosition, TEXT("%f,%f"), &fX, &fY );
        Assert( fX != SCALAR_INFINITE && fY != SCALAR_INFINITE );

        // Create area at iIndex
        SkyArea * pNewArea = New SkyArea( iIndex, Vertex2(fX,fY) );
        m_arrAreas[iIndex] = pNewArea;

        // Load area nodes
        pNewArea->_LoadFromXML( pXMLArea );

        // Grab doors attribute
        pAttrib = pXMLArea->GetAttribute( TEXT("doors") );
        Assert( pAttrib != NULL );
        const GChar * strDoors = pAttrib->GetValue();

        Int iEDoor = -1;
        Int iNDoor = -1;
        Int iWDoor = -1;
        Int iSDoor = -1;
        StringFn->Scan( strDoors, TEXT("%n,%n,%n,%n"), &iEDoor, &iNDoor, &iWDoor, &iSDoor );

        // Setup door nodes
        pNewArea->SetDoorNode( SKYGRAPH_EAST, (UInt)iEDoor );
        pNewArea->SetDoorNode( SKYGRAPH_NORTH, (UInt)iNDoor );
        pNewArea->SetDoorNode( SKYGRAPH_WEST, (UInt)iWDoor );
        pNewArea->SetDoorNode( SKYGRAPH_SOUTH, (UInt)iSDoor );
    }

    // Pass 2 : Area connections
    for( UInt i = 0; i < m_iAreaCount; ++i ) {
        XMLNode * pXMLArea = pXMLAtlas->GetChild( i );
        Assert( pXMLArea != NULL );

        // Grab index attribute
        XMLAttribute * pAttrib = pXMLArea->GetAttribute( TEXT("index") );
        Assert( pAttrib != NULL );
        UInt iIndex = (UInt)( StringFn->ToUInt( pAttrib->GetValue() ) );

        // Pick loaded area
        SkyArea * pLoadedArea = m_arrAreas[iIndex];

        // East-connection
        if ( pXMLArea->HasAttribute(TEXT("ELink")) ) {
            // Grab ELink attribute
            pAttrib = pXMLArea->GetAttribute( TEXT("ELink") );
            Assert( pAttrib != NULL );
            const GChar * strLink = pAttrib->GetValue();

            UInt iTargetIndex = INVALID_OFFSET;
            GChar strTargetPort[2];
            StringFn->Scan( strLink, TEXT("%un,%s"), &iTargetIndex, strTargetPort, 2 );
            Assert( iTargetIndex != INVALID_OFFSET );
            Assert( m_arrAreas[iTargetIndex] != NULL );

            // Convert target port
            SkyGraphPort iTargetPort = SKYGRAPH_PORT_COUNT;
            if ( strTargetPort[0] == TEXT('E') )
                iTargetPort = SKYGRAPH_EAST;
            else if ( strTargetPort[0] == TEXT('N') )
                iTargetPort = SKYGRAPH_NORTH;
            else if ( strTargetPort[0] == TEXT('W') )
                iTargetPort = SKYGRAPH_WEST;
            else if ( strTargetPort[0] == TEXT('S') )
                iTargetPort = SKYGRAPH_SOUTH;
            Assert( iTargetPort != SKYGRAPH_PORT_COUNT );

            pLoadedArea->Connect( SKYGRAPH_EAST, m_arrAreas[iTargetIndex], iTargetPort );
        }

        // North-connection
        if ( pXMLArea->HasAttribute(TEXT("NLink")) ) {
            // Grab NLink attribute
            pAttrib = pXMLArea->GetAttribute( TEXT("NLink") );
            Assert( pAttrib != NULL );
            const GChar * strLink = pAttrib->GetValue();

            UInt iTargetIndex = INVALID_OFFSET;
            GChar strTargetPort[2];
            StringFn->Scan( strLink, TEXT("%un,%s"), &iTargetIndex, strTargetPort, 2 );
            Assert( iTargetIndex != INVALID_OFFSET );
            Assert( m_arrAreas[iTargetIndex] != NULL );

            // Convert target port
            SkyGraphPort iTargetPort = SKYGRAPH_PORT_COUNT;
            if ( strTargetPort[0] == TEXT('E') )
                iTargetPort = SKYGRAPH_EAST;
            else if ( strTargetPort[0] == TEXT('N') )
                iTargetPort = SKYGRAPH_NORTH;
            else if ( strTargetPort[0] == TEXT('W') )
                iTargetPort = SKYGRAPH_WEST;
            else if ( strTargetPort[0] == TEXT('S') )
                iTargetPort = SKYGRAPH_SOUTH;
            Assert( iTargetPort != SKYGRAPH_PORT_COUNT );

            pLoadedArea->Connect( SKYGRAPH_NORTH, m_arrAreas[iTargetIndex], iTargetPort );
        }

        // West-connection
        if ( pXMLArea->HasAttribute(TEXT("WLink")) ) {
            // Grab WLink attribute
            pAttrib = pXMLArea->GetAttribute( TEXT("WLink") );
            Assert( pAttrib != NULL );
            const GChar * strLink = pAttrib->GetValue();

            UInt iTargetIndex = INVALID_OFFSET;
            GChar strTargetPort[2];
            StringFn->Scan( strLink, TEXT("%un,%s"), &iTargetIndex, strTargetPort, 2 );
            Assert( iTargetIndex != INVALID_OFFSET );
            Assert( m_arrAreas[iTargetIndex] != NULL );

            // Convert target port
            SkyGraphPort iTargetPort = SKYGRAPH_PORT_COUNT;
            if ( strTargetPort[0] == TEXT('E') )
                iTargetPort = SKYGRAPH_EAST;
            else if ( strTargetPort[0] == TEXT('N') )
                iTargetPort = SKYGRAPH_NORTH;
            else if ( strTargetPort[0] == TEXT('W') )
                iTargetPort = SKYGRAPH_WEST;
            else if ( strTargetPort[0] == TEXT('S') )
                iTargetPort = SKYGRAPH_SOUTH;
            Assert( iTargetPort != SKYGRAPH_PORT_COUNT );

            pLoadedArea->Connect( SKYGRAPH_WEST, m_arrAreas[iTargetIndex], iTargetPort );
        }

        // South-connection
        if ( pXMLArea->HasAttribute(TEXT("SLink")) ) {
            // Grab SLink attribute
            pAttrib = pXMLArea->GetAttribute( TEXT("SLink") );
            Assert( pAttrib != NULL );
            const GChar * strLink = pAttrib->GetValue();

            UInt iTargetIndex = INVALID_OFFSET;
            GChar strTargetPort[2];
            StringFn->Scan( strLink, TEXT("%un,%s"), &iTargetIndex, strTargetPort, 2 );
            Assert( iTargetIndex != INVALID_OFFSET );
            Assert( m_arrAreas[iTargetIndex] != NULL );

            // Convert target port
            SkyGraphPort iTargetPort = SKYGRAPH_PORT_COUNT;
            if ( strTargetPort[0] == TEXT('E') )
                iTargetPort = SKYGRAPH_EAST;
            else if ( strTargetPort[0] == TEXT('N') )
                iTargetPort = SKYGRAPH_NORTH;
            else if ( strTargetPort[0] == TEXT('W') )
                iTargetPort = SKYGRAPH_WEST;
            else if ( strTargetPort[0] == TEXT('S') )
                iTargetPort = SKYGRAPH_SOUTH;
            Assert( iTargetPort != SKYGRAPH_PORT_COUNT );

            pLoadedArea->Connect( SKYGRAPH_SOUTH, m_arrAreas[iTargetIndex], iTargetPort );
        }
    }

    // Release XML document
    XMLDocument::DestroyDocument( pDoc );
    pDoc = NULL;

    // Pre-Computing
    _PreComputeAll();

    // Done !
}
Void SkyGraph::SaveToXML( const GChar * strFilename ) const
{
    GChar strBuffer[64];

    // Create XML document
    XMLDocument * pDoc = XMLDocument::CreateDocument( TEXT("xml"), TEXT("1.0"), TEXT("utf-8") );

    // Create root node
    XMLNode * pXMLAtlas = XMLDocument::CreateNode( TEXT("SkyAtlas"), false );
    Assert( pXMLAtlas != NULL );

    pDoc->AppendChild( pXMLAtlas );

    // Enumerate all areas
    for( UInt i = 0; i < m_iAreaCount; ++i ) {
        SkyArea * pArea = m_arrAreas[i];

        // Create an xml node
        XMLNode * pXMLArea = XMLDocument::CreateNode( TEXT("SkyArea"), false );
        Assert( pXMLArea != NULL );

        // Add attributes
            // index
        StringFn->FromUInt( strBuffer, i );
        pXMLArea->CreateAttribute( TEXT("index"), strBuffer );

            // position
        StringFn->Format( strBuffer, TEXT("%f,%f"), pArea->GetPosition().X, pArea->GetPosition().Y );
        pXMLArea->CreateAttribute( TEXT("position"), strBuffer );

            // doors
        UInt iEDoor, iNDoor, iWDoor, iSDoor;
        pArea->GetDoorNode( SKYGRAPH_EAST, &iEDoor );
        pArea->GetDoorNode( SKYGRAPH_NORTH, &iNDoor );
        pArea->GetDoorNode( SKYGRAPH_WEST, &iWDoor );
        pArea->GetDoorNode( SKYGRAPH_SOUTH, &iSDoor );
        StringFn->Format( strBuffer, TEXT("%d,%d,%d,%d"), iEDoor, iNDoor, iWDoor, iSDoor );
        pXMLArea->CreateAttribute( TEXT("doors"), strBuffer );

            // links
        static const GChar * s_strTagNames[4] = {
            TEXT("ELink"), TEXT("NLink"), TEXT("WLink"), TEXT("SLink")
        };
        static const GChar * s_strPortLabels[4] = {
            TEXT("E"), TEXT("N"), TEXT("W"), TEXT("S")
        };
        for( UInt j = 0; j < SKYGRAPH_PORT_COUNT; ++j ) {
            SkyGraphPort iPort = (SkyGraphPort)j;
            SkyGraphPort iTargetPort = SKYGRAPH_PORT_COUNT;

            SkyArea * pTargetArea = pArea->GetLink( iPort, &iTargetPort );
            if ( pTargetArea == NULL )
                continue;
            Assert( iTargetPort != SKYGRAPH_PORT_COUNT );

            StringFn->Format( strBuffer, TEXT("%ud,%s"), pTargetArea->GetAreaIndex(), s_strPortLabels[iTargetPort] );
            pXMLArea->CreateAttribute( s_strTagNames[iPort], strBuffer );
        }

        // Save area content
        pArea->_SaveToXML( pXMLArea );

        // Append to atlas
        pXMLAtlas->AppendChild( pXMLArea );
    }

    // Render XML document to file
    Bool bOk = pDoc->Render( strFilename );
    Assert( bOk );

    // Release XML document
    XMLDocument::DestroyDocument( pDoc );
    pDoc = NULL;
}

/////////////////////////////////////////////////////////////////////////////////

Void SkyGraph::_PreComputeAll()
{
    for( UInt i = 0; i < m_iAreaCount; ++i ) {
        SkyArea * pArea = m_arrAreas[i];
        pArea->_PreComputeValues();
        pArea->_PreComputePaths();
    }
}
