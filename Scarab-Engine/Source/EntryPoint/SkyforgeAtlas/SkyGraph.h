/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SkyforgeAtlas/SkyGraph.h
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
// Known Bugs : XML schema for SkyAtlas.xml :
// <?xml version="1.0" encoding="utf-8"?>
// <SkyAtlas>
//     <SkyArea index="%un" doors="%un,%un,%un,%un" LinkE="%s" LinkN="%s" LinkW="%s" LinkS="%s">
//         <SkyNode index="%un" type="%s" color="%s" cost="%un" (prestige="%un") (proficiency="%un") statvalue="%un|%s" (LinkE="%s") (LinkN="%s") (LinkW="%s") (LinkS="%s")/>
//         ...
//     </SkyArea>
//     ...
// </SkyAtlas>
// [EOF]
//
// Area Doors = "E(%un),N(%un),W(%un),S(%un)"
// Area LinkX = "areaindex(%un),targetport(%c)", targetport = { E, N, W, S }
// Types = { MIGHT, STAMINA, STRENGTH, LUCK, SPIRIT, VALOR, GREATNESS, TALENT, CLASS }
// Colors = { RED, GREEN, BLUE, LOCK, CLASS, DIVINEFORM }
// StatValues for CLASS nodes = { CRYOMANCER, PALADIN, LIGHTBRINGER, BERSERKER, ARCHER, SLAYER, NECROMANCER, KINETIC, GUNNER, KNIGHT, MONK, ALCHIMIST, WITCH, DIVINEFORM }
// Node LinkX = "nodeindex(%un),targetport(%c)", targetport = { E, N, W, S }
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENTRYPOINT_SKYFORGEATLAS_SKYGRAPH_H
#define SCARAB_ENTRYPOINT_SKYFORGEATLAS_SKYGRAPH_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Engine/Application/ApplicationManager.h"

#include "../../Lib/Datastruct/Array/Array.h"
#include "../../Lib/Datastruct/Array/RingBuffer.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define SKYGRAPH_XML_FILE TEXT("SkyAtlas.xml")

#define SKYGRAPH_MAX_AREAS 256
#define SKYGRAPH_MAX_NODES_PER_AREA 32

enum SkyGraphPort {
    SKYGRAPH_EAST = 0,
    SKYGRAPH_NORTH,
    SKYGRAPH_WEST,
    SKYGRAPH_SOUTH,

    SKYGRAPH_PORT_COUNT
};

enum SkyNodeType {
    SKYNODE_MIGHT = 0,
    SKYNODE_STAMINA,

    SKYNODE_STRENGTH,
    SKYNODE_LUCK,
    SKYNODE_SPIRIT,
    SKYNODE_VALOR,

    SKYNODE_GREATNESS,

    SKYNODE_TALENT,
    SKYNODE_CLASS
};
enum SkyColor {
    SKYCOLOR_RED = 0,
    SKYCOLOR_GREEN,
    SKYCOLOR_BLUE,

    SKYCOLOR_LOCK,

    SKYCOLOR_CLASS,
    SKYCOLOR_DIVINE_FORM,

    SKYCOLOR_COUNT
};

enum SkyTalent {
    // Ether slots
    SKYTALENT_SLOT_SUN = 0,
    SKYTALENT_SLOT_HET,
    SKYTALENT_SLOT_COPH,

    // Offensive talents
    SKYTALENT_ULTIMATE_STRENGTH,      // Base damage strength influence +10%/Node
    SKYTALENT_MAXIMUM_RECOIL,         // Critical damage +10%/Node
    SKYTALENT_IMPULSE_INTENSITY,      // Impulse damage +10%/Node
    SKYTALENT_MOBILISATION,           // Bonus damage +10%/Node

    SKYTALENT_LUCKY_SHOT,             // +1% Crit Chance / 2s, resets on crit (cap = 50%)
    SKYTALENT_DOUBLE_DISCHARGE,       // On Impulse recharge = Chance to have double charge +12%/Node

