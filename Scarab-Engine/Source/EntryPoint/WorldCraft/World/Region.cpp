/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Region.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : Region (16*16 Areas)
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
#include "Region.h"

#include "BlockWorld.h"

/////////////////////////////////////////////////////////////////////////////////
// Region implementation
Region::Region( VoidSpace * pParentVoidSpace, RegionLayer iLayer, const RegionCoords & vRegionCoords ):
    m_vRegionCoords( vRegionCoords )
{
    Assert( iLayer == REGION_LAYER_OUTERSPACE || iLayer >= REGION_LAYER_FARSPACE );

    // Region data
    m_iLayer = iLayer;
    m_iMoonID = INVALID_OFFSET;

    // Parent linkage
    m_pParentPlanet = NULL;
    m_pParentVoidSpace = pParentVoidSpace;

    // Create content
    for( UInt i = 0; i < REGION_AREACOUNT; ++i )
        m_arrAreas[i] = NULL;
}
Region::Region( Planet * pParentPlanet, RegionLayer iLayer, const RegionCoords & vRegionCoords ):
    m_vRegionCoords( vRegionCoords )
{
    Assert( iLayer <= REGION_LAYER_ATMOSPHERE );

    // Region data
    m_iLayer = iLayer;
    m_iMoonID = INVALID_OFFSET;

    // Parent linkage
    m_pParentPlanet = pParentPlanet;
    m_pParentVoidSpace = NULL;

    // Create content
    for( UInt i = 0; i < REGION_AREACOUNT; ++i )
        m_arrAreas[i] = NULL;
}
Region::Region( Planet * pParentPlanet, MoonID iMoonID, const RegionCoords & vRegionCoords ):
    m_vRegionCoords( vRegionCoords )
{
    Assert( iMoonID != INVALID_OFFSET );

    // Region data
    m_iLayer = REGION_LAYER_MOONS;
    m_iMoonID = iMoonID;

    // Parent linkage
    m_pParentPlanet = pParentPlanet;
    m_pParentVoidSpace = NULL;

    // Create content
    for( UInt i = 0; i < REGION_AREACOUNT; ++i )
        m_arrAreas[i] = NULL;
}
Region::~Region()
{
    // Destroy content
    AreaCoords vAreaCoords;
    for( vAreaCoords.Y = 0; vAreaCoords.Y < REGION_SIZE; ++(vAreaCoords.Y) ) {
        for( vAreaCoords.X = 0; vAreaCoords.X < REGION_SIZE; ++(vAreaCoords.X) )
            DestroyArea( vAreaCoords );
    }
}

Void Region::MakeUID( RegionUID * outUID ) const
{
    outUID->iRegionLayer = m_iLayer;
    outUID->iMoonID = m_iMoonID;
    outUID->vRegionCoords = m_vRegionCoords;

    switch( outUID->iRegionLayer ) {
        case REGION_LAYER_UNDERGROUND:
        case REGION_LAYER_SURFACE:
        case REGION_LAYER_ATMOSPHERE:
        case REGION_LAYER_MOONS:
                outUID->iOrbit = m_pParentPlanet->GetOrbit();
                outUID->iStarSystemID = m_pParentPlanet->GetParentStarSystem()->GetID();
                outUID->iGalaxyID = m_pParentPlanet->GetParentGalaxy()->GetID();
                outUID->iDimendionID = m_pParentPlanet->GetParentDimension()->GetID();
            break;
        case REGION_LAYER_OUTERSPACE: {
                Planet * pParentPlanet = m_pParentVoidSpace->GetParentPlanet();
                outUID->iOrbit = pParentPlanet->GetOrbit();
                outUID->iStarSystemID = pParentPlanet->GetParentStarSystem()->GetID();
                outUID->iGalaxyID = pParentPlanet->GetParentGalaxy()->GetID();
                outUID->iDimendionID = pParentPlanet->GetParentDimension()->GetID();
            } break;
        case REGION_LAYER_FARSPACE: {
                StarSystem * pParentStarSystem = m_pParentVoidSpace->GetParentStarSystem();
                outUID->iOrbit = INVALID_OFFSET;
                outUID->iStarSystemID = pParentStarSystem->GetID();
                outUID->iGalaxyID = pParentStarSystem->GetParentGalaxy()->GetID();
                outUID->iDimendionID = pParentStarSystem->GetParentDimension()->GetID();
            } break;
        case REGION_LAYER_DEEPSPACE: {
                Galaxy * pParentGalaxy = m_pParentVoidSpace->GetParentGalaxy();
                outUID->iOrbit = INVALID_OFFSET;
                outUID->iStarSystemID = INVALID_OFFSET;
                outUID->iGalaxyID = pParentGalaxy->GetID();
                outUID->iDimendionID = pParentGalaxy->GetParentDimension()->GetID();
            } break;
        case REGION_LAYER_OBLIVIONSPACE: {
                Dimension * pParentDimension = m_pParentVoidSpace->GetParentDimension();
                outUID->iOrbit = INVALID_OFFSET;
                outUID->iStarSystemID = INVALID_OFFSET;
                outUID->iGalaxyID = INVALID_OFFSET;
                outUID->iDimendionID = pParentDimension->GetID();
            } break;
        default: Assert(false); break;
    }
}

