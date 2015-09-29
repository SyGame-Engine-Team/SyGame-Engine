/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SkyforgeAtlas/Entities.cpp
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
// Includes
#include "Entities.h"

#include "Main.h"

/////////////////////////////////////////////////////////////////////////////////
// EntityCharacter implementation
EntityCharacter::EntityCharacter():
    WorldLeaf( TEXT("Character") )
{
    // Geometry
    const GeometryInstance * pGeometry = GraphicsFn->GetGeometryInstance( GEOMETRYID_CHARACTER );

    SetMesh( pGeometry->pMesh );

    // Effect instance
    Light * pLight = GraphicsFn->GetLight( LIGHTID_SUN );
    Material * pMaterial = GraphicsFn->GetMaterial( MATERIALID_CHARACTER );
    EffectLightMaterial * pEffect = (EffectLightMaterial*)( GraphicsFn->GetEffect(EFFECTID_CHARACTER) );

    m_pEffectCharacter = pEffect->CreateInstance( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, pLight, pMaterial );
    SetEffectInstance( m_pEffectCharacter );

    // Body
    KinematicBody * pBody = PhysicsFn->CreateKinematicBody( false, pGeometry->pShape, 1.0f );
    pBody->SetRestitution( 0.0f );
    pBody->SetFriction( 0.0f );
    pBody->SetRollingFriction( 0.0f );
    pBody->SetCollisionGroup( 0 ); // Disable collisions
    pBody->SetCollisionMask( 0 );  // for the character

    m_pController = PhysicsFn->CreateCharacterController( TEXT("CharacterController"), Vertex3::Null, Quaternion::Identity, Vector3::Null, Vector3::Null );
    m_pController->Enabled = true;
    m_pController->EnableForces( false );
    m_pController->SetMovementSpeed( 5.0f );
    pBody->AttachController( m_pController );

    SetBody( pBody );

    // Auto-Add to world
    WorldFn->AddChild( this );
}
EntityCharacter::~EntityCharacter()
{
    // Auto-Remove from world
    WorldFn->RemoveChild( GetName() );

    // Body
    ((KinematicBody*)m_pBody)->DetachMotionController();
    PhysicsFn->DestroyController( m_pController );
    m_pController = NULL;

    PhysicsFn->DestroyBody( m_pBody );
    SetBody( NULL );

    // Effect instance
    Effect::DestroyInstance( m_pEffectCharacter );
    m_pEffectCharacter = NULL;

    SetEffectInstance( NULL );

    // Geometry
    SetMesh( NULL );
}

/////////////////////////////////////////////////////////////////////////////////
// EntityArea implementation
EntityArea::EntityArea( SkyArea * pArea ):
    WorldLeaf( _MakeName(pArea) )
{
    // Area data
    m_pArea = pArea;
    m_pArea->SetHolder( this );

    // Geometry
    const GeometryInstance * pGeometry = GraphicsFn->GetGeometryInstance( GEOMETRYID_AREA );

    SetMesh( pGeometry->pMesh );

    // Effect instance
    Light * pLight = GraphicsFn->GetLight( LIGHTID_SUN );
    Material * pMaterial = GraphicsFn->GetMaterial( MATERIALID_AREA );
    EffectLightMaterial * pEffect = (EffectLightMaterial*)( GraphicsFn->GetEffect(EFFECTID_AREA) );

    m_pEffectArea = pEffect->CreateInstance( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, pLight, pMaterial );
    SetEffectInstance( m_pEffectArea );

    // State
    WorldTransform.SetTranslate( m_pArea->GetPosition().ToVector() );
}
EntityArea::~EntityArea()
{
    // Effect instance
    Effect::DestroyInstance( m_pEffectArea );
    m_pEffectArea = NULL;

    SetEffectInstance( NULL );

    // Geometry
    SetMesh( NULL );

    // Area data
    m_pArea->SetHolder( NULL );
}