    SKYTALENT_LACERATED_WOUNDS,       // OnCrit = DOT(Total damage = Luck)
    SKYTALENT_PAINFUL_SPASMS,         // OnImpulse = DOT(Total damage = Spirit)

    SKYTALENT_WARRIORS_CREED,         // Incoming damage +5% / Damage dealt +5%

    SKYTALENT_ELEMENT_OF_SURPRISE,    // First 8s in fight = Damage dealt +12%/Node
    SKYTALENT_RAPID_ATTACK,           // First 3s in fight = Damage dealt +6%/Node

    SKYTALENT_SLOWING_ATTACKS,        // OnHit = Slow(10%/Node, 3s)
    SKYTALENT_IMPACT,                 // OnCrit = Stun(1.5s, CD 7s/target)
    SKYTALENT_GLORIOUS_TRIUMPH,       // OnFinisher = Incoming damage -10% / Damage dealt +10%, 10s
    SKYTALENT_MARK_OF_DEATH,          // OnHit(??%) = StackDmg(??s, damage for 20/30% of stack) (the ultimate one !)

    SKYTALENT_SUMMON_OCULAT,          // OnCast(??%) = Summon(Oculat, 18/25s)

    SKYTALENT_SHOCK_PAIN,             // OnCast(??%), Trigger = Stun(3s, CD 25/20s)
    SKYTALENT_SHOCK_PARALYZE,         // OnCast(??%), Trigger = Stun(3s, CD 25/20s)

    SKYTALENT_WAVE_DAMAGE,            // OnCast(??%), Trigger = AOE(Dmg(??-??), 8 yards, CD 25/20s)
    SKYTALENT_WAVE_SLOW,              // OnCast(??%), Trigger = AOE(Slow(50%, 4s), 8 yards, CD 25/20s)

    SKYTALENT_RUTHLESSNESS,           // Targets with (Slow, Stun/Fear/Blind/Immobilize) = Damage dealt (+2%/Node, +6%/Node)

    // Defensive talents
    SKYTALENT_TACTICAL_DEFENSE,       // Melee/Ranged defense = +5%/Node
    SKYTALENT_PROTECTORS_CREED,       // Incoming damage -5% / Damage dealt -5%
    SKYTALENT_BALANCE,                // When fighting 3+ ennemies = Incoming damage -6%/Node

    SKYTALENT_ADRENALINE_SURGE,       // OnDmgTaken( "(1 - HP/MaxHP)%" ) = Speed(+15%/Node, 5s)
    SKYTALENT_LIFE_ENERGY,            // OnHealOrb = Speed +30%, Incoming damage -30%, 3s

    SKYTALENT_IMPULSE_BARRIER,        // OnImpulse = Shield 3s, absorb damage 3%/Node of MaxHP

    SKYTALENT_BONUS_DASH,             // Dash Charges +1/Node
    SKYTALENT_CLEVER_MANOEUVER,       // OnDash = Shield 3s, absorb damage 5% of MaxHP
    SKYTALENT_INERTIA,                // OnDash = Incoming damage -12%/Node, 3s
    SKYTALENT_BREAKOUT,               // OnDash = AOE(Stun(1.5s), ?? yards, CD 21/14s)

    SKYTALENT_DESPERATE_RESISTANCE,   // Slow effects on character = -15%/Node
    SKYTALENT_FORCED_RESPITE,         // Slow effects on character = +1 Dash charge, 20%

    SKYTALENT_DEFENSE_REFLEX,         // Character with (Slow, Stun/Fear/Blind/Immobilize) = Incoming damage (-3%/Node, -10%/Node)

    // Misc talents
    SKYTALENT_CONSUMABLES_EFFICIENCY, // Consumables = Duration +12/25%, Heal +12/25%, Damage +12/25%
    SKYTALENT_RECHARGING_CONSUMABLES, // Consumables = CD -15/30%