StarSystem * Region::GetParentStarSystem() const
{
    if ( m_pParentPlanet != NULL )
        return m_pParentPlanet->GetParentStarSystem();
    Assert( m_pParentVoidSpace != NULL );
    return m_pParentVoidSpace->GetParentStarSystem();
}
Galaxy * Region::GetParentGalaxy() const
{
    if ( m_pParentPlanet != NULL )
        return m_pParentPlanet->GetParentGalaxy();
    Assert( m_pParentVoidSpace != NULL );
    return m_pParentVoidSpace->GetParentGalaxy();
}
Dimension * Region::GetParentDimension() const
{
    if ( m_pParentPlanet != NULL )
        return m_pParentPlanet->GetParentDimension();
    Assert( m_pParentVoidSpace != NULL );
    return m_pParentVoidSpace->GetParentDimension();
}

Region * Region::GetAdjacentRegion( RegionAdjacency iAdjacentRegion ) const
{
    RegionCoords vAdjacentCoords = m_vRegionCoords;

    if ( iAdjacentRegion & REGION_ADJACENCY_WEST )
        vAdjacentCoords.X -= 1;
    else if ( iAdjacentRegion & REGION_ADJACENCY_EAST )
        vAdjacentCoords.X += 1;

    if ( iAdjacentRegion & REGION_ADJACENCY_SOUTH )
        vAdjacentCoords.Y -= 1;
    else if ( iAdjacentRegion & REGION_ADJACENCY_NORTH )
        vAdjacentCoords.Y += 1;

    RegionLayer iAdjacentLayer = m_iLayer;
    switch( m_iLayer ) {
        case REGION_LAYER_UNDERGROUND:
            if ( iAdjacentRegion & REGION_ADJACENCY_DOWN )
                return NULL; // Wanna go quantum ?
            else if ( iAdjacentRegion & REGION_ADJACENCY_UP )
                iAdjacentLayer = REGION_LAYER_SURFACE;
            return m_pParentPlanet->GetRegion( iAdjacentLayer, vAdjacentCoords );
        case REGION_LAYER_SURFACE:
            if ( iAdjacentRegion & REGION_ADJACENCY_DOWN )
                iAdjacentLayer = REGION_LAYER_UNDERGROUND;
            else if ( iAdjacentRegion & REGION_ADJACENCY_UP )
                iAdjacentLayer = REGION_LAYER_ATMOSPHERE;
            return m_pParentPlanet->GetRegion( iAdjacentLayer, vAdjacentCoords );
        case REGION_LAYER_ATMOSPHERE:
            if ( iAdjacentRegion & REGION_ADJACENCY_DOWN )
                iAdjacentLayer = REGION_LAYER_SURFACE;
            else if ( iAdjacentRegion & REGION_ADJACENCY_UP )
                return m_pParentPlanet->GetOuterSpace()->GetRegion( vAdjacentCoords );
            return m_pParentPlanet->GetRegion( iAdjacentLayer, vAdjacentCoords );
        case REGION_LAYER_OUTERSPACE:
            if ( iAdjacentRegion & REGION_ADJACENCY_DOWN )
                return m_pParentVoidSpace->GetParentPlanet()->GetRegion( REGION_LAYER_ATMOSPHERE, vAdjacentCoords );
            else if ( iAdjacentRegion & REGION_ADJACENCY_UP ) {
                MoonID iMoonID = ( RandFn->XorShift128() % m_pParentVoidSpace->GetParentPlanet()->GetMoonCount() ); // Land on a random moon !
                return m_pParentVoidSpace->GetParentPlanet()->GetMoonRegion( iMoonID, vAdjacentCoords );
            }
            return m_pParentVoidSpace->GetRegion( vAdjacentCoords );
        case REGION_LAYER_MOONS:
            if ( iAdjacentRegion & REGION_ADJACENCY_DOWN )
                return m_pParentPlanet->GetOuterSpace()->GetRegion( vAdjacentCoords );
            else if ( iAdjacentRegion & REGION_ADJACENCY_UP )
                return m_pParentPlanet->GetParentStarSystem()->GetFarSpace()->GetRegion( vAdjacentCoords );
            return m_pParentPlanet->GetMoonRegion( m_iMoonID, vAdjacentCoords );
        case REGION_LAYER_FARSPACE:
            if ( iAdjacentRegion & REGION_ADJACENCY_DOWN ) {
                UInt iOrbit = ( RandFn->XorShift128() % m_pParentVoidSpace->GetParentStarSystem()->GetOrbitCount() ); // Land on a random orbit !
                return m_pParentVoidSpace->GetParentStarSystem()->GetPlanet( iOrbit )->GetOuterSpace()->GetRegion( vAdjacentCoords );
            } else if ( iAdjacentRegion & REGION_ADJACENCY_UP )
                return m_pParentVoidSpace->GetParentGalaxy()->GetDeepSpace()->GetRegion( vAdjacentCoords );
            return m_pParentVoidSpace->GetRegion( vAdjacentCoords );
        case REGION_LAYER_DEEPSPACE:
            if ( iAdjacentRegion & REGION_ADJACENCY_DOWN ) {
                UInt iIndex = ( RandFn->XorShift128() % m_pParentVoidSpace->GetParentGalaxy()->GetStarSystemCount() ); // Land on a random starsystem !
                m_pParentVoidSpace->GetParentGalaxy()->EnumStarSystems();
                StarSystemID iStarSystemID = m_pParentVoidSpace->GetParentGalaxy()->EnumNextStarSystem()->GetID();
                while( iIndex > 0 ) {
                    --iIndex;
                    iStarSystemID = m_pParentVoidSpace->GetParentGalaxy()->EnumNextStarSystem()->GetID();
                }
                return m_pParentVoidSpace->GetParentGalaxy()->GetStarSystem( iStarSystemID )->GetFarSpace()->GetRegion( vAdjacentCoords );
            } else if ( iAdjacentRegion & REGION_ADJACENCY_UP )
                return m_pParentVoidSpace->GetParentDimension()->GetOblivionSpace()->GetRegion( vAdjacentCoords );
            return m_pParentVoidSpace->GetRegion( vAdjacentCoords );
        case REGION_LAYER_OBLIVIONSPACE:
            if ( iAdjacentRegion & REGION_ADJACENCY_DOWN ) {
                UInt iIndex = ( RandFn->XorShift128() % m_pParentVoidSpace->GetParentDimension()->GetGalaxyCount() ); // Land on a random galaxy !
                m_pParentVoidSpace->GetParentDimension()->EnumGalaxies();
                GalaxyID iGalaxyID = m_pParentVoidSpace->GetParentDimension()->EnumNextGalaxy()->GetID();
                while( iIndex > 0 ) {
                    --iIndex;
                    iGalaxyID = m_pParentVoidSpace->GetParentDimension()->EnumNextGalaxy()->GetID();
                }
                return m_pParentVoidSpace->GetParentDimension()->GetGalaxy( iGalaxyID )->GetDeepSpace()->GetRegion( vAdjacentCoords );
            } else if ( iAdjacentRegion & REGION_ADJACENCY_UP )
                return NULL; // => Point of no-return, WarpZone, True Void !
            return m_pParentVoidSpace->GetRegion( vAdjacentCoords );
        default: Assert(false); break;
    }

    return NULL;
}

