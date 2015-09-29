/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Entities/CharEntity.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Entities : CharEntity
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
#include "CharEntity.h"

#include "EntityManager.h"
#include "../Gameplay/GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CharEntity implementation
CharEntity::CharEntity( BaseCharacter * pCharacter, const Vertex3 & vPosition ):
    Entity( pCharacter->GetName() ), m_vPathTarget()
{
    m_pCharacter = pCharacter;

    // Graphic data
    Void * arrParameters[2] = {
        GraphicsFn->GetLight( TEXT("Light_Sun") ),
        GraphicsFn->GetMaterial( TEXT("Material_Player") )
        //GraphicsFn->GetTexture( TEXT("Texture_Player") )
    };
    SetupGraphics( TEXT("Player"), EFFECTID_DEFAULT, arrParameters );

    // Animation data
    SetupKinematicAnimation( TEXT("Player"), vPosition, false );

    m_pController = PhysicsFn->CreateCharacterController( TEXT("CharacterController"), vPosition, Quaternion::Identity, Vector3::Null, Vector3::Null );
    m_pController->Enabled = true;
    m_pController->EnableForces( true );
    ((KinematicBody*)m_pBody)->AttachController( m_pController );

    // Debug ...
    SetDebugFlags( WORLD_DEBUGDRAW_GIZMOS |
                   WORLD_DEBUGDRAW_PROXIES | WORLD_DEBUGDRAW_CONTACTS | WORLD_DEBUGDRAW_CONTACTPOINTS |
                   WORLD_DEBUGDRAW_LINEARVELOCITY | WORLD_DEBUGDRAW_ANGULARVELOCITY );

    // Path tracking mode
    m_bPathTrackingMode = false;

    m_idPathTracking = INVALID_OFFSET;
    m_iLastPassedWaypoint = INVALID_OFFSET;
    m_vPathTarget = Vertex3::Null;

    m_pLinearPath = NULL;
    m_pLinearTravel = NULL;
    m_pAngularPath = NULL;
    m_pAngularTravel = NULL;

    m_pPathController = NULL;
}
CharEntity::~CharEntity()
{
    if ( m_bPathTrackingMode )
        EndPathTracking();

    ((KinematicBody*)m_pBody)->DetachMotionController();
    PhysicsFn->DestroyController( m_pController );
    m_pController = NULL;

    // Animation data
    RemoveAnimation();

    // Graphic data
    RemoveGraphics();
}

Void CharEntity::StartPathTracking( const Vertex3 & vTarget )
{
    Assert( !m_bPathTrackingMode );

    // Create path-finding
    WorldPathFinder * pPathFinder = WorldFn->GetPathFinder();
    Landscape * pLandscape = (Landscape*)( WorldFn->GetWorldRoot()->GetChild(TEXT("LandscapeArea")) );

    Vertex3 vPosition = GetPosition();
    WorldOutdoorRegion * pStartRegion = pLandscape->GetRegion( vPosition );
    WorldPathTriangleNode * pStart = pStartRegion->GetGridWaypoint( vPosition );

    WorldOutdoorRegion * pTargetRegion = pLandscape->GetRegion( vTarget );
    WorldPathTriangleNode * pTarget = pTargetRegion->GetGridWaypoint( vTarget );

    m_idPathTracking = pPathFinder->StartSearch( pStart, pTarget, PATHFIND_TYPE_SIMPLE ); // PATHFIND_TYPE_HIERARCHIC_2 );

    // Extract path
    const Array<WorldPathWaypoint*> * pPath = NULL;
    pPathFinder->GetPath( m_idPathTracking, &pPath );
    Assert( pPath != NULL );

    // Test for empty / trivial path
    if ( pPath->Count() < 1 ) {
        // Destroy path-finding
        pPathFinder->EndSearch( m_idPathTracking );
        m_idPathTracking = INVALID_OFFSET;

        // Done
        return;
    }

    // Create path-tracking
    m_iLastPassedWaypoint = 0;
    m_vPathTarget = vTarget;
    _PathTracking_Start( *pPath );

    // Done
    m_bPathTrackingMode = true;
}
Void CharEntity::EndPathTracking()
{
    Assert( m_bPathTrackingMode );

    // Pick path controller's state
    m_pController->SetPosition( m_pPathController->GetPosition() );
    m_pController->SetOrientation( m_pPathController->GetOrientation() );

    // Destroy path-tracking
    _PathTracking_End();
    m_iLastPassedWaypoint = INVALID_OFFSET;
    m_vPathTarget = Vertex3::Null;

    // Destroy path-finding
    if ( m_idPathTracking != INVALID_OFFSET ) {
        WorldPathFinder * pPathFinder = WorldFn->GetPathFinder();
        pPathFinder->EndSearch( m_idPathTracking );
        m_idPathTracking = INVALID_OFFSET;
    }

    // Done
    m_bPathTrackingMode = false;
}
Void CharEntity::UpdatePathTracking()
{
    if ( !m_bPathTrackingMode )
        return;

    // Check if target is reached
    if ( m_idPathTracking == INVALID_OFFSET ) {
        if ( m_pPathController->GetLocalTime() >= m_pPathController->MaxTime )
            EndPathTracking();
        return;
    }

    // Update path-finding
    WorldPathFinder * pPathFinder = WorldFn->GetPathFinder();

    if ( pPathFinder->IsFinished(m_idPathTracking) ) {
        pPathFinder->EndSearch( m_idPathTracking );
        m_idPathTracking = INVALID_OFFSET;
        return;
    }

    Bool bContinue = pPathFinder->UpdateSearch( m_idPathTracking, 100 );
    if ( !bContinue ) {
        if ( !(pPathFinder->IsPathFound(m_idPathTracking)) ) {
            EndPathTracking();
            return;
        }
    }

    // Extract path
    const Array<WorldPathWaypoint*> * pPath = NULL;
    pPathFinder->GetPath( m_idPathTracking, &pPath );
    Assert( pPath != NULL );

    // Update path-tracking
    _PathTracking_Update( *pPath );
}