    SKYTALENT_COUNT
};
static const GChar * _SkyTalentConvert( SkyTalent iTalent );
static SkyTalent _SkyTalentConvert( const GChar * strTalent );

enum SkyClass {
    // Class talents
    SKYCLASS_CRYOMANCER = 0,         // Control effects on character = -15% / -30% if HP < 50%
    SKYCLASS_PALADIN,                // OnDmgTaken(>10%MaxHP) = Absorbed, Immune 4s, CD 70s
    SKYCLASS_LIGHTBRINGER,           // OutOfCombat = Shield 5% of MaxHP, 3 stacks, restores in combat if no damage for 6s
    SKYCLASS_BERSERKER,              // Berserk = ThirstForBattle,UnbridledRage CD -15% / Others = OnDmgTaken( "(1 - HP/MaxHP)%" ) = Damage dealt +20%, 8s
    SKYCLASS_ARCHER,                 // Targets with HP>50% = Damage dealt +15%
    SKYCLASS_SLAYER,                 // If not hit for 5s = Crit chance +7%
    SKYCLASS_NECROMANCER,            // OnCast(??%) = Summon(Ghostly Viper, 20s)
    SKYCLASS_KINETIC,                // OnImpulse = Damage dealt +12%, 6s
    SKYCLASS_GUNNER,                 // InCombat, Not moving 5s = Summon 3 Drone Sentries, Vanish if moving more than 8yards, Attack closest in 9yards, Dmg+Slow(75%,5s), CD 20s
    SKYCLASS_KNIGHT,                 // Control effects on character = -15% / -30% if HP < 50%
    SKYCLASS_MONK,                   // Targets with Stun = Damage dealt +20%
    SKYCLASS_ALCHIMIST,              // OnCast(??%) = Damage dealt +15%, 10s
    SKYCLASS_WITCH,                  // Witch = Taint +25% damage / Others = OnCast(??%) = DOT(Dmg each sec, 6s)

    // The holy one !
    SKYCLASS_DIVINE_FORM,            // Unlocks god powers !

    SKYCLASS_COUNT
};
static const GChar * _SkyClassConvert( SkyClass iClass );
static SkyClass _SkyClassConvert( const GChar * strClass );

enum SkyHalfPath {
    SKYPATH_HALF_E = 0,
    SKYPATH_HALF_N,
    SKYPATH_HALF_W,
    SKYPATH_HALF_S,

    SKYPATH_HALF_COUNT
};
enum SkyCrossPath {
    SKYPATH_CROSS_EN = 0,
    SKYPATH_CROSS_EW,
    SKYPATH_CROSS_ES,

    SKYPATH_CROSS_NE,
    SKYPATH_CROSS_NW,
    SKYPATH_CROSS_NS,

    SKYPATH_CROSS_WE,
    SKYPATH_CROSS_WN,
    SKYPATH_CROSS_WS,

    SKYPATH_CROSS_SE,
    SKYPATH_CROSS_SN,
    SKYPATH_CROSS_SW,

    SKYPATH_CROSS_COUNT
};

    // Prototypes
class SkyNode;
class SkyArea;
class SkyGraph;

/////////////////////////////////////////////////////////////////////////////////
// The SkyNode class
class SkyNode
{
public:
    typedef struct _link {
        SkyNode * pTargetNode;
        SkyGraphPort iTargetPort;
    } Link;
public:
    SkyNode( SkyColor iColor, UInt iCost, UInt iPrestige );
    virtual ~SkyNode();

    // Node type & color
    virtual SkyNodeType GetType() const = 0;
    inline SkyColor GetColor() const;

    inline Bool HasProficiency() const;
    inline Bool IsLocked() const;

    // Linkage access
    inline SkyArea * GetParentArea( UInt * outNodeIndex = NULL ) const;

    inline SkyNode * GetLink( SkyGraphPort iPort, SkyGraphPort * outTargetPort = NULL ) const;
    inline UInt GetLinkCount() const;