Void EntityArea::GenerateStructure( WorldNode * outAreaContainer )
{
    SAApplication * pApp = (SAApplication*)( ApplicationFn->GetInstance() );

    // Area
    outAreaContainer->AddChild( this );

    // Nodes
    WorldNode * pNodeContainer = WorldFn->CreateNode( TEXT("NodeContainer") );
    for( UInt i = 0; i < m_pArea->GetNodeCount(); ++i ) {
        EntityNode * pEntNode = (EntityNode*)( m_pArea->GetNode(i)->GetHolder() );
        pNodeContainer->AddChild( pEntNode );
    }
    outAreaContainer->AddChild( pNodeContainer );

    // Node links
    WorldNode * pNodeLinkContainer = WorldFn->CreateNode( TEXT("NodeLinkContainer") );
    for( UInt i = 0; i < m_pArea->GetNodeCount(); ++i ) {
        SkyNode * pNode = m_pArea->GetNode(i);
        EntityNode * pEntNode = (EntityNode*)( pNode->GetHolder() );

        for( UInt k = 0; k < SKYGRAPH_PORT_COUNT; ++k ) {
            SkyGraphPort iPort = (SkyGraphPort)k;
            SkyNode * pTarget = pNode->GetLink(iPort);
            if ( pTarget == NULL )
                continue;
            if ( pTarget->GetParentArea() != m_pArea ) // skip area links at this point
                continue;
            EntityNode * pEntTarget = (EntityNode*)( pTarget->GetHolder() );

            EntityNodeLinkKey hKey(pEntNode,pEntTarget);
            EntityNodeLink * pLink = pApp->GetNodeLink( hKey );
            Assert( pLink != NULL );

            pNodeLinkContainer->AddChild( pLink );
        }
    }
    outAreaContainer->AddChild( pNodeLinkContainer );

    // Area links
    WorldNode * pAreaLinkContainer = WorldFn->CreateNode( TEXT("AreaLinkContainer") );
    for( UInt k = 0; k < SKYGRAPH_PORT_COUNT; ++k ) {
        SkyGraphPort iPort = (SkyGraphPort)k;
        SkyGraphPort iTargetPort;
        SkyArea * pTarget = m_pArea->GetLink( iPort, &iTargetPort );
        if ( pTarget == NULL )
            continue;
        SkyNode * pDoor = m_pArea->GetDoorNode(iPort);
        SkyNode * pTargetDoor = pTarget->GetDoorNode(iTargetPort);
        EntityNode * pEntDoor = (EntityNode*)( pDoor->GetHolder() );
        EntityNode * pEntTargetDoor = (EntityNode*)( pTargetDoor->GetHolder() );

        EntityNodeLinkKey hKey(pEntDoor,pEntTargetDoor);
        EntityNodeLink * pLink = pApp->GetNodeLink( hKey );
        Assert( pLink != NULL );

        if ( pLink->GetParent() == NULL )
            pAreaLinkContainer->AddChild( pLink );
    }
    outAreaContainer->AddChild( pAreaLinkContainer );
}

/////////////////////////////////////////////////////////////////////////////////

const GChar * EntityArea::_MakeName( SkyArea * pArea )
{
    static GChar s_strBuffer[64];
    StringFn->Format( s_strBuffer, TEXT("Area_%d"), pArea->GetAreaIndex() );
    return s_strBuffer;
}

/////////////////////////////////////////////////////////////////////////////////
// EntityNode implementation
EntityNode::EntityNode( SkyNode * pNode ):
    WorldLeaf( _MakeName(pNode) )
{
    // Node data
    m_pNode = pNode;
    m_pNode->SetHolder( this );

    // Geometry
    const GeometryInstance * pGeometry = GraphicsFn->GetGeometryInstance( GEOMETRYID_NODE );

    SetMesh( pGeometry->pMesh );

    // Effect instance
    Light * pLight = GraphicsFn->GetLight( LIGHTID_SUN );
    Material * pMaterial;
    switch( m_pNode->GetColor() ) {
        case SKYCOLOR_RED:         pMaterial = GraphicsFn->GetMaterial( MATERIALID_NODE_RED ); break;
        case SKYCOLOR_GREEN:       pMaterial = GraphicsFn->GetMaterial( MATERIALID_NODE_GREEN ); break;
        case SKYCOLOR_BLUE:        pMaterial = GraphicsFn->GetMaterial( MATERIALID_NODE_BLUE ); break;
        case SKYCOLOR_LOCK:        pMaterial = GraphicsFn->GetMaterial( MATERIALID_NODE_LOCK ); break;
        case SKYCOLOR_CLASS:       pMaterial = GraphicsFn->GetMaterial( MATERIALID_NODE_CLASS ); break;
        case SKYCOLOR_DIVINE_FORM: pMaterial = GraphicsFn->GetMaterial( MATERIALID_NODE_DIVINEFORM ); break;
        default: Assert( false ); break;
    }
    EffectLightMaterial * pEffect = (EffectLightMaterial*)( GraphicsFn->GetEffect(EFFECTID_NODE) );

    m_pEffectNode = pEffect->CreateInstance( EFFECTLIGHT_TECHNIQUE_POINT_PHONG, pLight, pMaterial );
    SetEffectInstance( m_pEffectNode );

    // State
    WorldTransform.SetTranslate( m_pNode->GetParentArea()->GetPosition().ToVector() + m_pNode->GetPosition().ToVector() );
}
EntityNode::~EntityNode()
{
    // Effect instance
    Effect::DestroyInstance( m_pEffectNode );
    m_pEffectNode = NULL;

    SetEffectInstance( NULL );

    // Geometry
    SetMesh( NULL );

    // Node data
    m_pNode->SetHolder( NULL );
}