/////////////////////////////////////////////////////////////////////////////////

Void CharEntity::_PathTracking_Start( const Array<WorldPathWaypoint*> & arrPath )
{
    Assert( m_pPathController == NULL );

    WorldPathFinder * pPathFinder = WorldFn->GetPathFinder();
    Bool bFinished = pPathFinder->IsFinished( m_idPathTracking );

    // Create path data
    EntityFn->SelectMemory( TEXT("Scratch") );

    Bool bBias = false;
    UInt iControlPointCount = arrPath.Count();
    if ( iControlPointCount == 1 ) {
        Assert( bFinished );
        ++iControlPointCount;
        bBias = true;
    }
    UInt iDegree = Min<UInt>( iControlPointCount - 1, 3 );

    Vertex3 * arrControlPoints = New Vertex3[iControlPointCount];
    Scalar * arrParameters = New Scalar[iControlPointCount];
    Vertex2 * arrTravelPoints = New Vertex2[iControlPointCount];

    arrControlPoints[0] = GetPosition();
    if ( bBias )
        arrControlPoints[1] = m_vPathTarget;
    else {
        for( UInt i = 1; i < iControlPointCount - 1; ++i )
            arrControlPoints[i] = arrPath[i]->GetPosition();
        if ( bFinished )
            arrControlPoints[iControlPointCount - 1] = m_vPathTarget;
        else
            arrControlPoints[iControlPointCount - 1] = arrPath[iControlPointCount - 1]->GetPosition();
    }
    m_pLinearPath = New BSplinePatch3( arrControlPoints, iControlPointCount, iDegree, false, false );

    m_pLinearPath->SubDivideByParameter( arrParameters, iControlPointCount );
    for( UInt i = 0; i < iControlPointCount; ++i ) {
        Vector3 vTangent = m_pLinearPath->Tangent( arrParameters[i] );
        arrControlPoints[i].X = 0.0f;
        arrControlPoints[i].Y = 0.0f;
        arrControlPoints[i].Z = MathFn->ArcCos( vTangent * Vector3::eI );
    }
    m_pAngularPath = New BSplinePatch3( arrControlPoints, iControlPointCount, iDegree, false, false );

    Scalar fLength = m_pLinearPath->Curve3::Length();
    Scalar fTime = ( fLength / m_pCharacter->GetMovementSpeed() );
    Scalar fLengthInterval = ( fLength / (Scalar)(iControlPointCount - 1) );
    Scalar fTimeInterval = ( fTime / (Scalar)(iControlPointCount - 1) );
    Scalar fX = 0.0f, fY = 0.0f;
    for( UInt i = 0; i < iControlPointCount; ++i ) {
        arrTravelPoints[i].X = fX;
        arrTravelPoints[i].Y = fY;
        fX += fLengthInterval;
        fY += fTimeInterval;
    }
    m_pLinearTravel = New BSplinePatch2( arrTravelPoints, iControlPointCount, iDegree, false, false );
    m_pAngularTravel = New BSplinePatch2( arrTravelPoints, iControlPointCount, iDegree, false, false );

    DeleteA( arrTravelPoints );
    arrTravelPoints = NULL;
    DeleteA( arrParameters );
    arrParameters = NULL;
    DeleteA( arrControlPoints );
    arrControlPoints = NULL;

    EntityFn->UnSelectMemory();
    
    // Disable character controller
    m_pController->SetStanding();

    KinematicBody * pBody = (KinematicBody*)m_pBody;
    m_pController->Enabled = false;
    pBody->DetachMotionController();

    // Enable path controller
    m_pPathController = PhysicsFn->CreatePathController( TEXT("PathController"), m_pLinearPath, m_pLinearTravel,
                                                         m_pAngularPath, m_pAngularTravel, EULER_ANGLES_ZYX );

    m_pPathController->Enabled = true;
    m_pPathController->MaxTime = fTime;
    pBody->AttachController( m_pPathController );
}
Void CharEntity::_PathTracking_End()
{
    Assert( m_pPathController != NULL );

    // Disable path controller
    KinematicBody * pBody = (KinematicBody*)m_pBody;
    m_pPathController->Enabled = false;
    pBody->DetachMotionController();

    PhysicsFn->DestroyController( m_pPathController );
    m_pPathController = NULL;

    // Enable character controller
    m_pController->Enabled = true;
    pBody->AttachController( m_pController );

    // Destroy path data
    EntityFn->SelectMemory( TEXT("Scratch") );

    Delete( m_pAngularTravel );
    m_pAngularTravel = NULL;
    Delete( m_pLinearTravel );
    m_pLinearTravel = NULL;
    Delete( m_pAngularPath );
    m_pAngularPath = NULL;
    Delete( m_pLinearPath );
    m_pLinearPath = NULL;

    EntityFn->UnSelectMemory();
}
Void CharEntity::_PathTracking_Update( const Array<WorldPathWaypoint*> & arrPath )
{
    // Destroy path tracking
    _PathTracking_End();

    // Find which waypoint we need to go to next
    Vertex3 vPosition = GetPosition();
    Scalar fSqrDistance, fPrevSqrDistance = SCALAR_INFINITE;
    //Scalar fPrevPrevSqrDistance;

    Assert( m_iLastPassedWaypoint != INVALID_OFFSET );
    UInt iNextWaypoint = m_iLastPassedWaypoint;
    while( iNextWaypoint < arrPath.Count() ) {        
        // Detect if the path stops getting closer from our position
        fSqrDistance = ( arrPath[iNextWaypoint]->GetPosition() - vPosition ).NormSqr();
        if ( fSqrDistance > fPrevSqrDistance ) {
            // Either one of those 2 cases : we allways choose i
            // -----*--X--|-----*----------*----
            //     i-1          i
            // -----*-----|--X--*----------*----  where i is iNextWaypoint
            //     i-2         i-1         i
            //if ( iNextWaypoint >= 2 ) {
            //    fPrevPrevSqrDistance = ( arrPath[iNextWaypoint-2]->GetPosition() - vPosition ).NormSqr();
            //    if ( fPrevPrevSqrDistance < fSqrDistance )
            //        --iNextWaypoint;
            //}
            break;
        }
        fPrevSqrDistance = fSqrDistance;

        ++iNextWaypoint;
    }

    // This is a safe assumption ...
    // We allways have the last call to this made way before
    // path tracking is finished, timing is obviously enforced here.
    Assert( iNextWaypoint < arrPath.Count() ); 
    m_iLastPassedWaypoint = ( iNextWaypoint - 1 );

    WorldPathFinder * pPathFinder = WorldFn->GetPathFinder();
    Bool bFinished = pPathFinder->IsFinished( m_idPathTracking );

    // Recreate path tracking
    EntityFn->SelectMemory( TEXT("Scratch") );

    UInt iControlPointCount = ( arrPath.Count() - iNextWaypoint + 1 );
    UInt iDegree = Min<UInt>( iControlPointCount - 1, 3 );

    Vertex3 * arrControlPoints = New Vertex3[iControlPointCount];
    Scalar * arrParameters = New Scalar[iControlPointCount];
    Vertex2 * arrTravelPoints = New Vertex2[iControlPointCount];

    arrControlPoints[0] = vPosition;
    for( UInt i = 1; i < iControlPointCount - 1; ++i )
        arrControlPoints[i] = arrPath[iNextWaypoint + (i-1)]->GetPosition();
    if ( bFinished )
        arrControlPoints[iControlPointCount - 1] = m_vPathTarget;
    else
        arrControlPoints[iControlPointCount - 1] = arrPath[iNextWaypoint + (iControlPointCount - 2)]->GetPosition();
    m_pLinearPath = New BSplinePatch3( arrControlPoints, iControlPointCount, iDegree, false, false );

    m_pLinearPath->SubDivideByParameter( arrParameters, iControlPointCount );
    for( UInt i = 0; i < iControlPointCount; ++i ) {
        Vector3 vTangent = m_pLinearPath->Tangent( arrParameters[i] );
        arrControlPoints[i].X = 0.0f;
        arrControlPoints[i].Y = 0.0f;
        arrControlPoints[i].Z = MathFn->ArcCos( vTangent * Vector3::eI );
    }
    m_pAngularPath = New BSplinePatch3( arrControlPoints, iControlPointCount, iDegree, false, false );

    Scalar fLength = m_pLinearPath->Curve3::Length();
    Scalar fTime = ( fLength / m_pCharacter->GetMovementSpeed() );
    Scalar fLengthInterval = ( fLength / (Scalar)(iControlPointCount - 1) );
    Scalar fTimeInterval = ( fTime / (Scalar)(iControlPointCount - 1) );
    Scalar fX = 0.0f, fY = 0.0f;
    for( UInt i = 0; i < iControlPointCount; ++i ) {
        arrTravelPoints[i].X = fX;
        arrTravelPoints[i].Y = fY;
        fX += fLengthInterval;
        fY += fTimeInterval;
    }
    m_pLinearTravel = New BSplinePatch2( arrTravelPoints, iControlPointCount, iDegree, false, false );
    m_pAngularTravel = New BSplinePatch2( arrTravelPoints, iControlPointCount, iDegree, false, false );

    DeleteA( arrTravelPoints );
    arrTravelPoints = NULL;
    DeleteA( arrParameters );
    arrParameters = NULL;
    DeleteA( arrControlPoints );
    arrControlPoints = NULL;

    EntityFn->UnSelectMemory();

    // Disable character controller
    m_pController->SetStanding();

    KinematicBody * pBody = (KinematicBody*)m_pBody;
    m_pController->Enabled = false;
    pBody->DetachMotionController();

    // Enable path controller
    m_pPathController = PhysicsFn->CreatePathController( TEXT("PathController"), m_pLinearPath, m_pLinearTravel,
                                                         m_pAngularPath, m_pAngularTravel, EULER_ANGLES_ZYX );

    m_pPathController->Enabled = true;
    m_pPathController->MaxTime = fTime;
    pBody->AttachController( m_pPathController );
}