    inline Void * GetHolder() const;
    inline Void SetHolder( Void * pHolder );

    // Position access
    inline const Vertex2 & GetPosition() const;

    // Cost access
    inline UInt GetCost() const;

    // Prestige access
    inline UInt GetPrestige() const;
    inline Scalar GetPrestigeRatio() const;

    // StatValue access
    virtual UInt GetStatValue() const = 0;

    // Node operations
    inline Void SetParentArea( SkyArea * pParentArea, UInt iNodeIndex );
    inline Void SetPosition( const Vertex2 & vPosition );

    Void Connect( SkyGraphPort iPort, SkyNode * pTargetNode, SkyGraphPort iTargetPort );
    Void Disconnect( SkyGraphPort iPort );

protected:
    // Linkage data
    SkyArea * m_pParentArea;
    UInt m_iNodeIndex; // Node's index in his area

    Link m_arrLinks[SKYGRAPH_PORT_COUNT];
    UInt m_iLinkCount;

    Void * m_pHolder;

    // Node data
    Vertex2 m_vPosition;

    SkyColor m_iColor;
    UInt m_iCost;
    Scalar m_fInvCost;

    UInt m_iPrestige;
    Scalar m_fPrestigeRatio;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkyProficiencyNode class
class SkyProficiencyNode : public SkyNode
{
public:
    SkyProficiencyNode( SkyColor iColor, UInt iCost, UInt iPrestige, UInt iProficiency );
    virtual ~SkyProficiencyNode();

    // Proficiency access
    inline UInt GetProficiency() const;
    inline Scalar GetProficiencyRatio() const;

protected:
    // Node data
    UInt m_iProficiency;

    // Pre-computed values
    Scalar m_fProficiencyRatio;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkyMightNode class
class SkyMightNode : public SkyProficiencyNode
{
public:
    SkyMightNode( UInt iCost, UInt iPrestige, UInt iProficiency, UInt iMight, Bool bIsLocked );
    virtual ~SkyMightNode();

    // Node type
    inline virtual SkyNodeType GetType() const;

    // Might access
    inline virtual UInt GetStatValue() const;

    inline UInt GetMight() const;
    inline Scalar GetMightRatio() const;

protected:
    // Node data
    UInt m_iMight;

    // Pre-computed values
    Scalar m_fMightRatio;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkyStaminaNode class
class SkyStaminaNode : public SkyProficiencyNode
{
public:
    SkyStaminaNode( UInt iCost, UInt iPrestige, UInt iProficiency, UInt iStamina, Bool bIsLocked );
    virtual ~SkyStaminaNode();

    // Node type
    inline virtual SkyNodeType GetType() const;

    // Stamina access
    inline virtual UInt GetStatValue() const;

    inline UInt GetStamina() const;
    inline Scalar GetStaminaRatio() const;

protected:
    // Node data
    UInt m_iStamina;

    // Pre-computed values
    Scalar m_fStaminaRatio;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkyStrengthNode class
class SkyStrengthNode : public SkyNode
{
public:
    SkyStrengthNode( UInt iCost, UInt iPrestige, UInt iStrength, Bool bIsLocked );
    virtual ~SkyStrengthNode();

    // Node type
    inline virtual SkyNodeType GetType() const;

    // Strength access
    inline virtual UInt GetStatValue() const;

    inline UInt GetStrength() const;
    inline Scalar GetStrengthRatio() const;

protected:
    // Node data
    UInt m_iStrength;

    // Pre-computed values
    Scalar m_fStrengthRatio;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkyLuckNode class
class SkyLuckNode : public SkyNode
{
public:
    SkyLuckNode( UInt iCost, UInt iPrestige, UInt iLuck, Bool bIsLocked );
    virtual ~SkyLuckNode();

    // Node type
    inline virtual SkyNodeType GetType() const;

    // Luck access
    inline virtual UInt GetStatValue() const;