Area * Region::CreateArea( const AreaCoords & vAreaCoords )
{
    Assert( vAreaCoords.X >= 0 && vAreaCoords.X < REGION_SIZE );
    Assert( vAreaCoords.Y >= 0 && vAreaCoords.Y < REGION_SIZE );
    UInt iIndex = ( vAreaCoords.Y * REGION_SIZE ) + vAreaCoords.X;
    
    if ( m_arrAreas[iIndex] != NULL )
        return m_arrAreas[iIndex];

    BlockWorldFn->SelectMemory( TEXT("Areas") );
    m_arrAreas[iIndex] = New Area( this, vAreaCoords );
    BlockWorldFn->UnSelectMemory();

    return m_arrAreas[iIndex];
}
Void Region::DestroyArea( const AreaCoords & vAreaCoords )
{
    Assert( vAreaCoords.X >= 0 && vAreaCoords.X < REGION_SIZE );
    Assert( vAreaCoords.Y >= 0 && vAreaCoords.Y < REGION_SIZE );
    UInt iIndex = ( vAreaCoords.Y * REGION_SIZE ) + vAreaCoords.X;

    if ( m_arrAreas[iIndex] != NULL ) {
        BlockWorldFn->SelectMemory( TEXT("Areas") );
        Delete( m_arrAreas[iIndex] );
        m_arrAreas[iIndex] = NULL;
        BlockWorldFn->UnSelectMemory();
    }
}