/////////////////////////////////////////////////////////////////////////////////

const GChar * EntityNode::_MakeName( SkyNode * pNode )
{
    static GChar s_strBuffer[64];

    UInt iNodeIndex = INVALID_OFFSET;
    pNode->GetParentArea( &iNodeIndex );
    Assert( iNodeIndex != INVALID_OFFSET );

    StringFn->Format( s_strBuffer, TEXT("Node_%d"), iNodeIndex );
    return s_strBuffer;
}

/////////////////////////////////////////////////////////////////////////////////
// EntityNodeLinkKey implementation
EntityNodeLinkKey::EntityNodeLinkKey()
{
    pNodeA = NULL;
    pNodeB = NULL;
}
EntityNodeLinkKey::EntityNodeLinkKey( EntityNode * pA, EntityNode * pB )
{
    Assert( pA != pB );

    // Ensure unicity, in a cache-friendly way !
    if ( pA < pB ) {
        pNodeA = pA;
        pNodeB = pB;
    } else {
        pNodeA = pB;
        pNodeB = pA;
    }
}
EntityNodeLinkKey::EntityNodeLinkKey( const EntityNodeLinkKey & rhs )
{
    pNodeA = rhs.pNodeA;
    pNodeB = rhs.pNodeB;
}
EntityNodeLinkKey::~EntityNodeLinkKey()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// EntityNodeLink implementation
EntityNodeLink::EntityNodeLink( EntityNode * pNodeA, EntityNode * pNodeB ):
    WorldLeaf( _MakeName(pNodeA,pNodeB) )
{
    Assert( pNodeA != pNodeB );

    // Ensure unicity, in a cache-friendly way !
    if ( pNodeA < pNodeB ) {
        m_pNodeA = pNodeA;
        m_pNodeB = pNodeB;
    } else {
        m_pNodeA = pNodeB;
        m_pNodeB = pNodeA;
    }

    SkyNode * pA = m_pNodeA->GetNode();
    SkyNode * pB = m_pNodeB->GetNode();

    // Geometry
    const GeometryInstance * pGeometry = GraphicsFn->GetGeometryInstance( GEOMETRYID_LINK );

    SetMesh( pGeometry->pMesh );

    // Effect instance
    Material * pMaterial;
    if ( pA->GetParentArea() == pB->GetParentArea() )
        pMaterial = GraphicsFn->GetMaterial( MATERIALID_NODELINK );
    else
        pMaterial = GraphicsFn->GetMaterial( MATERIALID_AREALINK );
    EffectMaterial * pEffect = (EffectMaterial*)( GraphicsFn->GetEffect(EFFECTID_LINK) );

    m_pEffectNodeLink = pEffect->CreateInstance( pMaterial );
    SetEffectInstance( m_pEffectNodeLink );

    // State
    Vertex3 vPosA = pA->GetParentArea()->GetPosition() + pA->GetPosition().ToVector();
    Vertex3 vPosB = pB->GetParentArea()->GetPosition() + pB->GetPosition().ToVector();
    Vector3 vAB = ( vPosB - vPosA );
    Scalar fDistance = vAB.Normalize();

    Matrix3 matRot;
    matRot.MakeRotate( Vector3::eK, MathFn->ArcTan2(vAB.Y, vAB.X) );

    WorldTransform.SetRotate( matRot );
    WorldTransform.SetUniformScale( fDistance );
    WorldTransform.SetTranslate( vPosA.ToVector() );
}
EntityNodeLink::~EntityNodeLink()
{
    // Effect instance
    Effect::DestroyInstance( m_pEffectNodeLink );
    m_pEffectNodeLink = NULL;

    SetEffectInstance( NULL );

    // Geometry
    SetMesh( NULL );
}

/////////////////////////////////////////////////////////////////////////////////

const GChar * EntityNodeLink::_MakeName( EntityNode * pNodeA, EntityNode * pNodeB )
{
    static GChar s_strBuffer[64];
    UInt iAreaA = INVALID_OFFSET, iAreaB = INVALID_OFFSET;
    UInt iIndexA = INVALID_OFFSET, iIndexB = INVALID_OFFSET;
    if ( pNodeA < pNodeB ) {
        iAreaA = pNodeA->GetNode()->GetParentArea( &iIndexA )->GetAreaIndex();
        iAreaB = pNodeB->GetNode()->GetParentArea( &iIndexB )->GetAreaIndex();
    } else {
        iAreaA = pNodeB->GetNode()->GetParentArea( &iIndexA )->GetAreaIndex();
        iAreaB = pNodeA->GetNode()->GetParentArea( &iIndexB )->GetAreaIndex();
    }
    StringFn->Format( s_strBuffer, TEXT("NodeLink_%d:%d_%d:%d"), iAreaA, iIndexA, iAreaB, iIndexB );
    return s_strBuffer;
}