    inline UInt GetLuck() const;
    inline Scalar GetLuckRatio() const;

protected:
    // Node data
    UInt m_iLuck;

    // Pre-computed values
    Scalar m_fLuckRatio;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkySpiritNode class
class SkySpiritNode : public SkyNode
{
public:
    SkySpiritNode( UInt iCost, UInt iPrestige, UInt iSpirit, Bool bIsLocked );
    virtual ~SkySpiritNode();

    // Node type
    inline virtual SkyNodeType GetType() const;

    // Spirit access
    inline virtual UInt GetStatValue() const;

    inline UInt GetSpirit() const;
    inline Scalar GetSpiritRatio() const;

protected:
    // Node data
    UInt m_iSpirit;

    // Pre-computed values
    Scalar m_fSpiritRatio;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkyValorNode class
class SkyValorNode : public SkyNode
{
public:
    SkyValorNode( UInt iCost, UInt iPrestige, UInt iValor, Bool bIsLocked );
    virtual ~SkyValorNode();

    // Node type
    inline virtual SkyNodeType GetType() const;

    // Valor access
    inline virtual UInt GetStatValue() const;

    inline UInt GetValor() const;
    inline Scalar GetValorRatio() const;

protected:
    // Node data
    UInt m_iValor;

    // Pre-computed values
    Scalar m_fValorRatio;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkyGreatnessNode class
class SkyGreatnessNode : public SkyNode
{
public:
    SkyGreatnessNode( UInt iCost, UInt iPrestige, UInt iGreatness );
    virtual ~SkyGreatnessNode();

    // Node type
    inline virtual SkyNodeType GetType() const;

    // Greatness access
    inline virtual UInt GetStatValue() const;

    inline UInt GetGreatness() const;
    inline Scalar GetGreatnessRatio() const;

protected:
    // Node data
    UInt m_iGreatness;

    // Pre-computed values
    Scalar m_fGreatnessRatio;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkyTalentNode class
class SkyTalentNode : public SkyNode
{
public:
    SkyTalentNode( UInt iCost, UInt iPrestige, SkyTalent iTalent );
    virtual ~SkyTalentNode();

    // Node type
    inline virtual SkyNodeType GetType() const;

    // Talent access
    inline virtual UInt GetStatValue() const;

    inline SkyTalent GetTalent() const;

protected:
    // Node data
    SkyTalent m_iTalent;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkyClassNode class
class SkyClassNode : public SkyNode
{
public:
    SkyClassNode( UInt iCost, SkyClass iClass );
    virtual ~SkyClassNode();

    // Node type
    inline virtual SkyNodeType GetType() const;

    inline Bool IsDivineForm() const;

    // Class access
    inline virtual UInt GetStatValue() const;

    inline SkyClass GetClass() const;

protected:
    // Node data
    SkyClass m_iClass;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkyArea class
class SkyArea
{
public:
    typedef struct _link {
        SkyArea * pTargetArea;
        SkyGraphPort iTargetPort;
    } Link;
    //class Path {
    //    public:
    //        Path();
    //        ~Path();
    //    private:
    //        UInt m_iPathLength;
    //        UInt m_iPathTotalCost;
    //        UInt m_iPathCost[SKYCOLOR_COUNT];
    //};
public:
    SkyArea( UInt iAreaIndex, const Vertex2 & vPosition );
    virtual ~SkyArea();

    // Linkage access
    inline UInt GetAreaIndex() const;

    inline SkyArea * GetLink( SkyGraphPort iPort, SkyGraphPort * outTargetPort = NULL ) const;
    inline UInt GetLinkCount() const;

    inline Void * GetHolder() const;
    inline Void SetHolder( Void * pHolder );

    // Position access
    inline const Vertex2 & GetPosition() const;

    // Nodes access
    inline UInt GetNodeCount() const;
    inline SkyNode * GetNode( UInt iIndex ) const;