/////////////////////////////////////////////////////////////////////////////////
// SelectionMarkerEntity implementation
const GChar * _Wrapper_GetName( const GChar * strCharName )
{
    static GChar strTmp[64];
    StringFn->Format( strTmp, TEXT("%s_SelectionMark"), strCharName );
    return strTmp;
}

SelectionMarkerEntity::SelectionMarkerEntity( BaseCharacter * pCharacter ):
    Entity( _Wrapper_GetName(pCharacter->GetName()) )
{
    m_pCharacter = pCharacter;

    // Graphic data
    Void * arrParameters[2] = {
        GraphicsFn->GetLight( TEXT("Light_Sun") ),
        GraphicsFn->GetMaterial( TEXT("Material_SelectionMarker") )
        //GraphicsFn->GetTexture( TEXT("Texture_SelectionMarker") )
    };
    SetupGraphics( TEXT("SelectionMarker"), EFFECTID_DEFAULT, arrParameters );

    // Animation data
    Vertex3 vPosition = m_pCharacter->GetEntity()->GetPosition();
    SetupKinematicAnimation( TEXT("SelectionMarker"), vPosition, true );

    m_pController = PhysicsFn->CreateManualController( TEXT("ManualController"), vPosition, Quaternion::Identity, Vector3::Null, Vector3::Null );
    m_pController->Enabled = true;
    ((KinematicBody*)m_pBody)->AttachController( m_pController );

    // Debug ...
}
SelectionMarkerEntity::~SelectionMarkerEntity()
{
    ((KinematicBody*)m_pBody)->DetachMotionController();
    PhysicsFn->DestroyController( m_pController );
    m_pController = NULL;

    // Animation data
    RemoveAnimation();

    // Graphic data
    RemoveGraphics();
}

Void SelectionMarkerEntity::TrackTarget( BaseCharacter * pTarget )
{
    Vertex3 vTarget = pTarget->GetEntity()->GetPosition();
    vTarget.Z += 1.5f;
    
    m_pController->SetPosition( vTarget );
}

