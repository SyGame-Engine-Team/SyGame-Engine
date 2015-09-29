/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/World/Landscape.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, World : Landscape
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_WORLD_LANDSCAPE_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_WORLD_LANDSCAPE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Graphics/GraphicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define RPGLANDSCAPE_MAX_REGIONS 128
#define RPGLANDSCAPE_REGION_SIZE 64.0f
#define RPGLANDSCAPE_REGION_HEIGHT_MIN  (-64.0f)
#define RPGLANDSCAPE_REGION_HEIGHT_MAX  (+256.0f)
#define RPGLANDSCAPE_REGION_SAMPLES 16

#define RPGLANDSCAPE_VIEW_DISTANCE 4

// Prototypes
class CharEntity;

class LandscapeRegion;
class Landscape;

/////////////////////////////////////////////////////////////////////////////////
// The LandscapeRegion class
class LandscapeRegion : public WorldOutdoorRegion
{
public:
    LandscapeRegion( Landscape * pParentLandscape, const Vertex2 & vOrigin, const Vector2 & vSize, Scalar fMinHeight, Scalar fMaxHeight,
			         UInt iSampleCountX, UInt iSampleCountY, const GChar * strName = NULL );
    virtual ~LandscapeRegion();

    // Update methods
	virtual Void OnRegionUpdate( UInt iRegionX, UInt iRegionY );

protected:
    Landscape * m_pParentLandscape;

    EffectColor * m_pLandscapeEffect;
    EffectColorInstance * m_pLandscapeEffectInstance;
    MeshHeightField * m_pLandscapeMesh;

    ShapeHeightField * m_pLandscapeShape;
    RigidBody * m_pLandscapeBody;

    WorldLeaf * m_pLandscape;
};

/////////////////////////////////////////////////////////////////////////////////
// The Landscape class
class Landscape : public WorldOutdoorArea
{
public:
    Landscape();
    virtual ~Landscape();

    Void SetEyeEntity( CharEntity * pEyeEntity );

protected:
    virtual WorldOutdoorRegion * _CreateRegion( const Vertex2 & vOrigin, const Vector2 & vSize, Scalar fMinHeight, Scalar fMaxHeight,
						                        UInt iSampleCountX, UInt iSampleCountY, const GChar * strName = NULL );
    virtual Void _OnEyeUpdate( Vertex2 * outEye ) const;

private:
    CharEntity * m_pEyeEntity;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Landscape.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_WORLD_LANDSCAPE_H