    inline SkyNode * GetDoorNode( SkyGraphPort iPort, UInt * outNodeIndex = NULL ) const;
    inline SkyNode * GetCenterNode() const;

    // Statistics access
    inline UInt GetTotalCost() const;

    inline UInt GetMinNodeCost() const;
    inline UInt GetMaxNodeCost() const;
    inline Scalar GetAverageNodeCost() const;

    inline UInt GetTotalPrestige() const;
    inline Scalar GetTotalPrestigeRatio() const;

    inline UInt GetTotalProficiency() const;
    inline Scalar GetTotalProficiencyRatio() const;

    inline UInt GetTotalGreatness() const;
    inline Scalar GetTotalGreatnessRatio() const;

    // Area operations
    Void SetDoorNode( SkyGraphPort iPort, UInt iDoorNode );

    Void Connect( SkyGraphPort iPort, SkyArea * pTargetArea, SkyGraphPort iTargetPort );
    Void Disconnect( SkyGraphPort iPort );

    // Traversal markups (A traversal MUST visit all nodes !)
    inline UInt GetTraversalMark() const;

    inline Bool IsVisited( UInt iBaseMark ) const;
    inline Void MarkVisited();
    inline Void MarkUnVisited();

protected:
    // Helper methods
    friend class SkyGraph;
    Void _LoadFromXML( const XMLNode * pXMLArea );
    Void _SaveToXML( XMLNode * pXMLArea ) const;

    Void _PreComputeValues();
    Void _PreComputePaths();

    UInt m_iTraversalMark;

    // Linkage data
    UInt m_iAreaIndex;

    Link m_arrLinks[SKYGRAPH_PORT_COUNT];
    UInt m_iLinkCount;

    Void * m_pHolder;

    // Area data
    Vertex2 m_vPosition;

    UInt m_iNodeCount;
    SkyNode * m_arrNodes[SKYGRAPH_MAX_NODES_PER_AREA];

    UInt m_arrDoorNode[SKYGRAPH_PORT_COUNT]; // Indices in m_arrNodes,
    UInt m_iCenterNode;                      // this area's node array

    // Pre-computed values
    Scalar m_fInvNodeCount;

    UInt m_iTotalCost;

    UInt m_iMinNodeCost;             // Min node cost in this area
    UInt m_iMaxNodeCost;             // Max node cost in this area
    Scalar m_fAverageNodeCost;       // Average node cost in this area

    UInt m_iPrestigeNodeCount;       // Number of nodes providing prestige in this area
    UInt m_iTotalPrestige;           // Total prestige available in this area
    Scalar m_fTotalPrestigeRatio;    // Prestige/Cost efficiency of this area

    UInt m_iProficiencyNodeCount;    // Number of nodes providing proficiency in this area
    UInt m_iTotalProficiency;        // Total proficiency available in this area
    Scalar m_fTotalProficiencyRatio; // Proficiency/Cost efficiency of this area

    UInt m_iGreatnessNodeCount;      // Number of nodes providing greatness in this area
    UInt m_iTotalGreatness;          // Total greatness available in this area
    Scalar m_fTotalGreatnessRatio;   // Greatness/Cost efficiency of this area

    // Pre-computed paths
    //Path m_arrHalfPath[SKYPATH_HALF_COUNT];
    //Path m_arrCrossPath[SKYPATH_CROSS_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// The SkyGraph class
class SkyGraph
{
public:
    SkyGraph();
    ~SkyGraph();

    // Areas access
    inline UInt GetAreaCount() const;
    inline SkyArea * GetArea( UInt iIndex ) const;

    // XML file management
    Void LoadFromXML( const GChar * strFilename );
    Void SaveToXML( const GChar * strFilename ) const;

private:
    // Helper methods
    Void _PreComputeAll();

    // Graph structure
    UInt m_iAreaCount;
    SkyArea * m_arrAreas[SKYGRAPH_MAX_AREAS];

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SkyGraph.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SKYFORGEATLAS_SKYGRAPH_H

